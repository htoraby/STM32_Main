#include "adc.h"

//! Значения констант из документации на CPU
#define VREF     (2.5f)
#define VBAT_DIV (4)
#define CORE_TEMP_V25       (0.76f)
#define CORE_TEMP_AVG_SLOPE (0.0025)

ADC_HandleTypeDef hadc[adcMax];
DMA_HandleTypeDef hdma_adc2;

/*!
 \brief Метод получения значения ADC

 \param num - номер ADC
 \param channel - канал @ref IS_ADC_CHANNEL
 \param value - возвращаемое значение
 \return StatusType - ошибка или ок
*/
static StatusType getValueADC(adcNum num, uint32_t channel, uint32_t *value,
                              uint32_t samplTime = ADC_SAMPLETIME_15CYCLES);

void adcInit(adcNum num)
{
  ADC_MultiModeTypeDef multiMode;

  /**Configure the global features of the ADC (Clock, Resolution,
   * Data Alignment and number of conversion)
  */
  ADC_HandleTypeDef *adcX = &hadc[num];
  switch (num) {
    case adc1:
      adcX->Instance = ADC1;
      break;
    case adc2:
      adcX->Instance = ADC2;
      break;
    case adc3:
      adcX->Instance = ADC3;
      break;
    default:
      break;
  }
  //! 82/2 = 42 МГц
  adcX->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  //! 2^12-1 = 0xFFF
  adcX->Init.Resolution = ADC_RESOLUTION12b;
  adcX->Init.ScanConvMode = DISABLE;
  adcX->Init.ContinuousConvMode = DISABLE;
  adcX->Init.DiscontinuousConvMode = DISABLE;
  adcX->Init.NbrOfDiscConversion = 0;
  adcX->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  adcX->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  adcX->Init.NbrOfConversion = 1;
  adcX->Init.DMAContinuousRequests = DISABLE;
  adcX->Init.EOCSelection = EOC_SEQ_CONV;
  HAL_ADC_Init(adcX);

  /**Configure the ADC multi-mode
  */
  multiMode.Mode = ADC_MODE_INDEPENDENT;
  multiMode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
  HAL_ADCEx_MultiModeConfigChannel(adcX, &multiMode);
}

void adcStartDma()
{
//  HAL_ADC_Start_DMA(&hadc[adc2], (uint32_t*)&uhADCxConvertedValue, 2);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1) {
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();
  } else if(hadc->Instance == ADC2) {
    /* Peripheral clock enable */
    __ADC2_CLK_ENABLE();
//    __DMA2_CLK_ENABLE();

//    hdma_adc2.Instance = DMA2_Stream0;
//    hdma_adc2.Init.Channel = DMA_CHANNEL_0;
//    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_adc2.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_adc2.Init.Mode = DMA_CIRCULAR;
//    hdma_adc2.Init.Priority = DMA_PRIORITY_MEDIUM;
//    hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    HAL_DMA_Init(&hdma_adc2);

//    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc2);

//    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1) {
    __ADC1_CLK_DISABLE();
  } else if (hadc->Instance == ADC2) {
    __ADC2_CLK_DISABLE();
//    HAL_DMA_DeInit(hadc->DMA_Handle);
  }
}

StatusType getAnalogIn(uint32_t channel, uint32_t numSamples, uint32_t *value)
{
  int count = 0;
  uint32_t data = 0;
  int result = 0;
  int result_last = 0;

  for(uint32_t i = 0; i < numSamples; i++) {
    if(getValueADC(adc1, channel, &data) == StatusOk) {
      result = result_last + (((int)data - result_last)/(count + 1));
      result_last = result;
      count++;
    }
  }

  if(count > 0) {
    *value = result;
    return StatusOk;
  }
  else {
    return StatusError;
  }
}

StatusType getCoreTemperature(float *value)
{
  uint32_t data = 0;
  StatusType status = StatusError;

  //! ADC_SAMPLETIME_480CYCLES - (480+12)/42 = 11 мксек
  if(getValueADC(adc1, ADC_CHANNEL_TEMPSENSOR, &data, ADC_SAMPLETIME_480CYCLES) == StatusOk) {
    *value = ((((int)data * VREF)/0xFFF - CORE_TEMP_V25) / CORE_TEMP_AVG_SLOPE) + 25;
    status = StatusOk;
  }
  ADC->CCR &= ~ADC_CCR_TSVREFE;
  return status;
}

StatusType getCoreVbattery(float *value)
{
  uint32_t data = 0;
  StatusType status = StatusError;

  if(getValueADC(adc1, ADC_CHANNEL_VBAT, &data, ADC_SAMPLETIME_480CYCLES) == StatusOk) {
    *value =  (((int)data * VBAT_DIV * VREF)/0xFFF);
    status = StatusOk;
  }
  ADC->CCR &= ~ADC_CCR_VBATE;
  return status;
}

StatusType getCoreVref(float *value)
{
  uint32_t data = 0;
  StatusType status = StatusError;

  if(getValueADC(adc1, ADC_CHANNEL_VREFINT, &data, ADC_SAMPLETIME_480CYCLES) == StatusOk) {
    *value = (((int)data * VREF)/0xFFF);
    status = StatusOk;
  }
  ADC->CCR &= ~ADC_CCR_TSVREFE;
  return status;
}

static StatusType getValueADC(adcNum num, uint32_t channel, uint32_t *value,
                              uint32_t samplTime)
{
  ADC_ChannelConfTypeDef sConfig;
  StatusType status = StatusError;

  sConfig.Channel = channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = samplTime;
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&hadc[num], &sConfig);

  HAL_ADC_Start(&hadc[num]);
  if ((HAL_ADC_PollForConversion(&hadc[num], 20) == HAL_OK) &&
      (HAL_ADC_GetState(&hadc[num]) == HAL_ADC_STATE_EOC_REG)) {
    *value = HAL_ADC_GetValue(&hadc[num]);
    status = StatusOk;
  }
  HAL_ADC_Stop(&hadc[num]);

  return status;
}

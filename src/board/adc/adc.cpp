#include "adc.h"
#include <string.h>

//! Значения констант из документации на CPU
#define VREF     (2.5f)
#define VBAT_DIV (4)
#define CORE_TEMP_V25       (0.76f)
#define CORE_TEMP_AVG_SLOPE (0.0025)

ADC_HandleTypeDef hadc[adcMax];
DMA_HandleTypeDef hdma_adc2;
TIM_HandleTypeDef htim3;

#if USE_EXT_MEM
uint16_t adcData[ADC_CNANNELS_NUM*ADC_POINTS_NUM] __attribute__((section(".extmem")));
#else
uint16_t adcData[ADC_CNANNELS_NUM*ADC_POINTS_NUM];
#endif

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
  ADC_ChannelConfTypeDef sConfig;

  /**Configure the global features of the ADC (Clock, Resolution,
   * Data Alignment and number of conversion)
  */
  ADC_HandleTypeDef *adcX = &hadc[num];
  switch (num) {
    case adc1:
      adcX->Instance = ADC1;
      // 82/2 = 42 МГц
      adcX->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
      // 2^12-1 = 0xFFF
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
      break;
    case adc2:
      adcX->Instance = ADC2;
      // 82/2 = 42 МГц
      adcX->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
      // 2^12-1 = 0xFFF
      adcX->Init.Resolution = ADC_RESOLUTION12b;
      adcX->Init.ScanConvMode = ENABLE;
      adcX->Init.ContinuousConvMode = DISABLE;
      adcX->Init.DiscontinuousConvMode = DISABLE;
      adcX->Init.NbrOfDiscConversion = 0;
      adcX->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
      adcX->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
      adcX->Init.DataAlign = ADC_DATAALIGN_RIGHT;
      adcX->Init.NbrOfConversion = ADC_CNANNELS_NUM;
      adcX->Init.DMAContinuousRequests = ENABLE;
      adcX->Init.EOCSelection = EOC_SEQ_CONV;
      break;
    case adc3:
      adcX->Instance = ADC3;
      break;
    default:
      break;
  }
  HAL_ADC_Init(adcX);

  if (num == adc2) {
    // Настройка каналов
    // Ua
    sConfig.Channel = ADC_CHANNEL_13;
    sConfig.Rank = 1;
    // ADC_SAMPLETIME_112CYCLES - (112+12)/42 = 3 мксек
    sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
    HAL_ADC_ConfigChannel(adcX, &sConfig);

    // Ub
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = 2;
    HAL_ADC_ConfigChannel(adcX, &sConfig);

    // Uc
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank = 3;
    HAL_ADC_ConfigChannel(adcX, &sConfig);


    // Настройка таймера
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = ((SystemCoreClock /2) / 100000) - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = ADC_TIM_PERIOD - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&htim3);

    TIM_MasterConfigTypeDef sMasterConfig;
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    adcStartDma();
  }

  /**Configure the ADC multi-mode
  */
  multiMode.Mode = ADC_MODE_INDEPENDENT;
  multiMode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
  multiMode.DMAAccessMode = DISABLE;
  HAL_ADCEx_MultiModeConfigChannel(adcX, &multiMode);
}
static uint32_t time = 0;
void adcStartDma()
{
  memset(&adcData, 0, sizeof(adcData));
  HAL_ADC_Start_DMA(&hadc[adc2], (uint32_t*)&adcData, ADC_CNANNELS_NUM*ADC_POINTS_NUM);
  HAL_TIM_Base_Start(&htim3);

  time = HAL_GetTick();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hadc->Instance == ADC1) {
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();
  } else if(hadc->Instance == ADC2) {
    /* Peripheral clock enable */
    __ADC2_CLK_ENABLE();
    __TIM3_CLK_ENABLE();

    /**ADC2 GPIO Configuration
    PC1     ------> ADC2_IN11 (Uc)
    PC2     ------> ADC2_IN12 (Ub)
    PC3     ------> ADC2_IN13 (Ua)
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_adc2.Instance = DMA2_Stream2;
    hdma_adc2.Init.Channel = DMA_CHANNEL_1;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adc2);

    __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc2);

    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, ADC_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    HAL_NVIC_SetPriority(TIM3_IRQn, ADC_IRQ_PREPRIO, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1) {
    __ADC1_CLK_DISABLE();
  } else if (hadc->Instance == ADC2) {
    __ADC2_CLK_DISABLE();
    __TIM3_CLK_DISABLE();
    HAL_DMA_DeInit(&hdma_adc2);
    HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
  }
}

StatusType getAnalogIn(uint32_t channel, uint32_t numSamples, uint32_t *value)
{
  static uint32_t data = 0;
  int count = 0;
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
  static uint32_t data = 0;
  int count = 0;
  int result = 0;
  int resultLast = 0;

  // ADC_SAMPLETIME_480CYCLES - (480+12)/42 = 11 мксек
  for(uint32_t i = 0; i < 3; i++) {
    if(getValueADC(adc1, ADC_CHANNEL_TEMPSENSOR, &data, ADC_SAMPLETIME_480CYCLES) == StatusOk) {
      result = resultLast + (((int)data - resultLast)/(count + 1));
      resultLast = result;
      count++;
    }
    HAL_Delay(1);
  }
  ADC->CCR &= ~ADC_CCR_TSVREFE;
  if(count > 0) {
    *value = (((result * VREF)/0xFFF - CORE_TEMP_V25) / CORE_TEMP_AVG_SLOPE) + 25;
    return StatusOk;
  }
  else {
    return StatusError;
  }
}

StatusType getCoreVbattery(float *value)
{
  static uint32_t data = 0;
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
  static uint32_t data = 0;
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

/*!
 \brief Завершение измерения через время ARCHIVE_TIME

 \param ADC_HandleTypeDef - указатель на ADC
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle)
{
  (void)adcHandle;

  // Для тестирования времени измерения
  static int t = 0;
  time = HAL_GetTick() - time;
  if (++t != 1)
    asm("nop");
  time = HAL_GetTick();
}

void copyAdcData(int16_t *data)
{
  int allPoints = ADC_CNANNELS_NUM*ADC_POINTS_NUM;
  int numDataTr = hadc[adc2].DMA_Handle->Instance->NDTR;
  memcpy(&data[numDataTr], &adcData[0], (allPoints - numDataTr)*2);
  memcpy(&data[0], &adcData[allPoints - numDataTr], (numDataTr)*2);
}

void getAdcDataInPeriod(uint16_t *data)
{
  int points = ADC_CNANNELS_NUM*HC_POINTS_NUM*2;
  int allPoints = ADC_CNANNELS_NUM*ADC_POINTS_NUM;
  int numDataTr = hadc[adc2].DMA_Handle->Instance->NDTR;
  int idx = numDataTr - points;
  if (idx >= 0) {
    memcpy(&data[0], &adcData[idx], (points)*2);
  } else {
    idx = 0 - idx;
    memcpy(&data[idx], &adcData[0], (numDataTr)*2);
    memcpy(&data[0], &adcData[allPoints - idx], (idx)*2);
  }
}

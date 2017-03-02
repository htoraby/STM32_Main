#include "adc_ext.h"
#include "gpio.h"
#include <string.h>

//! Выводов микросхемы 74HC595
#define HC595MR_PIN  GPIOH, GPIO_PIN_13
#define HC595DS_PIN  GPIOA, GPIO_PIN_8
#define HC595SH_PIN  GPIOH, GPIO_PIN_14
#define HC595ST_PIN  GPIOH, GPIO_PIN_15

#define SPI2_NSS_PIN  GPIOI, GPIO_PIN_0

#define ADC_EXT_DIV 4
#define ADC_EXT_MEASURE_NUM 10
#define ADC_EXT_INPUTS 8


SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi2_rx;

int channel = 0;
int measureCount = 0;
int measureCountAI6 = 0;

AnalogInputDef analogInExt[AnalogInputMax];
uint16_t adcDataAI6[ADC_EXT_INPUTS_6_POINTS];

static uint8_t bufferTx[3];
static uint8_t bufferRx[3];
osSemaphoreId semaphoreAdcExt;

static void adcExtThread(void *argument);

static void hc595delay()
{
  for (int i = 0; i < 1000; i++)
    asm("nop");
}
void testAnalogInExt();

static void updateModeAnalogIn()
{
  for (int i = AnalogInputMax - 1; i >= 0; i--) {
    if (analogInExt[i].mode == AnalogInModeVoltage)
      clrPinOut(HC595DS_PIN);
    else
      setPinOut(HC595DS_PIN);
    hc595delay();
    setPinOut(HC595SH_PIN);
    hc595delay();
    clrPinOut(HC595SH_PIN);
  }
  hc595delay();
  setPinOut(HC595ST_PIN);
  hc595delay();
  clrPinOut(HC595ST_PIN);
}

void adcExtInit()
{
  initPinOut(HC595MR_PIN, PinSet);
  initPinOut(HC595DS_PIN);
  initPinOut(HC595SH_PIN);
  initPinOut(HC595ST_PIN);

  // PI0     ------> SPI2_NSS
  initPinOut(SPI2_NSS_PIN, PinSet);

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0xFFFF;
  HAL_SPI_Init(&hspi2);

  /* Peripheral DMA init*/

  hdma_spi2_tx.Instance = DMA1_Stream4;
  hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
  hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi2_tx.Init.Mode = DMA_NORMAL;
  hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi2_tx);

  __HAL_LINKDMA(&hspi2,hdmatx,hdma_spi2_tx);

  hdma_spi2_rx.Instance = DMA1_Stream3;
  hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
  hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi2_rx.Init.Mode = DMA_NORMAL;
  hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi2_rx);

  __HAL_LINKDMA(&hspi2,hdmarx,hdma_spi2_rx);

  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, ADC_IRQ_PREPRIO, 1);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, ADC_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

  updateModeAnalogIn();

  semaphoreAdcExt = osSemaphoreCreate(NULL, 1);
  osThreadDef(AdcExt, adcExtThread, osPriorityLow, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(AdcExt), NULL);
}

void setModeAnalogInExt(uint8_t num, uint8_t mode)
{
  analogInExt[num].mode = mode;
  updateModeAnalogIn();
}

void adcExtTxRxCpltCallback()
{
  setPinOut(SPI2_NSS_PIN);
  uint16_t data = bufferRx[2];
  data += (bufferRx[1] & 0x0F) << 8;
  analogInExt[channel].valueTmp += data;
  channel++;
  osSemaphoreRelease(semaphoreAdcExt);
}

void adcExtThread(void *argument)
{
  (void)argument;
  uint16_t data;

  while (1) {
    osSemaphoreWait(semaphoreAdcExt, osWaitForever);
    osDelay(1);

    if (channel >= 8) {
      channel = 0;
      if (++measureCount >= ADC_EXT_MEASURE_NUM) {
        measureCount = 0;
        for (int i = 0; i < AnalogInputMax; ++i) {
          analogInExt[i].value = analogInExt[i].valueTmp/ADC_EXT_MEASURE_NUM;
          analogInExt[i].valueTmp = 0;
        }

        if (measureCountAI6 < ADC_EXT_INPUTS_6_POINTS) {
          measureCountAI6++;
        }
        else {
          measureCountAI6 = 0;
        }
        adcDataAI6[measureCountAI6] = analogInExt[AI6].value;
      }
    }

    clrPinOut(SPI2_NSS_PIN);

    data = (channel << 6) | 0x0600;
    bufferTx[0] = data >> 8;
    bufferTx[1] = data & 0xFF;
    bufferTx[2] = 0x00;

    if (HAL_SPI_TransmitReceive_DMA(&hspi2, bufferTx, bufferRx, 3) != HAL_OK)
      asm("nop");
  }
}

float getValueAnalogInExt(uint8_t num)
{
  // Преобразование измеренного значения
  float value = analogInExt[num].value;
  if (analogInExt[num].mode == AnalogInModeVoltage) {
    // V = value * 4.0 * 2.5 / 0xFFF
    value = (value * 10) / 0xFFF;
  }
  else {
    // mA = value * 4 * 2.5 * 1000 / 0xFFF / 493Om
    value = (value * 10000) / 0xFFF / 493;
  }

  return value;
}


void copyAdcDataAI6(uint16_t *data)
{
  memcpy(&data[measureCountAI6], &adcDataAI6[0], (ADC_EXT_INPUTS_6_POINTS - measureCountAI6)*2);
  memcpy(&data[0], &adcDataAI6[ADC_EXT_INPUTS_6_POINTS - measureCountAI6], (measureCountAI6)*2);
}

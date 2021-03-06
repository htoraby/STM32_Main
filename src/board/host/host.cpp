#include "host.h"
#include "gpio.h"
#include "string.h"
#include "user_main.h"
#include "user_debug.h"

//! Время таймаута запроса от мастера для сброса SPI (n*10 мс)
#define TIMEOUT_RX 1500

SPI_HandleTypeDef hspi4;

static osSemaphoreId hostSemaphoreId;
static uint8_t txBuffer[2*HOST_BUF_SIZE];
static uint8_t rxBuffer[HOST_BUF_SIZE];

StatHostDef statHost;
static int rxCount = 0;
static int rxTimeout = TIMEOUT_RX;
static int rxActive = 0;
static int unstuff = 0;
static uint8_t isConnect = 0;

static void hostTimer(const void *argument);

void hostInit()
{
  memset(&statHost, 0, sizeof(statHost));

  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_SLAVE;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 0xFFFF;
  HAL_SPI_Init(&hspi4);

  hostSemaphoreId = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(hostSemaphoreId, 0);

  osTimerDef_t timerDef = {hostTimer};
  osTimerId timerId = osTimerCreate(&timerDef, osTimerPeriodic, NULL);
  osTimerStart(timerId, 10);
}

void hostReset()
{
  if (isConnect) {
    isConnect = 0;
    logDebug.add(CriticalMsg, "Host reset");
  }

  HAL_SPI_DeInit(&hspi4);
  HAL_SPI_Init(&hspi4);
  hostEnable();
}

osSemaphoreId getHostSemaphore()
{
  return hostSemaphoreId;
}

void hostEnable()
{
  osDelay(5);
  __HAL_SPI_ENABLE_IT(&hspi4, (SPI_IT_RXNE | SPI_IT_ERR));
  if((hspi4.Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
    __HAL_SPI_ENABLE(&hspi4);
}

static void hostTimer(const void * argument)
{
  (void)argument;

  resetIrqError();

  if (rxActive) {
    if (rxTimeout) {
      rxTimeout--;
    }
    else {
      statHost.timeoutError++;
      rxActive = 0;
    }
  } else {
    if (rxTimeout) {
      rxTimeout--;
    } else {
      rxTimeout = TIMEOUT_RX;
      hostReset();
    }
  }
}

void hostRxIRQHandler(void)
{
  static uint32_t tmp1 = 0, tmp2 = 0, tmp3 = 0;
  static uint8_t data = 0;

  calcIrqError(0, 0);

  tmp1 = __HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_RXNE);
  tmp2 = __HAL_SPI_GET_IT_SOURCE(&hspi4, SPI_IT_RXNE);
  tmp3 = __HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_OVR);

  if((tmp1 != RESET) && (tmp2 != RESET) && (tmp3 == RESET)) {
    data = hspi4.Instance->DR;
    if (data == 0x7E) {
      // Конец пакета
      if (rxActive) {
        isConnect = 1;
        rxActive = 0;
        osSemaphoreRelease(hostSemaphoreId);
      }
      // Начало пакета
      else {
        rxTimeout = TIMEOUT_RX;
        rxCount = 0;
        rxActive = 1;
      }
    }
    // Прием данных пакета
    else {
      if (rxActive) {
        rxTimeout = TIMEOUT_RX;
        if (data == 0x7D) {
          unstuff = 1;
          return;
        }
        // Выкидываем байтстаффинг
        if (unstuff) {
          data |= (1 << 5);
          unstuff = 0;
        }
        if (rxCount < HOST_BUF_SIZE) {
          rxBuffer[rxCount++] = data;
        } else {
          rxActive = 0;
          osSemaphoreRelease(hostSemaphoreId);
        }
      }
    }
    return;
  }

  if(__HAL_SPI_GET_IT_SOURCE(&hspi4, SPI_IT_ERR) != RESET) {
    if(__HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_CRCERR) != RESET)
      __HAL_SPI_CLEAR_CRCERRFLAG(&hspi4);

    if(__HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_MODF) != RESET)
      __HAL_SPI_CLEAR_MODFFLAG(&hspi4);

    if(__HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_OVR) != RESET) {
      __HAL_SPI_CLEAR_OVRFLAG(&hspi4);
      statHost.ovrFlagError++;
    }

    if(__HAL_SPI_GET_FLAG(&hspi4, SPI_FLAG_FRE) != RESET)
      __HAL_SPI_CLEAR_FREFLAG(&hspi4);
  }
}

int hostReadData(uint8_t *data)
{
  const int count = rxCount;
  memcpy(data, rxBuffer, count);

  return count;
}

/*!
 \brief Ожидание установки флага статусного регистра SPI

 \param flag - флаг
 \param status - необходимое состояние флага
 \param timeout - таймаут
 \return HAL_StatusTypeDef
*/
static HAL_StatusTypeDef spiWaitOnFlagUntilTimeout(uint32_t flag, FlagStatus status,
                                                   uint32_t timeout)
{
  uint32_t tickstart = HAL_GetTick();

  while(__HAL_SPI_GET_FLAG(&hspi4, flag) == status) {
    if(timeout != HAL_MAX_DELAY) {
      if((timeout == 0) || ((HAL_GetTick() - tickstart) > timeout))
        return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

void hostSpiDmaTransmitCplt(DMA_HandleTypeDef *hdma)
{
  StatusType status = StatusOk;
  SPI_HandleTypeDef* hspi = (SPI_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;

  if((hdma->Instance->CR & DMA_SxCR_CIRC) == 0) {
    if(spiWaitOnFlagUntilTimeout(SPI_FLAG_TXE, RESET, 1000) != HAL_OK)
      status = StatusError;
    hspi->Instance->CR2 &= (uint32_t)(~SPI_CR2_TXDMAEN);

    if(spiWaitOnFlagUntilTimeout(SPI_FLAG_BSY, SET, 1000) != HAL_OK)
      status = StatusError;
  } else {
    asm("nop");
  }

  if(hspi->Init.Direction == SPI_DIRECTION_2LINES)
    __HAL_SPI_CLEAR_OVRFLAG(hspi);

  if(status != StatusOk)
    statHost.txError++;
  else
    statHost.txGood++;
}

StatusType hostWriteData(uint8_t *data, int size, uint32_t)
{
  int idx = 0;
  txBuffer[idx++] = 0x00;
  txBuffer[idx++] = 0x7E;

  for (int i = 0; i < size; i++) {
    if ((data[i] == 0x7E) || (data[i] == 0x7D)) {
      txBuffer[idx++] = 0x7D;
      txBuffer[idx++] = data[i] ^ (1<<5);
    } else {
      txBuffer[idx++] = data[i];
    }
  }

  txBuffer[idx++] = 0x7E;
  txBuffer[idx++] = 0x00;

  HAL_DMA_Start_IT(hspi4.hdmatx, (uint32_t)txBuffer, (uint32_t)&hspi4.Instance->DR, idx);
  hspi4.Instance->CR2 |= SPI_CR2_TXDMAEN;

  return StatusOk;
}

#include "uart.h"
#include "gpio.h"

#include "string.h"

UART_Def uarts[uartMax];

static void uart_setRts(uartNum num, GPIO_PinState value);

int uart_init(uartNum num, uint32_t baudRate, uint32_t parity, uint32_t stopBits)
{
  HAL_StatusTypeDef status = HAL_OK;
  UART_HandleTypeDef *uartX = &uarts[num].uart;
  switch (num) {
    case uart1:
      uartX->Instance = USART1;
      break;
    case uart2:
      uartX->Instance = USART2;
      break;
    case uart3:
      uartX->Instance = USART3;
      break;
    case uart4:
      uartX->Instance = UART4;
      break;
    case uart6:
      uartX->Instance = USART6;
      break;
    case uart7:
      uartX->Instance = UART7;
      break;
    default:
      return -1;
  }

  uartX->Init.BaudRate = baudRate;
  uartX->Init.WordLength = UART_WORDLENGTH_8B;
  uartX->Init.StopBits = stopBits;
  uartX->Init.Parity = parity;
  uartX->Init.Mode = UART_MODE_TX_RX;
  uartX->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartX->Init.OverSampling = UART_OVERSAMPLING_16;

  status = HAL_UART_Init(uartX);
  if (status == HAL_OK) {
    //! Разрешение прерываний
    status = HAL_UART_Receive_IT(uartX, uarts[num].rxBuffer, UART_BUF_SIZE);
  }
  return status;
}

HAL_StatusTypeDef uart_close(uartNum num)
{
  UART_HandleTypeDef *uartX = &uarts[num].uart;
  /* Process Locked */
  __HAL_LOCK(uartX);

  while (HAL_IS_BIT_SET(uartX->Instance->SR, UART_FLAG_RXNE)) {
  }
  __HAL_UART_DISABLE_IT(uartX, UART_IT_RXNE);

  /* Check if a transmit process is ongoing or not */
  if (uartX->State == HAL_UART_STATE_BUSY_TX_RX) {
    uartX->State = HAL_UART_STATE_BUSY_TX;
  }
  else {
    /* Disable the UART Parity Error Interrupt */
    __HAL_UART_DISABLE_IT(uartX, UART_IT_PE);

    /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_DISABLE_IT(uartX, UART_IT_ERR);

    uartX->State = HAL_UART_STATE_READY;
  }
  /* Call the Process Unlocked before calling the Rx callback API to give the possibility to
           start again the reception under the Rx callback API */
  __HAL_UNLOCK(uartX);

  return HAL_UART_DeInit(uartX);
}

void uart_setSemaphoreId(uartNum num, osSemaphoreId semaphoreId)
{
  uarts[num].semaphoreId = semaphoreId;
}

inline uartNum uart_getNum(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2) {
    return uart2;
  } else if (huart->Instance == USART3) {
    return uart3;
  } else if (huart->Instance == UART4) {
    return uart4;
  } else if (huart->Instance == USART6) {
    return uart6;
  } else if (huart->Instance == UART7) {
    return uart7;
  } else {
    return uart1;
  }
}

int uart_getRxCount(uartNum num)
{
  return uarts[num].uart.RxXferCount;
}

int uart_readData(uartNum num, uint8_t *data)
{
  UART_HandleTypeDef *uartX = &uarts[num].uart;

  while (HAL_IS_BIT_SET(uartX->Instance->SR, UART_FLAG_RXNE)) {
  }
  while (uartX->Lock == HAL_LOCKED) {
  }
  uartX->Lock = HAL_LOCKED;

  const int count = uartX->RxXferCount;
  memcpy(data, uarts[num].rxBuffer, count);

  uartX->pRxBuffPtr = uarts[num].rxBuffer;
  uartX->RxXferCount = 0;

  uartX->Lock = HAL_UNLOCKED;

  return count;
}

HAL_StatusTypeDef uart_writeData(uartNum num, uint8_t *data, int count,
                                 uint32_t timeout)
{
  memcpy(uarts[num].txBuffer, data, count);

  uart_setRts(num, GPIO_PIN_SET);
  HAL_StatusTypeDef status = HAL_UART_Transmit(&uarts[num].uart,
                                               uarts[num].txBuffer,
                                               count, timeout);
  uart_setRts(num, GPIO_PIN_RESET);
  return status;
}

/*!
 \brief Получение байта данных из прерывания и освобождение семафора

 \param huart - Указатель на порт UART
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  osSemaphoreId semaphoreId = uarts[uart_getNum(huart)].semaphoreId;
  if (semaphoreId == NULL)
    return;
  osSemaphoreRelease(semaphoreId);
}

/*!
 \brief Отправка пакета с данными из прерывания

 \param huart - Указатель на порт UART
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  (void)huart;
}

/*!
 \brief Управление выводом RTS порта UART

 \param num - номер порта (тип uartNum)
 \param value - значение на выходе 0 или 1
*/
static void uart_setRts(uartNum num, GPIO_PinState value)
{
  if (num == uart2) {
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, value);
  } else if (num == uart3) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, value);
  } else if (num == uart4) {
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, value);
  } else if (num == uart7) {
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, value);
  }
}

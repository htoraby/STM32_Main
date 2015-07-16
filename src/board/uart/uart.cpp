#include "uart.h"
#include "gpio.h"

#include "string.h"

UART_Def uarts[uartMax];

StatusType uartInit(uartNum num, uint32_t baudRate, uint32_t parity, uint32_t stopBits)
{
  StatusType status = StatusError;

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
      return status;
  }

  uartX->Init.BaudRate = baudRate;
  if (parity == UART_PARITY_NONE)
    uartX->Init.WordLength = UART_WORDLENGTH_8B;
  else
    uartX->Init.WordLength = UART_WORDLENGTH_9B;
  uartX->Init.StopBits = stopBits;
  uartX->Init.Parity = parity;
  uartX->Init.Mode = UART_MODE_TX_RX;
  uartX->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartX->Init.OverSampling = UART_OVERSAMPLING_16;

  uarts[num].semaphoreId = NULL;
  if (HAL_UART_Init(uartX) == HAL_OK) {
    // Разрешение прерываний
    if (HAL_UART_Receive_IT(uartX, uarts[num].rxBuffer, UART_BUF_SIZE) == HAL_OK) {
      uarts[num].semaphoreId = osSemaphoreCreate(NULL, 1);
      osSemaphoreWait(uarts[num].semaphoreId, 0);
      status = StatusOk;
    }
  }

  return status;
}

HAL_StatusTypeDef uartClose(uartNum num)
{
  if (uarts[num].semaphoreId)
    osSemaphoreDelete(uarts[num].semaphoreId);

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

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance == UART4) {
    /* Peripheral clock enable */
    __UART4_CLK_ENABLE();

    /**UART4 GPIO Configuration
    PA0/WKUP     ------> UART4_TX
    PA1          ------> UART4_RX
    PH2          ------> USART4_RTS (user pin)
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_RESET);

    HAL_NVIC_SetPriority(UART4_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  }
  else if(huart->Instance == UART7) {
    /* Peripheral clock enable */
    __UART7_CLK_ENABLE();

    /**UART7 GPIO Configuration
    PF6     ------> UART7_RX
    PF7     ------> UART7_TX
    PI11    ------> USART7_RTS (user pin)
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET);

    HAL_NVIC_SetPriority(UART7_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(UART7_IRQn);
  }
  else if(huart->Instance == USART1) {
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART1_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
  else if(huart->Instance == USART2) {
    /* Peripheral clock enable */
    __USART2_CLK_ENABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    PH5     ------> USART2_RTS (user pin)
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_RESET);

    HAL_NVIC_SetPriority(USART2_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
  else if(huart->Instance == USART3) {
    /* Peripheral clock enable */
    __USART3_CLK_ENABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    PB14     ------> USART3_RTS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11/*|GPIO_PIN_14*/;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PB14   ------> USART3_RTS
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

    HAL_NVIC_SetPriority(USART3_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  }
  else if(huart->Instance == USART6) {
    /* Peripheral clock enable */
    __USART6_CLK_ENABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART6_IRQn, UART_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance == UART4) {
    __UART4_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_2);
  }
  else if(huart->Instance == UART7) {
    __UART7_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_11);
  }
  else if(huart->Instance == USART1) {
    __USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
  }
  else if(huart->Instance == USART2) {
    __USART2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_5);
  }
  else if(huart->Instance == USART3) {
    __USART3_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_14);
  }
  else if(huart->Instance == USART6) {
    __USART6_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);
  }
}

osSemaphoreId uartGetSemaphoreId(uartNum num)
{
  return uarts[num].semaphoreId;
}

uint8_t uartGetNum(UART_HandleTypeDef *huart)
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

int uartGetRxCount(uartNum num)
{
  return uarts[num].uart.RxXferCount;
}

int uartReadData(uartNum num, uint8_t *data)
{
  UART_HandleTypeDef *uartX = &uarts[num].uart;

  const int count = uartX->RxXferCount;
  memcpy(data, uarts[num].rxBuffer, count);

  uartX->pRxBuffPtr = uarts[num].rxBuffer;
  uartX->RxXferCount = 0;

  return count;
}

uint8_t uartWriteData(uartNum num, uint8_t *data, int count,
                         uint32_t timeout)
{
  uint8_t status = 1;
  memcpy(uarts[num].txBuffer, data, count);

  uartSetRts(num, GPIO_PIN_SET);
  if (HAL_UART_Transmit(&uarts[num].uart, uarts[num].txBuffer, count, timeout) == HAL_OK)
    status = 0;
  uartSetRts(num, GPIO_PIN_RESET);

  return status;
}

/*!
 \brief Получение байта данных из прерывания и освобождение семафора

 \param huart - Указатель на порт UART
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  osSemaphoreRelease(uarts[uartGetNum(huart)].semaphoreId);
}

/*!
 \brief Отправка пакета с данными из прерывания

 \param huart - Указатель на порт UART
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  (void)huart;
}

void uartSetRts(uint8_t num, GPIO_PinState value)
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

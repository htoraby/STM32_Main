/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"
#include "uart.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation -----------------------------*/

UART_HandleTypeDef *uart;

static HAL_StatusTypeDef waitOnFlagUntilTimeout(UART_HandleTypeDef *huart,
                                                uint32_t Flag, FlagStatus Status,
                                                uint32_t Timeout)
{
  uint32_t tickstart = HAL_GetTick();
  if(Status == RESET) {
    while(__HAL_UART_GET_FLAG(huart, Flag) == RESET) {
      if(Timeout != HAL_MAX_DELAY) {
        if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout)) {
          return HAL_TIMEOUT;
        }
      }
    }
  }
  else {
    while(__HAL_UART_GET_FLAG(huart, Flag) != RESET) {
      if(Timeout != HAL_MAX_DELAY) {
        if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout)) {
          return HAL_TIMEOUT;
        }
      }
    }
  }
  return HAL_OK;
}

/* If xRXEnable enable serial receive interrupts. If xTxENable enable
 * transmitter empty interrupts.
 */
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
  if(xRxEnable) {
    uartSetRts(uartGetNum(uart), GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
  }
  else {
    __HAL_UART_DISABLE_IT(uart, UART_IT_RXNE);
    uartSetRts(uartGetNum(uart), GPIO_PIN_SET);
  }

  if (xTxEnable) {
    __HAL_UART_ENABLE_IT(uart, UART_IT_TXE);
  }
  else {
    __HAL_UART_DISABLE_IT(uart, UART_IT_TXE);

  }
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
                  eMBParity eParity)
{
  UNUSED(ucDataBits);

  uart = &uarts[ucPORT].uart;
  switch (ucPORT) {
  case uart1:
    uart->Instance = USART1;
    break;
  case uart2:
    uart->Instance = USART2;
    break;
  case uart3:
    uart->Instance = USART3;
    break;
  case uart4:
    uart->Instance = UART4;
    break;
  case uart6:
    uart->Instance = USART6;
    break;
  case uart7:
    uart->Instance = UART7;
    break;
  default:
    return FALSE;
  }

  uart->Init.BaudRate = ulBaudRate;

  switch (eParity) {
  case MB_PAR_ODD:
    uart->Init.WordLength = UART_WORDLENGTH_9B;
    uart->Init.Parity = UART_PARITY_ODD;
    break;
  case MB_PAR_EVEN:
    uart->Init.WordLength = UART_WORDLENGTH_9B;
    uart->Init.Parity = UART_PARITY_EVEN;
    break;
  default:
    uart->Init.WordLength = UART_WORDLENGTH_8B;
    uart->Init.Parity = UART_PARITY_NONE;
    break;
  };

  uart->Init.StopBits = UART_STOPBITS_1;
  uart->Init.Mode = UART_MODE_TX_RX;
  uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart->Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(uart) == HAL_OK) {
    return TRUE;
  }

  return FALSE;
}

void vMBPortClose(void)
{
  HAL_UART_DeInit(uart);
}

/* Put a byte in the UARTs transmit buffer. This function is called
   * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
   * called. */
BOOL xMBPortSerialPutByte(CHAR ucByte)
{
  if(uart->Init.WordLength == UART_WORDLENGTH_9B) {
    uart->Instance->DR = ((uint16_t)ucByte & (uint16_t)0x01FF);
  }
  else {
    uart->Instance->DR = ((uint16_t)ucByte & (uint8_t)0xFF);
  }

  return TRUE;
}

/* Return the byte in the UARTs receive buffer. This function is called
   * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
   */
BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
  static uint16_t tmp;
  if(uart->Init.WordLength == UART_WORDLENGTH_9B) {
    tmp = (uint16_t)(uart->Instance->DR & (uint16_t)0x00FF);
  }
  else {
    tmp = (uint8_t)(uart->Instance->DR & (uint16_t)0x00FF);
  }
  *pucByte = (CHAR)tmp;

  return TRUE;
}

void vMBPort_USART_IRQHandler(void)
{
  if ((__HAL_UART_GET_FLAG(uart, UART_FLAG_RXNE)) &&
      (__HAL_UART_GET_IT_SOURCE(uart, UART_IT_RXNE) != RESET)) {
    pxMBFrameCBByteReceived();
  }

  if ((__HAL_UART_GET_FLAG(uart, UART_FLAG_TXE)) &&
      (__HAL_UART_GET_IT_SOURCE(uart, UART_IT_TXE) != RESET)) {
    waitOnFlagUntilTimeout(uart, UART_FLAG_TC, RESET, 1000);
    pxMBFrameCBTransmitterEmpty();
  }
}

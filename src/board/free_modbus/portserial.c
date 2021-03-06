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

/* If xRXEnable enable serial receive interrupts. If xTxENable enable
 * transmitter empty interrupts.
 */
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
  static BOOL xTxEnableTmp = FALSE;
  if(xRxEnable) {
    if (!xTxEnableTmp && !xTxEnable) {
      uartSetRts(uartGetNum(uart), GPIO_PIN_RESET);
      __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
    }
  }
  else {
    __HAL_UART_DISABLE_IT(uart, UART_IT_RXNE);
    uartSetRts(uartGetNum(uart), GPIO_PIN_SET);
  }

  if (xTxEnable) {
    __HAL_UART_ENABLE_IT(uart, UART_IT_PE);
    __HAL_UART_ENABLE_IT(uart, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(uart, UART_IT_TXE);
  }
  else {
    __HAL_UART_DISABLE_IT(uart, UART_IT_TXE);
    __HAL_UART_ENABLE_IT(uart, UART_IT_TC);
  }

  xTxEnableTmp = xTxEnable;
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
                       eMBParity eParity, UCHAR stopBits)
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

  if (stopBits == 2)
    uart->Init.StopBits = UART_STOPBITS_2;
  else
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

extern void calcIrqError(uint8_t type, uint8_t state);

void vMBPort_USART_IRQHandler(void)
{
  uint32_t tmp1 = 0, tmp2 = 0;

  calcIrqError(uartGetNum(uart) + 1, uart->State);

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_PE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_PE);
  /* UART parity error interrupt occurred ------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  {
    __HAL_UART_CLEAR_PEFLAG(uart);
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_FE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_ERR);
  /* UART frame error interrupt occurred -------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  {
    __HAL_UART_CLEAR_FEFLAG(uart);
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_NE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_ERR);
  /* UART noise error interrupt occurred -------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  {
    __HAL_UART_CLEAR_NEFLAG(uart);
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_ORE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_ERR);
  /* UART Over-Run interrupt occurred ----------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  {
    __HAL_UART_CLEAR_OREFLAG(uart);
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_RXNE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_RXNE);
  if((tmp1 != RESET) && (tmp2 != RESET)) {
    pxMBFrameCBByteReceived();
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_TXE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_TXE);
  if((tmp1 != RESET) && (tmp2 != RESET)) {
    pxMBFrameCBTransmitterEmpty();
  }

  tmp1 = __HAL_UART_GET_FLAG(uart, UART_FLAG_TC);
  tmp2 = __HAL_UART_GET_IT_SOURCE(uart, UART_IT_TC);
  if((tmp1 != RESET) && (tmp2 != RESET)) {
    __HAL_UART_DISABLE_IT(uart, UART_IT_TC);

    uartSetRts(uartGetNum(uart), GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
  }
}

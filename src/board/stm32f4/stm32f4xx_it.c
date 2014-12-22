/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @date    11/09/2014 11:08:12
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "uart.h"
#include "adc.h"

/* External variables --------------------------------------------------------*/
 
extern void xPortSysTickHandler(void);
extern void hostRxIRQHandler(void);

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern uint8_t flagMcuInit;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi4_tx;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc[adc1]);
}

void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hadc[adc2].DMA_Handle);
}

void DMA1_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
}

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi4_tx);
}

void SPI4_IRQHandler(void)
{
  hostRxIRQHandler();
}

/**
* @brief This function handles USB On The Go FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(OTG_FS_IRQn);
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart1].uart);
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart2].uart);
}

void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart3].uart);
}

void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart6].uart);
}

void UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart4].uart);
}

void UART7_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[uart7].uart);
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  if (flagMcuInit) {
    xPortSysTickHandler();
  }
  HAL_IncTick();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

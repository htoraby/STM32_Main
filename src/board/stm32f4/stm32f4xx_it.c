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
#include "user_debug.h"

/* External variables --------------------------------------------------------*/

extern void xPortSysTickHandler(void);
extern void hostRxIRQHandler(void);

extern void vMBPort_TIM_IRQHandler(void);
extern void vMBPort_USART_IRQHandler(void);

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern uint8_t isBoardInit;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi5_tx;
extern DMA_HandleTypeDef hdma_spi5_rx;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;

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
  HAL_DMA_IRQHandler(hspi2.hdmatx);
}

void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hspi2.hdmarx);
}

void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hspi3.hdmatx);
}

void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hspi3.hdmarx);
}

void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hspi4.hdmatx);
}

void SPI4_IRQHandler(void)
{
  hostRxIRQHandler();
}

/*!
 * \brief Прерывания от SPI1
 */
void DMA2_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

/*!
 * \brief Прерывания от SPI5
 */
void DMA2_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi5_tx);
}

void DMA2_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi5_rx);
}

/**
* @brief This function handles USB On The Go FS global interrupt
*/
void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
}

void USART1_IRQHandler(void)
{
  if (SCADA_UART == uart1)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart1].uart);
}

void USART2_IRQHandler(void)
{
  if (SCADA_UART == uart2)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart2].uart);
}

void USART3_IRQHandler(void)
{
  if (SCADA_UART == uart3)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart3].uart);
}

void USART6_IRQHandler(void)
{
  if (SCADA_UART == uart6)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart6].uart);
}

void UART4_IRQHandler(void)
{
  if (SCADA_UART == uart4)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart4].uart);
}

void UART7_IRQHandler(void)
{
  if (SCADA_UART == uart7)
    vMBPort_USART_IRQHandler();
  else
    HAL_UART_IRQHandler(&uarts[uart7].uart);
}

/*!
 * \brief This function handles TIM interrupt request
 */
void TIM2_IRQHandler(void)
{
  vMBPort_TIM_IRQHandler();
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  if (isBoardInit) {
    osSystickHandler();
  }
  HAL_IncTick();
}

/* The fault handler implementation calls a function called
getRegistersFromStack(). */
void HardFault_Handler(void)
{
  __asm volatile
      (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word getRegistersFromStack       \n"
        );
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  __asm volatile
      (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const2                           \n"
        " bx r2                                                     \n"
        " handler2_address_const2: .word getRegistersFromStack      \n"
        );
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  __asm volatile
      (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const3                           \n"
        " bx r2                                                     \n"
        " handler2_address_const3: .word getRegistersFromStack      \n"
        );
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  __asm volatile
      (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const4                           \n"
        " bx r2                                                     \n"
        " handler2_address_const4: .word getRegistersFromStack      \n"
        );
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "board.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

TIM_HandleTypeDef htim2;

BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  __TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = ((SystemCoreClock /2) / 20000) - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = ((uint32_t)usTim1Timerout50us) - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim2);

  HAL_NVIC_SetPriority(TIM2_IRQn, SCADA_IRQ_PREPRIO, 1);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  return TRUE;
}

inline void
vMBPortTimersEnable(  )
{
  __HAL_TIM_SetCounter(&htim2, 0);
  HAL_TIM_Base_Start_IT(&htim2);
}

inline void
vMBPortTimersDisable(  )
{
  HAL_TIM_Base_Stop_IT(&htim2);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void vMBPort_TIM_IRQHandler(void)
{
  if((__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) &&
     (__HAL_TIM_GET_ITSTATUS(&htim2, TIM_IT_UPDATE) != RESET)) {
    __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    (void)pxMBPortCBTimerExpired();
  }
}

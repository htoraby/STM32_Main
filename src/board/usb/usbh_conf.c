/**
  ******************************************************************************
  * @file    FatFs/FatFs_USBDisk_RTOS/Src/usbh_conf.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    25-May-2015
  * @brief   USB Host configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "usbh_core.h"

/* Private variables ---------------------------------------------------------*/
HCD_HandleTypeDef hhcd_USB_OTG_FS;

/*******************************************************************************
                       HCD BSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspInit(HCD_HandleTypeDef* hhcd)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hhcd->Instance == USB_OTG_FS)
  { 
    /**USB_OTG_FS GPIO Configuration    
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __USB_OTG_FS_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(OTG_FS_IRQn, USB_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  }
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hhcd)
{
  if(hhcd->Instance == USB_OTG_FS)
  {
    /* Peripheral clock disable */
    __USB_OTG_FS_CLK_DISABLE();
  
    /**USB_OTG_FS GPIO Configuration    
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
  }
}

/*******************************************************************************
                       LL Driver Callbacks (HCD -> USB Host Library)
*******************************************************************************/

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_IncTimer (hhcd->pData);
}

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_Connect(hhcd->pData);
}

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_Disconnect(hhcd->pData);
} 

/**
  * @brief  Notify URB state change callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
  /* To be used with OS to sync URB state with the global state machine */
#if (USBH_USE_OS == 1)   
  USBH_LL_NotifyURBChange(hhcd->pData);
#endif 
}
/*******************************************************************************
                       LL Driver Interface (USB Host Library --> HCD)
*******************************************************************************/
/**
  * @brief  Initializes the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_LL_Init (USBH_HandleTypeDef *phost)
{
  /* Init USB_IP */
  if (phost->id == HOST_FS) {

  hhcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hhcd_USB_OTG_FS.Init.Host_channels = 11;
  hhcd_USB_OTG_FS.Init.speed = HCD_SPEED_FULL;
  hhcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hhcd_USB_OTG_FS.Init.phy_itface = HCD_PHY_EMBEDDED;
  hhcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hhcd_USB_OTG_FS.Init.low_power_enable = ENABLE;
  hhcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  hhcd_USB_OTG_FS.Init.use_external_vbus = DISABLE;
  /* Link The driver to the stack */
  hhcd_USB_OTG_FS.pData = phost;
  phost->pData = &hhcd_USB_OTG_FS;
  /* Initialize the LL Driver */	
  HAL_HCD_Init(&hhcd_USB_OTG_FS);
	
  USBH_LL_SetTimer (phost, HAL_HCD_GetCurrentFrame(&hhcd_USB_OTG_FS));
  }
  return USBH_OK;
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef  USBH_LL_DeInit (USBH_HandleTypeDef *phost)
{
  HAL_HCD_DeInit(phost->pData);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef  USBH_LL_Start(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Start(phost->pData);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef  USBH_LL_Stop (USBH_HandleTypeDef *phost)
{
  HAL_HCD_Stop(phost->pData);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_SpeedTypeDef USBH_LL_GetSpeed  (USBH_HandleTypeDef *phost)
{
  USBH_SpeedTypeDef speed = USBH_SPEED_FULL;
    
  switch (HAL_HCD_GetCurrentSpeed(phost->pData))
  {
  case 0 : 
    speed = USBH_SPEED_HIGH;
    break;
    
  case 1 : 
    speed = USBH_SPEED_FULL;    
    break;
    
  case 2 : 
    speed = USBH_SPEED_LOW;    
    break;
	
  default:  
   speed = USBH_SPEED_FULL;    
    break;  
  }
  return  speed;
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef USBH_LL_ResetPort (USBH_HandleTypeDef *phost) 
{
  HAL_HCD_ResetPort(phost->pData);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
uint32_t USBH_LL_GetLastXferSize  (USBH_HandleTypeDef *phost, uint8_t pipe)  
{
  return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef   USBH_LL_OpenPipe    (USBH_HandleTypeDef *phost, 
                                      uint8_t pipe_num,
                                      uint8_t epnum,                                      
                                      uint8_t dev_address,
                                      uint8_t speed,
                                      uint8_t ep_type,
                                      uint16_t mps)
{
  HAL_HCD_HC_Init(phost->pData,
                  pipe_num,
                  epnum,
                  dev_address,
                  speed,
                  ep_type,
                  mps);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef   USBH_LL_ClosePipe   (USBH_HandleTypeDef *phost, uint8_t pipe)   
{
  HAL_HCD_HC_Halt(phost->pData, pipe);
  return USBH_OK; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */

USBH_StatusTypeDef   USBH_LL_SubmitURB  (USBH_HandleTypeDef *phost, 
                                            uint8_t pipe, 
                                            uint8_t direction ,
                                            uint8_t ep_type,  
                                            uint8_t token, 
                                            uint8_t* pbuff, 
                                            uint16_t length,
                                            uint8_t do_ping ) 
{
  HAL_HCD_HC_SubmitRequest (phost->pData,
                            pipe, 
                            direction ,
                            ep_type,  
                            token, 
                            pbuff, 
                            length,
                            do_ping);
  return USBH_OK;   
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_URBStateTypeDef  USBH_LL_GetURBState (USBH_HandleTypeDef *phost, uint8_t pipe) 
{
  return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState (phost->pData, pipe);
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
USBH_StatusTypeDef  USBH_LL_DriverVBUS (USBH_HandleTypeDef *phost, uint8_t state)
{ 
 /* USER CODE BEGIN 0 */ 
 /* USER CODE END 0 */
  if(state == 0)
  {
    /* Drive high Charge pump */
    /* USER CODE BEGIN 1 */ 
    /* ToDo: Add IOE driver control */
    /* USER CODE END 1 */ 
  }
  else
  {
    /* Drive low Charge pump */
    /* USER CODE BEGIN 2 */
    /* ToDo: Add IOE driver control */
    /* USER CODE END 2 */
  }

  HAL_Delay(200);
  return USBH_OK;  
}

/**
  * @brief  USBH_LL_SetToggle 
  *         Initialize the HOST portion of the driver.
  * @param  phost: Selected device
  * @param  base_address: OTG base address
  * @retval Status
  */
USBH_StatusTypeDef   USBH_LL_SetToggle   (USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)   
{
  HCD_HandleTypeDef *pHandle;
  pHandle = phost->pData;
  
  if(pHandle->hc[pipe].ep_is_in)
  {
    pHandle->hc[pipe].toggle_in = toggle;
  }
  else
  {
    pHandle->hc[pipe].toggle_out = toggle;
  }
  
  return USBH_OK; 
}

/**
  * @brief  USBH_LL_GetToggle 
  *         Initialize the HOST portion of the driver.
  * @param  phost: Selected device
  * @param  base_address: OTG base address
  * @retval Status
  */
uint8_t  USBH_LL_GetToggle   (USBH_HandleTypeDef *phost, uint8_t pipe)   
{
  uint8_t toggle = 0;
  HCD_HandleTypeDef *pHandle;
  pHandle = phost->pData; 
  
  if(pHandle->hc[pipe].ep_is_in)
  {
    toggle = pHandle->hc[pipe].toggle_in;
  }
  else
  {
    toggle = pHandle->hc[pipe].toggle_out;
  }
  return toggle; 
}

/**
  * @brief   
  * @param  
  * @param  
  * @retval Status
  */
void  USBH_Delay (uint32_t Delay)
{
  HAL_Delay(Delay);  
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

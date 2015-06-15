#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "usb_host.h"

#include "gpio.h"
#include "rtc.h"
#include "sram.h"
#include "fram.h"
#include "flash_ext.h"
#include "adc.h"
#include "adc_ext.h"
#include "iwdg.h"
#include "temp_sensor.h"

bool isBoardInit = false;

void boardInit()
{
  gpioInit();
  sramInit();
  rtcInit();
  adcInit(adc1);
  adcInit(adc2);
  adcExtInit();
  framInit();
  flashExtInit(FlashSpi1);
  flashExtInit(FlashSpi5);
  tempSensorInit();

  iwdgInit();

  isBoardInit = true;
}

void usbInit()
{
  // FatFS: Link the USBH disk I/O driver
  USBH_DriverNum = FATFS_LinkDriver(&USBH_Driver, USBH_Path);

  // Инициализация USB HOST
  MX_USB_HOST_Init();
}

void HAL_Delay(__IO uint32_t Delay)
{
  if (isBoardInit) {
    osDelay(Delay);
  }
  else {
    uint32_t tickstart = 0;
    tickstart = HAL_GetTick();
    while((HAL_GetTick() - tickstart) < Delay);
  }
}

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

#include "board.h"

USBH_HandleTypeDef hUsbHostFS;
UsbStateTypeDef usbState = USB_IDLE;
uint8_t USBH_DriverNum;      /* FatFS USBH part */
char USBH_Path[4];           /* USBH logical drive path */
FATFS fatfs;

static void userProcess(USBH_HandleTypeDef *phost, uint8_t id);

void usbInit()
{
  // FatFS: Link the USBH disk I/O driver
  USBH_DriverNum = FATFS_LinkDriver(&USBH_Driver, USBH_Path);

  // Инициализация USB HOST
  USBH_Init(&hUsbHostFS, userProcess, HOST_FS);

  USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS);

  USBH_Start(&hUsbHostFS);
}

static void userProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;
  case HOST_USER_DISCONNECTION:
    f_mount(NULL, USB_DISK, 0);
    usbState = USB_DISCONNECT;
    break;
  case HOST_USER_CLASS_ACTIVE:
    if (f_mount(&fatfs, USB_DISK, 0) != FR_OK) {
      asm("nop"); // Ошибка
    } else {
      usbState = USB_READY;
    }
    break;
  case HOST_USER_CONNECTION:
    usbState = USB_START;
    break;
  default:
    break;
  }
}

#ifndef __usb_host_H
#define __usb_host_H

#include "ff_gen_drv.h"
#include "usbh_diskio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  USB_IDLE = 0,
  USB_START,
  USB_READY,
  USB_DISCONNECT,
} UsbStateTypeDef;

extern UsbStateTypeDef usbState;
extern USBH_HandleTypeDef hUsbHostFS;

/*!
 * \brief Инициализация USB и подключение файловой системы
 */
void usbInit();

#ifdef __cplusplus
}
#endif

#endif // __usb_host_H

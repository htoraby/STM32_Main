#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#define USE_WATCHDOG 0
#define USE_EXT_MEM  1
#define USE_TEST     1

#define WATCHDOG_TIMEOUT 5 // 5 сек


#define FIRMWARE_VERSION 0x0001
#define FIRMWARE_DATE __DATE__
#define FIRMWARE_TIME __TIME__


#define FLASH_IRQ_PREPRIO 6
#define UART_IRQ_PREPRIO  7
#define HOST_IRQ_PREPRIO  8
#define USB_IRQ_PREPRIO   9
#define ADC_IRQ_PREPRIO   10


#define VSD_UART uart3
#define TMS_UART uart2
#define EM_UART uart7
#define HOST_UART uart1


#define USB_DISK "0"


/*!
 * \brief Инициализация переферии
 */
void boardInit();

/*!
 * \brief Инициализация USB и подключение файловой системы
 */
void usbInit();

#endif // BOARD_H

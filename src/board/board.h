#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#ifdef USE_RTT
#include "SEGGER_RTT.h"
#endif

#define USE_WATCHDOG 0
#define USE_EXT_MEM  1
#define USE_TEST     0
#define USE_DEBUG    0
#define USE_LOG_DEBUG 0
#define USE_LOG_WARNING 0
#define USE_LOG_CRITICAL 0

#define WATCHDOG_TIMEOUT 10 // 10 сек


#define FIRMWARE_VERSION 0x0001
#define FIRMWARE_DATE __DATE__
#define FIRMWARE_TIME __TIME__


#define USB_IRQ_PREPRIO   5
#define FLASH_IRQ_PREPRIO 7
#define UART_IRQ_PREPRIO  8
#define SCADA_IRQ_PREPRIO  8
#define HOST_IRQ_PREPRIO  9
#define ADC_IRQ_PREPRIO   10


#define VSD_UART uart3
#define TMS_UART uart2
#define EM_UART uart7
#define HOST_UART uart1
#define SCADA_UART uart4
#define VSD_LOG_UART uart6


#define USB_DISK "0"


/*!
 * \brief Инициализация переферии
 */
void boardInit();

#endif // BOARD_H

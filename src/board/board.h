#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "version_rev.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#ifdef USE_RTT
#include "SEGGER_RTT.h"
#endif

#define USE_WATCHDOG 1
#define USE_EXT_MEM  1
#define USE_TEST     0
#define USE_DEBUG    0
#define USE_LOG_DEBUG 0           // Предлагаю формат сообщения "Группа - 10 максимум символов: Сообщение - 50 символов (Значения переменных и другая информация - максимум 100 символов)
#define USE_LOG_WARNING 1
#define USE_LOG_CRITICAL 1
#define USE_POWER_OFF 0

#define WATCHDOG_TIMEOUT 15 // сек (максимум 16 сек!)


#define HARDWARE_VERSION 0x0200 // 1.0 - Ext Flash - 32мб, 2.0 - Ext Flash - 64мб
#if (HARDWARE_VERSION >= 0x0200)
  #define FIRMWARE_VERSION 0x0203 //
#else
  #define FIRMWARE_VERSION 0x0103 //
#endif
#define FIRMWARE_DATE __DATE__
#define FIRMWARE_TIME __TIME__


#define USB_IRQ_PREPRIO   5
#define FLASH_IRQ_PREPRIO 7
#define UART_IRQ_PREPRIO  8
#define SCADA_IRQ_PREPRIO  8
#define HOST_IRQ_PREPRIO  5
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

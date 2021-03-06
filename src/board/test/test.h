#ifndef TEST_H
#define TEST_H

#include "blink_led.h"

#define TEST_USB_FAT     0
#define TEST_LED         0
#define TEST_UART        0
#define TEST_RTC         0
#define TEST_ADC         0
#define TEST_FRAM        0
#define TEST_FLASH1      0
#define TEST_FLASH2      0
#define TEST_T_SENSOR    1
#define TEST_DI          0
#define TEST_DO          0
#define TEST_ADC_EXT     0
#define TEST_HOST_SPI    0
#define TEST_LOG         0
#define TEST_HOST_UART   0
#define TEST_PROTECTIONS 0
#define TEST_POWER_OFF   0

#define HOST_TEST_UART uart6
#define TEST_UART_1 uart4 // 4
#define TEST_UART_2 uart2

void testInit();

#endif // TEST_H

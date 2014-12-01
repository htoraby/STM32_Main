#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#define USE_WATCHDOG 1
#define USE_EXT_MEM  1
#define USE_TEST     1

#define WATCHDOG_TIMEOUT 5 // 5 сек

#endif // BOARD_H

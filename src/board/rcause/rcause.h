#ifndef RCAUSE_H
#define RCAUSE_H

#include "board.h"

/*!
 \brief Структура счётчиков перезапуска СPU

*/
typedef struct {
    int bor;  //!< POR reset
    int pin;  //!< Pin reset
    int por;  //!< POR reset
    int sft;  //!< Software reset
    int iwdg; //!< Independent Watchdog reset
    int wwdg; //!< Window Watchdog reset
    int lpwr; //!< Low Power reset
} RCAUSE_COUNTS;

/*!
 \brief Функция подсчета счетчиков перезапуска СPU

*/
void resetCauseCheck();


/*!
 \brief Получение стетчиков перезапуска СPU

 \return RCAUSE_COUNTS
*/
RCAUSE_COUNTS rcauseCountersGet();

/*!
 \brief Обнуление счетчиков перезапуска СPU

*/
void rcauseCountersClear();


#endif // RCAUSE_H

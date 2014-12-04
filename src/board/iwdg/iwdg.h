#ifndef WWDG_H
#define WWDG_H

#include "board.h"

/*!
 \brief Инициализация Watchdog

*/
void iwdgInit();

/*!
 \brief Ручной сброс Watchdog

*/
void iwdgReset();

#endif // WWDG_H

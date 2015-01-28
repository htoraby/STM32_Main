#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "board.h"
#include "host.h"
#include "log_main.h"
#include "ccs.h"
#include "vsd_novomet.h"
#include "parameters.h"

extern Parameters parameters;
extern Ccs ksu;
extern VsdNovomet vsd;

/*!
 * \brief Инициализация пользовательских задач и объектов
 *
 */
void userInit();

#endif // USER_MAIN_H

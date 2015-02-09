#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "board.h"
#include "host.h"
#include "log_main.h"
#include "ccs.h"
#include "vsd_novomet.h"
#include "tms.h"
#include "em.h"
#include "parameters.h"
#include "novobus_slave.h"

extern Parameters parameters;
extern NovobusSlave novobusSlave;
extern Ccs ksu;
extern VsdNovomet *vsd;
extern Tms *tms;
extern Em *em;

/*!
 * \brief Инициализация пользовательских задач и объектов
 *
 */
void userInit();

#endif // USER_MAIN_H

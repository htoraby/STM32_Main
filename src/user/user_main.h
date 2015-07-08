#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "board.h"
#include "log_main.h"
#include "ccs.h"
#include "vsd_novomet.h"
#include "vsd_etalon.h"
#include "tms_novomet.h"
#include "em_set.h"
#include "scada.h"
#include "parameters.h"
#include "novobus_slave.h"

extern Parameters parameters;
extern NovobusSlave novobusSlave;
extern Ccs ksu;
extern Vsd *vsd;
extern Tms *tms;
extern Em *em;
extern Scada *scada;

/*!
 * \brief Инициализация пользовательских задач и объектов
 */
void userInit();
void createVsd();
void createDhs();
void createEm();
void createScada();

#endif // USER_MAIN_H

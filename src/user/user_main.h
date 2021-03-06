#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "board.h"
#include "log_main.h"
#include "ccs.h"
#include "vsd_novomet.h"
#include "vsd_etalon.h"
#include "vsd_danfoss.h"
#include "tms_novomet.h"
#include "tms_elekton_2.h"
#include "tms_elekton_3.h"
#include "tms_borets.h"
#include "tms_borets_2.h"
#include "tms_irz.h"
#include "tms_etalon.h"
#include "tms_almaz.h"
#include "tms_scad.h"
#include "tms_triol.h"
#include "tms_pic_v2.h"
#include "tms_sng.h"
#include "tms_transfer.h"
#include "tms_zenith.h"
#include "em_set.h"
#include "em_abb.h"
#include "scada_region_3_0.h"
#include "scada_novomet.h"
#include "scada_region_2000.h"
#include "scada_telescop.h"
#include "scada_lukoil.h"
#include "scada_karakuduk.h"
#include "scada_surgutneftegas.h"
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
void createVsd(bool init = false);
void createDhs(bool init = false);
void createEm(bool init = false);
void createScada();

#endif // USER_MAIN_H

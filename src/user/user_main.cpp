#include "user_main.h"
#include "regime_main.h"
#include "protection_main.h"
#include "user_debug.h"

Parameters parameters;
NovobusSlave novobusSlave;
Ccs ksu;
Vsd *vsd = NULL;
Tms *tms = NULL;
Em *em = NULL;
Scada *scada = NULL;

void userInit()
{
  ksu.init();

  createVsd();
  createDhs();
  createEm();

  parameters.init();
  novobusSlave.init();
  logInit();

  // Запись в журнал "Включение питания"
  logEvent.add(PowerCode, AutoType, PowerOnId);
  logDebug.add(WarningMsg, "*** Power On ***");
  checkRegistersFromStack();
  checkIrqError();
  checkRcauseCounters();

  vsd->init();
  tms->init();
  em->init();

  ksu.initTask();

  protectionInit();
  regimeInit();

  createScada();
}

void createVsd(bool init)
{
  if (vsd) {
    delete vsd;
    vsd = NULL;
  }

  uint8_t type = parameters.get(CCS_TYPE_VSD);
  switch (type) {
  case VSD_TYPE_NOVOMET:
    vsd = new VsdNovomet();
    break;
  case VSD_TYPE_DANFOSS:
    vsd = new VsdDanfoss();
    break;
  case VSD_TYPE_ETALON:
    vsd = new VsdEtalon();
    break;
  default:
    vsd = new Vsd();
    break;
  }

  if (init)
    vsd->init();
}

void createDhs(bool init)
{
  if (tms) {
    delete tms;
    tms = NULL;
  }

  uint8_t type = parameters.get(CCS_DHS_TYPE);
  switch (type) {
  case TYPE_DHS_NOVOMET:
    tms = new TmsNovomet();
    break;
  case TYPE_DHS_ELEKTON_2:
    tms = new TmsElekton2();
    break;
  case TYPE_DHS_ELEKTON_3:
    tms = new TmsElekton3();
    break;
  default:
    tms = new Tms();
    break;
  }

  if (init)
    tms->init();
}

void createEm(bool init)
{
  if (em) {
    delete em;
    em = NULL;
  }

  uint8_t type = parameters.get(CCS_EM_TYPE);
  switch (type) {
  case 1:
    em = new EmSet();
    break;
  default:
    em = new Em();
    break;
  }

  if (init)
    em->init();
}

void createScada()
{
  if (scada) {
    delete scada;
    scada = NULL;
  }

  uint8_t type = parameters.get(CCS_SCADA_TYPE);
  switch (type) {
  case Scada::Region30Type:
    scada = new ScadaRegion30();
    break;
  case Scada::NovometType:
    scada = new ScadaNovomet();
    break;
  default:
    break;
  }
}

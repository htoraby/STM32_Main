#include "user_main.h"
#include "regime_main.h"
#include "protection_main.h"

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

  vsd->init();
  tms->init();
  em->init();

  ksu.initTask();

  protectionInit();
  regimeInit();

  createScada();
}

void createVsd()
{
  if (vsd)
    delete vsd;

  uint8_t type = parameters.get(CCS_TYPE_VSD);
  switch (type) {
  case VSD_TYPE_NOVOMET:
    vsd = new VsdNovomet();
    break;
  case VSD_TYPE_ETALON:
    vsd = new VsdEtalon();
    break;
  default:
    vsd = new Vsd();
    break;
  }
}

void createDhs()
{
  if (tms)
    delete tms;

  uint8_t type = parameters.get(CCS_DHS_TYPE);
  switch (type) {
  case TYPE_DHS_NOVOMET:
    tms = new TmsNovomet();
    break;
  default:
    tms = new Tms();
    break;
  }
}

void createEm()
{
  if (em)
    delete em;

  uint8_t type = parameters.get(CCS_EM_TYPE);
  switch (type) {
  case 1:
    em = new EmSet();
    break;
  default:
    em = new Em();
    break;
  }
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
  default:
    break;
  }
}

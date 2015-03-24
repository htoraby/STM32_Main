#include "user_main.h"
#include "protection_main.h"
#include "regime_main.h"

Parameters parameters;
NovobusSlave novobusSlave;
Ccs ksu;
Vsd *vsd;
Tms *tms;
Em *em;

void userInit()
{
  ksu.init();

  vsd = new VsdNovomet();
  tms = new TmsNovomet();
  em = new EmSet();

  logInit();
  parameters.init();
  novobusSlave.init();

  vsd->init();
  tms->init();
  em->init();

  ksu.initTask();

  protectionInit();
//  regimeInit();

#if DEBUG
  ksu.setValue(CCS_WORKING_MODE, CCS_WORKING_MODE_AUTO);
  ksu.resetBlock();
  ksu.resetDelay();
#endif

}

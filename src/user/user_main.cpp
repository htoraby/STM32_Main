#include "user_main.h"
#include "regime_main.h"
#include "protection_main.h"

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

  parameters.init();
  novobusSlave.init();
  logInit();

  vsd->init();
  tms->init();
  em->init();

  ksu.initTask();

  protectionInit();
  regimeInit();
}

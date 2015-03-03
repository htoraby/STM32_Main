#include "user_main.h"
#include "protection_main.h"

Parameters parameters;
NovobusSlave novobusSlave;
Ccs ksu;
Vsd *vsd;
Tms *tms;
Em *em;

void userInit()
{
  logInit();
  novobusSlave.init();

  vsd = new VsdNovomet();
  tms = new Tms();
  em = new EmSet();

  parameters.init();
  ksu.init();

  protectionInit();
}

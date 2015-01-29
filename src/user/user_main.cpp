#include "user_main.h"
#include "protection_main.h"

Parameters parameters;
Ccs ksu;
VsdNovomet vsd;
Tms tms;
Em em;

void userInit()
{
  logInit();
  hostInit();

  vsd.init();

  parameters.init();
  protectionInit();

}


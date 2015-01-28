#include "user_main.h"

Parameters parameters;
Ccs ksu;
VsdNovomet vsd;

void userInit()
{
  logInit();
  hostInit();

  vsd.init();

  parameters.init();


}


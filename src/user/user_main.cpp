#include "user_main.h"

Parameters parameters;
Ccs KSU;
VsdNovomet vsd;

void userInit()
{
  logInit();
  hostInit();

  vsd.init();

  parameters.init();


}


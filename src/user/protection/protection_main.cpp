#include "protection_main.h"

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;

void protectionInit()
{
  protOverVoltIn.init();
  protUnderVoltIn.init();
}

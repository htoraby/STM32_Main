#include "protection_main.h"

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;
ProtectionOverloadMotor protOverloadMotor;
ProtectionUnderloadMotor protUnderloadMotor;

void protectionInit()
{
  protOverVoltIn.init();
  protUnderVoltIn.init();

  protOverloadMotor.init();
  protUnderloadMotor.init();

}

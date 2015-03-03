#include "protection_main.h"

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;

ProtectionOverloadMotor protOverloadMotor;
ProtectionUnderloadMotor protUnderloadMotor;
ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
ProtectionOutOfSyncMotor protOutOfSyncMotor;

void protectionInit()
{
  protOverVoltIn.init();
  protUnderVoltIn.init();

  protOverloadMotor.init();
  protUnderloadMotor.init();
  protImbalanceCurrentMotor.init();
  protOutOfSyncMotor.init();
}

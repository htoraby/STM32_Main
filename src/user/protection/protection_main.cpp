#include "protection_main.h"

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;
ProtectionImbalanceVoltageInput protImbalanceVoltIn;

ProtectionOverloadMotor protOverloadMotor;
ProtectionUnderloadMotor protUnderloadMotor;
ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
ProtectionOutOfSyncMotor protOutOfSyncMotor;
ProtectionTurbineRotation protTurbineRotation;

ProtectionTemperatureMotor protTemperatureMotor;
ProtectionPressureIntake protPressureIntake;
ProtectionResistanceIsolation protResistanceIsolation;

void protectionInit()
{
  // Supply
  protOverVoltIn.init();
  protUnderVoltIn.init();
  protImbalanceVoltIn.init();

  // Motor
  protOverloadMotor.init();
  protUnderloadMotor.init();
  protImbalanceCurrentMotor.init();
  protOutOfSyncMotor.init();
  protTurbineRotation.init();

  // Tms
  protTemperatureMotor.init();
  protPressureIntake.init();
  protResistanceIsolation.init();
}

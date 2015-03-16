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

static void protectionTask(void *argument);

void protectionInit()
{
  osThreadDef(ProtectionTask, protectionTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(ProtectionTask), NULL);
}

void protectionTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(10);

    // Supply
    protOverVoltIn.processing();
    protUnderVoltIn.processing();
    protImbalanceVoltIn.processing();

    // Motor
    protOverloadMotor.processing();
    protUnderloadMotor.processing();
    protImbalanceCurrentMotor.processing();
    protOutOfSyncMotor.processing();
    protTurbineRotation.processing();

    // Tms
    protTemperatureMotor.processing();
    protPressureIntake.processing();
    protResistanceIsolation.processing();
  }
}

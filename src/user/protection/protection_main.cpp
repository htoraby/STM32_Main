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
static void setProtectionPrevent();

void protectionInit()
{
  osThreadDef(ProtectionTask, protectionTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(ProtectionTask), NULL);
}

void protectionTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(100);

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

    setProtectionPrevent();
  }
}

void setProtectionPrevent()
{
  bool prevent = false;
  prevent = prevent ? true : protOverVoltIn.isPrevent();
  prevent = prevent ? true : protUnderVoltIn.isPrevent();
  prevent = prevent ? true : protImbalanceVoltIn.isPrevent();
  prevent = prevent ? true : protOverloadMotor.isPrevent();
  prevent = prevent ? true : protUnderloadMotor.isPrevent();
  prevent = prevent ? true : protImbalanceCurrentMotor.isPrevent();
  prevent = prevent ? true : protOutOfSyncMotor.isPrevent();
  prevent = prevent ? true : protTurbineRotation.isPrevent();
  prevent = prevent ? true : protTemperatureMotor.isPrevent();
  prevent = prevent ? true : protPressureIntake.isPrevent();
  prevent = prevent ? true : protResistanceIsolation.isPrevent();
  parameters.setValue(CCS_PROT_PREVENT, prevent);
}

void addEventProtectionPrevent()
{
  if (protOverVoltIn.isPrevent())
    logEvent.add(ProtectCode, AutoType, protOverVoltIn.getApvDisabledEventId());
  if (protUnderVoltIn.isPrevent())
    logEvent.add(ProtectCode, AutoType, protUnderVoltIn.getApvDisabledEventId());
  if (protImbalanceVoltIn.isPrevent())
    logEvent.add(ProtectCode, AutoType, protImbalanceVoltIn.getApvDisabledEventId());
  if (protOverloadMotor.isPrevent())
    logEvent.add(ProtectCode, AutoType, protOverloadMotor.getApvDisabledEventId());
  if (protUnderloadMotor.isPrevent())
    logEvent.add(ProtectCode, AutoType, protUnderloadMotor.getApvDisabledEventId());
  if (protImbalanceCurrentMotor.isPrevent())
    logEvent.add(ProtectCode, AutoType, protImbalanceCurrentMotor.getApvDisabledEventId());
  if (protOutOfSyncMotor.isPrevent())
    logEvent.add(ProtectCode, AutoType, protOutOfSyncMotor.getApvDisabledEventId());
  if (protTurbineRotation.isPrevent())
    logEvent.add(ProtectCode, AutoType, protTurbineRotation.getApvDisabledEventId());
  if (protTemperatureMotor.isPrevent())
    logEvent.add(ProtectCode, AutoType, protTemperatureMotor.getApvDisabledEventId());
  if (protPressureIntake.isPrevent())
    logEvent.add(ProtectCode, AutoType, protPressureIntake.getApvDisabledEventId());
  if (protResistanceIsolation.isPrevent())
    logEvent.add(ProtectCode, AutoType, protResistanceIsolation.getApvDisabledEventId());
}

#include "protection_main.h"

#define COUNT_PROTECTIONS 15

Protection *protections[COUNT_PROTECTIONS];

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
ProtectionHardwareVsd protHardwareVsd;

ProtectionDigitalInput1 protDigitalInput1;
ProtectionDigitalInput2 protDigitalInput2;
ProtectionDigitalInput3 protDigitalInput3;
ProtectionDigitalInput4 protDigitalInput4;

static void protectionTask(void *argument);
static void setProtectionPrevent();
static void setProtectionDelay();

void protectionInit()
{
  protections[0] = &protOverVoltIn;
  protections[1] = &protUnderVoltIn;
  protections[2] = &protImbalanceVoltIn;

  protections[3] = &protImbalanceCurrentMotor;
  protections[4] = &protTurbineRotation;

  protections[5] = &protResistanceIsolation;
  protections[6] = &protPressureIntake;
  protections[7] = &protTemperatureMotor;
  protections[8] = &protUnderloadMotor;
  protections[9] = &protOverloadMotor;

  protections[10] = &protHardwareVsd;

  protections[11] = &protDigitalInput1;
  protections[12] = &protDigitalInput2;
  protections[13] = &protDigitalInput3;
  protections[14] = &protDigitalInput4;

  osThreadDef(ProtectionTask, protectionTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(ProtectionTask), NULL);
}

void protectionTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(100);

    for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
      protections[i]->processing();
    }
    setProtectionDelay();
    setProtectionPrevent();
  }
}

void setProtectionDelay()
{
  for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
    if (protections[i]->isDelay()) {
      ksu.setDelay();
      return;
    }
  }
  ksu.resetDelay();
}

void setProtectionPrevent()
{
  bool prevent = false;
  for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
    prevent = prevent ? true : protections[i]->isPrevent();
  }
  parameters.set(CCS_PROT_PREVENT, prevent);
}

void addEventProtectionPrevent()
{
  for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
    if (protections[i]->isPrevent())
      logEvent.add(ProtectCode, AutoType, protections[i]->getApvDisabledEventId());
  }
}

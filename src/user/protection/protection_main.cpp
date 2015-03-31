#include "protection_main.h"

#define COUNT_PROTECTIONS 5 // 11

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

static void protectionTask(void *argument);
static void setProtectionPrevent();
static void setProtectionDelay();

void protectionInit()
{
  protections[0] = &protOverVoltIn;
  protections[1] = &protUnderVoltIn;
  protections[2] = &protImbalanceVoltIn;

  protections[3] = &protImbalanceCurrentMotor;
  protections[4] = &protTemperatureMotor;

//  protections[3] = &protOverloadMotor;
//  protections[4] = &protUnderloadMotor;
//  protections[5] = &protImbalanceCurrentMotor;
//  protections[6] = &protOutOfSyncMotor;
//  protections[7] = &protTurbineRotation;
//  protections[8] = &protTemperatureMotor;
//  protections[9] = &protPressureIntake;
//  protections[10] = &protResistanceIsolation;

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
    ksu.resetDelay();
  }
}

void setProtectionPrevent()
{
  bool prevent = false;
  for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
    prevent = prevent ? true : protections[i]->isPrevent();
  }
  parameters.setValue(CCS_PROT_PREVENT, prevent);
}

void addEventProtectionPrevent()
{
  for (int i = 0; i < COUNT_PROTECTIONS; ++i) {
    if (protections[i]->isPrevent())
      logEvent.add(ProtectCode, AutoType, protections[i]->getApvDisabledEventId());
  }
}

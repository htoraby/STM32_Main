#include "protection_main.h"

#define COUNT_PROTECTIONS 21

Protection *protections[COUNT_PROTECTIONS];

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;
ProtectionImbalanceVoltageInput protImbalanceVoltIn;
ProtectionImbalanceCurrentInput protImbalanceCurIn;

ProtectionOverloadMotor protOverloadMotor;
ProtectionUnderloadMotor protUnderloadMotor;
ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
ProtectionOutOfSyncMotor protOutOfSyncMotor;
ProtectionTurbineRotation protTurbineRotation;

ProtectionTemperatureMotor protTemperatureMotor;
ProtectionPressureIntake protPressureIntake;
ProtectionResistanceIsolation protResistanceIsolation;
ProtectionHardwareVsd protHardwareVsd;
ProtectionLockDoor protLockDoor;

ProtectionDigitalInput1 protDigitalInput1;
ProtectionDigitalInput2 protDigitalInput2;
ProtectionDigitalInput3 protDigitalInput3;
ProtectionDigitalInput4 protDigitalInput4;

ProtectionAnalogInput1 protAnalogInput1;
ProtectionAnalogInput2 protAnalogInput2;
ProtectionAnalogInput3 protAnalogInput3;
ProtectionAnalogInput4 protAnalogInput4;

static void protectionTask(void *argument);
static void setProtectionPrevent();
static void setProtectionDelay();

void protectionInit()
{
  protections[0] = &protOverVoltIn;
  protections[1] = &protUnderVoltIn;
  protections[2] = &protImbalanceVoltIn;
  protections[3] = &protImbalanceCurIn;

  protections[4] = &protImbalanceCurrentMotor;
  protections[5] = &protTurbineRotation;

  protections[6] = &protResistanceIsolation;
  protections[7] = &protPressureIntake;
  protections[8] = &protTemperatureMotor;
  protections[9] = &protUnderloadMotor;
  protections[10] = &protOverloadMotor;

  protections[11] = &protHardwareVsd;
  protections[12] = &protLockDoor;

  protections[13] = &protDigitalInput1;
  protections[14] = &protDigitalInput2;
  protections[15] = &protDigitalInput3;
  protections[16] = &protDigitalInput4;

  protections[17] = &protAnalogInput1;
  protections[18] = &protAnalogInput2;
  protections[19] = &protAnalogInput3;
  protections[20] = &protAnalogInput4;

  osThreadDef(Protections, protectionTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Protections), NULL);
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

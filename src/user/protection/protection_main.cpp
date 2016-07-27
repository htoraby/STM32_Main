#include "protection_main.h"

#define COUNT_PROTECTIONS 29

Protection *protections[COUNT_PROTECTIONS];

ProtectionOverVoltageInput protOverVoltIn;
ProtectionUnderVoltageInput protUnderVoltIn;
ProtectionImbalanceVoltageInput protImbalanceVoltIn;
ProtectionImbalanceCurrentInput protImbalanceCurIn;
ProtectionPowerOff protPowerOff;

ProtectionOverloadMotor protOverloadMotor;
ProtectionUnderloadMotor protUnderloadMotor;
ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
ProtectionOutOfSyncMotor protOutOfSyncMotor;
ProtectionTurbineRotation protTurbineRotation;

ProtectionTemperatureMotor protTemperatureMotor;
ProtectionPressureIntake protPressureIntake;
ProtectionPressureDischarge protPressureDischarge;
ProtectionResistanceIsolation protResistanceIsolation;
ProtectionVibrationMotor protVibrationMotor;
ProtectionFlowDischarge protFlowDischarge;

ProtectionOvernumberOfStart protOvernumberOfStart;
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

ProtectionCurrentMotor protCurrentMotor;

ProtectionOverheatInputFilter protOverheatInputFilter;

static void protectionTask(void *argument);
static void setProtectionPrevent();
static void setProtectionDelay();

void protectionInit()
{
  int i = 0;
  protections[i++] = &protOverVoltIn;
  protections[i++] = &protUnderVoltIn;
  protections[i++] = &protImbalanceVoltIn;
  protections[i++] = &protImbalanceCurIn;
  protections[i++] = &protPowerOff;

  protections[i++] = &protImbalanceCurrentMotor;
  protections[i++] = &protTurbineRotation;
  protections[i++] = &protOutOfSyncMotor;

  protections[i++] = &protResistanceIsolation;
  protections[i++] = &protPressureIntake;
  protections[i++] = &protPressureDischarge;
  protections[i++] = &protTemperatureMotor;
  protections[i++] = &protUnderloadMotor;
  protections[i++] = &protOverloadMotor;
  protections[i++] = &protVibrationMotor;
  protections[i++] = &protFlowDischarge;

  protections[i++] = &protOvernumberOfStart;
  protections[i++] = &protHardwareVsd;
  protections[i++] = &protLockDoor;

  protections[i++] = &protDigitalInput1;
  protections[i++] = &protDigitalInput2;
  protections[i++] = &protDigitalInput3;
  protections[i++] = &protDigitalInput4;

  protections[i++] = &protAnalogInput1;
  protections[i++] = &protAnalogInput2;
  protections[i++] = &protAnalogInput3;
  protections[i++] = &protAnalogInput4;

  protections[i++] = &protCurrentMotor;

  protections[i++] = &protOverheatInputFilter;

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

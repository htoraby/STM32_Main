#include "test_protection.h"
#include "user_main.h"

static void testProtectionThread(void *argument);
static void testProtectionOverVoltageInput();

void testProtectionInit()
{
  osThreadDef(TestProtection, testProtectionThread, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(TestProtection), NULL);
}

void testProtectionThread(void *argument)
{
  (void)argument;

  osDelay(1000);


//  parameters.setValue(CCS_TRANS_NOMINAL_VOLTAGE, 220);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 102);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 5);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 20);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 60);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 14);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 60);
//  parameters.setValue(CCS_TIMER_DIFFERENT_START, 30);
//  parameters.setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 0);


  while (1) {
    osDelay(1000);

    testProtectionOverVoltageInput();
  }
}

void testProtectionOverVoltageInput()
{
#if TEST_PROT_OVER_VOLTAGE_IN
  static int timeSec = 0;

  timeSec++;
  if (timeSec < 20)
    parameters.setValue(EM_VOLTAGE_PHASE_1, 220);
  else if ((timeSec > 90) && (timeSec < 130))
    parameters.setValue(EM_VOLTAGE_PHASE_1, 220);
  else if ((timeSec > 190) && (timeSec < 200))
    parameters.setValue(EM_VOLTAGE_PHASE_1, 220);
  else if (timeSec > 200) {
    parameters.setValue(EM_VOLTAGE_PHASE_1, 220);
  }
  else
    parameters.setValue(EM_VOLTAGE_PHASE_1, 230);
#endif
}

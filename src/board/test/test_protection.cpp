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


//  parameters.set(CCS_TRANS_NOMINAL_VOLTAGE, 220);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 102);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 5);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 20);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 60);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 14);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 60);
//  parameters.set(CCS_TIMER_DIFFERENT_START, 30);
//  parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 0);


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
//  if (timeSec < 20)
//    parameters.set(EM_VOLTAGE_PHASE_1, 220);
//  else if ((timeSec > 90) && (timeSec < 130))
//    parameters.set(EM_VOLTAGE_PHASE_1, 220);
//  else if ((timeSec > 190) && (timeSec < 200))
//    parameters.set(EM_VOLTAGE_PHASE_1, 220);
//  else if (timeSec > 200) {
//    parameters.set(EM_VOLTAGE_PHASE_1, 220);
//  }
//  else
//    parameters.set(EM_VOLTAGE_PHASE_1, 230);
#endif
}

#include "test_power_off.h"
#include "test.h"
#include "gpio.h"
#include "fram.h"
#include "rtc.h"

#if (TEST_POWER_OFF == 1)
static int timeMs = 0;

static void powerOffTimer(const void *argument)
{
  (void)argument;

  if (!isPowerGood()) {
    backupSaveParameter(RTC_BKP_DR1, ++timeMs);
  }
}

void testPowerOffInit()
{
  // Раскомментировать для получения значения времени отключения питания
  timeMs = backupRestoreParameter(RTC_BKP_DR1);

  osTimerDef_t timerDef = {powerOffTimer};
  osTimerId timerId = osTimerCreate(&timerDef, osTimerPeriodic, NULL);
  osTimerStart(timerId, 1);
}
#endif




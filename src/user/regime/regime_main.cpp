#include "regime_main.h"

RegimeTechnologPeriodic regimeTechnologPeriodic;

static void regimeTask(void *argument);

void regimeInit()
{
  osThreadDef(RegimeTask, regimeTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(RegimeTask), NULL);
}

void regimeTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(100);

    regimeTechnologPeriodic.processing();
  }
}


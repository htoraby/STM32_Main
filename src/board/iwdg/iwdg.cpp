#include "iwdg.h"

IWDG_HandleTypeDef hiwdg;

#if USE_WATCHDOG
static void iwdgThread(void const * argument)
{
  (void)argument;

  while (1) {
    osDelay(1000);
    HAL_IWDG_Refresh(&hiwdg);
  }
}
#endif

void iwdgInit()
{
#if USE_WATCHDOG

  //! Counter Reload Value = ms * LsiFreq / 1000 / Prescaler
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
  hiwdg.Init.Reload = WATCHDOG_TIMEOUT*32000/128;
  HAL_IWDG_Init(&hiwdg);

  osThreadDef(Iwdg_Thread, iwdgThread, osPriorityIdle, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Iwdg_Thread), NULL);

  HAL_IWDG_Start(&hiwdg);

#endif
}

void iwdgReset()
{
  HAL_IWDG_Refresh(&hiwdg);
}

#include "user_main.h"
#include "protection_main.h"

static void userMainTask(void *argument);

Parameters parameters;
NovobusSlave novobusSlave;
Ccs ksu;
VsdNovomet *vsd;
Tms *tms;
Em *em;

void userInit()
{
  logInit();
  novobusSlave.init();

  vsd = new VsdNovomet();
  tms = new Tms();
  em = new Em();

  parameters.init();

  protectionInit();

  osThreadDef(UserMain, userMainTask, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(UserMain), NULL);
}

/*!
 * \brief Инициализация основной пользовательской задачи
 *
 */
void userMainTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(1);

    ksu.controlModeChanged();
    ksu.conditionChanged();
  }
}


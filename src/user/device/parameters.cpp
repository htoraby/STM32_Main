#include "parameters.h"
#include "user_main.h"

Parameters::Parameters()
{

}

Parameters::~Parameters()
{

}

static void parametersTask(void *p)
{
  (static_cast<Parameters*>(p))->task();
}

void Parameters::init()
{
  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);

  osThreadDef_t t = {"SaveParameters", parametersTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  osThreadCreate(&t, this);

  read();
}

void Parameters::task()
{
  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    save();
  }
}

void Parameters::startSave()
{
  osSemaphoreRelease(semaphoreId_);
}

void Parameters::save()
{
  ksu.saveParameters();
  vsd.saveParameters();
}

void Parameters::read()
{
  ksu.readParameters();
  vsd.readParameters();
}

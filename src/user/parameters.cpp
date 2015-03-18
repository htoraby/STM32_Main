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
}

void Parameters::task()
{
  while (1) {
    osSemaphoreWait(semaphoreId_, PARAMS_SAVE_TIME);

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
  vsd->saveParameters();
  tms->saveParameters();
  em->saveParameters();
}

void Parameters::read()
{
  ksu.readParameters();
  vsd->readParameters();
  tms->readParameters();
  em->readParameters();
}

float Parameters::getValue(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValue(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValue(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValue(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValue(id);

  return 0;
}

int Parameters::setValue(unsigned short id, float value)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setValue(id, value);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setNewValue(id, value);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setNewValue(id, value);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setNewValue(id, value);
  return 0;
}

uint8_t Parameters::getPhysic(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getPhysic(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getPhysic(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getPhysic(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getPhysic(id);

  return 0;
}

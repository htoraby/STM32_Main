#include "parameters.h"
#include "user_main.h"
#include "gpio.h"

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
  static int time = 0;
  while (1) {  
    if (osSemaphoreWait(semaphoreId_, 1) != osEventTimeout) {
      time = 0;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Parameter: Сохранение параметров");
#endif
      save();
    } else {
      if ((++time >= PARAMS_SAVE_TIME) || ksu.isPowerOff()) {
        time = 0;
        save();
        if (ksu.isPowerOff())
          osDelay(osWaitForever);
      }
    }
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

float Parameters::get(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValue(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValue(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValue(id);
  if ((id > EM_BEGIN) && (id < EM_END)) {
    return em->getValue(id);
  }

  return 0;
}

uint32_t Parameters::getU32(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValueUint32(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValueUint32(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValueUint32(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValueUint32(id);

  return 0;
}

int Parameters::set(unsigned short id, float value, EventType eventType)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setNewValue(id, value, eventType);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setNewValue(id, value, eventType);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setNewValue(id, value, eventType);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setNewValue(id, value, eventType);
  return 0;
}

int Parameters::set(unsigned short id, uint32_t value, EventType eventType)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setNewValue(id, value, eventType);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setNewValue(id, value, eventType);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setNewValue(id, value, eventType);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setNewValue(id, value, eventType);
  return 0;
}

int Parameters::set(unsigned short id, int value, EventType eventType)
{
  return set(id, (float)value, eventType);
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

uint8_t Parameters::getValidity(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValidity(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValidity(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValidity(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValidity(id);

  return err_r;
}

bool Parameters::isValidity(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return (ksu.getValidity(id) == VALIDITY_OK);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return (vsd->getValidity(id) == VALIDITY_OK);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return (tms->getValidity(id) == VALIDITY_OK);
  if ((id > EM_BEGIN) && (id < EM_END))
    return (em->getValidity(id) == VALIDITY_OK);

  return false;
}

void Parameters::setValidity(uint16_t id, uint8_t validity)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setValidity(id, validity);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setValidity(id, validity);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setValidity(id, validity);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setValidity(id, validity);
}

float Parameters::getMin(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getMin(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getMin(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getMin(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getMin(id);
  return err_r;
}

uint8_t Parameters::setMin(uint16_t id, float value)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setMin(id, value);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setMin(id, value);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setMin(id, value);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setMin(id, value);
  return err_r;
}

float Parameters::getMax(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getMax(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getMax(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getMax(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getMax(id);
  return err_r;
}

float Parameters::getValueDef(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValueDef(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValueDef(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValueDef(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValueDef(id);
  return err_r;
}

uint8_t Parameters::setMax(uint16_t id, float value)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setMax(id, value);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setMax(id, value);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setMax(id, value);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setMax(id, value);
  return err_r;
}

void Parameters::setAccess(uint16_t id, uint8_t access)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setAccess(id, access);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setAccess(id, access);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setAccess(id, access);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setAccess(id, access);
}

void Parameters::setOperation(uint16_t id, uint8_t operation)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setOperation(id, operation);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setOperation(id, operation);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setOperation(id, operation);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setOperation(id, operation);
}

float Parameters::checkZero(unsigned short id, bool reset, float value)
{
  if (parameters.get(id) == 0) {
    if (reset) {
      if (value == 0) {
        if (parameters.getValueDef(id) == 0) {
          if (parameters.getMin(id) == 0) {
#if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Параметры: Параметр не прошел проверку на 0 (id = %d)");
#endif
            return 1;
          }
          else {
            parameters.set(id, parameters.getMin(id));
          }
        }
        else {
          parameters.set(id, parameters.getValueDef(id));
        }
      }
      else {
        parameters.set(id, value);
      }
    }
    else {
      if (value == 0) {
        return 1;
      }
      else {
        return value;
      }
    }
  }
  return parameters.get(id);
}


float Parameters::convertFrom(float value, int physic, int unit)
{
  if (unit >= MAX_CONV_ELEMS)
    unit = 0;
  return (value * units[physic][unit][0] + units[physic][unit][1]);
}

float Parameters::convertTo(float value, int physic, int unit)
{
  if (unit >= MAX_CONV_ELEMS)
    unit = 0;
  return (value - (units[physic][unit][1]))/(units[physic][unit][0]);
}

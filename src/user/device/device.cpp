/*
 * classDevice.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device.h"
#include "user_main.h"

#if USE_EXT_MEM
static float buffer[VSD_BEGIN] __attribute__((section(".extmem")));
#else
static float buffer[VSD_BEGIN];
#endif

static void deviceUpdateValueTask(void *p)
{
  (static_cast<Device*>(p))->updateValueTask();
}

Device::Device(uint32_t startAddrParams, parameter *parameters, uint16_t countParameters)
  : updateValueThreadId_(NULL)
  , getValueDeviceQId_(NULL)
  , startAddrParams_(startAddrParams)
  , parameters_(parameters)
  , countParameters_(countParameters)
{

}

Device::~Device()
{
  if (updateValueThreadId_)
    osThreadTerminate(updateValueThreadId_);
  if (getValueDeviceQId_)
    osMessageDelete(getValueDeviceQId_);
}

void Device::init()
{

}

void Device::createThread(const char *threadName)
{
  osMessageQDef(GetValueDeviceQueue, 100, uint32_t);
  getValueDeviceQId_ = osMessageCreate(osMessageQ(GetValueDeviceQueue), NULL);

  // Заполняем структуру для создания задачи
  osThreadDef_t t = {threadName,                  // Название задачи
                     deviceUpdateValueTask,       // Указатель на функцию задачи
                     osPriorityNormal,            // Приоритет задачи
                     0,                           //
                     4 * configMINIMAL_STACK_SIZE // Размер стека задачи
                    };
  updateValueThreadId_ = osThreadCreate(&t, this);// Создаём задачу
}

inline unsigned short Device::getFieldId(unsigned short index)
{
  return parameters_[index].id;
}

inline unsigned char Device::getFieldAccess(unsigned short index)
{
  return parameters_[index].access;
}

inline unsigned char Device::getFieldOperation(unsigned short index)
{
  return parameters_[index].operation;
}

inline unsigned char Device::getFieldPhysic(unsigned short index)
{
  return parameters_[index].physic;
}

inline unsigned char Device::getFieldValidity(unsigned short index)
{
  return parameters_[index].validity;
}

inline float Device::getFieldValue(unsigned short index)
{
  return parameters_[index].value.float_t;
}

inline EventCode Device::getFieldCode(uint16_t index)
{
  return (EventCode)parameters_[index].code;
}

inline uint32_t Device::getFieldValueUint32(uint16_t index)
{
  return parameters_[index].value.uint32_t;
}

inline float Device::getFieldMinimum(unsigned short index)
{
  return parameters_[index].min;
}

inline float Device::getFieldMaximum(unsigned short index)
{
  return parameters_[index].max;
}

inline float Device::getFieldDefault(unsigned short index)
{
  return parameters_[index].def;
}

inline parameter Device::getFieldAll(unsigned short index)
{
  return parameters_[index];
}

void Device::setFieldID(unsigned short index, unsigned short id)
{
  parameters_[index].id = id;
}

void Device::setFieldAccess(unsigned short index, unsigned char access)
{
  parameters_[index].access = access;
}

void Device::setFieldOperation(unsigned short index, unsigned char operation)
{
  parameters_[index].operation = operation;
}

void Device::setFieldPhysic(unsigned short index, unsigned char physic)
{
  parameters_[index].physic = physic;
}

void Device::setFieldValidity(unsigned short index, unsigned char validity)
{
  parameters_[index].validity = validity;
}

template<typename T> inline void Device::setFieldValue(const uint16_t &index, const T &value)
{
  parameters_[index].value.uint32_t = *(uint32_t*)&value;
}

void Device::setFieldMin(unsigned short index, float min)
{
  parameters_[index].min = min;
}

void Device::setFieldMax(unsigned short index, float max)
{
  parameters_[index].max = max;
}

void Device::setFieldDef(unsigned short index, float def)
{
  parameters_[index].def = def;
}

void Device::setFieldAll(unsigned short index, parameter param)
{
  parameters_[index] = param;
}

void Device::setFieldValueDef(unsigned short index)
{
  parameters_[index].value.float_t = parameters_[index].def;
}

float Device::applyCoef(float value, float coef)
{
  return value * coef;
}

float Device::applyUnit(float value, int physic, int unit)
{
  value = value * 1000000;
  value = value - (units[physic][unit][1]);
  value = value / (units[physic][unit][0]);
  value = value / 1000000;
  return ((value * 1000 - (units[physic][unit][1]))/(units[physic][unit][0])/1000);
}

unsigned short Device::getIndexAtId(unsigned short id)
{
  if ((id >= startAddrParams_) && (id < (startAddrParams_ + countParameters_))) {
    if (getFieldId(id - startAddrParams_) == id)
      return (id - startAddrParams_);
  }

  logDebug.add(WarningMsg, "Не найден параметр %d", id);
  return 0;
}

float Device::getValue(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  if (ok)
    *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValue(index);
}

uint32_t Device::getValueUint32(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  if (ok)
    *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValueUint32(index);
}

uint8_t Device::setValue(uint16_t id, float value, EventType eventType)
{
  uint16_t index = getIndexAtId(id);
  float oldValue = getFieldValue(index);
  EventCode code = getFieldCode(index);
  uint8_t units = getFieldPhysic(index);

  // Проверка на диапазон
  float min = getFieldMinimum(index);
  float max = getFieldMaximum(index);
  uint8_t check = checkRange(value, min, max, true);
  if((check != 0) && !isnan(value)) {
    novobusSlave.putMessageParams(id);
    return check;
  }

  setFieldValue(index, value);
  setFieldValidity(index, isnan(value) ? VALIDITY_ERROR : VALIDITY_OK);

  if ((value != oldValue) && !(isnan(value) && isnan(oldValue))) {
    // Сообщить контроллеру визуализации об обновлении параметра
    novobusSlave.putMessageParams(id);
    // Формирование сообщения в архив событий об изменении параметра
    if (code && (eventType != NoneType))
      logEvent.add(code, eventType, (EventId)id, oldValue, value, units);
  }

  if(isnan(value))
    return err_r;

  return ok_r;
}

uint8_t Device::setValue(uint16_t id, uint32_t value, EventType eventType)
{
  uint16_t index = getIndexAtId(id);
  uint32_t oldValue = getFieldValueUint32(index);
  EventCode code = getFieldCode(index);
  uint8_t units = getFieldPhysic(index);

  setFieldValue(index, value);
  setFieldValidity(index, (value == 0xFFFFFFFF) ? VALIDITY_ERROR : VALIDITY_OK);

  if (value != oldValue) {
    // Сообщить контроллеру визуализации об обновлении параметра
    novobusSlave.putMessageParams(id);
    // Формирование сообщения в архив событий об изменении параметра
    if (code && (eventType != NoneType))
      logEvent.add(code, eventType, (EventId)id, oldValue, value, units);
  }

  return 0;
}

uint8_t Device::setValue(uint16_t id, int value, EventType eventType)
{
  return setValue(id, (float)value, eventType);
}

uint8_t Device::setMin(uint16_t id, float min)
{
  uint16_t index = getIndexAtId(id);
  float oldMin = getFieldMinimum(index);

  setFieldMin(index, min);
  if (min != oldMin) {
    novobusSlave.putMessageParams(id, 7);   // 7 - ReadMinCommand
  }

  return 0;
}

uint8_t Device::setMax(uint16_t id, float max)
{
  uint16_t index = getIndexAtId(id);
  float oldMax = getFieldMaximum(index);

  setFieldMax(index, max);
  if (max != oldMax) {
    novobusSlave.putMessageParams(id, 8);   // 8 - ReadMaxCommand
  }

  return 0;
}

void Device::resetValue(uint16_t id)
{
  uint16_t index = getIndexAtId(id);
  float def = getFieldDefault(index);
  setValue(id, def);
}

uint8_t Device::getPhysic(unsigned short id)
{
  return getFieldPhysic(getIndexAtId(id));
}

uint8_t Device::getValidity(unsigned short id)
{
  return getFieldValidity(getIndexAtId(id));
}

void Device::setValidity(uint16_t id, uint8_t validity)
{
  setFieldValidity(getIndexAtId(id), validity);
}

float Device::getMin(uint16_t id)
{
  return getFieldMinimum(getIndexAtId(id));
}

float Device::getMax(uint16_t id)
{
  return getFieldMaximum(getIndexAtId(id));
}

void Device::updateValueTask()
{
  osEvent event;
  while (1) {
    osDelay(1);

    event = osMessageGet(getValueDeviceQId_, 0);
    if (event.status == osEventMessage)
      getNewValue(event.value.v);
  }
}

void Device::getNewValue(uint16_t)
{

}

void Device::calcParameters(uint16_t id)
{

}

uint8_t Device::setNewValue(uint16_t id, float value)
{
  return setValue(id, value);
}

uint8_t Device::setNewValue(uint16_t id, uint32_t value)
{
  return setValue(id, value);
}

uint8_t Device::setNewValue(uint16_t id, int value)
{
  return setValue(id, value);
}

StatusType Device::saveParameters()
{
  // Проверка на завершение работы DMA
  framReadData(startAddrParams_*4, (uint8_t *)buffer, 1*4);

  for (int i = 0; i < countParameters_; ++i) {
    buffer[i] = parameters_[i].value.float_t;
  }

  StatusType status = framWriteData(startAddrParams_*4,
                                    (uint8_t *)buffer,
                                    countParameters_*4);
  if (status == StatusError)
    asm("nop");
  return status;
}

StatusType Device::readParameters()
{
  StatusType status = framReadData(startAddrParams_*4,
                                   (uint8_t *)buffer,
                                   countParameters_*4);
  if (status == StatusError)
    asm("nop");

  for (int i = 0; i < countParameters_; ++i) {
    parameters_[i].value.float_t = buffer[i];
  }

  return status;
}

bool Device::isConnect()
{
  return false;
}

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

Device::Device(uint32_t startAddrParams, parameter *parameters, uint16_t countParameter)
  : startAddrParams_(startAddrParams)
  , parameters_(parameters)
  , countParameter_(countParameter)
{

}

Device::~Device()
{
  osThreadTerminate(updateValueThreadId_);
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
                     2 * configMINIMAL_STACK_SIZE // Размер стека задачи
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
  return (value * units[physic][unit][0] + units[physic][unit][1]);
}

unsigned short Device::getIndexAtId(unsigned short id)
{
  if ((id >= startAddrParams_) && (id < (startAddrParams_ + countParameter_))) {
    if (getFieldId(id - startAddrParams_) == id)
      return (id - startAddrParams_);
  }

  // TODO: Предупреждение о не найденом параметре

  return 0;
}

float Device::getValue(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValue(index);
}

uint32_t Device::getValueUint32(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValueUint32(index);
}

uint8_t Device::setValue(uint16_t id, float value)
{
  //TODO: Добавить проверки на корректность записываемых данных

  uint16_t index = getIndexAtId(id);
  float valueOld = getFieldValue(index);

  setFieldValue(index, value);

  // Сообщить контроллеру визуализации об обновлении параметра
  if (value != valueOld)
    novobusSlave.putMessageParams(id);
  return 0;
}

uint8_t Device::setValue(uint16_t id, uint32_t value)
{
  uint16_t index = getIndexAtId(id);
  uint32_t valueOld = getFieldValueUint32(index);

  setFieldValue(index, value);

  // Сообщить контроллеру визуализации об обновлении параметра
  if (value != valueOld)
    novobusSlave.putMessageParams(id);
  return 0;
}

uint8_t Device::setValue(uint16_t id, int value)
{
  return setValue(id, (float)value);
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

  for (int i = 0; i < countParameter_; ++i) {
    buffer[i] = parameters_[i].value.float_t;
  }

  StatusType status = framWriteData(startAddrParams_*4,
                                    (uint8_t *)buffer,
                                    countParameter_*4);
  if (status == StatusError)
    asm("nop");
  return status;
}

StatusType Device::readParameters()
{
  StatusType status = framReadData(startAddrParams_*4,
                                   (uint8_t *)buffer,
                                   countParameter_*4);
  if (status == StatusError)
    asm("nop");

  for (int i = 0; i < countParameter_; ++i) {
    parameters_[i].value.float_t = buffer[i];
  }

  return status;
}

bool Device::isConnect()
{
  return false;
}

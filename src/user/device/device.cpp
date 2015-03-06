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

float Units[28][6][2] =
{
  /// PHYSIC_ERROR Ошибка типа 0
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_NUMERIC Перечислимый тип 1
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_TIME Время 2
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_PERCENT Проценты 3
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_LENGHT Длина 4
  {
    {(float)LENGTH_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_DENSITY Плотность 5
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_SPACE     Площадь 6
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FREQUENCY Частота 7
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_VOLTAGE /// Напряжение 8
  {
    {VOLTAGE_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_CURRENT Ток 9
  {
    {CURRENT_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACTIVE_POWER Активная мощность 10
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FULL_POWER Полная мощность 11
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_REACTIVE_POWER Реактивная мощность 12
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ANGLE Угол 13
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RPM Скорость вращения 14
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_TEMP Темп (разгона или замедления) 15
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RESIST_ISOLATION Сопротивление изоляции 16
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RESIST_CABLE_LINE Сопротивление кабельной линии 17
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_PRESSURE Давление 18
  {
    {PRESSURE_LAST, 0.0},
    {1.0, 0.0},                            /// PRESSURE_MPA
    {0.1013250, 0.0},                    /// PRESSURE_ATM
    {0.0980665, 0.0},                    /// PRESSURE_AT
    {0.1000000, 0.0},                    /// PRESSURE_BAR
    {0.0068948, 0.0}                    /// PRESSURE_PSI

  },
  /// PHYSIC_TEMPERATURE Температура 19
  {
    {TEMPERATURE_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACCELERATION Ускорение 20
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_SPEED Cкорость 21
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_VISCOSITY Вязкость 22
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FLOW /// Расход 23
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_POWER Мощность 24
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACTIVE_ENERGY Активная энергия 25
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_REACTIVE_ENERGY Реактивная энергия 26
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FULL_ENERGY Полная энергия 27
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  }
};


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
  // TODO Auto-generated destructor stub
}

void Device::createThread(const char *threadName)
{
  osMessageQDef(GetValueDeviceQueue, 100, uint32_t);
  getValueDeviceQueue_ = osMessageCreate(osMessageQ(GetValueDeviceQueue), NULL);

  // Заполняем структуру для создания задачи
  osThreadDef_t t = {threadName,                  // Название задачи
                     deviceUpdateValueTask,       // Указатель на функцию задачи
                     osPriorityNormal,            // Приоритет задачи
                     0,                           //
                     2 * configMINIMAL_STACK_SIZE // Размер стека задачи
                    };
  updateValueThreadId_ = osThreadCreate(&t, this);// Создаём задачу
}

unsigned short Device::getFieldId(unsigned short index)
{
  return parameters_[index].id;
}

unsigned char Device::getFieldAccess(unsigned short index)
{
  return parameters_[index].access;
}

unsigned char Device::getFieldOperation(unsigned short index)
{
  return parameters_[index].operation;
}

unsigned char Device::getFieldPhysic(unsigned short index)
{
  return parameters_[index].physic;
}

unsigned char Device::getFieldValidity(unsigned short index)
{
  return parameters_[index].validity;
}

float Device::getFieldValue(unsigned short index)
{
  return parameters_[index].value;
}

float Device::getFieldMinimum(unsigned short index)
{
  return parameters_[index].min;
}

float Device::getFieldMaximum(unsigned short index)
{
  return parameters_[index].max;
}

float Device::getFieldDefault(unsigned short index)
{
  return parameters_[index].def;
}

parameter Device::getFieldAll(unsigned short index)
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

void Device::setFieldValue(unsigned short index, float value)
{
  parameters_[index].value = value;
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

float Device::applyCoef(float value, float coef)
{
  return value * coef;
}

float Device::applyUnit(float value, int physic, int units)
{
  return (value * Units[physic][units][0] + Units[physic][units][1]);
}

unsigned short Device::getIndexAtID(unsigned short id)
{
  if (getFieldId(id - startAddrParams_) == id)
    return (id - startAddrParams_);

  // TODO: Предупреждение о не найденом параметре

  return 0;
}

float Device::getValue(unsigned short id)
{
  return getFieldValue(getIndexAtID(id));
}

unsigned char Device::setValue(unsigned short id, float value)
{
  //TODO: Добавить проверки на корректность записываемых данных

  uint16_t index = getIndexAtID(id);
  float valueOld = getFieldValue(index);

  setFieldValue(index, value);

  // Сообщить контроллеру визуализации об обновлении параметра
  if (value != valueOld)
    novobusSlave.putMessageParams(id);

  return 0;
}

uint8_t Device::getPhysic(unsigned short id)
{
  return getFieldPhysic(getIndexAtID(id));
}

void Device::updateValueTask()
{
  osEvent event;
  while (1) {
    osDelay(1);

    event = osMessageGet(getValueDeviceQueue_, 0);
    if (event.status == osEventMessage)
      getNewValue(event.value.v);
  }
}

void Device::getNewValue(uint16_t)
{

}

uint8_t Device::setNewValue(uint16_t id, float value)
{
  return 0;
}


StatusType Device::saveParameters()
{
  // Проверка на завершение работы DMA
  framReadData(startAddrParams_*4, (uint8_t *)buffer, 1*4);

  for (int i = 0; i < countParameter_; ++i) {
    buffer[i] = parameters_[i].value;
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
    parameters_[i].value = buffer[i];
  }

  return status;
}

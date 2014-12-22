/*
 * classDevice.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device.h"
#include "service.h"

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


static void ThreadUpdateParameters(void *p)
{
  (static_cast<Device*>(p))->updateParameters();
}

// Конструктор класса
Device::Device()
{

}

// Деструктор класса
Device::~Device()
{
  // TODO Auto-generated destructor stub
}

// Метод создания задач для FreeRTOS
void Device::createThread(const char *threadName)
{
  // Заполняем структуру для создания задачи
  osThreadDef_t t = {threadName,                  // Название задачи
                     ThreadUpdateParameters,     // Указатель на функцию задачи
                     osPriorityNormal,            // Приоритет задачи
                     0,                           //
                     2 * configMINIMAL_STACK_SIZE // Размер стека задачи
                    };
  // Создаём задачу
  threadUpdateParametersId_ = osThreadCreate(&t, this);
}

// Метод создания очереди обновленных параметров
void Device::createMessageUpdateParameters()
{
  osMessageQDef(MessageUpdateParameters, 100, uint32_t);
  messageUpdateParameters_ = osMessageCreate(osMessageQ(MessageUpdateParameters), NULL);
}

// Получить элемент очереди обновленных параметров
int Device::getMessageUpdateParameters()
{
  osEvent Event;
  Event = osMessageGet(messageUpdateParameters_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

//Метод получения ID параметра по индексу в массиве параметров
unsigned short Device::getFieldId(unsigned short index)
{
  return parameters_[index].id;
}

// Метод получения Access параметрв по индексу в массиве параметров
unsigned char Device::getFieldAccess(unsigned short index)
{
  return parameters_[index].access;
}

// Метод получения Operation параметра по индексу в массиве параметров
unsigned char Device::getFieldOperation(unsigned short index)
{
  return parameters_[index].operation;
}

// Метод получения Physic параметра по индексу в массиве параметров
unsigned char Device::getFieldPhysic(unsigned short index)
{
  return parameters_[index].physic;
}

// Метод получения Validity параметра по индексу в массиве параметров
unsigned char Device::getFieldValidity(unsigned short index)
{
  return parameters_[index].validity;
}

// Метод получения Update параметра по индексу в массиве параметров
unsigned char Device::getFieldUpdate(unsigned short index)
{
  return parameters_[index].update;
}

// Метод получения Value параметра по индексу в массиве параметров
float Device::getFieldValue(unsigned short index)
{
  return parameters_[index].value;
}

// Метод получения Minimum параметра по индексу в массиве параметров
float Device::getFieldMinimum(unsigned short index)
{
  return parameters_[index].min;
}

// Метод получения Maximum параметра по индексу в массиве параметров
float Device::getFieldMaximum(unsigned short index)
{
  return parameters_[index].max;
}

// Метод получения Default параметра по индексу в массиве параметров
float Device::getFieldDefault(unsigned short index)
{
  return parameters_[index].def;
}

// Метод получения All полей параметра по индексу в массиве параметров
parameter Device::getFieldAll(unsigned short index)
{
  return parameters_[index];
}

// Метод присвоения поля ID в массиве параметров по индексу
void Device::setFieldID(unsigned short index, unsigned short id)
{
  parameters_[index].id = id;
}

// Метод присвоения поля Access в массиве параметров по индексу
void Device::setFieldAccess(unsigned short index, unsigned char access)
{
  parameters_[index].access = access;
}

// Метод присвоения поля Operation в массиве параметров по индексу
void Device::setFieldOperation(unsigned short index, unsigned char operation)
{
  parameters_[index].operation = operation;
}

// Метод присвоения поля Physic в массиве параметров по индексу
void Device::setFieldPhysic(unsigned short index, unsigned char physic)
{
  parameters_[index].physic = physic;
}

// Метод присвоения поля Validity в массиве параметров по индексу
void Device::setFieldValidity(unsigned short index, unsigned char validity)
{
  parameters_[index].validity = validity;
}

// Метод присвоения поля Update в массиве параметров по индексу
void Device::setFieldUpdate(unsigned short index, unsigned char update)
{
  parameters_[index].update = update;
}

// Метод присвоения поля value в массиве параметров по индексу
void Device::setFieldValue(unsigned short index, float value)
{
  parameters_[index].value = value;
}

// Метод присвоения поля Min в массиве параметров по индексу
void Device::setFieldMin(unsigned short index, float min)
{
  parameters_[index].min = min;
}

void Device::setFieldDef(unsigned short index, float def)
{
  parameters_[index].def = def;
}

// Метод присвоения поля All в массиве параметров по индексу
void Device::setFieldAll(unsigned short index, parameter param)
{
  parameters_[index] = param;
}

// Метод поиска и получения индекса параметра с указанным ID
unsigned short Device::getIndexAtID(unsigned short ID)
{
  for (int i = 0; i < countParameter_; i++) {
    if (ID == getFieldId(i))
      return i;
  }
  return 0;
}

// Метод присвоения значения параметру
unsigned char Device::setValue(unsigned short id, float value)
{
  //TODO: Добавить проверки на корректность записываемых данных
  setFieldValue(getIndexAtID(id), value);
  return 0;
}

// Метод проверки и обновления параметров устройства
void Device::updateParameters(void)
{
  // Проверяем есть ли новые значения параметров от устройства
  int UpdateParamID = getMessageUpdateParameters();
  // По ID параметра находим параметр и обновляем значение
  if (UpdateParamID) {

  }
  else {
    return;
  }
}



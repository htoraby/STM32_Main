/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"

Vsd::Vsd() : Device(VSD_BEGIN)
{
  initParameters();
}

Vsd::~Vsd()
{
  // TODO Auto-generated destructor stub
}

int Vsd::setFrequency(float Frequency)
{
  int Result = RETURN_ERROR;
  return Result;
}

// Метод задания минимальной частоты двигателя
unsigned char Vsd::setMinFrequency(float lowLimitFrequency)
{
  unsigned char result = RETURN_ERROR;
  // Читаем уставку максимальная частота
  float highLimitFreq = getValue(VSD_HIGH_LIM_SPEED_MOTOR);
  // Если задаваемая минимальная частоты больше или равна максимальноу
  if (lowLimitFrequency >= highLimitFreq) {
    result = RETURN_ERROR;
  }
  else {
    // Записывае минимум частоты
    result = setValue(VSD_LOW_LIM_SPEED_MOTOR, lowLimitFrequency);
    // Если записали минимум частоты
    if (!result) {
      // Читаем уставку частоты
      float frequency = getValue(VSD_FREQUENCY);
      // Если частота меньше вновь заданной уставки
      if (frequency < lowLimitFrequency) {
        result = setValue(VSD_FREQUENCY, lowLimitFrequency);
      }
    }
  }
  return result;
}

// Метод задания максимальной частоты
unsigned char Vsd::setMaxFrequency(float highLimitFrequency)
{
  unsigned char result = RETURN_ERROR;
  // Читаем уставку минимальной частоты
  float lowLimitFreq = getValue(VSD_LOW_LIM_SPEED_MOTOR);
  // Если задаваемая максимальная частота меньше или равна минимальной
  if ((highLimitFrequency <= lowLimitFreq)||(lowLimitFreq <= 0)) {
    result = RETURN_ERROR;
  }
  else {
    // Записываем значение частоты
    result = setValue(VSD_HIGH_LIM_SPEED_MOTOR, highLimitFrequency);
    if (!result) {
      // Читаем уставку частоты
      float frequency = getValue(VSD_FREQUENCY);
      // Если частота больше вновь заданной уставки
      if (frequency > highLimitFrequency) {
        result = setValue(VSD_FREQUENCY, highLimitFrequency);
      }
    }
  }
  return result;
}

// Метод задания направления вращения
unsigned char Vsd::setRotation(unsigned char rotation)
{
  unsigned char result = RETURN_ERROR;
  if (rotation) {
    result = setReverseRotation();
  }
  else {
    result = setDirectRotation();
  }
  return result;
}

// Метод задания прямого направления вращения
unsigned char Vsd::setDirectRotation()
{
  unsigned char result = RETURN_ERROR;
  result = setValue(VSD_ROTATION, 0);
  if (!result)
    result = RETURN_OK;
  return result;
}


// Метод задания обратного направления вращения
unsigned char Vsd::setReverseRotation()
{
  unsigned char result = RETURN_ERROR;
  result = setValue(VSD_ROTATION, 1);
  if (!result)
    result = RETURN_OK;
  return result;
}

// Метод получения текущей частоты
float Vsd::getCurrentFreq()
{
  return getValue(VSD_FREQUENCY_NOW);
}

void Vsd::initParameters()
{
  parameters_ = parametersArray_;
  countParameter_ = sizeof(parametersArray_)/sizeof(parameter);

  // Пустой элемент массива
  parameters_[VSD_BEGIN - VSD_BEGIN].id                = VSD_BEGIN;
  parameters_[VSD_BEGIN - VSD_BEGIN].access            = ACCESS_ERROR;
  parameters_[VSD_BEGIN - VSD_BEGIN].operation         = OPERATION_ERROR;
  parameters_[VSD_BEGIN - VSD_BEGIN].physic            = PHYSIC_ERROR;
  parameters_[VSD_BEGIN - VSD_BEGIN].validity          = VALIDITY_ERROR;
  parameters_[VSD_BEGIN - VSD_BEGIN].update            = UPDATE_ERROR;
  parameters_[VSD_BEGIN - VSD_BEGIN].value         = 0.0;
  parameters_[VSD_BEGIN - VSD_BEGIN].min           = 0.0;
  parameters_[VSD_BEGIN - VSD_BEGIN].max           = 0.0;
  parameters_[VSD_BEGIN - VSD_BEGIN].def           = 0.0;
  // Индекс "массивного параметра"
  parameters_[VSD_INDEX - VSD_BEGIN].id                = VSD_INDEX;
  parameters_[VSD_INDEX - VSD_BEGIN].access            = ACCESS_OPERATOR;
  parameters_[VSD_INDEX - VSD_BEGIN].operation         = OPERATION_LIMITED;
  parameters_[VSD_INDEX - VSD_BEGIN].physic            = PHYSIC_NUMERIC;
  parameters_[VSD_INDEX - VSD_BEGIN].validity          = VALIDITY_ERROR;
  parameters_[VSD_INDEX - VSD_BEGIN].update            = UPDATE_ERROR;
  parameters_[VSD_INDEX - VSD_BEGIN].value         = 0.0;
  parameters_[VSD_INDEX - VSD_BEGIN].min           = 0.0;
  parameters_[VSD_INDEX - VSD_BEGIN].max           = 5.0;
  parameters_[VSD_INDEX - VSD_BEGIN].def           = 0.0;
  // Единицы измерения скорости
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].id           = VSD_UNIT_SPEED;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].access       = ACCESS_OPERATOR;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].operation    = OPERATION_LIMITED;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].physic       = PHYSIC_NUMERIC;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].validity     = VALIDITY_ERROR;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].update       = UPDATE_ERROR;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].min      = 0.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].max      = 2.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].def      = 0.0;
  // Режим конфигурирования
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].id          = VSD_CONFIG_MODE;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].operation   = OPERATION_WRITE;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].update      = UPDATE_ERROR;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].value   = 0.0;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].min     = 0.0;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].max     = 3.0;
  parameters_[VSD_CONFIG_MODE - VSD_BEGIN].def     = 0.0;
  // Принцип управления двигателем
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].id        = VSD_CONTROL_MOTOR;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].max   = 3.0;
  parameters_[VSD_CONTROL_MOTOR - VSD_BEGIN].def   = 0.0;
  // Характеристика момента нагрузки
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].id        = VSD_TORQUE_CHARACTERISTIC;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].max   = 2.0;
  parameters_[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].def   = 0.0;
  // Режим перегрузки
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].id        = VSD_OVERLOAD_MODE;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].max   = 1.0;
  parameters_[VSD_OVERLOAD_MODE - VSD_BEGIN].def   = 0.0;
  // Конструкция двигателя
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].id           = VSD_MOTOR_TYPE;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].access       = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].operation    = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].physic       = PHYSIC_NUMERIC;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].validity     = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].update       = UPDATE_ERROR;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].min      = 0.0;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].max      = 1.0;
  parameters_[VSD_MOTOR_TYPE - VSD_BEGIN].def      = 0.0;
  // Усиление демпфирования
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].id         = VSD_DAMPING_GANE;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].value      = 140.0;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].max    = 250.0;
  parameters_[VSD_DAMPING_GANE - VSD_BEGIN].def    = 140.0;
  // Время фильтрации при низкой скорости
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].id        = VSD_LOW_SPEED_FILTER_TIME;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].max   = 20.0;
  parameters_[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].def   = 0.0;
  // Время фильтрации при высокой скорости
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].id       = VSD_HIGH_SPEED_FILTER_TIME;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].max  = 20.0;
  parameters_[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].def  = 0.0;
  // Мощность двигателя
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].id          = VSD_MOTOR_POWER;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].operation   = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].physic      = PHYSIC_POWER;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].update      = UPDATE_ERROR;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].value   = 0.0;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].min     = 0.0;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].max     = 1200.0;
  parameters_[VSD_MOTOR_POWER - VSD_BEGIN].def     = 0.0;
  // Напряжение двигателя
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].id        = VSD_MOTOR_VOLTAGE;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].max   = 1000.0;
  parameters_[VSD_MOTOR_VOLTAGE - VSD_BEGIN].def   = 0.0;
  // Частота двигателя
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].id        = VSD_MOTOR_FREQUENCY;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].min   = 20.0;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].max   = 1000.0;
  parameters_[VSD_MOTOR_FREQUENCY - VSD_BEGIN].def   = 0.0;
  // Ток двигателя
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].id        = VSD_MOTOR_CURRENT;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].max   = 2000.0;
  parameters_[VSD_MOTOR_CURRENT - VSD_BEGIN].def   = 0.0;
  // Скорость двигателя
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].id        = VSD_MOTOR_SPEED;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].physic    = PHYSIC_RPM;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].value     = 10.0;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].min   = 10.0;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].max   = 6000.0;
  parameters_[VSD_MOTOR_SPEED - VSD_BEGIN].def   = 10.0;
  // Длительный номинальный момент двигателя
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].id        = VSD_RATE_TORQUE_MOTOR;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].physic    = PHYSIC_RATE;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].max   = 10000.0;
  parameters_[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].def   = 1.0;
  // Сопротивление статора
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].id        = VSD_RESISTANCE_STATOR;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].physic    = PHYSIC_RESISTANCE;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].max   = 1000000.0;
  parameters_[VSD_RESISTANCE_STATOR - VSD_BEGIN].def   = 0.0;
  // Индуктивности по оси D
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].id       = VSD_D_AXIS_INDUNSTANCE;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].operation= OPERATION_LIMITED;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].physic   = PHYSIC_INDUNCTANCE;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].max  = 1000.0;
  parameters_[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].def  = 0.0;
  // Число полюсов двигателя
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].id        = VSD_MOTOR_POLES;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].value     = 2.0;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].min   = 2.0;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].max   = 10.0;
  parameters_[VSD_MOTOR_POLES - VSD_BEGIN].def   = 2.0;
  // Danfoss 1-40 Используется в проекте
  parameters_[VSD_BACK_EMF - VSD_BEGIN].id         = VSD_BACK_EMF;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].physic     = PHYSIC_VOLTAGE;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].value      = 10.0;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].min    =  10.0;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].max    = 9000.0;
  parameters_[VSD_BACK_EMF - VSD_BEGIN].def    = 10.0;
  // Характеристика Uf - U
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].id        = VSD_UF_CHARACTERISTIC_U;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].max   = 5.0;
  parameters_[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].def   = 0.0;
  // Характеристика Uf - f
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].id        = VSD_UF_CHARACTERISTIC_F;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].max   = 5.0;
  parameters_[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].def   = 0.0;
  // Подавление резонанса %
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].id         = VSD_RESONANCE_REMOVE;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].value      = 100.0;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].max    = 1000.0;
  parameters_[VSD_RESONANCE_REMOVE - VSD_BEGIN].def    = 100.0;
  // Постоянная времени подавления
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].id         = VSD_RESONANCE_TIME;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].physic     = PHYSIC_TIME;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].value      = 5.0;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].max    = 1000000.0;
  parameters_[VSD_RESONANCE_TIME - VSD_BEGIN].def    = 5.0;
  // Минимальный ток при низкой скорости
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].id        = VSD_MIN_CURRENT_LOW_SPEED;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].value     = 100.0;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].max   = 1000.0;
  parameters_[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].def   = 100.0;
  // Danfoss 1-70 Не используется в проекте; только при конфигурировании
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].id        = VSD_PM_START_MODE;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].max   = 1.0;
  parameters_[VSD_PM_START_MODE - VSD_BEGIN].def   = 0.0;
  // Задержка запуска
  parameters_[VSD_START_DELAY - VSD_BEGIN].id        = VSD_START_DELAY;
  parameters_[VSD_START_DELAY - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_START_DELAY - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_START_DELAY - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_START_DELAY - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_START_DELAY - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_START_DELAY - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_START_DELAY - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_START_DELAY - VSD_BEGIN].max   = 25.5;
  parameters_[VSD_START_DELAY - VSD_BEGIN].def   = 0.0;
  // Функция запуска
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].id       = VSD_START_FUNCTION;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].value    = 2.0;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].max  = 7.0;
  parameters_[VSD_START_FUNCTION - VSD_BEGIN].def  = 2.0;
  // Функция при остановке
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].id        = VSD_STOP_FUNCTION;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_STOP_FUNCTION - VSD_BEGIN].def   = 0.0;
  // Минимальная скорость для функции при останове
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].id         = VSD_STOP_SPEED;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].physic     = PHYSIC_FREQUENCY;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].max    = 20.0;
  parameters_[VSD_STOP_SPEED - VSD_BEGIN].def    = 0.0;
  // Контроль термистора двигателя
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].id        = VSD_CONTROL_TERMISTOR_MTR;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].max   = 21.0;
  parameters_[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].def   = 0.0;
  // Вход термистора
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].id        = VSD_THERMISTOR_RESOURCE;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].def   = 0.0;
  // Начальный ток
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].id        = VSD_PARKING_CURRENT;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].physic    = PHYSIC_PERCENT;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].value     = 50.0;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].max   = 1000.0;
  parameters_[VSD_PARKING_CURRENT - VSD_BEGIN].def   = 50.0;
  // Длительность действия начального тока
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].id         = VSD_PARKING_TIME;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].physic     = PHYSIC_TIME;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].value      = 3.0;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].max    = 60.0;
  parameters_[VSD_PARKING_TIME - VSD_BEGIN].def    = 3.0;
  // Контроль перенапряжения
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].id        = VSD_OVERVOLTAGE_CONTROL;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].max   = 2.0;
  parameters_[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].def   = 0.0;
  // Минимальное задание
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].id        = VSD_MIN_REFERENCE;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].max   = 999999.99;
  parameters_[VSD_MIN_REFERENCE - VSD_BEGIN].def   = 0.0;
  // Максимальное задание
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].id        = VSD_MAX_REFERENCE;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].max   = 999999.99;
  parameters_[VSD_MAX_REFERENCE - VSD_BEGIN].def   = 0.0;
  // Фиксированная скорость
  parameters_[VSD_FREQUENCY - VSD_BEGIN].id        = VSD_FREQUENCY;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].max   = 999999.99;
  parameters_[VSD_FREQUENCY - VSD_BEGIN].def   = 0.0;
  // Ресурс задания 1
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].id        = VSD_RESOURCE_TASK_1;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].max   = 29.0;
  parameters_[VSD_RESOURCE_TASK_1 - VSD_BEGIN].def   = 0.0;
  // Ресурс задания 2
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].id        = VSD_RESOURCE_TASK_2;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].max   = 29.0;
  parameters_[VSD_RESOURCE_TASK_2 - VSD_BEGIN].def   = 0.0;
  // Ресурс задания 3
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].id        = VSD_RESOURCE_TASK_3;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].max   = 29.0;
  parameters_[VSD_RESOURCE_TASK_3 - VSD_BEGIN].def   = 0.0;
  // Тип изменения скорости
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].id        = VSD_TYPE_SPEED_CHANGE;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].max   = 2.0;
  parameters_[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].def   = 0.0;
  // Время разгона
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].id        = VSD_TIMER_DISPERSAL;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].value     = 0.01;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].min   = 0.01;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].max   = 3600.0;
  parameters_[VSD_TIMER_DISPERSAL - VSD_BEGIN].def   = 0.01;
  // Время замедления
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].id        = VSD_TIMER_DELAY;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].value     = 0.01;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].min   = 0.01;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].max   = 3600.0;
  parameters_[VSD_TIMER_DELAY - VSD_BEGIN].def   = 0.01;
  // Время разгона фиксированной скорости
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].id         = VSD_TIMER_DISP_FIX_SPEED;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].physic     = PHYSIC_TIME;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].value      = 0.01;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].min    = 0.01;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].max    = 3600.0;
  parameters_[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].def    = 0.01;
  // Время замедления фиксированной скорости
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].id        = VSD_TIMER_DELAY_FIX_SPEED;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].value     = 0.01;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].min   = 0.01;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].max   = 3600.0;
  parameters_[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].def   = 0.01;
  // Направления вращения
  parameters_[VSD_ROTATION - VSD_BEGIN].id         = VSD_ROTATION;
  parameters_[VSD_ROTATION - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_ROTATION - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_ROTATION - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_ROTATION - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_ROTATION - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_ROTATION - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_ROTATION - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_ROTATION - VSD_BEGIN].max    = 2.0;
  parameters_[VSD_ROTATION - VSD_BEGIN].def    = 0.0;
  // Нижний предел скорости
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].id        = VSD_LOW_LIM_SPEED_MOTOR;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].max   = 999999.99;
  parameters_[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].def   = 0.0;
  // Верхний передел скорости
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].id       = VSD_HIGH_LIM_SPEED_MOTOR;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].physic   = PHYSIC_FREQUENCY;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].max  = 999999.99;
  parameters_[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].def  = 0.0;
  // Ограничение момента
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].id         = VSD_TORQUE_LIMIT;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].physic     = PHYSIC_PERCENT;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].max    = 1000.0;
  parameters_[VSD_TORQUE_LIMIT - VSD_BEGIN].def    = 0.0;
  // Ограничение момента в режиме генератора
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].id         = VSD_TORQUE_LIMIT_GEN;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].physic     = PHYSIC_PERCENT;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].value      = 100.0;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].max    = 1000.0;
  parameters_[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].def    = 100.0;
  // Предел тока
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].id        = VSD_CURRENT_LIMIT;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].max   = 1000.0;
  parameters_[VSD_CURRENT_LIMIT - VSD_BEGIN].def   = 1.0;
  // Максимальная выходная частота
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].id         = VSD_MAX_OUTPUT_FREQUENCY;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].value      = 132.0;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].min    = 1.0;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].max    = 590.0;
  parameters_[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].def    = 132.0;
  // Реакция на ошибку обратной связи
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].id       = VSD_MTR_FEEDBACK_LOSS_FUNC;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].value    = 2.0;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].max  = 11.0;
  parameters_[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].def  = 2.0;
  // Реакция на ошибку задания частоты
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].id         = VSD_TRACK_ERROR_FUNCTION;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].max    = 3.0;
  parameters_[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].def    = 0.0;
  // Величина несоответствия частота
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].id        = VSD_TRACK_ERROR;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].physic    = PHYSIC_RPM;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].value     = 10.0;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].max   = 600.0;
  parameters_[VSD_TRACK_ERROR - VSD_BEGIN].def   = 10.0;
  // Величина несоответствия частоты
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].id        = VSD_TRACK_ERROR_TIMEOUT;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].max   = 60.0;
  parameters_[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].def   = 1.0;
  // Величина несоответствия частоты при разгоне
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].id        = VSD_TRACK_ERROR_RAMPING;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].physic    = PHYSIC_RPM;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].value     = 100.0;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].max   = 600.0;
  parameters_[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].def   = 100.0;
  // Задержка срабатывания при несоответствии частоты при разгоне
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].id        = VSD_TRACK_ERROR_RAMP_TIME;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].max   = 60.0;
  parameters_[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].def   = 1.0;
  // Tracking Error After Ramping Timeout
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].id       = VSD_TRACK_ERROR_AFTER_RAMP;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].value    = 5.0;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].max  = 60.0;
  parameters_[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].def  = 5.0;
  // Danfoss 4-50 Не используется в проекте; только при конфигурировании
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].id        = VSD_WARNING_CURRENT_LOW;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].max   = 1000000.0;
  parameters_[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].def   = 0.0;
  // Danfoss 4-51 Не используется в проекте; только при конфигурировании
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].id       = VSD_WARNING_CURRENT_HIGH;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].physic   = PHYSIC_CURRENT;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].max  = 1000000.0;
  parameters_[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].def  = 0.0;
  // Минимальная скорость
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].id        = VSD_WARNING_SPEED_LOW;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].physic    = PHYSIC_RPM;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].max   = 1000000.0;
  parameters_[VSD_WARNING_SPEED_LOW - VSD_BEGIN].def   = 0.0;
  // Режим клеммы 27
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].id         = VSD_TERMINAL_27_MODE;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].max    = 1.0;
  parameters_[VSD_TERMINAL_27_MODE - VSD_BEGIN].def    = 0.0;
  // Режим клеммы 29
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].id         = VSD_TERMINAL_29_MODE;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].max    = 1.0;
  parameters_[VSD_TERMINAL_29_MODE - VSD_BEGIN].def    = 0.0;
  // Цифровой вход 18
  parameters_[VSD_DI_18 - VSD_BEGIN].id        = VSD_DI_18;
  parameters_[VSD_DI_18 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DI_18 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DI_18 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DI_18 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DI_18 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DI_18 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DI_18 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DI_18 - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_DI_18 - VSD_BEGIN].def   = 0.0;
  // Цифровой вход 19
  parameters_[VSD_DI_19 - VSD_BEGIN].id        = VSD_DI_19;
  parameters_[VSD_DI_19 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DI_19 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DI_19 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DI_19 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DI_19 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DI_19 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DI_19 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DI_19 - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_DI_19 - VSD_BEGIN].def   = 0.0;
  // Цифровой вход 27
  parameters_[VSD_DI_27 - VSD_BEGIN].id        = VSD_DI_27;
  parameters_[VSD_DI_27 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DI_27 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DI_27 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DI_27 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DI_27 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DI_27 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DI_27 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DI_27 - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_DI_27 - VSD_BEGIN].def   = 0.0;
  // Цифровой вход 32
  parameters_[VSD_DI_32 - VSD_BEGIN].id        = VSD_DI_32;
  parameters_[VSD_DI_32 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DI_32 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DI_32 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DI_32 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DI_32 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DI_32 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DI_32 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DI_32 - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_DI_32 - VSD_BEGIN].def   = 0.0;
  // Цифровой вход 33
  parameters_[VSD_DI_33 - VSD_BEGIN].id        = VSD_DI_33;
  parameters_[VSD_DI_33 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DI_33 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DI_33 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DI_33 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DI_33 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DI_33 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DI_33 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DI_33 - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_DI_33 - VSD_BEGIN].def   = 0.0;
  // Клемма 27 Цифровой вход
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].id       = VSD_TERMINAL_27_DI;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].max  = 199.0;
  parameters_[VSD_TERMINAL_27_DI - VSD_BEGIN].def  = 0.0;
  // Клемма 29 Цифровой вход
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].id       = VSD_TERMINAL_29_DI;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].max  = 199.0;
  parameters_[VSD_TERMINAL_29_DI - VSD_BEGIN].def  = 0.0;
  // Реле функций
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].id        = VSD_FUNCTION_RELE;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].max   = 199.0;
  parameters_[VSD_FUNCTION_RELE - VSD_BEGIN].def   = 0.0;
  // Нипонятно
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].id       = VSD_ON_DELAY_RELAY;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].value    = 0.01;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].min  = 0.01;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].max  = 600.0;
  parameters_[VSD_ON_DELAY_RELAY - VSD_BEGIN].def  = 0.01;
  // Клемма 42 аналоговый вход
  parameters_[VSD_42_AO - VSD_BEGIN].id        = VSD_42_AO;
  parameters_[VSD_42_AO - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_42_AO - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_42_AO - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_42_AO - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_42_AO - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_42_AO - VSD_BEGIN].value     = 133.0;
  parameters_[VSD_42_AO - VSD_BEGIN].min   = 131.0;
  parameters_[VSD_42_AO - VSD_BEGIN].max   = 150.0;
  parameters_[VSD_42_AO - VSD_BEGIN].def   = 133.0;
  // Режим контроллера SL
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].id       = VSD_SL_CONTROLLER_MODE;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].max  = 1.0;
  parameters_[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].def  = 0.0;
  // Событие запуска
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].id       = VSD_SL_START_EVENT;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].max  = 101.0;
  parameters_[VSD_SL_START_EVENT - VSD_BEGIN].def  = 0.0;
  // Событие останова
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].id        = VSD_SL_STOP_EVENT;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_STOP_EVENT - VSD_BEGIN].def   = 0.0;
  // Сброс настроек компараторов
  parameters_[VSD_SL_RESET - VSD_BEGIN].id         = VSD_SL_RESET;
  parameters_[VSD_SL_RESET - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_SL_RESET - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_SL_RESET - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_SL_RESET - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_SL_RESET - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_SL_RESET - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_SL_RESET - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_SL_RESET - VSD_BEGIN].max    = 1.0;
  parameters_[VSD_SL_RESET - VSD_BEGIN].def    = 0.0;
  // Операнд сравнения компаратора
  parameters_[VSD_SL_10 - VSD_BEGIN].id        = VSD_SL_10;
  parameters_[VSD_SL_10 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_10 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_10 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_10 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_10 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_10 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_10 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_10 - VSD_BEGIN].max   = 199.0;
  parameters_[VSD_SL_10 - VSD_BEGIN].def   = 0.0;
  // Оператор сравнения компаратора
  parameters_[VSD_SL_11 - VSD_BEGIN].id        = VSD_SL_11;
  parameters_[VSD_SL_11 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_11 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_11 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_11 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_11 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_11 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_11 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_11 - VSD_BEGIN].max   = 8.0;
  parameters_[VSD_SL_11 - VSD_BEGIN].def   = 0.0;
  // Значение компаратора
  parameters_[VSD_SL_12 - VSD_BEGIN].id        = VSD_SL_12;
  parameters_[VSD_SL_12 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_12 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_12 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_12 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_12 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_12 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_12 - VSD_BEGIN].min   = -100000.0;
  parameters_[VSD_SL_12 - VSD_BEGIN].max   = 100000.0;
  parameters_[VSD_SL_12 - VSD_BEGIN].def   = 0.0;
  // RS-FF Operand S
  parameters_[VSD_SL_15 - VSD_BEGIN].id        = VSD_SL_15;
  parameters_[VSD_SL_15 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_15 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_15 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_15 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_15 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_15 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_15 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_15 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_15 - VSD_BEGIN].def   = 0.0;
  // RS-FF Operand R
  parameters_[VSD_SL_16 - VSD_BEGIN].id        = VSD_SL_16;
  parameters_[VSD_SL_16 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_16 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_16 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_16 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_16 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_16 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_16 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_16 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_16 - VSD_BEGIN].def   = 0.0;
  // Временная задержка
  parameters_[VSD_SL_20 - VSD_BEGIN].id        = VSD_SL_20;
  parameters_[VSD_SL_20 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_20 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_20 - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_SL_20 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_20 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_20 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_20 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_20 - VSD_BEGIN].max   = 1000000.0;
  parameters_[VSD_SL_20 - VSD_BEGIN].def   = 0.0;
  // Булева переменная логического соотношения 1
  parameters_[VSD_SL_40 - VSD_BEGIN].id        = VSD_SL_40;
  parameters_[VSD_SL_40 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_40 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_40 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_40 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_40 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_40 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_40 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_40 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_40 - VSD_BEGIN].def   = 0.0;
  // Оператор логического соотношения 1
  parameters_[VSD_SL_41 - VSD_BEGIN].id        = VSD_SL_41;
  parameters_[VSD_SL_41 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_41 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_41 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_41 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_41 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_41 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_41 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_41 - VSD_BEGIN].max   = 8.0;
  parameters_[VSD_SL_41 - VSD_BEGIN].def   = 0.0;
  // Булева переменная логического соотношения 2
  parameters_[VSD_SL_42 - VSD_BEGIN].id        = VSD_SL_42;
  parameters_[VSD_SL_42 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_42 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_42 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_42 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_42 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_42 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_42 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_42 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_42 - VSD_BEGIN].def   = 0.0;
  // Logic Rule Operator 2
  parameters_[VSD_SL_43 - VSD_BEGIN].id        = VSD_SL_43;
  parameters_[VSD_SL_43 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_43 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_43 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_43 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_43 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_43 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_43 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_43 - VSD_BEGIN].max   = 8.0;
  parameters_[VSD_SL_43 - VSD_BEGIN].def   = 0.0;
  // Logic Rule Boolean 3
  parameters_[VSD_SL_44 - VSD_BEGIN].id        = VSD_SL_44;
  parameters_[VSD_SL_44 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_44 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_44 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_44 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_44 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_44 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_44 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_44 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_44 - VSD_BEGIN].def   = 0.0;
  // Событие контроллера SL
  parameters_[VSD_SL_51 - VSD_BEGIN].id        = VSD_SL_51;
  parameters_[VSD_SL_51 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_51 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_51 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_51 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_51 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_51 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_51 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_51 - VSD_BEGIN].max   = 101.0;
  parameters_[VSD_SL_51 - VSD_BEGIN].def   = 0.0;
  // Действие контроллера SL
  parameters_[VSD_SL_52 - VSD_BEGIN].id        = VSD_SL_52;
  parameters_[VSD_SL_52 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SL_52 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SL_52 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_SL_52 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SL_52 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SL_52 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SL_52 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_SL_52 - VSD_BEGIN].max   = 74.0;
  parameters_[VSD_SL_52 - VSD_BEGIN].def   = 0.0;
  // Частота коммутации
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].id        = VSD_SWITCHING_FREQUENCY;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].value     = 1000.0;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].min   = 1000.0;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].max   = 16000.0;
  parameters_[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].def   = 1000.0;
  // Сверхмодуляция
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].id       = VSD_OVERMODULATION;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].value    = 1.0;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].max  = 1.0;
  parameters_[VSD_OVERMODULATION - VSD_BEGIN].def  = 1.0;
  // Компенсация задержки
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].id       = VSD_DEAD_TIME_COMPENSATION;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].max  = 1000000.0;
  parameters_[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].def  = 0.0;
  // Отказ питания
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].id        = VSD_MAINS_FAILURE;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].max   = 7.0;
  parameters_[VSD_MAINS_FAILURE - VSD_BEGIN].def   = 0.0;
  // Напряжение сети при отказе питания
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].id        = VSD_MAINS_VOLTAGE_FAILURE;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].value     = 180.0;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].min   = 180.0;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].max   = 600.0;
  parameters_[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].def   = 180.0;
  // Режим сброса
  parameters_[VSD_RESET_MODE - VSD_BEGIN].id       = VSD_RESET_MODE;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].max  = 14.0;
  parameters_[VSD_RESET_MODE - VSD_BEGIN].def  = 0.0;
  // Время автоматического перезапуск
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].id       = VSD_AUTOSTART_TIME;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].value    = 10.0;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].max  = 600.0;
  parameters_[VSD_AUTOSTART_TIME - VSD_BEGIN].def  = 10.0;
  // Задержка перед выключение при пределе тока
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].id        = VSD_DELAY_CURRENT_LIMIM;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].value     = 60.0;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].max   = 60.0;
  parameters_[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].def   = 60.0;
  // Задержка перед выключение при пределе моменте
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].id         = VSD_DELAY_TORQUE_LIMIT;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].physic     = PHYSIC_TIME;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].value      = 60.0;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].max    = 60.0;
  parameters_[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].def    = 60.0;
  // Время фильтра регулятора тока
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].id         = VSD_FIL_TIME_CURRENT_LIMIT;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].physic     = PHYSIC_TIME;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].value      = 1.0;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].min    = 1.000;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].max    = 2.0;
  parameters_[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].def    = 1.0;
  // Косинус Фи
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].id        = VSD_MOTOR_COS_PHI;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].value     = 0.95;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].min   = 0.4;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].max   = 0.95;
  parameters_[VSD_MOTOR_COS_PHI - VSD_BEGIN].def   = 0.95;
  // Компенсация напряжения постоянного тока
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].id        = VSD_DC_COMPENSATION;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].max   = 1.0;
  parameters_[VSD_DC_COMPENSATION - VSD_BEGIN].def   = 1.0;
  // Управление вентилятором
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].id        = VSD_FAN_CONTROL;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].max   = 4.0;
  parameters_[VSD_FAN_CONTROL - VSD_BEGIN].def   = 0.0;
  // Выходной фильтр
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].id         = VSD_OUT_FILTER;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].max    = 2.0;
  parameters_[VSD_OUT_FILTER - VSD_BEGIN].def    = 0.0;
  // Сброс аварий
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].id         = VSD_FAIL_RESET;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].max    = 3.0;
  parameters_[VSD_FAIL_RESET - VSD_BEGIN].def    = 0.0;
  // Версия ПО ЧРП
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].id       = VSD_SOFT_VERSION;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].operation= OPERATION_LIMITED;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].max  = 100000.0;
  parameters_[VSD_SOFT_VERSION - VSD_BEGIN].def  = 0.0;
  // Командное слово
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].id       = VSD_COMMAND_WORD;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].operation= OPERATION_LIMITED;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].max  = 65535.0;
  parameters_[VSD_COMMAND_WORD - VSD_BEGIN].def  = 0.0;
  // Слово состояния
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].id        = VSD_STATUS_WORD;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].operation = OPERATION_LIMITED;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].max   = 65535.0;
  parameters_[VSD_STATUS_WORD - VSD_BEGIN].def   = 0.0;
  // Активная мощность
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].id       = VSD_ACTIVE_POWER;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].physic   = PHYSIC_ACTIVE_POWER;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].max  = 10000.0;
  parameters_[VSD_ACTIVE_POWER - VSD_BEGIN].def  = 0.0;
  // Напряжение двигателя
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].id        = VSD_OUT_VOLTAGE_MOTOR;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].max   = 6000.0;
  parameters_[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].def   = 0.0;
  // Текущая частота двигателя
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].id        = VSD_FREQUENCY_NOW;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].max   = 6500.0;
  parameters_[VSD_FREQUENCY_NOW - VSD_BEGIN].def   = 0.0;
  // Выходной ток средний
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].id        = VSD_OUT_CURRENT_MOTOR;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].max   = 10000.0;
  parameters_[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].def   = 0.0;
  // Текущая скорость вращения двигателя
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].id        = VSD_SPEED_RPM_NOW;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].physic    = PHYSIC_RPM;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].min   = -30000.0;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].max   = 30000.0;
  parameters_[VSD_SPEED_RPM_NOW - VSD_BEGIN].def   = 0.0;
  // Напряжение цепи пост.тока
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].id       = VSD_DC_VOLTAGE;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].physic   = PHYSIC_VOLTAGE;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].max  = 10000.0;
  parameters_[VSD_DC_VOLTAGE - VSD_BEGIN].def  = 0.0;
  // Температура радиатора
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].id       = VSD_RADIATOR_TEMPERATURE;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].physic   = PHYSIC_TEMPERATURE;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].max  = 255.0;
  parameters_[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].def  = 0.0;
  // Температура платы управления
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].id        = VSD_CONTROL_TEMPERATURE;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].physic    = PHYSIC_TEMPERATURE;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].max   = 100.0;
  parameters_[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].def   = 0.0;
  // Ток двигателя 1
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].id        = VSD_OUT_CURRENT_PHASE_1;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].max   = 10000.0;
  parameters_[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].def   = 0.0;
  // Ток двигателя 2
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].id        = VSD_OUT_CURRENT_PHASE_2;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].max   = 10000.0;
  parameters_[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].def   = 0.0;
  // Ток двигателя 3
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].id        = VSD_OUT_CURRENT_PHASE_3;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].max   = 10000.0;
  parameters_[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].def   = 0.0;
  // Цифровоый вход ПЧ
  parameters_[VSD_DI_VSD - VSD_BEGIN].id         = VSD_DI_VSD;
  parameters_[VSD_DI_VSD - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_DI_VSD - VSD_BEGIN].operation  = OPERATION_LIMITED;
  parameters_[VSD_DI_VSD - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_DI_VSD - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_DI_VSD - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_DI_VSD - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_DI_VSD - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_DI_VSD - VSD_BEGIN].max    = 1023.0;
  parameters_[VSD_DI_VSD - VSD_BEGIN].def    = 0.0;
  // Номер последней аварии и предупреждения
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].id       = VSD_LAST_ALARM;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].max  = 65535.0;
  parameters_[VSD_LAST_ALARM - VSD_BEGIN].def  = 0.0;
  // Слово Аварии 1
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].id       = VSD_ALARM_WORD_1;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].max  = 4294967295.0;
  parameters_[VSD_ALARM_WORD_1 - VSD_BEGIN].def  = 0.0;
  // Слово Аварии 2
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].id       = VSD_ALARM_WORD_2;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].max  = 4294967295.0;
  parameters_[VSD_ALARM_WORD_2 - VSD_BEGIN].def  = 0.0;
  // Слово предупреждения 1
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].id       = VSD_WARNING_WORD_1;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].max  = 4294967295.0;
  parameters_[VSD_WARNING_WORD_1 - VSD_BEGIN].def  = 0.0;
  // Слово предупреждения 2
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].id       = VSD_WARNING_WORD_2;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].max  = 4294967295.0;
  parameters_[VSD_WARNING_WORD_2 - VSD_BEGIN].def  = 0.0;
  // Время применения высокого пускового момента
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].id       = VSD_HIGH_START_TORQUE_TIME;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].max  = 60.0;
  parameters_[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].def  = 0.0;
  // Пусковой ток в % от номинального
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].id        = VSD_HIGH_START_TORQUE_CURRENT;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].physic    = PHYSIC_PERCENT;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].value     = 100.0;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].max   = 200.0;
  parameters_[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].def   = 100.0;
  // Защита блокировки ротора
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].id        = VSD_LOCK_ROTOR_PROTECTION;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].max   = 1.0;
  parameters_[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].def   = 0.0;
  // Время задержки аварии блокировки ротора
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].id        = VSD_LOCK_ROTOR_TIME;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].min   = 0.05;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].max   = 1.0;
  parameters_[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].def   = 0.1;
  // Угол управления тиристорами; служебный регистр для отладки работы выпрямителя
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].id       = VSD_THYR_ANGLE_REFERENCE;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].physic   = PHYSIC_ANGLE;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].max  = 180.0;
  parameters_[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].def  = 0.0;
  // Статус платы выпрямителя. Регистр для внутреннего использования в ПЧ
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].id        = VSD_THYR_STATUS;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].max   = 253.0;
  parameters_[VSD_THYR_STATUS - VSD_BEGIN].def   = 0.0;
  // Регистр управления тиристорами; используется только контроллером ПЧ
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].id       = VSD_THYR_CONTROL;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].max  = 3.0;
  parameters_[VSD_THYR_CONTROL - VSD_BEGIN].def  = 0.0;
  // Напряжение защиты от КЗ инвертора.
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].id        = VSD_THYR_VOLT_SHORT_CUILT;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].max   = 100000.0;
  parameters_[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].def   = 0.0;
  // Маска состояния цифровых выходов.
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].id         = VSD_DOUTPUTS;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].operation  = OPERATION_READ;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].max    = 65535.0;
  parameters_[VSD_DOUTPUTS - VSD_BEGIN].def    = 0.0;
  // Маска состояния цифровых входов
  parameters_[VSD_DINPUTS - VSD_BEGIN].id        = VSD_DINPUTS;
  parameters_[VSD_DINPUTS - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DINPUTS - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_DINPUTS - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DINPUTS - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DINPUTS - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DINPUTS - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DINPUTS - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DINPUTS - VSD_BEGIN].max   = 65535.0;
  parameters_[VSD_DINPUTS - VSD_BEGIN].def   = 0.0;
  // Маска включения цифровых выходов
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].id        = VSD_DOUTPUTS_ENABLE;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].max   = 65535.0;
  parameters_[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].def   = 0.0;
  // Маска выключения цифровых выходов
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].id       = VSD_DOUTPUTS_DISABLE;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].max  = 65535.0;
  parameters_[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].def  = 0.0;
  // Температура включения главного вентилятора.
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].id        = VSD_VENT_ON_TEMPERATURE;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].physic    = PHYSIC_TEMPERATURE;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].max   = 120.0;
  parameters_[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].def   = 0.0;
  // Температура выключения главного вентилятора.
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].id       = VSD_VENT_OFF_TEMPERATURE;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].physic   = PHYSIC_TEMPERATURE;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].max  = 120.0;
  parameters_[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].def  = 0.0;
  // Период проверки температуры радиаторов
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].id        = VSD_VENT_PERIOD;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].value     = 3.0;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].min   = 3.0;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].max   = 60.0;
  parameters_[VSD_VENT_PERIOD - VSD_BEGIN].def   = 3.0;
  // Температура срабатывания тепловой защиты радиаторов
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].id       = VSD_VENT_TEMPERATURE_FAULT;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].physic   = PHYSIC_TEMPERATURE;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].value    = 80.0;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].min  = 80.0;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].max  = 110.0;
  parameters_[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].def  = 80.0;
  // Флаговый регистр управления ПЧ
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].id       = VSD_INVERTOR_CONTROL;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].max  = 1023.0;
  parameters_[VSD_INVERTOR_CONTROL - VSD_BEGIN].def  = 0.0;
  // Регистр состояния инвертора
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].id = VSD_INVERTOR_STATUS;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].physic = PHYSIC_NUMERIC;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].value = 0.0;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].max    = 65535.0;
  parameters_[VSD_INVERTOR_STATUS - VSD_BEGIN].def    = 0.0;
  // Уставка частоты "тревожного" режима
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].id = VSD_FREQUENCY_ERROR;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].physic = PHYSIC_FREQUENCY;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].value = 1.0;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].min    = 1.0;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].max    = 500.0;
  parameters_[VSD_FREQUENCY_ERROR - VSD_BEGIN].def    = 1.0;
  // Темп нарастания частоты.
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].id        = VSD_T_SPEEDUP;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].physic    = PHYSIC_TEMP;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].value     = 0.25;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].min   = 0.25;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].max   = 12.5;
  parameters_[VSD_T_SPEEDUP - VSD_BEGIN].def   = 0.25;
  // Темп Период снижения частоты
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].id        = VSD_T_SPEEDDOWN;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].physic    = PHYSIC_TEMP;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].value     = 0.25;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].min   = 0.25;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].max   = 12.5;
  parameters_[VSD_T_SPEEDDOWN - VSD_BEGIN].def   = 0.25;
  // Период снижения частоты при токоограничении.
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].id         = VSD_T_ILIMIT_SPEEDDOWN;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].physic     = PHYSIC_TEMP;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].value      = 0.25;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].min    = 0.25;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].max    = 12.5;
  parameters_[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].def    = 0.25;
  // Период снижения частоты при нехватке напряжения на инверторе
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].id       = VSD_T_ULOW_SPEEDDOWN;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].physic   = PHYSIC_TEMP;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].value    = 0.25;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].min  = 0.25;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].max  = 12.5;
  parameters_[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].def  = 0.25;
  // Тип текущего алгоритма управления (регулятора)
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].id        = VSD_CURRENT_REGULATOR;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_CURRENT_REGULATOR - VSD_BEGIN].def   = -1.0;
  // Напряжение форсировки
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].id       = VSD_UF_U_FORCE;
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].physic   = PHYSIC_VOLTAGE;
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_UF_U_FORCE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].max  = 50.0;
  parameters_[VSD_UNIT_SPEED - VSD_BEGIN].def  = 0.0;
  // Частота форсировки
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].id       = VSD_UF_F_FORCE;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].physic   = PHYSIC_FREQUENCY;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].max  = 50.0;
  parameters_[VSD_UF_F_FORCE - VSD_BEGIN].def  = 0.0;
  // Расчетное напряжение холостого хода
  parameters_[VSD_UF_UHH - VSD_BEGIN].id       = VSD_UF_UHH;
  parameters_[VSD_UF_UHH - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_UF_UHH - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_UF_UHH - VSD_BEGIN].physic   = PHYSIC_VOLTAGE;
  parameters_[VSD_UF_UHH - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_UF_UHH - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_UF_UHH - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_UF_UHH - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_UF_UHH - VSD_BEGIN].max  = 50.0;
  parameters_[VSD_UF_UHH - VSD_BEGIN].def  = 0.0;
  // Постоянная времени компенсации индуктивности
  parameters_[VSD_IFB_COMP - VSD_BEGIN].id       = VSD_IFB_COMP;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].max  = 16.0;
  parameters_[VSD_IFB_COMP - VSD_BEGIN].def  = 0.0;
  // Пропорциональный коэффициент ОС частотного контура регулирования
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].id       = VSD_VUOUT_PROP;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].max  = 20.0;
  parameters_[VSD_VUOUT_PROP - VSD_BEGIN].def  = 0.0;
  // Максимальный порог ограничения тока на выходе ПЧ; абс. вел
  parameters_[VSD_ILIMIT - VSD_BEGIN].id       = VSD_ILIMIT;
  parameters_[VSD_ILIMIT - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_ILIMIT - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_ILIMIT - VSD_BEGIN].physic   = PHYSIC_CURRENT;
  parameters_[VSD_ILIMIT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_ILIMIT - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_ILIMIT - VSD_BEGIN].value    = 8.25;
  parameters_[VSD_ILIMIT - VSD_BEGIN].min  = 8.25;
  parameters_[VSD_ILIMIT - VSD_BEGIN].max  = 707.52;
  parameters_[VSD_ILIMIT - VSD_BEGIN].def  = 8.25;
  // Интегральный коэффициент ОС частотного контура регулирования
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].id        = VSD_VUOUT_INTEG;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].max   = 10.0;
  parameters_[VSD_VUOUT_INTEG - VSD_BEGIN].def   = 0.0;
  // Коэффициент постоянной времени обратного перехода с вектора на U/f
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].id        = VSD_VTOUF_INTEG;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].max   = 14.0;
  parameters_[VSD_VTOUF_INTEG - VSD_BEGIN].def   = 0.0;
  // Текущее выходное напряжение ПЧ
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].id         = VSD_UOUT_RATED;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].operation  = OPERATION_READ;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].physic     = PHYSIC_VOLTAGE;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].value      = 9.48;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].min    = 9.48;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].max    = 660.0;
  parameters_[VSD_UOUT_RATED - VSD_BEGIN].def    = 9.48;
  // Суммарная индуктивность на выходе ПЧ; фазное значение
  parameters_[VSD_LOUT - VSD_BEGIN].id         = VSD_LOUT;
  parameters_[VSD_LOUT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_LOUT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_LOUT - VSD_BEGIN].physic     = PHYSIC_INDUNCTANCE;
  parameters_[VSD_LOUT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_LOUT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_LOUT - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_LOUT - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_LOUT - VSD_BEGIN].max    = 3906.0;
  parameters_[VSD_LOUT - VSD_BEGIN].def    = 0.0;
  // Режим раскачки: частота
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].id       = VSD_SW_STARTUP_FREQUENCY;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].physic   = PHYSIC_FREQUENCY;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].value    = 0.03;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].min  = 0.03;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].max  = 10.0;
  parameters_[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].def  = 0.03;
  // Режим раскачки: угол качания
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].id         = VSD_SW_STARTUP_ANGLE_OSC;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].physic     = PHYSIC_ANGLE;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].value      = 2.0;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].min    = 2.0;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].max    = 359.0;
  parameters_[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].def    = 2.0;
  // Режим раскачки: угол сдвига
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].id       = VSD_SW_STARTUP_ANGLE_SHIFT;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].operation= OPERATION_READ;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].value    = 1.0;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].min  = 1.0;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].max  = 179.0;
  parameters_[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].def  = 1.0;
  // Режим раскачки: количество качаний на угол
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].id         = VSD_SW_STARTUP_OSC_COUNT;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].value      = 1.0;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].min    = 1.0;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].max    = 100.0;
  parameters_[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].def    = 1.0;
  // Режим раскачки: количество оборотов двигателя
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].id         = VSD_SW_STARTUP_ROTATIONS;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].value      = 1.0;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].min    = 1.0;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].max    = 100.0;
  parameters_[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].def    = 1.0;
  // Режим раскачки: кратность напряжения импульса расклинки
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].id       = VSD_SW_STARTUP_U_PULSE;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].value    = 0.9375;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].min  = 0.9375;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].max  = 10.0;
  parameters_[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].def  = 0.9375;
  // Режим раскачки: токоограничение
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].id         = VSD_SW_STARTUP_I_LIM;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].physic     = PHYSIC_CURRENT;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].value      = 8.0;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].min    = 8.0;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].max    = 250.0;
  parameters_[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].def    = 8.0;
  // Интегральный коэффициент ОС фазового контура регулирования
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].id        = VSD_VFREQ_INTEG;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].max   = 11.0;
  parameters_[VSD_VFREQ_INTEG - VSD_BEGIN].def   = 1.0;
  // Регистры рабочей очереди алгоритмов управления
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].id        = VSD_REGULATOR_QUEUE_1;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].def   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].id        = VSD_REGULATOR_QUEUE_2;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].def   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].id        = VSD_REGULATOR_QUEUE_3;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].def   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].id        = VSD_REGULATOR_QUEUE_4;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].def   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].id        = VSD_REGULATOR_QUEUE_5;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].value     = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].min   = -1.0;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].max   = 6.0;
  parameters_[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].def   = -1.0;
  // Порог защиты ПЧ по снижению напряжения на инверторе
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].id         = VSD_UD_LOW_FAULT;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].physic     = PHYSIC_VOLTAGE;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].value      = 250.0;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].min    = 250.0;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].max    = 400.0;
  parameters_[VSD_UD_LOW_FAULT - VSD_BEGIN].def    = 250.0;
  // Порог защиты ПЧ по превыщению напряжения на инверторе
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].id        = VSD_UD_HIGH_FAULT;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].value     = 550.0;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].min   = 550.0;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].max   = 700.0;
  parameters_[VSD_UD_HIGH_FAULT - VSD_BEGIN].def   = 550.0;
  // Порог защиты ПЧ по превыщению напряжения на инверторе
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].id       = VSD_UIN_ASYM_LEVEL;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].physic   = PHYSIC_PERCENT;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].value    = 10.0;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].min  = 10.0;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].max  = 90.0;
  parameters_[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].def  = 10.0;
  // Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].id       = VSD_UIN_ASYM_DELAY;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].physic   = PHYSIC_TIME;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].value    = 1.0;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].min  = 1.0;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].max  = 90.0;
  parameters_[VSD_UIN_ASYM_DELAY - VSD_BEGIN].def  = 1.0;
  // Частота перехода на векторный алгоритм
  parameters_[VSD_F_VECT - VSD_BEGIN].id         = VSD_F_VECT;
  parameters_[VSD_F_VECT - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_F_VECT - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_F_VECT - VSD_BEGIN].physic     = PHYSIC_FREQUENCY;
  parameters_[VSD_F_VECT - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_F_VECT - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_F_VECT - VSD_BEGIN].value      = 13.0;
  parameters_[VSD_F_VECT - VSD_BEGIN].min    = 13.0;
  parameters_[VSD_F_VECT - VSD_BEGIN].max    = 500.0;
  parameters_[VSD_F_VECT - VSD_BEGIN].def    = 13.0;
  // Пропорциональный коэффициент ОС фазового контура регулирования
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].id         = VSD_VFREQ_GAIN;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].max    = 17.0;
  parameters_[VSD_VFREQ_GAIN - VSD_BEGIN].def    = 0.0;
  // Фазовый коэффициент усиления системы подавления резонанса
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].id        = VSD_VANGLE_GAIN;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].max   = 15.0;
  parameters_[VSD_VANGLE_GAIN - VSD_BEGIN].def   = 1.0;
  // Постоянная времени системы подавления резонанса
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].id        = VSD_RES_TIMECONST;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].value     = 0.001;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].min   = 0.001;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].max   = 0.25;
  parameters_[VSD_RES_TIMECONST - VSD_BEGIN].def   = 0.001;
  // Служебный регистр для отладки алгоритмов управления
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].id       = VSD_RES_IAMP_VALID;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].access   = ACCESS_OPERATOR;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].operation= OPERATION_WRITE;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].physic   = PHYSIC_NUMERIC;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].update   = UPDATE_ERROR;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].value    = 0.0;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].min  = 0.0;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].max  = 100000.0;
  parameters_[VSD_RES_IAMP_VALID - VSD_BEGIN].def  = 0.0;
  // Выходной ток ПЧ по фазе В. (амплитудное значение)
  parameters_[VSD_IB - VSD_BEGIN].id         = VSD_IB;
  parameters_[VSD_IB - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_IB - VSD_BEGIN].operation  = OPERATION_READ;
  parameters_[VSD_IB - VSD_BEGIN].physic     = PHYSIC_CURRENT;
  parameters_[VSD_IB - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_IB - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_IB - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_IB - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_IB - VSD_BEGIN].max    = 300.0;
  parameters_[VSD_IB - VSD_BEGIN].def    = 0.0;
  // Фаза выходного тока В
  parameters_[VSD_PHB - VSD_BEGIN].id        = VSD_PHB;
  parameters_[VSD_PHB - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_PHB - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_PHB - VSD_BEGIN].physic    = PHYSIC_ANGLE;
  parameters_[VSD_PHB - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_PHB - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_PHB - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_PHB - VSD_BEGIN].min   = -180.0;
  parameters_[VSD_PHB - VSD_BEGIN].max   = 180.0;
  parameters_[VSD_PHB - VSD_BEGIN].def   = 0.0;
  // Выходной ток ПЧ по фазе В. (амплитудное значение)
  parameters_[VSD_IC - VSD_BEGIN].id         = VSD_IC;
  parameters_[VSD_IC - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_IC - VSD_BEGIN].operation  = OPERATION_READ;
  parameters_[VSD_IC - VSD_BEGIN].physic     = PHYSIC_CURRENT;
  parameters_[VSD_IC - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_IC - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_IC - VSD_BEGIN].value      = 0.0;
  parameters_[VSD_IC - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_IC - VSD_BEGIN].max    = 300.0;
  parameters_[VSD_IC - VSD_BEGIN].def    = 0.0;
  // Фаза выходного тока В
  parameters_[VSD_PHC - VSD_BEGIN].id        = VSD_PHC;
  parameters_[VSD_PHC - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_PHC - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_PHC - VSD_BEGIN].physic    = PHYSIC_ANGLE;
  parameters_[VSD_PHC - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_PHC - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_PHC - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_PHC - VSD_BEGIN].min   = -180.0;
  parameters_[VSD_PHC - VSD_BEGIN].max   = 180.0;
  parameters_[VSD_PHC - VSD_BEGIN].def   = 0.0;
  // Порог токоограничения импульса расклинки в режиме с раскачкой
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].id        = VSD_SW_STARTUP_I_LIM_PULSE;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].physic    = PHYSIC_CURRENT;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].value     = 8.25;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].min   = 8.25;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].max   = 500.0;
  parameters_[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].def   = 8.25;
  // Время снижения уставки в режиме остановки турбинного вращения
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].id        = VSD_DECEL_SPEEDDOWN;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].physic    = PHYSIC_TIME;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].max   = 50.0;
  parameters_[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].def   = 1.0;
  // Напряжение остаточного напряжения на шине (окончания разряда)
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].id        = VSD_DECEL_VOLTAGE;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].physic    = PHYSIC_VOLTAGE;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].value     = 9.8;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].min   = 9.8;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].max   = 49.4;
  parameters_[VSD_DECEL_VOLTAGE - VSD_BEGIN].def   = 9.8;
  // Начальная частота режима остановки турбинного вращения (для АД)
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].id        = VSD_DECEL_F_MAX;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].physic    = PHYSIC_FREQUENCY;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].value     = 1.0;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].min   = 1.0;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].max   = 500.0;
  parameters_[VSD_DECEL_F_MAX - VSD_BEGIN].def   = 1.0;
  // Время секунды
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].id        = VSD_TIME_SECOND;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].max   = 59.0;
  parameters_[VSD_TIME_SECOND - VSD_BEGIN].def   = 0.0;
  // Время минуты
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].id        = VSD_TIME_MINUTE;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].value     = 0.0;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].min   = 0.0;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].max   = 59.0;
  parameters_[VSD_TIME_MINUTE - VSD_BEGIN].def   = 0.0;
  // Время часы
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].id          = VSD_TIME_HOUR;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].operation   = OPERATION_WRITE;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].update      = UPDATE_ERROR;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].value   = 0.0;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].min     = 0.0;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].max     = 23.0;
  parameters_[VSD_TIME_HOUR - VSD_BEGIN].def     = 0.0;
  // Время дни
  parameters_[VSD_TIME_DAY - VSD_BEGIN].id           = VSD_TIME_DAY;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].access       = ACCESS_OPERATOR;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].operation    = OPERATION_WRITE;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].physic       = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].validity     = VALIDITY_ERROR;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].update       = UPDATE_ERROR;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].value    = 1.0;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].min      = 1.0;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].max      = 31.0;
  parameters_[VSD_TIME_DAY - VSD_BEGIN].def      = 1.0;
  // Время месяцы
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].id         = VSD_TIME_MONTH;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].access     = ACCESS_OPERATOR;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].operation  = OPERATION_WRITE;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].physic     = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].validity   = VALIDITY_ERROR;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].update     = UPDATE_ERROR;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].value      = 1.0;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].min    = 1.0;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].max    = 12.0;
  parameters_[VSD_TIME_MONTH - VSD_BEGIN].def    = 1.0;
  // Время годы
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].id          = VSD_TIME_YEAR;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].operation   = OPERATION_WRITE;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].update      = UPDATE_ERROR;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].value   = 2010.0;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].min     = 2010.0;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].max     = 2030.0;
  parameters_[VSD_TIME_YEAR - VSD_BEGIN].def     = 2010.0;
  // Величина измерительного тока автонастройки
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].id          = VSD_AST_IMEAS;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].operation   = OPERATION_WRITE;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].physic      = PHYSIC_PERCENT;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].update      = UPDATE_ERROR;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].value   = 20.0;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].min     = 20.0;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].max     = 100.0;
  parameters_[VSD_AST_IMEAS - VSD_BEGIN].def     = 20.0;
  // Величина измерительного напряжения автонастройки
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].id        = VSD_AST_UMEAS;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].physic    = PHYSIC_PERCENT;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].update    = UPDATE_ERROR;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].value     = 20.0;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].min   = 20.0;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].max   = 60.0;
  parameters_[VSD_AST_UMEAS - VSD_BEGIN].def   = 20.0;
  // Измеренная величина выходной индуктивности автонастройки
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].id = VSD_AST_LOUT_1_0;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].physic = PHYSIC_INDUNCTANCE;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].value = 0.0;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].max    = 4.0;
  parameters_[VSD_AST_LOUT_1_0 - VSD_BEGIN].def    = 0.0;
  // Дополнительный регистр автонастройки; зарезервирован
  parameters_[VSD_AST_STANDING - VSD_BEGIN].id = VSD_AST_STANDING;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].physic = PHYSIC_INDUNCTANCE;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].value = 0.0;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].max    = 4.0;
  parameters_[VSD_AST_STANDING - VSD_BEGIN].def    = 0.0;
  // Расширенный регистр состояния инвертора
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].id = VSD_INVERTOR_EXT_STATUS;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].physic = PHYSIC_NUMERIC;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].value = 0.0;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].max    = 65535.0;
  parameters_[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].def    = 0.0;
  // Период экскурсий частоты режима встряхивания.
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].id = VSD_JARRING_PERIOD;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].physic = PHYSIC_TIME;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].min    = 60.0;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].max    = 3600.0;
  parameters_[VSD_JARRING_PERIOD - VSD_BEGIN].def    = 0.0;
  // Величина прибавки частоты вращения выше уставки в режиме встряхивания
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].id = VSD_JARRING_UPDFREQ;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].physic = PHYSIC_FREQUENCY;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].max    = 15.0;
  parameters_[VSD_JARRING_UPDFREQ - VSD_BEGIN].def    = 0.0;
  // Время прибавки частоты вращения выше уставки в режиме встряхивания
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].id = VSD_JARRING_UPTIME;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].physic = PHYSIC_TIME;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].max    = 50.0;
  parameters_[VSD_JARRING_UPTIME - VSD_BEGIN].def    = 0.0;
  // Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].id = VSD_JARRING_DOWNDFREQ;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].physic = PHYSIC_FREQUENCY;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].max    = 15.0;
  parameters_[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].def    = 0.0;
  // Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].id = VSD_JARRING_DOWNTIME;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].physic = PHYSIC_TIME;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].max    = 50.0;
  parameters_[VSD_JARRING_DOWNTIME - VSD_BEGIN].def    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].id = VSD_JARRING_MODE;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].physic = PHYSIC_NUMERIC;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].max    = 100000.0;
  parameters_[VSD_JARRING_MODE - VSD_BEGIN].def    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].id = VSD_JARRING_TICK_CNT;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].physic = PHYSIC_NUMERIC;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].max    = 100000.0;
  parameters_[VSD_JARRING_TICK_CNT - VSD_BEGIN].def    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].id = VSD_JARRING_SECOND_CNT;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].physic = PHYSIC_NUMERIC;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].value = 0.0;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].max    = 100000.0;
  parameters_[VSD_JARRING_SECOND_CNT - VSD_BEGIN].def    = 0.0;
  // Стартовая частота вращения двигателя в основном режиме регулирования
  parameters_[VSD_START_FREQ - VSD_BEGIN].id = VSD_START_FREQ;
  parameters_[VSD_START_FREQ - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_START_FREQ - VSD_BEGIN].operation = OPERATION_WRITE;
  parameters_[VSD_START_FREQ - VSD_BEGIN].physic = PHYSIC_FREQUENCY;
  parameters_[VSD_START_FREQ - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_START_FREQ - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_START_FREQ - VSD_BEGIN].value = 0.03;
  parameters_[VSD_START_FREQ - VSD_BEGIN].min    = 0.03;
  parameters_[VSD_START_FREQ - VSD_BEGIN].max    = 10.0;
  parameters_[VSD_START_FREQ - VSD_BEGIN].def    = 0.03;
  // Выходной ток ПЧ по фазе A. (амплитудное значение)
  parameters_[VSD_IA - VSD_BEGIN].id = VSD_IA;
  parameters_[VSD_IA - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_IA - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_IA - VSD_BEGIN].physic = PHYSIC_CURRENT;
  parameters_[VSD_IA - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_IA - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_IA - VSD_BEGIN].value = 0.0;
  parameters_[VSD_IA - VSD_BEGIN].min    = 0.0;
  parameters_[VSD_IA - VSD_BEGIN].max    = 300.0;
  parameters_[VSD_IA - VSD_BEGIN].def    = 0.0;
  // Фаза выходного тока A
  parameters_[VSD_PHA - VSD_BEGIN].id = VSD_PHA;
  parameters_[VSD_PHA - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_PHA - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_PHA - VSD_BEGIN].physic = PHYSIC_ANGLE;
  parameters_[VSD_PHA - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_PHA - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_PHA - VSD_BEGIN].value = 0.0;
  parameters_[VSD_PHA - VSD_BEGIN].min    = -180.0;
  parameters_[VSD_PHA - VSD_BEGIN].max    = 180.0;
  parameters_[VSD_PHA - VSD_BEGIN].def    = 0.0;
  // Усредненная фаза выходного тока ПЧ
  parameters_[VSD_PHOUT - VSD_BEGIN].id = VSD_PHOUT;
  parameters_[VSD_PHOUT - VSD_BEGIN].access = ACCESS_OPERATOR;
  parameters_[VSD_PHOUT - VSD_BEGIN].operation = OPERATION_READ;
  parameters_[VSD_PHOUT - VSD_BEGIN].physic = PHYSIC_ANGLE;
  parameters_[VSD_PHOUT - VSD_BEGIN].validity = VALIDITY_ERROR;
  parameters_[VSD_PHOUT - VSD_BEGIN].update = UPDATE_ERROR;
  parameters_[VSD_PHOUT - VSD_BEGIN].value = 0.0;
  parameters_[VSD_PHOUT - VSD_BEGIN].min    = -180.0;
  parameters_[VSD_PHOUT - VSD_BEGIN].max    = 180.0;
  parameters_[VSD_PHOUT - VSD_BEGIN].def    = 0.0;
}

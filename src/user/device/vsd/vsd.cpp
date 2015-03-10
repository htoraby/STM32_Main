/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"

Vsd::Vsd() : Device(VSD_BEGIN, parametersArray_, VSD_END - VSD_BEGIN)
{
  initParameters();
}

Vsd::~Vsd()
{
  // TODO Auto-generated destructor stub
}

void Vsd::initParameters()
{
  for (int index = 0; index < (VSD_END - VSD_BEGIN); index++) {
    setFieldID(index, index + VSD_BEGIN);
    setFieldAccess(index, ACCESS_ERROR);
    setFieldOperation(index, OPERATION_ERROR);
    setFieldPhysic(index, PHYSIC_ERROR);
    setFieldMin(index, 0.0);
    setFieldMax(index, 0.0);
    setFieldDef(index, 0.0);
    setFieldValidity(index, VALIDITY_ERROR);
    setFieldValue(index, 0.0);
  }
}

float Vsd::getCurrentFreq()
{
  return getValue(VSD_FREQUENCY_NOW);
}

int Vsd::setMotorType(float TypeMotor)
{
  return setValue(VSD_MOTOR_TYPE, TypeMotor);
}

int Vsd::setRotation(float value)
{
  return setValue(VSD_ROTATION, value);
}

int Vsd::setFrequency(float value)
{
  return setValue(VSD_FREQUENCY, value);
}

int Vsd::setMinFrequency(float value)
{
  // Если записываемое значение "Минимальной частоты" больше значения
  // "Максимальной частоты"
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR)) {
    return RETURN_ERROR;                    // Возвращаем ошибку
  }
  else {
    // Если установили новое значение в массив параметров
    if (!setValue(VSD_LOW_LIM_SPEED_MOTOR, value)) {
      // Меняем поле минимум для уставки "Максимальной частоты"
      setFieldMin(getIndexAtID(VSD_HIGH_LIM_SPEED_MOTOR), value);
      // Меняем поле минимум для уставки "Частота"
      setFieldMin(getIndexAtID(VSD_FREQUENCY), value);
      return RETURN_OK;
    }
    return RETURN_ERROR;
  }
}

int Vsd::setMaxFrequency(float value)
{
  // Если записываемое значение "Максимальной частоты" меньше значения
  // "Минимальной частоты"
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR)) {
    return RETURN_ERROR;                    // Возвращаем ошибку
  }
  else {
    // Если установили новое значение в массив параметров
    if (!setValue(VSD_HIGH_SPEED_FILTER_TIME, value)) {
      // Меняем поле максимум для уставки "Максимальной частоты"
      setFieldMax(getIndexAtID(VSD_LOW_LIM_SPEED_MOTOR), value);
      // Меняем поле максимум для уставки "Частота"
      setFieldMax(getIndexAtID(VSD_FREQUENCY), value);
      return RETURN_OK;
    }
    return RETURN_ERROR;
  }
}

int Vsd::setSpeedUp(float value)
{
  return setValue(VSD_T_SPEEDUP, value);
}

int Vsd::setSpeedDown(float value)
{
  return setValue(VSD_T_SPEEDDOWN, value);
}

int Vsd::setMotorControl(float value)
{
  return setValue(VSD_MOTOR_CONTROL, value);
}










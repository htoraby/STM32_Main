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

// Метод задания минимальной частоты двигателя
int Vsd::setMinFrequency(float value)
{
  int result = RETURN_ERROR;
  // Читаем уставку максимальная частота
  float highLimitFreq = getValue(VSD_HIGH_LIM_SPEED_MOTOR);
  // Если задаваемая минимальная частоты больше или равна максимальноу
  if (value >= highLimitFreq) {
    result = RETURN_ERROR;
  }
  else {
    // Записывае минимум частоты
    result = setValue(VSD_LOW_LIM_SPEED_MOTOR, value);
    // Если записали минимум частоты
    if (!result) {
      // Читаем уставку частоты
      float frequency = getValue(VSD_FREQUENCY);
      // Если частота меньше вновь заданной уставки
      if (frequency < value) {
        result = setValue(VSD_FREQUENCY, value);
      }
    }
  }
  return result;
}

// Метод задания максимальной частоты
int Vsd::setMaxFrequency(float value)
{
  int result = RETURN_ERROR;
  // Читаем уставку минимальной частоты
  float lowLimitFreq = getValue(VSD_LOW_LIM_SPEED_MOTOR);
  // Если задаваемая максимальная частота меньше или равна минимальной
  if ((value <= lowLimitFreq)||(lowLimitFreq <= 0)) {
    result = RETURN_ERROR;
  }
  else {
    // Записываем значение частоты
    result = setValue(VSD_HIGH_LIM_SPEED_MOTOR, value);
    if (!result) {
      // Читаем уставку частоты
      float frequency = getValue(VSD_FREQUENCY);
      // Если частота больше вновь заданной уставки
      if (frequency > value) {
        result = setValue(VSD_FREQUENCY, value);
      }
    }
  }
  return result;
}

// Метод задания направления вращения
int Vsd::setRotation(uint8_t value)
{
  int result = RETURN_ERROR;
  if (value) {
    result = setReverseRotation();
  }
  else {
    result = setDirectRotation();
  }
  return result;
}

// Метод задания прямого направления вращения
int Vsd::setDirectRotation()
{
  int result = RETURN_ERROR;
  result = setValue(VSD_ROTATION, 0);
  if (!result)
    result = RETURN_OK;
  return result;
}


// Метод задания обратного направления вращения
int Vsd::setReverseRotation()
{
  int result = RETURN_ERROR;
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

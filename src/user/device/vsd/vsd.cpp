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

// Задаваемые параметры двигателя
int Vsd::setMotorType(float value)
{
  return setValue(VSD_MOTOR_TYPE, (float)value);
}

int Vsd::setMotorNominalFreq(float freq)
{
  return setValue(VSD_MOTOR_FREQUENCY, freq);
}

int Vsd::setMotorNominalCurrent(float current)
{
  return setValue(VSD_MOTOR_CURRENT, current);
}

int Vsd::setMotorNominalVoltage(float voltage)
{
    return setValue(VSD_MOTOR_VOLTAGE, voltage);
}

// Задаваемые параметры ЧРП
int Vsd::setSwitchingFrequency(float value)
{
  return setValue(VSD_SWITCHING_FREQUENCY, value);
}

int Vsd::setCurrentLim(float value)
{
  return setValue(VSD_ILIMIT, value);
}

int Vsd::setSumInduct(float induct)
{
  return setValue(VSD_LOUT, induct);
}

// Задаваемые параметры работы
int Vsd::setFrequency(float value)
{
  return setValue(VSD_FREQUENCY, value);
}

int Vsd::setMinFrequency(float value)
{
  // Если записываемое значение "Минимальной частоты" больше значения
  // "Максимальной частоты"
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR)) {
    return err_r;                    // Возвращаем ошибку
  }
  else {
    // Если установили новое значение в массив параметров
    if (!setValue(VSD_LOW_LIM_SPEED_MOTOR, value)) {
      // Меняем поле минимум для уставки "Максимальной частоты"
      setFieldMin(getIndexAtId(VSD_HIGH_LIM_SPEED_MOTOR), value);
      // Меняем поле минимум для уставки "Частота"
      setFieldMin(getIndexAtId(VSD_FREQUENCY), value);
      return ok_r;
    }
    return err_r;
  }
}

int Vsd::setMaxFrequency(float value)
{
  // Если записываемое значение "Максимальной частоты" меньше значения
  // "Минимальной частоты"
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR)) {
    return err_r;                    // Возвращаем ошибку
  }
  else {
    // Если установили новое значение в массив параметров
    if (!setValue(VSD_HIGH_SPEED_FILTER_TIME, value)) {
      // Меняем поле максимум для уставки "Максимальной частоты"
      setFieldMax(getIndexAtId(VSD_LOW_LIM_SPEED_MOTOR), value);
      // Меняем поле максимум для уставки "Частота"
      setFieldMax(getIndexAtId(VSD_FREQUENCY), value);
      return ok_r;
    }
    return err_r;
  }
}

int Vsd::setTempSpeedUp(float value)
{
  if (!setValue(VSD_TEMP_SPEEDUP, value)) {
    if (!setValue(VSD_TIMER_DISPERSAL, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP))) {
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));
    }
  }
  return err_r;
}

int Vsd::setTempSpeedDown(float value)
{
  if (!setValue(VSD_TEMP_SPEEDDOWN, value)) {
    if (!setValue(VSD_TIMER_DELAY, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN))) {
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));
    }
  }
  return err_r;
}

int Vsd::setSpeedUp(float speedUp)
{
  return setValue(VSD_T_SPEEDUP, speedUp);
}

int Vsd::setSpeedDown(float speedDown)
{
  return setValue(VSD_T_SPEEDDOWN, speedDown);
}

int Vsd::setRotation(float value)
{
  return setValue(VSD_ROTATION, value);
}

int Vsd::setMotorControl(float value)
{
  return setValue(VSD_MOTOR_CONTROL, value);
}

// Читаемые параметры ЧРП
float Vsd::getCurrentFreq()
{
  return getValue(VSD_FREQUENCY_NOW);
}

float Vsd::getSetpointFreq()
{
  return getValue(VSD_FREQUENCY);
}

float Vsd::getNominalFreq()
{
  return getValue(VSD_MOTOR_FREQUENCY);
}

// Команды и операции
bool Vsd::checkVsdStatus(uint8_t bit)
{
  if (bit < VSD_STATUS_FC_I2T_ERR)
    return checkBit(getValue(VSD_INVERTOR_STATUS), bit);
  if ((bit >= VSD_STATUS_FC_I2T_ERR) && (bit < VSD_STATUS_3))
    return checkBit(getValue(VSD_INVERTOR_STATUS2), bit - 16);
  if ((bit >= VSD_FLT_IMAX) && (bit < VSD_FLT_TEST))
    return checkBit(getValue(VSD_INV_FAULT), bit - 16);
  if ((bit >= VSD_THYR_ABC_STATE) && (bit < VSD_THYR_ERR_SHORTCIRQUIT))
    return checkBit(getValue(VSD_THYR_CONTROL), bit - 16);
  else
    return false;
}

void Vsd::initParameters()
{
  for (int index = 0; index < (VSD_END - VSD_BEGIN); index++) {
    setFieldID(index, index + VSD_BEGIN);
    setFieldAccess(index, ACCESS_ERROR);
    setFieldOperation(index, OPERATION_ERROR);
    setFieldPhysic(index, PHYSIC_NUMERIC);
    setFieldMin(index, 0.0);
    setFieldMax(index, 0.0);
    setFieldDef(index, 0.0);
    setFieldValidity(index, VALIDITY_ERROR);
    setFieldValue(index, 0.0);
  }
}

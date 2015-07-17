/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"
#include "user_main.h"

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

int Vsd::resetBlock()
{
  return 0;
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
    if (!setValue(VSD_HIGH_LIM_SPEED_MOTOR, value)) {
      // Меняем поле максимум для уставки "Минимальной частоты"
      setFieldMax(getIndexAtId(VSD_LOW_LIM_SPEED_MOTOR), value);
      // Меняем поле максимум для уставки "Частота"
      setFieldMax(getIndexAtId(VSD_FREQUENCY), value);
      return ok_r;
    }
    return err_r;
  }
}

int Vsd::setUfU(uint16_t id, float value)
{
  return setValue(id, value);
}

int Vsd::setTimeSpeedUp(float value)
{
  if (!setValue(VSD_TIMER_DISPERSAL, value)) {        // Записали время разгона (с)
    if (!setValue(VSD_TEMP_SPEEDUP,                   // Записали темп разгона (Гц/с)
        getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DISPERSAL))) {
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));       // Записали время на 1Гц
    }
  }
  return err_r;
}

int Vsd::setTimeSpeedDown(float value)
{
  if (!setValue(VSD_TIMER_DELAY, value)) {            // Записали время торможения (с)
    if (!setValue(VSD_TEMP_SPEEDDOWN,                 // Записали темп торможения (Гц/с)
        getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DELAY))) {
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));   // Записали время на 1Гц
    }
  }
  return err_r;
}

int Vsd::setTempSpeedUp(float value)
{
  if (!setValue(VSD_TEMP_SPEEDUP, value)) {           // Записали темп разгона (Гц/с)
    if (!setValue(VSD_TIMER_DISPERSAL,                // Записали время разгона (с)
        getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP))) {
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));       // Записали время на 1Гц
    }
  }
  return err_r;
}

int Vsd::setTempSpeedDown(float value)
{
  if (!setValue(VSD_TEMP_SPEEDDOWN, value)) {             // Записали темп торможения (Гц/с)
    if (!setValue(VSD_TIMER_DELAY,                        // Записали время торможения (с)
        getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN))) {
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));   // Записали время на 1Гц
    }
  }
  return err_r;
}

int Vsd::calcVsdCos()
{
  float actPwr = parameters.get(VSD_POWER_ACTIVE);
  float fullPwr =  parameters.get(VSD_POWER_FULL);
  float cos = actPwr;
  if (fullPwr != 0) {
    cos = actPwr / fullPwr;
  }
  if (cos < 0)
    cos = cos * (-1);
  if (cos > 1)
    cos = 1;
  setValue(VSD_MOTOR_COS_PHI_NOW, cos);
  return parameters.get(VSD_MOTOR_COS_PHI_NOW);
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

int Vsd::start()
{
  return 0;
}

int Vsd::stop()
{
  return 0;
}

bool Vsd::checkStart()
{
  return 0;
}

bool Vsd::checkStop()
{
  return 0;
}

bool Vsd::isConnect()
{
  return false;
}

// Команды и операции
bool Vsd::checkVsdStatus(uint8_t bit)
{
  if (bit < VSD_STATUS_FC_I2T_ERR)
    return checkBit(getValue(VSD_INVERTOR_STATUS), bit);
  if ((bit >= VSD_STATUS_FC_I2T_ERR) && (bit < VSD_STATUS_3))
    return checkBit(getValue(VSD_INVERTOR_STATUS2), bit - 16);
  if ((bit >= VSD_STATUS_IMAX) && (bit < VSD_STATUS_TEST))
    return checkBit(getValue(VSD_INV_FAULT), bit - 32);
  if ((bit >= VSD_THYR_ABC_STATE) && (bit < VSD_THYR_ERR_SHORTCIRQUIT))
    return checkBit(getValue(VSD_THYR_CONTROL), bit - 48);
  if ((bit >= VSD_STATUS_READY) && (bit < VSD_STATUS_TURBINE))
    return checkBit(getValue(VSD_INVERTOR_STATUS4), bit - 64);
  else
    return false;
}


bool Vsd::setBitVsdStatus(uint8_t bit, bool flag)
{
  if (bit < VSD_STATUS_FC_I2T_ERR)
    setValue(VSD_INVERTOR_STATUS, setBit(getValue(VSD_INVERTOR_STATUS), bit, flag));
  if ((bit >= VSD_STATUS_FC_I2T_ERR) && (bit < VSD_STATUS_3))
    setValue(VSD_INVERTOR_STATUS2, setBit(getValue(VSD_INVERTOR_STATUS2), bit - 16, flag));
  if ((bit >= VSD_STATUS_IMAX) && (bit < VSD_STATUS_TEST))
    setValue(VSD_INV_FAULT, setBit(getValue(VSD_INV_FAULT), bit - 32, flag));
  if ((bit >= VSD_THYR_ABC_STATE) && (bit < VSD_THYR_ERR_SHORTCIRQUIT))
    setValue(VSD_INV_FAULT, setBit(getValue(VSD_THYR_CONTROL), bit - 48, flag));
  if ((bit >= VSD_STATUS_READY) && (bit < VSD_STATUS_TURBINE))
    setValue(VSD_INV_FAULT, setBit(getValue(VSD_INVERTOR_STATUS4), bit - 64, flag));
  else
    return false;
  return true;
}

void Vsd::processingRegimeRun()
{

}

int Vsd::onRegimePush()
{
  return 0;
}

int Vsd::offRegimePush()
{
  return 0;
}

int Vsd::onRegimeSwing()
{
  return 0;
}

int Vsd::onRegimeJarring()
{
  return 0;
}

int Vsd::offRegimeJarring()
{
  return 0;
}

int Vsd::onRegimePickup()
{
  return 0;
}

int Vsd::offRegimeSwing()
{
  return 0;
}

int Vsd::offRegimePickup()
{
  return 0;
}

int Vsd::onRegimeSkipFreq()
{
  return 0;
}

int Vsd::offRegimeSkipFreq()
{
  return 0;
}

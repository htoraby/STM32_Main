/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"
#include "user_main.h"

Vsd::Vsd()
  : Device(VSD_BEGIN, parametersArray_, VSD_END - VSD_BEGIN)
  , log_(NULL)
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
  if (!setValue(VSD_MOTOR_TYPE, (float)value)) {
    if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
      return setValue(VSD_MOTOR_CONTROL, (float)VSD_MOTOR_CONTROL_UF);
    }
    return ok_r;
  }
  return err_r;
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
  if (!setValue(VSD_FREQUENCY, value)) {
    writeToDevice(VSD_FREQUENCY, value);
    return ok_r;
  }
  return err_r;
}

int Vsd::setLimitsMaxFrequence(float value)
{
  if (!setValue(VSD_HIGH_LIM_SPEED_MOTOR, value)) { // Если записали максимум частоты
    setMax(VSD_LOW_LIM_SPEED_MOTOR, value);         // Меняем поле максимум для уставки "Минимальной частоты"
    setMax(VSD_FREQUENCY, value);                   // Меняем поле максимум для уставки "Частота"
    return ok_r;
  }
  return err_r;
}

int Vsd::setLimitsMinFrequence(float value)
{
  if (!setValue(VSD_LOW_LIM_SPEED_MOTOR, value)) {  // Если записали минимум частоты
    setMin(VSD_HIGH_LIM_SPEED_MOTOR, value);        // Меняем поле минимум для уставки "Максимальной частоты"
    setMin(VSD_FREQUENCY, value);                   // Меняем поле минимум для уставки "Частота"
    return ok_r;
  }
  return err_r;
}

int Vsd::setMinFrequency(float value)
{
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR)) {   // Присваиваемая минимальная частота больше максимальной
    return err_r;                                     // Возвращаем ошибку
  }
  else {
    if (!setLimitsMinFrequence(value)) {              // Записали минимум частоты
      writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, value);  // Записываем в устройство
      if (value > getValue(VSD_FREQUENCY))            // Если минимальная частота больше уставки
        return setFrequency(value);                   // Приравнием уставку минимальной частоте
      else
        return ok_r;
    }
    return err_r;
  }
}

int Vsd::setMaxFrequency(float value)
{
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR)) {
    return err_r;
  }
  else {
    if (!setLimitsMaxFrequence(value)) {
      writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, value);
      if (value < getValue(VSD_FREQUENCY))
        return setFrequency(value);
      else
        return ok_r;
    }
    return err_r;
  }
}

int Vsd::setTimeSpeedUp(float value)
{
  // Записали время разгона (с)
  if (!setValue(VSD_TIMER_DISPERSAL, value)) {
    writeToDevice(VSD_TIMER_DISPERSAL, value);
    // Записали темп разгона (Гц/с)
    if (!setValue(VSD_TEMP_SPEEDUP, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DISPERSAL))) {
      // Записали время на 1Гц
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));
    }
  }
  return err_r;
}

int Vsd::setTimeSpeedDown(float value)
{
  // Записали время торможения (с)
  if (!setValue(VSD_TIMER_DELAY, value)) {
    writeToDevice(VSD_TIMER_DELAY, value);
    // Записали темп торможения (Гц/с)
    if (!setValue(VSD_TEMP_SPEEDDOWN, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DELAY))) {
      // Записали время на 1Гц
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));
    }
  }
  return err_r;
}

int Vsd::setTempSpeedUp(float value)
{ // Записали темп разгона (Гц/с)
  if (!setValue(VSD_TEMP_SPEEDUP, value)) {
    writeToDevice(VSD_TEMP_SPEEDUP, value);
    // Записали время разгона (с)
    if (!setValue(VSD_TIMER_DISPERSAL, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP))) {
      // Записали время на 1Гц
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));
    }
  }
  return err_r;
}

int Vsd::setTempSpeedDown(float value)
{
  // Записали темп торможения (Гц/с)
  if (!setValue(VSD_TEMP_SPEEDDOWN, value)) {
    writeToDevice(VSD_TEMP_SPEEDDOWN, value);
    // Записали время торможения (с)
    if (!setValue(VSD_TIMER_DELAY, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN))) {
      // Записали время на 1Гц
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));
    }
  }
  return err_r;
}

int Vsd::setUfU(uint16_t id, float value)
{
  return setValue(id, value);
}

int Vsd::setUfU1(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_1, value);
}

int Vsd::setUfU2(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_2, value);
}

int Vsd::setUfU3(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_3, value);
}

int Vsd::setUfU4(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_4, value);
}

int Vsd::setUfU5(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_5, value);
}

int Vsd::setCoefVoltageInAB(float value)
{
  return setValue(VSD_COEF_VOLTAGE_IN_AB, value);
}

int Vsd::setCoefVoltageInBC(float value)
{
  return setValue(VSD_COEF_VOLTAGE_IN_BC, value);
}

int Vsd::setCoefVoltageInCA(float value)
{
  return setValue(VSD_COEF_VOLTAGE_IN_CA, value);
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

void Vsd::writeToDevice(int id, float value)
{
  setValue(id, value);
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

float Vsd::getTypeStop()
{
  return getValue(VSD_TYPE_STOP);
}

int Vsd::start()
{
  return 0;
}

int Vsd::stop(float type)
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

// Команды и операции
bool Vsd::checkStatusVsd(uint8_t bit)
{
  if (bit <= VSD_STATUS_RESERVED)
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_1), bit);

  if ((bit >= VSD_STATUS_FC_I2T_ERR) && (bit <= (VSD_STATUS_FC_I2T_ERR+15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_2), bit - 16);

  if ((bit >= VSD_STATUS_IMAX) && (bit <= (VSD_STATUS_IMAX + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_7), bit - 32);

  if ((bit >= VSD_STATUS_ABC_STATE) && (bit <= (VSD_STATUS_ABC_STATE + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_5), bit - 48);

  if ((bit >= VSD_STATUS_READY) && (bit <= (VSD_STATUS_READY + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_4), bit - 64);

  else
    return false;
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

int Vsd::resetSetpoints()
{
  return 0;
}

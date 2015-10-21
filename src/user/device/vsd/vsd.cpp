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

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
int Vsd::setMotorType(float value)
{
  return setValue(VSD_MOTOR_TYPE, value);
}

int Vsd::setMotorFrequency(float value)
{
  return setValue(VSD_MOTOR_FREQUENCY, value);
}

int Vsd::setMotorCurrent(float value)
{
  return setValue(VSD_MOTOR_CURRENT, value);
}

int Vsd::setMotorVoltage(float value)
{
  return setValue(VSD_MOTOR_VOLTAGE, value);
}

void Vsd::setLimitsMotor()
{

}

  // РЕЖИМЫ ПУСКА
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

int Vsd::offRegimeSwing()
{
  return 0;
}

int Vsd::onRegimePickup()
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

int Vsd::onRegimeAutoAdaptation()
{
  return 0;
}

int Vsd::offRegimeAutoAdaptation()
{
  return 0;
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
int Vsd::setVsdControl(float value)
{
  return setValue(VSD_MOTOR_CONTROL, value);
}

int Vsd::setRotation(float value)
{
  return setValue(VSD_ROTATION, value);
}

int Vsd::setMinFrequency(float value)
{
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR))     // Присваиваемая минимальная частота больше максимальной
    return err_r;                                     // Возвращаем ошибку
  else
    return setLimitsMinFrequence(value);              // Записали минимум частоты
}

int Vsd::setLimitsMinFrequence(float value)
{
  if (!setValue(VSD_LOW_LIM_SPEED_MOTOR, value)) {    // Если записали минимум частоты
    setMin(VSD_HIGH_LIM_SPEED_MOTOR, value);          // Меняем поле минимум для уставки "Максимальной частоты"
    setMin(VSD_FREQUENCY, value);                     // Меняем поле минимум для уставки "Частота"
    return ok_r;
  }
  return err_r;
}

int Vsd::setMaxFrequency(float value)
{
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR))
    return err_r;
  else
    return setLimitsMaxFrequence(value);
}

int Vsd::setLimitsMaxFrequence(float value)
{
  if (!setValue(VSD_HIGH_LIM_SPEED_MOTOR, value)) {   // Если записали максимум частоты
    setMax(VSD_LOW_LIM_SPEED_MOTOR, value);           // Меняем поле максимум для уставки "Минимальной частоты"
    setMax(VSD_FREQUENCY, value);                     // Меняем поле максимум для уставки "Частота"
    return ok_r;
  }
  return err_r;
}

int Vsd::setFrequency(float value)
{
  return setValue(VSD_FREQUENCY, value);
}

int Vsd::setTimeSpeedUp(float value)
{ 
  if (!setValue(VSD_TIMER_DISPERSAL, value)) {  
    if (!setValue(VSD_TEMP_SPEEDUP, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DISPERSAL))) {
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));
    }
  }
  return err_r;
}

int Vsd::setTimeSpeedDown(float value)
{
  if (!setValue(VSD_TIMER_DELAY, value)) {
    if (!setValue(VSD_TEMP_SPEEDDOWN, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DELAY))) {
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));
    }
  }
  return err_r;
}

int Vsd::setSwitchingFrequency(float value)
{
  return setValue(VSD_SWITCHING_FREQUENCY, value);
}

int Vsd::setSwitchingFrequencyCode(float value)
{
  if (!setValue(VSD_SWITCHING_FREQUENCY_CODE, value)) {
    switch((int)getValue(VSD_SWITCHING_FREQUENCY_CODE)) {
    case 0:   value = 1000;   break;
    case 1:   value = 1500;   break;
    case 2:   value = 2000;   break;
    case 3:   value = 2500;   break;
    case 4:   value = 3000;   break;
    case 5:   value = 3500;   break;
    case 6:   value = 4000;   break;
    case 7:   value = 5000;   break;
    case 8:   value = 6000;   break;
    case 9:   value = 7000;   break;
    case 10:  value = 8000;   break;
    case 11:  value = 10000;  break;
    case 12:  value = 12000;  break;
    case 13:  value = 14000;  break;
    case 14:  value = 16000;  break;
    default:  value = 2500;   break;
    }
    return setValue(VSD_SWITCHING_FREQUENCY, value);
  }
  return err_r;
}

int Vsd::setSwitchingFrequencyMode(float value)
{
  return setValue(VSD_SWITCHING_FREQUENCY_MODE, value);
}

int Vsd::setResonanceRemoveSource(float value)
{
  return setValue(VSD_RES_MODE, value);
}

// НАСТРОЙКА U/f
int Vsd::setUf_f1(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_1, value);
}

int Vsd::setUf_f2(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_2, value);
}

int Vsd::setUf_f3(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_3, value);
}

int Vsd::setUf_f4(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_4, value);
}

int Vsd::setUf_f5(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_5, value);
}

int Vsd::setUf_f6(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_F_6, value);
}

int Vsd::setUf_U1(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_1, value);
}

int Vsd::setUf_U2(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_2, value);
}

int Vsd::setUf_U3(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_3, value);
}

int Vsd::setUf_U4(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_4, value);
}

int Vsd::setUf_U5(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_5, value);
}

int Vsd::setUf_U6(float value)
{
  return setValue(VSD_UF_CHARACTERISTIC_U_6, value);
}

int Vsd::calcUfCharacteristicU(float value)
{
  float voltStep = parameters.get(CCS_BASE_VOLTAGE) / 6;
  setUf_U1(voltStep * 1);
  setUf_U2(voltStep * 2);
  setUf_U3(voltStep * 3);
  setUf_U4(voltStep * 4);
  setUf_U5(voltStep * 5);
  setUf_U6(voltStep * 6);
  return ok_r;
}

int Vsd::calcUfCharacteristicF(float value)
{
  float freqStep = parameters.get(CCS_BASE_FREQUENCY) / 6;
  setUf_f1(freqStep * 1);
  setUf_f2(freqStep * 2);
  setUf_f3(freqStep * 3);
  setUf_f4(freqStep * 4);
  setUf_f5(freqStep * 5);
  setUf_f6(freqStep * 6);
  return ok_r;
}

int Vsd::setCurrentLim(float value)
{
  return setValue(VSD_ILIMIT, value);
}

int Vsd::setSumInduct(float value)
{
  return setValue(VSD_LOUT, value);
}

int Vsd::resetBlock()
{
  return 0;
}

int Vsd::setUfU(uint16_t id, float value)
{
  return setValue(id, value);
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

  if ((bit >= VSD_STATUS_RIGHT_DIRECTION) && (bit <= (VSD_STATUS_RIGHT_DIRECTION + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_3), bit - 32);

  if ((bit >= VSD_STATUS_IMAX) && (bit <= (VSD_STATUS_IMAX + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_7), bit - 48);

  if ((bit >= VSD_STATUS_ABC_STATE) && (bit <= (VSD_STATUS_ABC_STATE + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_5), bit - 64);

  if ((bit >= VSD_STATUS_READY) && (bit <= (VSD_STATUS_READY + 15)))
    return checkBit(parameters.get(CCS_VSD_STATUS_WORD_4), bit - 80);

  else
    return false;
}

void Vsd::processingRegimeRun()
{

}

int Vsd::onRegimeJarring()
{
  return 0;
}

int Vsd::offRegimeJarring()
{
  return 0;
}

int Vsd::resetSetpoints()
{
  return 0;
}

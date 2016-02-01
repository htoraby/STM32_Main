/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"
#include "user_main.h"

//! Массив параметров устройства
static parameter parametersArray[VSD_END - VSD_BEGIN] __attribute__((section(".extmem")));

Vsd::Vsd()
  : Device(VSD_BEGIN, parametersArray, VSD_END - VSD_BEGIN)
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
  if (setValue(VSD_MOTOR_TYPE, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания типа двигателя (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorFrequency(float value)
{
  if (setValue(VSD_MOTOR_FREQUENCY, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания частоты двигателя (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorCurrent(float value)
{
  if (setValue(VSD_MOTOR_CURRENT, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания тока двигателя (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorVoltage(float value)
{
  if (setValue(VSD_MOTOR_VOLTAGE, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания напряжения двигателя (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorSpeed(float value)
{
  return setValue(VSD_MOTOR_SPEED, value);
}

int Vsd::setMotorPower(float value)
{
  return setValue(VSD_MOTOR_POWER, value);
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
  if (setValue(VSD_ROTATION, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания направления вращения (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMinFrequency(float value)
{
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR)) {   // Присваиваемая минимальная частота больше максимальной
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания минимальной частоты (value = %d)",
                 value);
#endif
    return err_r;                                     // Возвращаем ошибку
  }
  else
    return setLimitsMinFrequence(value);              // Записали минимум частоты
}

int Vsd::setLimitsMinFrequence(float value)
{
  if (!setValue(VSD_LOW_LIM_SPEED_MOTOR, value)) {    // Если записали минимум частоты
    setMin(VSD_HIGH_LIM_SPEED_MOTOR, value);          // Меняем поле минимум для уставки "Максимальной частоты"
    setMin(VSD_FREQUENCY, value);                     // Меняем поле минимум для уставки "Частота"
    parameters.setMin(CCS_RGM_ALTERNATION_FREQ_FREQ_1, value);
    parameters.setMin(CCS_RGM_ALTERNATION_FREQ_FREQ_2, value);
    parameters.setMin(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ, value);
    parameters.setMin(CCS_RGM_CHANGE_FREQ_END_FREQ, value);
    parameters.setMin(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ, value);
    parameters.setMin(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ, value);
    parameters.setMin(CCS_RGM_PUMP_GAS_SETPOINT, value);
    return ok_r;
  }
  return err_r;
}

int Vsd::setMaxFrequency(float value)
{
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания минимальной частоты (value = %d)",
                 value);
#endif
    return err_r;
  }
  else
    return setLimitsMaxFrequence(value);
}

int Vsd::setLimitsMaxFrequence(float value)
{
  if (!setValue(VSD_HIGH_LIM_SPEED_MOTOR, value)) {   // Если записали максимум частоты
    setMax(VSD_LOW_LIM_SPEED_MOTOR, value);           // Меняем поле максимум для уставки "Минимальной частоты"
    setMax(VSD_FREQUENCY, value);                     // Меняем поле максимум для уставки "Частота"
    parameters.setMax(CCS_RGM_ALTERNATION_FREQ_FREQ_1, value);
    parameters.setMax(CCS_RGM_ALTERNATION_FREQ_FREQ_2, value);
    parameters.setMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ, value);
    parameters.setMax(CCS_RGM_CHANGE_FREQ_END_FREQ, value);
    parameters.setMax(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ, value);
    parameters.setMax(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ, value);
    parameters.setMax(CCS_RGM_PUMP_GAS_SETPOINT, value);
    return ok_r;
  }
  return err_r;
}

int Vsd::setFrequency(float value)
{ 
  if (setValue(VSD_FREQUENCY, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания минимальной частоты (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setTimeSpeedUp(float value)
{ 
  if (!setValue(VSD_TIMER_DISPERSAL, value)) {  
    if (!setValue(VSD_TEMP_SPEEDUP, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DISPERSAL))) {
      return setValue(VSD_T_SPEEDUP, 1/getValue(VSD_TEMP_SPEEDUP));
    }
  }
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания времени разгона (value = %d)",
                 value);
#endif
  return err_r;
}

int Vsd::setTimeSpeedDown(float value)
{
  if (!setValue(VSD_TIMER_DELAY, value)) {
    if (!setValue(VSD_TEMP_SPEEDDOWN, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TIMER_DELAY))) {
      return setValue(VSD_T_SPEEDDOWN, 1/getValue(VSD_TEMP_SPEEDDOWN));
    }
  }
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания времени торможения (value = %d)",
                 value);
#endif
  return err_r;
}

int Vsd::setSwitchingFrequency(float value)
{
  if (setValue(VSD_SWITCHING_FREQUENCY, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания частоты ШИМ (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
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
    return setSwitchingFrequency(value);
  }
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания кода частоты ШИМ (value = %d)",
                 value);
#endif
  return err_r;
}

int Vsd::setSwitchingFrequencyMode(float value)
{
  if (setValue(VSD_SWITCHING_FREQUENCY_MODE, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания режима ШИМ (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setResonanceRemoveSource(float value)
{
  if (setValue(VSD_RES_MODE, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания источника подавления резонанса (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setOutFilter(float value)
{
  if (value)
    value++;
  return setValue(VSD_OUT_FILTER, value);
}



// НАСТРОЙКА U/f
int Vsd::setUf_f1(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_1, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f1 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;

}

int Vsd::setUf_f2(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_2, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f2 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;

}

int Vsd::setUf_f3(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_3, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f3 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;

}

int Vsd::setUf_f4(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_4, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f4 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_f5(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_5, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f5 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;

}

int Vsd::setUf_f6(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_6, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания f6 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U1(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_U_1, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U1 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U2(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_U_2, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U2 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U3(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_U_3, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U3 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U4(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_U_4, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U4 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U5(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_U_5, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U5 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setUf_U6(float value)
{ 
  if (setValue(VSD_UF_CHARACTERISTIC_U_6, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания U6 (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setDischarge(float value)
{
  return err_r;
}

int Vsd::calcUfCharacteristicU(float value)
{
  return err_r;
}

int Vsd::calcUfCharacteristicF()
{
  return err_r;
}

int Vsd::setCurrentLim(float value)
{
  return setValue(VSD_ILIMIT, value);
}

int Vsd::setSumInduct(float value)
{
  if (setValue(VSD_LOUT, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "ЧРП: Ошибка задания суммарной индуктивности (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::resetBlock()
{
  return 0;
}

int Vsd::setUfU(uint16_t id, float value)
{
  return setValue(id, value);
}

int Vsd::calcMotorCos()
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

float Vsd::calcVsdPowerFull()
{
  setValue(VSD_POWER_FULL,
           calcAverage3Values(getValue(VSD_CURRENT_OUT_PHASE_1),
                              getValue(VSD_CURRENT_OUT_PHASE_2),
                              getValue(VSD_CURRENT_OUT_PHASE_3)) *
                              getValue(VSD_OUT_VOLTAGE_MOTOR) * 1.73);
  return getValue(VSD_POWER_FULL);
}

float Vsd::calcCurrentDC()
{
  float pwr = getValue(VSD_POWER_ACTIVE);
  float volt = getValue(VSD_VOLTAGE_DC);
  if (volt > 0) {
    setValue(VSD_CURRENT_DC, pwr/volt);
  }
  return getValue(VSD_CURRENT_DC);
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

int Vsd::start(bool /*init*/)
{
  return 0;
}

int Vsd::stop(float /*type*/)
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

float Vsd::checkAlarmVsd()
{
  return 0;
}

bool Vsd::checkPreventVsd()
{
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

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
  logDebug.add(WarningMsg, "Vsd::setMotorType() (value = %d)", value);
  #endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorTypeProfile()
{
  return ok_r;
}

enReturns Vsd::setMotorFrequency(float value, EventType eventType)
{
  return (enReturns)setValue(VSD_MOTOR_FREQUENCY, value, eventType);
  // TODO: тут была функция задания максимума частоты в зависимости от типа двигателя, считаю её туда и надо перенести
}

int Vsd::setMotorCurrent(float value, EventType eventType)
{
  if (setValue(VSD_MOTOR_CURRENT, value, eventType)) {
    #if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setMotorCurrent() (value = %d)", value);
    #endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorVoltage(float value, EventType eventType)
{
  if (setValue(VSD_MOTOR_VOLTAGE, value, eventType)) {
    #if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setMotorVoltage() (value = %d)", value);
    #endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setMotorSpeed(float value, EventType eventType)
{
  return setValue(VSD_MOTOR_SPEED, value, eventType);
}

void Vsd::setLimitsMotor()
{

}

int Vsd::setMotorResistanceStator(float value)
{
  return setValue(VSD_RESISTANCE_STATOR, value);
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

int Vsd::onRegimeCurrentLimitation()
{
  return 0;
}

int Vsd::offRegimeCurrentLimitation()
{
  return 0;
}

int Vsd::configRegimeCurrentLimitation()
{
  return 0;
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
int Vsd::setVsdControl(float value)
{
  return setValue(VSD_MOTOR_CONTROL, value);
}

int Vsd::setMinFrequency(float value)
{
  if (value > getValue(VSD_HIGH_LIM_SPEED_MOTOR)) {   // Присваиваемая минимальная частота больше максимальной
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setMinFrequency() (value = %d)",
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
    if (value > parameters.getMax(CCS_RGM_ALTERNATION_FREQ_FREQ_1))
      parameters.setMin(CCS_RGM_ALTERNATION_FREQ_FREQ_1, value);
    if (value > parameters.getMax(CCS_RGM_ALTERNATION_FREQ_FREQ_2))
      parameters.setMin(CCS_RGM_ALTERNATION_FREQ_FREQ_2, value);
    if (value > parameters.getMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ))
      parameters.setMin(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ, value);
    if (value > parameters.getMax(CCS_RGM_CHANGE_FREQ_END_FREQ))
      parameters.setMin(CCS_RGM_CHANGE_FREQ_END_FREQ, value);
    if (value > parameters.getMax(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ))
      parameters.setMin(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ, value);
    if (value > parameters.getMax(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ))
      parameters.setMin(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ, value);
    if (value > parameters.getMax(CCS_RGM_PUMP_GAS_SETPOINT))
      parameters.setMin(CCS_RGM_PUMP_GAS_SETPOINT, value);
    if (value > parameters.getMax(CCS_RGM_JARRING_FREQ_1))
      parameters.setMin(CCS_RGM_JARRING_FREQ_1, value);
    if (value > parameters.getMax(CCS_RGM_JARRING_FREQ_2))
      parameters.setMin(CCS_RGM_JARRING_FREQ_2, value);

    return ok_r;
  }
  return err_r;
}

int Vsd::setMaxFrequency(float value)
{
  if (value < getValue(VSD_LOW_LIM_SPEED_MOTOR)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setMaxFrequency() (value = %d)",
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
    if (value < parameters.getMax(CCS_RGM_ALTERNATION_FREQ_FREQ_1))
      parameters.setMax(CCS_RGM_ALTERNATION_FREQ_FREQ_1, value);
    if (value < parameters.getMax(CCS_RGM_ALTERNATION_FREQ_FREQ_2))
      parameters.setMax(CCS_RGM_ALTERNATION_FREQ_FREQ_2, value);
    if (value < parameters.getMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ))
      parameters.setMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ, value);
    if (value < parameters.getMax(CCS_RGM_CHANGE_FREQ_END_FREQ))
      parameters.setMax(CCS_RGM_CHANGE_FREQ_END_FREQ, value);
    if (value < parameters.getMax(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ))
      parameters.setMax(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ, value);
    if (value < parameters.getMax(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ))
      parameters.setMax(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ, value);
    if (value < parameters.getMax(CCS_RGM_PUMP_GAS_SETPOINT))
      parameters.setMax(CCS_RGM_PUMP_GAS_SETPOINT, value);
    if (value < parameters.getMax(CCS_RGM_JARRING_FREQ_1))
      parameters.setMax(CCS_RGM_JARRING_FREQ_1, value);
    if (value < parameters.getMax(CCS_RGM_JARRING_FREQ_2))
      parameters.setMax(CCS_RGM_JARRING_FREQ_2, value); 
    return ok_r;
  }
  return err_r;
}

int Vsd::setFrequency(float value, EventType eventType)
{ 
  if (setValue(VSD_FREQUENCY, value, eventType)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setFrequency() (value = %d)", value);
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
    logDebug.add(WarningMsg, "Vsd::setTimeSpeedUp() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setTimeSpeedDown() (value = %d)",
                 value);
#endif
  return err_r;
}

int Vsd::setSwitchingFrequency(float value)
{
  if (setValue(VSD_SWITCHING_FREQUENCY, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setSwitchingFrequency() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setSwitchingFrequencyCode() (value = %d)",
                 value);
#endif
  return err_r;
}

int Vsd::setSwitchingFrequencyMode(float value)
{
  if (setValue(VSD_SWITCHING_FREQUENCY_MODE, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setSwitchingFrequencyMode() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setResonanceRemoveSource() (value = %d)",
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

void Vsd::resetAdaptationVector(uint16_t type)
{
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_TYPE, type);
  ksu.calcSystemInduct();
}

void Vsd::setAdaptationVector()
{
  return;
}

bool Vsd::checkSetAdaptationVector()
{
  return false;
}

bool Vsd::checkResetAdaptationVector()
{
  return false;
}

// НАСТРОЙКА U/f
int Vsd::setUf_f1(float value)
{
  if (setValue(VSD_UF_CHARACTERISTIC_F_1, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setUf_f1() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_f2() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_f3() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_f4() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_f5() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_f6() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U1() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U2() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U3() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U4() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U5() (value = %d)",
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
    logDebug.add(WarningMsg, "Vsd::setUf_U6() (value = %d)",
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

int Vsd::calcUfCharacteristicF(float value)
{
  return err_r;
}

uint16_t Vsd::findUfHiPoint(float freq)
{
  if (freq <= parameters.get(VSD_UF_CHARACTERISTIC_F_2))
    return VSD_UF_CHARACTERISTIC_U_2;
  if (freq <= parameters.get(VSD_UF_CHARACTERISTIC_F_3))
    return VSD_UF_CHARACTERISTIC_U_3;
  if (freq <= parameters.get(VSD_UF_CHARACTERISTIC_F_4))
    return VSD_UF_CHARACTERISTIC_U_4;
  if (freq <= parameters.get(VSD_UF_CHARACTERISTIC_F_5))
    return VSD_UF_CHARACTERISTIC_U_5;
  if (freq <= parameters.get(VSD_UF_CHARACTERISTIC_F_6))
    return VSD_UF_CHARACTERISTIC_U_6;
  return 0;
}

uint16_t Vsd::findUfLowPoint(uint16_t hiPoint)
{
  if (hiPoint == VSD_UF_CHARACTERISTIC_U_2)
    return VSD_UF_CHARACTERISTIC_U_1;
  if (hiPoint == VSD_UF_CHARACTERISTIC_U_3)
    return VSD_UF_CHARACTERISTIC_U_2;
  if (hiPoint == VSD_UF_CHARACTERISTIC_U_4)
    return VSD_UF_CHARACTERISTIC_U_3;
  if (hiPoint == VSD_UF_CHARACTERISTIC_U_5)
    return VSD_UF_CHARACTERISTIC_U_4;
  if (hiPoint == VSD_UF_CHARACTERISTIC_U_6)
    return VSD_UF_CHARACTERISTIC_U_5;
  return 0;
}

int Vsd::setCurrentLim(float value)
{
  return setValue(VSD_ILIMIT, value);
}

int Vsd::setSumInduct(float value)
{
  if (setValue(VSD_LOUT, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setSumInduct() (value = %d)",
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
  if (setValue(id, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setUfU() (id = %d, value = %d)",
                 id, value);
#endif
    return err_r;
  }
  return ok_r;
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

void Vsd::calcTempSpeedUp()
{
  setValue(VSD_TEMP_SPEEDUP, 1/getValue(VSD_T_SPEEDUP));
}

void Vsd::calcTempSpeedDown()
{
  setValue(VSD_TEMP_SPEEDDOWN, 1/getValue(VSD_T_SPEEDDOWN));
}

void Vsd::calcTimeSpeedUp()
{
  setValue(VSD_TIMER_DISPERSAL, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP));
}

void Vsd::calcTimeSpeedDown()
{
  setValue(VSD_TIMER_DELAY, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN));
}

void Vsd::setLimitsCcsParameters()
{

}

void Vsd::writeToDevice(int id, float value)
{
  setValue(id, value);
}

void Vsd::readInDevice(int id)
{
  return;
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

int Vsd::setProtOverloadMotorTripSetpoint(float value)
{
  if (setValue(VSD_M_IRMS, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setProtOverloadMotorTripSetpoint() (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setProtOverloadMotorActivDelay(float value)
{
  if (setValue(VSD_T_BLANK, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setProtOverloadMotorActivDelay() (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setProtOverloadMotorTripDelay(float value)
{
  if (setValue(VSD_M_TRMS, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setProtOverloadMotorTripDelay() (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
}

int Vsd::setProtCurrentMotorTripSetpoint(float value)
{
  if (setValue(VSD_CURRENT_LIMIT, value)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Vsd::setProtCurrentMotorTripSetpoint() (value = %d)",
                 value);
#endif
    return err_r;
  }
  return ok_r;
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

bool Vsd::isControl()
{
  return false;
}

bool Vsd::isSetPointFreq()
{
  return true;
}

float Vsd::checkAlarmVsd()
{
  return 0;
}

float Vsd::checkAlarmVsdUnderVoltage()
{
  return VSD_ALARM_NONE;
}

float Vsd::checkAlarmVsdOverVoltage()
{
  return VSD_ALARM_NONE;
}

float Vsd::checkAlarmVsdCurrentMotor()
{
  return VSD_ALARM_NONE;
}

float Vsd::checkAlarmVsdOverloadMotor()
{
  return VSD_ALARM_NONE;
}

bool Vsd::checkPreventVsd()
{
  return false;
}

float Vsd::checkWarningVsd()
{
  return 0;
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

int Vsd::onRegimeAutoOptimCurrent()
{
  return 0;
}

int Vsd::offRegimeAutoOptimCurrent()
{
  return 0;
}

int Vsd::resetSetpoints()
{
  return 0;
}

void Vsd::getConnect() {}

void Vsd::setConnect(stConnectQuality *counters)
{
  parameters.set(CCS_VSD_CONNECTION_PERCENT, counters->quality);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_TOTAL, counters->transmite);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_SUCCESS, counters->resive);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_TRASH, counters->trash);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_CRC, counters->crc);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_ERR, counters->error);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_LOST, counters->lost);
}

void Vsd::resetConnect()
{
  parameters.set(CCS_VSD_CONNECTION_PERCENT, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_TOTAL, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_SUCCESS, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_TRASH, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_CRC, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_ERR, 0);
  parameters.set(CCS_VSD_CONNECTION_PACKAGE_LOST, 0);
}

int Vsd::onProtConnect()
{
  return 0;
}

int Vsd::offProtConnect()
{
  return 0;
}

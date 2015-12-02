/*
 * vsd_novomet.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd_novomet.h"
#include "user_main.h"
#include "regime_main.h"
#include "regime_run_novomet.h"

VsdNovomet::VsdNovomet()
{
  regimeRun_ = new RegimeRunNovomet();
}

VsdNovomet::~VsdNovomet()
{
  delete regimeRun_;
  delete dm_;
}

void VsdNovomet::init()
{
  initModbusParameters();

  // Создание задачи обновления параметров
  createThread("UpdateParamsVsd");
  // Создание объекта протокола связи с утройством
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 57600, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);

  initParameters();
  readParameters();
  setLimitsMotor();
  //setLimitsMinFrequence(getValue(VSD_LOW_LIM_SPEED_MOTOR));
  //setLimitsMaxFrequence(getValue(VSD_HIGH_LIM_SPEED_MOTOR));
}

void VsdNovomet::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int i = 0; i < count; i++) {         // Цикл по карте регистров
    if ((modbusParameters_[i].freqExchange != EVERY_TIME) && (modbusParameters_[i].freqExchange != NOT_READ)) {
      modbusParameters_[i].freqExchange = modbusParameters_[i].freqExchange + i;
      modbusParameters_[i].cntExchange = modbusParameters_[i].freqExchange;
    }

    int id = dm_->getFieldID(i);
    if (id <= 0)
      continue;
    int indexArray = getIndexAtId(id);                                   // Получаем индекс параметра в банке параметров
    if (indexArray) {                                                    // Если нашли параметр
      setFieldAccess(indexArray, ACCESS_OPERATOR);                       // Уровень доступа оператор
      setFieldOperation(indexArray, dm_->getFieldOperation(i)); // Операции над параметром
      setFieldPhysic(indexArray, dm_->getFieldPhysic(i));       // Физический смысл
      float tempVal = dm_->getFieldMinimum(i);                  // Получаем минимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setMin(id, tempVal);
      tempVal = dm_->getFieldMaximum(i);                        // Получаем мaксимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setMax(id, tempVal);
      tempVal = dm_->getFieldDefault(i);                        // Получаем значение по умолчанию
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(i));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(i), dm_->getFieldUnit(i));
      setFieldDef(indexArray, tempVal);
      setFieldValidity(indexArray, dm_->getFieldValidity(i));  // Получили флаг валидности
      setFieldValue(indexArray, getFieldDefault(indexArray));           // Присвоили значение значению по умолчанию
    }
  }
}

bool VsdNovomet::isConnect()
{
  bool curConnect = dm_->isConnect();

  if (prevConnect_ && !curConnect) {
    int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
    for (int indexModbus = 0; indexModbus < count; indexModbus++) {         // Цикл по карте регистров
      int id = dm_->getFieldID(indexModbus);
      if (id <= 0)
        continue;
      float value = NAN;
      setValue(id, value);
    }
  }
  prevConnect_ = curConnect;

  return curConnect;
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
int VsdNovomet::setMotorType(float value)
{
  if (!Vsd::setMotorType(value)) {
    value = (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) ? VSD_CONTROL_ASYN_MOTOR : VSD_CONTROL_VENT_MOTOR;
    writeToDevice(VSD_CONTROL_WORD_1, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMotorType");
    return err_r;
  }
}

int VsdNovomet::setMotorFrequency(float value)
{
  if (!Vsd::setMotorFrequency(value)) {
    if (!setBaseFrequency(value))
    setMin(VSD_TIMER_DISPERSAL, value * 0.25);
    setMax(VSD_TIMER_DELAY, value * 12.5);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMotorFrequency");
    return err_r;
  }
}

int VsdNovomet::setMotorCurrent(float value)
{
  if (!Vsd::setMotorCurrent(value)) {

    value = value * parameters.get(CCS_COEF_TRANSFORMATION);
    writeToDevice(VSD_MOTOR_CURRENT, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMotorCurrent");
    return err_r;
  }
}

int VsdNovomet::setMotorVoltage(float value)
{
  if (!Vsd::setMotorVoltage(value)) {
    value = value / parameters.get(CCS_COEF_TRANSFORMATION);
    if (!setBaseVoltage(value))
      return ok_r;
    else
      return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMotorVoltage");
    return err_r;
  }
}

void VsdNovomet::setLimitsMotor()
{
  setMin(VSD_MOTOR_CURRENT, dm_->getFieldMinimum(dm_->getIndexAtId(VSD_MOTOR_CURRENT)) / parameters.get(CCS_COEF_TRANSFORMATION));
  setMax(VSD_MOTOR_CURRENT, dm_->getFieldMaximum(dm_->getIndexAtId(VSD_MOTOR_CURRENT)) / parameters.get(CCS_COEF_TRANSFORMATION));
  setMin(VSD_MOTOR_VOLTAGE, dm_->getFieldMinimum(dm_->getIndexAtId(VSD_BASE_VOLTAGE)) * parameters.get(CCS_COEF_TRANSFORMATION));
  setMax(VSD_MOTOR_VOLTAGE, dm_->getFieldMaximum(dm_->getIndexAtId(VSD_BASE_VOLTAGE)) * parameters.get(CCS_COEF_TRANSFORMATION));
}

// РЕЖИМЫ ПУСКА
// Вариант с попыткой перевода общих параметров к ЧРП Новомет
//int VsdNovomet::onRegimePush()
//{
//  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == Regime::OffAction)
//    return 1;

//  float freq = parameters.get(CCS_RGM_RUN_PUSH_FREQ);       // Получаем частоту толчков
//  float numPush = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);// Получаем количество толчков
//  float calcNumPush = 0;
//  uint16_t rotation = 1;
//  if (freq) {
//    calcNumPush = (8.0 / freq);                             // Количество толчков на указанной частоте
//  }                                                         // За один оборот двигателя
//  else {
//    calcNumPush = numPush;
//  }
//  rotation = (uint16_t)((numPush / calcNumPush) + 0.5);     // Вычисляем требуемое количество оборотов

//  float impulse = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE); // Превышение напряжения
//  impulse = (uint16_t)((impulse - 100) / 10.0 + 0.5);       // Вычислили уставку напряжения
//  if ((impulse < 1.0) || (impulse > 10))
//    impulse = 1.0;

//  setNewValue(VSD_START_FREQ, freq);                        // Задаём частоту основного режима
//  setNewValue(VSD_SW_STARTUP_FREQUENCY, freq);              // Записали в ЧРП частоту
//  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, 180.0);             // Угол константа
//  setNewValue(VSD_SW_STARTUP_OSC_COUNT, 1.0);               // Количество качаний константа
//  setNewValue(VSD_SW_STARTUP_ROTATIONS, rotation);          // Записали количество оборотов
//  setNewValue(VSD_SW_STARTUP_U_PULSE, impulse);             // Записали кратность импульса толчка
//  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, 1500.0);   // Записали предел тока
//}

int VsdNovomet::onRegimePush()
{
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == Regime::OffAction)
    return 1;
  setNewValue(VSD_SW_STARTUP_FREQUENCY, parameters.get(CCS_SW_STARTUP_FREQUENCY_PUSH));
  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, parameters.get(CCS_SW_STARTUP_ANGLE_OSC_PUSH));
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, 1.0);
  setNewValue(VSD_SW_STARTUP_ROTATIONS, parameters.get(CCS_SW_STARTUP_ROTATIONS_PUSH));
  setNewValue(VSD_SW_STARTUP_U_PULSE, parameters.get(CCS_SW_STARTUP_U_PULSE_PUSH));
  setNewValue(VSD_SW_STARTUP_I_LIM, parameters.get(CCS_SW_STARTUP_I_LIM_PUSH));
  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, parameters.get(CCS_SW_STARTUP_I_LIM_PULSE_PUSH));
}

int VsdNovomet::offRegimePush()
{
  return 0;
}
// Вариант с попыткой перевода общих параметров к ЧРП Новомет
//int VsdNovomet::onRegimeSwing()
//{
//  if (parameters.get(CCS_RGM_RUN_SWING_MODE) == Regime::OffAction)
//    return 1;

//  float freq = parameters.get(CCS_RGM_RUN_SWING_FREQ);       // Получаем частоту толчков
//  float numPush = parameters.get(CCS_RGM_RUN_SWING_QUANTITY);// Получаем количество толчков
//  float calcNumPush = 0;
//  uint16_t rotation = 1;
//  if (freq) {
//    calcNumPush = (8.0 / freq);                             // Количество толчков на указанной частоте
//  }                                                         // За один оборот двигателя
//  else {
//    calcNumPush = numPush;
//  }
//  rotation = (uint16_t)((numPush / calcNumPush) + 0.5);     // Вычисляем требуемое количество оборотов

//  float impulse = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE); // Превышение напряжения
//  impulse = (uint16_t)((impulse - 100) / 10.0 + 0.5);       // Вычислили уставку напряжения
//  if ((impulse < 1.0) || (impulse > 10))
//    impulse = 1.0;

//  setNewValue(VSD_START_FREQ, freq);                        // Задаём частоту основного режима
//  setNewValue(VSD_SW_STARTUP_FREQUENCY, freq);              // Записали в ЧРП частоту
//  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, 359.0);             // Угол константа
//  setNewValue(VSD_SW_STARTUP_OSC_COUNT, 2.0);               // Количество качаний константа
//  setNewValue(VSD_SW_STARTUP_ROTATIONS, rotation);          // Записали количество оборотов
//  setNewValue(VSD_SW_STARTUP_U_PULSE, impulse);             // Записали кратность импульса толчка
//  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, 1500.0);   // Записали предел тока
//}

int VsdNovomet::onRegimeSwing()
{
  if (parameters.get(CCS_RGM_RUN_SWING_MODE) == Regime::OffAction)
    return 1;
  setNewValue(VSD_SW_STARTUP_FREQUENCY, parameters.get(CCS_SW_STARTUP_FREQUENCY_SWING));
  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, parameters.get(CCS_SW_STARTUP_ANGLE_OSC_SWING));
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, parameters.get(CCS_SW_STARTUP_OSC_COUNT_SWING));
  setNewValue(VSD_SW_STARTUP_ROTATIONS, parameters.get(CCS_SW_STARTUP_ROTATIONS_SWING));
  setNewValue(VSD_SW_STARTUP_U_PULSE, parameters.get(CCS_SW_STARTUP_U_PULSE_SWING));
  setNewValue(VSD_SW_STARTUP_I_LIM, parameters.get(CCS_SW_STARTUP_I_LIM_SWING));
  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, parameters.get(CCS_SW_STARTUP_I_LIM_PULSE_SWING));
}
int VsdNovomet::offRegimeSwing()
{
  return 0;
}

int VsdNovomet::onRegimePickup()
{
  if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_VENT) {
    writeToDevice(VSD_CONTROL_WORD_1, VSD_CONTROL_DISCHARGE_ON);
  }
  else {
    writeToDevice(VSD_CONTROL_WORD_1, VSD_CONTROL_DISCHARGE_OFF);
  }
  return 0;
}

int VsdNovomet::offRegimePickup()
{
  writeToDevice(VSD_CONTROL_WORD_1, VSD_CONTROL_DISCHARGE_OFF);
  return 0;
}

int VsdNovomet::onRegimeAutoAdaptation()
{
  return 0;
}

int VsdNovomet::offRegimeAutoAdaptation()
{
  return 0;
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
int VsdNovomet::setRotation(float value)
{
  if (!Vsd::setRotation(value)){
    value = (getValue(VSD_ROTATION) == VSD_ROTATION_DIRECT) ? VSD_CONTROL_RIGHT_DIRECTION : VSD_CONTROL_LEFT_DIRECTION;
    writeToDevice(VSD_CONTROL_WORD_1, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setRotation");
    return err_r;
  }
}

int VsdNovomet::setMinFrequency(float value)
{
  if (!Vsd::setMinFrequency(value)) {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMinFrequency");
    return err_r;
  }
}

int VsdNovomet::setMaxFrequency(float value)
{
  if (!Vsd::setMaxFrequency(value)) {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setMaxFrequency");
    return err_r;
  }
}

int VsdNovomet::setFrequency(float value)
{
  if (!Vsd::setFrequency(value)) {
    writeToDevice(VSD_FREQUENCY, getValue(VSD_FREQUENCY));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setFrequency");
    return err_r;
  }
}

int VsdNovomet::setTimeSpeedUp(float value)
{
  if (!Vsd::setTimeSpeedUp(value)) {
    writeToDevice(VSD_T_SPEEDUP, getValue(VSD_T_SPEEDUP));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setTimeSpeedUp");
    return err_r;
  }     
}

int VsdNovomet::setTimeSpeedDown(float value)
{
  if (!Vsd::setTimeSpeedDown(value)) {
    writeToDevice(VSD_T_SPEEDDOWN, getValue(VSD_T_SPEEDDOWN));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setTimeSpeedDown");
    return err_r;
  }
}

int VsdNovomet::setSwitchingFrequency(float value)
{
  if (!Vsd::setSwitchingFrequency(value)) {
    writeToDevice(VSD_SWITCHING_FREQUENCY, getValue(VSD_SWITCHING_FREQUENCY));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setSwitchingFrequency");
    return err_r;
  }
}

int VsdNovomet::setSwitchingFrequencyMode(float value)
{
  if (!Vsd::setSwitchingFrequencyMode(value)){
    switch ((uint16_t)getValue(VSD_SWITCHING_FREQUENCY_MODE)) {
    case VSD_SWITCHING_FREQUENCY_MODE_SIN:
      value = VSD_CONTROL_OVERPWM_OFF;
      break;
    case VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_1:
      value = VSD_CONTROL_OVERPWM1_ON;
      break;
    case VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_2:
      value = VSD_CONTROL_OVERPWM2_ON;
      break;
    }
    writeToDevice(VSD_CONTROL_WORD_1, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setSwitchingFrequencyMode");
    return err_r;
  }
}

int VsdNovomet::setResonanceRemoveSource(float value)
{
  if (!Vsd::setResonanceRemoveSource(value)){
    writeToDevice(VSD_CONTROL_WORD_2, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setResonanceRemoveSource");
    return err_r;
  }
}

int VsdNovomet::setSumInduct(float value)
{
  if (Vsd::setSumInduct(value)) {
    writeToDevice(VSD_LOUT, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setSumInduct");
    return err_r;
  }
}

int VsdNovomet::setBaseVoltage(float value)
{
  if (!setValue(VSD_BASE_VOLTAGE, value)) {
    writeToDevice(VSD_BASE_VOLTAGE, value);
    osDelay(1000);
    setMax(VSD_UF_CHARACTERISTIC_U, value);
    setMax(VSD_UF_CHARACTERISTIC_U_1, value);
    setMax(VSD_UF_CHARACTERISTIC_U_2, value);
    setMax(VSD_UF_CHARACTERISTIC_U_3, value);
    setMax(VSD_UF_CHARACTERISTIC_U_4, value);
    setMax(VSD_UF_CHARACTERISTIC_U_5, value);
    setMax(VSD_UF_CHARACTERISTIC_U_6, value);
    readUfCharacterictic();
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setBaseVoltage");
    return err_r;
  }
}

int VsdNovomet::setBaseFrequency(float value)
{
  if (!setValue(VSD_BASE_FREQUENCY, value)) {
    writeToDevice(VSD_BASE_FREQUENCY, value);
    value = (value * (parameters.get(VSD_MOTOR_VOLTAGE)/parameters.get(CCS_COEF_TRANSFORMATION)))/parameters.get(VSD_MOTOR_FREQUENCY);
    if (!setBaseVoltage(value)) {
      return ok_r;
    }
    else
      return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setBaseFrequency");
    return err_r;
  }
}

float VsdNovomet::checkAlarmVsd()
{
  float i = 0;
  float vsdStatus = parameters.get(CCS_VSD_ALARM_CODE);
  float vsdStatus1 = getValue(VSD_STATUS_WORD_1);
  float vsdStatus2 = getValue(VSD_STATUS_WORD_2);
  float vsdStatus5 = getValue(VSD_STATUS_WORD_5);
  float vsdStatus7 = getValue(VSD_STATUS_WORD_7);

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_I_LIMIT) && (vsdStatus <= VSD_NOVOMET_ALARM_ULOW))) {
    vsdStatus = 0;
    for (i = VSD_NOVOMET_ALARM_I_LIMIT; i <= VSD_NOVOMET_ALARM_ULOW; i++) {
      if (checkBit(vsdStatus1, i - 1000)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_UD_LOW_FAULT) && (vsdStatus <= VSD_NOVOMET_ALARM_UD_HIGH_FAULT))) {
    vsdStatus = 0;
    for (i = VSD_NOVOMET_ALARM_UD_LOW_FAULT; i <= VSD_NOVOMET_ALARM_UD_HIGH_FAULT; i++) {
      if (checkBit(vsdStatus1, i - 1000)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_UIN_ASYM) && (vsdStatus <= VSD_NOVOMET_ALARM_URECT_SHORT))) {
    vsdStatus = 0;
    for (i = VSD_NOVOMET_ALARM_UIN_ASYM; i <= VSD_NOVOMET_ALARM_URECT_SHORT; i++) {
      if (checkBit(vsdStatus1, i - 1000)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_FC_IT_ERR) && (vsdStatus <= VSD_NOVOMET_ALARM_I_LIMIT_FAST))) {
    vsdStatus = 0;
    for (i = VSD_NOVOMET_ALARM_FC_IT_ERR; i <= VSD_NOVOMET_ALARM_I_LIMIT_FAST; i++) {
      if (checkBit(vsdStatus2, i - 1016)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_DISCHARGE_ERR) && (vsdStatus <= VSD_NOVOMET_ALARM_M_I2T_ERR))) {
    vsdStatus = 0;
    for (i = VSD_NOVOMET_ALARM_DISCHARGE_ERR; i <= VSD_NOVOMET_ALARM_M_I2T_ERR; i++) {
      if (checkBit(vsdStatus2, i - 1016)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || (vsdStatus == VSD_NOVOMET_ALARM_ERR_STATE)) {
    if (checkBit(vsdStatus5, VSD_NOVOMET_ALARM_ERR_STATE - 1064)) {
      return VSD_NOVOMET_ALARM_ERR_STATE;
    }
  }

  if ((vsdStatus == 0) || (vsdStatus == VSD_NOVOMET_ALARM_ERR_SHORT)) {
    if (checkBit(vsdStatus5, VSD_NOVOMET_ALARM_ERR_SHORT - 1064)) {
      return VSD_NOVOMET_ALARM_ERR_SHORT;
    }
  }

  if ((vsdStatus == 0) || ((vsdStatus >= VSD_NOVOMET_ALARM_IMAX) && (vsdStatus <= VSD_NOVOMET_ALARM_TEMP))) {
    vsdStatus = 0;
    for (int i = VSD_NOVOMET_ALARM_IMAX; i <= VSD_NOVOMET_ALARM_TEMP; i++) {
      if (checkBit(vsdStatus7, i - 1096)) {
        return i;
      }
    }
  }

  if ((vsdStatus == 0) || (vsdStatus == VSD_NOVOMET_ALARM_ABC_STATE)||(vsdStatus == VSD_NOVOMET_ALARM_STOPPED_ALARM)) {
//    if (checkBit(vsdStatus5, VSD_NOVOMET_ALARM_ABC_STATE - 1064)) {
      return 0;/*VSD_NOVOMET_ALARM_ABC_STATE*/;
//    }
  }

  return vsdStatus;
}

bool VsdNovomet::checkPreventVsd()
{
  if (parameters.get(CCS_VSD_ALARM_CODE))
    return true;
  return false;
}

int VsdNovomet::calcUfCharacteristicU(float value)
{
  return setBaseVoltage(value);
}

int VsdNovomet::calcUfCharacteristicF(float value)
{
  return setBaseFrequency(value);
}

void VsdNovomet::readUfCharacterictic()
{
  readInDevice(VSD_UF_CHARACTERISTIC_F_1);
  readInDevice(VSD_UF_CHARACTERISTIC_F_2);
  readInDevice(VSD_UF_CHARACTERISTIC_F_3);
  readInDevice(VSD_UF_CHARACTERISTIC_F_4);
  readInDevice(VSD_UF_CHARACTERISTIC_F_5);
  readInDevice(VSD_UF_CHARACTERISTIC_F_6);
  readInDevice(VSD_UF_CHARACTERISTIC_U);
  readInDevice(VSD_UF_CHARACTERISTIC_U_1);
  readInDevice(VSD_UF_CHARACTERISTIC_U_2);
  readInDevice(VSD_UF_CHARACTERISTIC_U_3);
  readInDevice(VSD_UF_CHARACTERISTIC_U_4);
  readInDevice(VSD_UF_CHARACTERISTIC_U_5);
  readInDevice(VSD_UF_CHARACTERISTIC_U_6);
  readInDevice(VSD_IF_I_0);
  readInDevice(VSD_IF_I_1);
  readInDevice(VSD_IF_I_2);
  readInDevice(VSD_IF_I_3);
  readInDevice(VSD_IF_I_4);
  readInDevice(VSD_IF_I_5);
  readInDevice(VSD_IF_I_6);
}

// НАСТРОЙКА U/f
int VsdNovomet::setUf_f1(float value)
{
  if (!Vsd::setUf_f1(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_1, getValue(VSD_UF_CHARACTERISTIC_F_1));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f1");
    return err_r;
  }
}

int VsdNovomet::setUf_f2(float value)
{
  if (!Vsd::setUf_f2(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_2, getValue(VSD_UF_CHARACTERISTIC_F_2));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f2");
    return err_r;
  }
}

int VsdNovomet::setUf_f3(float value)
{
  if (!Vsd::setUf_f3(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_3, getValue(VSD_UF_CHARACTERISTIC_F_3));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f3");
    return err_r;
  }
}

int VsdNovomet::setUf_f4(float value)
{
  if (!Vsd::setUf_f4(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_4, getValue(VSD_UF_CHARACTERISTIC_F_4));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f4");
    return err_r;
  }
}

int VsdNovomet::setUf_f5(float value)
{
  if (!Vsd::setUf_f5(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_5, getValue(VSD_UF_CHARACTERISTIC_F_5));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f5");
    return err_r;
  }
}

int VsdNovomet::setUf_f6(float value)
{
  if (!Vsd::setUf_f6(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_F_6, getValue(VSD_UF_CHARACTERISTIC_F_6));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_f6");
    return err_r;
  }
}

int VsdNovomet::setUf_U1(float value)
{
  if (!Vsd::setUf_U1(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_1, getValue(VSD_UF_CHARACTERISTIC_U_1));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U1");
    return err_r;
  }
}

int VsdNovomet::setUf_U2(float value)
{
  if (!Vsd::setUf_U2(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_2, getValue(VSD_UF_CHARACTERISTIC_U_2));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U2");
    return err_r;
  }
}

int VsdNovomet::setUf_U3(float value)
{
  if (!Vsd::setUf_U3(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_3, getValue(VSD_UF_CHARACTERISTIC_U_3));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U3");
    return err_r;
  }
}

int VsdNovomet::setUf_U4(float value)
{
  if (!Vsd::setUf_U4(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_4, getValue(VSD_UF_CHARACTERISTIC_U_4));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U4");
    return err_r;
  }
}

int VsdNovomet::setUf_U5(float value)
{
  if (!Vsd::setUf_U5(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_5, getValue(VSD_UF_CHARACTERISTIC_U_5));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U5");
    return err_r;
  }
}

int VsdNovomet::setUf_U6(float value)
{
  if (!Vsd::setUf_U6(value)) {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_6, getValue(VSD_UF_CHARACTERISTIC_U_6));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdNovomet::setUf_U6");
    return err_r;
  }
}


// Метод проверки и обновления параметров устройства
void VsdNovomet::getNewValue(uint16_t id)
{
  float temp = 0;
  float value = 0;
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  // Проверка на валидность параметра в устройсте
  if (param->validity != ok_r) {
    value = NAN;
    setValue(id, value);
    return;
  }

  // Преобразование типа данных устройства -> float
  switch (param->typeData) {
  case TYPE_DATA_INT16:
    value = (float)param->value.int16_t[0];
    break;
  case TYPE_DATA_UINT16:
    value = (float)param->value.uint16_t[0];
    break;
  case  TYPE_DATA_INT32:
    value = (float)param->value.int32_t;
    break;
  case  TYPE_DATA_UINT32:
    value = (float)param->value.uint32_t;
    break;
  case  TYPE_DATA_FLOAT:
    value = (float)param->value.float_t;
    break;
  default:
    break;
  }

  // Применение коэффициента из устройства
  value = value * param->coefficient;

  // Преобразование из единиц измерения устройства в единицы КСУ
  value = (value - (units[param->physic][param->unit][1]))/(units[param->physic][param->unit][0]);

  // Особенная обработка некоторых параметров
  switch (id) {
  case VSD_MOTOR_TYPE:
    setValue(id, value);
    if (parameters.get(CCS_MOTOR_TYPE) != value)
      parameters.set(CCS_MOTOR_TYPE, value);
    break;
  case VSD_CURRENT_OUT_PHASE_1:             // Выходной ток ЧРП Фаза 1
    setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_1) * value);
    ksu.calcMotorCurrentPhase1();
    ksu.calcMotorCurrentAverage();
    ksu.calcMotorCurrentImbalance();
    break;
  case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
    setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_2) * value);
    ksu.calcMotorCurrentPhase2();
    ksu.calcMotorCurrentAverage();
    ksu.calcMotorCurrentImbalance();
    break;
  case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
    setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_3) * value);
    ksu.calcMotorCurrentPhase3();
    ksu.calcMotorCurrentAverage();
    ksu.calcMotorCurrentImbalance();
    break;
  case VSD_STATUS_WORD_1:
    setValue(id, value);
    parameters.set(CCS_VSD_ALARM_CODE, checkAlarmVsd());
    break;
  case VSD_STATUS_WORD_2:
    setValue(id, value);
    parameters.set(CCS_VSD_ALARM_CODE, checkAlarmVsd());
    break;
  case VSD_STATUS_WORD_3:
    setValue(id, value);
    parameters.set(CCS_VSD_STATUS_WORD_3, value);
    calcRotation();
    calcMotorType();
    calcSwitchFreqMode();
    calcResonanceRemoveSource();
    break;
  case VSD_STATUS_WORD_4:
    setValue(id, value);
    parameters.set(CCS_VSD_STATUS_WORD_4, value);
    break;
  case VSD_STATUS_WORD_7:
    setValue(id, value);
    break;
  case VSD_STATUS_WORD_5:
    setValue(id, value);
    break;
  case VSD_T_SPEEDUP:
    setValue(id, value);
    calcTempSpeedUp();
    calcTimeSpeedUp();
    break;
  case VSD_T_SPEEDDOWN:
    setValue(id, value);
    calcTempSpeedDown();
    calcTimeSpeedDown();
    break;
  case VSD_VOLTAGE_DC:
    setValue(id, value);
    calcCurrentDC();
    break;
  case VSD_POWER_ACTIVE:
    setValue(id, value);
    calcMotorCos();
    calcCurrentDC();
    break;
  case VSD_POWER_FULL:
    setValue(id, value);
    calcMotorCos();
    break;
  case VSD_LOW_LIM_SPEED_MOTOR:
    setValue(id, value);
    setLimitsMinFrequence(value);
    break;
  case VSD_HIGH_LIM_SPEED_MOTOR:
    setValue(id, value);
    setLimitsMaxFrequence(value);
    break;
  case VSD_FREQUENCY:
    setValue(id, value);
    ksu.calcMotorSpeed();
    break;
  case VSD_MOTOR_CURRENT:
    temp = parameters.get(CCS_COEF_TRANSFORMATION);
    if (temp != 0)
      value = value / temp;
    setValue(id, value);
    break;
  /*
  case VSD_MOTOR_VOLTAGE:
    temp = parameters.get(CCS_COEF_TRANSFORMATION);
    if (temp != 0)
      value = value * temp;
    setValue(id, value);
    break;
  */
  case VSD_LOUT:
    setValue(id, value);
    parameters.set(CCS_SYSTEM_INDUCTANCE, value);
    break;
  case VSD_TIME_MINUTE:
    setValue(id, value);
    if (getValue(VSD_TIME_MINUTE) != parameters.get(CCS_DATE_TIME_MIN)) {
      parameters.set(VSD_TIME_SECOND, parameters.get(CCS_DATE_TIME_SEC));
      parameters.set(VSD_TIME_MINUTE, parameters.get(CCS_DATE_TIME_MIN));
      parameters.set(VSD_TIME_HOUR, parameters.get(CCS_DATE_TIME_HOUR));
      parameters.set(VSD_TIME_MONTH, parameters.get(CCS_DATE_TIME_MONTH));
      parameters.set(VSD_TIME_YEAR, parameters.get(CCS_DATE_TIME_YEAR));
    }
    break;
  case VSD_THYR_T_EXT:
    value = ((value - 282)*222)/1023;
    setValue(id, value);
    break;
  case VSD_THYR_T_EXT_2:
    value = ((value - 282)*222)/1023;
    setValue(id, value);
    setValue(VSD_CONTROL_TEMPERATURE, max(getValue(VSD_THYR_T_EXT), getValue(VSD_THYR_T_EXT_2)));
    break;
  case VSD_DRV_0_T_EXT:
  case VSD_DRV_0_T_EXT_2:
  case VSD_DRV_0_T_EXT_3:
  case VSD_DRV_0_T_EXT_4:
  case VSD_DRV_1_T_EXT:
  case VSD_DRV_1_T_EXT_2:
  case VSD_DRV_1_T_EXT_3:
  case VSD_DRV_1_T_EXT_4:
  case VSD_DRV_2_T_EXT:
  case VSD_DRV_2_T_EXT_2:
  case VSD_DRV_2_T_EXT_3:
    value = ((value - 282)*222)/1023;
    setValue(id, value);
    break;
  case VSD_DRV_2_T_EXT_4:
    value = ((value - 282)*222)/1023;
    setValue(id, value);
    setValue(VSD_RADIATOR_TEMPERATURE, max(getValue(VSD_DRV_0_T_EXT),
                                           max(getValue(VSD_DRV_0_T_EXT_2),
                                               max(getValue(VSD_DRV_0_T_EXT_3),
                                                   max(getValue(VSD_DRV_0_T_EXT_4),
                                                       max(getValue(VSD_DRV_1_T_EXT),
                                                           max(getValue(VSD_DRV_1_T_EXT_2),
                                                               max(getValue(VSD_DRV_1_T_EXT_3),
                                                                   max(getValue(VSD_DRV_1_T_EXT_4),
                                                                       max(getValue(VSD_DRV_2_T_EXT),
                                                                           max(getValue(VSD_DRV_2_T_EXT_2),
                                                                               max(getValue(VSD_DRV_2_T_EXT_3),getValue(VSD_DRV_2_T_EXT_4)))))))))))));
    break;
  case VSD_DRV_0_T_AIR:
  case VSD_DRV_1_T_AIR:
    setValue(id, value);
    break;
  case VSD_DRV_2_T_AIR:
    setValue(id, value);
    setValue(VSD_RADIATOR_TEMPERATURE_1, max(getValue(VSD_DRV_0_T_AIR),
                                           max(getValue(VSD_DRV_1_T_AIR),
                                               (getValue(VSD_DRV_2_T_AIR)))));
    break;
  case VSD_BASE_VOLTAGE:
    setValue(id, value);
    if (parameters.get(CCS_BASE_VOLTAGE) != value)
      parameters.set(CCS_BASE_VOLTAGE, value);
    break;
  case VSD_BASE_FREQUENCY:
    setValue(id, value);
    if (parameters.get(CCS_BASE_FREQUENCY) != value)
      parameters.set(CCS_BASE_FREQUENCY, value);
    break;
  default:                                  // Прямая запись в массив параметров
    setValue(id, value);
    break;
  }
}

uint8_t VsdNovomet::setNewValue(uint16_t id, float value)
{
  switch (id) {

  case VSD_MOTOR_TYPE:
    if (!setMotorType(value)) {
      if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
        return setVsdControl(VSD_MOTOR_CONTROL_UF);
      }
      return ok_r;
    }
    return err_r;

  case VSD_MOTOR_FREQUENCY:
    return setMotorFrequency(value);

  case VSD_MOTOR_CURRENT:
    return setMotorCurrent(value);

  case VSD_MOTOR_VOLTAGE:
    if (!setMotorVoltage(value)) {
      ksu.calcTransRecommendedTapOff();
      return ok_r;
    }
    else
      return err_r;

  case VSD_FREQUENCY:
    return setFrequency(value);

  case VSD_LOW_LIM_SPEED_MOTOR:
    if (!setMinFrequency(value)) {
      if (getValue(VSD_LOW_LIM_SPEED_MOTOR) > getValue(VSD_FREQUENCY)) {
        return setFrequency(getValue(VSD_LOW_LIM_SPEED_MOTOR));
      }
      return ok_r;
    }
    return err_r;

  case VSD_HIGH_LIM_SPEED_MOTOR:
    if (!setMaxFrequency(value)) {
      if (getValue(VSD_HIGH_LIM_SPEED_MOTOR) < getValue(VSD_FREQUENCY)) {
        return setFrequency(getValue(VSD_HIGH_LIM_SPEED_MOTOR));
      }
      return ok_r;
    }
    return err_r;

  case VSD_ROTATION:
    return setRotation(value);

  case VSD_TIMER_DISPERSAL:
    return setTimeSpeedUp(value);

  case VSD_TIMER_DELAY:
    return setTimeSpeedDown(value);

  case VSD_RES_MODE:
    return setResonanceRemoveSource(value);

  case VSD_SWITCHING_FREQUENCY_MODE:
    return setSwitchingFrequencyMode(value);

  default:
    int result = setValue(id, value);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void VsdNovomet::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

void VsdNovomet::readInDevice(int id)
{
  dm_->readModbusParameter(id);
}

int VsdNovomet::start()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STARTED))
    return ok_r;

  resetRunQueue();

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_CONTROL_WORD_1, VSD_CONTROL_START))
        return err_r;

//      if (resetBlock())
//        return err_r;

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STARTED))
      return ok_r;
  }
}

bool VsdNovomet::checkStart()
{
#if USE_DEBUG
  return true;
#endif

  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STARTED)) {
    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_WAIT_START)) {
      return true;
    }
  }
  return false;
}

int VsdNovomet::stop(float type)
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит остановки по внешней команде
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_EXTERNAL))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      switch((uint16_t)type) {
      case TYPE_STOP_ALARM:
        if (setNewValue(VSD_CONTROL_WORD_1, VSD_CONTROL_ALARM))
          return err_r;
        break;
      default:
        if (setNewValue(VSD_CONTROL_WORD_1, VSD_CONTROL_STOP))
          return err_r;
       break;

       resetBlock();

      }
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_EXTERNAL))
      return ok_r;
  }
}

int VsdNovomet::alarmstop()
{
  // Если стоит бит остановки по внешней команде
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_ALARM))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_CONTROL_WORD_1, VSD_CONTROL_ALARM))
        return err_r;

      resetBlock();

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if ((checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_EXTERNAL)) ||
       (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_ALARM)))
      return ok_r;
  }
}

bool VsdNovomet::checkStop()
{
#if USE_DEBUG
  return true;
#endif

  if ((checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_EXTERNAL)) ||
      (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_ALARM)) ||
      (checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STOP_REGISTER))) {
    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_STARTED)) {
      if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_NOVOMET_STATUS_WAIT_STOP)) {
        resetBlock();
        return true;
      }
    }
  }
  return false;
}

int VsdNovomet::resetBlock()
{
  setNewValue(VSD_FLAG, 0);
  return setNewValue(VSD_CONTROL_WORD_1, VSD_CONTROL_RESET);
}

void VsdNovomet::processingRegimeRun()
{
  regimeRun_->processing();
}

void VsdNovomet::calcTempSpeedUp()
{
  setValue(VSD_TEMP_SPEEDUP, 1/getValue(VSD_T_SPEEDUP));
}

void VsdNovomet::calcTimeSpeedUp()
{
  setValue(VSD_TIMER_DISPERSAL, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP));
}

void VsdNovomet::calcTempSpeedDown()
{
  setValue(VSD_TEMP_SPEEDDOWN, 1/getValue(VSD_T_SPEEDDOWN));
}

void VsdNovomet::calcTimeSpeedDown()
{
  setValue(VSD_TIMER_DELAY, getValue(VSD_MOTOR_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN));
}

void VsdNovomet::calcRotation()
{
  if (checkBit(getValue(VSD_STATUS_WORD_3), VSD_NOVOMET_STATUS_RIGHT_DIRECT)) {
    setValue(VSD_ROTATION, VSD_ROTATION_DIRECT);
  }
  else {
    setValue(VSD_ROTATION, VSD_ROTATION_REVERSE);
  }
}

void VsdNovomet::calcMotorType()
{
  if (checkBit(getValue(VSD_STATUS_WORD_3), VSD_NOVOMET_STATUS_M_TYPE0)) {
    setValue(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  }
  else {
    setValue(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
    setValue(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  }
  float value = getValue(VSD_MOTOR_TYPE);
  if (parameters.get(CCS_MOTOR_TYPE) != value)
    parameters.set(CCS_MOTOR_TYPE, value);
}

void VsdNovomet::calcSwitchFreqMode()
{
  if (checkBit(getValue(VSD_STATUS_WORD_3), VSD_NOVOMET_STATUS_OWERPWM1)) {
    setValue(VSD_SWITCHING_FREQUENCY_MODE, VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_1);
  }
  else {
    if (checkBit(getValue(VSD_STATUS_WORD_3), VSD_NOVOMET_STATUS_OWERPWM2)) {
      setValue(VSD_SWITCHING_FREQUENCY_MODE, VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_2);
    }
    else {
      setValue(VSD_SWITCHING_FREQUENCY_MODE, VSD_SWITCHING_FREQUENCY_MODE_SIN);
    }
  }
}

void VsdNovomet::calcResonanceRemoveSource()
{
  setValue(VSD_RES_MODE, ((uint16_t)getValue(VSD_STATUS_WORD_3)) & 0x0003);
}

void VsdNovomet::calcSystemInduct()
{

}

void VsdNovomet::calcParameters(uint16_t id)
{

}

int VsdNovomet::onRegimeJarring()
{
  return 1;
}

int VsdNovomet::offRegimeJarring()
{
  return 1;
}

int VsdNovomet::setMainRegimeVSD()
{
  return setNewValue(VSD_REGULATOR_QUEUE_3, 0);             // Убираем режим из очереди алгоритмов
}

void VsdNovomet::resetRunQueue()
{
  int action = (parameters.get(CCS_RGM_RUN_PICKUP_MODE) ||
                parameters.get(CCS_RGM_RUN_PUSH_MODE) ||
                parameters.get(CCS_RGM_RUN_SWING_MODE) ||
                parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE));
  if (action)
    return;

  float queue[5];
  if (parameters.get(VSD_MOTOR_CONTROL) == VSD_MOTOR_CONTROL_UF) {
    for (int i = 0; i < 5; i++) {
      queue[i] = VSD_REQULATOR_QUEUE_UF;
    }
  }
  else if (parameters.get(VSD_MOTOR_CONTROL) == VSD_MOTOR_CONTROL_VECT) {
    for (int i = 0; i < 5; i++) {
      queue[i] = VSD_REQULATOR_QUEUE_VECT;
    }
  }

  parameters.set(VSD_REGULATOR_QUEUE_1, queue[0]);
  parameters.set(VSD_REGULATOR_QUEUE_2, queue[1]);
  parameters.set(VSD_REGULATOR_QUEUE_3, queue[2]);
  parameters.set(VSD_REGULATOR_QUEUE_4, queue[3]);
  parameters.set(VSD_REGULATOR_QUEUE_5, queue[4]);
}

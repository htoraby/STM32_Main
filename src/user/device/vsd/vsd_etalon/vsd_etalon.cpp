﻿/*
 * vsd_etalon.cpp
 *
 *  Created on: 13.04.2015
 *      Author: ablamtcev
 */

#include "vsd_etalon.h"
#include "user_main.h"
#include "regime_run_etalon.h"

VsdEtalon::VsdEtalon()
{
  regimeRun_ = new RegimeRunEtalon();
}

VsdEtalon::~VsdEtalon()
{
  delete regimeRun_;
  delete dm_;
}

void VsdEtalon::init()
{
  initModbusParameters();

  // Создание задачи обновления параметров
  createThread("UpdateParamsVsd");
  // Создание объекта протокола связи с утройством
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);

  initParameters();
  readParameters();

  setLimitsFrequence(0, getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsFrequence(1, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
}

// Метод заполнения внутреннего банка параметров по карте устройства
void VsdEtalon::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int indexModbus = 0; indexModbus < count; indexModbus++) {        // Цикл по карте регистров
    int id = dm_->getFieldID(indexModbus);
    if (id <= 0)
      continue;
    int indexArray = getIndexAtId(id);                                   // Получаем индекс параметра в банке параметров
    if (indexArray) {                                                    // Если нашли параметр
      setFieldAccess(indexArray, ACCESS_OPERATOR);                       // Уровень доступа оператор
      setFieldOperation(indexArray, dm_->getFieldOperation(indexModbus));// Операции над параметром
      setFieldPhysic(indexArray, dm_->getFieldPhysic(indexModbus));      // Физический смысл
      float tempVal = dm_->getFieldMinimum(indexModbus);                  // Получаем минимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMin(id, tempVal);
      tempVal = dm_->getFieldMaximum(indexModbus);                        // Получаем мaксимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMax(id, tempVal);
      tempVal = dm_->getFieldDefault(indexModbus);                        // Получаем значение по умолчанию
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldDef(indexArray, tempVal);
      setFieldValidity(indexArray, dm_->getFieldValidity(indexModbus));  // Получили флаг валидности
      setFieldValue(indexArray, getFieldDefault(indexArray));           // Присвоили значение значению по умолчанию
    }
  }
}

bool VsdEtalon::isConnect()
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

// Метод проверки и обновления параметров устройства
void VsdEtalon::getNewValue(uint16_t id)
{
  uint32_t vsdInvertorStatus = 0x0000;
  float value = 0;
  // Преобразуем данные из полученного типа данных в float
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity != ok_r) {
    value = NAN;
    setValue(id, value);
    return;
  }

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

  // Применяем коэффициент преобразования
  value = value * param->coefficient;

  // Применяем единицы измерения
  value = (value - (units[param->physic][param->unit][1]))/(units[param->physic][param->unit][0]);

  // Преобразования для параметров требующих особой обработки по id
  switch (id) {
  case VSD_ETALON_ON_STATE:                 // Получили подтверждение запуска
    vsdInvertorStatus = (uint32_t)parameters.get(CCS_VSD_INVERTOR_STATUS_1) & 0xFFFE;
    if (value)
      vsdInvertorStatus = setBit(vsdInvertorStatus, VSD_STATUS_READY, true);
    parameters.set(CCS_VSD_INVERTOR_STATUS_2,  (float)vsdInvertorStatus);
    setValue(id, value);
    break;
  case VSD_ETALON_OFF_STATE:                // Получили подтверждение останова
    vsdInvertorStatus = (uint32_t)parameters.get(CCS_VSD_INVERTOR_STATUS_1) & 0xFFF7;
    if (value)
      vsdInvertorStatus = setBit(vsdInvertorStatus, VSD_STATUS_STOPPED_EXTERNAL, true);
    parameters.set(CCS_VSD_INVERTOR_STATUS_1,  (float)vsdInvertorStatus);
    setValue(id, value);
    break;
  case VSD_INVERTOR_STATUS:                 // Получили слово состояния
    convertBitVsdStatus(value);
    setValue(id, value);
    break;
  case VSD_UF_CHARACTERISTIC_U_1_PERCENT:           // Получили точку напряжения U/f
    setValue(id, value);
    setValue(VSD_UF_CHARACTERISTIC_U_1, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
    break;
  case VSD_UF_CHARACTERISTIC_U_2_PERCENT:
    setValue(id, value);
    setValue(VSD_UF_CHARACTERISTIC_U_2, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
    break;
  case VSD_UF_CHARACTERISTIC_U_3_PERCENT:
    setValue(id, value);
    setValue(VSD_UF_CHARACTERISTIC_U_3, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
    break;
  case VSD_UF_CHARACTERISTIC_U_4_PERCENT:
    setValue(id, value);
    setValue(VSD_UF_CHARACTERISTIC_U_4, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
    break;
  case VSD_UF_CHARACTERISTIC_U_5_PERCENT:
    setValue(id, value);
    setValue(VSD_UF_CHARACTERISTIC_U_5, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
    break;
  case VSD_CURRENT_DC:                      // Получили ток Id
    setValue(id, getValue(VSD_COEF_CURRENT_IN_DC) * value);
    break;
  case VSD_VOLTAGE_DC:
    setValue(id, getValue(VSD_COEF_VOLTAGE_IN_DC) * value);
    break;
  case VSD_OUT_VOLTAGE_MOTOR:
    setValue(id, getValue(VSD_COEF_VOLTAGE_OUT_1) * getValue(VSD_COEF_VOLTAGE_OUT_2) * value);
    break;
  case VSD_LOW_LIM_SPEED_MOTOR:
    setLimitsFrequence(0, value);
    break;
  case VSD_HIGH_LIM_SPEED_MOTOR:
    setLimitsFrequence(1, value);
    break;
  case VSD_SOFT_VERSION:
    setValue(id, value);
    parameters.set(CCS_VERSION_SW_VSD, value);
    break;
  case VSD_SW_STARTUP_FREQUENCY:
    setValue(id, value);
    if ((parameters.get(CCS_RGM_RUN_PUSH_FREQ) != value) &&
        parameters.get(CCS_RGM_RUN_PUSH_MODE))
      parameters.set(CCS_RGM_RUN_PUSH_FREQ, value);
    if ((parameters.get(CCS_RGM_RUN_SWING_FREQ) != value) &&
        parameters.get(CCS_RGM_RUN_SWING_MODE))
      parameters.set(CCS_RGM_RUN_SWING_FREQ, value);
    break;
  case VSD_SW_STARTUP_U_PULSE:
    setValue(id, value);
    if ((parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) != (value + 100.0)) &&
        parameters.get(CCS_RGM_RUN_PUSH_MODE))
      parameters.set(CCS_RGM_RUN_PUSH_VOLTAGE, value + 100.0);
    break;
  case VSD_RGM_RUN_PUSH_UPTIME:
    setValue(id, value);
    if ((parameters.get(CCS_RGM_RUN_PUSH_TIME) != value) &&
        parameters.get(CCS_RGM_RUN_PUSH_MODE))
      parameters.set(CCS_RGM_RUN_PUSH_TIME, value);
    break;
  case VSD_RGM_RUN_PUSH_PERIOD:
    setValue(id, value);
    if ((parameters.get(CCS_RGM_RUN_PUSH_PERIOD) != value) &&
        parameters.get(CCS_RGM_RUN_PUSH_MODE))
      parameters.set(CCS_RGM_RUN_PUSH_PERIOD, value);
    break;
  case VSD_SW_STARTUP_OSC_COUNT:
    setValue(id, value);
    if ((parameters.get(CCS_RGM_RUN_PUSH_QUANTITY) != value) &&
        parameters.get(CCS_RGM_RUN_PUSH_MODE))
      parameters.set(CCS_RGM_RUN_PUSH_QUANTITY, value);
    if ((parameters.get(CCS_RGM_RUN_SWING_QUANTITY) != value) &&
        parameters.get(CCS_RGM_RUN_SWING_MODE))
      parameters.set(CCS_RGM_RUN_SWING_QUANTITY, value);
    break;
  case VSD_MOTOR_INDUCTANCE:
    setValue(id, value);
    if (parameters.get(CCS_MOTOR_INDUCTANCE) != value)
      parameters.set(CCS_MOTOR_INDUCTANCE, value);
    break;
  case VSD_MOTOR_INDUCTANCE_RESIST_PHASE:
    setValue(id, value);
    if (parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE) != value)
      parameters.set(CCS_MOTOR_INDUCTANCE_RESIST_PHASE, value);
    break;
  case VSD_TRANS_CABLE_CROSS:
    setValue(id, value);
    if (parameters.get(CCS_TRANS_CABLE_CROSS) != value)
      parameters.set(CCS_TRANS_CABLE_CROSS, value);
    break;
  case VSD_TRANS_NEED_VOLTAGE_TAP_OFF:
    setValue(id, value);
    if (parameters.get(CCS_TRANS_NEED_VOLTAGE_TAP_OFF) != value)
      parameters.set(CCS_TRANS_NEED_VOLTAGE_TAP_OFF, value);
    break;
  default:
    setValue(id, value);
    break;
  }
}

uint8_t VsdEtalon::setNewValue(uint16_t id, float value)
{
  int result;
  switch (id) {
  case VSD_FREQUENCY:
    return setFrequency(value);
  case VSD_MOTOR_TYPE:
    return setMotorType(value);
  case VSD_ROTATION:
    return setRotation(value);
  case VSD_LOW_LIM_SPEED_MOTOR:
    return setMinFrequency(value);
  case VSD_HIGH_LIM_SPEED_MOTOR:
    return setMaxFrequency(value);
  case  VSD_TEMP_SPEEDUP:
    return setTimeSpeedUp(value);
  case  VSD_TEMP_SPEEDDOWN:
    return setTimeSpeedDown(value);
  case  VSD_MOTOR_CONTROL:
    return setMotorControl(value);
  case VSD_UF_CHARACTERISTIC_U_1:
    return setUfU1(value);
  case VSD_UF_CHARACTERISTIC_U_2:
    return setUfU2(value);
  case VSD_UF_CHARACTERISTIC_U_3:
    return setUfU3(value);
  case VSD_UF_CHARACTERISTIC_U_4:
    return setUfU4(value);
  case VSD_UF_CHARACTERISTIC_U_5:
    return setUfU5(value);
  case VSD_UF_CHARACTERISTIC_U_6:
    return setUfU6(value);
  case VSD_UF_CHARACTERISTIC_F_6:
    return setUfF6(value);
  case VSD_COEF_VOLTAGE_IN_AB:
    return setCoefVoltageInAB(value);
  default:
    result = setValue(id, value);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void VsdEtalon::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

void VsdEtalon::readInDevice(int id)
{
  dm_->readModbusParameter(id);
}

int VsdEtalon::start()
{
#if USE_DEBUG
  return ok_r;
#endif

  if (getValue(VSD_ETALON_ON_STATE) == 1)
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

    if (countRepeats > VSD_CMD_NUMBER_REPEATS)
      return err_r;

    if (vsd->resetBlock())
      return err_r;
    if (setNewValue(VSD_ETALON_ON, 1))
      return err_r;
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (getValue(VSD_ETALON_ON_STATE) == 1)
      return ok_r;
    }
}

bool VsdEtalon::checkStart()
{
#if DEBUG
  return true;
#endif

  if (getValue(VSD_ETALON_ON_STATE) == 1)
    return ok_r;
  return false;
}

int VsdEtalon::stop(float type)
{
  // Если стоит бит остановки по внешней команде
  if (getValue(VSD_ETALON_OFF_STATE) == 1)
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  float oldTypeStop = getValue(VSD_TYPE_STOP);

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (type != oldTypeStop)
        writeToDevice(VSD_TYPE_STOP, type);

      if (setNewValue(VSD_ETALON_OFF, 1))
        return err_r;

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (getValue(VSD_ETALON_OFF_STATE) == 1) {
      writeToDevice(VSD_TYPE_STOP, oldTypeStop);
      return ok_r;
    }
  }
}

bool VsdEtalon::checkStop()
{
#if DEBUG
  return true;
#endif

  if (getValue(VSD_ETALON_OFF_STATE) == 1) {
    return ok_r;
  }
  return false;
}

int VsdEtalon::resetBlock()
{
  setNewValue(VSD_ETALON_FLAG, 0);
  return setNewValue(VSD_ETALON_UNLOCK, 1);
}

void VsdEtalon::processingRegimeRun()
{
  regimeRun_->processing();
}

int VsdEtalon::onRegimePush()
{
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == Regime::OffAction)
    return 1;

  setNewValue(VSD_SW_STARTUP_FREQUENCY, parameters.get(CCS_RGM_RUN_PUSH_FREQ));
  float UpVoltage = (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) - 100.0);
  if (UpVoltage < 0)
    UpVoltage = 0;
  setNewValue(VSD_SW_STARTUP_U_PULSE, UpVoltage);
  setNewValue(VSD_RGM_RUN_PUSH_UPTIME, parameters.get(CCS_RGM_RUN_PUSH_TIME));
  setNewValue(VSD_RGM_RUN_PUSH_PERIOD, parameters.get(CCS_RGM_RUN_PUSH_PERIOD));
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, parameters.get(CCS_RGM_RUN_PUSH_QUANTITY));
  return setNewValue(VSD_ETALON_START_TYPE, 1);
}

int VsdEtalon::offRegimePush()
{
  return setNewValue(VSD_ETALON_START_TYPE, 0);
}

int VsdEtalon::onRegimeSwing()
{
  if (parameters.get(CCS_RGM_RUN_SWING_MODE) == Regime::OffAction)
    return 1;

  setNewValue(VSD_SW_STARTUP_FREQUENCY, parameters.get(CCS_RGM_RUN_SWING_FREQ));
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, parameters.get(CCS_RGM_RUN_SWING_QUANTITY));
  return setNewValue(VSD_ETALON_START_TYPE, 2);
}

int VsdEtalon::offRegimeSwing()
{
  return setNewValue(VSD_ETALON_START_TYPE, 0);
}

int VsdEtalon::onRegimePickup()
{
  return setNewValue(VSD_ENGINE_DECEL_MODE, 2);
}

int VsdEtalon::offRegimePickup()
{
  return setNewValue(VSD_ENGINE_DECEL_MODE, 0);
}

int VsdEtalon::onRegimeSkipFreq()
{
  return setNewValue(VSD_FREQ_SKIP_MODE, 1);
}

int VsdEtalon::offRegimeSkipFreq()
{
  return setNewValue(VSD_FREQ_SKIP_MODE, 0);
}

int VsdEtalon::resetSetpoints()
{
  int result = setNewValue(VSD_ETALON_RESET, 1);
  setNewValue(VSD_ETALON_FLAG, 0);
  return result;
}

int VsdEtalon::setFrequency(float value)
{
  if (Vsd::setFrequency(value))
    return err_r;
  else {
    writeToDevice(VSD_FREQUENCY, value);
    return ok_r;
  }
}

int VsdEtalon::setMinFrequency(float value)
{
  if (Vsd::setMinFrequency(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
}

int VsdEtalon::setMaxFrequency(float value)
{
  if (Vsd::setMaxFrequency(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
}

int VsdEtalon::setMotorType(float value)
{
  if (Vsd::setMotorType(value)) {           // Записываем тип двигателя в массив
    logDebug.add(WarningMsg, "setTypeMotor");
    return err_r;                           // Если не записали возвращаем ошибку
  }
  else {                                    // Если записали
    writeToDevice(VSD_MOTOR_TYPE, value);   // Записываем в ЧРП
    return ok_r;
  }
}

int VsdEtalon::setTimeSpeedUp(float value)
{
  if (Vsd::setTimeSpeedUp(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_TIMER_DISPERSAL, getValue(VSD_TIMER_DISPERSAL));
    return ok_r;
  }
}

int VsdEtalon::setTimeSpeedDown(float value)
{
  if (Vsd::setTimeSpeedDown(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_TIMER_DELAY, getValue(VSD_TIMER_DELAY));
    return ok_r;
  }
}

int VsdEtalon::setUfU1(float value)
{
  int result = Vsd::setUfU1(value);
  if (result == ok_r) {
    result = setValue(VSD_UF_CHARACTERISTIC_U_1_PERCENT, (value * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
    if (result == ok_r) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_1_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_1_PERCENT));
    }
  }
  return result;
}

int VsdEtalon::setUfU2(float value)
{
  int result = Vsd::setUfU2(value);
  if (result == ok_r) {
    result = setValue(VSD_UF_CHARACTERISTIC_U_2_PERCENT, (value * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
    if (result == ok_r) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_2_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_2_PERCENT));
    }
  }
  return result;
}

int VsdEtalon::setUfU3(float value)
{
  int result = Vsd::setUfU3(value);
  if (result == ok_r) {
    result = setValue(VSD_UF_CHARACTERISTIC_U_3_PERCENT, (value * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
    if (result == ok_r) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_3_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_3_PERCENT));
    }
  }
  return result;
}

int VsdEtalon::setUfU4(float value)
{
  int result = Vsd::setUfU4(value);
  if (result == ok_r) {
    result = setValue(VSD_UF_CHARACTERISTIC_U_4_PERCENT, (value * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
    if (result == ok_r) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_4_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_4_PERCENT));
    }
  }
  return result;
}

int VsdEtalon::setUfU5(float value)
{
  int result = Vsd::setUfU5(value);
  if (result == ok_r) {
    result = setValue(VSD_UF_CHARACTERISTIC_U_5_PERCENT, (value * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
    if (result == ok_r) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_5_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_5_PERCENT));
    }
  }
  return result;
}

int VsdEtalon::setUfF6(float value)
{
  int result = setValue(VSD_UF_CHARACTERISTIC_F_6, value);
  if (!result)
    writeToDevice(VSD_UF_CHARACTERISTIC_F_6, value);
  result = setValue(VSD_BLDC_MAX_WORK_FREQ, value);
  if (!result)
    writeToDevice(VSD_BLDC_MAX_WORK_FREQ, value);
  osDelay(200);
  readUfCharacterictic();
  return result;
}

int VsdEtalon::setUfU(uint16_t id, float value)
{
  if (Vsd::setUfU(id, value)) {
    return err_r;
  }
  else {
    writeToDevice(id, (getValue(id) * 100.0) / getValue(VSD_UF_CHARACTERISTIC_U_6));
  return ok_r;
  }
}

int VsdEtalon::setUfU6(float value)
{
  int result = setValue(VSD_UF_CHARACTERISTIC_U_6, value);
  if (!result)
    writeToDevice(VSD_UF_CHARACTERISTIC_U_6, value);
  osDelay(200);
  readUfCharacterictic();
  setMax(VSD_UF_CHARACTERISTIC_U_1, value);
  setMax(VSD_UF_CHARACTERISTIC_U_2, value);
  setMax(VSD_UF_CHARACTERISTIC_U_3, value);
  setMax(VSD_UF_CHARACTERISTIC_U_4, value);
  setMax(VSD_UF_CHARACTERISTIC_U_5, value);
  return result;
}

void VsdEtalon::readUfCharacterictic()
{
  readInDevice(VSD_UF_CHARACTERISTIC_F_1);
  readInDevice(VSD_UF_CHARACTERISTIC_F_2);
  readInDevice(VSD_UF_CHARACTERISTIC_F_3);
  readInDevice(VSD_UF_CHARACTERISTIC_F_4);
  readInDevice(VSD_UF_CHARACTERISTIC_F_5);
  readInDevice(VSD_UF_CHARACTERISTIC_U_1_PERCENT);
  readInDevice(VSD_UF_CHARACTERISTIC_U_2_PERCENT);
  readInDevice(VSD_UF_CHARACTERISTIC_U_3_PERCENT);
  readInDevice(VSD_UF_CHARACTERISTIC_U_4_PERCENT);
  readInDevice(VSD_UF_CHARACTERISTIC_U_5_PERCENT);
}

int VsdEtalon::setRotation(float value)
{
  if(Vsd::setRotation(value)){
    return err_r;
  }
  else {
    writeToDevice(VSD_ROTATION, value);
    return ok_r;
  }
}

void VsdEtalon::calcParameters(uint16_t id)
{
  switch (id) {
  default:

    break;
  }
}

int VsdEtalon::setCoefVoltageInAB(float value)
{
  if (Vsd::setCoefVoltageInAB(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_COEF_VOLTAGE_IN_AB, value);
    readInDevice(VSD_VOLTAGE_PHASE_1_2);
  return ok_r;
  }
}

int VsdEtalon::setCoefVoltageInBC(float value)
{
  if (Vsd::setCoefVoltageInBC(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_COEF_VOLTAGE_IN_BC, value);
    readInDevice(VSD_VOLTAGE_PHASE_2_3);
  return ok_r;
  }
}

int VsdEtalon::setCoefVoltageInCA(float value)
{
  if (Vsd::setCoefVoltageInCA(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_COEF_VOLTAGE_IN_CA, value);
    readInDevice(VSD_VOLTAGE_PHASE_3_1);
  return ok_r;
  }
}

void VsdEtalon::convertBitVsdStatus(float value)
{
  // Получаем значение из регистры и сбрасываем в 0, только те биты,
  // которыми мы управляем, остальные не изменяем
  uint32_t vsdInvertorStatus1 = (uint32_t)parameters.get(CCS_VSD_INVERTOR_STATUS_1) & 0xD1DF;
  uint32_t vsdInvertorStatus2 = (uint32_t)parameters.get(CCS_VSD_INVERTOR_STATUS_2) & 0xDFFE;
  uint32_t vsdInvFault = (uint32_t)parameters.get(CCS_VSD_INV_FAULT) & 0xFF81;
  uint32_t vsdThyrStatus = (uint32_t)parameters.get(VSD_THYR_STATUS) & 0xFFF1;
  uint32_t vsdInvertorStatus4 = (uint32_t)parameters.get(CCS_VSD_INVERTOR_STATUS_4) & 0x8000;

  switch ((uint32_t)value) {
  case VSD_ETALON_INFO_READY:             // VSD_STATUS_READY
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_READY, true);
    break;
  case VSD_ETALON_INFO_UNDERLOAD:         // VSD_STATUS_UNDERLOAD
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_UNDERLOAD, true);
    break;
  case VSD_ETALON_INFO_OVERLOAD:          // VSD_STATUS_M_I2T_ERR
    vsdInvertorStatus2 = setBit(vsdInvertorStatus2, VSD_STATUS_M_I2T_ERR, true);
    break;
  case VSD_ETALON_INFO_RESISTANCE:        // VSD_STATUS_RESISTANCE
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_RESISTANCE, true);
    break;
  case VSD_ETALON_INFO_UNDERVOLTAGE:      // VSD_STATUS_UNDERVOLTAGE
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_UNDERVOLTAGE, true);
    break;
  case VSD_ETALON_INFO_OVERVOLTAGE:       // VSD_STATUS_OVERVOLTAGE
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_OVERVOLTAGE, true);
    break;
  case VSD_ETALON_INFO_UNDERVOLTAGE_DC:    // VSD_STATUS_UD_LOW_FAULT
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_UD_LOW_FAULT, true);
    break;
  case VSD_ETALON_INFO_OVERVOLTAGE_DC:    // VSD_STATUS_UD_HIGH_FAULT
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_UD_HIGH_FAULT, true);
    break;
  case VSD_ETALON_INFO_RUN_COUNT:         // VSD_STATUS_RUN_COUNT
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_RUN_COUNT, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_IGBT:     // VSD_STATUS_FC_I2T_ERR
    vsdInvertorStatus2 = setBit(vsdInvertorStatus2, VSD_STATUS_FC_I2T_ERR, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_FILTER:   // VSD_STATUS_OVERHEAT_FILTER
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_OVERHEAT_FILTER, true);
    break;
  case VSD_ETALON_INFO_PROT:              // VSD_STATUS_STOPPED_ALARM
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_INFO_SUPPLY_DRIVERS:    // VSD_FLT_DRV0
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_DRV0, true);
    break;
  case VSD_ETALON_INFO_MONOMETR:          // VSD_STATUS_MONOMETR
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_MONOMETR, true);
    break;
  case VSD_ETALON_INFO_AI_0:              // VSD_STATUS_AI_0
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_AI_0, true);
    break;
  case VSD_ETALON_INFO_SEQUENCE_PHASE:    // VSD_THYR_ABC_STATE
    vsdThyrStatus = setBit(vsdThyrStatus, VSD_THYR_ABC_STATE, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_MOTOR:    // VSD_STATUS_OVERHEAT_MOTOR
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_OVERHEAT_MOTOR, true);
    break;
  case VSD_ETALON_INFO_OVERVIBRATION:     // VSD_STATUS_OVERVIBRATION
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_OVERVIBRATION, true);
    break;
  case VSD_ETALON_INFO_PRESSURE:          // VSD_STATUS_PRESSURE
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_PRESSURE, true);
    break;
  case VSD_ETALON_INFO_19:                // VSD_STATUS_FAULT_STOPPED
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_IMBALANCE_CURRENT: // VSD_FLT_IZ
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_IZ, true);
    break;
  case VSD_ETALON_INFO_IMBALANCE_VOLTAGE: // VSD_STATUS_UIN_ASYM
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_UIN_ASYM, true);
    break;
  case VSD_ETALON_INFO_TURBINE:           // VSD_STATUS_TURBINE
    vsdInvertorStatus4 = setBit(vsdInvertorStatus4, VSD_STATUS_TURBINE, true);
    break;
  case VSD_ETALON_INFO_24:                // VSD_STATUS_FAULT_STOPPED
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_FAILURE_SUPPLY:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_DOOR:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_LOST_SUPPLY:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_CONDENSATOR:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_TERISTORS:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_CURRENT_LIMIT:     // VSD_STATUS_I_LIMIT
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_I_LIMIT, true);
    break;
  case VSD_ETALON_INFO_31:                // VSD_STATUS_FAULT_STOPPED
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_32:
    break;
  case VSD_ETALON_INFO_AUTO_STOP:
    break;
  case VSD_ETALON_INFO_MANUAL_STOP:
    break;
  case VSD_ETALON_INFO_REMOTE_STOP:
    break;
  case VSD_ETALON_INFO_AUTO_RUN:
    break;
  case VSD_ETALON_INFO_MANUAL_RUN:
    break;
  case VSD_ETALON_INFO_REMOTE_RUN:
    break;
  case VSD_ETALON_INFO_RESTART_COUNT:
    break;
  case VSD_ETALON_INFO_MEMORY:            // VSD_STATUS_CLK_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_INFO_41:
    break;
  case VSD_ETALON_INFO_DI:                // VSD_STATUS_CTR_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_CTR_MON, true);
    break;
  case VSD_ETALON_INFO_ADC:               // VSD_STATUS_AN_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_INFO_ANALOG_SUPPLY:     // VSD_STATUS_AN_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_INFO_SENSOR_SUPPLY:     // VSD_STATUS_MB_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_MB_MON, true);
    break;
  case VSD_ETALON_INFO_EEPROM:            // VSD_STATUS_CLK_MON
    vsdInvFault = setBit(vsdInvFault, VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_INFO_NOT_READY:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_SETPOINT:
    break;
  case VSD_ETALON_INFO_BLOCK_RUN:
    vsdInvertorStatus1 = setBit(vsdInvertorStatus1, VSD_STATUS_FAULT_STOPPED, true);
    break;
  default:
    break;
  }

  parameters.set(CCS_VSD_INVERTOR_STATUS_1,  (float)vsdInvertorStatus1);
  parameters.set(CCS_VSD_INVERTOR_STATUS_2, (float)vsdInvertorStatus2);
  parameters.set(CCS_VSD_INV_FAULT, (float)vsdInvFault);
  parameters.set(CCS_VSD_THYR_STATUS, (float)vsdThyrStatus);
  parameters.set(CCS_VSD_INVERTOR_STATUS_4, (float)vsdInvertorStatus4);
}
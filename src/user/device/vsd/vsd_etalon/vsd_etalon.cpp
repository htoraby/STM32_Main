/*
 * vsd_etalon.cpp
 *
 *  Created on: 13.04.2015
 *      Author: ablamtcev
 */

#include "vsd_etalon.h"
#include "user_main.h"
#include "regime_run_etalon.h"
#include "vsd_etalon_log.h"

VsdEtalon::VsdEtalon()
{
  regimeRun_ = new RegimeRunEtalon();
  log_ = new VsdEtalonLog();
}

VsdEtalon::~VsdEtalon()
{
  delete regimeRun_;
  delete dm_;
  delete log_;
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

  setLimitsMinFrequence(getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsMaxFrequence(parameters.get(CCS_BASE_FREQUENCY));
}

// Метод заполнения внутреннего банка параметров по карте устройства
void VsdEtalon::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int i = 0; i < count; i++) {        // Цикл по карте регистров
    if ((modbusParameters_[i].freqExchange != EVERY_TIME) || (modbusParameters_[i].freqExchange != NOT_READ)) {
      modbusParameters_[i].freqExchange = modbusParameters_[i].freqExchange + i;
      modbusParameters_[i].cntExchange = modbusParameters_[i].freqExchange;
    }

    int id = dm_->getFieldID(i);
    if (id <= 0)
      continue;
    int indexArray = getIndexAtId(id);                                   // Получаем индекс параметра в банке параметров
    if (indexArray) {                                                    // Если нашли параметр
      setFieldAccess(indexArray, ACCESS_OPERATOR);                       // Уровень доступа оператор
      setFieldOperation(indexArray, dm_->getFieldOperation(i));// Операции над параметром
      setFieldPhysic(indexArray, dm_->getFieldPhysic(i));      // Физический смысл
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

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
int VsdEtalon::setMotorType(float value)
{
  if (!Vsd::setMotorType(value)) {          // Записываем в массив
    writeToDevice(VSD_MOTOR_TYPE, value);   // Записываем в ЧРП
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setMotorType");
    return err_r;
  }
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
int VsdEtalon::setRotation(float value)
{
  if (!Vsd::setRotation(value)){
    writeToDevice(VSD_ROTATION, getValue(VSD_ROTATION));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setRotation");
    return err_r;
  }
}

int VsdEtalon::setMinFrequency(float value)
{
  if (!Vsd::setMinFrequency(value)) {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setMinFrequency");
    return err_r;
  }
}

int VsdEtalon::setMaxFrequency(float value)
{
  if (!Vsd::setMaxFrequency(value)) {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setMaxFrequency");
    return err_r;
  }
}

int VsdEtalon::setFrequency(float value)
{
  if (!Vsd::setFrequency(value)) {
    writeToDevice(VSD_FREQUENCY, getValue(VSD_FREQUENCY));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setFrequency");
    return err_r;
  }
}

int VsdEtalon::setTimeSpeedUp(float value)
{
  if (!Vsd::setTimeSpeedUp(value)) {
    writeToDevice(VSD_TIMER_DISPERSAL, getValue(VSD_TIMER_DISPERSAL));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setTimeSpeedUp");
    return err_r;
  }
}

int VsdEtalon::setTimeSpeedDown(float value)
{
  if (!Vsd::setTimeSpeedDown(value)) {
    writeToDevice(VSD_TIMER_DELAY, getValue(VSD_TIMER_DELAY));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setTimeSpeedUp");
    return err_r;
  }
}

int VsdEtalon::setSwitchingFrequency(float value)
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

int VsdEtalon::setCoefVoltageInAB(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_AB, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_AB, getValue(VSD_COEF_VOLTAGE_IN_AB));
    readInDevice(VSD_VOLTAGE_PHASE_1_2);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setCoefVoltageInAB");
    return err_r;
  }
}

int VsdEtalon::setCoefVoltageInBC(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_BC, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_BC, getValue(VSD_COEF_VOLTAGE_IN_BC));
    readInDevice(VSD_VOLTAGE_PHASE_2_3);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setCoefVoltageInBC");
    return err_r;
  }
}

int VsdEtalon::setCoefVoltageInCA(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_CA, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_CA, getValue(VSD_COEF_VOLTAGE_IN_CA));
    readInDevice(VSD_VOLTAGE_PHASE_3_1);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setCoefVoltageInCA");
    return err_r;
  }
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

  // Если получено новое значение параметра
  if ((getValue(id) != value) || (param->validity != getValidity(id))) {
    // Преобразования для параметров требующих особой обработки по id
    switch (id) {
    case VSD_MOTOR_TYPE:
      setValue(id, value);
      if (parameters.get(CCS_MOTOR_TYPE) != value)
        parameters.set(CCS_MOTOR_TYPE, value);
      break;
    case VSD_ETALON_ON_STATE:                 // Получили подтверждение запуска
      vsdInvertorStatus = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_1) & 0xFFFE;
      if (value)
        vsdInvertorStatus = setBit(vsdInvertorStatus, VSD_STATUS_STARTED, true);
      parameters.set(CCS_VSD_STATUS_WORD_1,  (float)vsdInvertorStatus);
      setValue(id, value);
      break;
    case VSD_ETALON_OFF_STATE:                // Получили подтверждение останова
      vsdInvertorStatus = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_1) & 0xFFF7;
      if (value)
        vsdInvertorStatus = setBit(vsdInvertorStatus, VSD_STATUS_STOPPED_EXTERNAL, true);
      parameters.set(CCS_VSD_STATUS_WORD_1,  (float)vsdInvertorStatus);
      setValue(id, value);
      break;
    case VSD_STATUS_WORD_1:                 // Получили слово состояния
      convertBitVsdStatus(value);
      setValue(id, value);
      break;
    case VSD_UF_CHARACTERISTIC_U_1_PERCENT:           // Получили точку напряжения U/f
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_1, (int)(parameters.get(VSD_BASE_VOLTAGE) * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_2_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_2, (int)(parameters.get(VSD_BASE_VOLTAGE) * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_3_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_3, (int)(parameters.get(VSD_BASE_VOLTAGE) * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_4_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_4, (int)(parameters.get(VSD_BASE_VOLTAGE) * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_5_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_5, (int)(parameters.get(VSD_BASE_VOLTAGE) * value / 100.0));
      break;
    case VSD_LOW_LIM_SPEED_MOTOR:
      setLimitsMinFrequence(value);
      break;
    case VSD_HIGH_LIM_SPEED_MOTOR:
      setLimitsMaxFrequence(value);
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
    case VSD_TRANS_VOLTAGE_TAP_OFF:
      setValue(id, value);
      if (parameters.get(CCS_TRANS_VOLTAGE_TAP_OFF) != value)
        parameters.set(CCS_TRANS_VOLTAGE_TAP_OFF, value);
      break;
    case VSD_TRANS_NEED_VOLTAGE_TAP_OFF:
      setValue(id, value);
      if (parameters.get(CCS_TRANS_NEED_VOLTAGE_TAP_OFF) != value)
        parameters.set(CCS_TRANS_NEED_VOLTAGE_TAP_OFF, value);
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
    case VSD_CURRENT_OUT_PHASE_1:             // Выходной ток ЧРП Фаза 1
      setValue(id, value);
      ksu.calcMotorCurrentPhase1();
      ksu.calcMotorCurrentAverage();
      ksu.calcMotorCurrentImbalance();
      break;
    case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
      setValue(id, value);
      ksu.calcMotorCurrentPhase2();
      ksu.calcMotorCurrentAverage();
      ksu.calcMotorCurrentImbalance();
      break;
    case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
      setValue(id, value);
      ksu.calcMotorCurrentPhase3();
      ksu.calcMotorCurrentAverage();
      ksu.calcMotorCurrentImbalance();
      break;
    case VSD_FREQUENCY_NOW:
      setValue(id, value);
      ksu.calcMotorSpeed();
      break;
    case VSD_COEF_OUT_CURRENT_1:
      setValue(id, value);
      if (parameters.get(CCS_COEF_OUT_CURRENT_1) != value)
        parameters.set(CCS_COEF_OUT_CURRENT_1, value);
      break;
    case VSD_COEF_OUT_CURRENT_2:
      setValue(id, value);
      if (parameters.get(CCS_COEF_OUT_CURRENT_2) != value)
        parameters.set(CCS_COEF_OUT_CURRENT_2, value);
      break;
    case VSD_COEF_OUT_CURRENT_3:
      setValue(id, value);
      if (parameters.get(CCS_COEF_OUT_CURRENT_3) != value)
        parameters.set(CCS_COEF_OUT_CURRENT_3, value);
      break;
    case VSD_OUT_VOLTAGE_MOTOR:
      setValue(id, value);
      break;
    case VSD_COEF_VOLTAGE_OUT_1:
      setValue(id, value);
      break;
    case VSD_PROT_NO_CONNECT_MODE:
      setValue(id, value);
      if (parameters.get(VSD_PROT_NO_CONNECT_MODE) && !value)
        parameters.set(VSD_PROT_NO_CONNECT_MODE, 0.0);
      else if (!parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE) && value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE, 3.0);
      break;
    case VSD_PROT_NO_CONNECT_TRIP_DELAY:
      setValue(id, value);
      if (parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY) != value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY, value);
      break;
    case VSD_TURBO_ROTATION_NOW:
      setValue(id, value);
      parameters.set(CCS_TURBO_ROTATION_NOW, value);
      break;
    case VSD_DOOR_VALUE:
      setValue(id, value);
      parameters.set(CCS_DOOR_VALUE, value);
      break;
    default:
      setValue(id, value);
      break;
    }
  }
}

uint8_t VsdEtalon::setNewValue(uint16_t id, float value)
{
  int result;
  switch (id) {

  case VSD_MOTOR_TYPE:
    if (!setMotorType(value)) {
      if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
        return setVsdControl(VSD_MOTOR_CONTROL_UF);
      }
      return ok_r;
    }
    return err_r;
    
  case  VSD_MOTOR_CONTROL:
    return setVsdControl(value);
    
  case VSD_ROTATION:
    return setRotation(value);

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
    
  case VSD_FREQUENCY:
    return setFrequency(value);

  case VSD_TEMP_SPEEDUP:
    return setTimeSpeedUp(value);
    
  case VSD_TEMP_SPEEDDOWN:
    return setTimeSpeedDown(value);
    
  case VSD_SWITCHING_FREQUENCY:
    return setSwitchingFrequency(value);

  case VSD_COEF_VOLTAGE_IN_AB:
    return setCoefVoltageInAB(value);

  case VSD_COEF_VOLTAGE_IN_BC:
    return setCoefVoltageInBC(value);

  case VSD_COEF_VOLTAGE_IN_CA:
    return setCoefVoltageInCA(value);

  case VSD_UF_CHARACTERISTIC_U_1:
    return setUf_U1(value);

  case VSD_UF_CHARACTERISTIC_U_2:
    return setUf_U2(value);

  case VSD_UF_CHARACTERISTIC_U_3:
    return setUf_U3(value);

  case VSD_UF_CHARACTERISTIC_U_4:
    return setUf_U4(value);

  case VSD_UF_CHARACTERISTIC_U_5:
    return setUf_U5(value);

  case VSD_BASE_VOLTAGE:
    return setBaseVoltage(value);

  case VSD_BASE_FREQUENCY:
    return setBaseFrequency(value);

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

  if (checkStatusVsd(VSD_STATUS_STARTED))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (resetBlock())
        return err_r;
      if (setNewValue(VSD_ON, 1))
        return err_r;
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkStatusVsd(VSD_STATUS_STARTED))
      return ok_r;
  }
}

bool VsdEtalon::checkStart()
{
#if USE_DEBUG
  return true;
#endif

  if ((getValue(VSD_ETALON_ON_STATE) == 1) && parameters.isValidity(VSD_ETALON_ON_STATE))
    return true;
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

      if (setNewValue(VSD_OFF, 1))
        return err_r;

      resetBlock();

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
#if USE_DEBUG
  return true;
#endif

  if ((getValue(VSD_ETALON_OFF_STATE) == 1) && parameters.isValidity(VSD_ETALON_OFF_STATE)) {
    return true;
  }
  return false;
}

int VsdEtalon::resetBlock()
{
  setNewValue(VSD_FLAG, 0);
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

int VsdEtalon::setUf_U1(float value)
{
  if (!Vsd::setUf_U1(value)) {
    if (!setValue(VSD_UF_CHARACTERISTIC_U_1_PERCENT, (getValue(VSD_UF_CHARACTERISTIC_U_1) * 100.0) / getValue(VSD_BASE_VOLTAGE))) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_1_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_1_PERCENT));
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setUfU1");
    return err_r;
  }
}

int VsdEtalon::setUf_U2(float value)
{
  if (!Vsd::setUf_U2(value)) {
    if (!setValue(VSD_UF_CHARACTERISTIC_U_2_PERCENT, (getValue(VSD_UF_CHARACTERISTIC_U_2) * 100.0) / getValue(VSD_BASE_VOLTAGE))) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_2_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_2_PERCENT));
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setUfU2");
    return err_r;
  }
}

int VsdEtalon::setUf_U3(float value)
{
  if (!Vsd::setUf_U3(value)) {
    if (!setValue(VSD_UF_CHARACTERISTIC_U_3_PERCENT, (getValue(VSD_UF_CHARACTERISTIC_U_3) * 100.0) / getValue(VSD_BASE_VOLTAGE))) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_3_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_3_PERCENT));
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setUfU3");
    return err_r;
  }
}

int VsdEtalon::setUf_U4(float value)
{
  if (!Vsd::setUf_U4(value)) {
    if (!setValue(VSD_UF_CHARACTERISTIC_U_4_PERCENT, (getValue(VSD_UF_CHARACTERISTIC_U_4) * 100.0) / getValue(VSD_BASE_VOLTAGE))) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_4_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_4_PERCENT));
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setUfU4");
    return err_r;
  }
}

int VsdEtalon::setUf_U5(float value)
{
  if (!Vsd::setUf_U5(value)) {
    if (!setValue(VSD_UF_CHARACTERISTIC_U_5_PERCENT, (getValue(VSD_UF_CHARACTERISTIC_U_5) * 100.0) / getValue(VSD_BASE_VOLTAGE))) {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_5_PERCENT, getValue(VSD_UF_CHARACTERISTIC_U_5_PERCENT));
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setUfU5");
    return err_r;
  }
}

int VsdEtalon::calcUfCharacteristicU(float value)
{
  return setBaseVoltage(value);
}

int VsdEtalon::calcUfCharacteristicF(float value)
{
  return setBaseFrequency(value);
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

int VsdEtalon::setBaseVoltage(float value)
{
  if (!setValue(VSD_BASE_VOLTAGE, value)) {
    writeToDevice(VSD_BASE_VOLTAGE, value);
    osDelay(200);
    readInDevice(VSD_TRANS_NEED_VOLTAGE_TAP_OFF);
    osDelay(200);
    value = value*(getValue(VSD_TRANS_NEED_VOLTAGE_TAP_OFF)/getValue(VSD_TRANS_VOLTAGE_TAP_OFF));
    setMax(VSD_UF_CHARACTERISTIC_U_1, value);
    setMax(VSD_UF_CHARACTERISTIC_U_2, value);
    setMax(VSD_UF_CHARACTERISTIC_U_3, value);
    setMax(VSD_UF_CHARACTERISTIC_U_4, value);
    setMax(VSD_UF_CHARACTERISTIC_U_5, value);
    readUfCharacterictic();
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setBaseVoltage");
    return err_r;
  }
}

int VsdEtalon::setBaseFrequency(float value)
{
  if (!setValue(VSD_BASE_FREQUENCY, value)) {
    writeToDevice(VSD_BASE_FREQUENCY, value);
    if (!setValue(VSD_BLDC_MAX_WORK_FREQ, value)) {
      writeToDevice(VSD_BLDC_MAX_WORK_FREQ, value);
      osDelay(200);
      readUfCharacterictic();
      return ok_r;
    }
    return err_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdEtalon::setBaseFrequency");
    return err_r;
  }
}

int VsdEtalon::setUfU(uint16_t id, float value)
{
  if (Vsd::setUfU(id, value)) {
    return err_r;
  }
  else {
    writeToDevice(id, (getValue(id) * 100.0) / getValue(VSD_BASE_VOLTAGE));
    return ok_r;
  }
}

int VsdEtalon::resetSetpoints()
{
  int result = setNewValue(VSD_ETALON_RESET, 1);
  setNewValue(VSD_FLAG, 0);
  return result;
}

void VsdEtalon::calcParameters(uint16_t id)
{
  switch (id) {
  default:

    break;
  }
}

void VsdEtalon::convertBitVsdStatus(float value)
{
  // Получаем значение из регистры и сбрасываем в 0, только те биты,
  // которыми мы управляем, остальные не изменяем
  uint32_t vsdStatusWord1 = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_1) & 0xD1DF;
  uint32_t vsdStatusWord2 = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_2) & 0xDBFE;
  uint32_t vsdStatusWord4 = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_4) & 0x8000;
  uint32_t vsdStatusWord5 = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_5) & 0xFFF1;
  uint32_t vsdStatusWord7 = (uint32_t)parameters.get(CCS_VSD_STATUS_WORD_7) & 0xFF81;

  switch ((uint32_t)value) {
  case VSD_ETALON_STATUS_READY:             // VSD_STATUS_READY
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_READY, true);
    break;
  case VSD_ETALON_STATUS_UNDERLOAD:         // VSD_STATUS_UNDERLOAD
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_UNDERLOAD, true);
    break;
  case VSD_ETALON_STATUS_OVERLOAD:          // VSD_STATUS_M_I2T_ERR
    vsdStatusWord2 = setBit(vsdStatusWord2, VSD_STATUS_M_I2T_ERR, true);
    break;
  case VSD_ETALON_STATUS_RESISTANCE:        // VSD_STATUS_RESISTANCE
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_RESISTANCE, true);
    break;
  case VSD_ETALON_STATUS_UNDERVOLTAGE:      // VSD_STATUS_UNDERVOLTAGE
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_UNDERVOLTAGE, true);
    break;
  case VSD_ETALON_STATUS_OVERVOLTAGE:       // VSD_STATUS_OVERVOLTAGE
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_OVERVOLTAGE, true);
    break;
  case VSD_ETALON_STATUS_UNDERVOLTAGE_DC:    // VSD_STATUS_UD_LOW_FAULT
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_UD_LOW_FAULT, true);
    break;
  case VSD_ETALON_STATUS_OVERVOLTAGE_DC:    // VSD_STATUS_UD_HIGH_FAULT
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_UD_HIGH_FAULT, true);
    break;
  case VSD_ETALON_STATUS_RUN_COUNT:         // VSD_STATUS_RUN_COUNT
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_RUN_COUNT, true);
    break;
  case VSD_ETALON_STATUS_OVERHEAT_IGBT:     // VSD_STATUS_FC_I2T_ERR
    vsdStatusWord2 = setBit(vsdStatusWord2, VSD_STATUS_FC_IT_ERR, true);
    break;
  case VSD_ETALON_STATUS_OVERHEAT_FILTER:   // VSD_STATUS_OVERHEAT_FILTER
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_OVERHEAT_FILTER, true);
    break;
  case VSD_ETALON_STATUS_PROT:              // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_SUPPLY_DRIVERS:    // VSD_FLT_DRV0
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_DRV0, true);
    break;
  case VSD_ETALON_STATUS_MONOMETR:          // VSD_STATUS_MONOMETR
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_MONOMETR, true);
    break;
  case VSD_ETALON_STATUS_AI_0:              // VSD_STATUS_AI_0
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_AI_0, true);
    break;
  case VSD_ETALON_STATUS_SEQUENCE_PHASE:    // VSD_THYR_ABC_STATE
    vsdStatusWord5 = setBit(vsdStatusWord5, VSD_STATUS_ABC_STATE, true);
    break;
  case VSD_ETALON_STATUS_OVERHEAT_MOTOR:    // VSD_STATUS_OVERHEAT_MOTOR
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_OVERHEAT_MOTOR, true);
    break;
  case VSD_ETALON_STATUS_OVERVIBRATION:     // VSD_STATUS_OVERVIBRATION
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_OVERVIBRATION, true);
    break;
  case VSD_ETALON_STATUS_PRESSURE:          // VSD_STATUS_PRESSURE
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_PRESSURE, true);
    break;
  case VSD_ETALON_STATUS_19:                // VSD_STATUS_ERR_19
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_ERR_19, true);
    break;
  case VSD_ETALON_STATUS_IMBALANCE_CURRENT: // VSD_FLT_IZ
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_IZ, true);
    break;
  case VSD_ETALON_STATUS_IMBALANCE_VOLTAGE: // VSD_STATUS_UIN_ASYM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_UIN_ASYM, true);
    break;
  case VSD_ETALON_STATUS_TURBINE:           // VSD_STATUS_TURBINE
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_TURBINE, true);
    break;
  case VSD_ETALON_STATUS_24:                // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_FAILURE_SUPPLY:    // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_DOOR:              // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_LOST_SUPPLY:       // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_CONDENSATOR:       // VSD_THYR_CHARGE_STATE
    vsdStatusWord1 = setBit(vsdStatusWord5, VSD_STATUS_CHARGE_STATE, true);
    break;
  case VSD_ETALON_STATUS_TERISTORS:         // VSD_STATUS_STOPPED_ALARM
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_CURRENT_LIMIT:     // VSD_STATUS_I_LIMIT
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_I_LIMIT, true);
    break;
  case VSD_ETALON_STATUS_31:                // VSD_STATUS_ERR_31
    vsdStatusWord4 = setBit(vsdStatusWord4, VSD_STATUS_ERR_31, true);
    break;
  case VSD_ETALON_STATUS_32:
    break;
  case VSD_ETALON_STATUS_AUTO_STOP:
    break;
  case VSD_ETALON_STATUS_MANUAL_STOP:
    break;
  case VSD_ETALON_STATUS_REMOTE_STOP:
    break;
  case VSD_ETALON_STATUS_AUTO_RUN:
    break;
  case VSD_ETALON_STATUS_MANUAL_RUN:
    break;
  case VSD_ETALON_STATUS_REMOTE_RUN:
    break;
  case VSD_ETALON_STATUS_RESTART_COUNT:
    break;
  case VSD_ETALON_STATUS_MEMORY:            // VSD_STATUS_CLK_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_STATUS_41:
    break;
  case VSD_ETALON_STATUS_DI:                // VSD_STATUS_CTR_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_CTR_MON, true);
    break;
  case VSD_ETALON_STATUS_ADC:               // VSD_STATUS_AN_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_STATUS_ANALOG_SUPPLY:     // VSD_STATUS_AN_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_STATUS_SENSOR_SUPPLY:     // VSD_STATUS_MB_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_MB_MON, true);
    break;
  case VSD_ETALON_STATUS_EEPROM:            // VSD_STATUS_CLK_MON
    vsdStatusWord7 = setBit(vsdStatusWord7, VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_STATUS_NOT_READY:
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_STATUS_SETPOINT:
    break;
  case VSD_ETALON_STATUS_BLOCK_RUN:
    vsdStatusWord1 = setBit(vsdStatusWord1, VSD_STATUS_STOPPED_ALARM, true);
    break;
  default:
    break;
  }

  parameters.set(CCS_VSD_STATUS_WORD_1, (float)vsdStatusWord1);
  parameters.set(CCS_VSD_STATUS_WORD_2, (float)vsdStatusWord2);
  parameters.set(CCS_VSD_STATUS_WORD_4, (float)vsdStatusWord4);
  parameters.set(CCS_VSD_STATUS_WORD_5, (float)vsdStatusWord5);
  parameters.set(CCS_VSD_STATUS_WORD_7, (float)vsdStatusWord7);
}

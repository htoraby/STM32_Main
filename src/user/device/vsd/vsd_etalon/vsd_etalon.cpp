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

#define BASE_VOLTAGE 380

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
  createThread("UpdParamsVsd");
  // Создание объекта протокола связи с утройством
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);

  initParameters();
  readParameters();

  setLimitsCcsParameters();
  setLimitsMinFrequence(getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsMaxFrequence(parameters.get(CCS_BASE_FREQUENCY));
}

// Метод заполнения внутреннего банка параметров по карте устройства
void VsdEtalon::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int i = 0; i < count; i++) {        // Цикл по карте регистров
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
      dm_->setFieldValidity(indexModbus, err_r);
      getNewValue(id);
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
  return err_r;
}

int VsdEtalon::setMinFrequency(float value)
{
  if (!Vsd::setMinFrequency(value)) {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setMaxFrequency(float value)
{
  if (!Vsd::setMaxFrequency(value)) {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setFrequency(float value, EventType eventType)
{
  if (!Vsd::setFrequency(value, eventType)) {
    writeToDevice(VSD_FREQUENCY, getValue(VSD_FREQUENCY));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setTimeSpeedUp(float value)
{
  if (!Vsd::setTimeSpeedUp(value)) {
    writeToDevice(VSD_TIMER_DISPERSAL, getValue(VSD_TIMER_DISPERSAL));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setTimeSpeedDown(float value)
{
  if (!Vsd::setTimeSpeedDown(value)) {
    writeToDevice(VSD_TIMER_DELAY, getValue(VSD_TIMER_DELAY));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setSwitchingFrequency(float value)
{
  if (!Vsd::setSwitchingFrequency(value)) {
    writeToDevice(VSD_SWITCHING_FREQUENCY, getValue(VSD_SWITCHING_FREQUENCY));
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setCoefVoltageInAB(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_AB, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_AB, getValue(VSD_COEF_VOLTAGE_IN_AB));
    readInDevice(VSD_VOLTAGE_PHASE_1_2);
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setCoefVoltageInBC(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_BC, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_BC, getValue(VSD_COEF_VOLTAGE_IN_BC));
    readInDevice(VSD_VOLTAGE_PHASE_2_3);
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setCoefVoltageInCA(float value)
{
  if (!setValue(VSD_COEF_VOLTAGE_IN_CA, value)) {
    writeToDevice(VSD_COEF_VOLTAGE_IN_CA, getValue(VSD_COEF_VOLTAGE_IN_CA));
    readInDevice(VSD_VOLTAGE_PHASE_3_1);
    return ok_r;
  }
  return err_r;
}

float VsdEtalon::checkAlarmVsd()
{
  float vsdAlarm = parameters.get(CCS_VSD_ALARM_CODE);
  float vsdStatus1 = getValue(VSD_STATUS_WORD_1);

  if (vsdStatus1 == VSD_ALARM_NONE) {
    return vsdStatus1;
  }

  if ((vsdStatus1 >= VSD_ETALON_ALARM_UNDERLOAD - 3000) && (vsdStatus1 <= VSD_ETALON_ALARM_RESISTANCE - 3000)) {
    return vsdStatus1 + 3000;
  }

  if ((vsdStatus1 == VSD_ETALON_ALARM_UNDERVOLTAGE - 3000) && (!parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE))) {
    return vsdStatus1 + 3000;
  }

  if ((vsdStatus1 == VSD_ETALON_ALARM_OVERVOLTAGE - 3000) && (!parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE))) {
    return vsdStatus1 + 3000;
  }

  if ((vsdStatus1 >= VSD_ETALON_ALARM_OVERVOLTAGE_DC - 3000) && (vsdStatus1 <= VSD_ETALON_ALARM_32 - 3000)) {
    return vsdStatus1 + 3000;
  }

  if((vsdStatus1 >= VSD_ETALON_ALARM_RESTART_COUNT - 3000) && (vsdStatus1 <= VSD_ETALON_ALARM_BLOCK_RUN - 3000)) {
    return vsdStatus1 + 3000;
  }

  return vsdAlarm;
}

float VsdEtalon::checkAlarmVsdUnderVoltage()
{
  float vsdStatus1 = getValue(VSD_STATUS_WORD_1);
  if (vsdStatus1 == VSD_ETALON_ALARM_UNDERVOLTAGE - 3000) {
    return VSD_ETALON_ALARM_UNDERVOLTAGE;
  }
  return VSD_ALARM_NONE;
}

float VsdEtalon::checkAlarmVsdOverVoltage()
{
  float vsdStatus1 = getValue(VSD_STATUS_WORD_1);
  if (vsdStatus1 == VSD_ETALON_ALARM_OVERVOLTAGE - 3000) {
    return VSD_ETALON_ALARM_OVERVOLTAGE;
  }
  return VSD_ALARM_NONE;
}



bool VsdEtalon::checkPreventVsd()
{
  if (parameters.get(CCS_VSD_ALARM_CODE))
    return true;
  return false;
}

// Метод проверки и обновления параметров устройства
void VsdEtalon::getNewValue(uint16_t id)
{
  float value = 0;
  // Преобразуем данные из полученного типа данных в float
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity == ok_r) {
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
  }
  else {
    value = NAN;
  }

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
      setValue(id, value);
      break;
    case VSD_ETALON_OFF_STATE:                // Получили подтверждение останова
      setValue(id, value);
      break;
    case VSD_STATUS_WORD_1:                 // Получили слово состояния
      setValue(id, value);
      break;
    case VSD_UF_CHARACTERISTIC_U_1_PERCENT:           // Получили точку напряжения U/f
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_1, roundf(BASE_VOLTAGE * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_2_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_2, roundf(BASE_VOLTAGE * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_3_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_3, roundf(BASE_VOLTAGE * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_4_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_4, roundf(BASE_VOLTAGE * value / 100.0));
      break;
    case VSD_UF_CHARACTERISTIC_U_5_PERCENT:
      setValue(id, value);
      setValue(VSD_UF_CHARACTERISTIC_U_5, roundf(BASE_VOLTAGE * value / 100.0));
      if (value > 100)
        ksu.setError(SetVoltageTapOfErr);
      break;
    case VSD_LOW_LIM_SPEED_MOTOR:
      setLimitsMinFrequence(value);
      break;
    case VSD_HIGH_LIM_SPEED_MOTOR:
      setLimitsMaxFrequence(value);
      break;
    case VSD_SOFT_VERSION_1:
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
    case VSD_FREQ_SKIP_START_FREQ:
      setValue(id, value);
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ) != value)
        parameters.set(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ, value);
      break;
    case VSD_FREQ_SKIP_STOP_FREQ:
      setValue(id, value);
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ) != value)
        parameters.set(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ, value);
      break;
    case VSD_FREQ_SKIP_ACCEL:
      setValue(id, value);
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_TEMP) != value)
        parameters.set(CCS_RGM_RUN_SKIP_RESONANT_TEMP, value);
      break;
    case VSD_ETALON_AUTO_OPTIM_MODE:
      setValue(id, value);
      parameters.set(CCS_RGM_OPTIM_VOLTAGE_MODE, value);
      break;
    case VSD_I_LIMIT_MODE:
      setValue(id, value);
      parameters.set(CCS_RGM_CURRENT_LIMIT_MODE, value);
      break;
    case VSD_ILIMIT:
      setValue(id, value);
      parameters.set(CCS_RGM_CURRENT_LIMIT_SETPOINT, value);
      break;
    case VSD_I_LIMIT_TM:
      setValue(id, value);
      parameters.set(CCS_RGM_CURRENT_LIMIT_DELAY_REACTION, value);
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
      break;
    case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
      setValue(id, value);
      break;
    case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
      setValue(id, value);
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
      if (parameters.get(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE) && !value)
        parameters.set(CCS_PROT_OTHER_VSD_NO_CONNECT_MODE, 0.0);
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
      if (!ksu.isWorkMotor())
        parameters.set(CCS_TURBO_ROTATION_NOW, value);
      else
        parameters.set(CCS_TURBO_ROTATION_NOW, 0);
      break;
    case VSD_DOOR_VALUE:
      setValue(id, value);
      parameters.set(CCS_DOOR_VALUE, value);
      break;
    case VSD_ETALON_RESISTANCE_ISOLATION:
      setValue(id, value);
      break;
    default:
      setValue(id, value);
      break;
    }
  }
}

uint8_t VsdEtalon::setNewValue(uint16_t id, float value, EventType eventType)
{
  int result;
  switch (id) {

  case VSD_MOTOR_TYPE:
    if (!setMotorType(value)) {
      readTransNeedVoltageTapOff();
      if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
        return setVsdControl(VSD_MOTOR_CONTROL_UF);
      }
      return ok_r;
    }
    return err_r;
    
  case  VSD_MOTOR_CONTROL:
    return setVsdControl(value);
    
  case VSD_LOW_LIM_SPEED_MOTOR:
    if (!setMinFrequency(value)) {
      if (getValue(VSD_LOW_LIM_SPEED_MOTOR) > getValue(VSD_FREQUENCY)) {
        return ksu.setFreq(getValue(VSD_LOW_LIM_SPEED_MOTOR), AutoType);
      }
      return ok_r;
    }
    return err_r;

  case VSD_HIGH_LIM_SPEED_MOTOR:
    if (!setMaxFrequency(value)) {
      if (getValue(VSD_HIGH_LIM_SPEED_MOTOR) < getValue(VSD_FREQUENCY)) {
        return ksu.setFreq(getValue(VSD_HIGH_LIM_SPEED_MOTOR), AutoType);
      }
      return ok_r;
    }
    return err_r;
    
  case VSD_FREQUENCY:
    return ksu.setFreq(value, eventType, false);

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
    return setUfU(VSD_UF_CHARACTERISTIC_U_1, VSD_UF_CHARACTERISTIC_U_1_PERCENT, value);

  case VSD_UF_CHARACTERISTIC_U_2:
    return setUfU(VSD_UF_CHARACTERISTIC_U_2, VSD_UF_CHARACTERISTIC_U_2_PERCENT, value);

  case VSD_UF_CHARACTERISTIC_U_3:
    return setUfU(VSD_UF_CHARACTERISTIC_U_3, VSD_UF_CHARACTERISTIC_U_3_PERCENT, value);

  case VSD_UF_CHARACTERISTIC_U_4:
    return setUfU(VSD_UF_CHARACTERISTIC_U_4, VSD_UF_CHARACTERISTIC_U_4_PERCENT, value);

  case VSD_UF_CHARACTERISTIC_U_5:
    return setUfU(VSD_UF_CHARACTERISTIC_U_5, VSD_UF_CHARACTERISTIC_U_5_PERCENT, value);

  case VSD_BASE_VOLTAGE:
    return setBaseVoltage(value);

  case VSD_BASE_FREQUENCY:
    return setBaseFrequency(value);

  case VSD_DEPTH:
  case VSD_TRANS_CABLE_CROSS:
  case VSD_MOTOR_VOLTAGE:
  case VSD_MOTOR_CURRENT:
  case VSD_BLDC_COEF_NORM_UF:
  case VSD_TEMPERATURE_INTAKE:
  case VSD_TRANS_VOLTAGE_TAP_OFF:
    result = setValue(id, value, eventType);
    if (!result) {
      writeToDevice(id, value);
      readTransNeedVoltageTapOff();
    }
    return result;

  default:
    result = setValue(id, value, eventType);
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

int VsdEtalon::start(bool init)
{
#if USE_DEBUG
  return ok_r;
#endif

  if (init) {
    if (getValue(VSD_ETALON_ON_STATE))
      return ok_r;

    startTimeMs_ = VSD_CMD_TIMEOUT;
    startCountRepeats_ = 0;
  }

  if (startTimeMs_ >= VSD_CMD_TIMEOUT) {
    startTimeMs_ = 0;
    startCountRepeats_++;

    if (startCountRepeats_ > VSD_CMD_NUMBER_REPEATS)
      return err_r;

    if (resetBlock())
      return err_r;
    if (setNewValue(VSD_ON, 1))
      return err_r;
  }
  else {
    startTimeMs_ += 10;
  }

  if (getValue(VSD_ETALON_ON_STATE))
    return ok_r;
  return -1;
}

bool VsdEtalon::checkStart()
{
#if USE_DEBUG
  return true;
#endif

  if (getValue(VSD_ETALON_ON_STATE)) {
    if (getValue(VSD_ETALON_OFF_STATE) == 0) {
      return true;
    }
  }
  return false;
}

int VsdEtalon::stop(bool isAlarm)
{
  // Если стоит бит остановки по внешней команде
  if (getValue(VSD_ETALON_OFF_STATE) == 1)
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (isAlarm) {
        setNewValue(VSD_ETALON_FAST_OFF, 1);
      }
      else {
        setNewValue(VSD_OFF, 1);
      }

      resetBlock();

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (getValue(VSD_ETALON_OFF_STATE) == 1) {
      return ok_r;
    }
  }
}

bool VsdEtalon::checkStop()
{
#if USE_DEBUG
  return true;
#endif

  if (getValue(VSD_ETALON_OFF_STATE)) {
    if (getValue(VSD_ETALON_ON_STATE) == 0) {
      return true;
    }
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
  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) == Regime::OffAction)
    return 1;

  setNewValue(VSD_FREQ_SKIP_START_FREQ, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ));
  setNewValue(VSD_FREQ_SKIP_STOP_FREQ, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ));
  setNewValue(VSD_FREQ_SKIP_ACCEL, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_TEMP));
  return setNewValue(VSD_FREQ_SKIP_MODE, 1);
}

int VsdEtalon::offRegimeSkipFreq()
{
  return setNewValue(VSD_FREQ_SKIP_MODE, 0);
}

int VsdEtalon::onRegimeAutoOptimCurrent()
{
  return setNewValue(VSD_ETALON_AUTO_OPTIM_MODE, 1);
}

int VsdEtalon::offRegimeAutoOptimCurrent()
{
  return setNewValue(VSD_ETALON_AUTO_OPTIM_MODE, 0);
}

int VsdEtalon::onRegimeCurrentLimitation()
{
  return setNewValue(VSD_I_LIMIT_MODE, 1);
}

int VsdEtalon::offRegimeCurrentLimitation()
{
  return setNewValue(VSD_I_LIMIT_MODE, 0);
}

int VsdEtalon::configRegimeCurrentLimitation()
{
  writeToDevice(VSD_ILIMIT, parameters.get(CCS_RGM_CURRENT_LIMIT_SETPOINT));
  writeToDevice(VSD_I_LIMIT_TM, parameters.get(CCS_RGM_CURRENT_LIMIT_DELAY_REACTION));
  return true;
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
    readTransNeedVoltageTapOff();
    return ok_r;
  }
  return err_r;
}

int VsdEtalon::setBaseFrequency(float value)
{
  if (!setValue(VSD_BASE_FREQUENCY, value)) {
    writeToDevice(VSD_BASE_FREQUENCY, value);
    if (!setValue(VSD_BLDC_MAX_WORK_FREQ, value)) {
      writeToDevice(VSD_BLDC_MAX_WORK_FREQ, value);
      readTransNeedVoltageTapOff();
      return ok_r;
    }  
  }
  return err_r;
}

void VsdEtalon::readTransNeedVoltageTapOff()
{
  osDelay(500);
  readInDevice(VSD_TRANS_NEED_VOLTAGE_TAP_OFF);
  osDelay(200);

  float value = getValue(VSD_BASE_VOLTAGE) + 10;
  if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_VENT)
    value = BASE_VOLTAGE*(getValue(VSD_TRANS_NEED_VOLTAGE_TAP_OFF)/getValue(VSD_TRANS_VOLTAGE_TAP_OFF)) + 10;

  setMax(VSD_UF_CHARACTERISTIC_U_1, value);
  setMax(VSD_UF_CHARACTERISTIC_U_2, value);
  setMax(VSD_UF_CHARACTERISTIC_U_3, value);
  setMax(VSD_UF_CHARACTERISTIC_U_4, value);
  setMax(VSD_UF_CHARACTERISTIC_U_5, value);
  readUfCharacterictic();
}

int VsdEtalon::setUfU(uint16_t idU, uint16_t idUPercent, float value)
{
  if (!Vsd::setUfU(idU, value)) {
    if (!setValue(idUPercent, (getValue(idU) * 100.0) / BASE_VOLTAGE)) {
      writeToDevice(idUPercent, getValue(idUPercent));
      return ok_r;
    }
  }
  return err_r;
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

void VsdEtalon::getConnect()
{
  Vsd::setConnect(dm_->getMms()->getCounters());
}

void VsdEtalon::resetConnect()
{
  Vsd::resetConnect();
  dm_->getMms()->resetCounters();
}

int VsdEtalon::onProtConnect()
{
  return setNewValue(VSD_PROT_NO_CONNECT_MODE, 1.0);
}

int VsdEtalon::offProtConnect()
{
  return setNewValue(VSD_PROT_NO_CONNECT_MODE, 0.0);
}

void VsdEtalon::setLimitsCcsParameters()
{
  parameters.setMax(CCS_COEF_OUT_CURRENT_1, getMax(VSD_COEF_OUT_CURRENT_1));
  parameters.setMax(CCS_COEF_OUT_CURRENT_2, getMax(VSD_COEF_OUT_CURRENT_2));
  parameters.setMax(CCS_COEF_OUT_CURRENT_3, getMax(VSD_COEF_OUT_CURRENT_3));
  // Задание пределов для "Режим ограничения тока уставка"
  parameters.setMin(CCS_RGM_CURRENT_LIMIT_SETPOINT, getMin(VSD_ILIMIT));
  parameters.setMax(CCS_RGM_CURRENT_LIMIT_SETPOINT, getMax(VSD_ILIMIT));
  // Задание пределов для Режим ограничения тока задержка срабатывания
  parameters.setMin(CCS_RGM_CURRENT_LIMIT_DELAY_REACTION, getMin(VSD_I_LIMIT_TM));
  parameters.setMax(CCS_RGM_CURRENT_LIMIT_DELAY_REACTION, getMax(VSD_I_LIMIT_TM));
  // Задание пределов для Режим Пропуска F
  parameters.setMin(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ, getMin(VSD_FREQ_SKIP_START_FREQ));
  parameters.setMax(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ, getMax(VSD_FREQ_SKIP_START_FREQ));
  parameters.setMin(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ, getMin(VSD_FREQ_SKIP_STOP_FREQ));
  parameters.setMax(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ, getMax(VSD_FREQ_SKIP_STOP_FREQ));
  parameters.setMin(CCS_RGM_RUN_SKIP_RESONANT_TEMP, getMin(VSD_FREQ_SKIP_ACCEL));
  parameters.setMax(CCS_RGM_RUN_SKIP_RESONANT_TEMP, getMax(VSD_FREQ_SKIP_ACCEL));
}

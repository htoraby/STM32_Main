/*
 * vsd_etalon.cpp
 *
 *  Created on: 13.04.2015
 *      Author: ablamtcev
 */

#include "vsd_etalon.h"
#include "user_main.h"

VsdEtalon::VsdEtalon()
{
  initModbusParameters();
  readParameters();
}

VsdEtalon::~VsdEtalon()
{
  // TODO Auto-generated destructor stub
}

void VsdEtalon::init()
{
  // Создание задачи обновления параметров
  createThread("UpdateParametersVsd");
  // Создание объекта протокола связи с утройством
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);
}

// Метод заполнения внутреннего банка параметров по карте устройства
void VsdEtalon::initParameters()
{
  Vsd::initParameters();
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int indexModbus = 0; indexModbus < count; indexModbus++) {         // Цикл по карте регистров
    int indexDevice = getIndexAtId(dm_->getFieldID(indexModbus));         // Получаем индекс параметра в банке параметров
    if (indexDevice) {                                                    // Если нашли параметр
      setFieldAccess(indexDevice, ACCESS_OPERATOR);                       // Уровень доступа оператор
      setFieldOperation(indexDevice, dm_->getFieldOperation(indexModbus));// Операции над параметром
      setFieldPhysic(indexDevice, dm_->getFieldPhysic(indexModbus));      // Физический смысл
      float tempVal = dm_->getFieldMinimum(indexModbus);                  // Получаем минимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldMin(indexDevice, tempVal);
      tempVal = dm_->getFieldMaximum(indexModbus);                        // Получаем мaксимум
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldMax(indexDevice, tempVal);
      tempVal = dm_->getFieldDefault(indexModbus);                        // Получаем значение по умолчанию
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));// Применяем коэффициент
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldDef(indexDevice, tempVal);
      setFieldValidity(indexDevice, dm_->getFieldValidity(indexModbus));  // Получили флаг валидности
      setFieldValue(indexDevice, getFieldDefault(indexDevice));           // Присвоили значение значению по умолчанию
    }
  }
}

bool VsdEtalon::isConnect()
{
  return dm_->isConnect();
}

// Метод проверки и обновления параметров устройства
void VsdEtalon::getNewValue(uint16_t id)
{
  float value = 0;
  // Преобразуем данные из полученного типа данных в float
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity == err_r) {
    value = 0xFFFF;
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
    setBitVsdStatus(VSD_STATUS_STARTED, value);
    setValue(id, value);
    break;
  case VSD_ETALON_OFF_STATE:                // Получили подтверждение останова
    setBitVsdStatus(VSD_STATUS_STOPPED_EXTERNAL, value);
    setValue(id, value);
    break;
  case VSD_INVERTOR_STATUS:                 // Получили слово состояния
    convertBitVsdStatus(value);
    break;
  case VSD_UF_CHARACTERISTIC_U_1:           // Получили точку напряжения U/f
  case VSD_UF_CHARACTERISTIC_U_2:
  case VSD_UF_CHARACTERISTIC_U_3:
  case VSD_UF_CHARACTERISTIC_U_4:
  case VSD_UF_CHARACTERISTIC_U_5:
    setValue(id, getValue(VSD_UF_CHARACTERISTIC_U_6) * value / 100.0);
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
    if (value > getValue(VSD_FREQUENCY))
      setFrequency(value);
    return setMinFrequency(value);
  case VSD_HIGH_LIM_SPEED_MOTOR:
    if (value < getValue(VSD_FREQUENCY))
      setFrequency(value);
    return setMaxFrequency(value);
  case  VSD_TEMP_SPEEDUP:
    return setTimeSpeedUp(value);
  case  VSD_TEMP_SPEEDDOWN:
    return setTimeSpeedDown(value);
  case  VSD_MOTOR_CONTROL:
    return setMotorControl(value);
  case VSD_UF_CHARACTERISTIC_U_1:
  case VSD_UF_CHARACTERISTIC_U_2:
  case VSD_UF_CHARACTERISTIC_U_3:
  case VSD_UF_CHARACTERISTIC_U_4:
  case VSD_UF_CHARACTERISTIC_U_5:
    return setUfU(id, value);
  case VSD_UF_CHARACTERISTIC_U_6:
    return setUfU6(value);
  case VSD_UF_CHARACTERISTIC_F_6:
    return setUfF6(value);
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

    if (setNewValue(VSD_ETALON_UNLOCK, 1))
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

int VsdEtalon::stop()
{
  /*
#if DEBUG
  return ok_r;
#endif
*/
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

      if (setNewValue(VSD_ETALON_OFF, 1))
        return err_r;
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (getValue(VSD_ETALON_OFF_STATE) == 1)
      return ok_r;
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

int VsdEtalon::onRegimePush()
{
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
  return result;
}

void VsdEtalon::readUfCharacterictic()
{
  readInDevice(VSD_UF_CHARACTERISTIC_F_1);
  readInDevice(VSD_UF_CHARACTERISTIC_F_2);
  readInDevice(VSD_UF_CHARACTERISTIC_F_3);
  readInDevice(VSD_UF_CHARACTERISTIC_F_4);
  readInDevice(VSD_UF_CHARACTERISTIC_F_5);
  readInDevice(VSD_UF_CHARACTERISTIC_U_1);
  readInDevice(VSD_UF_CHARACTERISTIC_U_2);
  readInDevice(VSD_UF_CHARACTERISTIC_U_3);
  readInDevice(VSD_UF_CHARACTERISTIC_U_4);
  readInDevice(VSD_UF_CHARACTERISTIC_U_5);
  //readInDevice(VSD_UF_CHARACTERISTIC_F_6);
  //readInDevice(VSD_UF_CHARACTERISTIC_U_6);
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

int VsdEtalon::setMainRegimeVSD()
{
  return 1;
}

void VsdEtalon::convertBitVsdStatus(float value)
{
  // Стираем биты в словах состояниях которыми управляем
  setBitVsdStatus(VSD_STATUS_READY, false);
  setBitVsdStatus(VSD_STATUS_UNDERLOAD, false);
  setBitVsdStatus(VSD_STATUS_M_I2T_ERR, false);
  setBitVsdStatus(VSD_STATUS_RESISTANCE, false);
  setBitVsdStatus(VSD_STATUS_UNDERVOLTAGE, false);
  setBitVsdStatus(VSD_STATUS_OVERVOLTAGE, false);
  setBitVsdStatus(VSD_STATUS_UD_LOW_FAULT, false);
  setBitVsdStatus(VSD_STATUS_UD_HIGH_FAULT, false);
  setBitVsdStatus(VSD_STATUS_RUN_COUNT, false);
  setBitVsdStatus(VSD_STATUS_FC_I2T_ERR, false);
  setBitVsdStatus(VSD_STATUS_OVERHEAT_FILTER, false);
  setBitVsdStatus(VSD_STATUS_STOPPED_ALARM, false);
  setBitVsdStatus(VSD_STATUS_DRV0, false);
  setBitVsdStatus(VSD_STATUS_MONOMETR, false);
  setBitVsdStatus(VSD_STATUS_AI_0, false);
  setBitVsdStatus(VSD_THYR_ABC_STATE, false);
  setBitVsdStatus(VSD_STATUS_OVERHEAT_MOTOR, false);
  setBitVsdStatus(VSD_STATUS_OVERVIBRATION, false);
  setBitVsdStatus(VSD_STATUS_PRESSURE, false);
  setBitVsdStatus(VSD_STATUS_FAULT_STOPPED, false);
  setBitVsdStatus(VSD_STATUS_I_LIMIT, false);
  setBitVsdStatus(VSD_STATUS_CLK_MON, false);
  setBitVsdStatus(VSD_STATUS_CTR_MON, false);
  setBitVsdStatus(VSD_STATUS_AN_MON, false);
  setBitVsdStatus(VSD_STATUS_MB_MON, false);
  setBitVsdStatus(VSD_STATUS_CLK_MON, false);
  switch ((uint16_t)value) {
  case VSD_ETALON_INFO_READY:             // VSD_STATUS_READY
    setBitVsdStatus(VSD_STATUS_READY, true);
    break;
  case VSD_ETALON_INFO_UNDERLOAD:         // VSD_STATUS_UNDERLOAD
    setBitVsdStatus(VSD_STATUS_UNDERLOAD, true);
    break;
  case VSD_ETALON_INFO_OVERLOAD:          // VSD_STATUS_M_I2T_ERR
    setBitVsdStatus(VSD_STATUS_M_I2T_ERR, true);
    break;
  case VSD_ETALON_INFO_RESISTANCE:        // VSD_STATUS_RESISTANCE
    setBitVsdStatus(VSD_STATUS_RESISTANCE, true);
    break;
  case VSD_ETALON_INFO_UNDERVOLTAGE:      // VSD_STATUS_UNDERVOLTAGE
    setBitVsdStatus(VSD_STATUS_UNDERVOLTAGE, true);
    break;
  case VSD_ETALON_INFO_OVERVOLTAGE:       // VSD_STATUS_OVERVOLTAGE
    setBitVsdStatus(VSD_STATUS_OVERVOLTAGE, true);
    break;
  case VSD_ETALON_INFO_OVERVOLTAGE_DC:    // VSD_STATUS_UD_LOW_FAULT
    setBitVsdStatus(VSD_STATUS_UD_LOW_FAULT, true);
    break;
  case VSD_ETALON_INFO_UNDERVOLTAGE_DC:   // VSD_STATUS_UD_HIGH_FAULT
    setBitVsdStatus(VSD_STATUS_UD_HIGH_FAULT, true);
    break;
  case VSD_ETALON_INFO_RUN_COUNT:         // VSD_STATUS_RUN_COUNT
    setBitVsdStatus(VSD_STATUS_RUN_COUNT, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_IGBT:     // VSD_STATUS_FC_I2T_ERR
    setBitVsdStatus(VSD_STATUS_FC_I2T_ERR, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_FILTER:   // VSD_STATUS_OVERHEAT_FILTER
    setBitVsdStatus(VSD_STATUS_OVERHEAT_FILTER, true);
    break;
  case VSD_ETALON_INFO_PROT:              // VSD_STATUS_STOPPED_ALARM
    setBitVsdStatus(VSD_STATUS_STOPPED_ALARM, true);
    break;
  case VSD_ETALON_INFO_SUPPLY_DRIVERS:    // VSD_FLT_DRV0
    setBitVsdStatus(VSD_STATUS_DRV0, true);
    break;
  case VSD_ETALON_INFO_MONOMETR:          // VSD_STATUS_MONOMETR
    setBitVsdStatus(VSD_STATUS_MONOMETR, true);
    break;
  case VSD_ETALON_INFO_AI_0:              // VSD_STATUS_AI_0
    setBitVsdStatus(VSD_STATUS_AI_0, true);
    break;
  case VSD_ETALON_INFO_SEQUENCE_PHASE:    // VSD_THYR_ABC_STATE
    setBitVsdStatus(VSD_THYR_ABC_STATE, true);
    break;
  case VSD_ETALON_INFO_OVERHEAT_MOTOR:    // VSD_STATUS_OVERHEAT_MOTOR
    setBitVsdStatus(VSD_STATUS_OVERHEAT_MOTOR, true);
    break;
  case VSD_ETALON_INFO_OVERVIBRATION:     // VSD_STATUS_OVERVIBRATION
    setBitVsdStatus(VSD_STATUS_OVERVIBRATION, true);
    break;
  case VSD_ETALON_INFO_PRESSURE:          // VSD_STATUS_PRESSURE
    setBitVsdStatus(VSD_STATUS_PRESSURE, true);
    break;
  case VSD_ETALON_INFO_19:                // VSD_STATUS_FAULT_STOPPED
    setBitVsdStatus(VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_IMBALANCE_CURRENT: // VSD_FLT_IZ
    setBitVsdStatus(VSD_STATUS_IZ, true);
    break;
  case VSD_ETALON_INFO_IMBALANCE_VOLTAGE: // VSD_STATUS_UIN_ASYM
    setBitVsdStatus(VSD_STATUS_UIN_ASYM, true);
    break;
  case VSD_ETALON_INFO_TURBINE:           // VSD_STATUS_TURBINE
    setBitVsdStatus(VSD_STATUS_TURBINE, true);
    break;
  case VSD_ETALON_INFO_24:                // VSD_STATUS_FAULT_STOPPED
    setBitVsdStatus(VSD_STATUS_FAULT_STOPPED, true);
    break;
  case VSD_ETALON_INFO_FAILURE_SUPPLY:
    break;
  case VSD_ETALON_INFO_DOOR:
    break;
  case VSD_ETALON_INFO_LOST_SUPPLY:
    break;
  case VSD_ETALON_INFO_CONDENSATOR:
    break;
  case VSD_ETALON_INFO_TERISTORS:
    break;
  case VSD_ETALON_INFO_CURRENT_LIMIT:     // VSD_STATUS_I_LIMIT
    setBitVsdStatus(VSD_STATUS_I_LIMIT, true);
    break;
  case VSD_ETALON_INFO_31:                // VSD_STATUS_FAULT_STOPPED
    setBitVsdStatus(VSD_STATUS_FAULT_STOPPED, true);
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
    setBitVsdStatus(VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_INFO_41:
    break;
  case VSD_ETALON_INFO_DI:                // VSD_STATUS_CTR_MON
    setBitVsdStatus(VSD_STATUS_CTR_MON, true);
    break;
  case VSD_ETALON_INFO_ADC:               // VSD_STATUS_AN_MON
    setBitVsdStatus(VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_INFO_ANALOG_SUPPLY:     // VSD_STATUS_AN_MON
    setBitVsdStatus(VSD_STATUS_AN_MON, true);
    break;
  case VSD_ETALON_INFO_SENSOR_SUPPLY:     // VSD_STATUS_MB_MON
    setBitVsdStatus(VSD_STATUS_MB_MON, true);
    break;
  case VSD_ETALON_INFO_EEPROM:            // VSD_STATUS_CLK_MON
    setBitVsdStatus(VSD_STATUS_CLK_MON, true);
    break;
  case VSD_ETALON_INFO_NOT_READY:
    break;
  case VSD_ETALON_INFO_SETPOINT:
    break;
  case VSD_ETALON_INFO_BLOCK_RUN:
    break;
  default:
    break;
  }
}

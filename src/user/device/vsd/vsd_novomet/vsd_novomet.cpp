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
                         VSD_UART, 19200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);

  initParameters();
  readParameters();
  setLimitsFrequence(0, getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsFrequence(1, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
}

void VsdNovomet::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int indexModbus = 0; indexModbus < count; indexModbus++) {         // Цикл по карте регистров
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

// Метод проверки и обновления параметров устройства
void VsdNovomet::getNewValue(uint16_t id)
{
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
  case VSD_CURRENT_OUT_PHASE_1:             // Выходной ток ЧРП Фаза 1
    setValue(id, getValue(VSD_COEF_OUT_CURRENT_1) * value);
    break;
  case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
    setValue(id, getValue(VSD_COEF_OUT_CURRENT_2) * value);
    break;
  case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
    setValue(id, getValue(VSD_COEF_OUT_CURRENT_3) * value);
    break;
  case VSD_INVERTOR_STATUS:
    setValue(id, value);
    calcRotation();
    parameters.set(CCS_VSD_INVERTOR_STATUS_1, value);
    break;
  case VSD_INVERTOR_STATUS2:
    setValue(id, value);
    calcMotorType();
    parameters.set(CCS_VSD_INVERTOR_STATUS_2, value);
    break;
  case VSD_INVERTOR_STATUS3:
    setValue(id, value);
    parameters.set(CCS_VSD_INVERTOR_STATUS_3, value);
    break;
  case VSD_INVERTOR_STATUS4:
    setValue(id, value);
    parameters.set(CCS_VSD_INVERTOR_STATUS_4, value);
    break;
  case VSD_INV_FAULT:
    setValue(id, value);
    parameters.set(CCS_VSD_INV_FAULT, value);
    break;
  case VSD_THYR_STATUS:
    setValue(id, value);
    parameters.set(CCS_VSD_THYR_STATUS, value);
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
    calcVsdCos();
    calcCurrentDC();
    break;
  case VSD_POWER_FULL:
    setValue(id, value);
    calcVsdCos();
    break;
  case VSD_LOW_LIM_SPEED_MOTOR:
    setValue(id, value);
    setLimitsFrequence(0, value);
    break;
  case VSD_HIGH_LIM_SPEED_MOTOR:
    setValue(id, value);
    setLimitsFrequence(1, value);
    break;
  default:                                  // Прямая запись в массив параметров
    setValue(id, value);
    break;
  }
}

uint8_t VsdNovomet::setNewValue(uint16_t id, float value)
{
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
  case VSD_TIMER_DISPERSAL:
    return setTimeSpeedUp(value);
  case VSD_TIMER_DELAY:
    return setTimeSpeedDown(value);
  case VSD_MOTOR_VOLTAGE:
    ksu.calcTransRecommendedTapOff();
    return ok_r;
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

int VsdNovomet::start()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
  if (checkVsdStatus(VSD_STATUS_STARTED))
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

      if (setNewValue(VSD_INVERTOR_CONTROL, INV_CONTROL_START))
        return err_r;
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkVsdStatus(VSD_STATUS_STARTED)) {
      return ok_r;
    }
  }
}

bool VsdNovomet::checkStart()
{
#if USE_DEBUG
  return true;
#endif

  if (checkVsdStatus(VSD_STATUS_STARTED)) {
    if (!checkVsdStatus(VSD_STATUS_WAIT_RECT_START)) {
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
  if (checkVsdStatus(VSD_STATUS_STOPPED_EXTERNAL))
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
        if (setNewValue(VSD_INVERTOR_CONTROL, INV_CONTROL_ALARM))
          return err_r;
        break;
      default:
        if (setNewValue(VSD_INVERTOR_CONTROL, INV_CONTROL_STOP))
          return err_r;
       break;
      }
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkVsdStatus(VSD_STATUS_STOPPED_EXTERNAL))
      return ok_r;
  }
}

int VsdNovomet::alarmstop()
{
  // Если стоит бит остановки по внешней команде
  if (checkVsdStatus(VSD_STATUS_STOPPED_ALARM))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_INVERTOR_CONTROL, INV_CONTROL_ALARM))
        return err_r;
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkVsdStatus(VSD_STATUS_STOPPED_EXTERNAL))
      return ok_r;
  }
}

bool VsdNovomet::checkStop()
{
#if USE_DEBUG
  return true;
#endif

  if (checkVsdStatus(VSD_STATUS_STOPPED_EXTERNAL)) {
    if (!checkVsdStatus(VSD_STATUS_STARTED)) {
      if (!checkVsdStatus(VSD_STATUS_WAIT_RECT_STOP)) {
        return true;
      }
    }
  }
  return false;
}

void VsdNovomet::processingRegimeRun()
{
  regimeRun_->processing();
}

int VsdNovomet::setFrequency(float value)
{
  if (Vsd::setFrequency(value))
    return err_r;
  else {
    writeToDevice(VSD_FREQUENCY, value);
    return ok_r;
  }
}

int VsdNovomet::setMinFrequency(float value)
{
  if (Vsd::setMinFrequency(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
}

int VsdNovomet::setMaxFrequency(float value)
{
  if (Vsd::setMaxFrequency(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
}

int VsdNovomet::setMotorType(float value)
{
  if (Vsd::setMotorType(value)) {           // Записываем тип двигателя в массив
    logDebug.add(WarningMsg, "setTypeMotor");
    return err_r;                           // Если не записали возвращаем ошибку
  }
  else {
    if (value == VSD_MOTOR_TYPE_ASYNC) {
      writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_ASYN_MOTOR);
    }
    else {
      writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_VENT_MOTOR);
    }
    return ok_r;
  }
}

void VsdNovomet::calcMotorType()
{
  if (checkVsdStatus(VSD_STATUS_M_TYPE0)) {
    setValue(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  }
  else {
    setValue(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  }
}

void VsdNovomet::calcTempSpeedUp()
{
  setValue(VSD_TEMP_SPEEDUP, 1/getValue(VSD_T_SPEEDUP));
}

void VsdNovomet::calcTimeSpeedUp()
{
  setValue(VSD_TIMER_DISPERSAL, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDUP));
}

void VsdNovomet::calcTempSpeedDown()
{
  setValue(VSD_TEMP_SPEEDDOWN, 1/getValue(VSD_T_SPEEDDOWN));
}

void VsdNovomet::calcTimeSpeedDown()
{
  setValue(VSD_TIMER_DELAY, getValue(VSD_FREQUENCY)/getValue(VSD_TEMP_SPEEDDOWN));
}

void VsdNovomet::calcRotation()
{
  if (checkVsdStatus(VSD_STATUS_RIGHT_DIRECTION)) {
    setValue(VSD_ROTATION, VSD_ROTATION_DIRECT);
  }
  else {
    setValue(VSD_ROTATION, VSD_ROTATION_REVERSE);
  }
}

void VsdNovomet::calcCurrentDC()
{
  float pwr = getValue(VSD_POWER_ACTIVE);
  float volt = getValue(VSD_VOLTAGE_DC);
  if (volt > 0) {
    setValue(VSD_CURRENT_DC, pwr/volt);
  }
}

int VsdNovomet::setTimeSpeedUp(float value)
{
  if (Vsd::setTimeSpeedUp(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_T_SPEEDUP, getValue(VSD_T_SPEEDUP));
    return ok_r;
  }
}

int VsdNovomet::setTimeSpeedDown(float value)
{
  if (Vsd::setTimeSpeedDown(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_T_SPEEDDOWN, getValue(VSD_T_SPEEDDOWN));
    return ok_r;
  }
}

int VsdNovomet::setTempSpeedUp(float value)
{
  if (Vsd::setTempSpeedUp(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_T_SPEEDUP, getValue(VSD_T_SPEEDUP));
    return ok_r;
  }
}

int VsdNovomet::setTempSpeedDown(float value)
{
  if (Vsd::setTempSpeedDown(value)) {
    return err_r;
  }
  else {
    writeToDevice(VSD_T_SPEEDDOWN, getValue(VSD_T_SPEEDDOWN));
    return ok_r;
  }
}

int VsdNovomet::setRotation(float value)
{
  if (value == VSD_ROTATION_DIRECT) {
    writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_RIGHT_DIRECTION);
  }
  else {
    writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_LEFT_DIRECTION);
  }
  return ok_r;
}

int VsdNovomet::setUfU1(float value)
{
  return Vsd::setUfU1(value);
}

int VsdNovomet::setUfU2(float value)
{
  return Vsd::setUfU2(value);
}

int VsdNovomet::setUfU3(float value)
{
  return Vsd::setUfU3(value);
}

int VsdNovomet::setUfU4(float value)
{
  return Vsd::setUfU4(value);
}

int VsdNovomet::setUfU5(float value)
{
  return Vsd::setUfU5(value);
}

void VsdNovomet::calcParameters(uint16_t id)
{

}

int VsdNovomet::onRegimePush()
{
  float freq = parameters.get(CCS_RGM_RUN_PUSH_FREQ);       // Получаем частоту толчков
  float numPush = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);// Получаем количество толчков
  float calcNumPush = 0;
  uint16_t rotation = 1;
  if (freq) {
    calcNumPush = (8.0 / freq);                             // Количество толчков на указанной частоте
  }                                                         // За один оборот двигателя
  else {
    calcNumPush = numPush;
  }
  rotation = (uint16_t)((numPush / calcNumPush) + 0.5);     // Вычисляем требуемое количество оборотов

  float impulse = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE); // Превышение напряжения
  impulse = (uint16_t)((impulse - 100) / 10.0 + 0.5);       // Вычислили уставку напряжения
  if ((impulse < 1.0) || (impulse > 10))
    impulse = 1.0;

  setNewValue(VSD_START_FREQ, freq);                        // Задаём частоту основного режима
  setNewValue(VSD_SW_STARTUP_FREQUENCY, freq);              // Записали в ЧРП частоту
  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, 180.0);             // Угол константа
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, 1.0);               // Количество качаний константа
  setNewValue(VSD_SW_STARTUP_ROTATIONS, rotation);          // Записали количество оборотов
  setNewValue(VSD_SW_STARTUP_U_PULSE, impulse);             // Записали кратность импульса толчка
  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, 1500.0);   // Записали предел тока
}

int VsdNovomet::onRegimePickup()
{
  return 0;
}

int VsdNovomet::offRegimePush()
{
  return 0;
}

int VsdNovomet::onRegimeSwing()
{
  float freq = parameters.get(CCS_RGM_RUN_SWING_FREQ);       // Получаем частоту толчков
  float numPush = parameters.get(CCS_RGM_RUN_SWING_QUANTITY);// Получаем количество толчков
  float calcNumPush = 0;
  uint16_t rotation = 1;
  if (freq) {
    calcNumPush = (8.0 / freq);                             // Количество толчков на указанной частоте
  }                                                         // За один оборот двигателя
  else {
    calcNumPush = numPush;
  }
  rotation = (uint16_t)((numPush / calcNumPush) + 0.5);     // Вычисляем требуемое количество оборотов

  float impulse = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE); // Превышение напряжения
  impulse = (uint16_t)((impulse - 100) / 10.0 + 0.5);       // Вычислили уставку напряжения
  if ((impulse < 1.0) || (impulse > 10))
    impulse = 1.0;

  setNewValue(VSD_START_FREQ, freq);                        // Задаём частоту основного режима
  setNewValue(VSD_SW_STARTUP_FREQUENCY, freq);              // Записали в ЧРП частоту
  setNewValue(VSD_SW_STARTUP_ANGLE_OSC, 359.0);             // Угол константа
  setNewValue(VSD_SW_STARTUP_OSC_COUNT, 2.0);               // Количество качаний константа
  setNewValue(VSD_SW_STARTUP_ROTATIONS, rotation);          // Записали количество оборотов
  setNewValue(VSD_SW_STARTUP_U_PULSE, impulse);             // Записали кратность импульса толчка
  return setNewValue(VSD_SW_STARTUP_I_LIM_PULSE, 1500.0);   // Записали предел тока
}

int VsdNovomet::offRegimeSwing()
{
  return 0;
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
                parameters.get(CCS_RGM_RUN_SWING_MODE));
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

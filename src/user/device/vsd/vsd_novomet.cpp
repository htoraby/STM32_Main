/*
 * vsd_novomet.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd_novomet.h"
#include "user_main.h"
#include "regime_main.h"

VsdNovomet::VsdNovomet()
{

}

VsdNovomet::~VsdNovomet()
{
  // TODO Auto-generated destructor stub
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
}

void VsdNovomet::initParameters()
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

bool VsdNovomet::isConnect()
{
  return dm_->isConnect();
}

// Метод проверки и обновления параметров устройства
void VsdNovomet::getNewValue(uint16_t id)
{
  float value = 0;
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity == err_r) {
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
  value = value * param->coefficient;
  value = (value - (units[param->physic][param->unit][1]))/(units[param->physic][param->unit][0]);
  setValue(id, value);
  calcParameters(id);
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
    if (value > getValue(VSD_FREQUENCY))
      setFrequency(value);
    return setMinFrequency(value);
  case VSD_HIGH_LIM_SPEED_MOTOR:
    if (value < getValue(VSD_FREQUENCY)) {
      setFrequency(value);
      ksu.calcTransTapOff();
    }
    return setMaxFrequency(value);
  case VSD_TIMER_DISPERSAL:
    return setTimeSpeedUp(value);
  case VSD_TIMER_DELAY:
    return setTimeSpeedDown(value);
  case VSD_MOTOR_VOLTAGE:
    ksu.calcTransTapOff();
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

int VsdNovomet::stop()
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

      if (setNewValue(VSD_INVERTOR_CONTROL, INV_CONTROL_STOP))
        return err_r;
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
  if (value == VSD_MOTOR_TYPE_ASYNC) {
    writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_ASYN_MOTOR);
  }
  else {
    writeToDevice(VSD_INVERTOR_CONTROL, INV_CONTROL_VENT_MOTOR);
  }
  return ok_r;
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

int VsdNovomet::setSwitchingFrequency(float value)
{
  if(Vsd::setSwitchingFrequency(value)){
    return err_r;
  }
  else {
    writeToDevice(VSD_SWITCHING_FREQUENCY, value);
    return ok_r;
  }
}

void VsdNovomet::calcParameters(uint16_t id)
{
  switch (id) {
  case VSD_INVERTOR_STATUS2:
    calcMotorType();
    break;
  case VSD_T_SPEEDUP:
    calcTempSpeedUp();
    calcTimeSpeedUp();
    break;
  case VSD_INVERTOR_STATUS:
    calcRotation();
    break;
  case  VSD_VOLTAGE_DC:
    calcCurrentDC();
    break;
  case  VSD_POWER_ACTIVE:
    calcCurrentDC();
    break;
  default:

    break;
  }
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

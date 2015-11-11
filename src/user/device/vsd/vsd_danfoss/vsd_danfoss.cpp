#include "vsd_danfoss.h"
#include "user_main.h"

VsdDanfoss::VsdDanfoss()
{

}

VsdDanfoss::~VsdDanfoss()
{

}

void VsdDanfoss::initParameters()
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

void VsdDanfoss::init()
{
  initModbusParameters();

  // Создание задачи обновления параметров
  createThread("UpdateParamsVsd");
  // Создание объекта протокола связи с утройством
  int16_t count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);

  initParameters();
  readParameters();

  setLimitsMinFrequence(getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsMaxFrequence(getValue(VSD_HIGH_LIM_SPEED_MOTOR));
}

bool VsdDanfoss::isConnect()
{
  bool curConnect = dm_->isConnect();

  if (prevConnect_ && !curConnect) {
    int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
    // Цикл по карте регистров
    for (int indexModbus = 0; indexModbus < count; indexModbus++) {
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
int VsdDanfoss::setMotorType(float value)
{
  if (!Vsd::setMotorType(value)) {
    configVsd();
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMotorType");
    return err_r;
  }
}

int VsdDanfoss::setMotorSpeed(float value)
{
  if (!Vsd::setMotorSpeed(value)) {
    configVsd();
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMotorSpeed");
    return err_r;
  }
}

int VsdDanfoss::setVsdControl(float value)
{
  if (!Vsd::setVsdControl(value)) {
    configVsd();
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setVsdControl");
    return err_r;
  }
}

// ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
int VsdDanfoss::setRotation(float value)
{
  if(Vsd::setRotation(value)){
    logDebug.add(WarningMsg, "VsdDanfoss::setRotation");
    return err_r;
  }
  else {
    writeToDevice(VSD_ROTATION, parameters.get(VSD_ROTATION));
    return ok_r;
  }
}

int VsdDanfoss::setMinFrequency(float value)
{
  if (!Vsd::setMinFrequency(value)) {
    writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, getValue(VSD_LOW_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMinFrequency");
    return err_r;
  }
}

int VsdDanfoss::setMaxFrequency(float value)
{
  if (!Vsd::setMaxFrequency(value)) {
    writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMaxFrequency");
    return err_r;
  }
}

int VsdDanfoss::setFrequency(float value)
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

int VsdDanfoss::setTimeSpeedUp(float value)
{
  if (!Vsd::setTimeSpeedUp(value)) {
    writeToDevice(VSD_TIMER_DISPERSAL, getValue(VSD_TIMER_DISPERSAL));
    writeToDevice(VSD_TIMER_DISP_FIX_SPEED, getValue(VSD_TIMER_DISPERSAL));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setTimeSpeedUp");
    return err_r;
  }    
}

int VsdDanfoss::setTimeSpeedDown(float value)
{
  if (!Vsd::setTimeSpeedDown(value)) {
    writeToDevice(VSD_TIMER_DELAY, getValue(VSD_TIMER_DELAY));
    writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, getValue(VSD_TIMER_DELAY));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setTimeSpeedDown");
    return err_r;
  }
}

int VsdDanfoss::setSwitchingFrequencyCode(float value)
{
  if (!Vsd::setSwitchingFrequencyCode(value)) {
    writeToDevice(VSD_SWITCHING_FREQUENCY_CODE, getValue(VSD_SWITCHING_FREQUENCY_CODE));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setSwitchingFrequencyCode");
    return err_r;
  }
}

int VsdDanfoss::setOutFilter(float value)
{
  if (!Vsd::setOutFilter(value)) {
    writeToDevice(VSD_OUT_FILTER, getValue(VSD_OUT_FILTER));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setOutFilter");
    return err_r;
  }
}

// НАСТРОЙКА U/f
int VsdDanfoss::setUf_f1(float value)
{
  if (!Vsd::setUf_f1(value)) {
    writeUf_F(0, getValue(VSD_UF_CHARACTERISTIC_F_1));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f1");
    return err_r;
  }
}

int VsdDanfoss::setUf_f2(float value)
{
  if (!Vsd::setUf_f2(value)) {
    writeUf_F(1, getValue(VSD_UF_CHARACTERISTIC_F_2));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f2");
    return err_r;
  }
}

int VsdDanfoss::setUf_f3(float value)
{
  if (!Vsd::setUf_f3(value)) {
    writeUf_F(2, getValue(VSD_UF_CHARACTERISTIC_F_3));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f3");
    return err_r;
  }
}

int VsdDanfoss::setUf_f4(float value)
{
  if (!Vsd::setUf_f4(value)) {
    writeUf_F(3, getValue(VSD_UF_CHARACTERISTIC_F_4));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f4");
    return err_r;
  }
}

int VsdDanfoss::setUf_f5(float value)
{
  if (!Vsd::setUf_f5(value)) {
    writeUf_F(4, getValue(VSD_UF_CHARACTERISTIC_F_5));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f5");
    return err_r;
  }
}

int VsdDanfoss::setUf_f6(float value)
{
  if (!Vsd::setUf_f6(value)) {
    writeUf_F(5, getValue(VSD_UF_CHARACTERISTIC_F_6));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_f6");
    return err_r;
  }
}

int VsdDanfoss::setUf_U1(float value)
{
  if (!Vsd::setUf_U1(value)) {
    writeUf_U(0, getValue(VSD_UF_CHARACTERISTIC_U_1));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U1");
    return err_r;
  }
}

int VsdDanfoss::setUf_U2(float value)
{
  if (!Vsd::setUf_U2(value)) {
    writeUf_U(1, getValue(VSD_UF_CHARACTERISTIC_U_2));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U2");
    return err_r;
  }
}

int VsdDanfoss::setUf_U3(float value)
{
  if (!Vsd::setUf_U3(value)) {
    writeUf_U(2, getValue(VSD_UF_CHARACTERISTIC_U_3));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U3");
    return err_r;
  }
}

int VsdDanfoss::setUf_U4(float value)
{
  if (!Vsd::setUf_U4(value)) {
    writeUf_U(3, getValue(VSD_UF_CHARACTERISTIC_U_4));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U4");
    return err_r;
  }
}

int VsdDanfoss::setUf_U5(float value)
{
  if (!Vsd::setUf_U5(value)) {
    writeUf_U(4, getValue(VSD_UF_CHARACTERISTIC_U_5));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U5");
    return err_r;
  }
}

int VsdDanfoss::setUf_U6(float value)
{
  if (!Vsd::setUf_U6(value)) {
    writeUf_U(5, getValue(VSD_UF_CHARACTERISTIC_U_6));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U6");
    return err_r;
  }
}

void VsdDanfoss::readUf_F(uint16_t numPoint)
{
  if (getValue(VSD_INDEX) != numPoint) {
    writeToDevice(VSD_INDEX, numPoint);
  }
  readInDevice(VSD_UF_CHARACTERISTIC_F);
}

void VsdDanfoss::writeUf_F(uint16_t numPoint, float value)
{
  if (getValue(VSD_INDEX) != numPoint) {
    writeToDevice(VSD_INDEX, numPoint);
  }
  writeToDevice(VSD_UF_CHARACTERISTIC_F, value);
}

void VsdDanfoss::readUf_U(uint16_t numPoint)
{
  if (getValue(VSD_INDEX) != numPoint) {
    writeToDevice(VSD_INDEX, numPoint);
  }
  readInDevice(VSD_UF_CHARACTERISTIC_U);
}

void VsdDanfoss::writeUf_U(uint16_t numPoint, float value)
{
  if (getValue(VSD_INDEX) != numPoint) {
    writeToDevice(VSD_INDEX, numPoint);
  }
  writeToDevice(VSD_UF_CHARACTERISTIC_U, value);
}

void VsdDanfoss::readUfCharacterictic()
{
  for (uint8_t i = 0; i < 6; i++) {
    readUf_F(i);
    readUf_U(i);
  }
}

uint16_t VsdDanfoss::configVsd()
{
  uint16_t typeMotor = parameters.get(VSD_MOTOR_TYPE);
  uint16_t typeControl = parameters.get(VSD_MOTOR_CONTROL);
  uint16_t nomSpeed = parameters.get(VSD_MOTOR_SPEED);
  switch (typeMotor) {
  case VSD_MOTOR_TYPE_ASYNC:
    return configVsdAsync();
  case VSD_MOTOR_TYPE_VENT:
    switch (typeControl) {
    case VSD_MOTOR_CONTROL_UF:
      if (nomSpeed <= 500) {
        return configVsdVentUf500();
      }
      else {
        if (nomSpeed <= 1000) {
          return configVsdVentUf1000();
        }
        else {
          if (nomSpeed <= 3000) {
            return configVsdVentUf3000();
          }
          else {
            if (nomSpeed <= 6000) {
              return configVsdVentUf6000();
            }
            return err_r;
          }

        }
      }
    case VSD_MOTOR_CONTROL_VECT:
      if (nomSpeed <= 500) {
        return configVsdVentVect500();
      }
      else {
        if (nomSpeed <= 1000) {
          return configVsdVentVect1000();
        }
        else {
          if (nomSpeed <= 3000) {
            return configVsdVentVect3000();
          }
          else {
            if (nomSpeed <= 6000) {
              return configVsdVentVect6000();
            }
            return err_r;
          }
        }
      }
    default:
      return err_r;
    }
    break;
  default:
    return err_r;
  }
}

uint16_t VsdDanfoss::configVsdAsync()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  writeToDevice(VSD_MOTOR_FREQUENCY, 50);
  writeToDevice(VSD_MOTOR_SPEED, 3000);
  writeToDevice(VSD_MOTOR_POLES, 2);
  setUf_f1(0);
  setUf_U1(5);
  setUf_f2(10);
  setUf_U2(68);
  setUf_f3(20);
  setUf_U3(131);
  setUf_f4(30);
  setUf_U4(194);
  setUf_f5(40);
  setUf_U5(257);
  setUf_f6(50);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 0);
  writeToDevice(VSD_RESONANCE_TIME, 2);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 100);
  writeToDevice(VSD_PM_START_MODE, 0);
  writeToDevice(VSD_STOP_FUNCTION, 0);
  writeToDevice(VSD_MIN_REFERENCE, 30);
  writeToDevice(VSD_MAX_REFERENCE, 70);
  writeToDevice(VSD_FREQUENCY, 50);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 71);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 30);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 50);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 230);
  writeToDevice(VSD_WARNING_SPEED_LOW, 650);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.1);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf500()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);
  writeToDevice(VSD_MOTOR_SPEED, 1000);
  writeToDevice(VSD_MOTOR_POLES, 14);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(23);
  setUf_U2(74);
  setUf_f3(46);
  setUf_U3(136);
  setUf_f4(70);
  setUf_U4(197);
  setUf_f5(93);
  setUf_U5(259);
  setUf_f6(117);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 0);
  writeToDevice(VSD_STOP_FUNCTION, 0);
  writeToDevice(VSD_MIN_REFERENCE, 80);
  writeToDevice(VSD_MAX_REFERENCE, 117);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf1000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);
  writeToDevice(VSD_MOTOR_SPEED, 1000);
  writeToDevice(VSD_MOTOR_POLES, 14);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(23);
  setUf_U2(74);
  setUf_f3(46);
  setUf_U3(136);
  setUf_f4(70);
  setUf_U4(197);
  setUf_f5(93);
  setUf_U5(259);
  setUf_f6(117);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 0);
  writeToDevice(VSD_STOP_FUNCTION, 0);
  writeToDevice(VSD_MIN_REFERENCE, 80);
  writeToDevice(VSD_MAX_REFERENCE, 117);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf3000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  writeToDevice(VSD_MOTOR_FREQUENCY, 100);
  writeToDevice(VSD_MOTOR_SPEED, 3000);
  writeToDevice(VSD_MOTOR_POLES, 4);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(20);
  setUf_U2(74);
  setUf_f3(40);
  setUf_U3(136);
  setUf_f4(60);
  setUf_U4(197);
  setUf_f5(80);
  setUf_U5(259);
  setUf_f6(100);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 100);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 0);
  writeToDevice(VSD_STOP_FUNCTION, 0);
  writeToDevice(VSD_MIN_REFERENCE, 70);
  writeToDevice(VSD_MAX_REFERENCE, 100);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf6000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);
  writeToDevice(VSD_MOTOR_FREQUENCY, 200);
  writeToDevice(VSD_MOTOR_SPEED, 6000);
  writeToDevice(VSD_MOTOR_POLES, 4);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(40);
  setUf_U2(74);
  setUf_f3(80);
  setUf_U3(136);
  setUf_f4(120);
  setUf_U4(197);
  setUf_f5(160);
  setUf_U5(259);
  setUf_f6(200);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 100);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 0);
  writeToDevice(VSD_STOP_FUNCTION, 0);
  writeToDevice(VSD_MIN_REFERENCE, 70);
  writeToDevice(VSD_MAX_REFERENCE, 100);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 60);
  writeToDevice(VSD_TIMER_DELAY, 60);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 60);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 60);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect500()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);
  writeToDevice(VSD_MOTOR_SPEED, 1000);
  writeToDevice(VSD_MOTOR_POLES, 14);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(23);
  setUf_U2(74);
  setUf_f3(46);
  setUf_U3(136);
  setUf_f4(70);
  setUf_U4(197);
  setUf_f5(93);
  setUf_U5(259);
  setUf_f6(117);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 1);
  writeToDevice(VSD_STOP_FUNCTION, 5);
  writeToDevice(VSD_MIN_REFERENCE, 80);
  writeToDevice(VSD_MAX_REFERENCE, 117);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect1000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);
  writeToDevice(VSD_MOTOR_SPEED, 1000);
  writeToDevice(VSD_MOTOR_POLES, 14);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(23);
  setUf_U2(74);
  setUf_f3(46);
  setUf_U3(136);
  setUf_f4(70);
  setUf_U4(197);
  setUf_f5(93);
  setUf_U5(259);
  setUf_f6(117);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 1);
  writeToDevice(VSD_STOP_FUNCTION, 5);
  writeToDevice(VSD_MIN_REFERENCE, 80);
  writeToDevice(VSD_MAX_REFERENCE, 117);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect3000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  writeToDevice(VSD_MOTOR_FREQUENCY, 100);
  writeToDevice(VSD_MOTOR_SPEED, 3000);
  writeToDevice(VSD_MOTOR_POLES, 4);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(20);
  setUf_U2(74);
  setUf_f3(40);
  setUf_U3(136);
  setUf_f4(60);
  setUf_U4(197);
  setUf_f5(80);
  setUf_U5(259);
  setUf_f6(100);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 1);
  writeToDevice(VSD_STOP_FUNCTION, 5);
  writeToDevice(VSD_MIN_REFERENCE, 70);
  writeToDevice(VSD_MAX_REFERENCE, 100);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 30);
  writeToDevice(VSD_TIMER_DELAY, 30);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect6000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);
  writeToDevice(VSD_MOTOR_FREQUENCY, 200);
  writeToDevice(VSD_MOTOR_SPEED, 6000);
  writeToDevice(VSD_MOTOR_POLES, 4);
  setUf_f1(0);
  setUf_U1(13);
  setUf_f2(40);
  setUf_U2(74);
  setUf_f3(80);
  setUf_U3(136);
  setUf_f4(120);
  setUf_U4(197);
  setUf_f5(160);
  setUf_U5(259);
  setUf_f6(200);
  setUf_U6(320);
  writeToDevice(VSD_RESONANCE_REMOVE, 500);
  writeToDevice(VSD_RESONANCE_TIME, 3);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);
  writeToDevice(VSD_PM_START_MODE, 1);
  writeToDevice(VSD_STOP_FUNCTION, 5);
  writeToDevice(VSD_MIN_REFERENCE, 70);
  writeToDevice(VSD_MAX_REFERENCE, 100);
  writeToDevice(VSD_FREQUENCY, 100);
  writeToDevice(VSD_TIMER_DISPERSAL, 60);
  writeToDevice(VSD_TIMER_DELAY, 60);
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 60);
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 60);
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);
  writeToDevice(VSD_TORQUE_LIMIT, 160);
  writeToDevice(VSD_CURRENT_LIMIT, 160);
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 5);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);
  return ok_r;
}


float VsdDanfoss::checkAlarmVsd()
{
  uint16_t i = 0;
  float vsdAlarm = parameters.get(CCS_VSD_ALARM_CODE);
  uint32_t vsdAlarm78 = getValue(VSD_STATUS_WORD_2);     // BUS_READOUT_ALARM
  uint32_t vsdAlarmWord1 = getValue(VSD_STATUS_WORD_3);  // ALARM_WORD_1
  uint32_t vsdAlarmWord2 = getValue(VSD_STATUS_WORD_4);  // ALARM_WORD_2


  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_28) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_12))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_28; i <= VSD_DANFOSS_ALARM_A_12; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_10) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_47))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_10; i <= VSD_DANFOSS_ALARM_A_47; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_48) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_63))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_48; i <= VSD_DANFOSS_ALARM_A_63; i++) {
      if (checkBit(vsdAlarmWord1, i - 2000)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_SERVICE_TRIP)) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = 0; i < 5; i++) {
      if (checkBit(vsdAlarmWord2, i)) {
        return VSD_DANFOSS_ALARM_SERVICE_TRIP;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_HI_TEMP_DISCHARGE) && (vsdAlarm <= VSD_DANFOSS_ALARM_PROT_DEVICE))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_HI_TEMP_DISCHARGE; i <= VSD_DANFOSS_ALARM_PROT_DEVICE; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_KTY) && (vsdAlarm <= VSD_DANFOSS_ALARM_ECB))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_KTY; i <= VSD_DANFOSS_ALARM_ECB; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_A_59)) {
    if (checkBit(vsdAlarmWord2, VSD_DANFOSS_ALARM_A_59 - 2032)) {
      return VSD_DANFOSS_ALARM_A_59;
    }
  }

  if ((vsdAlarm == VSD_ALARM_NONE) || ((vsdAlarm >= VSD_DANFOSS_ALARM_A_90) && (vsdAlarm <= VSD_DANFOSS_ALARM_A_72))) {
    vsdAlarm = VSD_ALARM_NONE;
    for (i = VSD_DANFOSS_ALARM_A_90; i <= VSD_DANFOSS_ALARM_A_72; i++) {
      if (checkBit(vsdAlarmWord2, i - 2032)) {
        return i;
      }
    }
  }

  if(vsdAlarm78 == 0x4E00)        // А78, т.к. её нет в словах аварии проверяем по
    return VSD_DANFOSS_ALARM_A_78;// коду в регистре последний аварий, код от представителей Danfoss

  return vsdAlarm;
}

bool VsdDanfoss::checkPreventVsd()
{
  return checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_TRIP);
}

int VsdDanfoss::start()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      uint32_t controlVsd = getValue(VSD_CONTROL_WORD_1);
      setBit(controlVsd, VSD_DANFOSS_CONTROL_RAMP, true);
      setBit(controlVsd, VSD_DANFOSS_CONTROL_JOG, true);
      if (setNewValue(VSD_CONTROL_WORD_1, controlVsd))
        return err_r;
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
      return ok_r;
  }
}

int VsdDanfoss::stop(float type)
{
#if USE_DEBUG
  return ok_r;
#endif
  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
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

      uint32_t controlVsd = getValue(VSD_CONTROL_WORD_1);

      if (type != oldTypeStop) {
        switch((uint16_t)type) {
        case TYPE_STOP_ALARM:
          setBit(controlVsd, VSD_DANFOSS_CONTROL_COASTING, false);
          break;
        default:
          setBit(controlVsd, VSD_DANFOSS_CONTROL_RAMP, false);
          break;
        }
      }
      setBit(controlVsd, VSD_DANFOSS_CONTROL_JOG, false);

      if (setNewValue(VSD_CONTROL_WORD_1, controlVsd))
        return err_r;
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
      return ok_r;
  }
}

int VsdDanfoss::startCoil()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_ON, 1))
        if (setNewValue(VSD_FLAG, 1))
          return err_r;

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
      return ok_r;
  }
}

int VsdDanfoss::stopCoil(float type)
{
#if USE_DEBUG
  return ok_r;
#endif
  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
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

      if (type != oldTypeStop) {
        switch((uint16_t)type) {
        default:
          if (setNewValue(VSD_FLAG, 0))
            if (setNewValue(VSD_ON, 0))
              return err_r;
          break;
        }
      }
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_STARTED))
      return ok_r;
  }
}

int VsdDanfoss::resetSetpoints()
{
  writeToDevice(VSD_WORK_STATE_WHEN_ON, 1);
  writeToDevice(VSD_CONFIG_MODE, 0);
  writeToDevice(VSD_TORQUE_CHARACTERISTIC, 0);
  writeToDevice(VSD_OVERLOAD_MODE, 1);
  writeToDevice(VSD_MOTOR_VOLTAGE, 320);
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 100);
  writeToDevice(VSD_START_DELAY, 0);
  writeToDevice(VSD_START_FUNCTION, 2);
  writeToDevice(VSD_STOP_SPEED, 10);
  writeToDevice(VSD_CONTROL_TERMISTOR_MTR, 2);
  writeToDevice(VSD_THERMISTOR_RESOURCE, 5);
  writeToDevice(VSD_PARKING_CURRENT, 50);
  writeToDevice(VSD_PARKING_TIME, 1);
  writeToDevice(VSD_OVERVOLTAGE_CONTROL, 2);
  writeToDevice(VSD_RESOURCE_TASK_1, 0);
  writeToDevice(VSD_RESOURCE_TASK_2, 0);
  writeToDevice(VSD_RESOURCE_TASK_3, 0);
  writeToDevice(VSD_TYPE_SPEED_CHANGE, 0);
  writeToDevice(VSD_ROTATION, 2);
  writeToDevice(VSD_MTR_FEEDBACK_LOSS_FUNC, 0);
  writeToDevice(VSD_TRACK_ERROR_FUNCTION, 2);
  writeToDevice(VSD_TRACK_ERROR, 600);
  writeToDevice(VSD_TRACK_ERROR_TIMEOUT, 10);
  writeToDevice(VSD_TRACK_ERROR_RAMPING, 600);
  writeToDevice(VSD_TRACK_ERROR_RAMP_TIME, 5);
  writeToDevice(VSD_TRACK_ERROR_AFTER_RAMP, 10);
  writeToDevice(VSD_TRACK_ERROR_AFTER_RAMP, 10);
  writeToDevice(VSD_TERMINAL_27_MODE, 1);
  writeToDevice(VSD_TERMINAL_29_MODE, 1);
  writeToDevice(VSD_DI_18, 0);
  writeToDevice(VSD_DI_19, 0);
  writeToDevice(VSD_DI_27, 0);
  writeToDevice(VSD_DI_32, 0);
  writeToDevice(VSD_TERMINAL_27_DI, 75);
  writeToDevice(VSD_TERMINAL_29_DI, 80);
//  FUNCTION_RELE|5;70;0;0;0;0;0;0;0;|
//  ON_DELAY_RELAY|0.01;60;|
  writeToDevice(VSD_42_AO, 133);
  writeToDevice(VSD_CONTROL_WORD_TIMEOUT_TIME, 600);
  writeToDevice(VSD_SL_CONTROLLER_MODE, 1);
  writeToDevice(VSD_SL_START_EVENT, 1);
  writeToDevice(VSD_SL_STOP_EVENT, 0);
  writeToDevice(VSD_SL_RESET, 0);
//  SMART_LOGIC_10|4;4;4;8;0;0;|
//  SMART_LOGIC_11|0;0;2;0;1;1;|
//  SMART_LOGIC_12|32;5;100;440;0;0;|
//  SMART_LOGIC_15|9;|
//  SMART_LOGIC_16|22;|
//  SMART_LOGIC_20|0;0;0;0;0;0;0.001;0.001;|
//  SMART_LOGIC_40|11;25;25;27;29;20;|
//  SMART_LOGIC_41|7;1;1;2;6;2;|
//  SMART_LOGIC_42|20;23;24;28;0;29;|
//  SMART_LOGIC_43|1;|
//  SMART_LOGIC_44|94;|
//  SMART_LOGIC_51|1;29;73;60;74;61;|
//  SMART_LOGIC_52|38;73;32;74;38;1;|
  writeToDevice(VSD_OVERMODULATION, 0);
  writeToDevice(VSD_DEAD_TIME_COMPENSATION, 0);
  writeToDevice(VSD_MAINS_FAILURE, 6);
  writeToDevice(VSD_MAINS_VOLTAGE_FAILURE, 270);
  writeToDevice(VSD_RESET_MODE, 5);
  writeToDevice(VSD_AUTOSTART_TIME, 5);
  writeToDevice(VSD_TRIP_DELAY_AT_INVERTER_FAULT, 1);
  writeToDevice(VSD_DC_COMPENSATION, 1);
  writeToDevice(VSD_FAN_CONTROL, 4);
  writeToDevice(VSD_OUT_FILTER, 2);
//  FAULT_LEVEL|1;3;3;1;2;2;1;2;3;2;3;2;2;3;3;|
  writeToDevice(VSD_LOCK_ROTOR_PROTECTION, 0);
  writeToDevice(VSD_LOCK_ROTOR_TIME, 1);
  return ok_r;
}

void VsdDanfoss::getNewValue(uint16_t id)
{
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
  if (getValue(id) != value) {
    switch (id) {
    case VSD_MOTOR_TYPE:
      setValue(id, value);
      if (parameters.get(CCS_MOTOR_TYPE) != value)
        parameters.set(CCS_MOTOR_TYPE, value);
      break;
    case VSD_UF_CHARACTERISTIC_F:
      switch ((uint16_t)getValue(VSD_INDEX)) {
      case 0:
        setValue(VSD_UF_CHARACTERISTIC_F_1, value);
        break;
      case 1:
        setValue(VSD_UF_CHARACTERISTIC_F_2, value);
        break;
      case 2:
        setValue(VSD_UF_CHARACTERISTIC_F_3, value);
        break;
      case 3:
        setValue(VSD_UF_CHARACTERISTIC_F_4, value);
        break;
      case 4:
        setValue(VSD_UF_CHARACTERISTIC_F_5, value);
        break;
      case 5:
        setValue(VSD_UF_CHARACTERISTIC_F_6, value);
        break;
      default:
        break;
      }
      break;
    case VSD_UF_CHARACTERISTIC_U:
      switch ((uint16_t)getValue(VSD_INDEX)) {
      case 0:
        setValue(VSD_UF_CHARACTERISTIC_U_1, value);
        break;
      case 1:
        setValue(VSD_UF_CHARACTERISTIC_U_2, value);
        break;
      case 2:
        setValue(VSD_UF_CHARACTERISTIC_U_3, value);
        break;
      case 3:
        setValue(VSD_UF_CHARACTERISTIC_U_4, value);
        break;
      case 4:
        setValue(VSD_UF_CHARACTERISTIC_U_5, value);
        break;
      case 5:
        setValue(VSD_UF_CHARACTERISTIC_U_6, value);
        break;
      default:
        break;
      }
      break;
    case VSD_OUT_FILTER:
      setValue(VSD_OUT_FILTER, value);
      if (value) {
         parameters.set(CCS_FILTER_OUTPUT, 1);
      }
      else {
        parameters.set(CCS_FILTER_OUTPUT, value);
      }
    default:
      setValue(id, value);
      break;
    }
  }
}

uint8_t VsdDanfoss::setNewValue(uint16_t id, float value)
{
  int16_t result;
  switch (id) {

  case VSD_MOTOR_TYPE:
    if (!setMotorType(value)) {
      if (getValue(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
        return setVsdControl(VSD_MOTOR_CONTROL_UF);
      }
      return ok_r;
    }
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

  case VSD_SWITCHING_FREQUENCY_CODE:
    return setSwitchingFrequencyCode(value);

  case VSD_ROTATION:
    return setRotation(value);
    break;
  default:
    result = setValue(id, value);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void VsdDanfoss::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

void VsdDanfoss::readInDevice(int id)
{
  dm_->readModbusParameter(id);
}

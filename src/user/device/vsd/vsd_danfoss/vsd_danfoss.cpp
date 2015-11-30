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
                         VSD_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 131);
  dm_->createThread("ProtocolVsd", getValueDeviceQId_);
  dm_->setAddrIndexReg(dm_->getFieldAddress(dm_->getIndexAtId(VSD_INDEX)));

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

int VsdDanfoss::setMotorPower(float value)
{
  if (Vsd::setMotorSpeed(value)) {
    logDebug.add(WarningMsg, "VsdDanfoss::setMotorPower");
    return err_r;
  }
  else {
    writeToDevice(VSD_MOTOR_POWER, parameters.get(VSD_MOTOR_POWER));
    // TODO: Автоадаптация
    return ok_r;
  }
}

int VsdDanfoss::setMotorCurrent(float value)
{
  if (!Vsd::setMotorCurrent(value)) {
    value = value * parameters.get(CCS_COEF_TRANSFORMATION);
    writeToDevice(VSD_MOTOR_CURRENT, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMotorCurrent");
    return err_r;
  }
}

int VsdDanfoss::setMotorVoltage(float value)
{
  if (!Vsd::setMotorVoltage(value)) {
    value = value / parameters.get(CCS_COEF_TRANSFORMATION);
    writeToDevice(VSD_MOTOR_VOLTAGE, value);
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setMotorVoltage");
    return err_r;
  }
}

int VsdDanfoss::onRegimePush()
{
  /*
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == Regime::OffAction)
    return 1;
  float freqPush = parameters.get(CCS_RGM_RUN_PUSH_FREQ);

  // Задание Набора 2
  writeToDevice(VSD_CHANGE_SETUP, 2);
  writeToDevice(VSD_FREQUENCY, freqPush);

  // Задание Набора 3
  writeToDevice(VSD_CHANGE_SETUP, 3);
  writeToDevice(VSD_FREQUENCY, freqPush);

  if (freqPush > parameters.get(VSD_UF_CHARACTERISTIC_F_1)) {
    if (freqPush > parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
      if (freqPush > parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
        if (freqPush > parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
          if (freqPush > parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
            writeToDevice(VSD_UF_CHARACTERISTIC_U_5, parameters.get(VSD_UF_CHARACTERISTIC_U_5) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
            writeToDevice(VSD_UF_CHARACTERISTIC_U_6, parameters.get(VSD_UF_CHARACTERISTIC_U_6) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
          }
          else {
            writeToDevice(VSD_UF_CHARACTERISTIC_U_4, parameters.get(VSD_UF_CHARACTERISTIC_U_4) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
            writeToDevice(VSD_UF_CHARACTERISTIC_U_5, parameters.get(VSD_UF_CHARACTERISTIC_U_5) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
          }
        }
        else {
          writeToDevice(VSD_UF_CHARACTERISTIC_U_3, parameters.get(VSD_UF_CHARACTERISTIC_U_3) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
          writeToDevice(VSD_UF_CHARACTERISTIC_U_4, parameters.get(VSD_UF_CHARACTERISTIC_U_4) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
        }
      }
      else {
        writeToDevice(VSD_UF_CHARACTERISTIC_U_2, parameters.get(VSD_UF_CHARACTERISTIC_U_2) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
        writeToDevice(VSD_UF_CHARACTERISTIC_U_3, parameters.get(VSD_UF_CHARACTERISTIC_U_3) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
      }
    }
    else {
      writeToDevice(VSD_UF_CHARACTERISTIC_U_1, parameters.get(VSD_UF_CHARACTERISTIC_U_1) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
      writeToDevice(VSD_UF_CHARACTERISTIC_U_2, parameters.get(VSD_UF_CHARACTERISTIC_U_2) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
    }
  }
  else {
    writeToDevice(VSD_UF_CHARACTERISTIC_U_1, parameters.get(VSD_UF_CHARACTERISTIC_U_1) * (parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE) / 100.0));
  }
  */
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
    logDebug.add(WarningMsg, "VsdDanfoss::setFrequency");
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

int VsdDanfoss::calcFiltTimeCurLim(float value)
{
  // Текущая частота равна частоте уставки
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_SPEED)) {
    if (getValue(VSD_FIL_TIME_CURRENT_LIMIT) != 0.005)
      writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);
  }
  else {
    if (getValue(VSD_FIL_TIME_CURRENT_LIMIT) != 0.1)
      writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.1);
  }
  return ok_r;
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_1, getValue(VSD_UF_CHARACTERISTIC_F_1));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_2, getValue(VSD_UF_CHARACTERISTIC_F_2));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_3, getValue(VSD_UF_CHARACTERISTIC_F_3));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_4, getValue(VSD_UF_CHARACTERISTIC_F_4));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_5, getValue(VSD_UF_CHARACTERISTIC_F_5));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_F_6, getValue(VSD_UF_CHARACTERISTIC_F_6));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_1, getValue(VSD_UF_CHARACTERISTIC_U_1));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_2, getValue(VSD_UF_CHARACTERISTIC_U_2));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_3, getValue(VSD_UF_CHARACTERISTIC_U_3));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_4, getValue(VSD_UF_CHARACTERISTIC_U_4));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_5, getValue(VSD_UF_CHARACTERISTIC_U_5));
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
    writeToDevice(VSD_UF_CHARACTERISTIC_U_6, getValue(VSD_UF_CHARACTERISTIC_U_6));
    return ok_r;
  }
  else {
    logDebug.add(WarningMsg, "VsdDanfoss::setUf_U6");
    return err_r;
  }
}

int VsdDanfoss::calcUfCharacteristicU()
{
  float point1 = getValue(VSD_UF_CHARACTERISTIC_U_1);
  float point6 = parameters.get(CCS_BASE_VOLTAGE);
  setUf_U1((point6 * 0 + point1 * 5) / 5);
  setUf_U2((point6 * 1 + point1 * 4) / 5);
  setUf_U3((point6 * 2 + point1 * 3) / 5);
  setUf_U4((point6 * 3 + point1 * 2) / 5);
  setUf_U5((point6 * 4 + point1 * 1) / 5);
  setUf_U6((point6 * 5 + point1 * 0) / 5);
  return ok_r;
}

int VsdDanfoss::calcUfCharacteristicF()
{
  float point1 = getValue(VSD_UF_CHARACTERISTIC_F_1);
  float point6 = parameters.get(CCS_BASE_FREQUENCY);
  setUf_f1((point6 * 0 + point1 * 5) / 5);
  setUf_f2((point6 * 1 + point1 * 4) / 5);
  setUf_f3((point6 * 2 + point1 * 3) / 5);
  setUf_f4((point6 * 3 + point1 * 2) / 5);
  setUf_f5((point6 * 4 + point1 * 1) / 5);
  setUf_f6((point6 * 5 + point1 * 0) / 5);
  return ok_r;
}

void VsdDanfoss::readUfCharacterictic()
{
  readInDevice(VSD_UF_CHARACTERISTIC_F_1);
  readInDevice(VSD_UF_CHARACTERISTIC_F_2);
  readInDevice(VSD_UF_CHARACTERISTIC_F_3);
  readInDevice(VSD_UF_CHARACTERISTIC_F_4);
  readInDevice(VSD_UF_CHARACTERISTIC_F_5);
  readInDevice(VSD_UF_CHARACTERISTIC_F_6);
  readInDevice(VSD_UF_CHARACTERISTIC_U_1);
  readInDevice(VSD_UF_CHARACTERISTIC_U_2);
  readInDevice(VSD_UF_CHARACTERISTIC_U_3);
  readInDevice(VSD_UF_CHARACTERISTIC_U_4);
  readInDevice(VSD_UF_CHARACTERISTIC_U_5);
  readInDevice(VSD_UF_CHARACTERISTIC_U_6);
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
  // TODO: Может тут лучше setNewValue или parameters.set ?
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 50);                       // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 3000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 2);                            // 1-39

  parameters.set(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  parameters.set(VSD_UF_CHARACTERISTIC_U_1, 5);                  // 1-55.0
  parameters.set(VSD_UF_CHARACTERISTIC_F_2, 10);                 // 1-56.1
  parameters.set(VSD_UF_CHARACTERISTIC_U_2, 68);                 // 1-55.1
  parameters.set(VSD_UF_CHARACTERISTIC_F_3, 20);                 // 1-56.2
  parameters.set(VSD_UF_CHARACTERISTIC_U_3, 131);                // 1-55.2
  parameters.set(VSD_UF_CHARACTERISTIC_F_4, 30);                 // 1-56.3
  parameters.set(VSD_UF_CHARACTERISTIC_U_4, 194);                // 1-55.3
  parameters.set(VSD_UF_CHARACTERISTIC_F_5, 40);                 // 1-56.4
  parameters.set(VSD_UF_CHARACTERISTIC_U_5, 257);                // 1-55.4
  parameters.set(VSD_UF_CHARACTERISTIC_F_6, 50);                 // 1-56.5
  parameters.set(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  parameters.set(VSD_RESONANCE_REMOVE, 0);                       // 1-64
  parameters.set(VSD_RESONANCE_TIME, 2);                         // 1-65
  parameters.set(VSD_MIN_CURRENT_LOW_SPEED, 100);                // 1-66
  parameters.set(VSD_PM_START_MODE, 0);                          // 1-70
  parameters.set(VSD_STOP_FUNCTION, 0);                          // 1-80
  parameters.set(VSD_MIN_REFERENCE, 30);                         // 3-02
  parameters.set(VSD_MAX_REFERENCE, 70);                         // 3-03
  parameters.set(VSD_FREQUENCY, 50);                             // 3-11
  parameters.set(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  parameters.set(VSD_TIMER_DELAY, 30);                           // 3-42
  parameters.set(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  parameters.set(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  parameters.set(VSD_MAX_OUTPUT_FREQUENCY, 71);                  // 4-19
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, 30);                   // 4-12
  parameters.set(VSD_HIGH_LIM_SPEED_MOTOR, 50);                  // 4-14
  parameters.set(VSD_TORQUE_LIMIT, 160);                         // 4-16
  parameters.set(VSD_CURRENT_LIMIT, 230);                        // 4-18
  parameters.set(VSD_WARNING_SPEED_LOW, 650);                    // 4-52
  parameters.set(VSD_DELAY_CURRENT_LIMIT, 5);                    // 14-24
  parameters.set(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  parameters.set(VSD_FIL_TIME_CURRENT_LIMIT, 0.1);               // 14-32
  parameters.set(VSD_DC_COMPENSATION, 1);                        // 14-51
  parameters.set(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  parameters.set(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21

//  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
//  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
//  writeToDevice(VSD_MOTOR_FREQUENCY, 50);                       // 1-23
//  writeToDevice(VSD_MOTOR_SPEED, 3000);                         // 1-25
//  writeToDevice(VSD_MOTOR_POLES, 2);                            // 1-39
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 5);                  // 1-55.0
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 10);                 // 1-56.1
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 68);                 // 1-55.1
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 20);                 // 1-56.2
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 131);                // 1-55.2
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 30);                 // 1-56.3
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 194);                // 1-55.3
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 40);                 // 1-56.4
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 257);                // 1-55.4
//  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 50);                 // 1-56.5
//  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
//  writeToDevice(VSD_RESONANCE_REMOVE, 0);                       // 1-64
//  writeToDevice(VSD_RESONANCE_TIME, 2);                         // 1-65
//  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 100);                // 1-66
//  writeToDevice(VSD_PM_START_MODE, 0);                          // 1-70
//  writeToDevice(VSD_STOP_FUNCTION, 0);                          // 1-80
//  writeToDevice(VSD_MIN_REFERENCE, 30);                         // 3-02
//  writeToDevice(VSD_MAX_REFERENCE, 70);                         // 3-03
//  writeToDevice(VSD_FREQUENCY, 50);                             // 3-11
//  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
//  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
//  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
//  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
//  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 71);                  // 4-19
//  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 30);                   // 4-12
//  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 50);                  // 4-14
//  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
//  writeToDevice(VSD_CURRENT_LIMIT, 230);                        // 4-18
//  writeToDevice(VSD_WARNING_SPEED_LOW, 650);                    // 4-52
//  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 5);                    // 14-24
//  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
//  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.1);               // 14-32
//  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
//  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
//  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf500()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 1000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 14);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 23);                 // 1-56.1
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 46);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 70);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 93);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 117);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 0);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 0);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 80);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 117);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf1000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 1000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 14);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 23);                 // 1-56.1
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 46);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 70);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 93);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 117);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 0);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 0);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 80);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 117);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf3000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 100);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 3000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 4);                            // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 20);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 40);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 60);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 80);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 100);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 100);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 0);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 0);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 70);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 100);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentUf6000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_UF);       // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_ASYNC);          // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 200);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 6000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 4);                            // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 40);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 80);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 120);                // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 160);                // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 200);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 100);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 0);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 0);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 70);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 100);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 60);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 60);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 60);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 60);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect500()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);     // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);           // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 1000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 14);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 23);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 46);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 70);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 93);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 117);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 1);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 5);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 80);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 117);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect1000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);     // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);           // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 117);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 1000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 14);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 23);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 46);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 70);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 93);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 117);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 1);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 5);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 80);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 117);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 117);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 110);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect3000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);     // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);           // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 100);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 1000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 4);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 20);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 40);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 60);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 80);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 100);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 1);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 5);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 70);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 100);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 30);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 30);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 30);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 30);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 80);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
  return ok_r;
}

uint16_t VsdDanfoss::configVsdVentVect6000()
{
  writeToDevice(VSD_MOTOR_CONTROL, VSD_MOTOR_CONTROL_VECT);     // 1-01
  writeToDevice(VSD_MOTOR_TYPE, VSD_MOTOR_TYPE_VENT);           // 1-10
  writeToDevice(VSD_MOTOR_FREQUENCY, 200);                      // 1-23
  writeToDevice(VSD_MOTOR_SPEED, 6000);                         // 1-25
  writeToDevice(VSD_MOTOR_POLES, 4);                           // 1-39
  writeToDevice(VSD_UF_CHARACTERISTIC_F_1, 0);                  // 1-56.0
  writeToDevice(VSD_UF_CHARACTERISTIC_U_1, 13);                 // 1-55.0
  writeToDevice(VSD_UF_CHARACTERISTIC_F_2, 40);                 // 1-56.10
  writeToDevice(VSD_UF_CHARACTERISTIC_U_2, 74);                 // 1-55.1
  writeToDevice(VSD_UF_CHARACTERISTIC_F_3, 80);                 // 1-56.2
  writeToDevice(VSD_UF_CHARACTERISTIC_U_3, 136);                // 1-55.2
  writeToDevice(VSD_UF_CHARACTERISTIC_F_4, 120);                 // 1-56.3
  writeToDevice(VSD_UF_CHARACTERISTIC_U_4, 197);                // 1-55.3
  writeToDevice(VSD_UF_CHARACTERISTIC_F_5, 160);                 // 1-56.4
  writeToDevice(VSD_UF_CHARACTERISTIC_U_5, 259);                // 1-55.4
  writeToDevice(VSD_UF_CHARACTERISTIC_F_6, 200);                // 1-56.5
  writeToDevice(VSD_UF_CHARACTERISTIC_U_6, 320);                // 1-55.5
  writeToDevice(VSD_RESONANCE_REMOVE, 500);                     // 1-64
  writeToDevice(VSD_RESONANCE_TIME, 3);                         // 1-65
  writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 50);                 // 1-66
  writeToDevice(VSD_PM_START_MODE, 1);                          // 1-70
  writeToDevice(VSD_STOP_FUNCTION, 5);                          // 1-80
  writeToDevice(VSD_MIN_REFERENCE, 70);                         // 3-02
  writeToDevice(VSD_MAX_REFERENCE, 100);                        // 3-03
  writeToDevice(VSD_FREQUENCY, 100);                            // 3-11
  writeToDevice(VSD_TIMER_DISPERSAL, 60);                       // 3-41
  writeToDevice(VSD_TIMER_DELAY, 60);                           // 3-42
  writeToDevice(VSD_TIMER_DISP_FIX_SPEED, 60);                  // 3-80
  writeToDevice(VSD_TIMER_DELAY_FIX_SPEED, 60);                 // 3-81
  writeToDevice(VSD_MAX_OUTPUT_FREQUENCY, 205);                 // 4-19
  writeToDevice(VSD_LOW_LIM_SPEED_MOTOR, 70);                   // 4-12
  writeToDevice(VSD_HIGH_LIM_SPEED_MOTOR, 100);                 // 4-14
  writeToDevice(VSD_TORQUE_LIMIT, 160);                         // 4-16
  writeToDevice(VSD_CURRENT_LIMIT, 160);                        // 4-18
  writeToDevice(VSD_WARNING_SPEED_LOW, 325);                    // 4-52
  writeToDevice(VSD_DELAY_CURRENT_LIMIT, 0);                    // 14-24
  writeToDevice(VSD_DELAY_TORQUE_LIMIT, 0);                     // 14-25
  writeToDevice(VSD_FIL_TIME_CURRENT_LIMIT, 0.005);             // 14-32
  writeToDevice(VSD_DC_COMPENSATION, 1);                        // 14-51
  writeToDevice(VSD_HIGH_START_TORQUE_TIME, 10);                // 30-20
  writeToDevice(VSD_HIGH_START_TORQUE_CURRENT, 50);             // 30-21
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

  if ((vsdAlarm == VSD_ALARM_NONE) || (vsdAlarm == VSD_DANFOSS_ALARM_A_78)) {
    if (vsdAlarm78 == 0x4E00)                                   // А78, т.к. её нет в словах аварии проверяем по
      return VSD_DANFOSS_ALARM_A_78;                            // коду в регистре последний аварий, код от представителей Danfoss
    else
      return VSD_ALARM_NONE;
  }
  return vsdAlarm;
}

bool VsdDanfoss::checkPreventVsd()
{
  return checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_TRIP);
}

int VsdDanfoss::start()
{
  // Если стоит бит запуска двигателя
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (!setNewValue(VSD_ON, 1))           // VSD_DANFOSS_CONTROL_RAMP 6
        if (!setNewValue(VSD_FLAG, 1))       // VSD_DANFOSS_CONTROL_JOG 8
          return ok_r;

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
      return ok_r;
  }
}

int VsdDanfoss::stop(float type)
{
  // Если нет бита "В работе"
  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return ok_r;

  int timeMs = VSD_CMD_TIMEOUT;
  int countRepeats = 0;

  while (1) {
    if (timeMs >= VSD_CMD_TIMEOUT) {
      timeMs = 0;
      countRepeats++;

      if (countRepeats > VSD_CMD_NUMBER_REPEATS)
        return err_r;

      if (setNewValue(VSD_FLAG, 0))       // VSD_DANFOSS_CONTROL_JOG 8
        return err_r;
      else {
        if(setNewValue(VSD_ON, 0))        // VSD_DANFOSS_CONTROL_RAMP 6
          return err_r;
        else
          return ok_r;
      }

//      if (!setNewValue(VSD_FLAG, 0))       // VSD_DANFOSS_CONTROL_JOG 8
//        if(!setNewValue(VSD_ON, 0))        // VSD_DANFOSS_CONTROL_RAMP 6
//          return ok_r;
    }
    else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION)) {
      return ok_r;
    }
  }
}

bool VsdDanfoss::checkStart()
{
  if (checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return true;
  return false;

}

bool VsdDanfoss::checkStop()
{
  if (!checkBit(getValue(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION))
    return true;
  return false;
}

/*
void VsdDanfoss::processingRegimeRun()
{
  regimeRun_->processing();
}
*/

int VsdDanfoss::resetSetpoints()
{
//  for (uint8_t i = 1; i++; i < 2) {
    writeToDevice(VSD_ACTIVE_SETUP, 1);               // 0-10
    writeToDevice(VSD_CHANGE_SETUP, 1);               // 0-11
    writeToDevice(VSD_WORK_STATE_WHEN_ON, 1);         // 0-02
    writeToDevice(VSD_CONFIG_MODE, 0);                // 1-00
    writeToDevice(VSD_TORQUE_CHARACTERISTIC, 0);      // 1-03
    writeToDevice(VSD_OVERLOAD_MODE, 1);              // 1-04
    writeToDevice(VSD_MOTOR_VOLTAGE, 320);            // 1-22
    writeToDevice(VSD_MIN_CURRENT_LOW_SPEED, 100);    // 1-66
    writeToDevice(VSD_START_DELAY, 0);                // 1-71
    writeToDevice(VSD_START_FUNCTION, 2);             // 1-72
    writeToDevice(VSD_STOP_SPEED, 10);                // 1-82
    writeToDevice(VSD_CONTROL_TERMISTOR_MTR, 2);      // 1-90
    writeToDevice(VSD_THERMISTOR_RESOURCE, 5);        // 1-93
    writeToDevice(VSD_PARKING_CURRENT, 50);           // 2-06
    writeToDevice(VSD_PARKING_TIME, 1);               // 2-07
    writeToDevice(VSD_OVERVOLTAGE_CONTROL, 2);        // 2-17
    writeToDevice(VSD_RESOURCE_TASK_1, 0);            // 3-15
    writeToDevice(VSD_RESOURCE_TASK_2, 0);            // 3-16
    writeToDevice(VSD_RESOURCE_TASK_3, 0);            // 3-17
    writeToDevice(VSD_TYPE_SPEED_CHANGE, 0);          // 3-40
    writeToDevice(VSD_ROTATION_FLAG, 2);              // 4-10
    writeToDevice(VSD_MTR_FEEDBACK_LOSS_FUNC, 0);     // 4-30
    writeToDevice(VSD_TRACK_ERROR_FUNCTION, 2);       // 4-34
    writeToDevice(VSD_TRACK_ERROR, 600);              // 4-35
    writeToDevice(VSD_TRACK_ERROR_TIMEOUT, 10);       // 4-36
    writeToDevice(VSD_TRACK_ERROR_RAMPING, 600);      // 4-37
    writeToDevice(VSD_TRACK_ERROR_RAMP_TIME, 5);      // 4-38
    writeToDevice(VSD_TRACK_ERROR_AFTER_RAMP, 10);    // 4-39
    writeToDevice(VSD_TERMINAL_27_MODE, 1);           // 5-01
    writeToDevice(VSD_TERMINAL_29_MODE, 1);           // 5-02
    writeToDevice(VSD_DI_18, 0);                      // 5-10
    writeToDevice(VSD_DI_19, 0);                      // 5-11
    writeToDevice(VSD_DI_27, 0);                      // 5-12
    writeToDevice(VSD_DI_32, 0);                      // 5-14
    writeToDevice(VSD_TERMINAL_27_DI, 75);            // 5-30
    writeToDevice(VSD_TERMINAL_29_DI, 80);            // 5-31
    writeToDevice(VSD_FUNCTION_RELE, 5);              // 5-40.0
    writeToDevice(VSD_FUNCTION_RELE_1, 70);           // 5-40.1
    writeToDevice(VSD_42_AO, 133);                    // 5-41
    writeToDevice(VSD_CONTROL_WORD_TIMEOUT_TIME, 600);// 8-03
    writeToDevice(VSD_SL_CONTROLLER_MODE, 1);         // 13-00
    writeToDevice(VSD_SL_START_EVENT, 1);             // 13-01
    writeToDevice(VSD_SL_STOP_EVENT, 0);              // 13-02
    writeToDevice(VSD_SL_RESET, 0);                   // 13-03
    writeToDevice(VSD_SL_10, 4);                      // 13-10.0
    writeToDevice(VSD_SL_10_1, 4);                    // 13-10.1
    writeToDevice(VSD_SL_10_2, 4);                    // 13-10.2
    writeToDevice(VSD_SL_10_3, 8);                    // 13-10.3
    writeToDevice(VSD_SL_10_4, 0);                    // 13-10.4
    writeToDevice(VSD_SL_10_5, 0);                    // 13-10.5
    writeToDevice(VSD_SL_11, 0);                      // 13-11.0
    writeToDevice(VSD_SL_11_1, 0);                    // 13-11.1
    writeToDevice(VSD_SL_11_2, 2);                    // 13-11.2
    writeToDevice(VSD_SL_11_3, 0);                    // 13-11.3
    writeToDevice(VSD_SL_11_4, 1);                    // 13-11.4
    writeToDevice(VSD_SL_11_5, 1);                    // 13-11.5
    writeToDevice(VSD_SL_12, 32);                     // 13-12.0
    writeToDevice(VSD_SL_12_1, 5);                    // 13-12.1
    writeToDevice(VSD_SL_12_2, 100);                  // 13-12.2
    writeToDevice(VSD_SL_12_3, 440);                  // 13-12.3
    writeToDevice(VSD_SL_12_4, 0);                    // 13-12.4
    writeToDevice(VSD_SL_12_5, 0);                    // 13-12.5
    writeToDevice(VSD_SL_15, 9);                      // 13-15
    writeToDevice(VSD_SL_16, 22);                     // 13-16
    writeToDevice(VSD_SL_20, 0);                      // 13-20.0
    writeToDevice(VSD_SL_20_1, 0);                    // 13-20.1
    writeToDevice(VSD_SL_20_2, 0);                    // 13-20.2
    writeToDevice(VSD_SL_20_3, 0);                    // 13-20.3
    writeToDevice(VSD_SL_20_4, 0);                    // 13-20.4
    writeToDevice(VSD_SL_20_5, 0);                    // 13-20.5
    writeToDevice(VSD_SL_20_6, 0.001);                // 13-20.6
    writeToDevice(VSD_SL_20_7, 0.001);                // 13-20.7
    writeToDevice(VSD_SL_40, 11);                     // 13-40.0
    writeToDevice(VSD_SL_40_1, 25);                   // 13-40.1
    writeToDevice(VSD_SL_40_2, 25);                   // 13-40.2
    writeToDevice(VSD_SL_40_3, 27);                   // 13-40.3
    writeToDevice(VSD_SL_40_4, 29);                   // 13-40.4
    writeToDevice(VSD_SL_40_5, 20);                   // 13-40.5
    writeToDevice(VSD_SL_41, 7);                      // 13-41.0
    writeToDevice(VSD_SL_41_1, 1);                    // 13-41.1
    writeToDevice(VSD_SL_41_2, 1);                    // 13-41.2
    writeToDevice(VSD_SL_41_3, 2);                    // 13-41.3
    writeToDevice(VSD_SL_41_4, 6);                    // 13-41.4
    writeToDevice(VSD_SL_41_5, 2);                    // 13-41.5
    writeToDevice(VSD_SL_42, 20);                     // 13-42.0
    writeToDevice(VSD_SL_42_1, 23);                   // 13-42.1
    writeToDevice(VSD_SL_42_2, 24);                   // 13-42.2
    writeToDevice(VSD_SL_42_3, 28);                   // 13-42.3
    writeToDevice(VSD_SL_42_4, 0);                    // 13-42.4
    writeToDevice(VSD_SL_42_5, 29);                   // 13-42.5
    writeToDevice(VSD_SL_43, 1);                      // 13-43
    writeToDevice(VSD_SL_44, 94);                     // 13-44
    writeToDevice(VSD_SL_51, 1);                      // 13-51.0
    writeToDevice(VSD_SL_51_1, 29);                   // 13-51.1
    writeToDevice(VSD_SL_51_2, 73);                   // 13-51.2
    writeToDevice(VSD_SL_51_3, 60);                   // 13-51.3
    writeToDevice(VSD_SL_51_4, 74);                   // 13-51.4
    writeToDevice(VSD_SL_51_5, 61);                   // 13-51.5
    writeToDevice(VSD_SL_52, 38);                     // 13-52.0
    writeToDevice(VSD_SL_52_1, 73);                   // 13-52.1
    writeToDevice(VSD_SL_52_2, 32);                   // 13-52.2
    writeToDevice(VSD_SL_52_3, 74);                   // 13-52.3
    writeToDevice(VSD_SL_52_4, 38);                   // 13-52.4
    writeToDevice(VSD_SL_52_5, 1);                    // 13-52.5

    writeToDevice(VSD_OVERMODULATION, 0);             // 14-03
    writeToDevice(VSD_DEAD_TIME_COMPENSATION, 0);     // 14-06
    writeToDevice(VSD_MAINS_FAILURE, 6);              // 14-10
    writeToDevice(VSD_MAINS_VOLTAGE_FAILURE, 270);    // 14-11
    writeToDevice(VSD_RESET_MODE, 5);                 // 14-20
    writeToDevice(VSD_AUTOSTART_TIME, 5);             // 14-21
    writeToDevice(VSD_TRIP_DELAY_AT_INVERTER_FAULT, 1);// 14-26
    writeToDevice(VSD_DC_COMPENSATION, 1);            // 14-51
    writeToDevice(VSD_FAN_CONTROL, 4);                // 14-52
    writeToDevice(VSD_OUT_FILTER, 2);                 // 14-55
    writeToDevice(VSD_FAIL_RESET, 1);                 // 14-90.0
    writeToDevice(VSD_FAIL_RESET_1, 3);               // 14-90.1
    writeToDevice(VSD_FAIL_RESET_2, 3);               // 14-90.2
    writeToDevice(VSD_FAIL_RESET_3, 1);               // 14-90.3
    writeToDevice(VSD_FAIL_RESET_4, 2);               // 14-90.4
    writeToDevice(VSD_FAIL_RESET_5, 2);               // 14-90.5
    writeToDevice(VSD_FAIL_RESET_6, 1);               // 14-90.6
    writeToDevice(VSD_FAIL_RESET_7, 2);               // 14-90.7
    writeToDevice(VSD_FAIL_RESET_8, 3);               // 14-90.8
    writeToDevice(VSD_FAIL_RESET_9, 2);               // 14-90.9
    writeToDevice(VSD_FAIL_RESET_10, 3);              // 14-90.10
    writeToDevice(VSD_FAIL_RESET_11, 2);              // 14-90.11
    writeToDevice(VSD_FAIL_RESET_12, 2);              // 14-90.12
    writeToDevice(VSD_FAIL_RESET_13, 3);              // 14-90.13
    writeToDevice(VSD_FAIL_RESET_14, 3);              // 14-90.14
    writeToDevice(VSD_LOCK_ROTOR_PROTECTION, 0);      // 30-22
    writeToDevice(VSD_LOCK_ROTOR_TIME, 1);            // 30-23
//  }
//  writeToDevice(VSD_ACTIVE_SETUP, 1);
//  writeToDevice(VSD_CHANGE_SETUP, 1);
  return ok_r;
}

void VsdDanfoss::getNewValue(uint16_t id)
{
  float value = 0;
  float temp = 0;
  // Преобразуем данные из полученного типа данных в float
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity != ok_r) {
    value = NAN;
    setValue(id, value);
    return;
  }

  switch (param->typeData) {
  case TYPE_DATA_INT16:
  case TYPE_DATA_ARRAY_INT16:
    value = (float)param->value.int16_t[0];
    break;
  case TYPE_DATA_UINT16:
    value = (float)param->value.uint16_t[0];
    break;
  case TYPE_DATA_INT32:
  case TYPE_DATA_ARRAY_INT32:
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
      if ((parameters.get(CCS_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) && (value > VSD_MOTOR_TYPE_ASYNC))
        parameters.set(CCS_MOTOR_TYPE, value);
      break;
    case VSD_OUT_FILTER:
      setValue(VSD_OUT_FILTER, value);
      if (value) {
         parameters.set(CCS_FILTER_OUTPUT, 1);
      }
      else {
        parameters.set(CCS_FILTER_OUTPUT, value);
      }
    case VSD_CURRENT_OUT_PHASE_1:             // Выходной ток ЧРП Фаза 1
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_1) * value);
      calcVsdPowerFull();
      calcMotorCos();
      ksu.calcMotorCurrentPhase1();           // Вычисляем ток двигателя фаза 1
      ksu.calcMotorCurrentAverage();          // Вычисляем средний ток двигателя
      ksu.calcMotorCurrentImbalance();        // Вычисляем дисбаланс тока двигателя
      break;
    case VSD_CURRENT_OUT_PHASE_2:             // Выходной ток ЧРП Фаза 2
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_2) * value);
      calcVsdPowerFull();
      calcMotorCos();
      ksu.calcMotorCurrentPhase2();
      ksu.calcMotorCurrentAverage();
      ksu.calcMotorCurrentImbalance();
      break;
    case VSD_CURRENT_OUT_PHASE_3:             // Выходной ток ЧРП Фаза 3
      setValue(id, parameters.get(CCS_COEF_OUT_CURRENT_3) * value);
      calcVsdPowerFull();
      calcMotorCos();
      ksu.calcMotorCurrentPhase3();
      ksu.calcMotorCurrentAverage();
      ksu.calcMotorCurrentImbalance();
      break;
    case VSD_OUT_VOLTAGE_MOTOR:
      setValue(id, value);
      calcVsdPowerFull();
      calcMotorCos();
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
    case VSD_MOTOR_CURRENT:
      temp = parameters.get(CCS_COEF_TRANSFORMATION);
      if (temp != 0)
        value = value / temp;
      setValue(id, value);
      break;
    case VSD_MOTOR_VOLTAGE:
      temp = parameters.get(CCS_COEF_TRANSFORMATION);
      if (temp != 0)
        value = value * temp;
      setValue(id, value);
      break;
    case VSD_STATUS_WORD_1:
      setValue(id, value);
      calcFiltTimeCurLim(value);
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

  case VSD_MOTOR_CURRENT:
    return setMotorCurrent(value);

  case VSD_MOTOR_VOLTAGE:
    if (!setMotorVoltage(value)) {
      ksu.calcTransRecommendedTapOff();
      // TODO: Сообщение о правильности отпайки
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

  case VSD_SWITCHING_FREQUENCY_CODE:
    return setSwitchingFrequencyCode(value);

  case VSD_ROTATION:
    return setRotation(value);

  case VSD_MOTOR_SPEED:
    return setMotorSpeed(value);

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

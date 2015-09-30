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
  // Цикл по карте регистров Modbus
  for (int indexModbus = 0; indexModbus < count; indexModbus++) {
    int id = dm_->getFieldID(indexModbus);
    if (id <= 0)
      continue;
    // Получаем индекс параметра в банке параметров
    int indexArray = getIndexAtId(id);
    // Если нашли параметр
    if (indexArray) {
      // Уровень доступа оператор
      setFieldAccess(indexArray, ACCESS_OPERATOR);
      // Операции над параметром
      setFieldOperation(indexArray, dm_->getFieldOperation(indexModbus));
      // Физический смысл
      setFieldPhysic(indexArray, dm_->getFieldPhysic(indexModbus));
      // Получаем минимум
      float tempVal = dm_->getFieldMinimum(indexModbus);
      // Применяем коэффициент
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMin(id, tempVal);
      // Получаем мaксимум
      tempVal = dm_->getFieldMaximum(indexModbus);
      // Применяем коэффициент
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMax(id, tempVal);
      // Получаем значение по умолчанию
      tempVal = dm_->getFieldDefault(indexModbus);
      // Применяем коэффициент
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldDef(indexArray, tempVal);
      // Получили флаг валидности
      setFieldValidity(indexArray, dm_->getFieldValidity(indexModbus));
      // Присвоили значение значению по умолчанию
      setFieldValue(indexArray, getFieldDefault(indexArray));
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

  setLimitsFrequence(0, getValue(VSD_LOW_LIM_SPEED_MOTOR));
  setLimitsFrequence(1, getValue(VSD_HIGH_LIM_SPEED_MOTOR));
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

int VsdDanfoss::start()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
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

      uint32_t controlVsd = getValue(VSD_CONTROL_WORD_1);
      setBit(controlVsd, VSD_DANFOSS_CONTROL_RAMP, true);
      setBit(controlVsd, VSD_DANFOSS_CONTROL_JOG, true);
      if (setNewValue(VSD_CONTROL_WORD_1, controlVsd))
        return err_r;
    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkStatusVsd(VSD_STATUS_STARTED)) {
      return ok_r;
    }
  }
}

int VsdDanfoss::stop(float type)
{
#if USE_DEBUG
  return ok_r;
#endif
  if (!checkStatusVsd(VSD_STATUS_STARTED))
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

    if (!checkStatusVsd(VSD_STATUS_STARTED))
      return ok_r;
  }
}

int VsdDanfoss::startCoil()
{
#if USE_DEBUG
  return ok_r;
#endif

  // Если стоит бит запуска двигателя
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

      if (setNewValue(VSD_ON, 1))
        if (setNewValue(VSD_FLAG, 1))
          return err_r;

    } else {
      timeMs = timeMs + 100;
    }

    osDelay(100);

    if (checkStatusVsd(VSD_STATUS_STARTED)) {
      return ok_r;
    }
  }
}

int VsdDanfoss::stopCoil(float type)
{
#if USE_DEBUG
  return ok_r;
#endif
  if (!checkStatusVsd(VSD_STATUS_STARTED))
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

    if (!checkStatusVsd(VSD_STATUS_STARTED))
      return ok_r;
  }
}

void VsdDanfoss::getNewValue(uint16_t id)
{
  float value = 0;
  uint32_t vsdStatus = 0x0000;
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
    case VSD_STATUS_WORD_1:
      vsdStatus = parameters.get(CCS_VSD_STATUS_WORD_1);
      setBit(vsdStatus, VSD_STATUS_STARTED, checkBit(value, VSD_DANFOSS_STATUS_STATE));
      parameters.set(CCS_VSD_STATUS_WORD_1, (float)vsdStatus);
      setValue(id, value);
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


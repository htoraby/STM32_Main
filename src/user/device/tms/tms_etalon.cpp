﻿#include "tms_etalon.h"

void TmsEtalon::initModbusParameters()
{
  modbusParameters_[0] = { // Пустой регистр
                          TMS_BEGIN,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          NOT_READ,
                          NOT_READ,
                          0,
                          0,
                          0
                         };
  modbusParameters_[1] = { // Rиз. - сопротивление изоляции, КОм
                           TMS_RESISTANCE_ISOLATION,
                           0,               // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_RESISTANCE,
                           RESISTANCE_KOM,  // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           9999.0,          // Максимально значение параметра
                           9999.0,          // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[2] = { // Тпласта - температура пласта
                           TMS_TEMPERATURE_INTAKE,
                           1,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           200.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[3] = { // Тдвигателя - температура обмодки ПЭД
                           TMS_TEMPERATURE_WINDING,
                           2,               // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           200.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[4] = { // Вибрация Vху
                           TMS_ACCELERATION_XY_INTAKE,// Идентификатор параметра
                           3,                         // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MMSS2,        // Единицы измерения параметра
                           TYPE_DATA_INT16_4,           // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           10000.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[5] = { // Вибрация Vy
                           TMS_ACCELERATION_Y_INTAKE,// Идентификатор параметра
                           4,                         // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MMSS2,        // Единицы измерения параметра
                           TYPE_DATA_INT16_4,           // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           10000.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[6] = { // Давление на приёме насоса
                           TMS_PRESSURE_INTAKE,
                           5,               // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_ATM,    // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           1,               // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           350.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[7] = { // Вибрация Vz
                           TMS_ACCELERATION_Z_INTAKE, // Идентификатор параметра
                           6,                         // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MMSS2,        // Единицы измерения параметра
                           TYPE_DATA_INT16_4,           // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           10000.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[8] = { // Версия ПО
                           TMS_SOFT_TMSN,
                           7,               // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           0.01,            // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           99900.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[9] = { // Слово состояние системы
                           TMS_PSW_TMS,
                           8,               // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16_4, // Тип данных
                           0,               // Индекс
                           1,               // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           31.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
}

TmsEtalon::TmsEtalon()
{

}

TmsEtalon::~TmsEtalon()
{
  delete dm_;
}

void TmsEtalon::init()
{
  initModbusParameters();                   // Инициализация modbus карты

  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 19200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);

  initParameters();
  readParameters();                         // Чтение параметров из памяти
}

void TmsEtalon::initParameters()
{
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int indexModbus = 0; indexModbus < count; indexModbus++) {
    int id = dm_->getFieldID(indexModbus);
    if (id <= 0)
      continue;
    int indexArray = getIndexAtId(id);                                   // Получаем индекс параметра в банке параметров
    if (indexArray) {
      setFieldAccess(indexArray, ACCESS_OPERATOR);
      setFieldOperation(indexArray, dm_->getFieldOperation(indexModbus));
      setFieldPhysic(indexArray, dm_->getFieldPhysic(indexModbus));
      float tempVal = dm_->getFieldMinimum(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMin(id, tempVal);
      tempVal = dm_->getFieldMaximum(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setMax(id, tempVal);
      tempVal = dm_->getFieldDefault(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldDef(indexArray, tempVal);
      setFieldValidity(indexArray, dm_->getFieldValidity(indexModbus));
      setFieldValue(indexArray, getFieldDefault(indexArray));
    }
  }
}

void TmsEtalon::getNewValue(uint16_t id)
{
  float value = 0;
  ModbusParameter *param = dm_->getFieldAll(dm_->getIndexAtId(id));

  if (param->validity != ok_r) {
    value = NAN;
    setValue(id, value);
    return;
  }

  switch (param->typeData) {
  case TYPE_DATA_INT16:
  case TYPE_DATA_INT16_4:
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
  value = convertTo(value, param->physic, param->unit);

  switch (id) {
  default:
    setValue(id, value);
    break;
  }
}

uint8_t TmsEtalon::setNewValue(uint16_t id, float value, EventType eventType)
{
  switch (id) {
  default:
    int result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsEtalon::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

bool TmsEtalon::isConnect()
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

void TmsEtalon::getConnect()
{
  Tms::setConnect(dm_->getMms()->getCounters());
}

void TmsEtalon::resetConnect()
{
  Tms::resetConnect();
  dm_->getMms()->resetCounters();
}

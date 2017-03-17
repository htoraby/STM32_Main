/*
 * classTmsNovomet.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "tms_novomet.h"
#include "user_main.h"

void TmsNovomet::initModbusParameters()
{
  modbusParameters_[0] = {// Пустой регистр
                          TMS_BEGIN,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,                // Индекс
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
  modbusParameters_[1] = {// Слово состояние системы
                           TMS_PSW_TMS,     // Идентификатор параметра
                           1537,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,                // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           65535.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[2] = {// Сопротивление изоляции
                           TMS_RESISTANCE_ISOLATION,
                           1538,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_RESISTANCE,
                           RESISTANCE_KOM,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,                // Индекс
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
  modbusParameters_[3] = {// Температура жидкости на приёме насоса
                           TMS_TEMPERATURE_INTAKE,
                           1539,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           150.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[4] = {// Температура обмоток двигателя
                           TMS_TEMPERATURE_WINDING,
                           1541,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           220.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[5] = {// Давление жидкости на приёме
                           TMS_PRESSURE_INTAKE,
                           1543,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_MPA,    // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           40.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[6] = {// Радиальная вибрация двигателя
                           TMS_SPEED_XY_INTAKE,
                           1545,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_SPEED,    // Физическая величина параметра
                           SPEED_MM_SS,     // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           50.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[7] = {// Осевая вибрация двигателя
                           TMS_SPEED_Z_INTAKE,
                           1547,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_SPEED,    // Физическая величина параметра
                           SPEED_MM_SS,     // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           50.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[8] = {// Зенитный угол двигателя
                           TMS_ANGLE_INTAKE,
                           1549,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_ANGLE,    // Физическая величина параметра
                           ANGLE_DEGREES,   // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           90.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[9] = {// Слово состояние блока двигателя
                           TMS_PSW_TMSP_1,
                           1551,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           90.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[10] = {// Температура жидкости на выкиде
                           TMS_TEMPERATURE_DISCHARGE,
                           1552,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           150.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[11] = {// Давление жидкости на выкиде
                           TMS_PRESSURE_DISCHARGE,
                           1554,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_MPA,
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1,               // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           40.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[12] = {// Радиальная вибрация насоса
                           TMS_SPEED_XY_DISCHARGE,
                           1556,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_SPEED,    // Физическая величина параметра
                           SPEED_MM_SS,     // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           50.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[13] = {// Осевая вибрация насоса
                           TMS_SPEED_Z_DISCHARGE,
                           1558,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_SPEED,    // Физическая величина параметра
                           SPEED_MM_SS,     // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           50.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[14] = {// Зенитный угол насоса
                           TMS_ANGLE_DISCHARGE,
                           1560,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_ANGLE,    // Физическая величина параметра
                           ANGLE_DEGREES,   // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           90.0,            // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[15] = {// Слово состояние погружного блока 2
                           TMS_PSW_TMSP_2,
                           1562,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           65535.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[16] = {// Расход на выкиде
                           TMS_FLOW_DISCHARGE,
                           1563,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_FLOW,     // Физическая величина параметра
                           FLOW_M3_DD,      // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           65535.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[17] = {// Обводнённость на выкиде насоса
                           TMS_HOWMIDITY_DISCHARGE,
                           1565,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PERCENT,  // Физическая величина параметра
                           PERCENT_PERCENT, // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           -1.0,            // Минимальное значение параметра
                           100.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[18] = {// Версия ПО ТМСН
                           TMS_SOFT_TMSN,
                           1586,            // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           0.0001,          // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           65535,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
}

TmsNovomet::TmsNovomet()
{

}

TmsNovomet::~TmsNovomet()
{
  delete dm_;
}

void TmsNovomet::init()
{
  initModbusParameters();                   // Инициализация modbus карты

  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 9600, 8, UART_STOPBITS_1, UART_PARITY_NONE, 37);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);

  initParameters();
  readParameters();                         // Чтение параметров из памяти
}

void TmsNovomet::initParameters()
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

void TmsNovomet::getNewValue(uint16_t id)
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

uint8_t TmsNovomet::setNewValue(uint16_t id, float value, EventType eventType)
{
  switch (id) {
  case TMS_PRESSURE_UNIT:
    return setUnitPressure(value);
  case TMS_TEMPERATURE_UNIT:
    return setUnitTemperature(value);    
  default:
    int result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsNovomet::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

int TmsNovomet::setUnitPressure(float unit)
{
  if (Tms::setUnitPressure(unit)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "TmsNovomet::setUnitPressure() (unit = %d)",
                 unit);
#endif
    return err_r;
  }
  else {
    switch ((uint16_t)getValue(TMS_PRESSURE_UNIT)) {
    case PRESSURE_MPA:
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "TmsNovomet::setUnitPressure() Set MPa (unit = %d)",
                 unit);
#endif
      writeToDevice(TMS_PRESSURE_UNIT, 0);
      break;
    case PRESSURE_ATM:
      return err_r;
    case PRESSURE_AT:
      return err_r;
      break;
    case PRESSURE_BAR:
      return err_r;
      break;
    case PRESSURE_PSI:
      writeToDevice(TMS_TEMPERATURE_UNIT, 1);
      break;
    }
    return ok_r;
  }
}

int TmsNovomet::setUnitTemperature(float unit)
{
  if (Tms::setUnitTemperature(unit)) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "TmsNovomet::setUnitTemperature() (unit = %d)",
                 unit);
#endif
    return err_r;
  }
  else {
    writeToDevice(TMS_TEMPERATURE_UNIT, unit);
    return ok_r;
  }
}

bool TmsNovomet::isConnect()
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

void TmsNovomet::getConnect()
{
  Tms::setConnect(dm_->getMms()->getCounters());
}

void TmsNovomet::resetConnect()
{
  Tms::resetConnect();
  dm_->getMms()->resetCounters();
}

/*
 * classTmsElekton3.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "tms_elekton_3.h"
#include "user_main.h"

TmsElekton3::TmsElekton3()
{
	// TODO Auto-generated constructor stub

}

TmsElekton3::~TmsElekton3()
{
  delete dm_;
}

void TmsElekton3::initModbusParameters()
{
  modbusParameters_[0] = { // Пустой регистр
                           TMS_BEGIN,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,               // Индекс
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
  modbusParameters_[1] = { // 00Fh Статус состояния ТМСН
                           TMS_PSW_TMSN_ELEKTON,      // Идентификатор параметра
                           15,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_NUMERIC,            // Физическая величина параметра
                           NUMERIC_NUMBER,            // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[2] = { // 00Fh Статус состояния ТМСН
                           TMS_PSW_TMSN_ELEKTON,      // Идентификатор параметра
                           15,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_NUMERIC,            // Физическая величина параметра
                           NUMERIC_NUMBER,            // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[3] = { // 02Dh Rиз. - сопротивление изоляции, КОм
                           TMS_RESISTANCE_ISOLATION,  // Идентификатор параметра
                           45,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_RESISTANCE,         // Физическая величина параметра
                           RESISTANCE_KOM,            // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[4] = { // 02Eh Pпласта - давление пласта
                           TMS_PRESSURE_INTAKE,       // Идентификатор параметра
                           46,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_PRESSURE,           // Физическая величина параметра
                           PRESSURE_ATM,              // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[5] = { // 02Fh Q - расход
                           TMS_FLOW_DISCHARGE,        // Идентификатор параметра
                           47,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_FLOW,               // Физическая величина параметра
                           FLOW_M3_DD,                // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[6] = { // 030h Тдвигателя - температура масла ПЭД
                           TMS_TEMPERATURE_MOTOR,     // Идентификатор параметра
                           48,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_TEMPERATURE,        // Физическая величина параметра
                           TEMPERATURE_C,             // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[7] = { // 031h Тпласта - температура пласта
                           TMS_TEMPERATURE_INTAKE,    // Идентификатор параметра
                           49,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_TEMPERATURE,        // Физическая величина параметра
                           TEMPERATURE_C,             // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[8] = { // 032h Рвык.насоса - давление на выкиде насоса
                           TMS_PRESSURE_DISCHARGE,    // Идентификатор параметра
                           50,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_PRESSURE,           // Физическая величина параметра
                           PRESSURE_ATM,              // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[9] = { // 033h Твык.насоса - температура на выкиде насоса
                           TMS_TEMPERATURE_DISCHARGE, // Идентификатор параметра
                           51,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_TEMPERATURE,        // Физическая величина параметра
                           TEMPERATURE_C,             // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[10] = { // 034h Gху - вибрация в поперечной плоскости(ху)
                           TMS_ACCELERATION_XY_INTAKE,// Идентификатор параметра
                           52,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[11] = { // 035h Gz - вибрация в продольной плоскости(z)
                           TMS_ACCELERATION_Z_INTAKE,// Идентификатор параметра
                           53,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[12] = { // 036h ID1, ID2 - конфигурация погружного блока
                           TMS_CONSTRUCTION_TMSP_ELEKTON,// Идентификатор параметра
                           54,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[13] = { // 037h Состав датчиков в ТМСП
                           TMS_SENSOR_TMSP_ELEKTON,   // Идентификатор параметра
                           75,                        // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_UINT16,          // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           65535.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };

}

void TmsElekton3::initParameters()
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

void TmsElekton3::init()
{
  initModbusParameters();                             // Инициализация modbus карты
  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 9600, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);
  initParameters();                         // Инициализация параметров
  readParameters();                         // Чтение параметров из памяти
}

void TmsElekton3::getNewValue(uint16_t id)
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
  value = (value - (units[param->physic][param->unit][1]))/(units[param->physic][param->unit][0]);

  switch (id) {
  default:
    setValue(id, value);
    break;
  }
}

uint8_t TmsElekton3::setNewValue(uint16_t id, float value, EventType eventType)
{
  switch (id) {
  default:
    int result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsElekton3::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

void TmsElekton3::checkTypeTMSP(int id)
{
  if (parameters.isValidity(TMS_PSW_TMSN_ELEKTON)) {
    if (checkBit(parameters.get(TMS_PSW_TMSN_ELEKTON), DHS_ELEKTON_3_STATUS_A12)) {
      ElektonTypeTMSP_ = DHS_ELEKTON_TMSP_3;
    }
    else {
      ElektonTypeTMSP_ = DHS_ELEKTON_TMSP_2;
    }
  }
}

void TmsElekton3::checkDiscretePressure(int id)
{

}

int TmsElekton3::setUnitPressure(float unit)
{
  return 0;
}

int TmsElekton3::setUnitTemperature(float unit)
{
  return 0;
}

bool TmsElekton3::isConnect()
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


#include "tms_borets_2.h"
#include "user_main.h"

void TmsBorets2::initModbusParameters()
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
  modbusParameters_[1] = { // 240h Rиз. - сопротивление изоляции, КОм
                           TMS_RESISTANCE_ISOLATION,
                           0x240,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_RESISTANCE,
                           RESISTANCE_KOM,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
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
  modbusParameters_[2] = { // 241h Тпласта - температура пласта
                           TMS_TEMPERATURE_INTAKE,
                           0x241,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           0.01,            // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           25500.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[3] = { // 242h Тдвигателя - температура обмодки ПЭД
                           TMS_TEMPERATURE_WINDING,
                           0x242,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           0.01,            // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           25500.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[4] = { // 243h Gх - вибрация в поперечной плоскости(х)
                           TMS_ACCELERATION_X_INTAKE, // Идентификатор параметра
                           0x243,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_INT16,         // Тип данных
                           0,                         // Индекс
                           0.01,                      // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           1000.0,                    // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[5] = { // 244h Gy - вибрация в поперечной плоскости(y)
                           TMS_ACCELERATION_Y_INTAKE, // Идентификатор параметра
                           0x244,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_INT16,         // Тип данных
                           0,                         // Индекс
                           0.01,                      // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           1000.0,                    // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[6] = { // 245h Gz - вибрация в продольной плоскости(z)
                           TMS_ACCELERATION_Z_INTAKE, // Идентификатор параметра
                           0x245,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_INT16,         // Тип данных
                           0,                         // Индекс
                           0.01,                      // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           1000.0,                    // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[7] = { // 246h Давление жидкости на приёме
                           TMS_PRESSURE_INTAKE,
                           0x246,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_MPA,    // Единицы измерения параметра
                           TYPE_DATA_UINT32,// Тип данных
                           0,               // Индекс
                           0.0001,          // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           1023000.0,       // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[8] = { // 248h Давление жидкости на выкиде
                           TMS_PRESSURE_DISCHARGE,
                           0x248,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_MPA,    // Единицы измерения параметра
                           TYPE_DATA_UINT32,// Тип данных
                           0,               // Индекс
                           0.0001,          // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           1023000.0,       // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[9] = { // 24Ah Температура обмотки вверху ПЭД
                           TMS_TEMPERATURE_MOTOR,
                           0x24A,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           0.01,            // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           25500.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[10] = { // 24Bh Температура жидкости на выходе насоса
                           TMS_TEMPERATURE_DISCHARGE,
                           0x24B,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           0.01,            // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           25500.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[11] = { // Расход
                           TMS_FLOW_DISCHARGE,
                           0x24C,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_FLOW,     // Физическая величина параметра
                           FLOW_M3_DD,      // Единицы измерения параметра
                           TYPE_DATA_UINT16,// Тип данных
                           0,               // Индекс
                           0.1,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           65535.0,          // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                         };
  modbusParameters_[12] = { // Версия ПО ТМСН
                           TMS_SOFT_TMSN,
                           0x253,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_UINT32, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           4294967295.0,    // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
}

TmsBorets2::TmsBorets2()
{

}

TmsBorets2::~TmsBorets2()
{
  delete dm_;
}

void TmsBorets2::init()
{
  initModbusParameters();                   // Инициализация modbus карты

  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 19200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 0x32);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);

  initParameters();
  readParameters();                         // Чтение параметров из памяти
}

void TmsBorets2::initParameters()
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

void TmsBorets2::getNewValue(uint16_t id)
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
  case TMS_SOFT_TMSN:
    {
      unTypeData data;
      data.uint16_t[0] = bcdToDec(param->value.uint16_t[0]);
      data.uint16_t[1] = bcdToDec(param->value.uint16_t[1]);
      value = data.uint16_t[1]*10000 + data.uint16_t[0];
      setValue(id, value);
    }
    break;
  default:
    setValue(id, value);
    break;
  }
}

uint8_t TmsBorets2::setNewValue(uint16_t id, float value, EventType eventType)
{
  switch (id) {
  default:
    int result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsBorets2::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

bool TmsBorets2::isConnect()
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

void TmsBorets2::getConnect()
{
  Tms::setConnect(dm_->getMms()->getCounters());
}

void TmsBorets2::resetConnect()
{
  Tms::resetConnect();
  dm_->getMms()->resetCounters();
}

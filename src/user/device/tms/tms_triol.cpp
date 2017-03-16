#include "tms_triol.h"
#include "user_main.h"

void TmsTriol::initModbusParameters()
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
                           10000.0,         // Максимально значение параметра
                           10000.0,         // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[2] = { // 241h Тпласта - Температура пласта
                           TMS_TEMPERATURE_INTAKE,
                           0x241,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           255.0,           // Максимально значение параметра
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
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           255.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[4] = { // 243h Gху - вибрация в поперечной плоскости(ху)
                           TMS_ACCELERATION_XY_INTAKE,// Идентификатор параметра
                           0x243,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           300.0,                     // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[5] = { // 244h Gz - вибрация в продольной плоскости(z)
                           TMS_ACCELERATION_Z_INTAKE, // Идентификатор параметра
                           0x244,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_MSS2,         // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           0.1,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           300.0,                     // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[6] = { // 245h Давление жидкости на приёме
                           TMS_PRESSURE_INTAKE,
                           0x245,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_ATM,    // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           0.1,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           99990.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[7] = { // 246h Давление жидкости на приёме высокой точности
                           TMS_PRESSURE_INTAKE_HD,
                           0x246,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_ATM,    // Единицы измерения параметра
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           40000.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[8] = { // 248h Температура жидкости на выкиде
                           TMS_TEMPERATURE_DISCHARGE,
                           0x248,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           150.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[9] = { // 249h Температура нижней лобовой части ПЭД
                           TMS_TEMPERATURE_MOTOR,
                           0x249,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           255.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[10] = { // 24Ah Давление жидкости на выкиде
                           TMS_PRESSURE_DISCHARGE,
                           0x24A,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_ATM,
                           TYPE_DATA_FLOAT, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           40000.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[11] = { // 250h Слово состояние системы
                           TMS_PSW_TMS,     // Идентификатор параметра
                           0x250,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_UINT16,// Тип данных
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
  modbusParameters_[12] = { // 251h Слово состояния БИД
                           TMS_PSW_TMSP_1,  // Идентификатор параметра
                           0x251,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_UINT16,// Тип данных
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
  modbusParameters_[13] = { // Версия ПО ТМСП
                           TMS_SOFT_TMSP,
                           0x255,           // Адрес регистра в устройстве
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
  modbusParameters_[14] = { // Версия ПО ТМСН
                           TMS_SOFT_TMSN,
                           0x258,           // Адрес регистра в устройстве
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
  modbusParameters_[15] = { // 274h Слово состояния БИГР
                           TMS_PSW_TMSP_2,  // Идентификатор параметра
                           0x274,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_UINT16,// Тип данных
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
}

TmsTriol::TmsTriol()
{

}

TmsTriol::~TmsTriol()
{
  delete dm_;
}

void TmsTriol::init()
{
  initModbusParameters();                   // Инициализация modbus карты

  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 19200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 0x33);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);

  initParameters();
  readParameters();                         // Чтение параметров из памяти
}

void TmsTriol::initParameters()
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

void TmsTriol::getNewValue(uint16_t id)
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

uint8_t TmsTriol::setNewValue(uint16_t id, float value, EventType eventType)
{
  switch (id) {
  default:
    int result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsTriol::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

bool TmsTriol::isConnect()
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

void TmsTriol::getConnect()
{
  Tms::setConnect(dm_->getMms()->getCounters());
}

void TmsTriol::resetConnect()
{
  Tms::resetConnect();
  dm_->getMms()->resetCounters();
}

#include "tms_pic_v2.h"
#include "user_main.h"

void TmsPicV2::initModbusParameters()
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
  modbusParameters_[1] = { // FFh Слово состояние системы
                           TMS_PSW_TMS,     // Идентификатор параметра
                           255,             // Адрес регистра в устройстве
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
  modbusParameters_[2] = { // 17Eh Статус загрузки
                           TMS_BOOT_STATUS, // Идентификатор параметра
                           382,             // Адрес регистра в устройстве
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
  modbusParameters_[3] = { // 180h Давление жидкости на приёме
                           TMS_PRESSURE_INTAKE,
                           384,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_BAR,    // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,          // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[4] = { // 181h Давление жидкости на выкиде
                           TMS_PRESSURE_DISCHARGE,
                           385,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_PRESSURE, // Физическая величина параметра
                           PRESSURE_BAR,
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[5] = { // 182h Тпласта - температура пласта
                           TMS_TEMPERATURE_INTAKE,
                           386,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[6] = { // 183h Тдвигателя - температура обмодки ПЭД
                           TMS_TEMPERATURE_WINDING,
                           387,           // Адрес регистра в устройстве
                           OPERATION_READ,  // Операции с параметром
                           PHYSIC_TEMPERATURE,
                           TEMPERATURE_C,   // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,           // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           EVERY_TIME,      // Частота опроса параметра
                           EVERY_TIME,      // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[7] = { // 185h Gху - вибрация в поперечной плоскости(ху)
                           TMS_ACCELERATION_XY_INTAKE,// Идентификатор параметра
                           389,                     // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_ACCELERATION,       // Физическая величина параметра
                           ACCELERATION_G,            // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           32727.0,                      // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[8] = { // 186h Активные утечки тока
                           TMS_ACTIVE_CURRENT_LEAKAGE,// Идентификатор параметра
                           390,                       // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_CURRENT,            // Физическая величина параметра
                           CURRENT_MA,                // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           32727.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[9] = { // 187h Пассивные утечки тока
                           TMS_PASSIVE_CURRENT_LEAKAGE,// Идентификатор параметра
                           391,                       // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_CURRENT,            // Физическая величина параметра
                           CURRENT_MA,                // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           32727.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[10] = { // 188h Cz
                           TMS_CZ,                    // Идентификатор параметра
                           392,                       // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_CURRENT,            // Физическая величина параметра
                           CURRENT_MA,                // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           32727.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[11] = { // 189h Cf
                           TMS_CF,                    // Идентификатор параметра
                           393,                       // Адрес регистра в устройстве
                           OPERATION_READ,            // Операции с параметром
                           PHYSIC_CURRENT,            // Физическая величина параметра
                           CURRENT_MA,                // Единицы измерения параметра
                           TYPE_DATA_INT16,           // Тип данных
                           0,                         // Индекс
                           1.0,                       // Коэффициент преобразования параметра
                           0.0,                       // Минимальное значение параметра
                           32727.0,                   // Максимально значение параметра
                           0.0,                       // Считываемое значение "по умолчанию"
                           EVERY_TIME,                // Частота опроса параметра
                           EVERY_TIME,                // Количество запросов к параметру
                           OPERATION_ERROR,           // Команда
                           VALIDITY_ERROR,            // Поле состояния параметра
                           0                          // Значение
                          };
  modbusParameters_[12] = { // 200h Сброс пассивных утечек тока
                           TMS_PASSIVE_CURRENT_LEAKAGE_CONTROL,     // Идентификатор параметра
                           512,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           1.0,             // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[13] = { // 201h Время режима RSR
                           TMS_RSR_TIME,    // Идентификатор параметра
                           513,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_TIME,     // Физическая величина параметра
                           TIME_MM,         // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[14] = { // 202h Включение режима RSR
                           TMS_RSR_CONTROL,     // Идентификатор параметра
                           514,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           1.0,             // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[15] = { // 205h Сброс PICv2 (4112 + 257)
                           TMS_RESTART_PIC_V2,     // Идентификатор параметра
                           517,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           32727.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           NOT_READ,        // Частота опроса параметра
                           NOT_READ,        // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[16] = { // 207h Задание единиц измерения (5)
                           TMS_UNITS_MEASURE,     // Идентификатор параметра
                           519,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_INT16, // Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           7.0,             // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
  modbusParameters_[17] = { // 258h Тип погружного оборудования
                           TMS_TOOL_TYPE,     // Идентификатор параметра
                           600,             // Адрес регистра в устройстве
                           OPERATION_WRITE, // Операции с параметром
                           PHYSIC_NUMERIC,  // Физическая величина параметра
                           NUMERIC_NUMBER,  // Единицы измерения параметра
                           TYPE_DATA_UINT16,// Тип данных
                           0,               // Индекс
                           1.0,             // Коэффициент преобразования параметра
                           0.0,             // Минимальное значение параметра
                           65535.0,         // Максимально значение параметра
                           0.0,             // Считываемое значение "по умолчанию"
                           OFTEN,           // Частота опроса параметра
                           OFTEN,           // Количество запросов к параметру
                           OPERATION_ERROR, // Команда
                           VALIDITY_ERROR,  // Поле состояния параметра
                           0                // Значение
                          };
}

TmsPicV2::TmsPicV2()
  : isConfigurated_(false)
{

}

TmsPicV2::~TmsPicV2()
{
  delete dm_;
}

void TmsPicV2::init()
{
  initModbusParameters();                   // Инициализация modbus карты

  createThread("UpdParamsTms");
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                         TMS_UART, 9600, 8, UART_STOPBITS_1, UART_PARITY_NONE, 0x01);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);

  initParameters();
  readParameters();                         // Чтение параметров из памяти
}

void TmsPicV2::initParameters()
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

void TmsPicV2::getNewValue(uint16_t id)
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
  case TYPE_DATA_INT32:
    value = (float)param->value.int32_t;
    break;
  case TYPE_DATA_UINT32:
    value = (float)param->value.uint32_t;
    break;
  case TYPE_DATA_FLOAT:
    value = (float)param->value.float_t;
    break;
  default:
    break;
  }
  value = value * param->coefficient;
  value = convertTo(value, param->physic, param->unit);

  switch (id) {
  case TMS_PSW_TMS:
    setValue(id, value);
    config();
    break;
  default:
    setValue(id, value);
    break;
  }
}

uint8_t TmsPicV2::setNewValue(uint16_t id, float value, EventType eventType)
{
  int result = 0;
  switch (id) {
  case TMS_RESTART_PIC_V2:
    result = setValue(id, value, eventType);
    if (!result && value) {
      writeToDevice(id, 4112);
      osDelay(500);
      writeToDevice(id, 257);
      osDelay(500);
    }
    return result;
  default:
    result = setValue(id, value, eventType);
    if (!result)
      writeToDevice(id, value);
    return result;
  }
}

void TmsPicV2::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}

bool TmsPicV2::config()
{
  if (parameters.isValidity(TMS_PSW_TMS) && !isConfigurated_) {
    setNewValue(TMS_UNITS_MEASURE, 5.0);
    osDelay(500);
    isConfigurated_ = true;
    return true;
  }
  else {
    isConfigurated_ = false;
    return false;
  }
}

bool TmsPicV2::isConnect()
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

void TmsPicV2::getConnect()
{
  Tms::setConnect(dm_->getMms()->getCounters());
}

void TmsPicV2::resetConnect()
{
  Tms::resetConnect();
  dm_->getMms()->resetCounters();
}

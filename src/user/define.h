/*
 * define.h
 *
 *  Created on: 14.06.2012
 *      Author: trofimenko
 */

#ifndef DEFINE_H_
#define DEFINE_H_

/*!
 * \brief The unDataType union
 * Объединение для преобразования типов данных
 */
union unTypeData
{
  unsigned char      char_t[4];
  signed short int   int16_t[2];
  signed long int    int32_t;
  unsigned short int uint16_t[2];
  unsigned long int  uint32_t;
  float              float_t;
};

/*!
 * \brief The enTypeData enum
 * Перечисление типов данных
 */
enum enTypeData {
  TYPE_DATA_ERROR  = 0,
  TYPE_DATA_CHAR   = 1,
  TYPE_DATA_INT16  = 2,
  TYPE_DATA_INT32  = 3,
  TYPE_DATA_UINT16 = 4,
  TYPE_DATA_UINT32 = 5,
  TYPE_DATA_FLOAT  = 6,
  TYPE_DATA_LAST
};

/*!
 * \brief The enPeriodExchange enum
 */
enum enPeriodExchange
{
  NOT_READ                        = 0,      /// Не читать
  EVERY_TIME                      = 1,      /// Каждый раз
  VERY_OFTEN                      = 10,     /// Очень часто
  OFTEN                           = 100,    /// Часто
  RARELY                          = 1000,   /// Редко
  HARDLY_EVER                     = 10000   /// Очень редко
};

enum enReturns
{
  RETURN_OK                     = 0,
  RETURN_ERROR                  = 1,
  RETURN_ERROR_ID_VALUE         = 2,
  RETURN_ERROR_ID               = 3,
  RETURN_ERROR_ACCESS_VALUE     = 4,
  RETURN_ERROR_ACCESS           = 5,
  RETURN_ERROR_OPERATION_VALUE  = 6,
  RETURN_ERROR_OPERATION        = 7,
  RETURN_ERROR_PHYSIC_VALUE     = 8,
  RETURN_ERROR_PHYSIC           = 9,
  RETURN_ERROR_VALIDITY_VALUE   = 10,
  RETURN_ERROR_VALIDITY         = 11,
  RETURN_ERROR_MAXIMUM          = 12,
  RETURN_ERROR_MINIMUM          = 13,
  RETURN_ERROR_TYPE_DATA_VALUE  = 14,
  RETURN_ERROR_UNIT_VALUE       = 15,
  RETURN_MODBUS_TIMEOUT         = 16,
  RETURN_MODBUS_ERROR_CRC       = 17,
  RETURN_MODBUS_ERROR           = 18,
  RETURN_MODBUS_OK              = 19,
  RETURN_ERROR_UPDATE_VALUE     = 20,
  RETURN_LAST
};

/*!
 * \brief Список уровеней доступа
 */
enum enAccess
{
  ACCESS_ERROR          = 0,
  ACCESS_OPERATOR				= 1,
  ACCESS_TECHNOLOG			= 2,
  ACCESS_SERVICE				= 3,
  ACCESS_GOD            = 4,
	ACCESS_LAST
};

/*!
 * \brief Список команд
 */
enum enOperation
{
  OPERATION_ERROR			  = 0,                //!<
  OPERATION_READ				= 1,                //!< Параметр только для чтения
  OPERATION_WRITE				= 2,                //!< Параметр только для записи
  OPERATION_LIMITED			= 3,                //!< Параметр для записи только во время останова
	OPERATION_LAST
};

/*!
 * \brief The enPhysic enum
 * Типы физических величин
 */
enum enPhysic
{
  PHYSIC_ERROR,
  PHYSIC_NUMERIC,                           //!< Просто число
  PHYSIC_DATE_TIME,                         //!< Дата и время
  PHYSIC_TIME,                              //!< Время
  PHYSIC_PERCENT,                           //!< Проценты
  PHYSIC_LENGHT,                            //!< Длина
  PHYSIC_DENSITY,                           //!< Плотность
  PHYSIC_SPACE,                             //!< Площадь
  PHYSIC_FREQUENCY,                         //!< Частота
  PHYSIC_VOLTAGE,                           //!< Напряжение
  PHYSIC_CURRENT,                           //!< Ток
  PHYSIC_ACTIVE_POWER,                      //!< Активная мощность
  PHYSIC_FULL_POWER,                        //!< Полная мощность
  PHYSIC_REACTIVE_POWER,                    //!< Реактивная мощность
  PHYSIC_ANGLE,                             //!< Угол
  PHYSIC_RPM,                               //!< Скорость вращения
  PHYSIC_TEMP,                              //!< Темп (разгона или замедления)
  PHYSIC_RESISTANCE,                        //!< Сопротивление изоляции
  PHYSIC_RESIST_CABLE,                      //!< Сопротивление кабельной линии
  PHYSIC_PRESSURE,                          //!< Давление
  PHYSIC_TEMPERATURE,                       //!< Температура
  PHYSIC_ACCELERATION,                      //!< Ускорение
  PHYSIC_SPEED,                             //!< Cкорость
  PHYSIC_VISCOSITY,                         //!< Вязкость
  PHYSIC_FLOW,                              //!< Расход
  PHYSIC_POWER,                             //!< Мощность
  PHYSIC_ACTIVE_ENERGY,                     //!< Активная энергия
  PHYSIC_REACTIVE_ENERGY,                   //!< Реактивная энергия
  PHYSIC_FULL_ENERGY,                       //!< Полная энергия
  PHYSIC_RATE,                              //!< Номинальный момент
  PHYSIC_INDUNCTANCE,                       //!< Индуктивность Гн
  PHYSIC_LAST
};

enum enPhysicNumeric {
  NUMERIC_NUMBER,
  NUMERIC_LAST
};

enum enPhysicDateTime {
  DATE_TIME_SS,
  DATE_TIME_LAST
};

enum enPhysicTime {
  TIME_SS,
  TIME_US,
  TIME_MS,
  TIME_SS_MS,
  TIME_MM_SS,
  TIME_HH,
  TIME_HH_MM,
  TIME_HH_MM_SS,
  TIME_YY_MM_DD,
  TIME_YY_MM_DD_HH_MM_SS,
  TIME_LAST
};

enum enPhysicPercent {
  PERCENT_PERCENT,
  PERCENT_LAST
};

enum enPhysicLength {
  LENGTH_M,
  LENGTH_MILE,
  LENGTH_FOOT,
  LENGTH_LAST
};

enum enPhysicDensity {
  DENSITY_KG_M3,
  DENSITY_LAST
};

enum enPhysicSpace {
  SPACE_M2,
  SPACE_LAST
};

enum enPhysicFrequency {
  FREQUENCY_HZ,
  FREQUENCY_KHZ,
  FREQUENCY_LAST
};

enum enPhysicVoltage {
  VOLTAGE_V,
  VOLTAGE_KV,
  VOLTAGE_LAST
};

enum enPhysicCurrent {
  CURRENT_A,
  CURRENT_MA,
  CURRENT_LAST
};

enum enPhysicActivPower {
  ACTIV_POWER_W,
  ACTIV_POWER_KW,
  ACTIV_POWER_LAST
};

enum enPhysicFullPower {
  FULL_POWER_VA,
  FULL_POWER_KVA,
  FULL_POWER_LAST
};

enum enPhysicReactivPower {
  REACTIV_POWER_VAR,
  REACTIV_POWER_KVAR,
  REACTIV_POWER_LAST,
};

enum enPhysicAngle {
  ANGLE_DEGREES,
  ANGLE_LAST
};

enum enPhysicRpm {
  RPM_RPM,
  RPM_LAST
};

enum enPhysicTemp {
  TEMP_HZ_SS,
  TEMP_LAST
};

enum enPhysicResistence {
  RESISTANCE_OM,
  RESISTANCE_KOM,
  RESISTANCE_MOM,
  RESISTANCE_MILIOM,
  RESISTANCE_LAST
};

enum enPhysicResisCable {
  RESIST_CABLE_KOM_M,
  RESIST_CABLE_LAST
};

enum enPhysicPressure {
  PRESSURE_MPA,
  PRESSURE_ATM,
  PRESSURE_AT,
  PRESSURE_BAR,
  PRESSURE_PSI,
  PRESSURE_LAST
};

enum enPhysicTemperature {
  TEMPERATURE_C,
  TEMPERATURE_LAST
};

enum enPhysicAcceleration {
  ACCELERATION_G,
  ACCELERATION_MSS2,
  ACCELERATION_LAST
};

enum enPhysicSpeed {
  SPEED_M_SS,
  SPEED_LAST
};

enum enPhysicViscosity {
  VISCOSITY_MPA_SS,
  VISCOSITY_LAST
};

enum enPhysicFlow {
  FLOW_M3_DD,
  FLOW_LAST
};

enum emPhysicPower {
  POWER_W,
  POWER_KW,
  POWER_LAST
};

enum enPhysicActivEnergy {
  ACTIVE_ENERGY_W_HH,
  ACTIVE_ENERGY_KW_HH,
  ACTIVE_ENERGY_LAST,
};

enum enPhysicReactivEnergy {
  REACTIVE_ENERGY_VAR_HH,
  REACTIVE_ENERGY_KVAR_HH,
  REACTIVE_ENERGY_LAST,
};

enum enPhysicFullEnergy {
  FULL_ENERGY_VAHH,
  FULL_ENERGY_LAST
};

enum enPhysicRate {
  RATE_NM,
  RATE_LAST
};

enum enPhysicIndunctance {
  INDUNCTANCE_MH,
  INDUNCTANCE_LAST
};

/*!
 * \brief Перечисление типов двигателя
 */
enum enMotorType {
  VSD_MOTOR_TYPE_ASYNC = 0,                 /// Асинхронный двигатель
  VSD_MOTOR_TYPE_VENT                       /// Вентильный двигатель
};

/*!
 * \brief Перечисление направлений вращений двигателя
 */
enum enRotation {
  VSD_ROTATION_DIRECT = 0,                   /// Прямое(правое) направление вращения
  VSD_ROTATION_REVERSE                       /// Обратное(левое) направление вращения
};

/*!
 * \brief Перечисление типов управления двигателем
 */
enum enMotorControl {
  VSD_MOTOR_CONTROL_UF = 0,                 /// Метод управления U/f
  VSD_MOTOR_CONTROL_VECT                    /// Векторное управление
};

/*!
 * \brief The enUfType enum
 * Перечисление типов зависимости U/f
 */
enum enUfType {
  /// По точкам
  VSD_UF_TYPE_POINT = 0,
  /// Линейное
  VSD_UF_TYPE_LINE,
  /// Квадратичное
  VSD_UF_TYPE_SQR,
  /// Обратно квадратичное
  VSD_UF_TYPE_SQRT
};

/*!
 * \brief The enNominalVoltage enum
 * Перечисление типов номинального напряжения
 */
enum enNominalVoltage {
  /// 380 вольт
  NOMINAL_VOLTAGE_380 = 0,
  /// 480 вольт
  NOMINAL_VOLTAGE_480,
  /// 500 вольт
  NOVINAL_VOLTAGE_500
};

/*!
 * \brief The enNominalFrequency enum
 * Перечисление номинальная частота
 */
enum enNominalFrequency {
  /// 50Гц
  NOMINAL_FREQUENCY_50 = 0,
  /// 60Гц
  NOMINAL_FREQUENCY_60
};

/*!
 * \brief The enCableCross enum
 * Перечисление сечений кабеля
 */
enum enCableCross {
  /// 16
  CABLE_CROSS_16 = 0,
  /// 21
  CABLE_CROSS_21,
  /// 25
  CABLE_CROSS_25,
  /// 33
  CABLE_CROSS_33,
  /// 42
  CABLE_CROSS_42
};

/*!
 * \brief The enTypeDHS enum
 * Перечисление типов ТМС
 */
enum enTypeDHS {
  /// Новомет
  TYPE_DHS_NOVOMET,
  /// Алмаз
  TYPE_DHS_ALMAZ,
  /// Борец
  TYPE_DHS_BORETS,
  /// Электон
  TYPE_DHS_ELEKTON,
  /// Эталон
  TYPE_DHS_ETALON,
  /// Ирз
  TYPE_DHS_IRZ,
  /// Орион
  TYPE_DHS_ORION,
  /// Шлюмберже
  TYPE_DHS_PIC_V2,
  /// Скад
  TYPE_DHS_SCAD,
  /// Скан
  TYPE_DHS_SCAN,
  /// Триол
  TYPE_DHS_TRIOL,
  /// Зенит
  TYPE_DHS_ZENIT
};

/*!
 * \brief The enByteRate enum
 * Скорость устройства по RS
 */
enum enByteRate {
  BYTE_RATE_9600,
  BYTE_RATE_14400,
  BYTE_RATE_19200,
  BYTE_RATE_38400,
  BYTE_RATE_57600,
  BYTE_RATE_115200
};

/*!
 * \brief The enSequencePhase enum
 * Порядок чередования фаз
 */
enum enSequencePhase {
  PHASE_ABC = 0,
  PHASE_CBA
};

/*!
 * \brief The enParamProt enum
 * Кретерий защиты
 */
enum enParamProt {
  PARAM_PROT_POWER = 0,
  PARAM_PROT_CURRENT
};

enum enValidity {
  VALIDITY_OK         = 0,
  VALIDITY_ERROR      = 1,
  VALIDITY_MIN        = 2,
  VALIDITY_MAX        = 3,
	VALIDITY_LAST
};

enum enUpdate {
  UPDATE_ERROR				= 0,
  UPDATE_LAST
};

enum enCcsCondition {
  CCS_CONDITION_STOP,                       /// Останов
  CCS_CONDITION_STOPPING,                   /// Торможение
  CCS_CONDITION_RUNNING,                    /// Запуск
  CCS_CONDITION_RUN                         /// Работа
};

enum enCcsWorkingMode {
  CCS_WORKING_MODE_MANUAL = 0,
  CCS_WORKING_MODE_AUTO,
  CCS_WORKING_MODE_PROGRAM
};

enum enVsdCondition {
  /// Состояние останов
  VSD_CONDITION_STOP,
  /// Состояние торможение
  VSD_CONDITION_STOPPING,
  /// Состояние ожидание начала торможения
  VSD_CONDITION_WAIT_STOP,
  /// Состояние работа
  VSD_CONDITION_RUN,
  /// Состояние запуск
  VSD_CONDITION_RUNNING,
  /// Состояние ожидание запуска
  VSD_CONDITION_WAIT_RUN
};

enum enCcsConditionFlag {
  CCS_CONDITION_FLAG_NULL    = 0,
  CCS_CONDITION_FLAG_DELAY,                      /// Ожидание срабатывания
  CCS_CONDITION_FLAG_RESTART,                    /// Ожидание АПВ
  CCS_CONDITION_FLAG_BLOCK                       /// Блокировка
};

/*!
 * \brief Список причин пуска
 */
enum LastReasonRun {
  LastReasonRunNone                     = 0,    //!< Нет пуска
  LastReasonRunOperator                 = 1,    //!< "Оператор"
  LastReasonRunAuto                     = 2,    //!< "Автоматический"
  LastReasonRunRemote                   = 3,    //!< "Телеуправление"
  LastReasonRunProgram                  = 4,    //!< "По программе"
  LastReasonRunApvUnderloadMotor        = 5,    //!< "АПВ по защите: Недогруз двигателя"
  LastReasonRunApvOverloadMotor         = 6,    //!< "АПВ по защите: Перегруз двигателя"
  LastReasonRunApvImbalanceCurMotor     = 7,    //!< "АПВ по защите: Дисбаланс токов ПЭД"
  LastReasonRunApvUnderVoltIn           = 8,    //!< "АПВ по защите: Снижение питания сети"
  LastReasonRunApvOverVoltIn            = 9,    //!< "АПВ по защите: Превышение питания сети"
  LastReasonRunApvImbalanceVoltIn       = 10,   //!< "АПВ по защите: Дисбаланс напряжения"
  LastReasonRunApvTurbineRotation       = 11,   //!< "АПВ по защите: Турбинное вращение"
  LastReasonRunApvPhaseSequence         = 12,   //!< "АПВ по защите: Чередование фаз"
  LastReasonRunApvLockDoor              = 13,   //!< "АПВ по защите: Блокировка двери"
  LastReasonRunApvPressureIntake        = 14,   //!< "АПВ по защите: Pmin на приёме насоса"
  LastReasonRunApvTemperatureMotor      = 15,   //!< "АПВ по защите: Перегрев двигателя"
  LastReasonRunApvVibrationMotor        = 16,   //!< "АПВ по защите: Превышение вибрации"
  LastReasonRunApvUnderPressure1        = 17,   //!< "АПВ по защите: "
  LastReasonRunApvMinAnalog1            = 18,   //!< "АПВ по защите: "
  LastReasonRunApvMaxAnalog1            = 19,   //!< "АПВ по защите: "
  LastReasonRunApvMinAnalog2            = 20,   //!< "АПВ по защите: "
  LastReasonRunApvMaxAnalog2            = 21,   //!< "АПВ по защите: "
  LastReasonRunApvFreqPower             = 22,   //!< "АПВ по защите: "
  LastReasonRunApvPower                 = 23,   //!< "АПВ по защите: "
  // Дополнительные причины пуска
  LastReasonRunMinAnalog3               = 100,  //!<
  LastReasonRunMinAnalog4               = 101,  //!<
  LastReasonRunMaxAnalog3               = 102,  //!<
  LastReasonRunMaxAnalog4               = 103,  //!<
  LastReasonRunApvCurrentMotor          = 104,  //!< "АПВ по защите: Предел тока двигателя"
  LastReasonRunApvOutOfSyncMotor        = 105,  //!< "АПВ по защите: Рассинхронизация ПВЭД"
  LastReasonRunApvResistIsolation       = 106,  //!< "АПВ по защите: Сопротивление изоляции"
  LastReasonRunApvHardwareVsd           = 107,  //!< "АПВ по защите: Аппаратные защиты ЧРП"
};

/*!
 * \brief Список причин останова
 */
enum LastReasonStop {
  LastReasonStopNone                    = 0,    //!< Нет останова
  LastReasonStopOperator                = 1,    //!< "Оператор"
  LastReasonStopResistIsolation         = 2,    //!< "Сопротивление изоляции"
  LastReasonStopUnderloadMotor          = 3,    //!< "Недогруз двигателя"
  LastReasonStopOverloadMotor           = 4,    //!< "Перегруз двигателя"
  LastReasonStopUnderVoltIn             = 5,    //!< "Снижение питания сети"
  LastReasonStopOverVoltIn              = 6,    //!< "Превышение питания сети"
  LastReasonStopHackSu                  = 9,    //!< "Взлом СУ"
  LastReasonStopManometr                = 12,   //!< "Контактный манометр"
  LastReasonStopPressureIntake          = 13,   //!< "Pmin на приёме насоса"
  LastReasonStopTemperatureMotor        = 14,   //!< "Перегрев двигателя"
  LastReasonStopVibrationMotor          = 16,   //!< "Превышение вибрации"
  LastReasonStopImbalanceCurMotor       = 20,   //!< "Дисбаланс токов ПЭД"
  LastReasonStopImbalanceVoltIn         = 21,   //!< "Дисбаланс напряжения"
  LastReasonStopProgram                 = 24,   //!< "По программе"
  LastReasonStopMaxAnalog1              = 26,   //!< "Ан.вх.№1 max"
  LastReasonStopMaxAnalog2              = 27,   //!< "Ан.вх.№2 max"
  LastReasonStopRemote                  = 28,   //!< "Телеуправление"
  LastReasonStopNoVoltage               = 30,   //!<
  LastReasonStopOverloadNoMinVolt       = 35,   //!<
  LastReasonStopUnderloadNoMinVolt      = 36,   //!<
  LastReasonStopOverloadNoMaxVolt       = 37,   //!<
  LastReasonStopUnderloadNoMaxVolt      = 38,   //!<
  LastReasonStopImbalanceVolt           = 39,   //!<
  LastReasonStopMinAnalog1              = 41,   //!<
  LastReasonStopMinAnalog2              = 42,   //!<
  LastReasonStopOverloadBadVolt         = 50,   //!<
  LastReasonStopUnderloadBadVolt        = 51,   //!<
  LastReasonStopImbalanceCurrentBadVolt = 52,   //!<
  LastReasonStopImbalanceVoltageBadVolt = 53,   //!<
  LastReasonStopDigital1                = 59,   //!<
  LastReasonStopDigital2                = 60,   //!<
  LastReasonStopDigital3                = 61,   //!<
  LastReasonStopDigital4                = 62,   //!<
  // Дополнительные причины останова
  LastReasonStopMinAnalog3              = 100,  //!<
  LastReasonStopMinAnalog4              = 101,  //!<
  LastReasonStopMaxAnalog3              = 102,  //!<
  LastReasonStopMaxAnalog4              = 103,  //!<
  LastReasonStopCurrentMotor            = 104,  //!< "Предел тока двигателя"
  LastReasonStopOutOfSyncMotor          = 105,  //!< "Рассинхронизация ПВЭД"
  LastReasonStopHardwareVsd             = 106,  //!< "Аппаратные защиты ЧРП"
};

/*!
 * \brief Флаги статусных регистров инвертора
 */
enum enVsdFlags {
  VSD_STATUS_STARTED          = 0,          //!< Запуск ПЧ
  VSD_STATUS_WAIT_RECT_START,               //!< Ожидаем запуска выпрямителя
  VSD_STATUS_STOPPED_REGISTER,              //!< Инвертор остановлен по изменению важного параметра
  VSD_STATUS_STOPPED_EXTERNAL,              //!< Инвертор остановлен по команде извне
  VSD_STATUS_WAIT_RECT_STOP,                //!< Ожидание остнова выпрямителя
  VSD_STATUS_FAULT_STOPPED,                 //!< Остановлен по причине FAULT
  VSD_STATUS_RIGHT_DIRECTION,               //!< Правое направление вращения
  VSD_STATUS_I_LIMIT,                       //!< Токоограничение
  VSD_STATUS_ULOW,                          //!< Недостаточно напряжения
  VSD_STATUS_STOPPED_ALARM,                 //!< Остановлен аварийно
  VSD_STATUS_UD_LOW_FAULT,                  //!< Остановлен по снижению напряжения на шин
  VSD_STATUS_UD_HIGH_FAULT,                 //!< Остановлен по превышению напряжения на шине
  VSD_STATUS_TO_STOP_MODE,                  //!< Режим плавной остановки двигателя
  VSD_STATUS_UIN_ASYM,                      //!< Остановлен по несимметрии входного напряжения
  VSD_STATUS_URECT_SHORTCIRCUIT,            //!< Остановлен по КЗ от выпрямителя
  VSD_STATUS_RESERVED,                      //!< Резерв
  VSD_STATUS_FC_I2T_ERR       = 16,         //!< Сработала токовая тепловая защита инвертора
  VSD_STATUS_AST_ERR,                       //!< Система автонастройки не смогла определить параметры линии
  VSD_STATUS_I_LIMIT_FAST,                  //!< Превышение порога мгновенного токоограничения
  VSD_STATUS_CURRENT_OPT,                   //!< Включена оптимизация по току
  VSD_STATUS_POWER_OPT,                     //!< Включена оптимизация по выходной мощности
  VSD_STATUS_OPT_DONE,                      //!< Оптимизация состоялась
  VSD_STATUS_M_TYPE0,                       //!< Бит типа двигателя 0
  VSD_STATUS_M_TYPE1,                       //!< Бит типа двигателя 1
  VSD_STATUS_DISCHARGE_ON,                  //!< Задействован режим разряда шины
  VSD_STATUS_DISCAHRGE,                     //!< Режим разряда шины работает
  VSD_STATUS_DISCHARGE_ERR,                 //!< Ошибка режима разряда шины
  VSD_STATUS_VC_ERR,                        //!< Ошибка векторного режима
  VSD_STATUS_I_FAST_ERR,                    //!< Быстрая токовая защита двигателя
  VSD_STATUS_M_I2T_ERR,                     //!< Токовая защита двигателя (перегруз)
  VSD_STATUS_OWERPWM1,                      //!< OverPWM тип 1
  VSD_STATUS_OWERPWM2,                      //!< OverPWM тип 2
  VSD_STATUS_3               = 32
};

/*!
 * \brief Список частотных приводов поддерживаемых в ПО
 */
enum enVsdType {
  VSD_TYPE_NULL = 0,                        //!< Неизвестный или отсутствует
  VSD_TYPE_NOVOMET,                         //!< ЧРП Новомет
  VSD_TYPE_DANFOSS,                         //!< ЧРП Danfoss
  VSD_TYPE_ETALON                           //!< ЧРП Эталон
};

#define NUM_PI 3.14159265358979323846

#endif /* DEFINE_H_ */

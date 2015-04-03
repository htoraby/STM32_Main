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

enum enOperation
{
  OPERATION_ERROR			  = 0,
  OPERATION_READ				= 1,
  OPERATION_WRITE				= 2,
  OPERATION_LIMITED			= 3,
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
  NUMERIC_NUMBER  = 1,
  NUMERIC_LAST
};

enum enPhysicTime {
  TIME_SS     = 1,
  TIME_LAST
};

enum enPhysicPercent {
  PERCENT_PERCENT = 1,
  PERCENT_LAST
};

enum enPhysicLength {
  LENGTH_M = 1,
  LENGTH_MILE  = 2,
  LENGTH_FOOT  = 3,
  LENGTH_LAST
};

enum enPhysicDensity {
  DENSITY_KG_M3  = 1,
  DENSITY_LAST
};

enum enPhysicSpace {
  SPACE_M2  = 1,
  SPACE_LAST
};

enum enPhysicFrequency {
  FREQUENCY_HZ  = 1,
  FREQUENCY_KHZ,
  FREQUENCY_LAST
};

enum enPhysicVoltage {
  VOLTAGE_V = 1,
  VOLTAGE_KV,
  VOLTAGE_LAST
};

enum enPhysicCurrent {
  CURRENT_A = 1,
  CURRENT_MA,
  CURRENT_LAST
};

enum enPhysicActivPower {
  ACTIV_POWER_W = 1,
  ACTIV_POWER_KW,
  ACTIV_POWER_LAST
};

enum enPhysicFullPower {
  FULL_POWER_VA = 1,
  FULL_POWER_KVA,
  FULL_POWER_LAST
};

enum enPhysicReactivPower {
  REACTIV_POWER_VAR = 1,
  REACTIV_POWER_KVAR,
  REACTIV_POWER_LAST,
};

enum enPhysicAngle {
  ANGLE_DEGREES = 1,
  ANGLE_LAST
};

enum enPhysicRpm {
  RPM_RPM = 1,
  RPM_LAST
};

enum enPhysicTemp {
  TEMP_HZ_SS = 1,
  TEMP_LAST
};

enum enPhysicResistence {
  RESISTANCE_KOM  = 1,
  RESISTANCE_OM,
  RESISTANCE_MOM,
  RESISTANCE_LAST
};

enum enPhysicResisCable {
  RESIST_CABLE_KOM_M  = 1,
  RESIST_CABLE_LAST
};

enum enPhysicPressure {
  PRESSURE_MPA  = 1,
  PRESSURE_ATM = 2,
  PRESSURE_AT  = 3,
  PRESSURE_BAR = 4,
  PRESSURE_PSI = 5,
  PRESSURE_LAST
};

enum enPhysicTemperature {
  TEMPERATURE_C = 1,
  TEMPERATURE_F = 2,
  TEMPERATURE_K = 3,
  TEMPERATURE_LAST
};

enum enPhysicAcceleration {
  ACCELERATION_G = 1,
  ACCELERATION_MSS2,
  ACCELERATION_LAST
};

enum enPhysicSpeed {
  SPEED_M_SS = 1,
  SPEED_LAST
};

enum enPhysicViscosity {
  VISCOSITY_MPA_SS,
  VISCOSITY_LAST
};

enum enPhysicFlow {
  FLOW_M3_DD = 1,
  FLOW_LAST
};

enum emPhysicPower {
  POWER_W  = 1,
  POWER_KW,
  POWER_LAST
};

enum enPhysicActivEnergy {
  ACTIVE_ENERGY_W_HH = 1,
  ACTIVE_ENERGY_KW_HH,
  ACTIVE_ENERGY_LAST,
};

enum enPhysicReactivEnergy {
  REACTIVE_ENERGY_VAR_HH = 1,
  REACTIVE_ENERGY_KVAR_HH,
  REACTIVE_ENERGY_LAST,
};

enum enPhysicFullEnergy {
  FULL_ENERGY_VAHH = 1,
  FULL_ENERGY_LAST
};

enum enPhysicRate {
  RATE_NM   = 1,
  RATE_LAST
};

enum enPhysicIndunctance {
  INDUNCTANCE_MH = 1,
  INDUNCTANCE_LAST
};

/*!
 * \brief Перечисление типов двигателя
 */
enum enMotorType {
  VSD_MOTOR_TYPE_ASYNC = 1,                 /// Асинхронный двигатель
  VSD_MOTOR_TYPE_VENT                       /// Вентильный двигатель
};

/*!
 * \brief Перечисление направлений вращений двигателя
 */
enum enRotation {
  VSD_ROTATION_DIRECT = 1,                   /// Прямое(правое) направление вращения
  VSD_ROTATION_REVERSE                       /// Обратное(левое) направление вращения
};

/*!
 * \brief Перечисление типов управления двигателем
 */
enum enMotorControl {
  VSD_MOTOR_CONTROL_UF = 1,                 /// Метод управления U/f
  VSD_MOTOR_CONTROL_VECT                    /// Векторное управление
};

/*!
 * \brief The enUfType enum
 * Перечисление типов зависимости U/f
 */
enum enUfType {
  /// По точкам
  VSD_UF_TYPE_POINT = 1,
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
  NOMINAL_VOLTAGE_380 = 1,
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
  NOMINAL_FREQUENCY_50 = 1,
  /// 60Гц
  NOMINAL_FREQUENCY_60
};

/*!
 * \brief The enCableCross enum
 * Перечисление сечений кабеля
 */
enum enCableCross {
  /// 16
  CABLE_CROSS_16 = 1,
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
  PHASE_ABC = 1,
  PHASE_CBA
};

/*!
 * \brief The enParamProt enum
 * Кретерий защиты
 */
enum enParamProt {
  PARAM_PROT_POWER = 1,
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
  CCS_WORKING_MODE_MANUAL = 1,
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
 * \brief Список причин запуска
 */
enum LastReasonRun {
  LastReasonRunNone                     = 0,
  LastReasonRunOperator                 = 1,
  LastReasonRunAuto                     = 2,
  LastReasonRunRemote                   = 3,
  LastReasonRunProgram                  = 4,
  LastReasonRunApvUnderloadMotor        = 5,   //!< "АПВ по защите: Недогруз двигателя"
  LastReasonRunApvOverloadMotor         = 6,   //!< "АПВ по защите: Перегруз двигателя"
  LastReasonRunApvImbalanceCurMotor     = 7,   //!< "АПВ по защите: Дисбаланс токов ПЭД"
  LastReasonRunApvUnderVoltIn           = 8,   //!< "АПВ по защите: Снижение питания сети"
  LastReasonRunApvOverVoltIn            = 9,   //!< "АПВ по защите: Превышение питания сети"
  LastReasonRunApvImbalanceVoltIn       = 10,  //!< "АПВ по защите: Дисбаланс напряжения"
  LastReasonRunApvTurborotation         = 11,
  LastReasonRunApvPhaseSequence         = 12,
  LastReasonRunApvLockDoor              = 13,
  LastReasonRunApvPressureIntake        = 14,  //!< "АПВ по защите: Давление на приёме насоса"
  LastReasonRunApvTemperatureMotor      = 15,  //!< "АПВ по защите: Перегрев двигателя"
  LastReasonRunApvMaxVibration          = 16,
  LastReasonRunApvUnderPressure1        = 17,
  LastReasonRunApvMinAnalog1            = 18,
  LastReasonRunApvMaxAnalog1            = 19,
  LastReasonRunApvMinAnalog2            = 20,
  LastReasonRunApvMaxAnalog2            = 21,
  LastReasonRunApvFreqPower             = 22,
  LastReasonRunApvPower                 = 23,
  // Дополнительные виды запуска
  LastReasonRunMinAnalog3               = 100,
  LastReasonRunMinAnalog4               = 101,
  LastReasonRunMaxAnalog3               = 102,
  LastReasonRunMaxAnalog4               = 103,
  LastReasonRunApvCurrentMotor          = 104, //!< "АПВ по защите: Предел тока двигателя"
  LastReasonRunApvOutOfSyncMotor        = 105, //!< "АПВ по защите: Рассинхронизация ПВЭД"
  LastReasonRunApvResistIsolation       = 106, //!< "АПВ по защите: Сопротивление изоляции"
};

/*!
 * \brief Список причин останова
 */
enum LastReasonStop {
  LastReasonStopNone                    = 0,
  LastReasonStopOperator                = 1,
  LastReasonStopResistance              = 2,
  LastReasonStopUnderload               = 3,
  LastReasonStopOverload                = 4,
  LastReasonStopUndervoltage            = 5,
  LastReasonStopOvervoltage             = 6,
  LastReasonStopOpenDoor                = 9,
  LastReasonStopManometr                = 12,
  LastReasonStopUnderPressure           = 13,
  LastReasonStopOverheat                = 14,
  LastReasonStopMaxVibration            = 16,
  LastReasonStopImbalanceCurrent        = 20,
  LastReasonStopImbalanceVoltage        = 21,
  LastReasonStopProgram                 = 24,
  LastReasonStopMaxAnalog1              = 26,
  LastReasonStopMaxAnalog2              = 27,
  LastReasonStopRemote                  = 28,
  LastReasonStopNoVoltage               = 30,
  LastReasonStopOverloadNoMinVolt       = 35,
  LastReasonStopUnderloadNoMinVolt      = 36,
  LastReasonStopOverloadNoMaxVolt       = 37,
  LastReasonStopUnderloadNoMaxVolt      = 38,
  LastReasonStopImbalanceVolt           = 39,
  LastReasonStopMinAnalog1              = 41,
  LastReasonStopMinAnalog2              = 42,
  LastReasonStopOverloadBadVolt         = 50,
  LastReasonStopUnderloadBadVolt        = 51,
  LastReasonStopImbalanceCurrentBadVolt = 52,
  LastReasonStopImbalanceVoltageBadVolt = 53,
  LastReasonStopDigital1                = 59,
  LastReasonStopDigital2                = 60,
  LastReasonStopDigital3                = 61,
  LastReasonStopDigital4                = 62,
  // Дополнительные первопричины останова
  LastReasonStopMinAnalog3              = 100,
  LastReasonStopMinAnalog4              = 101,
  LastReasonStopMaxAnalog3              = 102,
  LastReasonStopMaxAnalog4              = 103,
};

#endif /* DEFINE_H_ */

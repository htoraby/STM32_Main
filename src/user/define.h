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
  unsigned char      tdChar[4];
  signed short int   tdInt16[2];
  signed long int    tdInt32;
  unsigned short int tdUint16[2];
  unsigned long int  tdUint32;
  float              tdFloat;
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
  /// Просто число
  PHYSIC_NUMERIC,
  /// Время
  PHYSIC_TIME,
  /// Проценты
  PHYSIC_PERCENT,
  /// Длина
  PHYSIC_LENGHT,
  /// Плотность
  PHYSIC_DENSITY,
  /// Площадь
  PHYSIC_SPACE,
  /// Частота
  PHYSIC_FREQUENCY,
  /// Напряжение
  PHYSIC_VOLTAGE,
  /// Ток
  PHYSIC_CURRENT,
  /// Активная мощность
  PHYSIC_ACTIVE_POWER,
  /// Полная мощность
  PHYSIC_FULL_POWER,
  /// Реактивная мощность
  PHYSIC_REACTIVE_POWER,
  /// Угол
  PHYSIC_ANGLE,
  /// Скорость вращения
  PHYSIC_RPM,
  /// Темп (разгона или замедления)
  PHYSIC_TEMP,
  /// Сопротивление изоляции
  PHYSIC_RESISTANCE,
  /// Сопротивление кабельной линии
  PHYSIC_RESIST_CABLE_LINE,
  /// Давление
  PHYSIC_PRESSURE,
  /// Температура
  PHYSIC_TEMPERATURE,
  /// Ускорение
  PHYSIC_ACCELERATION,
  /// Cкорость
  PHYSIC_SPEED,
  /// Вязкость
  PHYSIC_VISCOSITY,
  /// Расход
  PHYSIC_FLOW,
  /// Мощность
  PHYSIC_POWER,
  /// Активная энергия
  PHYSIC_ACTIVE_ENERGY,
  /// Реактивная энергия
  PHYSIC_REACTIVE_ENERGY,
  /// Полная энергия
  PHYSIC_FULL_ENERGY,
  /// Номинальный момент
  PHYSIC_RATE,
  /// Индуктивность Гн
  PHYSIC_INDUNCTANCE,

  PHYSIC_LAST
};

enum enPhysicTime {
  TIME_SECOND  = 1,
  TIME_MINUTE  = 2,
  TIME_MILISEC
};

enum enPhysicLength {
  LENGTH_METER = 1,
  LENGTH_MILE  = 2,
  LENGTH_FOOT  = 3,
  LENGTH_LAST
};

enum enPhysicPressure {
  PRESSURE_PA  = 1,
  PRESSURE_ATM = 2,
  PRESSURE_AT  = 3,
  PRESSURE_BAR = 4,
  PRESSURE_PSI = 5,
  PRESSURE_LAST
};

enum enPhysicVoltage {
  VOLTAGE_VOLT = 1,
  VOLTAGE_LAST
};

enum enPhysicCurrent {
  CURRENT_AMP = 1,
  CURRENT_LAST
};

enum enPhysicTemperature {
  TEMPERATURE_C = 1,
  TEMPERATURE_F = 2,
  TEMPERATURE_K = 3,
  TEMPERATURE_LAST
};

enum enPhysicSpeed {
  SPEED_M_S = 1,
  SPEED_LAST
};

enum enPhysicActivPower {
  ACTIV_POWER_VATT = 1,
  ACTIV_POWER_LAST
};

enum enPhysicFullPower {
  FULL_POWER_VA = 1,
  FULL_POWER_LAST
};

enum enPhysicFrequency {
  FREQUENCY_HZ  = 1,
  FREQUENCY_LAST
};

enum enPhysicRpm {
  RPM_RPM = 1,
  RPM_LAST
};

enum enPhysicRate {
  RATE_NM   = 1,
  RATE_LAST
};

enum enPhysicReesistence {
  RESISTANCE_OM  = 1,
  RESISTANCE_KOM,
  RESISTANCE_MOM,
  RESISTANCE_LAST
};

enum enPhysicIndunctance {
  INDUNCTANCE_H  = 1,
  INDUNCTANCE_mH = 2,
  INDUNCTANCE_LAST
};

enum enPhysicAcceleration {
  ACCELERATION_G = 1,
  ACCELERATION_MC2,
  ACCELERATION_LAST
};

enum enPhysicFlow {
  FLOW_M3DAY = 1,
  FLOW_LAST
};

enum enPhysicAngle {
  ANGLE_DEGREES = 1,
  ANGLE_LAST
};

enum enPhysicTemp {
  TEMP_HZ_SECOND = 1,
  TEMP_LAST
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
 * \brief The enOnOff enum
 * Перечисление включено / отключено
 */
enum enOnOff {
  /// Выключено
  OFF = 1,
  /// Включено
  ON
};

/*!
 * \brief The enProt enum
 * Перечисление действия защиты
 */
enum enProt {
  /// Выключено
  PROT_OFF = 1,
  /// АПВ
  PROT_RESTART,
  /// Блокировка
  PROT_BLOCK
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
  VALIDITY_GOOD       = 0,
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
  CCS_CONDITION_BLOCK,
  CCS_CONDITION_STOP,                       /// Останов
  CCS_CONDITION_STOPPING,                   /// Торможение
  CCS_CONDITION_WAIT_APV,
  CCS_CONDITION_DELAY,
  CCS_CONDITION_RUNNING,                    /// Запуск
  CCS_CONDITION_RUN                         /// Работа
};

enum enCcsWorkingMode {
  CCS_WORKING_MODE_STOP = 0,
  CCS_WORKING_MODE_MANUAL,
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
#endif /* DEFINE_H_ */

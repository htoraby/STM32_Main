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
  PHYSIC_ERROR              = 0,
  /// Просто число
  PHYSIC_NUMERIC            = 1,
  /// Время
  PHYSIC_TIME               = 2,
  /// Проценты
  PHYSIC_PERCENT            = 3,
  /// Длина
  PHYSIC_LENGHT             = 4,
  /// Плотность
  PHYSIC_DENSITY            = 5,
  /// Площадь
  PHYSIC_SPACE              = 6,
  /// Частота
  PHYSIC_FREQUENCY          = 7,
  /// Напряжение
  PHYSIC_VOLTAGE            = 8,
  /// Ток
  PHYSIC_CURRENT            = 9,
  /// Активная мощность
  PHYSIC_ACTIVE_POWER       = 10,
  /// Полная мощность
  PHYSIC_FULL_POWER         = 11,
  /// Реактивная мощность
  PHYSIC_REACTIVE_POWER     = 12,
  /// Угол
  PHYSIC_ANGLE              = 13,
  /// Скорость вращения
  PHYSIC_RPM                = 14,
  /// Темп (разгона или замедления)
  PHYSIC_TEMP               = 15,
  /// Сопротивление изоляции
  PHYSIC_RESISTANCE         = 16,
  /// Сопротивление кабельной линии
  PHYSIC_RESIST_CABLE_LINE  = 16,
  /// Давление
  PHYSIC_PRESSURE           = 17,
  /// Температура
  PHYSIC_TEMPERATURE        = 18,
  /// Ускорение
  PHYSIC_ACCELERATION       = 19,
  /// Cкорость
  PHYSIC_SPEED             	= 20,
  /// Вязкость
  PHYSIC_VISCOSITY	        = 21,
  /// Расход
  PHYSIC_FLOW            		= 22,
  /// Мощность
  PHYSIC_POWER              = 23,
  /// Активная энергия
  PHYSIC_ACTIVE_ENERGY      = 24,
  /// Реактивная энергия
  PHYSIC_REACTIVE_ENERGY    = 25,
  /// Полная энергия
  PHYSIC_FULL_ENERGY        = 26,
  /// Номинальный момент
  PHYSIC_RATE               = 27,
  /// Индуктивность Гн
  PHYSIC_INDUNCTANCE        = 28,
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
  PRESSURE_MPA = 1,
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

enum enPhysicPower {
  POWER_VATT = 1,
  POWER_KVATT,
};

enum enPhysicFrequency {
  FREQUENCY_HZ  = 1,
  FREQUENCY_KHZ = 2,
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
  FLOW_LAST,
};

enum enPhysicAngle {
  ANGLE_DEGREES = 1,
  ANGLE_LAST
};

/*!
 * \brief The enTypeMotor enum
 * Перечисление типов двигателя
 */
enum enTypeMotor {
  /// Ассинхронный
  VSD_TYPE_MOTOR_ASYNC = 1,
  /// Вентильный 1000 оборотов
  VSD_TYPE_MOTOR_VENT_1000,
  /// Вентильный 3000 оборотов
  VSD_TYPE_MOTOR_VENT_3000,
  /// Вентильный 6000 оборотов
  VSD_TYPE_MOTOR_VENT_6000
};

/*!
 * \brief The enControlMotor enum
 * Перечисление типов управления двигателем
 */
enum enControlMotor {
  /// Метод управления U/f
  VSD_CONTROL_MOTOR_UF = 1,
  /// Векторное управление
  VSD_CONTROL_MOTOR_VECT
};

/*!
 * \brief The enRotation enum
 * Перечисление направлений вращений двигателя
 */
enum enRotation {
  /// Правое направление вращения
  VSD_ROTATION_RIGHT = 1,
  /// Левое направление вращения
  VSD_ROTATION_LEFT
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
  CCS_CONDITION_STOP,     //!< Останов
  CCS_CONDITION_BLOCK,    //!< Блок
  CCS_CONDITION_WAIT_APV, //!< АПВ
  CCS_CONDITION_DELAY,    //!< Ожидание
  CCS_CONDITION_RUN       //!< Работа
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
#endif /* DEFINE_H_ */

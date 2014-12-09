/*
 * define.h
 *
 *  Created on: 14.06.2012
 *      Author: trofimenko
 */

#ifndef DEFINE_H_
#define DEFINE_H_

union DataType
{
  unsigned char      DtChar[4];
  signed short int   DtInt16[2];
  signed long int    DtInt32;
  unsigned short int DtUint16[2];
  unsigned long int  DtUint32;
  float              DtFloat;
};

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

union unTypeData {
  //// Символ
  unsigned char Chars[4];
  //// Знаковый INT16
  signed short int Int16[2];
  //// Знаковый INT32
  signed long int Int32;
  //// Беззнаковый INT16
  unsigned short int Uint16[2];
  //// Беззнаковый INT32
  unsigned long int Uint32;
  //// С плавающей точкой
  float Float;
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

enum enPhysic
{
  PHYSIC_ERROR              = 0,
  PHYSIC_NUMERIC            = 1,        /// Перечислимый тип
  PHYSIC_TIME               = 2,        /// Время
  PHYSIC_PERCENT            = 3,        /// Проценты
  PHYSIC_LENGHT             = 4,        /// Длина
  PHYSIC_DENSITY            = 5,        /// Плотность
  PHYSIC_SPACE              = 6,        /// Площадь
  PHYSIC_FREQUENCY          = 7,        /// Частота
  PHYSIC_VOLTAGE            = 8,        /// Напряжение
  PHYSIC_CURRENT            = 9,        /// Ток
  PHYSIC_ACTIVE_POWER       = 10,       /// Активная мощность
  PHYSIC_FULL_POWER         = 11,       /// Полная мощность
  PHYSIC_REACTIVE_POWER     = 12,       /// Реактивная мощность
  PHYSIC_ANGLE              = 13,       /// Угол
  PHYSIC_RPM                = 14,       /// Скорость вращения
  PHYSIC_TEMP               = 15,       /// Темп (разгона или замедления)
  PHYSIC_RESISTANCE         = 16,       /// Сопротивление изоляции
  PHYSIC_RESIST_CABLE_LINE  = 16,       /// Сопротивление кабельной линии
  PHYSIC_PRESSURE           = 17,       /// Давление
  PHYSIC_TEMPERATURE        = 18,       /// Температура
  PHYSIC_ACCELERATION       = 19,       /// Ускорение
  PHYSIC_SPEED             	= 20,       /// Cкорость
  PHYSIC_VISCOSITY	        = 21,       /// Вязкость
  PHYSIC_FLOW            		= 22,       /// Расход
  PHYSIC_POWER              = 23,       /// Мощность
  PHYSIC_ACTIVE_ENERGY      = 24,       /// Активная энергия
  PHYSIC_REACTIVE_ENERGY    = 25,       /// Реактивная энергия
  PHYSIC_FULL_ENERGY        = 26,       /// Полная энергия
  PHYSIC_RATE               = 27,       /// Номинальный момент
  PHYSIC_INDUNCTANCE        = 28,       /// Индуктивность Гн
	PHYSIC_LAST
};

enum enValidity
{
  VALIDITY_ERROR      = 0,
  VALIDITY_GOOD       = 1,
  VALIDITY_MIN        = 2,
  VALIDITY_MAX        = 3,
	VALIDITY_LAST
};

enum enUpdate
{
  UPDATE_ERROR				= 0,
  UPDATE_LAST
};

enum enCondition
{
	CS_CONDITION_STOP			= 0,
	CS_CONDITION_WAIT,
	CS_CONDITION_DELAY,
	CS_CONDITION_BLOCK,
	CS_CONDITION_WORK
};

enum enID
{
	CCS_BEGIN        = 0,

  /// ИЗМЕРЯЕМЫЕ, РАСЧЁТНЫЕ И КОНТРОЛИРУЕМЫЕ ПАРАМЕТРЫ
  CCS_RESISTANCE_ISOLATION,									/// Сопротивление изоляции, берем с ТМС или УКИ

  CCS_ACCESS_LEVEL,											/// Уровень доступа к контроллеру
	CCS_CONDITION,
  CCS_DEVICE_STOP,											/// Устройство остановлено
  CCS_LOAD_MOTOR,												/// Текущая загрузка двигателя
  /// ЗАЩИТА ОТ СНИЖЕНИЯ ДАВЛЕНИЯ НА ПРИЁМЕ НАСОСА
  CCS_PROT_PRESSURE_INTAKE_MODE,								/// Защита Вкл/Выкл
  CCS_PROT_PRESSURE_INTAKE_ACTION,							/// Защита Откл/АПВ/Блок
  CCS_PROT_PRESSURE_INTAKE_TIMER_ACTIVATION,					/// Задержка активации
  CCS_PROT_PRESSURE_INTAKE_TIMER_DELAY,						/// Задержка срабатывания
  CCS_PROT_PRESSURE_INTAKE_TIMER_RESTART,						/// Задержка АПВ
  CCS_PROT_PRESSURE_INTAKE_CONTROL_PARAM,						/// Контролируемый параметр
  CCS_PROT_PRESSURE_INTAKE_SETPOINT,							/// Значение уставки защиты
  CCS_PROT_PRESSURE_INTAKE_COUNTER_RESTART,					/// Счётчик запусков
  CCS_PROT_PRESSURE_INTAKE_TIMER_COUNTER_RESTART,				/// Период сброса запусков
  CCS_PROT_PRESSURE_INTAKE_BAN,								/// Наличие запрещающего параметра
  CCS_PROT_PRESSURE_INTAKE_BAN_SETPOINT,						/// Уставка запрещающего параметра
  CCS_PROT_PRESSURE_INTAKE_STATE,								/// Текущее состояние автомата защит
  CCS_PROT_PRESSURE_INTAKE_TIMER,								/// Текущее состояние таймера
  CCS_PROT_PRESSURE_INTAKE_COUNTER,							/// Текущее состояние счётчиков срабатывания
  CCS_PROT_PRESSURE_INTAKE_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_PRESSURE_INTAKE_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ ПЕРЕГРЕВА ДВИГАТЕЛЯ
	CCS_PROT_MOTOR_OVERHEAD_MODE,
	CCS_PROT_MOTOR_OVERHEAD_ACTION,
	CCS_PROT_MOTOR_OVERHEAD_TIMER_ACTIVATION,
	CCS_PROT_MOTOR_OVERHEAD_TIMER_DELAY,
	CCS_PROT_MOTOR_OVERHEAD_TIMER_RESTART,
	CCS_PROT_MOTOR_OVERHEAD_CONTROL_PARAM,
	CCS_PROT_MOTOR_OVERHEAD_SETPOINT,
	CCS_PROT_MOTOR_OVERHEAD_COUNTER_RESTART,
	CCS_PROT_MOTOR_OVERHEAD_TIMER_COUNTER_RESTART,
	CCS_PROT_MOTOR_OVERHEAD_BAN,
	CCS_PROT_MOTOR_OVERHEAD_BAN_SETPOINT,
	CCS_PROT_MOTOR_OVERHEAD_STATE,
	CCS_PROT_MOTOR_OVERHEAD_TIMER,
	CCS_PROT_MOTOR_OVERHEAD_COUNTER,
  CCS_PROT_MOTOR_OVERHEAD_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_MOTOR_OVERHEAD_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ ВИБРАЦИИ ДВИГАТЕЛЯ
	CCS_PROT_MOTOR_VIBRATION_MODE,
	CCS_PROT_MOTOR_VIBRATION_ACTION,
	CCS_PROT_MOTOR_VIBRATION_TIMER_ACTIVATION,
	CCS_PROT_MOTOR_VIBRATION_TIMER_DELAY,
	CCS_PROT_MOTOR_VIBRATION_TIMER_RESTART,
	CCS_PROT_MOTOR_VIBRATION_CONTROL_PARAM,
	CCS_PROT_MOTOR_VIBRATION_SETPOINT,
	CCS_PROT_MOTOR_VIBRATION_COUNTER_RESTART,
	CCS_PROT_MOTOR_VIBRATION_TIMER_COUNTER_RESTART,
	CCS_PROT_MOTOR_VIBRATION_BAN,
	CCS_PROT_MOTOR_VIBRATION_BAN_SETPOINT,
	CCS_PROT_MOTOR_VIBRATION_STATE,
	CCS_PROT_MOTOR_VIBRATION_TIMER,
	CCS_PROT_MOTOR_VIBRATION_COUNTER,
  CCS_PROT_MOTOR_VIBRATION_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_MOTOR_VIBRATION_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты_MODE,
	CCS_PROT_MOTOR_VIBRATION_VALUE,
  /// ЗАЩИТА ОТ ТУРБИННОГО ВРАЩЕНИЯ
	CCS_PROT_TURBO_ROTATION_MODE,
	CCS_PROT_TURBO_ROTATION_ACTION,
	CCS_PROT_TURBO_ROTATION_TIMER_ACTIVATION,
	CCS_PROT_TURBO_ROTATION_TIMER_DELAY,
	CCS_PROT_TURBO_ROTATION_TIMER_RESTART,
	CCS_PROT_TURBO_ROTATION_CONTROL_PARAM,
	CCS_PROT_TURBO_ROTATION_SETPOINT,
	CCS_PROT_TURBO_ROTATION_COUNTER_RESTART,
	CCS_PROT_TURBO_ROTATION_TIMER_COUNTER_RESTART,
	CCS_PROT_TURBO_ROTATION_BAN,
	CCS_PROT_TURBO_ROTATION_BAN_SETPOINT,
	CCS_PROT_TURBO_ROTATION_STATE,
	CCS_PROT_TURBO_ROTATION_TIMER,
	CCS_PROT_TURBO_ROTATION_COUNTER,
  CCS_PROT_TURBO_ROTATION_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_TURBO_ROTATION_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
	CCS_PROT_TIRBO_ROTATION_VALUE,
  /// ЗАЩИТА СНИЖЕНИЯ ПИТАНИЯ СЕТИ
	CCS_PROT_LOW_VOLTAGE_MODE,
	CCS_PROT_LOW_VOLTAGE_ACTION,
	CCS_PROT_LOW_VOLTAGE_ACTIVATION,
	CCS_PROT_LOW_VOLTAGE_TIMER_DELAY,
	CCS_PROT_LOW_VOLTAGEN_TIMER_RESTART,
	CCS_PROT_LOW_VOLTAGE_CONTROL_PARAM,
	CCS_PROT_LOW_VOLTAGE_SETPOINT,
	CCS_PROT_LOW_VOLTAGE_COUNTER_RESTART,
	CCS_PROT_LOW_VOLTAGE_TIMER_COUNTER_RESTART,
	CCS_PROT_LOW_VOLTAGE_BAN,
	CCS_PROT_LOW_VOLTAGE_BAN_SETPOINT,
	CCS_PROT_LOW_VOLTAGE_STATE,
	CCS_PROT_LOW_VOLTAGE_TIMER,
	CCS_PROT_LOW_VOLTAGE_COUNTER,
  CCS_PROT_LOW_VOLTAGE_GENERAL_COUNTER_RESTART,				/// Общий счётчик
  CCS_PROT_LOW_VOLTAGE_GENERAL_COUNTER_STOP,					/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА СНИЖЕНИЯ СОПРОТИВЛЕНИЯ ИЗОЛЯЦИИ
	CCS_PROT_RESISTANCE_ISOLATION_MODE,
	CCS_PROT_RESISTANCE_ISOLATION_ACTION,
	CCS_PROT_RESISTANCE_ISOLATION_ACTIVATION,
	CCS_PROT_RESISTANCE_ISOLATION_TIMER_DELAY,
	CCS_PROT_RESISTANCE_ISOLATION_TIMER_RESTART,
	CCS_PROT_RESISTANCE_ISOLATION_CONTROL_PARAM,
	CCS_PROT_RESISTANCE_ISOLATION_SETPOINT,
	CCS_PROT_RESISTANCE_ISOLATION_COUNTER_RESTART,
	CCS_PROT_RESISTANCE_ISOLATION_TIMER_COUNTER_RESTART,
	CCS_PROT_RESISTANCE_ISOLATION_BAN,
	CCS_PROT_RESISTANCE_ISOLATION_BAN_SETPOINT,
	CCS_PROT_RESISTANCE_ISOLATION_STATE,
	CCS_PROT_RESISTANCE_ISOLATION_TIMER,
	CCS_PROT_RESISTANCE_ISOLATION_COUNTER,
  CCS_PROT_RESISTANCE_ISOLATION_GENERAL_COUNTER_RESTART,		/// Общий счётчик
  CCS_PROT_RESISTANCE_ISOLATION_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ПРЕВЫШЕНИЯ ПИТАНИЯ СЕТИ
	CCS_PROT_HIGH_VOLTAGE_MODE,
	CCS_PROT_HIGH_VOLTAGE_ACTION,
	CCS_PROT_HIGH_VOLTAGE_ACTIVATION,
	CCS_PROT_HIGH_VOLTAGE_TIMER_DELAY,
	CCS_PROT_HIGH_VOLTAGE_TIMER_RESTART,
	CCS_PROT_HIGH_VOLTAGE_CONTROL_PARAM,
	CCS_PROT_HIGH_VOLTAGE_SETPOINT,
	CCS_PROT_HIGH_VOLTAGE_COUNTER_RESTART,
	CCS_PROT_HIGH_VOLTAGE_TIMER_COUNTER_RESTART,
	CCS_PROT_HIGH_VOLTAGE_BAN,
	CCS_PROT_HIGH_VOLTAGE_BAN_SETPOINT,
	CCS_PROT_HIGH_VOLTAGE_STATE,
	CCS_PROT_HIGH_VOLTAGE_TIMER,
	CCS_PROT_HIGH_VOLTAGE_COUNTER,
  CCS_PROT_HIGH_VOLTAGE_GENERAL_COUNTER_RESTART,				/// Общий счётчик
  CCS_PROT_HIGH_VOLTAGE_GENERAL_COUNTER_STOP,					/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ПРЕВЫШЕНИЯ ЧИСЛА ПУСКОВ
	CCS_PROT_COUNTER_RESTART_MODE,
	CCS_PROT_COUNTER_RESTART_ACTION,
	CCS_PROT_COUNTER_RESTART_ACTIVATION,
	CCS_PROT_COUNTER_RESTART_TIMER_DELAY,
	CCS_PROT_COUNTER_RESTART_TIMER_RESTART,
	CCS_PROT_COUNTER_RESTARTE_CONTROL_PARAM,
	CCS_PROT_COUNTER_RESTART_SETPOINT,
	CCS_PROT_COUNTER_RESTART_COUNTER_RESTART,
	CCS_PROT_COUNTER_RESTART_TIMER_COUNTER_RESTART,
	CCS_PROT_COUNTER_RESTART_BAN,
	CCS_PROT_COUNTER_RESTART_BAN_SETPOINT,
	CCS_PROT_COUNTER_RESTART_STATE,
	CCS_PROT_COUNTER_RESTART_TIMER,
	CCS_PROT_COUNTER_RESTART_COUNTER,
  CCS_PROT_COUNTER_RESTART_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_COUNTER_RESTART_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ ПЕРЕГРУЗА
	CCS_PROT_OVERLOAD_MODE,
	CCS_PROT_OVERLOAD_ACTION,
	CCS_PROT_OVERLOAD_ACTIVATION,
	CCS_PROT_OVERLOAD_TIMER_DELAY,
	CCS_PROT_OVERLOAD_TIMER_RESTART,
	CCS_PROT_OVERLOAD_CONTROL_PARAM,
	CCS_PROT_OVERLOAD_SETPOINT,
	CCS_PROT_OVERLOAD_COUNTER_RESTART,
	CCS_PROT_OVERLOAD_TIMER_COUNTER_RESTART,
	CCS_PROT_OVERLOAD_BAN,
	CCS_PROT_OVERLOAD_BAN_SETPOINT,
	CCS_PROT_OVERLOAD_STATE,
	CCS_PROT_OVERLOAD_TIMER,
	CCS_PROT_OVERLOAD_COUNTER,
  CCS_PROT_OVERLOAD_GENERAL_COUNTER_RESTART,					/// Общий счётчик
  CCS_PROT_OVERLOAD_GENERAL_COUNTER_STOP,						/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ НЕДОГРУЗА
	CCS_PROT_UNDERLOAD_MODE,
	CCS_PROT_UNDERLOAD_ACTION,
	CCS_PROT_UNDERLOAD_ACTIVATION,
	CCS_PROT_UNDERLOAD_TIMER_DELAY,
	CCS_PROT_UNDERLOAD_TIMER_RESTART,
	CCS_PROT_UNDERLOAD_CONTROL_PARAM,
	CCS_PROT_UNDERLOAD_SETPOINT,
	CCS_PROT_UNDERLOAD_COUNTER_RESTART,
	CCS_PROT_UNDERLOAD_TIMER_COUNTER_RESTART,
	CCS_PROT_UNDERLOAD_BAN,
	CCS_PROT_UNDERLOAD_BAN_SETPOINT,
	CCS_PROT_UNDERLOAD_STATE,
	CCS_PROT_UNDERLOAD_TIMER,
	CCS_PROT_UNDERLOAD_COUNTER,
  CCS_PROT_UNDERLOAD_GENERAL_COUNTER_RESTART,					/// Общий счётчик
  CCS_PROT_UNDERLOAD_GENERAL_COUNTER_STOP,					/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ НЕДОГРУЗА
	CCS_PROT_MAX_CURRENT_MODE,
	CCS_PROT_MAX_CURRENT_ACTION,
	CCS_PROT_MAX_CURRENT_ACTIVATION,
	CCS_PROT_MAX_CURRENT_TIMER_DELAY,
	CCS_PROT_MAX_CURRENT_TIMER_RESTART,
	CCS_PROT_MAX_CURRENT_CONTROL_PARAM,
	CCS_PROT_MAX_CURRENT_SETPOINT,
	CCS_PROT_MAX_CURRENT_COUNTER_RESTART,
	CCS_PROT_MAX_CURRENT_TIMER_COUNTER_RESTART,
	CCS_PROT_MAX_CURRENT_BAN,
	CCS_PROT_MAX_CURRENT_BAN_SETPOINT,
	CCS_PROT_MAX_CURRENT_STATE,
	CCS_PROT_MAX_CURRENT_TIMER,
	CCS_PROT_MAX_CURRENT_COUNTER,
  CCS_PROT_MAX_CURRENT_GENERAL_COUNTER_RESTART,				/// Общий счётчик
  CCS_PROT_MAX_CURRENT_GENERAL_COUNTER_STOP,					/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ДИСБАЛАНС ПИТАНИЯ СЕТИ
	CCS_PROT_DISBALANCE_VOLTAGE_MODE,
	CCS_PROT_DISBALANCE_VOLTAGE_ACTION,
	CCS_PROT_DISBALANCE_VOLTAGE_ACTIVATION,
	CCS_PROT_DISBALANCE_VOLTAGE_TIMER_DELAY,
	CCS_PROT_DISBALANCE_VOLTAGE_TIMER_RESTART,
	CCS_PROT_DISBALANCE_VOLTAGE_CONTROL_PARAM,
	CCS_PROT_DISBALANCE_VOLTAGE_SETPOINT,
	CCS_PROT_DISBALANCE_VOLTAGE_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_VOLTAGE_TIMER_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_VOLTAGE_BAN,
	CCS_PROT_DISBALANCE_VOLTAGE_BAN_SETPOINT,
	CCS_PROT_DISBALANCE_VOLTAGE_STATE,
	CCS_PROT_DISBALANCE_VOLTAGE_TIMER,
	CCS_PROT_DISBALANCE_VOLTAGE_COUNTER,
  CCS_PROT_DISBALANCE_VOLTAGE_GENERAL_COUNTER_RESTART,		/// Общий счётчик
  CCS_PROT_DISBALANCE_VOLTAGE_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
  CCS_PROT_DISBALANCE_VOLTAGE_VALUE,							/// Значение дисбаланса
  /// ЗАЩИТА ДИСБАЛАНС ТОКА СЕТИ СЕТИ
	CCS_PROT_DISBALANCE_CURRENT_MODE,
	CCS_PROT_DISBALANCE_CURRENT_ACTION,
	CCS_PROT_DISBALANCE_CURRENT_ACTIVATION,
	CCS_PROT_DISBALANCE_CURRENT_TIMER_DELAY,
	CCS_PROT_DISBALANCE_CURRENT_TIMER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_CONTROL_PARAM,
	CCS_PROT_DISBALANCE_CURRENT_SETPOINT,
	CCS_PROT_DISBALANCE_CURRENT_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_TIMER_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_BAN,
	CCS_PROT_DISBALANCE_CURRENT_BAN_SETPOINT,
	CCS_PROT_DISBALANCE_CURRENT_STATE,
	CCS_PROT_DISBALANCE_CURRENT_TIMER,
	CCS_PROT_DISBALANCE_CURRENT_COUNTER,
  CCS_PROT_DISBALANCE_CURRENT_GENERAL_COUNTER_RESTART,		/// Общий счётчик
  CCS_PROT_DISBALANCE_CURRENT_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
  CCS_PROT_DISBALANCE_CURRENT_VALUE,							/// Текущее значение дисбаланса
  /// ЗАЩИТА ДИСБАЛАНС ТОКА СЕТИ СЕТИ
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_MODE,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_ACTION,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_ACTIVATION,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_TIMER_DELAY,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_TIMER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_CONTROL_PARAM,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_SETPOINT,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_TIMER_COUNTER_RESTART,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_BAN,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_BAN_SETPOINT,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_STATE,
	CCS_PROT_DISBALANCE_CURRENT_MOTOR_COUNTER,
  CCS_PROT_DISBALANCE_CURRENT_MOTOR_GENERAL_COUNTER_RESTART,	/// Общий счётчик
  CCS_PROT_DISBALANCE_CURRENT_MOTOR_GENERAL_COUNTER_STOP,		/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ПО МИНИМАЛЬНОЙ ЧАСТОТЕ
	CCS_PROT_MINIMUM_FREQUENCY_MODE,
	CCS_PROT_MINIMUM_FREQUENCY_ACTION,
	CCS_PROT_MINIMUM_FREQUENCY_ACTIVATION,
	CCS_PROT_MINIMUM_FREQUENCY_TIMER_DELAY,
	CCS_PROT_MINIMUM_FREQUENCY_TIMER_RESTART,
	CCS_PROT_MINIMUM_FREQUENCY_CONTROL_PARAM,
	CCS_PROT_MINIMUM_FREQUENCY_MOTOR_SETPOINT,
	CCS_PROT_MINIMUM_FREQUENCY_COUNTER_RESTART,
	CCS_PROT_MINIMUM_FREQUENCY_TIMER_COUNTER_RESTART,
	CCS_PROT_MINIMUM_FREQUENCY_BAN,
	CCS_PROT_MINIMUM_FREQUENCY_BAN_SETPOINT,
	CCS_PROT_MINIMUM_FREQUENCY_STATE,
	CCS_PROT_MINIMUM_FREQUENCY_TIMER,
	CCS_PROT_MINIMUM_FREQUENCY_COUNTER,
  CCS_PROT_MINIMUM_FREQUENCY_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_MINIMUM_FREQUENCY_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ПО МАКСИМАЛЬНОЙ ЧАСТОТЕ
	CCS_PROT_MAXIMUM_FREQUENCY_MODE,
	CCS_PROT_MAXIMUM_FREQUENCY_ACTION,
	CCS_PROT_MAXIMUM_FREQUENCY_ACTIVATION,
	CCS_PROT_MAXIMUM_FREQUENCY_TIMER_DELAY,
	CCS_PROT_MAXIMUM_FREQUENCY_TIMER_RESTART,
	CCS_PROT_MAXIMUM_FREQUENCY_CONTROL_PARAM,
	CCS_PROT_MAXIMUM_FREQUENCY_MOTOR_SETPOINT,
	CCS_PROT_MAXIMUM_FREQUENCY_COUNTER_RESTART,
	CCS_PROT_MAXIMUM_FREQUENCY_TIMER_COUNTER_RESTART,
	CCS_PROT_MAXIMUM_FREQUENCY_BAN,
	CCS_PROT_MAXIMUM_FREQUENCY_BAN_SETPOINT,
	CCS_PROT_MAXIMUM_FREQUENCY_STATE,
	CCS_PROT_MAXIMUM_FREQUENCY_TIMER,
	CCS_PROT_MAXIMUM_FREQUENCY_COUNTER,
  CCS_PROT_MAXIMUM_FREQUENCY_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_MAXIMUM_FREQUENCY_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
  /// ЗАЩИТА ОТ ЧЕРЕДОВАНИЯ ФАЗ
	CCS_PROT_ROTATION_PHASE_MODE,
	CCS_PROT_ROTATION_PHASE_ACTION,
	CCS_PROT_ROTATION_PHASE_ACTIVATION,
	CCS_PROT_ROTATION_PHASE_TIMER_DELAY,
	CCS_PROT_ROTATION_PHASE_TIMER_RESTART,
	CCS_PROT_ROTATION_PHASE_CONTROL_PARAM,
	CCS_PROT_ROTATION_PHASE_MOTOR_SETPOINT,
	CCS_PROT_ROTATION_PHASE_COUNTER_RESTART,
	CCS_PROT_ROTATION_PHASE_TIMER_COUNTER_RESTART,
	CCS_PROT_ROTATION_PHASE_BAN,
	CCS_PROT_ROTATION_PHASE_BAN_SETPOINT,
	CCS_PROT_ROTATION_PHASE_STATE,
	CCS_PROT_ROTATION_PHASE_TIMER,
	CCS_PROT_ROTATION_PHASE_COUNTER,
  CCS_PROT_ROTATION_PHASE_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_ROTATION_PHASE_GENERAL_COUNTER_STOP,			/// Общий счётчик срабатывания защиты
	CCS_PROT_ROTATION_PHASE_VALUE,
  /// ЗАЩИТА ПО КОНТАКТНОМУ МОНОМЕТРУ (ДУМАЕТСЯ ЧТО ЗАЩИТА ПО ДИСКРЕТНОМУ ВХОДУ)
	CCS_PROT_CONTACT_MONOMETR_MODE,
	CCS_PROT_CONTACT_MONOMETR_ACTION,
	CCS_PROT_CONTACT_MONOMETR_ACTIVATION,
	CCS_PROT_CONTACT_MONOMETR_TIMER_DELAY,
	CCS_PROT_CONTACT_MONOMETR_TIMER_RESTART,
	CCS_PROT_CONTACT_MONOMETR_CONTROL_PARAM,
	CCS_PROT_CONTACT_MONOMETR_SETPOINT,
	CCS_PROT_CONTACT_MONOMETR_COUNTER_RESTART,
	CCS_PROT_CONTACT_MONOMETR_TIMER_COUNTER_RESTART,
	CCS_PROT_CONTACT_MONOMETR_BAN,
	CCS_PROT_CONTACT_MONOMETR_BAN_SETPOINT,
	CCS_PROT_CONTACT_MONOMETR_STATE,
	CCS_PROT_CONTACT_MONOMETR_TIMER,
	CCS_PROT_CONTACT_MONOMETR_COUNTER,
  CCS_PROT_CONTACT_MONOMETR_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_CONTACT_MONOMETR_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
	CCS_PROT_CONTACT_MONOMETR_VALUE,
  /// ЗАЩИТА ПО ДОПОЛНИТЕЛЬНОМУ АНАЛОГОВОМУ ВХОДУ 1
	CCS_PROT_ADD_AI_1_MODE,
	CCS_PROT_ADD_AI_1_ACTION,
	CCS_PROT_ADD_AI_1_ACTIVATION,
	CCS_PROT_ADD_AI_1_TIMER_DELAY,
	CCS_PROT_ADD_AI_1_TIMER_RESTART,
	CCS_PROT_ADD_AI_1_CONTROL_PARAM,
	CCS_PROT_ADD_AI_1_SETPOINT,
	CCS_PROT_ADD_AI_1_COUNTER_RESTART,
	CCS_PROT_ADD_AI_1_TIMER_COUNTER_RESTART,
	CCS_PROT_ADD_AI_1_BAN,
	CCS_PROT_ADD_AI_1_BAN_SETPOINT,
	CCS_PROT_ADD_AI_1_STATE,
	CCS_PROT_ADD_AI_1_TIMER,
	CCS_PROT_ADD_AI_1_COUNTER,
  CCS_PROT_ADD_AI_1_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_ADD_AI_1_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
	CCS_PROT_ADD_AI_1_VALUE,
  /// ЗАЩИТА ПО ДОПОЛНИТЕЛЬНОМУ АНАЛОГОВОМУ ВХОДУ 2
	CCS_PROT_ADD_AI_2_MODE,
	CCS_PROT_ADD_AI_2_ACTION,
	CCS_PROT_ADD_AI_2_ACTIVATION,
	CCS_PROT_ADD_AI_2_TIMER_DELAY,
	CCS_PROT_ADD_AI_2_TIMER_RESTART,
	CCS_PROT_ADD_AI_2_CONTROL_PARAM,
	CCS_PROT_ADD_AI_2_SETPOINT,
	CCS_PROT_ADD_AI_2_COUNTER_RESTART,
	CCS_PROT_ADD_AI_2_TIMER_COUNTER_RESTART,
	CCS_PROT_ADD_AI_2_BAN,
	CCS_PROT_ADD_AI_2_BAN_SETPOINT,
	CCS_PROT_ADD_AI_2_STATE,
	CCS_PROT_ADD_AI_2_TIMER,
	CCS_PROT_ADD_AI_2_COUNTER,
  CCS_PROT_ADD_AI_2_GENERAL_COUNTER_RESTART,			/// Общий счётчик
  CCS_PROT_ADD_AI_2_GENERAL_COUNTER_STOP,				/// Общий счётчик срабатывания защиты
	CCS_PROT_ADD_AI_2_VALUE,

  /// РЕЖИМ ПРОГРАММНОГО ИЗМЕНЕНИЯ ЧАСТОТЫ
  /// Уставка включения режима
	CCS_RGM_PROGRAM_CHANGE_FREQ_MODE,

  /// Уставка режима, выключен, однократно или каждый запуск
	CCS_RGM_PROGRAM_CHANGE_FREQ_ACTION,

  /// Уставка начальная частота
	CCS_RGM_PROGRAM_CHANGE_FREQ_BEGIN_FREQ,

  /// Уставка конечная частота
	CCS_RGM_PROGRAM_CHANGE_FREQ_END_FREQ,

  /// Уставка темп изменения частоты
	CCS_RGM_PROGRAM_CHANGE_FREQ_RATE,

  /// Уставка время изменения частоты
	CCS_RGM_PROGRAM_CHANGE_FREQ_TIME,

  /// Уставка нижняя граница ЗСП
	CCS_RGM_PROGRAM_CHANGE_FREQ_ZSP,

  /// Вычисленное значение уставки ЗСП от текущей частоты
	CCS_RGM_PROGRAM_CHANGE_FREQ_CALC_ZSP,

  /// Вычисленное время изменения частоты на один шаг
	CCS_RGM_PROGRAM_CHANGE_FREQ_PERIOD,

  /// Текущее значение таймера между повышениями частоты
	CCS_RGM_PROGRAM_CHANGE_FREQ_TIMER,

  /// Время до конца выхода на конечную частоту
	CCS_RGM_PROGRAM_CHANGE_FREQ_TIMER_END,

  /// РЕЖИМ ПЕРИОДИЧЕСКОЙ РАБОТЫ
  /// Уставка включение режима
	CCS_RGM_PERIODIC_MODE,

  /// Уставка режима, выключен, однократно или каждый запуск
	CCS_RGM_PERIODIC_ACTION,

  /// Уставка времени работы в периодическом режиме
	CCS_RGM_PERIODIC_TIME_WORK,

  /// Уставка времени паузы в периодическом режиме
	CCS_RGM_PERIODIC_TIME_STOP,

  /// Текущее значение таймера цикла работы или паузы
	CCS_RGM_PERIODIC_TIME,

  /// Время до пуска / или время до останова по таймеру
	CCS_RGM_PERIODIC_TIME_END,

  /// Текущее состояние режима
	CCS_RGM_PERIODIC_STATE,

  /// РЕЖИМ ЧЕРЕДОВАНИЯ ЧАСТОТ
  /// Уставка включения режима
	CCS_RGM_SWITCH_FREQ_MODE,

  /// Уставка режим выключен, однократно или каждый запуск
	CCS_RGM_SWITCH_FREQ_ACTION,

  /// Уставка низкая частота
	CCS_RGM_SWITCH_FREQ_LOW,

  /// Уставка высокая частота
	CCS_RGM_SWITCH_FREQ_HI,

  /// Уставка время работы на низкой частоте
	CCS_RGM_SWITCH_FREQ_LOW_TIME,

  /// Уставка время работы на высокой частоте
	CCS_RGM_SWITCH_FREQ_HI_TIME,

  /// Текущее значение таймера цикла
	CCS_RGM_SWITCH_FREQ_TIME,

  /// Время до конца работы на текущей частоте
	CCS_RGM_SWITCH_FREQ_TIME_END,

  /// РЕЖИМ ВСТРЯХИВАНИЯ
    /// Период встряхивания, то есть интервал времени между сериями
    /// изменений частоты.
	CCS_RGM_SHAKE_PERIOD,

  /// Количество встряхиваний
	CCS_RGM_SHAKE_QUANTITY,

    /// Счётчик встряхиваний
    CCS_RGM_SHAKE_COUNT,

    /// Частота встряхиваний низкая
    CCS_RGM_LOW_FREQ,

    /// Частота встряхиваний высокая
    CCS_RGM_HI_FREQ,

    /// Темп набора частоты низкой частоты
    CCS_RGM_LOW_RATE,

    /// Темп набора высокой частоты
    CCS_RGM_HI_RATE,

    /// Время до следующего встряхивания
    CCS_RGM_TIME,

  /// Номер ЦДНГ
	CCS_NUMBER_CDNG,

  /// Номер куста
	CCS_NUMBER_BUSH,

  /// Номер скважины
	CCS_NUMBER_WELL,

  /// Производительность насоса
	CCS_PUMP_CAPACITY,

  /// Напор ЭЦН
	CCS_PUMP_HEAD,

  /// Напряжение отпайки
	CCS_VOLTAGE_TAP_OFF,

  /// Дата последнего запуска
	CCS_DATA_LAST_START,

  /// Время последнего запуска
	CCS_TIME_LAST_START,

  /// Дата последнего останова
	CCS_DATA_LAST_STOP,

  /// Время последнего останова
	CCS_TIME_LAST_STOP,

  /// Причина последнего пуска
	CCS_CODE_START,

  /// Причина останова
	CCS_CODE_STOP,

  /// Ток холостого хода
	CCS_CURRENT_HH_MOTOR,

  /// Глубина спуска
	CCS_DEPTH,

  /// Тип Scada системы
	CCS_PROTOCOL_SCADA,

  /// Адрес во внешний телемеханики
	CCS_ADDRESS_SCADA,

  /// Скорость работы со Scada
	CCS_BITRATE_SCADA,

  /// Версия протокола Scada
	CCS_CODE_SCADA,

  CCS_PHYSIC_MIN_ADD_AI_1,									/// Физический минимнимум дополнительного аналогового входа 1
  CCS_PHYSIC_MAX_ADD_AI_1,									/// Физический максимум дополнительного аналогового входа 1
  CCS_PHYSIC_MIN_ADD_AI_2,									/// Физический минимнимум дополнительного аналогового входа 2
  CCS_PHYSIC_MAX_ADD_AI_2,									/// Физический максимум дополнительного аналогового входа 2
  CCS_GENERAL_WORK_TIME,										/// Общее время работы СУ
  CCS_GENERAL_STOP_TIME,										/// Общее время простоя СУ
  CCS_CURRENT_WORK_TIME,										/// Текущее время работы СУ
  CCS_PASSWORD_OPERATOR,										/// Пароль оператора
  CCS_PASSWORD_TECHNOLOG,										/// Пароль технолога
  CCS_PASSWORD_SERVICE,										/// Пароль сервисного инженера
  CCS_PASSWORD_GOD,											/// Пароль режима "бога"
  CCS_PERIOD_LOG_NOLMAL_MODE,	  								/// Период записи в нормальном режиме
  CCS_PERIOD_LOG_FAST_MODE,									/// Период записи в ускоренном режиме
  CCS_PERIOD_LOG_SHORT_TMS,									/// Период записи температуры обмоток двиагтеля и давления на приёме
  CCS_BREAK_RTC,												/// СНГ сбой часов реального времени
  CCS_OPEN_DOOR,												/// СНГ открыта дверь
  CCS_REMOTE_BLOCK,											/// СНГ Блокировка дистанционно
  CCS_BREAK_CONTACTOR,										/// СНГ Поломка контактора
  CCS_WORKING_OPERATOR,										/// СНГ Местный режим, должен устанавливаться в 1 и держать 1 минуту
  CCS_WORKING_MODE,											/// СНГ Режим работы
  CCS_REMOTE_PROGRAM_CCS,										/// СНГ Удаленное перепрограммирование СУ, команда
  CSS_REMOTE_PROGRAM_TMS,										/// СНГ Удаленное перепрограммирование ТМС, команда
  CCS_REMOTE_PROGRAM_MS,										/// СНГ Удаленное перепрограммирование МС, команда
	CCS_WORK_PROGRAM_MODE,

	CCS_END,

	VSD_BEGIN        = 10000,
  //// Первый параметр ЧРП

	VSD_INDEX,
  //// Указатель индекса для массивных параметров
  //// Danfoss 0x0008 используется для массивов параметров

	VSD_UNIT_SPEED,
  //// Единицы измерения скорости
  //// Danfoss 0-02 Не используется в проекте, только при конфигурировании

	VSD_CONFIG_MODE,
  //// Режим конфигурирования
  //// Danfoss 1-00 Не используется в проекте, только при конфигурировании

	VSD_CONTROL_MOTOR,
  //// Принцип управления двигателем
  //// Danfoss 1-01 Не используется в проекте, только при конфигурировании

	VSD_TORQUE_CHARACTERISTIC,
  //// Характеристика момента нагрузки
  //// Danfoss 1-03 Не используется в проекте, только при конфигурировании

	VSD_OVERLOAD_MODE,
  //// Режим перегрузки
  //// Danfoss 1-04 Не используется в проекте, только при конфигурировании

	VSD_TYPE_MOTOR,
  //// Конструкция двигателя
  //// Danfoss 1-10 Используется в интерфейсе

	VSD_DAMPING_GANE,
  //// Усиление демпфирования
  //// Danfoss 1-14 Не используется в проекте, только при конфигурировании

	VSD_LOW_SPEED_FILTER_TIME,
  //// Время фильтрации при низкой скорости
  //// Danfoss 1-15 Используется в режиме автоадаптации

	VSD_HIGH_SPEED_FILTER_TIME,
  //// Время фильтрации при высокой скорости
  //// Danfoss 1-16 Используется в режиме автоадаптации

	VSD_POWER_MOTOR,
  //// Мощность двигателя
  //// Danfoss 1-20 Используется в проекте, интерфейс

	VSD_VOLTAGE_MOTOR,
  //// Напряжение двигателя
  //// Danfoss 1-22 Используется в проекте, интерфейс
  //// Novomet 0х0045 IREG_MOTOR_U_MAX

	VSD_FREQUENCY_MOTOR,
  //// Частота двигателя
  //// Danfoss 1-23 Используется в проекте, интерфейс
  //// Novomet 0х0043 IREG_MOTOR_F_MAX

	VSD_CURRENT_MOTOR,
  //// Ток двигателя
  //// Danfoss 1-24 Используется в проекте, интерфейс
  //// Novomet 0х0044 IREG_MOTOR_I_MAX

	VSD_SPEED_MOTOR,
  //// Скорость двигателя
  //// Danfoss 1-25 Используется в проекте, интерфейс

	VSD_RATE_TORQUE_MOTOR,
  //// Длительный номинальный момент двигателя
  //// Danfoss 1-26 Записывается в проекте один раз

	VSD_RESISTANCE_STATOR,
  //// Сопротивление статора
  //// Danfoss 1-30 Используется в режиме автоадаптации

	VSD_D_AXIS_INDUNSTANCE,
  //// Индуктивности по оси D
  //// Danfoss 1-37 Используется в проекте

	VSD_POLES_MOTOR,
  //// Число полюсов двигателя
  //// Danfoss 1-39 Используется в проекте

	VSD_BACK_EMF,
  //// Противо эдс
  //// Danfoss 1-40 Используется в проекте

	VSD_UF_CHARACTERISTIC_U,
  //// Характеристика Uf - U
  //// Danfoss 1-55

	VSD_UF_CHARACTERISTIC_F,
  //// Характеристика Uf - f
  //// Danfoss 1-56

	VSD_RESONANCE_REMOVE,
  //// Подавление резонанса %
  //// Danfoss 1-64 Не используется в проекте, только при конфигурировании

	VSD_RESONANCE_TIME,
  //// Постаянная времени подавления
  //// Danfoss 1-65 Не используется в проекте, только при конфигурировании
  //// Novomet 0x0067

	VSD_MIN_CURRENT_LOW_SPEED,
  //// Минимальный ток при низкой скорости
  //// Danfoss 1-66 Не используется в проекте, только при конфигурировании

	VSD_PM_START_MODE,
  //// Тип запуска вентиля
  //// Danfoss 1-70 Не используется в проекте, только при конфигурировании

	VSD_START_DELAY,
  //// Задержка запуска
  //// Danfoss 1-71 Используется в защите от невыхода на заданную частоту

	VSD_START_FUNCTION,
  //// Функция запуска
  //// Danfoss 1-72 Не используется в проекте, только при конфигурировании

	VSD_STOP_FUNCTION,
  //// Функция при остановке
  //// Danfoss 1-80 Не используется в проекте, только при конфигурировании

	VSD_STOP_SPEED,
  //// Минимальная скорость для функции при останове
  //// Danfoss 1-82 Не используется в проекте, только при конфигурировании

	VSD_CONTROL_TERMISTOR_MTR,
  //// Контроль термистора двигателя
  //// Danfoss 1-90 Не используется в проекте, только при конфигурировании

	VSD_THERMISTOR_RESOURCE,
  //// Вход термистора
  //// Danfoss 1-93 Не используется в проекте, только при конфигурировании

	VSD_PARKING_CURRENT,
  //// Начальный ток
  //// Danfoss 2-06 Не используется в проекте, только при конфигурировании

	VSD_PARKING_TIME,
  //// Длительность действия начального тока
  //// Danfoss 2-07 В проекте есть, но отключено, используется при конфигурировании

	VSD_OVERVOLTAGE_CONTROL,
  //// Контроль перенапряжения
  //// Danfoss 2-17 Не используется в проекте, только при конфигурировании

	VSD_MIN_REFERENCE,
  //// Минимальное задание
  //// Danfoss 3-02 Не используется в проекте, только при конфигурировании

	VSD_MAX_REFERENCE,
  //// Максимальное задание
  //// Danfoss 3-03 Не используется в проекте, только при конфигурировании

	VSD_FREQUENCY,
  //// Фиксированная скорость
  //// Danfoss 3-11 Уставка частоты, основной задаваемый параметр
  //// Novomet 0х0034 IREG_FREQ_REF

	VSD_RESOURCE_TASK_1,
  //// Ресурс задания 1
  //// Danfoss 3-15 Не используется в проекте, только при конфигурировании

	VSD_RESOURCE_TASK_2,
  //// Ресурс задания 2
  //// Danfoss 3-16 Не используется в проекте, только при конфигурировании

	VSD_RESOURCE_TASK_3,
  //// Ресурс задания 3
  //// Danfoss 3-17 Не используется в проекте, только при конфигурировании

	VSD_TYPE_SPEED_CHANGE,
  //// Тип изменения скорости
  //// Danfoss 3-40 Не используется в проекте, только при конфигурировании

	VSD_TIMER_DISPERSAL,
  //// Время разгона
  //// Danfoss 3-41 % Активно используется в проекте

	VSD_TIMER_DELAY,
  //// Время замедления
  //// Danfoss 3-42 % Активно используется в проекте

	VSD_TIMER_DISP_FIX_SPEED,
  //// Время разгона фиксированной скорости
  //// Danfoss 3-80 Используется в проекте один раз

	VSD_TIMER_DELAY_FIX_SPEED,
  //// Время замедления фиксированной скорости
  //// Danfoss 3-81 Используется в проекте один раз

	VSD_ROTATION,
  //// Направления вращения
  //// Danfoss 4-10 Не используется в проекте, только при конфигурировании

	VSD_LOW_LIM_SPEED_MOTOR,
  //// Нижний предел скорости
  //// Danfoss 4-12 Гц Используется в проекте

	VSD_HIGH_LIM_SPEED_MOTOR,
  //// Верхний передел скорости
  //// Danfoss 4-14 Используется в проекте
  //// Novomet 0х0037 IREG_FREQ_REF_MAX

	VSD_TORQUE_LIMIT,
  //// Ограничение момента
  //// Danfoss 4-16 % Используется в защитах ЧРП. Интерфейс

	VSD_TORQUE_LIMIT_GEN,
  //// Ограничение момента в режиме генератора
  //// Danfoss 4-17 % Не используется в проекте, только при конфигурировании

	VSD_CURRENT_LIMIT,
  //// Предел тока
  //// Danfoss 4-18 %

	VSD_MAX_OUTPUT_FREQUENCY,
  //// Максимальная выходная частота
  //// Danfoss 4-19 Используется в проекте

	VSD_MTR_FEEDBACK_LOSS_FUNC,
  //// Реакция на ошибку обратной связи
  //// Danfoss 4-30 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR_FUNCTION,
  //// Реакция на ошибку задания частоты
  //// Danfoss 4-34 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR,
  //// Величина несоответствия частота
  //// Danfoss 4-35 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR_TIMEOUT,
  //// Величина несоответствия частоты
  //// Danfoss 4-36 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR_RAMPING,
  //// Величина несоответствия частоты при разгоне
  //// Danfoss 4-37 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR_RAMP_TIME,
  //// Задержка срабатывания при несоответствии частоты при разгоне
  //// Danfoss 4-38 Не используется в проекте, только при конфигурировании

	VSD_TRACK_ERROR_AFTER_RAMP,
  //// Tracking Error After Ramping Timeout
  //// Danfoss 4-39 Не используется в проекте, только при конфигурировании

	VSD_WARNING_CURRENT_LOW,
  //// Danfoss 4-50 Не используется в проекте, только при конфигурировании

	VSD_WARNING_CURRENT_HIGH,
  //// Danfoss 4-51 Не используется в проекте, только при конфигурировании

	VSD_WARNING_SPEED_LOW,
  //// Минимальная скорость
  //// Danfoss 4-52 об/мин Не используется в проекте, только при конфигурировании

	VSD_TERMINAL_27_MODE,
  //// Режим клеммы 27
  //// Danfoss 5-01 Не используется в проекте, только при конфигурировании

	VSD_TERMINAL_29_MODE,
  //// Режим клеммы 29
  //// Danfoss 5-02 Не используется в проекте, только при конфигурировании

	VSD_DI_18,
  //// Цифровой вход 18
  //// Danfoss 5-10 Не используется в проекте, только при конфигурировании

	VSD_DI_19,
  //// Цифровой вход 19
  //// Danfoss 5-11 Не используется в проекте, только при конфигурировании

	VSD_DI_27,
  //// Цифровой вход 27
  //// Danfoss 5-12 Не используется в проекте, только при конфигурировании

	VSD_DI_32,
  //// Цифровой вход 32
  //// Danfoss 5-14 Не используется в проекте, только при конфигурировании

	VSD_DI_33,
  //// Цифровой вход 33
  //// Danfoss 5-15 Не используется в проекте, только при конфигурировании

	VSD_TERMINAL_27_DI,
  //// Клемма 27 Цифровой вход
  //// Danfoss 5-30 Не используется в проекте, только при конфигурировании

	VSD_TERMINAL_29_DI,
  //// Клемма 29 Цифровой вход
  //// Danfoss 5-31 Не используется в проекте, только при конфигурировании

	VSD_FUNCTION_RELE,
  //// Реле функций
  //// Danfoss 5-40 Не используется в проекте, только при конфигурировании

	VSD_ON_DELAY_RELAY,
  //// Нипонятно
  //// Danfoss 5-41 Не используется в проекте, только при конфигурировании

	VSD_42_AO,
  //// Клемма 42 аналоговый вход
  //// Danfoss 6-50 Не используется в проекте, только при конфигурировании

	VSD_SL_CONTROLLER_MODE,
  //// Режим контроллера SL
  //// Danfoss 13-00 Не используется в проекте, только при конфигурировании

	VSD_SL_START_EVENT,
  //// Событие запуска
  //// Danfoss 13-01 Не используется в проекте, только при конфигурировании

	VSD_SL_STOP_EVENT,
  //// Событие останова
  //// Danfoss 13-02 Не используется в проекте, только при конфигурировании

	VSD_SL_RESET,
  //// Сброс настроек компараторов
  //// Danfoss 13-03 Не используется в проекте, только при конфигурировании

	VSD_SL_10,
  //// Операнд сравнения компаратора
  //// Danfoss 13-10 Не используется в проекте, только при конфигурировании

	VSD_SL_11,
  //// Оператор сравнения компаратора
  //// Danfoss 13-11 Не используется в проекте, только при конфигурировании

	VSD_SL_12,
  //// Значение компаратора
  //// Danfoss 13-12 Не используется в проекте, только при конфигурировании

	VSD_SL_15,
  //// RS-FF Operand S
  //// Danfoss 13-15 Не используется в проекте, только при конфигурировании

	VSD_SL_16,
  //// RS-FF Operand R
  //// Danfoss 13-16 Не используется в проекте, только при конфигурировании

	VSD_SL_20,
  //// Временная задержка
  //// Danfoss 13-20 Не используется в проекте, только при конфигурировании

	VSD_SL_40,
  //// Булева переменная логического соотношения 1
  //// Danfoss 13-40 Не используется в проекте, только при конфигурировании

	VSD_SL_41,
  //// Оператор логического соотношения 1
  //// Danfoss 13-41 Не используется в проекте, только при конфигурировании

	VSD_SL_42,
  //// Булева переменная логического соотношения 2
  //// Danfoss  13-42 Не используется в проекте, только при конфигурировании

	VSD_SL_43,
  //// Logic Rule Operator 2
  //// Danfoss 13-43 Не используется в проекте, только при конфигурировании

	VSD_SL_44,
  //// Logic Rule Boolean 3
  //// Danfoss 13-44 Не используется в проекте, только при конфигурировании

	VSD_SL_51,
  //// Событие контроллера SL
  //// Danfoss 13-51 Не используется в проекте, только при конфигурировании

	VSD_SL_52,
  //// Действие контроллера SL
  //// Danfoss 13-52 Не используется в проекте, только при конфигурировании

	VSD_SWITCHING_FREQUENCY,
  //// Частота коммутации
  //// Danfoss 14-01 Используется в проекте
  //// Novomet 0х003C IREG_FREQ_PWM

	VSD_OVERMODULATION,
  //// Сверхмодуляция
  //// Danfoss 14-03 Не используется в проекте, только при конфигурировании

	VSD_DEAD_TIME_COMPENSATION,
  //// Компенсация задержки
  //// Danfoss 14-06 Не используется в проекте, только при конфигурировании

	VSD_MAINS_FAILURE,
  //// Отказ питания
  //// Danfoss 14-10 Не используется в проекте, только при конфигурировании

	VSD_MAINS_VOLTAGE_FAILURE,
  //// Напряжение сети при отказе питания
  //// Danfoss 14-11 Не используется в проекте, только при конфигурировании

	VSD_RESET_MODE,
  //// Режим сброса
  //// Danfoss 14-20 Не используется в проекте, только при конфигурировании

	VSD_AUTOSTART_TIME,
  //// Время автоматического перезапуск
  //// Danfoss 14-21 Не используется в проекте, только при конфигурировании

	VSD_DELAY_CURRENT_LIMIM,
  //// Задержка перед выключение при пределе тока
  //// Danfoss 14-24 Не используется в проекте, только при конфигурировании

	VSD_DELAY_TORQUE_LIMIT,
  //// Задержка перед выключение при пределе моменте
  //// Danfoss 14-25 Не используется в проекте, только при конфигурировании

	VSD_FIL_TIME_CURRENT_LIMIT,
  //// Время фильтра регулятора тока
  //// Danfoss 14-32 Используется в проекте, CalcParam()

	VSD_COS_PHI_MOTOR,
  //// Косинус Фи
  //// Danfoss 14-43 Используется в проекте

	VSD_DC_COMPENSATION,
  //// Компенсация напряжения постоянного тока
  //// Danfoss 14-51 Не используется в проекте, только при конфигурировании

	VSD_FAN_CONTROL,
  //// Управление вентилятором
  //// Danfoss 14-52 Не используется в проекте, только при конфигурировании

	VSD_OUT_FILTER,
  //// Выходной фильтр
  //// Danfoss 14-55 Используется в проекте, интерфейс

	VSD_FAIL_RESET,
  //// Сброс аварий
  //// Danfoss 14-90 Не используется в проекте, только при конфигурировании

	VSD_SOFT_VERSION,
  //// Версия ПО ЧРП
  //// Danfoss 15-43 Используется в проекте

	VSD_COMMAND_WORD,
  //// Командное слово
  //// Danfoss 16-00 Используется в проекте

	VSD_STATUS_WORD,
  //// Слово состояния
  //// Danfoss 16-03 Активно используется в проекте

	VSD_ACTIVE_POWER,
  //// Активная мощность
  //// Danfoss 16-10 Используется в проекте
  //// Novomet 0х0071 IREG_POUT

	VSD_OUT_VOLTAGE_MOTOR,
  //// Напряжение двигателя
  //// Danfoss 16-12 Используется в проекте

	VSD_CURRENT_FREQUENCY,
  //// Текущая частота двигателя
  //// Danfoss 16-13 Используется в проекте
  //// Novomet 0х0035 IREG_FREQ

	VSD_OUT_CURRENT_MOTOR,
  //// Выходной ток средний
  //// Danfoss 16-14 Используется в проекте
  //// Novomet 0х0072 IREG_IOUT

	VSD_CURRENT_SPEED_RPM,
  //// Текущая скорость вращения двигателя
  //// Danfoss 16-17 Используется в проекте

	VSD_DC_VOLTAGE,
  //// Напряжение цепи пост.тока
  //// Danfoss 16-30 Используется в проекте
  //// Novomet 0х008B IREG_UD

	VSD_RADIATOR_TEMPERATURE,
  //// Температура радиатора
  //// Danfoss 16-34 Используется в проекте

	VSD_CONTROL_TEMPERATURE,
  //// Температура платы управления
  //// Danfoss 16-39 Используется в проекте

	VSD_OUT_CURRENT_PHASE_1,
  //// Ток двигателя 1
  //// Danfoss 16-45 Используется в проекте

	VSD_OUT_CURRENT_PHASE_2,
  //// Ток двигателя 2
  //// Danfoss 16-46 Используется в проекте

	VSD_OUT_CURRENT_PHASE_3,
  //// Ток двигателя 3
  //// Danfoss 16-47 Используется в проекте

	VSD_DI_VSD,
  //// Цифровоый вход ПЧ
  //// Danfoss 16-60 Используется в проекте

	VSD_LAST_ALARM,
  //// Номер последней аварии и предупреждения
  //// Danfoss 16-87 Используется в проекте

	VSD_ALARM_WORD_1,
  //// Слово Аварии 1
  //// Danfoss 16-90 Используется в проекте

	VSD_ALARM_WORD_2,
  //// Слово Аварии 2
  //// Danfoss 16-91 Используется в проекте

	VSD_WARNING_WORD_1,
  //// Слово предупреждения 1
  //// Danfoss 16-92 Используется в проекте

	VSD_WARNING_WORD_2,
  //// Слово предупреждения 2
  //// Danfoss 16-93 Используется в проекте

	VSD_HIGH_START_TORQUE_TIME,
  //// Время применения высокого пускового момента
  //// Danfoss 30-20 Не используется в проекте, только при конфигурировании

	VSD_HIGH_START_TORQUE_CURRENT,
  //// Пусковой ток в % от номинального
  //// Danfoss 30-21 Не используется в проекте, только при конфигурировании

	VSD_LOCK_ROTOR_PROTECTION,
  //// Защита блокировки ротора
  //// Danfoss 30-22 Не используется в проекте, только при конфигурировании

	VSD_LOCK_ROTOR_TIME,
  //// Время задержки аварии блокировки ротора
  //// Danfoss 30-23 Не используется в проекте, только при конфигурировании

	VSD_THYR_ANGLE_REFERENCE,
  //// Угол управления тиристорами; служебный регистр для отладки работы выпрямителя
  //// Novomet 0x000A IREG_THYR_ANGLEREF

	VSD_THYR_STATUS,
  //// Статус платы выпрямителя. Регистр для внутреннего использования в ПЧ
  //// Novomet 0x000B IREG_THYR_STATUS

	VSD_THYR_CONTROL,
  //// Регистр управления тиристорами; используется только контроллером ПЧ
  //// Novomet 0x000С IREG_THYR_CONTROL

	VSD_THYR_VOLT_SHORT_CUILT,
  //// Напряжение защиты от КЗ инвертора.
  //// Novomet 0x000D IREG_THYR_USHORTCIRCUIT

  /// Статус индикатора
  /// Novomet 0x0014
	VSD_INDICATOR_STATUS,

	VSD_DOUTPUTS,
  //// Маска состояния цифровых выходов.
  //// Novomet 0х001E IREG_MEASURE_OUTPUTS

	VSD_DINPUTS,
  //// Маска состояния цифровых входов.
  //// Novomet 0х001F IREG_MEASURE_INPUTS

	VSD_DOUTPUTS_ENABLE,
  //// Маска включения цифровых выходов.
  //// Novomet 0х0020 IREG_MEASURE_OUTPUT_ENABLE

	VSD_DOUTPUTS_DISABLE,
  //// Маска выключения цифровых выходов
  //// Novomet 0х0021 IREG_MEASURE_OUTPUT_DISABLE

	VSD_VENT_ON_TEMPERATURE,
  //// Температура включения главного вентилятора.
  //// Novomet 0х0022 IREG_MEASURE_VENT1_ON

	VSD_VENT_OFF_TEMPERATURE,
  //// Температура выключения главного вентилятора.
  //// Novomet 0х0023 IREG_MEASURE_VENT1_OFF

	VSD_VENT_PERIOD,
  //// Период проверки температуры радиаторов
  //// Novomet 0х0024 IREG_MEASURE_VENT1_SCAN_PERIOD

	VSD_VENT_TEMPERATURE_FAULT,
  //// Температура срабатывания тепловой защиты радиаторов
  //// Novomet 0х0025 IREG_MEASURE_TFAULT

	VSD_INVERTOR_CONTROL,
  //// Флаговый регистр управления ПЧ
  //// Novomet 0х0032 IREG_INVERTOR_CONTROL

	VSD_INVERTOR_STATUS,
  //// Регистр состояния инвертора
  //// Novomet 0х0033 IREG_INVERTOR_STATUS

	VSD_FREQUENCY_ERROR,
  //// Уставка частоты "тревожного" режима
  //// Novomet 0х0036 IREG_FREQ_ERROR

	VSD_T_SPEEDUP,
  //// Период нарастания частоты.
  //// Novomet 0х0038 IREG_T_SPEEDUP

	VSD_T_SPEEDDOWN,
  //// Период Период снижения частоты
  //// Novomet 0х0039 IREG_T_SPEEDDOWN

	VSD_T_ILIMIT_SPEEDDOWN,
  //// Период снижения частоты при токоограничении.
  //// Novomet 0х003A IREG_T_ILIMIT_SPEEDDOWN

	VSD_T_ULOW_SPEEDDOWN,
  //// Период снижения частоты при нехватке напряжения на инверторе
  //// Novomet 0х003B IREG_T_ULOW_SPEEDDOWN

	VSD_CURRENT_REGULATOR,
  //// Тип текущего алгоритма управления (регулятора)
  //// Novomet 0х003D IREG_CURRENT_REGULATOR

	VSD_UF_U_FORCE,
  //// Напряжение форсировки
  //// Novomet 0х003E IREG_UF_U_FORCE

	VSD_UF_F_FORCE,
  //// Частота форсировки
  //// Novomet 0х003F IREG_UF_F_FORCE

	VSD_UF_UHH,
  //// Расчетное напряжение холостого хода.
  //// Novomet 0х0040 IREG_UF_UHH

	VSD_IFB_COMP,
  //// Постоянная времени компенсации индуктивности
  //// Novomet 0х0041 IREG_IFB_COMP

	VSD_VUOUT_PROP,
  //// Пропорциональный коэффициент ОС частотного контура регулирования
  //// Novomet 0х0042 IREG_VUOUT_PROP

	VSD_ILIMIT,
  //// Максимальный порог ограничения тока на выходе ПЧ, абс. вел
  //// Novomet 0х004E IREG_ILIMIT

	VSD_VUOUT_INTEG,
  //// Интегральный коэффициент ОС частотного контура регулирования
  //// Novomet 0х004F IREG_VUOUT_INTEG

	VSD_VTOUF_INTEG,
  //// Коэффициент постоянной времени обратного перехода с вектора на U/f
  //// Novomet 0х0050 IREG_VTOUF_INTEG

	VSD_UOUT_RATED,
  //// Текущее выходное напряжение ПЧ
  //// Novomet 0х0051 IREG_UOUT_RATED

	VSD_LOUT,
  //// Суммарная индуктивность на выходе ПЧ, фазное значение
  //// Novomet 0х0052 IREG_LOUT

	VSD_SW_STARTUP_FREQUENCY,
  //// Режим раскачки: частота
  //// Novomet 0х0053 IREG_STARTUP_FREQ

	VSD_SW_STARTUP_ANGLE_OSC,
  //// Режим раскачки: угол качания
  //// Novomet 0х0054 IREG_STARTUP_ANGLE_OSC

	VSD_SW_STARTUP_ANGLE_SHIFT,
  //// Режим раскачки: угол сдвига
  //// Novomet 0х0055 IREG_STARTUP_ANGLE_SHIFT

	VSD_SW_STARTUP_OSC_COUNT,
  //// Режим раскачки: количество качаний на угол
  //// Novomet 0х0056 IREG_STARTUP_OSC_COUNT

	VSD_SW_STARTUP_ROTATIONS,
  //// Режим раскачки: количество оборотов двигателя
  //// Novomet 0х0057    IREG_STARTUP_ROTATIONS

	VSD_SW_STARTUP_U_PULSE,
  //// Режим раскачки: кратность напряжения импульса расклинки
  //// Novomet 0х0058 IREG_STARTUP_U_PULSE

	VSD_SW_STARTUP_I_LIM,
  //// Режим раскачки: токоограничение
  //// Novomet 0х0059 IREG_STARTUP_I_LIM

	VSD_VFREQ_INTEG,
  //// Интегральный коэффициент ОС фазового контура регулирования
  //// Novomet 0х005A IREG_VFREQ_INTEG

	VSD_REGULATOR_QUEUE_1,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005B IREG_REGULATOR_QUEUE1

	VSD_REGULATOR_QUEUE_2,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005C IREG_REGULATOR_QUEUE2

	VSD_REGULATOR_QUEUE_3,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005D IREG_REGULATOR_QUEUE3

	VSD_REGULATOR_QUEUE_4,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005E IREG_REGULATOR_QUEUE4

	VSD_REGULATOR_QUEUE_5,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005F IREG_REGULATOR_QUEUE5

	VSD_UD_LOW_FAULT,
  //// Порог защиты ПЧ по снижению напряжения на инверторе
  //// Novomet 0х0060 IREG_UD_LOW_FAULT

	VSD_UD_HIGH_FAULT,
  //// Порог защиты ПЧ по превыщению напряжения на инверторе
  //// Novomet 0х0061 IREG_UD_HIGH_FAULT

	VSD_UIN_ASYM_LEVEL,
  //// Порог защиты ПЧ по несимметрии входного напряжения на инверторе
  //// Novomet 0х0062 IREG_UIN_ASYM_LEVEL

	VSD_UIN_ASYM_DELAY,
  //// Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
  //// Novomet 0х0063 IREG_UIN_ASYM_DELAY

	VSD_F_VECT,
  //// Частота перехода на векторный алгоритм
  //// Novomet 0х0064 IREG_F_VECT

	VSD_VFREQ_GAIN,
  //// Пропорциональный коэффициент ОС фазового контура регулирования
  //// Novomet 0х0065 IREG_VFREQ_GAIN

	VSD_VANGLE_GAIN,
  //// Фазовый коэффициент усиления системы подавления резонанса
  //// Novomet 0х0066 IREG_VANGLE_GAIN

	VSD_RES_TIMECONST,
  //// Постоянная времени системы подавления резонанса
  //// Novomet 0х0067 IREG_RES_TIMECONST

	VSD_RES_IAMP_VALID,
  //// Служебный регистр для отладки алгоритмов управления
  //// Novomet 0х0068 IREG_RES_IAMP_VALID

	VSD_IB,
  //// Выходной ток ПЧ по фазе В. (амплитудное значение)
  //// Novomet 0х0069    IREG_IB

	VSD_PHB,
  //// Фаза выходного тока В
  //// Novomet 0х006A IREG_PHB

	VSD_IC,
  //// Выходной ток ПЧ по фазе В. (амплитудное значение)
  //// Novomet 0х006B    IREG_IC

	VSD_PHC,
  //// Фаза выходного тока В
  //// Novomet 0х006C IREG_PHC

	VSD_SW_STARTUP_I_LIM_PULSE,
  //// Порог токоограничения импульса расклинки в режиме с раскачкой
  //// Novomet 0х006D IREG_STARTUP_I_LIM_PULSE

	VSD_DECEL_SPEEDDOWN,
  //// Время снижения уставки в режиме остановки турбинного вращения
  //// Novomet 0х006E IREG_DECEL_SPEEDDOWN

	VSD_DECEL_VOLTAGE,
  //// Напряжение остаточного напряжения на шине (окончания разряда) в режиме остановки турбинного вращения (для ВД)
  //// Novomet 0х006F IREG_DECEL_VOLTAGE

	VSD_DECEL_F_MAX,
  //// Начальная частота режима остановки турбинного вращения (для АД)
  //// Novomet 0х0070 IREG_DECEL_F_MAX

	VSD_TIME_SECOND,
  //// Время секунды
  //// Novomet 0х0073 IREG_TIME_SECOND

	VSD_TIME_MINUTE,
  //// Время минуты
  //// Novomet 0х0074 IREG_TIME_MINUTE

	VSD_TIME_HOUR,
  //// Время часы
  //// Novomet 0х0075 IREG_TIME_HOUR

	VSD_TIME_DAY,
  //// Время дни
  //// Novomet 0х0076 IREG_TIME_DAY

	VSD_TIME_MONTH,
  //// Время месяцы
  //// Novomet 0х0077 IREG_TIME_MONTH

	VSD_TIME_YEAR,
  //// Время годы
  //// Novomet 0х0078 IREG_TIME_YEAR

	VSD_AST_IMEAS,
  //// Величина измерительного тока автонастройки
  //// Novomet 0х0079 IREG_AST_IMEAS

	VSD_AST_UMEAS,
  //// Величина измерительного напряжения автонастройки
  //// Novomet 0х007A IREG_AST_UMEAS

	VSD_AST_LOUT_1_0,
  //// Измеренная величина выходной индуктивности автонастройки
  //// Novomet 0х007B IREG_AST_LOUT_1_0

	VSD_AST_STANDING,
  //// Дополнительный регистр автонастройки, зарезервирован
  //// Novomet 0х007С IREG_AST_STANDING

	VSD_INVERTOR_EXT_STATUS,
  //// Расширенный регистр состояния инвертора
  //// Novomet 0х007D IREG_INVERTOR_EXT_STATUS

	VSD_JARRING_PERIOD,
  //// Период экскурсий частоты режима встряхивания.
  //// Novomet 0х007E IREG_JARRING_PERIOD

	VSD_JARRING_UPDFREQ,
  //// Величина прибавки частоты вращения выше уставки в режиме встряхивания
  //// Novomet 0х007F IREG_JARRING_UPDFREQ

	VSD_JARRING_UPTIME,
  //// Время прибавки частоты вращения выше уставки в режиме встряхивания
  //// Novomet 0х0080 IREG_JARRING_UPTIME

	VSD_JARRING_DOWNDFREQ,
  //// Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  //// Novomet 0х0081 IREG_JARRING_DOWNDFREQ

	VSD_JARRING_DOWNTIME,
  //// Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  //// Novomet 0х0082 IREG_JARRING_DOWNTIME

	VSD_JARRING_MODE,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0083 IREG_JARRING_MODE

	VSD_JARRING_TICK_CNT,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0084 IREG_JARRING_TICK_CNT

	VSD_JARRING_SECOND_CNT,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0085 IREG_JARRING_SECOND_CNT

	VSD_START_FREQ,
  //// Стартовая частота вращения двигателя в основном режиме регулирования
  //// Novomet 0х0086 IREG_START_FREQ

	VSD_IA,
  //// Выходной ток ПЧ по фазе A. (амплитудное значение)
  //// Novomet 0х0087 IREG_IA

	VSD_PHA,
  //// Фаза выходного тока A
  //// Novomet 0х0088 IREG_PHA

	VSD_PHOUT,
  //// Усредненная фаза выходного тока ПЧ
  //// Novomet 0х0089 IREG_PHOUT

	VSD_END,

	TMS_BEGIN       = 20000,

	TMS_RESISTANCE_ISOLATION,
  //// Сопротивление изоляции
  //// Novomet BN-03

	TMS_PRESSURE_INTAKE,
  //// Давление на приёме

	TMS_TEMPERATURE_WINDING,
  //// Температура обмоток двигателя

	TMS_TEMPERATURE_INTAKE,
  //// Температура на приёме

	TMS_ACCELERATION_X_INTAKE,
  //// Ускорение по оси Х на приёме

	TMS_ACCELERATION_Y_INTAKE,
  //// Ускорение по оси Y на приёме

	TMS_ACCELERATION_XY_INTAKE,
  //// Ускорение по оси Y на приёме

	TMS_ACCELERATION_Z_INTAKE,
  //// Ускорение по оси Z на приёме

	TMS_ACCELERATIN_XYZ_INTAKE,
  //// Ускорение по оси по трём осям

	TMS_FLOW_DISCHARGE,
  //// Расход на выкиде

	TMS_PRESSURE_DISCHARGE,
  //// Давление на выкиде

	TMS_TEMPERATURE_DISCHARGE,
  //// Температура на выкиде

	TMS_MAX_PRESSURE_INTAKE,
  //// Максимальное давление на приёме

	TMS_MAX_TEMPERATUR_WINDING,
  //// Максимальная температура обмоток

	TMS_MAX_TEMPERATURE_INTAKE,
  //// Максимальная температура на приёме

	TMS_MAX_ACCELERATION_INTAKE,
  //// Максимальное ускорение

	TMS_FAIL_LINK_TMSP,
  //// Обрыв связи с ТМСП

	TMS_MANUFACTURE_TMSP,
  //// Код изготовителя ТМСП

	TMS_NUMBER_TMSP,
  //// Серийный номер ТМСП

	TMS_DATE_TMSP,
  //// Дата изготовления ТМСП

	TMS_CONSTRUCTION_TMSP,
  //// Код конструкции ТМСП

	TMS_PROTOCOL_TMSP,
  //// Код протокола ТМСП

	TMS_SENSOR_TMSP,
  //// Состав датчиков ТМСП

	TMS_SOFT_TMSP,
  //// Версия По ТМСП

	TMS_TIME_BIT,
  //// Скорости передачи бита

	TMS_MANUFACTURE_TMSN,
  //// Код завода изготовителя ТМСН

	TMS_NUMBER_TMSN,
  //// Заводской номер ТМСН

	TMS_DATE_TMSN,
  //// Дата изготовления ТМСН

	TMS_CONSTRUCTION_TMSN,
  //// Код конструкции ТМСН

	TMS_SOFT_TMSN,
  //// Версия ПО ТМСН

	TMS_PROTOCOL_TMSN,
  //// Код протокола ТМСН

	TMS_MIN_FLOW_DISCHARGE,
  //// Минимальный расход на выкиде

	TMS_MAX_FLOW_DISCHARGE,
  //// Максимальный расход на выкиде

	TMS_MAX_PRESSURE_DISHARGE,
  //// Максимальное давление на выкиде

	TMS_MAX_TEMPERATURE_DISCHARGE,
  //// Максимальная температура на выкиде

	TMS_RESISTANCE_ISOLATION_TIME,
  //// Время измерения сопротивления изоляции

	TMS_MEASURE_TIME,
  //// Время измерения данных и передачи

	TMS_RESISTANCE_TRANS_TIME,
  //// Время перехода в режим измерения сопротивления изоляции

	TMS_NUMBER_WELL,
  //// Номер скважины

	TMS_NUMBER_BUSH,
  //// Номер куста

	TMS_NUMBER_CDNG,
  //// Номер ЦДНГ

	TMS_MODBUS_ADDRESS,
  //// Адрес Modbus

	TMS_MODBUS_SPEED,
  //// Скорость Modbus

	TMS_MODBUS_STOP_BIT,
  //// Количество Стоп-бит

	TMS_DATA_TIME,
  //// Текущее дата время

	TMS_READ_STATIC_PARAM,
  //// Команда чтения статических параметров

	TMS_PACKAGE_ERROR_TMSP,
  //// Количество ошибок от ТМСП

	TMS_TEMPERATURE_MOTOR,
  //// Температура масла двигателя

	TMS_TEMPERATURE_UNIT,
  //// Единицы измерения температуры

	TMS_PRESSURE_UNIT,
  //// Единицы измерения давления

	TMS_PSW_TMS,
  //// Слово состояние системы

	TMS_PSW_TMSN,
  //// Слово состояние наземного блока

	TMS_PSW_TMSP_1,
  //// Cлово состояние погружного блока

	TMS_PSW_TMSP_2,
  //// Слово состояние погружного блока 2

	TMS_ANGLE_INTAKE,
  //// Зенитный угол

	TMS_END,
  //// Последний параметр ТМС, должен быть всегда последним параметром ТМС
  //// и быть без номера, всё новое добавлять до него

	EM_BEGIN         = 30000,

  //// Первый параметр группы счётчиков электроэнергии
	EM_FREQUENCY,
  //// Частота питающей сети

	EM_CURRENT_PHASE_1,
  //// Ток первой фазы

	EM_CURRENT_PHASE_2,
  //// Ток второй фазы

	EM_CURRENT_PHASE_3,
  //// Ток третей фазы

	EM_VOLTAGE_PHASE_1,
  //// Напряжение первой фазы

	EM_VOLTAGE_PHASE_2,
  //// Напряжение второй фазы

	EM_VOLTAGE_PHASE_3,
  //// Напряжение третей фазы

	EM_VOLTAGE_PHASE_1_2,
  //// Напряжение межфазное первой и второй фазы

	EM_VOLTAGE_PHASE_2_3,
  //// Напряжение межфазное второй и третьей фазы

	EM_VOLTAGE_PHASE_3_1,
  //// Напряжение межфазное третьей и первой фазы

	EM_COS_PHI,
  //// Общий коэффициент мощности

	EM_COS_PHI_PHASE_1,
  //// Коэффициент мощности первой фазы

	EM_COS_PHI_PHASE_2,
  //// Коэффициент мощности второй фазы

	EM_COS_PHI_PHASE_3,
  //// Коэффициент мощности третей фазы

	EM_FULL_POWER,
  //// Полная мощность

	EM_FULL_POWER_PHASE_1,
  //// Полная мощность первой фазы

	EM_FULL_POWER_PHASE_2,
  //// Полная мощность второй фазы

	EM_FULL_POWER_PHASE_3,
  //// Полная мощность третей фазы

	EM_ACTIVE_POWER,
  //// Общая активная мощность

	EM_ACTIVE_POWER_PHASE_1,
  //// Активная мощность первой фазы

	EM_ACTIVE_POWER_PHASE_2,
  //// Активная мощность второй фазы

	EM_ACTIVE_POWER_PHASE_3,
  //// Активная мощность третей фазы

	EM_REACTIVE_POWER,
  //// Общая реактивная мощность

	EM_REACTIVE_POWER_PHASE_1,
  //// Реактивная мощность первой фазы

	EM_REACTIVE_POWER_PHASE_2,
  //// Реактивная мощность второй фазы

	EM_REACTIVE_POWER_PHASE_3,
  //// Реактивная мощность третей фазы

	EM_ACTIVE_ENERGY,
  //// Общая активная энергия

	EM_ACTIVE_PLUS_ALL,
  //// Обшая активная плюс энергия за весь период

	EM_ACTIVE_MINUS_ALL,
  //// Обшая активная минус энергия за весь период

	EM_ACTIVE_PLUS_CUR_DAY,
  //// Активная плюс энергия за текущий день

	EM_ACTIVE_MINUS_CUR_DAY,
  //// Активная минус энергия за текущий день

	EM_ACTIVE_PLUS_PREV_DAY,
  //// Активная плюс энергия за предыдущий день

	EM_ACTIVE_MINUS_PREV_DAY,
  //// Активная минус энергия за текущий день

	EM_ACTIVE_PLUS_CUR_MONTH,
  //// Активная плюс энергия за текущий месяц

	EM_ACTIVE_MINUS_CUR_MONTH,
  //// Активная минус энергия за текущий месяц

	EM_ACTIVE_PLUS_PREV_MONTH,
  //// Активная плюс энергия на предыдущий месяц

	EM_ACTIVE_MINUS_PREV_MONTH,
  //// Активная минус энергия на предыдущий месяц

	EM_REACTIVE_ENERGY,
  //// Общая реактивня энергия

	EM_REACTIVE_PLUS_ALL,
  //// Общая плюс реактивная энергия за весь период

	EM_REACTIVE_MINUS_ALL,
  //// Общая минус реактивная энергия за весь период

	EM_REACTIVE_PLUS_CUR_DAY,
  //// Плюс реактивная энергия за весь день

	EM_REACTIVE_MINUS_CUR_DAY,
  //// Минус реактивная энергия за весь день

	EM_REACTIVE_PLUS_PREV_DAY,
  //// Плюс реактивная энергия за весь день

	EM_REACTIVE_MINUS_PREV_DAY,
  //// Минус реактивная энергия за весь день

	EM_REACTIVE_PLUS_CUR_MONTH,
  //// Плюс реактивная энергия за месяц

	EM_REACTIVE_MINUS_CUR_MONTH,
  //// Минус реактивная энергия за месяц

	EM_REACTIVE_PLUS_PREV_MONTH,
  //// Плюс реактивная энергия за предыдущий месяц

	EM_REACTIVE_MINUS_PREV_MONTH,
  //// Минус реактивная энергия за предыдущий месяц

	EM_COEFFICIENT_TRANS_CURRENT,
  //// Коэффицинт трансформации тока

	EM_COEFFICIENT_TRANS_VOLTAGE,
  //// Коэффициент трансформации напряжения

	EM_END
};

#endif /* DEFINE_H_ */

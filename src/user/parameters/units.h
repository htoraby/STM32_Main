#ifndef UNITS
#define UNITS

//! Число ячеек массива для ковертора
#define MAX_CONV_ELEMS 9

/*!
 * \brief The enPhysic enum
 * Типы физических величин
 */
enum enPhysic
{
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
  PHYSIC_CAPACITY,                          //!< Ёмкость
  PHYSIC_LAST
};

enum enPhysicNumeric {
  NUMERIC_NUMBER,
  NUMERIC_LAST
};

enum enPhysicDateTime {
  DATE_TIME_SS,
  DATE_TIME_YY_MM_DD_HH_MM_SS,
  DATE_TIME_YY_MM_DD,
  DATE_TIME_LAST
};

enum enPhysicTime {
  TIME_SS,
  TIME_US,
  TIME_MS,
  TIME_SS_MS,
  TIME_MM,
  TIME_MM_SS,
  TIME_HH,
  TIME_HH_MM,
  TIME_HH_MM_SS,
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
  SPACE_MM2,
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
  TEMPERATURE_F,
  TEMPERATURE_LAST
};

enum enPhysicAcceleration {
  ACCELERATION_G,
  ACCELERATION_MSS2,
  ACCELERATION_LAST
};

enum enPhysicSpeed {
  SPEED_M_SS,
  SPEED_MM_SS,
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
  ACTIVE_ENERGY_KW_HH,
  ACTIVE_ENERGY_LAST
};

enum enPhysicReactivEnergy {
  REACTIVE_ENERGY_KVAR_HH,
  REACTIVE_ENERGY_LAST
};

enum enPhysicFullEnergy {
  FULL_ENERGY_KVAHH,
  FULL_ENERGY_LAST
};

enum enPhysicRate {
  RATE_NM,
  RATE_LAST
};

enum enPhysicIndunctance {
  INDUNCTANCE_MH,
  INDUNCTANCE_MKH,
  INDUNCTANCE_LAST
};

enum enPhysicCapacity {
  CAPACITY_MKF,
  CAPACITY_LAST
};

/*!
 * \brief Коэффициенты преобразования единиц измерения
 */
extern float units[PHYSIC_LAST][MAX_CONV_ELEMS][2];

#endif // UNITS


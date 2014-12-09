/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"

Vsd::Vsd()
{
  asm("nop");
  /*
  setNumberElementArray(VSD_END - VSD_BEGIN);
  setShiftID(VSD_BEGIN);
  initParameters();
  */
}

Vsd::~Vsd()
{
  // TODO Auto-generated destructor stub
}

int Vsd::setFrequency(float Frequency)
{
  int Result = RETURN_ERROR;
  return Result;
}

void Vsd::initParameters()
{
  // Пустой элемент массива
  Parameters[VSD_BEGIN - VSD_BEGIN].ID                = VSD_BEGIN;
  Parameters[VSD_BEGIN - VSD_BEGIN].Access            = ACCESS_ERROR;
  Parameters[VSD_BEGIN - VSD_BEGIN].Operation         = OPERATION_ERROR;
  Parameters[VSD_BEGIN - VSD_BEGIN].Physic            = PHYSIC_ERROR;
  Parameters[VSD_BEGIN - VSD_BEGIN].Validity          = VALIDITY_ERROR;
  Parameters[VSD_BEGIN - VSD_BEGIN].Update            = UPDATE_ERROR;
  Parameters[VSD_BEGIN - VSD_BEGIN].Value             = 0.0;
  Parameters[VSD_BEGIN - VSD_BEGIN].Minimum           = 0.0;
  Parameters[VSD_BEGIN - VSD_BEGIN].Maximum           = 0.0;
  Parameters[VSD_BEGIN - VSD_BEGIN].Default           = 0.0;
  // Индекс "массивного параметра"
  Parameters[VSD_INDEX - VSD_BEGIN].ID                = VSD_INDEX;
  Parameters[VSD_INDEX - VSD_BEGIN].Access            = ACCESS_OPERATOR;
  Parameters[VSD_INDEX - VSD_BEGIN].Operation         = OPERATION_LIMITED;
  Parameters[VSD_INDEX - VSD_BEGIN].Physic            = PHYSIC_NUMERIC;
  Parameters[VSD_INDEX - VSD_BEGIN].Validity          = VALIDITY_ERROR;
  Parameters[VSD_INDEX - VSD_BEGIN].Update            = UPDATE_ERROR;
  Parameters[VSD_INDEX - VSD_BEGIN].Value             = 0.0;
  Parameters[VSD_INDEX - VSD_BEGIN].Minimum           = 0.0;
  Parameters[VSD_INDEX - VSD_BEGIN].Maximum           = 5.0;
  Parameters[VSD_INDEX - VSD_BEGIN].Default           = 0.0;
  // Единицы измерения скорости
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].ID           = VSD_UNIT_SPEED;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Access       = ACCESS_OPERATOR;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Operation    = OPERATION_LIMITED;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Physic       = PHYSIC_NUMERIC;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Validity     = VALIDITY_ERROR;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Update       = UPDATE_ERROR;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Value        = 0.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Minimum      = 0.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Maximum      = 2.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Default      = 0.0;
  // Режим конфигурирования
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].ID          = VSD_CONFIG_MODE;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Access      = ACCESS_OPERATOR;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Operation   = OPERATION_WRITE;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Physic      = PHYSIC_NUMERIC;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Validity    = VALIDITY_ERROR;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Update      = UPDATE_ERROR;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Value       = 0.0;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Minimum     = 0.0;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Maximum     = 3.0;
  Parameters[VSD_CONFIG_MODE - VSD_BEGIN].Default     = 0.0;
  // Принцип управления двигателем
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].ID        = VSD_CONTROL_MOTOR;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Maximum   = 3.0;
  Parameters[VSD_CONTROL_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Характеристика момента нагрузки
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].ID        = VSD_TORQUE_CHARACTERISTIC;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Maximum   = 2.0;
  Parameters[VSD_TORQUE_CHARACTERISTIC - VSD_BEGIN].Default   = 0.0;
  // Режим перегрузки
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].ID        = VSD_OVERLOAD_MODE;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Maximum   = 1.0;
  Parameters[VSD_OVERLOAD_MODE - VSD_BEGIN].Default   = 0.0;
  // Конструкция двигателя
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].ID           = VSD_TYPE_MOTOR;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Access       = ACCESS_OPERATOR;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Operation    = OPERATION_LIMITED;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Physic       = PHYSIC_NUMERIC;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Validity     = VALIDITY_ERROR;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Update       = UPDATE_ERROR;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Value        = 0.0;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Minimum      = 0.0;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Maximum      = 1.0;
  Parameters[VSD_TYPE_MOTOR - VSD_BEGIN].Default      = 0.0;
  // Усиление демпфирования
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].ID         = VSD_DAMPING_GANE;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Value      = 140.0;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Maximum    = 250.0;
  Parameters[VSD_DAMPING_GANE - VSD_BEGIN].Default    = 140.0;
  // Время фильтрации при низкой скорости
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].ID        = VSD_LOW_SPEED_FILTER_TIME;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Maximum   = 20.0;
  Parameters[VSD_LOW_SPEED_FILTER_TIME - VSD_BEGIN].Default   = 0.0;
  // Время фильтрации при высокой скорости
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].ID       = VSD_HIGH_SPEED_FILTER_TIME;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Maximum  = 20.0;
  Parameters[VSD_HIGH_SPEED_FILTER_TIME - VSD_BEGIN].Default  = 0.0;
  // Мощность двигателя
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].ID          = VSD_POWER_MOTOR;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Access      = ACCESS_OPERATOR;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Operation   = OPERATION_LIMITED;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Physic      = PHYSIC_POWER;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Validity    = VALIDITY_ERROR;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Update      = UPDATE_ERROR;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Value       = 0.0;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Minimum     = 0.0;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Maximum     = 1200.0;
  Parameters[VSD_POWER_MOTOR - VSD_BEGIN].Default     = 0.0;
  // Напряжение двигателя
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].ID        = VSD_VOLTAGE_MOTOR;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Maximum   = 1000.0;
  Parameters[VSD_VOLTAGE_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Частота двигателя
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].ID        = VSD_FREQUENCY_MOTOR;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Minimum   = 20.0;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Maximum   = 1000.0;
  Parameters[VSD_FREQUENCY_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Ток двигателя
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].ID        = VSD_CURRENT_MOTOR;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Maximum   = 2000.0;
  Parameters[VSD_CURRENT_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Скорость двигателя
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].ID        = VSD_SPEED_MOTOR;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Physic    = PHYSIC_RPM;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Value     = 10.0;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Minimum   = 10.0;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Maximum   = 6000.0;
  Parameters[VSD_SPEED_MOTOR - VSD_BEGIN].Default   = 10.0;
  // Длительный номинальный момент двигателя
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].ID        = VSD_RATE_TORQUE_MOTOR;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Physic    = PHYSIC_RATE;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Maximum   = 10000.0;
  Parameters[VSD_RATE_TORQUE_MOTOR - VSD_BEGIN].Default   = 1.0;
  // Сопротивление статора
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].ID        = VSD_RESISTANCE_STATOR;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Physic    = PHYSIC_RESISTANCE;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Maximum   = 1000000.0;
  Parameters[VSD_RESISTANCE_STATOR - VSD_BEGIN].Default   = 0.0;
  // Индуктивности по оси D
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].ID       = VSD_D_AXIS_INDUNSTANCE;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Operation= OPERATION_LIMITED;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Physic   = PHYSIC_INDUNCTANCE;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Maximum  = 1000.0;
  Parameters[VSD_D_AXIS_INDUNSTANCE - VSD_BEGIN].Default  = 0.0;
  // Число полюсов двигателя
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].ID        = VSD_POLES_MOTOR;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Value     = 2.0;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Minimum   = 2.0;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Maximum   = 10.0;
  Parameters[VSD_POLES_MOTOR - VSD_BEGIN].Default   = 2.0;
  // Danfoss 1-40 Используется в проекте
  Parameters[VSD_BACK_EMF - VSD_BEGIN].ID         = VSD_BACK_EMF;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Physic     = PHYSIC_VOLTAGE;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Value      = 10.0;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Minimum    =  10.0;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Maximum    = 9000.0;
  Parameters[VSD_BACK_EMF - VSD_BEGIN].Default    = 10.0;
  // Характеристика Uf - U
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].ID        = VSD_UF_CHARACTERISTIC_U;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Maximum   = 5.0;
  Parameters[VSD_UF_CHARACTERISTIC_U - VSD_BEGIN].Default   = 0.0;
  // Характеристика Uf - f
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].ID        = VSD_UF_CHARACTERISTIC_F;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Maximum   = 5.0;
  Parameters[VSD_UF_CHARACTERISTIC_F - VSD_BEGIN].Default   = 0.0;
  // Подавление резонанса %
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].ID         = VSD_RESONANCE_REMOVE;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Value      = 100.0;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Maximum    = 1000.0;
  Parameters[VSD_RESONANCE_REMOVE - VSD_BEGIN].Default    = 100.0;
  // Постоянная времени подавления
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].ID         = VSD_RESONANCE_TIME;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Physic     = PHYSIC_TIME;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Value      = 5.0;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Maximum    = 1000000.0;
  Parameters[VSD_RESONANCE_TIME - VSD_BEGIN].Default    = 5.0;
  // Минимальный ток при низкой скорости
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].ID        = VSD_MIN_CURRENT_LOW_SPEED;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Value     = 100.0;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Maximum   = 1000.0;
  Parameters[VSD_MIN_CURRENT_LOW_SPEED - VSD_BEGIN].Default   = 100.0;
  // Danfoss 1-70 Не используется в проекте; только при конфигурировании
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].ID        = VSD_PM_START_MODE;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Maximum   = 1.0;
  Parameters[VSD_PM_START_MODE - VSD_BEGIN].Default   = 0.0;
  // Задержка запуска
  Parameters[VSD_START_DELAY - VSD_BEGIN].ID        = VSD_START_DELAY;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Maximum   = 25.5;
  Parameters[VSD_START_DELAY - VSD_BEGIN].Default   = 0.0;
  // Функция запуска
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].ID       = VSD_START_FUNCTION;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Value    = 2.0;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Maximum  = 7.0;
  Parameters[VSD_START_FUNCTION - VSD_BEGIN].Default  = 2.0;
  // Функция при остановке
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].ID        = VSD_STOP_FUNCTION;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_STOP_FUNCTION - VSD_BEGIN].Default   = 0.0;
  // Минимальная скорость для функции при останове
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].ID         = VSD_STOP_SPEED;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Physic     = PHYSIC_FREQUENCY;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Maximum    = 20.0;
  Parameters[VSD_STOP_SPEED - VSD_BEGIN].Default    = 0.0;
  // Контроль термистора двигателя
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].ID        = VSD_CONTROL_TERMISTOR_MTR;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Maximum   = 21.0;
  Parameters[VSD_CONTROL_TERMISTOR_MTR - VSD_BEGIN].Default   = 0.0;
  // Вход термистора
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].ID        = VSD_THERMISTOR_RESOURCE;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_THERMISTOR_RESOURCE - VSD_BEGIN].Default   = 0.0;
  // Начальный ток
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].ID        = VSD_PARKING_CURRENT;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Physic    = PHYSIC_PERCENT;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Value     = 50.0;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Maximum   = 1000.0;
  Parameters[VSD_PARKING_CURRENT - VSD_BEGIN].Default   = 50.0;
  // Длительность действия начального тока
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].ID         = VSD_PARKING_TIME;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Physic     = PHYSIC_TIME;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Value      = 3.0;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Maximum    = 60.0;
  Parameters[VSD_PARKING_TIME - VSD_BEGIN].Default    = 3.0;
  // Контроль перенапряжения
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].ID        = VSD_OVERVOLTAGE_CONTROL;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Maximum   = 2.0;
  Parameters[VSD_OVERVOLTAGE_CONTROL - VSD_BEGIN].Default   = 0.0;
  // Минимальное задание
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].ID        = VSD_MIN_REFERENCE;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Maximum   = 999999.99;
  Parameters[VSD_MIN_REFERENCE - VSD_BEGIN].Default   = 0.0;
  // Максимальное задание
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].ID        = VSD_MAX_REFERENCE;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Maximum   = 999999.99;
  Parameters[VSD_MAX_REFERENCE - VSD_BEGIN].Default   = 0.0;
  // Фиксированная скорость
  Parameters[VSD_FREQUENCY - VSD_BEGIN].ID        = VSD_FREQUENCY;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Maximum   = 999999.99;
  Parameters[VSD_FREQUENCY - VSD_BEGIN].Default   = 0.0;
  // Ресурс задания 1
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].ID        = VSD_RESOURCE_TASK_1;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Maximum   = 29.0;
  Parameters[VSD_RESOURCE_TASK_1 - VSD_BEGIN].Default   = 0.0;
  // Ресурс задания 2
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].ID        = VSD_RESOURCE_TASK_2;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Maximum   = 29.0;
  Parameters[VSD_RESOURCE_TASK_2 - VSD_BEGIN].Default   = 0.0;
  // Ресурс задания 3
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].ID        = VSD_RESOURCE_TASK_3;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Maximum   = 29.0;
  Parameters[VSD_RESOURCE_TASK_3 - VSD_BEGIN].Default   = 0.0;
  // Тип изменения скорости
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].ID        = VSD_TYPE_SPEED_CHANGE;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Maximum   = 2.0;
  Parameters[VSD_TYPE_SPEED_CHANGE - VSD_BEGIN].Default   = 0.0;
  // Время разгона
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].ID        = VSD_TIMER_DISPERSAL;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Value     = 0.01;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Minimum   = 0.01;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Maximum   = 3600.0;
  Parameters[VSD_TIMER_DISPERSAL - VSD_BEGIN].Default   = 0.01;
  // Время замедления
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].ID        = VSD_TIMER_DELAY;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Value     = 0.01;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Minimum   = 0.01;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Maximum   = 3600.0;
  Parameters[VSD_TIMER_DELAY - VSD_BEGIN].Default   = 0.01;
  // Время разгона фиксированной скорости
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].ID         = VSD_TIMER_DISP_FIX_SPEED;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Physic     = PHYSIC_TIME;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Value      = 0.01;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Minimum    = 0.01;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Maximum    = 3600.0;
  Parameters[VSD_TIMER_DISP_FIX_SPEED - VSD_BEGIN].Default    = 0.01;
  // Время замедления фиксированной скорости
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].ID        = VSD_TIMER_DELAY_FIX_SPEED;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Value     = 0.01;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Minimum   = 0.01;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Maximum   = 3600.0;
  Parameters[VSD_TIMER_DELAY_FIX_SPEED - VSD_BEGIN].Default   = 0.01;
  // Направления вращения
  Parameters[VSD_ROTATION - VSD_BEGIN].ID         = VSD_ROTATION;
  Parameters[VSD_ROTATION - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_ROTATION - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_ROTATION - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_ROTATION - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_ROTATION - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_ROTATION - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_ROTATION - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_ROTATION - VSD_BEGIN].Maximum    = 2.0;
  Parameters[VSD_ROTATION - VSD_BEGIN].Default    = 0.0;
  // Нижний предел скорости
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].ID        = VSD_LOW_LIM_SPEED_MOTOR;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Maximum   = 999999.99;
  Parameters[VSD_LOW_LIM_SPEED_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Верхний передел скорости
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].ID       = VSD_HIGH_LIM_SPEED_MOTOR;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Physic   = PHYSIC_FREQUENCY;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Maximum  = 999999.99;
  Parameters[VSD_HIGH_LIM_SPEED_MOTOR - VSD_BEGIN].Default  = 0.0;
  // Ограничение момента
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].ID         = VSD_TORQUE_LIMIT;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Physic     = PHYSIC_PERCENT;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Maximum    = 1000.0;
  Parameters[VSD_TORQUE_LIMIT - VSD_BEGIN].Default    = 0.0;
  // Ограничение момента в режиме генератора
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].ID         = VSD_TORQUE_LIMIT_GEN;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Physic     = PHYSIC_PERCENT;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Value      = 100.0;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Maximum    = 1000.0;
  Parameters[VSD_TORQUE_LIMIT_GEN - VSD_BEGIN].Default    = 100.0;
  // Предел тока
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].ID        = VSD_CURRENT_LIMIT;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Maximum   = 1000.0;
  Parameters[VSD_CURRENT_LIMIT - VSD_BEGIN].Default   = 1.0;
  // Максимальная выходная частота
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].ID         = VSD_MAX_OUTPUT_FREQUENCY;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Value      = 132.0;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Minimum    = 1.0;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Maximum    = 590.0;
  Parameters[VSD_MAX_OUTPUT_FREQUENCY - VSD_BEGIN].Default    = 132.0;
  // Реакция на ошибку обратной связи
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].ID       = VSD_MTR_FEEDBACK_LOSS_FUNC;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Value    = 2.0;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Maximum  = 11.0;
  Parameters[VSD_MTR_FEEDBACK_LOSS_FUNC - VSD_BEGIN].Default  = 2.0;
  // Реакция на ошибку задания частоты
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].ID         = VSD_TRACK_ERROR_FUNCTION;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Maximum    = 3.0;
  Parameters[VSD_TRACK_ERROR_FUNCTION - VSD_BEGIN].Default    = 0.0;
  // Величина несоответствия частота
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].ID        = VSD_TRACK_ERROR;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Physic    = PHYSIC_RPM;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Value     = 10.0;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Maximum   = 600.0;
  Parameters[VSD_TRACK_ERROR - VSD_BEGIN].Default   = 10.0;
  // Величина несоответствия частоты
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].ID        = VSD_TRACK_ERROR_TIMEOUT;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Maximum   = 60.0;
  Parameters[VSD_TRACK_ERROR_TIMEOUT - VSD_BEGIN].Default   = 1.0;
  // Величина несоответствия частоты при разгоне
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].ID        = VSD_TRACK_ERROR_RAMPING;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Physic    = PHYSIC_RPM;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Value     = 100.0;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Maximum   = 600.0;
  Parameters[VSD_TRACK_ERROR_RAMPING - VSD_BEGIN].Default   = 100.0;
  // Задержка срабатывания при несоответствии частоты при разгоне
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].ID        = VSD_TRACK_ERROR_RAMP_TIME;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Maximum   = 60.0;
  Parameters[VSD_TRACK_ERROR_RAMP_TIME - VSD_BEGIN].Default   = 1.0;
  // Tracking Error After Ramping Timeout
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].ID       = VSD_TRACK_ERROR_AFTER_RAMP;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Value    = 5.0;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Maximum  = 60.0;
  Parameters[VSD_TRACK_ERROR_AFTER_RAMP - VSD_BEGIN].Default  = 5.0;
  // Danfoss 4-50 Не используется в проекте; только при конфигурировании
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].ID        = VSD_WARNING_CURRENT_LOW;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Maximum   = 1000000.0;
  Parameters[VSD_WARNING_CURRENT_LOW - VSD_BEGIN].Default   = 0.0;
  // Danfoss 4-51 Не используется в проекте; только при конфигурировании
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].ID       = VSD_WARNING_CURRENT_HIGH;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Physic   = PHYSIC_CURRENT;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Maximum  = 1000000.0;
  Parameters[VSD_WARNING_CURRENT_HIGH - VSD_BEGIN].Default  = 0.0;
  // Минимальная скорость
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].ID        = VSD_WARNING_SPEED_LOW;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Physic    = PHYSIC_RPM;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Maximum   = 1000000.0;
  Parameters[VSD_WARNING_SPEED_LOW - VSD_BEGIN].Default   = 0.0;
  // Режим клеммы 27
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].ID         = VSD_TERMINAL_27_MODE;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Maximum    = 1.0;
  Parameters[VSD_TERMINAL_27_MODE - VSD_BEGIN].Default    = 0.0;
  // Режим клеммы 29
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].ID         = VSD_TERMINAL_29_MODE;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Maximum    = 1.0;
  Parameters[VSD_TERMINAL_29_MODE - VSD_BEGIN].Default    = 0.0;
  // Цифровой вход 18
  Parameters[VSD_DI_18 - VSD_BEGIN].ID        = VSD_DI_18;
  Parameters[VSD_DI_18 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DI_18 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DI_18 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DI_18 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DI_18 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DI_18 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DI_18 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DI_18 - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_DI_18 - VSD_BEGIN].Default   = 0.0;
  // Цифровой вход 19
  Parameters[VSD_DI_19 - VSD_BEGIN].ID        = VSD_DI_19;
  Parameters[VSD_DI_19 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DI_19 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DI_19 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DI_19 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DI_19 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DI_19 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DI_19 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DI_19 - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_DI_19 - VSD_BEGIN].Default   = 0.0;
  // Цифровой вход 27
  Parameters[VSD_DI_27 - VSD_BEGIN].ID        = VSD_DI_27;
  Parameters[VSD_DI_27 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DI_27 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DI_27 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DI_27 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DI_27 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DI_27 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DI_27 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DI_27 - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_DI_27 - VSD_BEGIN].Default   = 0.0;
  // Цифровой вход 32
  Parameters[VSD_DI_32 - VSD_BEGIN].ID        = VSD_DI_32;
  Parameters[VSD_DI_32 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DI_32 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DI_32 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DI_32 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DI_32 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DI_32 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DI_32 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DI_32 - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_DI_32 - VSD_BEGIN].Default   = 0.0;
  // Цифровой вход 33
  Parameters[VSD_DI_33 - VSD_BEGIN].ID        = VSD_DI_33;
  Parameters[VSD_DI_33 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DI_33 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DI_33 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DI_33 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DI_33 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DI_33 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DI_33 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DI_33 - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_DI_33 - VSD_BEGIN].Default   = 0.0;
  // Клемма 27 Цифровой вход
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].ID       = VSD_TERMINAL_27_DI;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Maximum  = 199.0;
  Parameters[VSD_TERMINAL_27_DI - VSD_BEGIN].Default  = 0.0;
  // Клемма 29 Цифровой вход
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].ID       = VSD_TERMINAL_29_DI;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Maximum  = 199.0;
  Parameters[VSD_TERMINAL_29_DI - VSD_BEGIN].Default  = 0.0;
  // Реле функций
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].ID        = VSD_FUNCTION_RELE;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Maximum   = 199.0;
  Parameters[VSD_FUNCTION_RELE - VSD_BEGIN].Default   = 0.0;
  // Нипонятно
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].ID       = VSD_ON_DELAY_RELAY;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Value    = 0.01;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Minimum  = 0.01;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Maximum  = 600.0;
  Parameters[VSD_ON_DELAY_RELAY - VSD_BEGIN].Default  = 0.01;
  // Клемма 42 аналоговый вход
  Parameters[VSD_42_AO - VSD_BEGIN].ID        = VSD_42_AO;
  Parameters[VSD_42_AO - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_42_AO - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_42_AO - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_42_AO - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_42_AO - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_42_AO - VSD_BEGIN].Value     = 133.0;
  Parameters[VSD_42_AO - VSD_BEGIN].Minimum   = 131.0;
  Parameters[VSD_42_AO - VSD_BEGIN].Maximum   = 150.0;
  Parameters[VSD_42_AO - VSD_BEGIN].Default   = 133.0;
  // Режим контроллера SL
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].ID       = VSD_SL_CONTROLLER_MODE;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Maximum  = 1.0;
  Parameters[VSD_SL_CONTROLLER_MODE - VSD_BEGIN].Default  = 0.0;
  // Событие запуска
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].ID       = VSD_SL_START_EVENT;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Maximum  = 101.0;
  Parameters[VSD_SL_START_EVENT - VSD_BEGIN].Default  = 0.0;
  // Событие останова
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].ID        = VSD_SL_STOP_EVENT;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_STOP_EVENT - VSD_BEGIN].Default   = 0.0;
  // Сброс настроек компараторов
  Parameters[VSD_SL_RESET - VSD_BEGIN].ID         = VSD_SL_RESET;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Maximum    = 1.0;
  Parameters[VSD_SL_RESET - VSD_BEGIN].Default    = 0.0;
  // Операнд сравнения компаратора
  Parameters[VSD_SL_10 - VSD_BEGIN].ID        = VSD_SL_10;
  Parameters[VSD_SL_10 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_10 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_10 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_10 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_10 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_10 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_10 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_10 - VSD_BEGIN].Maximum   = 199.0;
  Parameters[VSD_SL_10 - VSD_BEGIN].Default   = 0.0;
  // Оператор сравнения компаратора
  Parameters[VSD_SL_11 - VSD_BEGIN].ID        = VSD_SL_11;
  Parameters[VSD_SL_11 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_11 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_11 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_11 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_11 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_11 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_11 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_11 - VSD_BEGIN].Maximum   = 8.0;
  Parameters[VSD_SL_11 - VSD_BEGIN].Default   = 0.0;
  // Значение компаратора
  Parameters[VSD_SL_12 - VSD_BEGIN].ID        = VSD_SL_12;
  Parameters[VSD_SL_12 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_12 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_12 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_12 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_12 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_12 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_12 - VSD_BEGIN].Minimum   = -100000.0;
  Parameters[VSD_SL_12 - VSD_BEGIN].Maximum   = 100000.0;
  Parameters[VSD_SL_12 - VSD_BEGIN].Default   = 0.0;
  // RS-FF Operand S
  Parameters[VSD_SL_15 - VSD_BEGIN].ID        = VSD_SL_15;
  Parameters[VSD_SL_15 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_15 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_15 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_15 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_15 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_15 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_15 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_15 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_15 - VSD_BEGIN].Default   = 0.0;
  // RS-FF Operand R
  Parameters[VSD_SL_16 - VSD_BEGIN].ID        = VSD_SL_16;
  Parameters[VSD_SL_16 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_16 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_16 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_16 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_16 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_16 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_16 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_16 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_16 - VSD_BEGIN].Default   = 0.0;
  // Временная задержка
  Parameters[VSD_SL_20 - VSD_BEGIN].ID        = VSD_SL_20;
  Parameters[VSD_SL_20 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_20 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_20 - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_SL_20 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_20 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_20 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_20 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_20 - VSD_BEGIN].Maximum   = 1000000.0;
  Parameters[VSD_SL_20 - VSD_BEGIN].Default   = 0.0;
  // Булева переменная логического соотношения 1
  Parameters[VSD_SL_40 - VSD_BEGIN].ID        = VSD_SL_40;
  Parameters[VSD_SL_40 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_40 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_40 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_40 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_40 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_40 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_40 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_40 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_40 - VSD_BEGIN].Default   = 0.0;
  // Оператор логического соотношения 1
  Parameters[VSD_SL_41 - VSD_BEGIN].ID        = VSD_SL_41;
  Parameters[VSD_SL_41 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_41 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_41 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_41 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_41 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_41 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_41 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_41 - VSD_BEGIN].Maximum   = 8.0;
  Parameters[VSD_SL_41 - VSD_BEGIN].Default   = 0.0;
  // Булева переменная логического соотношения 2
  Parameters[VSD_SL_42 - VSD_BEGIN].ID        = VSD_SL_42;
  Parameters[VSD_SL_42 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_42 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_42 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_42 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_42 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_42 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_42 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_42 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_42 - VSD_BEGIN].Default   = 0.0;
  // Logic Rule Operator 2
  Parameters[VSD_SL_43 - VSD_BEGIN].ID        = VSD_SL_43;
  Parameters[VSD_SL_43 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_43 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_43 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_43 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_43 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_43 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_43 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_43 - VSD_BEGIN].Maximum   = 8.0;
  Parameters[VSD_SL_43 - VSD_BEGIN].Default   = 0.0;
  // Logic Rule Boolean 3
  Parameters[VSD_SL_44 - VSD_BEGIN].ID        = VSD_SL_44;
  Parameters[VSD_SL_44 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_44 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_44 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_44 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_44 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_44 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_44 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_44 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_44 - VSD_BEGIN].Default   = 0.0;
  // Событие контроллера SL
  Parameters[VSD_SL_51 - VSD_BEGIN].ID        = VSD_SL_51;
  Parameters[VSD_SL_51 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_51 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_51 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_51 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_51 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_51 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_51 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_51 - VSD_BEGIN].Maximum   = 101.0;
  Parameters[VSD_SL_51 - VSD_BEGIN].Default   = 0.0;
  // Действие контроллера SL
  Parameters[VSD_SL_52 - VSD_BEGIN].ID        = VSD_SL_52;
  Parameters[VSD_SL_52 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SL_52 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SL_52 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_SL_52 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SL_52 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SL_52 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_SL_52 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_SL_52 - VSD_BEGIN].Maximum   = 74.0;
  Parameters[VSD_SL_52 - VSD_BEGIN].Default   = 0.0;
  // Частота коммутации
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].ID        = VSD_SWITCHING_FREQUENCY;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Value     = 1000.0;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Minimum   = 1000.0;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Maximum   = 16000.0;
  Parameters[VSD_SWITCHING_FREQUENCY - VSD_BEGIN].Default   = 1000.0;
  // Сверхмодуляция
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].ID       = VSD_OVERMODULATION;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Value    = 1.0;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Maximum  = 1.0;
  Parameters[VSD_OVERMODULATION - VSD_BEGIN].Default  = 1.0;
  // Компенсация задержки
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].ID       = VSD_DEAD_TIME_COMPENSATION;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Maximum  = 1000000.0;
  Parameters[VSD_DEAD_TIME_COMPENSATION - VSD_BEGIN].Default  = 0.0;
  // Отказ питания
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].ID        = VSD_MAINS_FAILURE;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Maximum   = 7.0;
  Parameters[VSD_MAINS_FAILURE - VSD_BEGIN].Default   = 0.0;
  // Напряжение сети при отказе питания
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].ID        = VSD_MAINS_VOLTAGE_FAILURE;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Value     = 180.0;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Minimum   = 180.0;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Maximum   = 600.0;
  Parameters[VSD_MAINS_VOLTAGE_FAILURE - VSD_BEGIN].Default   = 180.0;
  // Режим сброса
  Parameters[VSD_RESET_MODE - VSD_BEGIN].ID       = VSD_RESET_MODE;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Maximum  = 14.0;
  Parameters[VSD_RESET_MODE - VSD_BEGIN].Default  = 0.0;
  // Время автоматического перезапуск
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].ID       = VSD_AUTOSTART_TIME;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Value    = 10.0;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Maximum  = 600.0;
  Parameters[VSD_AUTOSTART_TIME - VSD_BEGIN].Default  = 10.0;
  // Задержка перед выключение при пределе тока
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].ID        = VSD_DELAY_CURRENT_LIMIM;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Value     = 60.0;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Maximum   = 60.0;
  Parameters[VSD_DELAY_CURRENT_LIMIM - VSD_BEGIN].Default   = 60.0;
  // Задержка перед выключение при пределе моменте
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].ID         = VSD_DELAY_TORQUE_LIMIT;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Physic     = PHYSIC_TIME;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Value      = 60.0;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Maximum    = 60.0;
  Parameters[VSD_DELAY_TORQUE_LIMIT - VSD_BEGIN].Default    = 60.0;
  // Время фильтра регулятора тока
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].ID         = VSD_FIL_TIME_CURRENT_LIMIT;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Physic     = PHYSIC_TIME;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Value      = 1.0;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Minimum    = 1.000;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Maximum    = 2.0;
  Parameters[VSD_FIL_TIME_CURRENT_LIMIT - VSD_BEGIN].Default    = 1.0;
  // Косинус Фи
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].ID        = VSD_COS_PHI_MOTOR;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Value     = 0.95;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Minimum   = 0.4;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Maximum   = 0.95;
  Parameters[VSD_COS_PHI_MOTOR - VSD_BEGIN].Default   = 0.95;
  // Компенсация напряжения постоянного тока
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].ID        = VSD_DC_COMPENSATION;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Maximum   = 1.0;
  Parameters[VSD_DC_COMPENSATION - VSD_BEGIN].Default   = 1.0;
  // Управление вентилятором
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].ID        = VSD_FAN_CONTROL;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Maximum   = 4.0;
  Parameters[VSD_FAN_CONTROL - VSD_BEGIN].Default   = 0.0;
  // Выходной фильтр
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].ID         = VSD_OUT_FILTER;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Maximum    = 2.0;
  Parameters[VSD_OUT_FILTER - VSD_BEGIN].Default    = 0.0;
  // Сброс аварий
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].ID         = VSD_FAIL_RESET;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Maximum    = 3.0;
  Parameters[VSD_FAIL_RESET - VSD_BEGIN].Default    = 0.0;
  // Версия ПО ЧРП
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].ID       = VSD_SOFT_VERSION;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Operation= OPERATION_LIMITED;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Maximum  = 100000.0;
  Parameters[VSD_SOFT_VERSION - VSD_BEGIN].Default  = 0.0;
  // Командное слово
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].ID       = VSD_COMMAND_WORD;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Operation= OPERATION_LIMITED;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Maximum  = 65535.0;
  Parameters[VSD_COMMAND_WORD - VSD_BEGIN].Default  = 0.0;
  // Слово состояния
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].ID        = VSD_STATUS_WORD;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Operation = OPERATION_LIMITED;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Maximum   = 65535.0;
  Parameters[VSD_STATUS_WORD - VSD_BEGIN].Default   = 0.0;
  // Активная мощность
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].ID       = VSD_ACTIVE_POWER;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Physic   = PHYSIC_ACTIVE_POWER;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Maximum  = 10000.0;
  Parameters[VSD_ACTIVE_POWER - VSD_BEGIN].Default  = 0.0;
  // Напряжение двигателя
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].ID        = VSD_OUT_VOLTAGE_MOTOR;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Maximum   = 6000.0;
  Parameters[VSD_OUT_VOLTAGE_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Текущая частота двигателя
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].ID        = VSD_CURRENT_FREQUENCY;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Maximum   = 6500.0;
  Parameters[VSD_CURRENT_FREQUENCY - VSD_BEGIN].Default   = 0.0;
  // Выходной ток средний
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].ID        = VSD_OUT_CURRENT_MOTOR;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Maximum   = 10000.0;
  Parameters[VSD_OUT_CURRENT_MOTOR - VSD_BEGIN].Default   = 0.0;
  // Текущая скорость вращения двигателя
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].ID        = VSD_CURRENT_SPEED_RPM;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Physic    = PHYSIC_RPM;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Minimum   = -30000.0;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Maximum   = 30000.0;
  Parameters[VSD_CURRENT_SPEED_RPM - VSD_BEGIN].Default   = 0.0;
  // Напряжение цепи пост.тока
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].ID       = VSD_DC_VOLTAGE;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Physic   = PHYSIC_VOLTAGE;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Maximum  = 10000.0;
  Parameters[VSD_DC_VOLTAGE - VSD_BEGIN].Default  = 0.0;
  // Температура радиатора
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].ID       = VSD_RADIATOR_TEMPERATURE;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Physic   = PHYSIC_TEMPERATURE;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Maximum  = 255.0;
  Parameters[VSD_RADIATOR_TEMPERATURE - VSD_BEGIN].Default  = 0.0;
  // Температура платы управления
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].ID        = VSD_CONTROL_TEMPERATURE;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Physic    = PHYSIC_TEMPERATURE;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Maximum   = 100.0;
  Parameters[VSD_CONTROL_TEMPERATURE - VSD_BEGIN].Default   = 0.0;
  // Ток двигателя 1
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].ID        = VSD_OUT_CURRENT_PHASE_1;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Maximum   = 10000.0;
  Parameters[VSD_OUT_CURRENT_PHASE_1 - VSD_BEGIN].Default   = 0.0;
  // Ток двигателя 2
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].ID        = VSD_OUT_CURRENT_PHASE_2;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Maximum   = 10000.0;
  Parameters[VSD_OUT_CURRENT_PHASE_2 - VSD_BEGIN].Default   = 0.0;
  // Ток двигателя 3
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].ID        = VSD_OUT_CURRENT_PHASE_3;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Maximum   = 10000.0;
  Parameters[VSD_OUT_CURRENT_PHASE_3 - VSD_BEGIN].Default   = 0.0;
  // Цифровоый вход ПЧ
  Parameters[VSD_DI_VSD - VSD_BEGIN].ID         = VSD_DI_VSD;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Operation  = OPERATION_LIMITED;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Maximum    = 1023.0;
  Parameters[VSD_DI_VSD - VSD_BEGIN].Default    = 0.0;
  // Номер последней аварии и предупреждения
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].ID       = VSD_LAST_ALARM;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Maximum  = 65535.0;
  Parameters[VSD_LAST_ALARM - VSD_BEGIN].Default  = 0.0;
  // Слово Аварии 1
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].ID       = VSD_ALARM_WORD_1;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Maximum  = 4294967295.0;
  Parameters[VSD_ALARM_WORD_1 - VSD_BEGIN].Default  = 0.0;
  // Слово Аварии 2
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].ID       = VSD_ALARM_WORD_2;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Maximum  = 4294967295.0;
  Parameters[VSD_ALARM_WORD_2 - VSD_BEGIN].Default  = 0.0;
  // Слово предупреждения 1
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].ID       = VSD_WARNING_WORD_1;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Maximum  = 4294967295.0;
  Parameters[VSD_WARNING_WORD_1 - VSD_BEGIN].Default  = 0.0;
  // Слово предупреждения 2
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].ID       = VSD_WARNING_WORD_2;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Maximum  = 4294967295.0;
  Parameters[VSD_WARNING_WORD_2 - VSD_BEGIN].Default  = 0.0;
  // Время применения высокого пускового момента
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].ID       = VSD_HIGH_START_TORQUE_TIME;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Maximum  = 60.0;
  Parameters[VSD_HIGH_START_TORQUE_TIME - VSD_BEGIN].Default  = 0.0;
  // Пусковой ток в % от номинального
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].ID        = VSD_HIGH_START_TORQUE_CURRENT;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Physic    = PHYSIC_PERCENT;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Value     = 100.0;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Maximum   = 200.0;
  Parameters[VSD_HIGH_START_TORQUE_CURRENT - VSD_BEGIN].Default   = 100.0;
  // Защита блокировки ротора
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].ID        = VSD_LOCK_ROTOR_PROTECTION;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Maximum   = 1.0;
  Parameters[VSD_LOCK_ROTOR_PROTECTION - VSD_BEGIN].Default   = 0.0;
  // Время задержки аварии блокировки ротора
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].ID        = VSD_LOCK_ROTOR_TIME;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Minimum   = 0.05;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Maximum   = 1.0;
  Parameters[VSD_LOCK_ROTOR_TIME - VSD_BEGIN].Default   = 0.1;
  // Угол управления тиристорами; служебный регистр для отладки работы выпрямителя
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].ID       = VSD_THYR_ANGLE_REFERENCE;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Physic   = PHYSIC_ANGLE;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Maximum  = 180.0;
  Parameters[VSD_THYR_ANGLE_REFERENCE - VSD_BEGIN].Default  = 0.0;
  // Статус платы выпрямителя. Регистр для внутреннего использования в ПЧ
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].ID        = VSD_THYR_STATUS;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Maximum   = 253.0;
  Parameters[VSD_THYR_STATUS - VSD_BEGIN].Default   = 0.0;
  // Регистр управления тиристорами; используется только контроллером ПЧ
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].ID       = VSD_THYR_CONTROL;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Maximum  = 3.0;
  Parameters[VSD_THYR_CONTROL - VSD_BEGIN].Default  = 0.0;
  // Напряжение защиты от КЗ инвертора.
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].ID        = VSD_THYR_VOLT_SHORT_CUILT;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Maximum   = 100000.0;
  Parameters[VSD_THYR_VOLT_SHORT_CUILT - VSD_BEGIN].Default   = 0.0;
  // Маска состояния цифровых выходов.
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].ID         = VSD_DOUTPUTS;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Operation  = OPERATION_READ;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Maximum    = 65535.0;
  Parameters[VSD_DOUTPUTS - VSD_BEGIN].Default    = 0.0;
  // Маска состояния цифровых входов
  Parameters[VSD_DINPUTS - VSD_BEGIN].ID        = VSD_DINPUTS;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Maximum   = 65535.0;
  Parameters[VSD_DINPUTS - VSD_BEGIN].Default   = 0.0;
  // Маска включения цифровых выходов
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].ID        = VSD_DOUTPUTS_ENABLE;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Maximum   = 65535.0;
  Parameters[VSD_DOUTPUTS_ENABLE - VSD_BEGIN].Default   = 0.0;
  // Маска выключения цифровых выходов
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].ID       = VSD_DOUTPUTS_DISABLE;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Maximum  = 65535.0;
  Parameters[VSD_DOUTPUTS_DISABLE - VSD_BEGIN].Default  = 0.0;
  // Температура включения главного вентилятора.
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].ID        = VSD_VENT_ON_TEMPERATURE;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Physic    = PHYSIC_TEMPERATURE;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Maximum   = 120.0;
  Parameters[VSD_VENT_ON_TEMPERATURE - VSD_BEGIN].Default   = 0.0;
  // Температура выключения главного вентилятора.
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].ID       = VSD_VENT_OFF_TEMPERATURE;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Physic   = PHYSIC_TEMPERATURE;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Maximum  = 120.0;
  Parameters[VSD_VENT_OFF_TEMPERATURE - VSD_BEGIN].Default  = 0.0;
  // Период проверки температуры радиаторов
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].ID        = VSD_VENT_PERIOD;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Value     = 3.0;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Minimum   = 3.0;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Maximum   = 60.0;
  Parameters[VSD_VENT_PERIOD - VSD_BEGIN].Default   = 3.0;
  // Температура срабатывания тепловой защиты радиаторов
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].ID       = VSD_VENT_TEMPERATURE_FAULT;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Physic   = PHYSIC_TEMPERATURE;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Value    = 80.0;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Minimum  = 80.0;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Maximum  = 110.0;
  Parameters[VSD_VENT_TEMPERATURE_FAULT - VSD_BEGIN].Default  = 80.0;
  // Флаговый регистр управления ПЧ
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].ID       = VSD_INVERTOR_CONTROL;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Maximum  = 1023.0;
  Parameters[VSD_INVERTOR_CONTROL - VSD_BEGIN].Default  = 0.0;
  // Регистр состояния инвертора
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].ID = VSD_INVERTOR_STATUS;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Physic = PHYSIC_NUMERIC;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Maximum    = 65535.0;
  Parameters[VSD_INVERTOR_STATUS - VSD_BEGIN].Default    = 0.0;
  // Уставка частоты "тревожного" режима
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].ID = VSD_FREQUENCY_ERROR;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Physic = PHYSIC_FREQUENCY;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Value = 1.0;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Minimum    = 1.0;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Maximum    = 500.0;
  Parameters[VSD_FREQUENCY_ERROR - VSD_BEGIN].Default    = 1.0;
  // Темп нарастания частоты.
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].ID        = VSD_T_SPEEDUP;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Physic    = PHYSIC_TEMP;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Value     = 0.25;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Minimum   = 0.25;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Maximum   = 12.5;
  Parameters[VSD_T_SPEEDUP - VSD_BEGIN].Default   = 0.25;
  // Темп Период снижения частоты
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].ID        = VSD_T_SPEEDDOWN;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Physic    = PHYSIC_TEMP;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Value     = 0.25;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Minimum   = 0.25;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Maximum   = 12.5;
  Parameters[VSD_T_SPEEDDOWN - VSD_BEGIN].Default   = 0.25;
  // Период снижения частоты при токоограничении.
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].ID         = VSD_T_ILIMIT_SPEEDDOWN;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Physic     = PHYSIC_TEMP;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Value      = 0.25;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Minimum    = 0.25;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Maximum    = 12.5;
  Parameters[VSD_T_ILIMIT_SPEEDDOWN - VSD_BEGIN].Default    = 0.25;
  // Период снижения частоты при нехватке напряжения на инверторе
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].ID       = VSD_T_ULOW_SPEEDDOWN;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Physic   = PHYSIC_TEMP;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Value    = 0.25;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Minimum  = 0.25;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Maximum  = 12.5;
  Parameters[VSD_T_ULOW_SPEEDDOWN - VSD_BEGIN].Default  = 0.25;
  // Тип текущего алгоритма управления (регулятора)
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].ID        = VSD_CURRENT_REGULATOR;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_CURRENT_REGULATOR - VSD_BEGIN].Default   = -1.0;
  // Напряжение форсировки
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].ID       = VSD_UF_U_FORCE;
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].Physic   = PHYSIC_VOLTAGE;
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_UF_U_FORCE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Maximum  = 50.0;
  Parameters[VSD_UNIT_SPEED - VSD_BEGIN].Default  = 0.0;
  // Частота форсировки
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].ID       = VSD_UF_F_FORCE;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Physic   = PHYSIC_FREQUENCY;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Maximum  = 50.0;
  Parameters[VSD_UF_F_FORCE - VSD_BEGIN].Default  = 0.0;
  // Расчетное напряжение холостого хода
  Parameters[VSD_UF_UHH - VSD_BEGIN].ID       = VSD_UF_UHH;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Physic   = PHYSIC_VOLTAGE;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Maximum  = 50.0;
  Parameters[VSD_UF_UHH - VSD_BEGIN].Default  = 0.0;
  // Постоянная времени компенсации индуктивности
  Parameters[VSD_IFB_COMP - VSD_BEGIN].ID       = VSD_IFB_COMP;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Maximum  = 16.0;
  Parameters[VSD_IFB_COMP - VSD_BEGIN].Default  = 0.0;
  // Пропорциональный коэффициент ОС частотного контура регулирования
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].ID       = VSD_VUOUT_PROP;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Maximum  = 20.0;
  Parameters[VSD_VUOUT_PROP - VSD_BEGIN].Default  = 0.0;
  // Максимальный порог ограничения тока на выходе ПЧ; абс. вел
  Parameters[VSD_ILIMIT - VSD_BEGIN].ID       = VSD_ILIMIT;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Physic   = PHYSIC_CURRENT;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Value    = 8.25;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Minimum  = 8.25;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Maximum  = 707.52;
  Parameters[VSD_ILIMIT - VSD_BEGIN].Default  = 8.25;
  // Интегральный коэффициент ОС частотного контура регулирования
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].ID        = VSD_VUOUT_INTEG;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Maximum   = 10.0;
  Parameters[VSD_VUOUT_INTEG - VSD_BEGIN].Default   = 0.0;
  // Коэффициент постоянной времени обратного перехода с вектора на U/f
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].ID        = VSD_VTOUF_INTEG;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Maximum   = 14.0;
  Parameters[VSD_VTOUF_INTEG - VSD_BEGIN].Default   = 0.0;
  // Текущее выходное напряжение ПЧ
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].ID         = VSD_UOUT_RATED;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Operation  = OPERATION_READ;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Physic     = PHYSIC_VOLTAGE;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Value      = 9.48;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Minimum    = 9.48;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Maximum    = 660.0;
  Parameters[VSD_UOUT_RATED - VSD_BEGIN].Default    = 9.48;
  // Суммарная индуктивность на выходе ПЧ; фазное значение
  Parameters[VSD_LOUT - VSD_BEGIN].ID         = VSD_LOUT;
  Parameters[VSD_LOUT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_LOUT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_LOUT - VSD_BEGIN].Physic     = PHYSIC_INDUNCTANCE;
  Parameters[VSD_LOUT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_LOUT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_LOUT - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_LOUT - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_LOUT - VSD_BEGIN].Maximum    = 3906.0;
  Parameters[VSD_LOUT - VSD_BEGIN].Default    = 0.0;
  // Режим раскачки: частота
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].ID       = VSD_SW_STARTUP_FREQUENCY;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Physic   = PHYSIC_FREQUENCY;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Value    = 0.03;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Minimum  = 0.03;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Maximum  = 10.0;
  Parameters[VSD_SW_STARTUP_FREQUENCY - VSD_BEGIN].Default  = 0.03;
  // Режим раскачки: угол качания
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].ID         = VSD_SW_STARTUP_ANGLE_OSC;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Physic     = PHYSIC_ANGLE;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Value      = 2.0;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Minimum    = 2.0;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Maximum    = 359.0;
  Parameters[VSD_SW_STARTUP_ANGLE_OSC - VSD_BEGIN].Default    = 2.0;
  // Режим раскачки: угол сдвига
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].ID       = VSD_SW_STARTUP_ANGLE_SHIFT;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Operation= OPERATION_READ;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Value    = 1.0;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Minimum  = 1.0;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Maximum  = 179.0;
  Parameters[VSD_SW_STARTUP_ANGLE_SHIFT - VSD_BEGIN].Default  = 1.0;
  // Режим раскачки: количество качаний на угол
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].ID         = VSD_SW_STARTUP_OSC_COUNT;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Value      = 1.0;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Minimum    = 1.0;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Maximum    = 100.0;
  Parameters[VSD_SW_STARTUP_OSC_COUNT - VSD_BEGIN].Default    = 1.0;
  // Режим раскачки: количество оборотов двигателя
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].ID         = VSD_SW_STARTUP_ROTATIONS;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Value      = 1.0;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Minimum    = 1.0;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Maximum    = 100.0;
  Parameters[VSD_SW_STARTUP_ROTATIONS - VSD_BEGIN].Default    = 1.0;
  // Режим раскачки: кратность напряжения импульса расклинки
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].ID       = VSD_SW_STARTUP_U_PULSE;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Value    = 0.9375;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Minimum  = 0.9375;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Maximum  = 10.0;
  Parameters[VSD_SW_STARTUP_U_PULSE - VSD_BEGIN].Default  = 0.9375;
  // Режим раскачки: токоограничение
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].ID         = VSD_SW_STARTUP_I_LIM;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Physic     = PHYSIC_CURRENT;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Value      = 8.0;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Minimum    = 8.0;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Maximum    = 250.0;
  Parameters[VSD_SW_STARTUP_I_LIM - VSD_BEGIN].Default    = 8.0;
  // Интегральный коэффициент ОС фазового контура регулирования
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].ID        = VSD_VFREQ_INTEG;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Maximum   = 11.0;
  Parameters[VSD_VFREQ_INTEG - VSD_BEGIN].Default   = 1.0;
  // Регистры рабочей очереди алгоритмов управления
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].ID        = VSD_REGULATOR_QUEUE_1;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_REGULATOR_QUEUE_1 - VSD_BEGIN].Default   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].ID        = VSD_REGULATOR_QUEUE_2;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_REGULATOR_QUEUE_2 - VSD_BEGIN].Default   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].ID        = VSD_REGULATOR_QUEUE_3;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_REGULATOR_QUEUE_3 - VSD_BEGIN].Default   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].ID        = VSD_REGULATOR_QUEUE_4;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_REGULATOR_QUEUE_4 - VSD_BEGIN].Default   = -1.0;
  // Регистры рабочей очереди алгоритмов управления
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].ID        = VSD_REGULATOR_QUEUE_5;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Value     = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Minimum   = -1.0;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Maximum   = 6.0;
  Parameters[VSD_REGULATOR_QUEUE_5 - VSD_BEGIN].Default   = -1.0;
  // Порог защиты ПЧ по снижению напряжения на инверторе
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].ID         = VSD_UD_LOW_FAULT;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Physic     = PHYSIC_VOLTAGE;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Value      = 250.0;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Minimum    = 250.0;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Maximum    = 400.0;
  Parameters[VSD_UD_LOW_FAULT - VSD_BEGIN].Default    = 250.0;
  // Порог защиты ПЧ по превыщению напряжения на инверторе
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].ID        = VSD_UD_HIGH_FAULT;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Value     = 550.0;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Minimum   = 550.0;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Maximum   = 700.0;
  Parameters[VSD_UD_HIGH_FAULT - VSD_BEGIN].Default   = 550.0;
  // Порог защиты ПЧ по превыщению напряжения на инверторе
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].ID       = VSD_UIN_ASYM_LEVEL;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Physic   = PHYSIC_PERCENT;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Value    = 10.0;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Minimum  = 10.0;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Maximum  = 90.0;
  Parameters[VSD_UIN_ASYM_LEVEL - VSD_BEGIN].Default  = 10.0;
  // Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].ID       = VSD_UIN_ASYM_DELAY;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Physic   = PHYSIC_TIME;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Value    = 1.0;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Minimum  = 1.0;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Maximum  = 90.0;
  Parameters[VSD_UIN_ASYM_DELAY - VSD_BEGIN].Default  = 1.0;
  // Частота перехода на векторный алгоритм
  Parameters[VSD_F_VECT - VSD_BEGIN].ID         = VSD_F_VECT;
  Parameters[VSD_F_VECT - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_F_VECT - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_F_VECT - VSD_BEGIN].Physic     = PHYSIC_FREQUENCY;
  Parameters[VSD_F_VECT - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_F_VECT - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_F_VECT - VSD_BEGIN].Value      = 13.0;
  Parameters[VSD_F_VECT - VSD_BEGIN].Minimum    = 13.0;
  Parameters[VSD_F_VECT - VSD_BEGIN].Maximum    = 500.0;
  Parameters[VSD_F_VECT - VSD_BEGIN].Default    = 13.0;
  // Пропорциональный коэффициент ОС фазового контура регулирования
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].ID         = VSD_VFREQ_GAIN;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Maximum    = 17.0;
  Parameters[VSD_VFREQ_GAIN - VSD_BEGIN].Default    = 0.0;
  // Фазовый коэффициент усиления системы подавления резонанса
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].ID        = VSD_VANGLE_GAIN;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Maximum   = 15.0;
  Parameters[VSD_VANGLE_GAIN - VSD_BEGIN].Default   = 1.0;
  // Постоянная времени системы подавления резонанса
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].ID        = VSD_RES_TIMECONST;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Value     = 0.001;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Minimum   = 0.001;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Maximum   = 0.25;
  Parameters[VSD_RES_TIMECONST - VSD_BEGIN].Default   = 0.001;
  // Служебный регистр для отладки алгоритмов управления
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].ID       = VSD_RES_IAMP_VALID;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Access   = ACCESS_OPERATOR;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Operation= OPERATION_WRITE;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Physic   = PHYSIC_NUMERIC;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Update   = UPDATE_ERROR;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Value    = 0.0;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Minimum  = 0.0;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Maximum  = 100000.0;
  Parameters[VSD_RES_IAMP_VALID - VSD_BEGIN].Default  = 0.0;
  // Выходной ток ПЧ по фазе В. (амплитудное значение)
  Parameters[VSD_IB - VSD_BEGIN].ID         = VSD_IB;
  Parameters[VSD_IB - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_IB - VSD_BEGIN].Operation  = OPERATION_READ;
  Parameters[VSD_IB - VSD_BEGIN].Physic     = PHYSIC_CURRENT;
  Parameters[VSD_IB - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_IB - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_IB - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_IB - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_IB - VSD_BEGIN].Maximum    = 300.0;
  Parameters[VSD_IB - VSD_BEGIN].Default    = 0.0;
  // Фаза выходного тока В
  Parameters[VSD_PHB - VSD_BEGIN].ID        = VSD_PHB;
  Parameters[VSD_PHB - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_PHB - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_PHB - VSD_BEGIN].Physic    = PHYSIC_ANGLE;
  Parameters[VSD_PHB - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_PHB - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_PHB - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_PHB - VSD_BEGIN].Minimum   = -180.0;
  Parameters[VSD_PHB - VSD_BEGIN].Maximum   = 180.0;
  Parameters[VSD_PHB - VSD_BEGIN].Default   = 0.0;
  // Выходной ток ПЧ по фазе В. (амплитудное значение)
  Parameters[VSD_IC - VSD_BEGIN].ID         = VSD_IC;
  Parameters[VSD_IC - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_IC - VSD_BEGIN].Operation  = OPERATION_READ;
  Parameters[VSD_IC - VSD_BEGIN].Physic     = PHYSIC_CURRENT;
  Parameters[VSD_IC - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_IC - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_IC - VSD_BEGIN].Value      = 0.0;
  Parameters[VSD_IC - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_IC - VSD_BEGIN].Maximum    = 300.0;
  Parameters[VSD_IC - VSD_BEGIN].Default    = 0.0;
  // Фаза выходного тока В
  Parameters[VSD_PHC - VSD_BEGIN].ID        = VSD_PHC;
  Parameters[VSD_PHC - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_PHC - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_PHC - VSD_BEGIN].Physic    = PHYSIC_ANGLE;
  Parameters[VSD_PHC - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_PHC - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_PHC - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_PHC - VSD_BEGIN].Minimum   = -180.0;
  Parameters[VSD_PHC - VSD_BEGIN].Maximum   = 180.0;
  Parameters[VSD_PHC - VSD_BEGIN].Default   = 0.0;
  // Порог токоограничения импульса расклинки в режиме с раскачкой
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].ID        = VSD_SW_STARTUP_I_LIM_PULSE;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Physic    = PHYSIC_CURRENT;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Value     = 8.25;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Minimum   = 8.25;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Maximum   = 500.0;
  Parameters[VSD_SW_STARTUP_I_LIM_PULSE - VSD_BEGIN].Default   = 8.25;
  // Время снижения уставки в режиме остановки турбинного вращения
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].ID        = VSD_DECEL_SPEEDDOWN;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Physic    = PHYSIC_TIME;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Maximum   = 50.0;
  Parameters[VSD_DECEL_SPEEDDOWN - VSD_BEGIN].Default   = 1.0;
  // Напряжение остаточного напряжения на шине (окончания разряда)
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].ID        = VSD_DECEL_VOLTAGE;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Physic    = PHYSIC_VOLTAGE;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Value     = 9.8;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Minimum   = 9.8;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Maximum   = 49.4;
  Parameters[VSD_DECEL_VOLTAGE - VSD_BEGIN].Default   = 9.8;
  // Начальная частота режима остановки турбинного вращения (для АД)
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].ID        = VSD_DECEL_F_MAX;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Physic    = PHYSIC_FREQUENCY;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Value     = 1.0;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Minimum   = 1.0;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Maximum   = 500.0;
  Parameters[VSD_DECEL_F_MAX - VSD_BEGIN].Default   = 1.0;
  // Время секунды
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].ID        = VSD_TIME_SECOND;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Maximum   = 59.0;
  Parameters[VSD_TIME_SECOND - VSD_BEGIN].Default   = 0.0;
  // Время минуты
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].ID        = VSD_TIME_MINUTE;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Physic    = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Value     = 0.0;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Minimum   = 0.0;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Maximum   = 59.0;
  Parameters[VSD_TIME_MINUTE - VSD_BEGIN].Default   = 0.0;
  // Время часы
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].ID          = VSD_TIME_HOUR;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Access      = ACCESS_OPERATOR;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Operation   = OPERATION_WRITE;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Physic      = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Validity    = VALIDITY_ERROR;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Update      = UPDATE_ERROR;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Value       = 0.0;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Minimum     = 0.0;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Maximum     = 23.0;
  Parameters[VSD_TIME_HOUR - VSD_BEGIN].Default     = 0.0;
  // Время дни
  Parameters[VSD_TIME_DAY - VSD_BEGIN].ID           = VSD_TIME_DAY;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Access       = ACCESS_OPERATOR;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Operation    = OPERATION_WRITE;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Physic       = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Validity     = VALIDITY_ERROR;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Update       = UPDATE_ERROR;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Value        = 1.0;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Minimum      = 1.0;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Maximum      = 31.0;
  Parameters[VSD_TIME_DAY - VSD_BEGIN].Default      = 1.0;
  // Время месяцы
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].ID         = VSD_TIME_MONTH;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Access     = ACCESS_OPERATOR;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Operation  = OPERATION_WRITE;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Physic     = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Validity   = VALIDITY_ERROR;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Update     = UPDATE_ERROR;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Value      = 1.0;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Minimum    = 1.0;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Maximum    = 12.0;
  Parameters[VSD_TIME_MONTH - VSD_BEGIN].Default    = 1.0;
  // Время годы
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].ID          = VSD_TIME_YEAR;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Access      = ACCESS_OPERATOR;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Operation   = OPERATION_WRITE;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Physic      = PHYSIC_NUMERIC;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Validity    = VALIDITY_ERROR;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Update      = UPDATE_ERROR;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Value       = 2010.0;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Minimum     = 2010.0;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Maximum     = 2030.0;
  Parameters[VSD_TIME_YEAR - VSD_BEGIN].Default     = 2010.0;
  // Величина измерительного тока автонастройки
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].ID          = VSD_AST_IMEAS;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Access      = ACCESS_OPERATOR;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Operation   = OPERATION_WRITE;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Physic      = PHYSIC_PERCENT;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Validity    = VALIDITY_ERROR;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Update      = UPDATE_ERROR;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Value       = 20.0;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Minimum     = 20.0;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Maximum     = 100.0;
  Parameters[VSD_AST_IMEAS - VSD_BEGIN].Default     = 20.0;
  // Величина измерительного напряжения автонастройки
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].ID        = VSD_AST_UMEAS;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Access    = ACCESS_OPERATOR;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Physic    = PHYSIC_PERCENT;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Validity  = VALIDITY_ERROR;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Update    = UPDATE_ERROR;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Value     = 20.0;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Minimum   = 20.0;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Maximum   = 60.0;
  Parameters[VSD_AST_UMEAS - VSD_BEGIN].Default   = 20.0;
  // Измеренная величина выходной индуктивности автонастройки
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].ID = VSD_AST_LOUT_1_0;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Physic = PHYSIC_INDUNCTANCE;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Maximum    = 4.0;
  Parameters[VSD_AST_LOUT_1_0 - VSD_BEGIN].Default    = 0.0;
  // Дополнительный регистр автонастройки; зарезервирован
  Parameters[VSD_AST_STANDING - VSD_BEGIN].ID = VSD_AST_STANDING;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Physic = PHYSIC_INDUNCTANCE;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Maximum    = 4.0;
  Parameters[VSD_AST_STANDING - VSD_BEGIN].Default    = 0.0;
  // Расширенный регистр состояния инвертора
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].ID = VSD_INVERTOR_EXT_STATUS;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Physic = PHYSIC_NUMERIC;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Maximum    = 65535.0;
  Parameters[VSD_INVERTOR_EXT_STATUS - VSD_BEGIN].Default    = 0.0;
  // Период экскурсий частоты режима встряхивания.
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].ID = VSD_JARRING_PERIOD;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Physic = PHYSIC_TIME;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Minimum    = 60.0;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Maximum    = 3600.0;
  Parameters[VSD_JARRING_PERIOD - VSD_BEGIN].Default    = 0.0;
  // Величина прибавки частоты вращения выше уставки в режиме встряхивания
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].ID = VSD_JARRING_UPDFREQ;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Physic = PHYSIC_FREQUENCY;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Maximum    = 15.0;
  Parameters[VSD_JARRING_UPDFREQ - VSD_BEGIN].Default    = 0.0;
  // Время прибавки частоты вращения выше уставки в режиме встряхивания
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].ID = VSD_JARRING_UPTIME;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Physic = PHYSIC_TIME;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Maximum    = 50.0;
  Parameters[VSD_JARRING_UPTIME - VSD_BEGIN].Default    = 0.0;
  // Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].ID = VSD_JARRING_DOWNDFREQ;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Physic = PHYSIC_FREQUENCY;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Maximum    = 15.0;
  Parameters[VSD_JARRING_DOWNDFREQ - VSD_BEGIN].Default    = 0.0;
  // Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].ID = VSD_JARRING_DOWNTIME;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Physic = PHYSIC_TIME;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Maximum    = 50.0;
  Parameters[VSD_JARRING_DOWNTIME - VSD_BEGIN].Default    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].ID = VSD_JARRING_MODE;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Physic = PHYSIC_NUMERIC;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Maximum    = 100000.0;
  Parameters[VSD_JARRING_MODE - VSD_BEGIN].Default    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].ID = VSD_JARRING_TICK_CNT;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Physic = PHYSIC_NUMERIC;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Maximum    = 100000.0;
  Parameters[VSD_JARRING_TICK_CNT - VSD_BEGIN].Default    = 0.0;
  // Регистры обслуживания внутренних нужд режима встряхивания
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].ID = VSD_JARRING_SECOND_CNT;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Physic = PHYSIC_NUMERIC;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Maximum    = 100000.0;
  Parameters[VSD_JARRING_SECOND_CNT - VSD_BEGIN].Default    = 0.0;
  // Стартовая частота вращения двигателя в основном режиме регулирования
  Parameters[VSD_START_FREQ - VSD_BEGIN].ID = VSD_START_FREQ;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Operation = OPERATION_WRITE;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Physic = PHYSIC_FREQUENCY;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Value = 0.03;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Minimum    = 0.03;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Maximum    = 10.0;
  Parameters[VSD_START_FREQ - VSD_BEGIN].Default    = 0.03;
  // Выходной ток ПЧ по фазе A. (амплитудное значение)
  Parameters[VSD_IA - VSD_BEGIN].ID = VSD_IA;
  Parameters[VSD_IA - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_IA - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_IA - VSD_BEGIN].Physic = PHYSIC_CURRENT;
  Parameters[VSD_IA - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_IA - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_IA - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_IA - VSD_BEGIN].Minimum    = 0.0;
  Parameters[VSD_IA - VSD_BEGIN].Maximum    = 300.0;
  Parameters[VSD_IA - VSD_BEGIN].Default    = 0.0;
  // Фаза выходного тока A
  Parameters[VSD_PHA - VSD_BEGIN].ID = VSD_PHA;
  Parameters[VSD_PHA - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_PHA - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_PHA - VSD_BEGIN].Physic = PHYSIC_ANGLE;
  Parameters[VSD_PHA - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_PHA - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_PHA - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_PHA - VSD_BEGIN].Minimum    = -180.0;
  Parameters[VSD_PHA - VSD_BEGIN].Maximum    = 180.0;
  Parameters[VSD_PHA - VSD_BEGIN].Default    = 0.0;
  // Усредненная фаза выходного тока ПЧ
  Parameters[VSD_PHOUT - VSD_BEGIN].ID = VSD_PHOUT;
  Parameters[VSD_PHOUT - VSD_BEGIN].Access = ACCESS_OPERATOR;
  Parameters[VSD_PHOUT - VSD_BEGIN].Operation = OPERATION_READ;
  Parameters[VSD_PHOUT - VSD_BEGIN].Physic = PHYSIC_ANGLE;
  Parameters[VSD_PHOUT - VSD_BEGIN].Validity = VALIDITY_ERROR;
  Parameters[VSD_PHOUT - VSD_BEGIN].Update = UPDATE_ERROR;
  Parameters[VSD_PHOUT - VSD_BEGIN].Value = 0.0;
  Parameters[VSD_PHOUT - VSD_BEGIN].Minimum    = -180.0;
  Parameters[VSD_PHOUT - VSD_BEGIN].Maximum    = 180.0;
  Parameters[VSD_PHOUT - VSD_BEGIN].Default    = 0.0;
}




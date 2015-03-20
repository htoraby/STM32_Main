#ifndef PROTECTION_MAIN_H
#define PROTECTION_MAIN_H

#include "protection_overvoltage_input.h"
#include "protection_undervoltage_input.h"
#include "protection_imbalance_voltage_input.h"

#include "protection_overload_motor.h"
#include "protection_underload_motor.h"
#include "protection_imbalance_current_motor.h"
#include "protection_out_of_sync_motor.h"
#include "protection_turbine_rotation.h"

#include "protection_temperature_motor.h"
#include "protection_pressure_intake.h"
#include "protection_resistance_isolation.h"

extern ProtectionOverVoltageInput protOverVoltIn;
extern ProtectionUnderVoltageInput protUnderVoltIn;
extern ProtectionImbalanceVoltageInput protImbalanceVoltIn;

extern ProtectionOverloadMotor protOverloadMotor;
extern ProtectionUnderloadMotor protUnderloadMotor;
extern ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
extern ProtectionOutOfSyncMotor protOutOfSyncMotor;
extern ProtectionTurbineRotation protTurbineRotation;

extern ProtectionTemperatureMotor protTemperatureMotor;
extern ProtectionPressureIntake protPressureIntake;
extern ProtectionResistanceIsolation protResistanceIsolation;

/*!
 * \brief Функция инициализации задачи обработки всех защит
 */
void protectionInit();

/*!
 * \brief Метод проверки запрещающего параметра во всех защитах
 * \return
 */
bool isProtectionPrevent();

#endif // PROTECTION_MAIN_H


#ifndef PROTECTION_MAIN_H
#define PROTECTION_MAIN_H

#include "protection_overvoltage_input.h"
#include "protection_undervoltage_input.h"
#include "protection_imbalance_voltage_input.h"
#include "protection_imbalance_current_input.h"
#include "protection_power_off.h"

#include "protection_overload_motor.h"
#include "protection_underload_motor.h"
#include "protection_imbalance_current_motor.h"
#include "protection_out_of_sync_motor.h"
#include "protection_turbine_rotation.h"

#include "protection_temperature_motor.h"
#include "protection_pressure_intake.h"
#include "protection_resistance_isolation.h"

#include "protection_hardware_vsd.h"
#include "protection_lock_door.h"

#include "protection_digital_input_1.h"
#include "protection_digital_input_2.h"
#include "protection_digital_input_3.h"
#include "protection_digital_input_4.h"

#include "protection_analog_input_1.h"
#include "protection_analog_input_2.h"
#include "protection_analog_input_3.h"
#include "protection_analog_input_4.h"

extern ProtectionOverVoltageInput protOverVoltIn;
extern ProtectionUnderVoltageInput protUnderVoltIn;
extern ProtectionImbalanceVoltageInput protImbalanceVoltIn;
extern ProtectionImbalanceCurrentInput protImbalanceCurIn;
extern ProtectionPowerOff protPowerOff;

extern ProtectionOverloadMotor protOverloadMotor;
extern ProtectionUnderloadMotor protUnderloadMotor;
extern ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;
extern ProtectionOutOfSyncMotor protOutOfSyncMotor;
extern ProtectionTurbineRotation protTurbineRotation;

extern ProtectionTemperatureMotor protTemperatureMotor;
extern ProtectionPressureIntake protPressureIntake;
extern ProtectionResistanceIsolation protResistanceIsolation;

extern ProtectionHardwareVsd protHardwareVsd;
extern ProtectionLockDoor protLockDoor;

extern ProtectionDigitalInput1 protDigitalInput1;
extern ProtectionDigitalInput2 protDigitalInput2;
extern ProtectionDigitalInput3 protDigitalInput3;
extern ProtectionDigitalInput4 protDigitalInput4;

extern ProtectionAnalogInput1 protAnalogInput1;
extern ProtectionAnalogInput2 protAnalogInput2;
extern ProtectionAnalogInput3 protAnalogInput3;
extern ProtectionAnalogInput4 protAnalogInput4;

/*!
 * \brief Функция инициализации задачи обработки всех защит
 */
void protectionInit();

/*!
 * \brief Метод добавления события "АПВ запрещён"
 */
void addEventProtectionPrevent();

#endif // PROTECTION_MAIN_H


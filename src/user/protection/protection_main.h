#ifndef PROTECTION_MAIN_H
#define PROTECTION_MAIN_H

#include "protection_overvoltage_input.h"
#include "protection_undervoltage_input.h"

#include "protection_overload_motor.h"
#include "protection_underload_motor.h"
#include "protection_imbalance_current_motor.h"

extern ProtectionOverVoltageInput protOverVoltIn;
extern ProtectionUnderVoltageInput protUnderVoltIn;

extern ProtectionOverloadMotor protOverloadMotor;
extern ProtectionUnderloadMotor protUnderloadMotor;
extern ProtectionImbalanceCurrentMotor protImbalanceCurrentMotor;

/*!
 * \brief protectionInit
 * Функция инициализации автоматов защит
 */
void protectionInit();

#endif // PROTECTION_MAIN_H


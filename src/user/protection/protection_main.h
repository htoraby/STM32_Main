#ifndef PROTECTION_MAIN_H
#define PROTECTION_MAIN_H

#include "protection_overvoltage_input.h"
#include "protection_undervoltage_input.h"

extern ProtectionOverVoltageInput protOverVoltIn;
extern ProtectionUnderVoltageInput protUnderVoltIn;

/*!
 * \brief protectionInit
 * Функция инициализации автоматов защит
 */
void protectionInit();

#endif // PROTECTION_MAIN_H


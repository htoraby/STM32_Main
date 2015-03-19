#ifndef PROTECTIONUNDERVOLTAGEINPUT_H
#define PROTECTIONUNDERVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Снижение питания сети"
 */
class ProtectionUnderVoltageInput : public Protection
{
public:
  ProtectionUnderVoltageInput();
  ~ProtectionUnderVoltageInput();

private:

  bool checkAlarm();
  bool checkBlock();
  float calcValue();

};

#endif // PROTECTIONUNDERVOLTAGEINPUT_H

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

  void getOtherSetpointProt();

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();
};

#endif // PROTECTIONUNDERVOLTAGEINPUT_H

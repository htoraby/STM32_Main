#ifndef PROTECTIONIMBALANCEVOLTAGEINPUT_H
#define PROTECTIONIMBALANCEVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Дисбаланс напряжения"
 */
class ProtectionImbalanceVoltageInput : public Protection
{
public:
  ProtectionImbalanceVoltageInput();
  ~ProtectionImbalanceVoltageInput();

private:

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

};

#endif // PROTECTIONIMBALANCEVOLTAGEINPUT_H

#ifndef PROTECTIONRESISTANCEISOLATION_H
#define PROTECTIONRESISTANCEISOLATION_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Сопротивление изоляции"
 */
class ProtectionResistanceIsolation : public Protection
{
public:
  ProtectionResistanceIsolation();
  ~ProtectionResistanceIsolation();

private:

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

};

#endif // PROTECTIONRESISTANCEISOLATION_H

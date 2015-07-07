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
  void getOtherSetpointProt();

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONRESISTANCEISOLATION_H

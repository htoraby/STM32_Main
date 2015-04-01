#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Превышение питания сети"
 */
class ProtectionOverVoltageInput : public Protection
{
public:
  ProtectionOverVoltageInput();
  ~ProtectionOverVoltageInput();

private:
  void getOtherSetpointProt();

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

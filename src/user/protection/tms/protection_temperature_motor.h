#ifndef PROTECTIONTEMPERATUREMOTOR_H
#define PROTECTIONTEMPERATUREMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Перегрев двигателя"
 */
class ProtectionTemperatureMotor : public Protection
{
public:
  ProtectionTemperatureMotor();
  ~ProtectionTemperatureMotor();

private:

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONTEMPERATUREMOTOR_H

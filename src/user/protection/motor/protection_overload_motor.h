#ifndef PROTECTIONOVERLOADMOTOR_H
#define PROTECTIONOVERLOADMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Перегруз двигателя"
 *
 */
class ProtectionOverloadMotor : public Protection
{
public:
  ProtectionOverloadMotor();
  ~ProtectionOverloadMotor();

private:

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

};

#endif // PROTECTIONOVERLOADMOTOR_H

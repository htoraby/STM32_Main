#ifndef PROTECTIONUNDERLOADMOTOR_H
#define PROTECTIONUNDERLOADMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Недогруз двигателя"
 */
class ProtectionUnderloadMotor : public Protection
{
public:
  ProtectionUnderloadMotor();
  ~ProtectionUnderloadMotor();

private:

  bool checkAlarm();
  float calcValue();

};

#endif // PROTECTIONUNDERLOADMOTOR_H

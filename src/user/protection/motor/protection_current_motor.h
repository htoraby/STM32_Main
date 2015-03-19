#ifndef PROTECTIONCURRENTMOTOR_H
#define PROTECTIONCURRENTMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Предел тока двигателя"
 */
class ProtectionCurrentMotor : public Protection
{
public:
  ProtectionCurrentMotor();
  ~ProtectionCurrentMotor();

private:
  bool checkAlarm();

};

#endif // PROTECTIONCURRENTMOTOR_H

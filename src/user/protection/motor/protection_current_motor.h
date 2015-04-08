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
  void getOtherSetpointProt();

  bool checkAlarm();
  float calcValue();
};

#endif // PROTECTIONCURRENTMOTOR_H

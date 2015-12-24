#ifndef PROTECTIONOVERLOADMOTOR_H
#define PROTECTIONOVERLOADMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Перегруз двигателя"
 */
class ProtectionOverloadMotor : public Protection
{
public:
  ProtectionOverloadMotor();
  ~ProtectionOverloadMotor();

private:
  void getOtherSetpointProt();

  bool checkAlarm();
  float calcValue();
};

#endif // PROTECTIONOVERLOADMOTOR_H

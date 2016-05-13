#ifndef PROTECTIONVIBRATIONMOTOR_H
#define PROTECTIONVIBRATIONMOTOR_H

#include "protection.h"

/*!
 * \brief Класс защиты "Вибрация двигателя"
 */
class ProtectionVibrationMotor : public Protection
{
public:
  ProtectionVibrationMotor();
  ~ProtectionVibrationMotor();

private:
  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONVIBRATIONMOTOR_H

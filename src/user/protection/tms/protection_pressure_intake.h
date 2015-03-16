#ifndef PROTECTIONPRESSUREINTAKE_H
#define PROTECTIONPRESSUREINTAKE_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Давление на приёме насоса"
 */
class ProtectionPressureIntake : public Protection
{
public:
  ProtectionPressureIntake();
  ~ProtectionPressureIntake();

private:

  bool checkAlarm();
  bool checkBlock();
  float calcValue();

};

#endif // PROTECTIONPRESSUREINTAKE_H

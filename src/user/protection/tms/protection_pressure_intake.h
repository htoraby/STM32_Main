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
  void getOtherSetpointProt();

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

};

#endif // PROTECTIONPRESSUREINTAKE_H

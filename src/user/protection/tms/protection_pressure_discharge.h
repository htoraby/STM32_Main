#ifndef PROTECTIONPRESSUREDISCHARGE_H
#define PROTECTIONPRESSUREDISCHARGE_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Давление на выкиде насоса"
 */
class ProtectionPressureDischarge : public Protection
{
public:
  ProtectionPressureDischarge();
  ~ProtectionPressureDischarge();

private:
  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONPRESSUREDISCHARGE_H

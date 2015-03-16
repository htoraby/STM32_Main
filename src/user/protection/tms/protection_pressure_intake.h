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

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:

  bool checkAlarm();
  bool checkBlock();

};

#endif // PROTECTIONPRESSUREINTAKE_H

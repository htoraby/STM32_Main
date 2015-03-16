#ifndef PROTECTIONTEMPERATUREMOTOR_H
#define PROTECTIONTEMPERATUREMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Перегрев двигателя"
 */
class ProtectionTemperatureMotor : public Protection
{
public:
  ProtectionTemperatureMotor();
  ~ProtectionTemperatureMotor();

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:

  bool checkAlarm();
  bool checkBlock();
  float calcValue();

};

#endif // PROTECTIONTEMPERATUREMOTOR_H

#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Превышение питания сети"
 */
class ProtectionOverVoltageInput : public Protection
{
public:
  ProtectionOverVoltageInput();
  ~ProtectionOverVoltageInput();

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:

  bool checkAlarm();
  bool checkBlock();
  float calcValue();

};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

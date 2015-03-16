#ifndef PROTECTIONIMBALANCEVOLTAGEINPUT_H
#define PROTECTIONIMBALANCEVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Дисбаланс напряжения"
 */
class ProtectionImbalanceVoltageInput : public Protection
{
public:
  ProtectionImbalanceVoltageInput();
  ~ProtectionImbalanceVoltageInput();

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:

  bool checkAlarm();
  bool checkBlock();
  float calcValue();

};

#endif // PROTECTIONIMBALANCEVOLTAGEINPUT_H

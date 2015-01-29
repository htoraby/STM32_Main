#ifndef PROTECTIONUNDERVOLTAGEINPUT_H
#define PROTECTIONUNDERVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты от снижения входного питания
 *
 */
class ProtectionUnderVoltageInput : public Protection
{
public:
  ProtectionUnderVoltageInput();
  ~ProtectionUnderVoltageInput();

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

  /*!
   * \brief Метод проверки условия срабатывания защиты
   *
   * \return 0 параметр в норме, 1 параметр не в норме
   */
  bool checkTripSetPoint();

};

#endif // PROTECTIONUNDERVOLTAGEINPUT_H

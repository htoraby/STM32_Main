#ifndef PROTECTIONUNDERVOLTAGEINPUT_H
#define PROTECTIONUNDERVOLTAGEINPUT_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Снижение питания сети"
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
   * \return false параметр в норме, true параметр не в норме
   */
  bool checkAlarm();

  /*!
   * \brief Метод проверки выполнения условия АПВ
   *
   * \return false параметр в норме, true параметр не в норме
   */
  bool checkBlock();

};

#endif // PROTECTIONUNDERVOLTAGEINPUT_H

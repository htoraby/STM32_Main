#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"
#include "em.h"
#include "ccs.h"

/*!
 * \brief Класс автомата защиты "Превышение питания сети"
 *
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
  virtual bool checkBlock();

private:

  /*!
   * \brief Пересчёт текущего значения для сравнения с уставкой
   */
  float calcValue();

};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

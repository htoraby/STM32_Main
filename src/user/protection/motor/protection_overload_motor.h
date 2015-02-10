#ifndef PROTECTIONOVERLOADMOTOR_H
#define PROTECTIONOVERLOADMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Перегруз двигателя"
 *
 */
class ProtectionOverloadMotor : public Protection
{
public:
  ProtectionOverloadMotor();
  ~ProtectionOverloadMotor();

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
};

#endif // PROTECTIONOVERLOADMOTOR_H

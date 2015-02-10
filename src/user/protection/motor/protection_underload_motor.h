#ifndef PROTECTIONUNDERLOADMOTOR_H
#define PROTECTIONUNDERLOADMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Недогруз двигателя"
 *
 */
class ProtectionUnderloadMotor : public Protection
{
public:
  ProtectionUnderloadMotor();
  ~ProtectionUnderloadMotor();

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

#endif // PROTECTIONUNDERLOADMOTOR_H

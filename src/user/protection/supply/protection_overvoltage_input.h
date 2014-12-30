#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"
#include "em.h"

class ProtectionOvervoltageInput : public Protection
{
public:
  ProtectionOvervoltageInput();
  ~ProtectionOvervoltageInput();

  /*!
   * \brief calcControlParameter
   * Метод вычисления контролируемого параметра, свой для каждой защиты
   * Вычисляем среднее напряжение
   * \return
   */
  float calcControlParameter();

  unsigned char protection();
};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

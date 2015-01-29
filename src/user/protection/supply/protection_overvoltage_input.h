#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"
#include "em.h"
#include "ccs.h"

class ProtectionOverVoltageInput : public Protection
{
public:
  ProtectionOverVoltageInput();
  ~ProtectionOverVoltageInput();

  void init();

  /*!
   * \brief Метод проверки условия срабатывания защиты
   *
   * \return 0 параметр в норме, 1 параметр не в норме
   */
  bool checkTripSetPoint();

};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

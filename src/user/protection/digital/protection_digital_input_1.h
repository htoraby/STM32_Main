#ifndef PROTECTIONDIGITALINPUT1_H
#define PROTECTIONDIGITALINPUT1_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Цифровой вход 1"
 */
class ProtectionDigitalInput1 : public Protection
{
public:
  ProtectionDigitalInput1();
  ~ProtectionDigitalInput1();

private:
  bool isProtect();
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

};

#endif // PROTECTIONDIGITALINPUT1_H

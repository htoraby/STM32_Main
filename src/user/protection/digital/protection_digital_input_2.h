#ifndef PROTECTIONDIGITALINPUT2_H
#define PROTECTIONDIGITALINPUT2_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Цифровой вход 2"
 */
class ProtectionDigitalInput2 : public Protection
{
public:
  ProtectionDigitalInput2();
  ~ProtectionDigitalInput2();

private:
  bool isProtect();
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

};

#endif // PROTECTIONDIGITALINPUT2_H

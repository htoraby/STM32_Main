#ifndef PROTECTIONDIGITALINPUT3_H
#define PROTECTIONDIGITALINPUT3_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Цифровой вход 3"
 */
class ProtectionDigitalInput3 : public Protection
{
public:
  ProtectionDigitalInput3();
  ~ProtectionDigitalInput3();

private:
  bool isProtect();
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

};

#endif // PROTECTIONDIGITALINPUT3_H

#ifndef PROTECTIONDIGITALINPUT4_H
#define PROTECTIONDIGITALINPUT4_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Цифровой вход 4"
 */
class ProtectionDigitalInput4 : public Protection
{
public:
  ProtectionDigitalInput4();
  ~ProtectionDigitalInput4();

private:
  bool isProtect();
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

};

#endif // PROTECTIONDIGITALINPUT4_H

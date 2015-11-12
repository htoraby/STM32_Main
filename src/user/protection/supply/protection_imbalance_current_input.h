#ifndef PROTECTIONIMBALANCECURRENTINPUT_H
#define PROTECTIONIMBALANCECURRENTINPUT_H

#include "protection.h"

class ProtectionImbalanceCurrentInput : public Protection
{
public:
  ProtectionImbalanceCurrentInput();
  ~ProtectionImbalanceCurrentInput();

private:
  void getOtherSetpointProt();

  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONIMBALANCECURRENTINPUT_H

#ifndef PROTECTION_ANALOG_INPUT_4_H
#define PROTECTION_ANALOG_INPUT_4_H

#include "protection.h"

class ProtectionAnalogInput4 : public Protection
{
public:
  ProtectionAnalogInput4();
  ~ProtectionAnalogInput4();

private:
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

  float nominal_;

};

#endif // PROTECTION_ANALOG_INPUT_4_H

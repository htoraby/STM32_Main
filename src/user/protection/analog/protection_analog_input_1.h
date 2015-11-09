#ifndef PROTECTION_ANALOG_INPUT_1_H
#define PROTECTION_ANALOG_INPUT_1_H

#include "protection.h"

class ProtectionAnalogInput1 : public Protection
{
public:
  ProtectionAnalogInput1();
  ~ProtectionAnalogInput1();

private:
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

  float nominal_;

};

#endif // PROTECTION_ANALOG_INPUT_1_H

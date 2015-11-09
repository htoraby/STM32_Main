#ifndef PROTECTION_ANALOG_INPUT_2_H
#define PROTECTION_ANALOG_INPUT_2_H

#include "protection.h"

class ProtectionAnalogInput2 : public Protection
{
public:
  ProtectionAnalogInput2();
  ~ProtectionAnalogInput2();

private:
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

  float nominal_;

};

#endif // PROTECTION_ANALOG_INPUT_2_H

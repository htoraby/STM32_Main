#ifndef PROTECTION_ANALOG_INPUT_3_H
#define PROTECTION_ANALOG_INPUT_3_H

#include "protection.h"

class ProtectionAnalogInput3 : public Protection
{
public:
  ProtectionAnalogInput3();
  ~ProtectionAnalogInput3();

private:
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();

  float nominal_;

};

#endif // PROTECTION_ANALOG_INPUT_3_H

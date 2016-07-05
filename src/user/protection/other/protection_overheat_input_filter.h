#ifndef PROTECTIONOVERHEATINPUTFILTER_H
#define PROTECTIONOVERHEATINPUTFILTER_H

#include "protection.h"

class ProtectionOverheatInputFilter : public Protection
{
public:
  ProtectionOverheatInputFilter();
  ~ProtectionOverheatInputFilter();
private:
  bool checkAlarm();
  bool checkPrevent();
};

#endif // PROTECTIONOVERHEATINPUTFILTER_H

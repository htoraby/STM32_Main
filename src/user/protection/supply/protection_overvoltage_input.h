#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"

class ProtectionOvervoltageInput : public Protection
{
public:
  ProtectionOvervoltageInput();
  ~ProtectionOvervoltageInput();

  unsigned char protection();
};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

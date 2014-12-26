#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"
#include "em.h"

class ProtectionOvervoltageInput : public Protection
{
public:
  ProtectionOvervoltageInput();
  ~ProtectionOvervoltageInput();
  float calcControlParameter();
  unsigned char protection();
};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

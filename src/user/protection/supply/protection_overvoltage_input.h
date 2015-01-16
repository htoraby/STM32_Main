#ifndef PROTECTIONOVERVOLTAGEINPUT_H
#define PROTECTIONOVERVOLTAGEINPUT_H

#include "protection.h"
#include "em.h"
#include "ccs.h"

class ProtectionOvervoltageInput : public Protection
{
public:
  ProtectionOvervoltageInput();
  ~ProtectionOvervoltageInput();

  void init();

};

#endif // PROTECTIONOVERVOLTAGEINPUT_H

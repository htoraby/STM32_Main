#ifndef PROTECTIONLOCKDOOR_H
#define PROTECTIONLOCKDOOR_H

#include "protection.h"
#include "gpio.h"

class ProtectionLockDoor : public Protection
{
public:
  ProtectionLockDoor();
  ~ProtectionLockDoor();
private:
  bool checkAlarm();
  bool checkPrevent();
};

#endif // PROTECTIONLOCKDOOR_H

#ifndef PROTECTIONHARDWAREVSD_H
#define PROTECTIONHARDWAREVSD_H

#include "protection.h"

class ProtectionHardwareVsd : public Protection
{
public:
  ProtectionHardwareVsd();
  ~ProtectionHardwareVsd();
private:

  void setOtherParamProt();
  bool checkAlarm();
};

#endif // PROTECTIONHARDWAREVSD_H

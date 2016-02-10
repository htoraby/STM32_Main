#ifndef PROTECTIONPOWEROFF_H
#define PROTECTIONPOWEROFF_H

#include "protection.h"

class ProtectionPowerOff : public Protection
{
public:
  ProtectionPowerOff();
  ~ProtectionPowerOff();

private:
  void processing();
  void getCurrentParamProt();
  void setCurrentParamProt();
  bool checkAlarm();
  float calcValue();
  void addEventReactionProt();

  void processingStateRun();
  void proccessingStateStop();
  void automatProtection();

  bool isInit_;

};

#endif // PROTECTIONPOWEROFF_H

#ifndef REGIMERUNDIRECT_H
#define REGIMERUNDIRECT_H

#include "regime_run.h"

class RegimeRunDirect: public RegimeRun
{
public:
  RegimeRunDirect();
  virtual ~RegimeRunDirect();

  void getOtherSetpoint();
  void setOtherSetpoint();

  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();

private:

};

#endif // REGIMERUNDIRECT_H

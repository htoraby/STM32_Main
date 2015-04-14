#ifndef REGIMERUNSWING_H
#define REGIMERUNSWING_H

#include "regime_run.h"

class RegimeRunSwing : public RegimeRun
{
public:
  RegimeRunSwing();
  ~RegimeRunSwing();
private:
  void getOtherSetpoint();
  void setOtherSetpoint();

  float freq_;
  float quantity_;
  float voltage_;
};

#endif // REGIMERUNSWING_H

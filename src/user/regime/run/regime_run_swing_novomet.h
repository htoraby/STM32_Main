#ifndef REGIMERUNSWINGNOVOMET_H
#define REGIMERUNSWINGNOVOMET_H

#include "regime_run_swing.h"

class RegimeRunSwingNovomet : public RegimeRunSwing
{
public:
  RegimeRunSwingNovomet();
  ~RegimeRunSwingNovomet();

private:
  void processingStateRunning();
  void processingStateWork();
};

#endif // REGIMERUNSWINGNOVOMET_H

#ifndef REGIMERUNPUSHNOVOMET_H
#define REGIMERUNPUSHNOVOMET_H

#include "regime_run_push.h"

class RegimeRunPushNovomet : public RegimeRunPush
{
public:
  RegimeRunPushNovomet();
  ~RegimeRunPushNovomet();

  void processingStateRunning();
  void processingStateWork();
};

#endif // REGIMERUNPUSHNOVOMET_H

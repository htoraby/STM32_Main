#ifndef REGIMERUNPUSH_H
#define REGIMERUNPUSH_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "Толчковый"
 */
class RegimeRunPush: public RegimeRun
{
public:
  RegimeRunPush();
  ~RegimeRunPush();

private:
  void processing();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();

  void getOtherSetpoint();
  void setOtherSetpoint();

  float freq_;
  float time_;
  float quantity_;
  float period_;
  float voltage_;
};

#endif // REGIMERUNPUSH_H

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

  float freq_;
  float time_;
  float quantity_;
  float period_;
  float voltage_;

private:
  void getOtherSetPoint();
};

#endif // REGIMERUNPUSH_H

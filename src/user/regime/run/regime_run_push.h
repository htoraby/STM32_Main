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

  void processing();


};

#endif // REGIMERUNPUSH_H

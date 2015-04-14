#ifndef REGIMERUNVSDNOVOMET_H
#define REGIMERUNVSDNOVOMET_H

#include "regime_run.h"

/*!
 * \brief Класс режимов пуска аппаратно поддерживаемых в ЧРП Новомет
 */
class RegimeRunVsdNovomet : public RegimeRun
{
public:
  RegimeRunVsdNovomet();
  ~RegimeRunVsdNovomet();

  void processing();
};

#endif // REGIMERUNVSDNOVOMET_H

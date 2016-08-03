#ifndef REGIMERUNPICKUP_H
#define REGIMERUNPICKUP_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "Подхват"
 */
class RegimeRunPickup: public RegimeRun
{
public:
  RegimeRunPickup();
  virtual ~RegimeRunPickup();

  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();

private:
  void saveBeforeRegimeRun();
  void onRegime();
  bool checkOnRegime();
  void offRegime();
  bool checkOffRegime();

  float delay_;
};

#endif // REGIMERUNPICKUP_H

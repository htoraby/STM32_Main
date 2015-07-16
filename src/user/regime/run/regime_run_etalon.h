#ifndef REGIMERUNETALON_H
#define REGIMERUNETALON_H

#include "regime.h"

/*!
 * \brief Единый класс для всех режимов запуска реализованных в ЧРП Эталон
 */
class RegimeRunEtalon : public Regime
{
public:
  RegimeRunEtalon();
  virtual ~RegimeRunEtalon();

  void processing();
  void automatRegime();
  void processingStateWork();
  void processingStateRunning();
  void processingStateIdle();
  void getOtherSetpoint();
  void getGeneralSetpoint();
  void setOtherSetpoint();
  void setGeneralSetPoint();

  LastReasonRun runReason_;                 //!< Причина последней попытки запуска
};

#endif // REGIMERUNETALON_H

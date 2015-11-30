#ifndef REGIMERUNPUSHDANFOSS_H
#define REGIMERUNPUSHDANFOSS_H

#include "regime.h"

/*!
 * \brief The RegimeRunPushDanfoss class
 * Класс реализующий толчковый режим запуска ЧРП Danfoss
 */
class RegimeRunPushDanfoss : public Regime
{
public:
  RegimeRunPushDanfoss();
  virtual ~RegimeRunPushDanfoss();

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

#endif // REGIMERUNPUSHDANFOSS_H

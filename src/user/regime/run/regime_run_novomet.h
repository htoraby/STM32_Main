#ifndef REGIMERUNNOVOMET_H
#define REGIMERUNNOVOMET_H

#include "regime.h"

/*!
 * \brief Единый класс для всех режимов запуска реализованных в ЧРП Новомет
 */
class RegimeRunNovomet : public Regime
{
public:
  RegimeRunNovomet();
  virtual ~RegimeRunNovomet();

  void processing();

private:
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

#endif // REGIMERUNNOVOMET_H

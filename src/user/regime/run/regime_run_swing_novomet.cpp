#include "regime_run_swing_novomet.h"

RegimeRunSwingNovomet::RegimeRunSwingNovomet()
{

}

RegimeRunSwingNovomet::~RegimeRunSwingNovomet()
{

}

void RegimeRunSwingNovomet::processingStateRunning()
{
//  vsd->onRegimeSwing();
  ksu.start(runReason_);
  state_ = WorkState;
}

void RegimeRunSwingNovomet::processingStateWork()
{
  if ((parameters.get(VSD_CURRENT_REGULATOR > 2)) ||
     (vsd->getCurrentFreq() == vsd->getSetpointFreq())){        // Если режим отработал
    if (action_ == SingleAction) {                              // Если режим отрабатывает один раз
      parameters.set(CCS_RGM_RUN_SWING_MODE, OffAction);         // Выключаем режим
//      vsd->offRegimeSwing();                                    // Выключаем режим в ЧРП
      logEvent.add(SetpointCode, AutoType, RegimeRunSwingOffId); // Записываем данные в лог
    }
    state_ = IdleState;
  }
}


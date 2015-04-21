#include "regime_run_push_novomet.h"

RegimeRunPushNovomet::RegimeRunPushNovomet()
{

}

RegimeRunPushNovomet::~RegimeRunPushNovomet()
{

}

void RegimeRunPushNovomet::processingStateRunning()
{
  vsd->onRegimePush();
  ksu.start(runReason_);
  state_ = WorkState;
}

void RegimeRunPushNovomet::processingStateWork()
{
  if ((parameters.get(VSD_CURRENT_REGULATOR > 2)) ||
     (vsd->getCurrentFreq() == vsd->getSetpointFreq())){        // Если режим отработал
    if (action_ == SingleAction) {                              // Если режим отрабатывает один раз
      parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
      // vsd->offRegimePush();                                     // Выключаем режим в ЧРП
      logEvent.add(SetpointCode, AutoType, RegimeRunPushOffId); // Записываем данные в лог
    }
    state_ = IdleState;
  }
}

#include "regime_run_etalon.h"

RegimeRunEtalon::RegimeRunEtalon()
{

}

RegimeRunEtalon::~RegimeRunEtalon()
{

}

void RegimeRunEtalon::getGeneralSetpoint()
{
  runReason_= (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
}

void RegimeRunEtalon::setOtherSetpoint()
{

}

void RegimeRunEtalon::setGeneralSetPoint()
{
  parameters.set(CCS_RGM_RUN_VSD_STATE, state_);
}

void RegimeRunEtalon::getOtherSetpoint()
{
  action_ = (parameters.get(CCS_RGM_RUN_PICKUP_MODE) ||
             parameters.get(CCS_RGM_RUN_PUSH_MODE) ||
             parameters.get(CCS_RGM_RUN_SWING_MODE));
  state_ = parameters.get(CCS_RGM_RUN_VSD_STATE);
}

void RegimeRunEtalon::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunEtalon::processingStateRunning()
{
  ksu.start(runReason_);
  state_ = WorkState;
}

void RegimeRunEtalon::processingStateWork()
{
  if (ksu.getValue(CCS_CONDITION) != CCS_CONDITION_STOP) {
    if (vsd->getCurrentFreq() == vsd->getSetpointFreq()) {         // Если режимы отработали
      if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_PICKUP_MODE, OffAction);         // Выключаем режим
        vsd->offRegimePickup();
        logEvent.add(SetpointCode, AutoType, RegimeRunPickupOffId); // Записываем данные в лог
      }
      if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
        vsd->offRegimePush();
        logEvent.add(SetpointCode, AutoType, RegimeRunPushOffId); // Записываем данные в лог
      }
      if (parameters.get(CCS_RGM_RUN_SWING_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_SWING_MODE, OffAction);         // Выключаем режим
        vsd->offRegimeSwing();
        logEvent.add(SetpointCode, AutoType, RegimeRunSwingOffId); // Записываем данные в лог
      }
      state_ = IdleState;
    }
  }
  else {
    state_ = IdleState;
  }
}


void RegimeRunEtalon::processing()
{
  getGeneralSetpoint();
  getOtherSetpoint();
  automatRegime();
  setGeneralSetPoint();
  setOtherSetpoint();
}

void RegimeRunEtalon::automatRegime()
{
  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }
  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
    processingStateWork();
    break;
  default:
    break;
  }
}


#include "regime_run.h"

RegimeRun::RegimeRun()
{

}

RegimeRun::~RegimeRun()
{

}

void RegimeRun::getGeneralSetpoint()
{
  runReason_= (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
}

void RegimeRun::getOtherSetpoint()
{

}

void RegimeRun::setGeneralSetPoint()
{

}

void RegimeRun::setOtherSetpoint()
{

}

void RegimeRun::processingStateIdle()
{
  if (action_ != OffAction) {                                 // Режим - включен
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRun::processingStateRunning()
{
  state_ = IdleState;
}

void RegimeRun::processingStateWork()
{
  state_ = IdleState;
}

void RegimeRun::automatRegime()
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

void RegimeRun::processing()
{
  getGeneralSetpoint();
  getOtherSetpoint();
  automatRegime();
  setGeneralSetPoint();
  setOtherSetpoint();
}

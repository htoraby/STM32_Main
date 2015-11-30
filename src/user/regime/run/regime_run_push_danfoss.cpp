#include "regime_run_push_danfoss.h"

RegimeRunPushDanfoss::RegimeRunPushDanfoss()
{

}

RegimeRunPushDanfoss::~RegimeRunPushDanfoss()
{

}

void RegimeRunPushDanfoss::processing()
{

}

void RegimeRunPushDanfoss::automatRegime()
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

void RegimeRunPushDanfoss::processingStateWork()
{

}

void RegimeRunPushDanfoss::processingStateRunning()
{
  ksu.start(runReason_);
  state_ = WorkState;
}

void RegimeRunPushDanfoss::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunPushDanfoss::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);
  state_  = parameters.get(CCS_RGM_RUN_PUSH_STATE);

}

void RegimeRunPushDanfoss::getGeneralSetpoint()
{
  runReason_= (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
}

void RegimeRunPushDanfoss::setOtherSetpoint()
{

}

void RegimeRunPushDanfoss::setGeneralSetPoint()
{
  parameters.set(CCS_RGM_RUN_PUSH_STATE, state_);
}


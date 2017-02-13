#include "regime_run_direct.h"

RegimeRunDirect::RegimeRunDirect()
{

}

RegimeRunDirect::~RegimeRunDirect()
{

}

void RegimeRunDirect::getOtherSetpoint()
{
  // Уставки режима
  action_ = parameters.get(CCS_RGM_RUN_DIRECT_MODE);

  // Текущие параметры режима
  state_ = parameters.get(CCS_RGM_RUN_DIRECT_STATE);
}

void RegimeRunDirect::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_DIRECT_STATE, state_);
}

void RegimeRunDirect::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunDirect::processingStateRunning()
{
  int16_t err = 0;
  switch (state_) {
  case RunningState:
    err = parameters.set(CCS_BYPASS_CONTACTOR_KM1_CONTROL, 1);
    if (!err) {
      ksu.start(runReason_, true);
      logEvent.add(OtherCode, AutoType, RegimeRunPushStartId);
      state_ = WorkState;
    }
    break;
  }
}

void RegimeRunDirect::processingStateWork()
{
  // После пуска здесь
}

void RegimeRunDirect::processingStateStop()
{
  state_ = IdleState;
}

void RegimeRunDirect::automatRegime()
{
  if ((action_ == OffAction) && (state_ != IdleState)) {
    state_ = StopState;
  }
  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
    if (ksu.isStopMotor()) {
      state_ = StopState;
      break;
    }
    processingStateWork();
    break;
  case StopState:
    processingStateStop();
    break;
  default:
    state_ = StopState;
    break;
  }
}


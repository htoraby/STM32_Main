#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::processing()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);

  LastReasonRun runReason = (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);

  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    if (action_ != OffAction) { // Режим - включен
      if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
        if (runReason != LastReasonRunNone) { // Попытка пуска
          state_ = RestartState;
        }
      }
    }
    break;
  case RestartState:
    vsd->onRegimePush();
    ksu.start(runReason);
    state_ = WorkState;
    break;
  case WorkState:
//    if (parameters.get(CCS_IREG_CUR) == ) {
      if (action_ == SingleAction) {
        parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);
        logEvent.add(SetpointCode, AutoType, RegimeRunPushOffId);
      }
      state_ = IdleState;
//    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_CHANGE_FREQ_STATE, state_);
}

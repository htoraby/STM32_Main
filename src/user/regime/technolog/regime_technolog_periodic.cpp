#include "regime_technolog_periodic.h"

RegimeTechnologPeriodic::RegimeTechnologPeriodic()
{

}

RegimeTechnologPeriodic::~RegimeTechnologPeriodic()
{

}

void RegimeTechnologPeriodic::processing()
{
  action_ = parameters.getValue(CCS_RGM_PERIODIC_MODE);
  state_ = parameters.getValue(CCS_RGM_PERIODIC_STATE);

  workPeriod_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_PERIOD);
  stopPeriod_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_PERIOD);
  workBeginTime_ = parameters.getValueUint32(CCS_RGM_PERIODIC_RUN_BEGIN_TIME);
  stopBeginTime_ = parameters.getValueUint32(CCS_RGM_PERIODIC_STOP_BEGIN_TIME);
  workTimeToEnd_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
  stopTimeToEnd_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_TIME_TO_END);

  if (action_ == offAction) { // Режим - выключен
    state_ = IdleState;
  }

  switch (state_) {
    case IdleState:
      workBeginTime_ = ksu.getTime();
      stopBeginTime_ = ksu.getTime();
      workTimeToEnd_ = 0;
      stopTimeToEnd_ = 0;
      if (action_ != offAction) {                       // Режим - включен
        if (ksu.isWorkMotor() && ksu.isProgramMode()) { // Двигатель - работа; Режим - программа;
          state_ = WorkState;
        }
      }
      break;
    case WorkState:
      if (ksu.isWorkMotor()) {       // Двигатель - работа;
        uint32_t time = ksu.getSecFromCurTime(workBeginTime_);
        workTimeToEnd_ = getTimeToEnd(workPeriod_, time);
        if (workTimeToEnd_ == 0) {
          if (ksu.isProgramMode()) { // Режим - программа;
            ksu.stop(LastReasonStopProgram);
            state_ = StartPauseState;
          }
        }
      } else {
        // TODO: Станция в останове

      }
      break;
    case StartPauseState:
      if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) { // Двигатель - останов;
        stopBeginTime_ = ksu.getTime();
        state_ = PauseState;
      }
      break;
    case PauseState:
      if (ksu.isStopMotor()) {     // Двигатель - останов;
        uint32_t time = ksu.getSecFromCurTime(stopBeginTime_);
        stopTimeToEnd_ = getTimeToEnd(stopPeriod_, time);
        if (ksu.isProgramMode()) { // Режим - программа;
          if (stopTimeToEnd_ == 0) {
            ksu.start(LastReasonRunProgram);
            state_ = RestartState;
          }
        } else {
          // TODO: Режим программы отключен

        }
      } else {
        state_ = IdleState;
      }
      break;
    case RestartState:
      if (ksu.isProgramMode()) { // Режим - программа;
        if (ksu.isWorkMotor()) { // Двигатель - работа;
          workBeginTime_ = ksu.getTime();
          state_ = WorkState;
        }
      } else {
        state_ = PauseState;
      }
      break;
    case StopState:

      break;
    default:
      state_ = IdleState;
      break;
  }

  parameters.setValue(CCS_RGM_PERIODIC_STATE, state_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_PERIOD, workPeriod_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_PERIOD, stopPeriod_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_BEGIN_TIME, workBeginTime_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_BEGIN_TIME, stopBeginTime_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END, workTimeToEnd_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_TIME_TO_END, stopTimeToEnd_);
}

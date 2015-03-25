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
  workTimer_ = parameters.getValueUint32(CCS_RGM_PERIODIC_RUN_TIMER);
  stopTimer_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_TIMER);
  workTime_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME);
  workTimeEnd_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME_END);
  stopTime_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_TIME);
  stopTimeEnd_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_TIME_END);

  switch (state_) {
    case IdleState:
      workTimer_ = getTime();
      workTimeEnd_ = 0;
      if (action_ == onAction) {                        // Режим - включен
        if (ksu.isWorkMotor() && ksu.isProgramMode()) { // Двигатель - работа; Режим - программа;
          state_ = WorkState;
        }
      }
      break;
    case WorkState:

      break;
    case StartPauseState:

      break;
    case PauseState:

      break;
    case RestartState:

      break;
    case StopState:

      break;
    default:
      state_ = IdleState;
      break;
  }
}

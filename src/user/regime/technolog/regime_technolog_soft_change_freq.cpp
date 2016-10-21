#include "regime_technolog_soft_change_freq.h"

RegimeTechnologSoftChangeFreq::RegimeTechnologSoftChangeFreq()
{
  state_ = IdleState;
}

RegimeTechnologSoftChangeFreq::~RegimeTechnologSoftChangeFreq()
{

}

void RegimeTechnologSoftChangeFreq::processing()
{
  action_ = parameters.get(CCS_RGM_CHANGE_FREQ_MODE);
  state_ = parameters.get(CCS_RGM_CHANGE_FREQ_STATE);

  beginFreq_ = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ);
  endFreq_ = parameters.get(CCS_RGM_CHANGE_FREQ_END_FREQ);
  timeout_ = parameters.getU32(CCS_RGM_CHANGE_FREQ_BEGIN_TIME);
  period_one_step_ = parameters.get(CCS_RGM_CHANGE_FREQ_PERIOD_ONE_STEP);

  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    if (action_ != OffAction) { // Режим - включен
      if ((((parameters.get(CCS_CONDITION) == CCS_CONDITION_RUNNING) &&
            (parameters.get(VSD_FREQUENCY_NOW) >= beginFreq_)) ||
           (parameters.get(CCS_CONDITION) == CCS_CONDITION_RUN)) && ksu.isAutoMode()) {
        ksu.setFreq(beginFreq_);
        timeout_ = period_one_step_*10;
        state_ = WorkState;
      }
    }
    break;
  case WorkState:
    if (ksu.isWorkMotor() && ksu.isAutoMode()) { // Двигатель - работа; Режим - авто;
      if (timeout_ != 0)
        timeout_--;
      if ((timeout_ <= 0) && period_one_step_ && vsd->isConnect()) {
        timeout_ = period_one_step_*10;
        float freq = parameters.get(VSD_FREQUENCY) + copySign(0.1, endFreq_ - beginFreq_);
        float sign = copySign(1, endFreq_ - beginFreq_);
        if (freq * sign < endFreq_ * sign + 0.05) {
          ksu.setFreq(freq);
        }
        else {
          if (action_ == SingleAction) {
            parameters.set(CCS_RGM_CHANGE_FREQ_MODE, OffAction);
            logEvent.add(SetpointCode, AutoType, RegimeSoftChangeFreqOffId);
          }
        }
      }
    }
    else { // Станция в останове
      state_ = IdleState;
    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_CHANGE_FREQ_STATE, state_);
  parameters.set(CCS_RGM_CHANGE_FREQ_BEGIN_TIME, timeout_);
}

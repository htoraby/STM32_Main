#include "regime_technolog_soft_change_freq.h"

RegimeTechnologSoftChangeFreq::RegimeTechnologSoftChangeFreq()
{

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
  beginTime_ = parameters.getU32(CCS_RGM_CHANGE_FREQ_RUN_TIMER_FREQ);
  period_ = parameters.get(CCS_RGM_CHANGE_FREQ_TIMER_FREQ);

  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    if (action_ != OffAction) { // Режим - включен
      if (ksu.isWorkMotor() && ksu.isAutoMode()) { // Двигатель - работа; Режим - авто;
        vsd->setFrequency(beginFreq_);
        beginTime_ = ksu.getTime();
        state_ = WorkState;
      }
    }
    break;
  case WorkState:
    if (ksu.isWorkMotor() && ksu.isAutoMode()) { // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);
      if ((time > period_) && period_) {
        beginTime_ = ksu.getTime();
        float freq = parameters.get(VSD_FREQUENCY) + copySign(0.1, endFreq_ - beginFreq_);
        float sign = copySign(1, endFreq_ - beginFreq_);
        if (freq * sign < endFreq_ * sign + 0.05) {
          vsd->setFrequency(freq);
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
      if (action_ == SingleAction) {
        parameters.set(CCS_RGM_CHANGE_FREQ_MODE, OffAction);
        logEvent.add(SetpointCode, AutoType, RegimeSoftChangeFreqOffId);
      }
      state_ = IdleState;
    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_CHANGE_FREQ_STATE, state_);
  parameters.set(CCS_RGM_CHANGE_FREQ_RUN_TIMER_FREQ, beginTime_);
}

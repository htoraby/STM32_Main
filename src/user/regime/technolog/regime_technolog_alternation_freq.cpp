#include "regime_technolog_alternation_freq.h"

RegimeTechnologAlternationFreq::RegimeTechnologAlternationFreq()
{

}

RegimeTechnologAlternationFreq::~RegimeTechnologAlternationFreq()
{

}

void RegimeTechnologAlternationFreq::processing()
{
  // Получение уставок режима
  action_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE);
  firstFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_1);
  secondFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_2);
  timeFirstFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_TIMER_1);
  timeSecondFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_TIMER_2);

  // Получение текущих параметров режима
  state_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_STATE);
  beginTime_ = parameters.getU32(CCS_RGM_ALTERNATION_FREQ_TIMER_1_END);

  switch (state_) {
  case IdleState:
    if (action_ != OffAction) {                       // Режим - включен
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {    // Двигатель - работа; Режим - авто;
        vsd->setFrequency(firstFreq_);                // Устанавливаем первую частоту
        beginTime_ = ksu.getTime();                   // Запоминаем время когда перешли на частоту
        state_ = RunningState;                        // Переходим на работу на первой частоте
      }
    }
    break;
  case RunningState:
    //TODO: Задержка для перехода на новую частоту
    state_ = WorkState;
    break;
  case WorkState:
    if (ksu.isWorkMotor() && ksu.isAutoMode()) { // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);
      if ((time > timeFirstFreq_) && timeFirstFreq_) {
        vsd->setFrequency(secondFreq_);
        beginTime_ = ksu.getTime();
        state_ = WorkState + 1;
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

  }
}


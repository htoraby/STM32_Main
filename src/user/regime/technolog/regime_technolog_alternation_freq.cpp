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

  if (action_ == OffAction) {
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    if (action_ != OffAction) {                       // Режим - включен
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {    // Двигатель - работа; Режим - авто;
        ksu.setFreq(firstFreq_);                // Устанавливаем первую частоту
        beginTime_ = ksu.getTime();                   // Запоминаем время когда перешли на частоту
        state_ = RunningState;                        // Переходим на работу на первой частоте
      }
    }
    break;
  case RunningState:
    state_ = WorkState;                               // TODO: Задержка для перехода на новую частоту
    break;
  case WorkState:                                     // Работа на первой частоте
    if (ksu.isWorkMotor() && ksu.isAutoMode()) {      // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);        // Вычисляем сколько времени работаем на первой частоте
      if ((time > timeFirstFreq_) && timeFirstFreq_) {// Если время работы на первой частоте вышло
        ksu.setFreq(secondFreq_);               // Посылаем команду на изменение частоты
        beginTime_ = ksu.getTime();
        state_ = RunningState + 1;
      }
    }
    else {
      state_ = IdleState;
    }
    break;
  case RunningState + 1: 
    state_ = WorkState + 1;                           // TODO: Задержка для перехода на новую частоту
    break;
  case WorkState + 1:
    if (ksu.isWorkMotor() && ksu.isAutoMode()) {      // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);
      if ((time > timeSecondFreq_) && timeSecondFreq_) {
        ksu.setFreq(firstFreq_);
        beginTime_ = ksu.getTime();
        state_ = RunningState;
      }
    }
    else {
      state_ = IdleState;
    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_ALTERNATION_FREQ_STATE, state_);
  parameters.set(CCS_RGM_ALTERNATION_FREQ_TIMER_1_END, beginTime_);
}


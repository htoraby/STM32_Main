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
      if ((((parameters.get(CCS_CONDITION) == CCS_CONDITION_RUN) &&
            (parameters.get(VSD_FREQUENCY_NOW) >= parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_1))) ||
           (parameters.get(CCS_CONDITION) == CCS_CONDITION_WORK)) && ksu.isAutoMode()) {
        ksu.setFreq(firstFreq_);                      // Устанавливаем первую частоту
        beginTime_ = ksu.getTime();                   // Запоминаем время когда перешли на частоту
        state_ = RunningState;                        // Переходим на работу на первой частоте
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Alter Freq: Idle -> F1 (firstFreq = %6.2f state = %d)", firstFreq_, state_);
#endif
      }
    }
    break;
  case RunningState:
    state_ = WorkState;                               // TODO: Задержка для перехода на новую частоту
    break;
  case WorkState:                                     // Работа на первой частоте
    if (ksu.isRunOrWorkMotor() && ksu.isAutoMode()) {      // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);        // Вычисляем сколько времени работаем на первой частоте
      if ((time > timeFirstFreq_) && timeFirstFreq_) {// Если время работы на первой частоте вышло
        ksu.setFreq(secondFreq_);               // Посылаем команду на изменение частоты
        beginTime_ = ksu.getTime();
        state_ = RunningState + 1;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Alter Freq: F1 -> F2 (firstFreq = %6.2f, secondFreq = %6.2f, state = %d)", firstFreq_, secondFreq_, state_);
#endif
      }
    }
    else {
      state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Alter Freq: F1 -> Idle (firstFreq = %6.2f, state = %d)", firstFreq_, state_);
#endif
    }
    break;
  case RunningState + 1:
    state_ = WorkState + 1;                           // TODO: Задержка для перехода на новую частоту
    break;
  case WorkState + 1:
    if (ksu.isRunOrWorkMotor() && ksu.isAutoMode()) {      // Двигатель - работа; Режим - авто;
      uint32_t time = ksu.getSecFromCurTime(beginTime_);
      if ((time > timeSecondFreq_) && timeSecondFreq_) {
        ksu.setFreq(firstFreq_);
        beginTime_ = ksu.getTime();
        state_ = RunningState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Alter Freq: F2 -> F1 (firstFreq = %6.2f, secondFreq = %6.2f, state = %d)", firstFreq_, secondFreq_, state_);
#endif
      }
    }
    else {
      state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Alter Freq: F2 -> Idle (secondFreq = %6.2f, state = %d)", secondFreq_, state_);
#endif
    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_ALTERNATION_FREQ_STATE, state_);
  parameters.set(CCS_RGM_ALTERNATION_FREQ_TIMER_1_END, beginTime_);
}


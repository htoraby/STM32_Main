#include "regime_technolog_jarring.h"

RegimeTechnologJarring::RegimeTechnologJarring()
{

}

RegimeTechnologJarring::~RegimeTechnologJarring()
{

}

void RegimeTechnologJarring::processing()
{
  // Получение уставок режима
  action_ = parameters.get(CCS_RGM_JARRING_MODE);
  firstFreq_ = parameters.get(CCS_RGM_JARRING_FREQ_1);
  secondFreq_ = parameters.get(CCS_RGM_JARRING_FREQ_2);
  periodJar_ = parameters.get(CCS_RGM_JARRING_PERIOD);
  countJar_ = parameters.get(CCS_RGM_JARRING_COUNT);
  upTemp_ = parameters.get(CCS_RGM_JARRING_UPTEMP);
  downTemp_ = parameters.get(CCS_RGM_JARRING_DOWNTEMP);

  // Получение текущих параметров режима
  state_ = parameters.get(CCS_RGM_JARRING_STATE);
  beginTime_ = parameters.get(CCS_RGM_JARRING_TIMER);

  if (action_ == offAction) {
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:                           // Состояние Idle
    if (action_ != OffAction) {             // Режим включен
      if (((parameters.get(CCS_CONDITION) == CCS_CONDITION_RUNNING) ||
           (parameters.get(CCS_CONDITION) == CCS_CONDITION_RUN)) &&
          ksu.isAutoMode()) {               // Мы в работе
        beginTime_ = ksu.getTime();         // Запоминаем время перехода в работу
        state = RunningState;               // Переходим в состояние отсчёта периода встряхивания
#if (USE_LOG_DEBUG == 1)
        logDebug.add((DebugMsg, "Встряхив.: запуск таймера периода встряхивания");
#endif
      }
    }
    break;
  case RunningState:                        // Состояние отсчёта периода встряхивания
    if (ksu.isWorkMotor() && ksu.isAutoMode()) {
      uint32_t time = ksu.getSecFromCurTime(beginTime_);
      if ((time > periodJar_) && periodJar_) {
        state_ = WorkState;
      }
    }
    else {
      state = IdleState;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Встряхив.: выключение режима в ожидании");
#endif
    }
    break;
  case WorkState:
    if (ksu.isWorkMotor() && ksu.isAutoMode()) {
      // Задать темп
      // Задать частоту
      state_ = WorkState + 1;
    }
    else {
      state = IdleState;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Встряхив.: выключение режима в работе ");
#endif
    }
    break;
  case WorkState + 1:

    break;
  case WorkState + 2:

    break;
  case WorkState + 3:
  }
}


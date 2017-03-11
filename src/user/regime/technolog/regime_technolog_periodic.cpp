#include "regime_technolog_periodic.h"
#include "protection_main.h"

RegimeTechnologPeriodic::RegimeTechnologPeriodic()
  : attempt_(false)
  , addTime_(0)
{

}

RegimeTechnologPeriodic::~RegimeTechnologPeriodic()
{

}

void RegimeTechnologPeriodic::processing()
{
  action_ = parameters.get(CCS_RGM_PERIODIC_MODE);
  state_ = parameters.get(CCS_RGM_PERIODIC_STATE);

  workPeriod_ = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
  stopPeriod_ = parameters.get(CCS_RGM_PERIODIC_STOP_PERIOD);
  workBeginTime_ = parameters.getU32(CCS_RGM_PERIODIC_RUN_BEGIN_TIME);
  stopBeginTime_ = parameters.getU32(CCS_RGM_PERIODIC_STOP_BEGIN_TIME);
  workTimeToEnd_ = parameters.get(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
  stopTimeToEnd_ = parameters.get(CCS_RGM_PERIODIC_STOP_TIME_TO_END);

  LastReasonRun runReason = (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
  LastReasonStop stopReason = (LastReasonStop)parameters.get(CCS_LAST_STOP_REASON);

  if (action_ == OffAction) { // Режим - выключен
    if ((state_ == PauseState) || (state_ == RestartState) ||
        (state_ == WaitPauseState)) {
      ksu.resetRestart();
    }
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    workBeginTime_ = ksu.getTime();
    stopBeginTime_ = ksu.getTime();
    workTimeToEnd_ = 0;
    stopTimeToEnd_ = 0;
    addTime_ = 0;
    if (action_ != OffAction) {                       // Режим - включен
      if (ksu.isWorkMotor() && ksu.isProgramMode()) { // Двигатель - работа; Режим - программа;
        state_ = WorkState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Start-up mode (state_ = %d)", state_);
#endif
      }
    }
    break;
  case WorkState:
    if (ksu.getValue(CCS_CONDITION) != CCS_CONDITION_STOP) { // Станция в работе;
      uint32_t time = ksu.getSecFromCurTime(workBeginTime_);
      workTimeToEnd_ = getTimeToEnd(workPeriod_, time);
      stopTimeToEnd_ = 0;
      if (workTimeToEnd_ == 0) {   // Время работы истекло
        if (ksu.isProgramMode()) { // Режим - программа;
          ksu.setRestart();
          ksu.stop(LastReasonStopProgram);
          state_ = WaitPauseState;
#if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Transition to pause from work (workPeriod = %d, time = %d, state = %d,)",
                       workPeriod_, time, state_);
#endif
        }
      }
    }
    else { // Станция в останове
      // Если причина останова "Исключение"
      if ((stopReason == LastReasonStopOperator) ||
          (stopReason == LastReasonStopRemote) ||
          (stopReason == LastReasonStopUnderVoltIn) ||
          (stopReason == LastReasonStopOverVoltIn) ||
          (stopReason == LastReasonStopImbalanceVoltIn) ||
          (stopReason == LastReasonStopNoVoltage)) {
        if (runReason != LastReasonRunNone) { // Попытка пуска
          if (ksu.isProgramMode()) { // Режим - программа;
            uint32_t stopBeginTime = parameters.getU32(CCS_LAST_STOP_DATE_TIME);        // Время остановки двигателя
            uint32_t workEndTime = workBeginTime_ + workPeriod_;                         // Предпологаемое время останова по программе
            uint32_t workTimeToEnd = workEndTime - stopBeginTime;                        // Время доработки по программе
            uint32_t stopTime = ksu.getSecFromCurTime(stopBeginTime);                    // Время от останова до пуска
            workTimeToEnd = workTimeToEnd + stopTime;                                    // Время доработки
            if (workTimeToEnd < (30 * 60)) { // Если время доработки меньше 30 минут
              ksu.setRestart();
              stopBeginTime_ = parameters.getU32(CCS_LAST_STOP_DATE_TIME); // Время перехода в паузу фиксируем как время остановки двигателя
              parameters.set(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
              state_ = PauseState;
#if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop user or network and time of revision < half an hour (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                           workPeriod_, workTimeToEnd, stopReason, state_);
#endif
            }
            else {
              if (workTimeToEnd < workPeriod_) {
                workBeginTime_ = ksu.getSecFromCurTime(workPeriod_ - workTimeToEnd);
#if (USE_LOG_DEBUG == 1)
                logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop user or network and time of revision < setpoint (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                             workPeriod_, workTimeToEnd, stopReason, state_);
#endif
              } else {
                workBeginTime_ = ksu.getTime();
#if (USE_LOG_DEBUG == 1)
                logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop user or network and time improvements > setpoint (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                             workPeriod_, workTimeToEnd_, stopReason, state_);
#endif
              }
              state_ = RunningState;
#if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop user or network and transition to work (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                           workPeriod_, workTimeToEnd_, stopReason, state_);
#endif
            }
          }
          else {
            ksu.start(runReason);
            state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop user or network, and turn off program (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                         workPeriod_, workTimeToEnd_, stopReason, state_);
#endif
          }
        }
      }
      else {
        state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Stop during operation (workPeriod = %d, workTimeToEnd = %d, stopReason = %d, state = %d)",
                     workPeriod_, workTimeToEnd_, stopReason, state_);
#endif
      }
    }
    break;
  case RunningState:
    ksu.start(runReason);
    state_ = WorkState;
    break;
  case WaitPauseState:
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) { // Двигатель - останов;
      stopBeginTime_ = ksu.getTime();
      state_ = PauseState;
    }
    break;
  case PauseState:
    if (ksu.isStopMotor()) {       // Двигатель - останов;
      uint32_t time = ksu.getSecFromCurTime(stopBeginTime_); // Прошедшее время с начала останова
      stopTimeToEnd_ = getTimeToEnd(stopPeriod_ + addTime_, time);
      workTimeToEnd_ = 0;
      if (!protPowerOff.isRestart()) {
        parameters.set(CCS_RESTART_TIMER, stopTimeToEnd_);
        if (ksu.isProgramMode()) {   // Режим - программа;
          if (runReason == LastReasonRunOperator) { // Попытка пуска оператором
            workBeginTime_ = ksu.getTime();
            state_ = RunningState;
#if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() During pause, launch operator (stopPeriod_ = %d, stopTimeToEnd_ = %d, runReason = %d, state = %d)",
                         stopPeriod_, stopTimeToEnd_, runReason, state_);
#endif
          } else {
            if (stopTimeToEnd_ == 0) { // Время паузы истекло
              state_ = RestartState;
#if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Pause time has expired (stopPeriod_ = %d, stopTimeToEnd_ = %d, state = %d)",
                           stopPeriod_, stopTimeToEnd_, state_);
#endif
            }
          }
        }
        else { // Режим программы отключен
          state_ = StopState;
#if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() During pause off (state = %d))", state_);
#endif
        }
      }
    } else {
      state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() During pause proved to be (state = %d))", state_);
#endif
    }

    break;
  case RestartState:
    if (ksu.isProgramMode()) { // Режим - программа;
      if (ksu.isStopMotor()) {
        if (ksu.isPrevent()) {
          if (!attempt_) {                // Первая попытка запуска
            attempt_ = true;
            ksu.start(LastReasonRunProgram);
          }
        } else {
          addTime_ = 0;
          attempt_ = false;
          ksu.start(LastReasonRunProgram);
          state_ = RestartState;
#if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Not good trying to start from pause (state = %d))", state_);
#endif
        }
      } else {
        workBeginTime_ = ksu.getTime();
        state_ = WorkState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeTechnologPeriodic::processing() Transition to job from pause (state = %d))", state_);
#endif
      }
    }
    else {
      state_ = PauseState;
    }
    break;
  case StopState:
    if (ksu.isStopMotor()) {     // Двигатель - останов;
      uint32_t time = ksu.getSecFromCurTime(stopBeginTime_);
      stopTimeToEnd_ = getTimeToEnd(stopPeriod_, time);
      parameters.set(CCS_RESTART_TIMER, stopTimeToEnd_);
      workTimeToEnd_ = 0;
      if (ksu.isProgramMode()) { // Режим - программа;
        addTime_ = parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY) - stopTimeToEnd_;
        if (addTime_ < 0) {
          addTime_ = 0;
        }
        else {
          if (int(time - stopPeriod_) > 0)
            addTime_ = addTime_ + (time - stopPeriod_);
        }
        ksu.setRestart();
        state_ = PauseState;
      }
    } else {
      state_ = IdleState;
    }
    break;

  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_PERIODIC_STATE, state_);
  parameters.set(CCS_RGM_PERIODIC_RUN_BEGIN_TIME, workBeginTime_);
  parameters.set(CCS_RGM_PERIODIC_STOP_BEGIN_TIME, stopBeginTime_);
  parameters.set(CCS_RGM_PERIODIC_RUN_TIME_TO_END, workTimeToEnd_);
  parameters.set(CCS_RGM_PERIODIC_STOP_TIME_TO_END, stopTimeToEnd_);
}

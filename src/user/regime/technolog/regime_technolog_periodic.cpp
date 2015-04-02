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
  action_ = parameters.getValue(CCS_RGM_PERIODIC_MODE);
  state_ = parameters.getValue(CCS_RGM_PERIODIC_STATE);

  workPeriod_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_PERIOD);
  stopPeriod_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_PERIOD);
  workBeginTime_ = parameters.getValueUint32(CCS_RGM_PERIODIC_RUN_BEGIN_TIME);
  stopBeginTime_ = parameters.getValueUint32(CCS_RGM_PERIODIC_STOP_BEGIN_TIME);
  workTimeToEnd_ = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
  stopTimeToEnd_ = parameters.getValue(CCS_RGM_PERIODIC_STOP_TIME_TO_END);

  LastReasonRun runReason = (LastReasonRun)parameters.getValue(CCS_LAST_RUN_REASON_TMP);
  LastReasonStop stopReason = (LastReasonStop)parameters.getValue(CCS_LAST_STOP_REASON);

  if (action_ == OffAction) { // Режим - выключен
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
        }
      }
      break;
    case WorkState:
      if (ksu.getValue(CCS_CONDITION) != CCS_CONDITION_STOP) { // Станция в работе;
        uint32_t time = ksu.getSecFromCurTime(workBeginTime_);
        workTimeToEnd_ = getTimeToEnd(workPeriod_, time);
        if (workTimeToEnd_ == 0) {   // Время работы истекло
          if (ksu.isProgramMode()) { // Режим - программа;
            ksu.stop(LastReasonStopProgram);
            state_ = WaitPauseState;
          }
        }
      }
      else { // Станция в останове
        if (runReason != parameters.getValue(CCS_LAST_RUN_REASON)) { // Попытка пуска
          // Если причина останова "Исключение"
          if ((stopReason == LastReasonStopOperator) ||
              (stopReason == LastReasonStopRemote) ||
              (stopReason == LastReasonStopUndervoltage) ||
              (stopReason == LastReasonStopOvervoltage) ||
              (stopReason == LastReasonStopImbalanceVoltage) ||
              (stopReason == LastReasonStopNoVoltage)) {
            if (ksu.isProgramMode()) { // Режим - программа;
              uint32_t stopBeginTime = parameters.getValueUint32(CCS_LAST_STOP_DATE_TIME); // Время остановки двигателя
              uint32_t workEndTime = workBeginTime_ + workPeriod_;                         // Предпологаемое время останова по программе
              uint32_t workTimeToEnd = workEndTime - stopBeginTime;                        // Время доработки по программе
              uint32_t stopTime = ksu.getSecFromCurTime(stopBeginTime);                    // Время от останова до пуска
              workTimeToEnd = workTimeToEnd + stopTime;                                    // Время доработки
              if (workTimeToEnd < (30 * 60)) { // Если время доработки меньше 30 минут
                stopBeginTime_ = parameters.getValueUint32(CCS_LAST_STOP_DATE_TIME); // Время перехода в паузу фиксируем как время остановки двигателя
                state_ = PauseState;
              }
              else {
                if (workTimeToEnd < workPeriod_) {
                  workBeginTime_ = ksu.getSecFromCurTime(workPeriod_ - workTimeToEnd);
                } else {
                  workBeginTime_ = ksu.getTime();
                }
                state_ = RunningState;
              }
            }
            else {
              state_ = IdleState;
            }
          }
          else {
            if (ksu.isProgramMode()) {
              workBeginTime_ = ksu.getTime();
              state_ = RunningState;
            }
            else {
             state_ = IdleState;
            }
          }
        }
      }
      break;
    case RunningState:
      if (ksu.isPrevent()) {
        if (!attempt_) {                // Первая попытка запуска
          attempt_ = true;
          addEventProtectionPrevent();  // Сообщение неудачной попытке пуска
        }
      } else {
        ksu.start(runReason);
        attempt_ = false;
        state_ = WorkState;
      }
      break;
    case WaitPauseState:
      if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) { // Двигатель - останов;
        stopBeginTime_ = ksu.getTime();
        state_ = PauseState;
      }
      break;
    case PauseState:
      if (ksu.isStopMotor()) {     // Двигатель - останов;
        uint32_t time = ksu.getSecFromCurTime(stopBeginTime_); // Прошедшее время с начала останова
        stopTimeToEnd_ = getTimeToEnd(stopPeriod_ + addTime_, time);
        if (ksu.isProgramMode()) {   // Режим - программа;
          if (stopTimeToEnd_ == 0) { // Время паузы истекло
            if (ksu.isPrevent()) {
              if (!attempt_) {                // Первая попытка запуска
                attempt_ = true;
                addEventProtectionPrevent();  // Сообщение неудачной попытке пуска
              }
            } else {
              addTime_ = 0;
              ksu.start(LastReasonRunProgram);
              attempt_ = false;
              state_ = RestartState;
            }
          }
        }
        else { // Режим программы отключен
          state_ = StopState;
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
      }
      else {
        state_ = PauseState;
      }
      break;
    case StopState:
      if (ksu.isStopMotor()) {     // Двигатель - останов;
        uint32_t time = ksu.getSecFromCurTime(stopBeginTime_);
        stopTimeToEnd_ = getTimeToEnd(stopPeriod_, time);
        if (ksu.isProgramMode()) { // Режим - программа;
          addTime_ = parameters.getValue(CCS_TIMER_DIFFERENT_START) - stopTimeToEnd_;
          if (addTime_ < 0) {
            addTime_ = 0;
          }
          else {
            if (int(time - stopPeriod_) > 0)
              addTime_ = addTime_ + (time - stopPeriod_);
          }
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

  parameters.setValue(CCS_RGM_PERIODIC_STATE, state_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_PERIOD, workPeriod_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_PERIOD, stopPeriod_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_BEGIN_TIME, workBeginTime_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_BEGIN_TIME, stopBeginTime_);
  parameters.setValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END, workTimeToEnd_);
  parameters.setValue(CCS_RGM_PERIODIC_STOP_TIME_TO_END, stopTimeToEnd_);
}

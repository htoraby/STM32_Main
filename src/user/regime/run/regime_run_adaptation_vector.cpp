#include "regime_run_adaptation_vector.h"

RegimeRunAdaptationVector::RegimeRunAdaptationVector()
{

}

RegimeRunAdaptationVector::~RegimeRunAdaptationVector()
{

}

void RegimeRunAdaptationVector::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE);
  state_ = parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_STATE);
  type_ = parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_TYPE);
  dR_ = parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_STATOR_RESISTANCE_DELTA);
  pwrTrans_ = parameters.get(CCS_TRANS_NOMINAL_POWER);
  nowCurMtr_ = (parameters.get(VSD_CURRENT_OUT_PHASE_1)
              + parameters.get(VSD_CURRENT_OUT_PHASE_2)
              + parameters.get(VSD_CURRENT_OUT_PHASE_2))/ 3.0;
  nomCurMtr_ = parameters.get(VSD_MOTOR_CURRENT);
  strR_ = parameters.get(VSD_RESISTANCE_STATOR);
}

void RegimeRunAdaptationVector::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_STATE, state_);
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_TYPE, type_);
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_STATOR_RESISTANCE_DELTA, dR_);
  parameters.set(VSD_RESISTANCE_STATOR, strR_);
}

void RegimeRunAdaptationVector::processingStateIdle()
{
  timer_ = 0;                                                 // Таймер задержки реакции
  if (action_) {                                              // Если режим активен
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        if (type_ == typeInWork) {                            // Если режим в работе
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Адаптация: Idle -> WorkState");
          #endif
          state_ = WorkState;                                 // Переход в состояние работа
        }
        else {
          type_ = typeInWork;                                 // Говорим что в работе и переходим
          logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationStartId);
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Адаптация: Idle -> WorkState+3");
          #endif
          state_ = WorkState + 3;                             // Переходим на состояние задания параметров автоадаптации
        }
      }
    }
  }
}

void RegimeRunAdaptationVector::processingStateWork()
{
  if (ksu.isStopMotor()) {                  // Если двигатель стоит
    resetAdaptationVector(typeErr);         // Сброс адаптации
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Адаптация: WorkState -> IdleState isStopMotor");
    #endif
    state_ = IdleState;
  }
  switch (state_) {
  case WorkState:                           //
    if (timer_ > 30) {                      // Задержка на чтение текущего тока
      timer_ = 0;
      if (dR_ == 0) {                       // Сбой, шаг изменения сопротивления
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Адаптация: WorkState -> WorkState + 3 (dr = %5.4f)",
                       dR_);
        #endif
        state_ = WorkState + 3;
      }
      else {
        if (nowCurMtr_ < 0.47 * nomCurMtr_) {
          strR_ = strR_ + dR_;
          if (strR_ > 0.3) {
            strR_ = 0.3;
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Адаптация: WorkState -> WorkState + 6 (strR_ = %5.4f)",
                           strR_);
            #endif
            state_ = WorkState + 6;
          }
          else {
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Адаптация: WorkState -> WorkState + 1 (strR_ = %5.4f)",
                           strR_);
            #endif
            state_ = WorkState + 1;
          }
        }
        else {
          if (nowCurMtr_ > 0.52 * nomCurMtr_) {
            strR_ = strR_ - dR_;            // Уменьшаем сопротивление на шаг
            dR_ = dR_ / 5;                  // Уменьшаем шаг
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Адаптация: WorkState -> WorkState + 1 (strR_ = %5.4f, dR_ = %5.4f)",
                           strR_, dR_);
            #endif
            state_ = WorkState + 1;         // Задаём новое сопротивление
          }
          else {
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Адаптация: WorkState -> WorkState + 4 (strR_ = %5.4f, dR_ = %5.4f)",
                           strR_, dR_);
            #endif
            state_ =  WorkState + 4;        // Сбрасываем режим автоадаптации
          }
        }
      }
      ksu.stop((LastReasonStop)parameters.get(CCS_LAST_STOP_REASON));
    }
    break;
  case WorkState + 11:
    if (timer_ > 10) {
      timer_ = 0;
      ksu.stop((LastReasonStop)parameters.get(CCS_LAST_STOP_REASON));
      state_ = WorkState + 1;
    }
    break;
  case WorkState + 1:                       // Записываем новое значение сопротивления
    if (vsd->checkStop()) {
      timer_ = 0;
      vsd->setMotorResistanceStator(strR_);
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 1 -> WorkState + 2 (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = WorkState + 2;
    }
    else {
      if (repeat_ > 5) {
        repeat_ = 0;
        resetAdaptationVector(typeErr);
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Адаптация: WorkState + 1 -> IdleState (strR_ = %5.4f, dR_ = %5.4f)",
                       strR_, dR_);
        #endif
        state_ = IdleState;
      }
      else {
        repeat_++;
        state_ = WorkState + 11;
      }
    }
    break;
  case WorkState + 2:
    if (timer_ > 10) {
      timer_ = 0;
      ksu.start((LastReasonRun)parameters.get(CCS_LAST_RUN_REASON));
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 2 -> WorkState (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = WorkState;
    }
    break;
  case WorkState + 3:
    if (timer_ > 10) {
      timer_ = 0;
      resetAdaptationVector(typeErr);
      ksu.start((LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP));
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 3 -> WorkState + 5 (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = WorkState + 5;
    }
    break;
  case WorkState + 4:
    if (timer_ > 50) {
      timer_ = 0;
      resetAdaptationVector(typeOff);
      ksu.start((LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP));
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 4 -> IdleState (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = IdleState;
    }
    break;
  case WorkState + 5:
    if (timer_ > 10) {
      timer_ = 0;
      ksu.start((LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP));
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 5 -> IdleState (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = IdleState;
    }
    break;
  case WorkState + 6:
    if (timer_ > 10) {
      timer_ = 0;
      resetAdaptationVector(typeErr);
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Адаптация: WorkState + 6 -> IdleState (strR_ = %5.4f, dR_ = %5.4f)",
                     strR_, dR_);
      #endif
      state_ = IdleState;
    }
    break;
  default:
    state_ = IdleState;
    break;
  }
}

void RegimeRunAdaptationVector::automatRegime()
{
  if ((parameters.get(VSD_MOTOR_CONTROL) != VSD_MOTOR_CONTROL_VECT)       // Не векторное управление
    || (action_ == OffAction))    // Режим выключен
    return;                       // Выходим если режим выключен или не векторный режим

  if ((state_ != IdleState) && (!vsd->isConnect() || vsd->checkAlarmVsd())) {
    resetAdaptationVector(typeErr);
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Адаптация: ");
    #endif
    state_ = IdleState;           // Выходим из режима с ошибкой, если пропала связь с ЧРП или авария
  }

  if (dR_ < 0.001) {
    if (pwrTrans_ < 300001) {
      dR_ = 0.0075;
    }
    else if (pwrTrans_ < 600001) {
      dR_ = 0.005;
    }
    else if (pwrTrans_ < 900001) {
      dR_ = 0.0025;
    }
    else {
      dR_ = 0.0010;
    }
    parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_STATOR_RESISTANCE_DELTA, dR_);
  }

  timer_++;

  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case WorkState:
    processingStateWork();
    break;
  default:
    break;
  }

}

void RegimeRunAdaptationVector::resetAdaptationVector(uint16_t type)
{
  if (type) {
    logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationIncompleteId);
  }
  else {
    logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationCompleteId);
  }
  vsd->resetAdaptationVector(type);
}


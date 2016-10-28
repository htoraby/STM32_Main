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
  if (state_ != PauseState) {
    strR_ = parameters.get(VSD_RESISTANCE_STATOR);
  }
}

void RegimeRunAdaptationVector::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_STATE, state_);
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_TYPE, type_);
  parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_STATOR_RESISTANCE_DELTA, dR_);
}

void RegimeRunAdaptationVector::processingStateIdle()           // Состояние ожидания начала работы режима
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {    // Станция в останове
      if (runReason_ != LastReasonRunNone) {                    // Попытка пуска
        logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationStartId);
        state_ = RunningState;                                  // Задание настроек
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateIdle() Idle -> RunningState");
        #endif
      }
    }
  }
}

void RegimeRunAdaptationVector::processingStateRunning()        // Состояние первого пуска в режиме
{
  switch (state_) {
  case RunningState:                                            // Состояние задания настроек
    if (vsd->checkSetAdaptationVector()) {                      // Настройки заданы
      repeat_ = 0;
      timer_ = 0;
      state_ = RunningState + 1;                                // Переход в состояние первого запуска
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() RunningState -> RunningState + 1");
      #endif
    }
    else {                                                      // Настройки не заданы
      if (timer_ > 2) {                                         // Истекла задержка применения настроек
        timer_ = 0;
        if (repeat_ > 5) {                                      // Сделали 5 попыток применения настроек
          repeat_ = 0;
          state_ = StopState;                                   // Переходим в состояние выхода из режима
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() RunningState -> StopState");
          #endif
        }
        else {                                                  // Не сделали 5 попыток
          repeat_++;
          vsd->setAdaptationVector();                           // Снова пытаемся настроить режим
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() vsd->setAdaptationVector() (repeat_ = %5.0f)",
                         repeat_);
          #endif
        }
      }
    }
    break;
  case RunningState + 1:                                        // Состояние первого запуска
    if (vsd->checkStart()) {                                    // Если ЧРП в работе
      repeat_ = 0;
      timer_ = 0;
      state_ = WorkState;                                       // Переход на состояние подбора сопротивления
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() RunningState + 1 -> WorkState");
      #endif
    }
    else {                                                      // Если ЧРП стоит
      if (timer_ > 2) {                                         // Истекла задержка запуска ЧРП
        timer_ = 0;
        if (repeat_ > 5) {                                      // Сделали 5 попыток пуска
          repeat_ = 0;
          state_ = StopState;                                   // Переходим в состояние выхода из режима
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() RunningState + 1 -> StopState");
          #endif
        }
        else {
          repeat_++;                                            // Не сделали 5 попыток
          if (!ksu.isWorkMotor()) {                             // КСУ не в работе
            ksu.start((LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP));      // Пробуем запуститься ещё раз
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRunning() ksu.start(LastReasonRunProgram) (repeat_ = %5.0f)",
                           repeat_);
            #endif
          }
        }
      }
    }
    break;
  default:
    state_ = StopState;                    // Выходим из адаптации с ошибкой
    break;
  }
}

void RegimeRunAdaptationVector::processingStateWork()           // Состояние подбора сопротивления
{
  switch (state_) {
  case WorkState:                                               // Состояние подбора сопротивления
    if (timer_ > 2) {                                          // Задержка измерения тока
      timer_ = 0;
      if (dR_ == 0) {                                           // Сбой, шаг изменения сопротивления
        state_ = RunningState;                                  // Начинаем адаптацию снова
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> RunningState (dR_ = %5.4f)",
                       dR_);
        #endif
      }
      else {                                                    // Измерили ток
        if (nowCurMtr_ < 0.47 * nomCurMtr_) {                   // Если ток меньше 47% от номинального
          strR_ = strR_ + dR_;                                  // Увеличиваем сопротивление статора
          if (strR_ > 0.3) {                                    // Если сопротивление статора достигло максимального значения
            state_ = StopState;                                 // Выходим из алгоритма с ошибкой
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> StopState (strR_ = %5.4f)",
                           strR_);
            #endif
          }
          else {                                                // Если не достигло максимума
           // vsd->setMotorResistanceStator(strR_);
            state_ = PauseState;                                // Переходим на состояние останова для задания нового сопротивления
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> PauseState (strR_ = %5.4f)",
                           strR_);
            #endif
          }
        }
        else {
          if (nowCurMtr_ > 0.52 * nomCurMtr_) {
            strR_ = strR_ - dR_;                                // Уменьшаем сопротивление на шаг
            dR_ = dR_ / 5;                                      // Уменьшаем шаг
            state_ = PauseState;                                // Задаём новое сопротивление
            if (dR_ < 0.0001) {
              state_ = StopState;                                 // Выходим из алгоритма с ошибкой
              #if (USE_LOG_DEBUG == 1)
                logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> StopState (strR_ = %5.4f)",
                             strR_);
              #endif
            }
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> PauseState (strR_ = %5.4f, dR_ = %5.4f)",
                           strR_, dR_);
            #endif
          }
          else {
            state_ =  WorkState + 1;                            // Корректно закончили автоадаптацию
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateWork() WorkState -> WorkState + 1 (strR_ = %5.4f, dR_ = %5.4f)",
                           strR_, dR_);
            #endif
          }
        }
      }
    }
    break;
  case WorkState + 1:                                           // Состояние подобранного сопротивления
    type_ = typeOff;                                            // Выход из алгоритма
    resetAdaptationVector(type_);
    state_ = IdleState;
    break;
  default:
    state_ = StopState;
    break;
  }
}

void RegimeRunAdaptationVector::processingStateStop()
{
  switch (state_) {
  case StopState:
    if (vsd->checkStop()) {                                    // Если ЧРП в работе
      repeat_ = 0;
      timer_ = 0;
      state_ = StopState + 1;                                      // Переход на состояние подбора сопротивления
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateStop() StopState -> StopState + 1");
      #endif
    }
    else {                                                      // Если ЧРП стоит
      if (timer_ > 2) {                                        // Истекла задержка запуска ЧРП
        timer_ = 0;
        if (repeat_ > 5) {                                      // Сделали 5 попыток пуска
          repeat_ = 0;
          state_ = StopState + 1;                               // Переходим в состояние выхода из режима
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateStop() StopState -> StopState + 1");
          #endif
        }
        else {
          repeat_++;                                            // Не сделали 5 попыток
          if (!ksu.isStopMotor()) {                             // КСУ не в стопе
            ksu.stop(LastReasonStopAdaptationVector);           // Пробуем остановить ещё раз
            #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateStop() ksu.stop(LastReasonStopAdaptationVector) (repeat_= %5.0f)",
                           repeat_);
            #endif
          }
        }
      }
    }
    break;
  case StopState + 1:                                           // Состояние сброса настроек
    if (vsd->checkResetAdaptationVector()) {                    // Настройки сброшены
      repeat_ = 0;
      timer_ = 0;
      state_ = IdleState;
    }
    else {
      if (timer_ > 2) {                                        // Истекла задержка останова и применения настроек
        timer_ = 0;
        if (repeat_ > 5) {                                      // Сделали 5 попыток пуска
          type_ = typeErr;                                      // Выход из алгоритма
          resetAdaptationVector(type_);                         //
          state_ = IdleState;
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateStop() StopState -> IdleState repeat_ = 5");
          #endif
        }
        else {
          repeat_++;                                            // Не сделали 5 попыток
          type_ = typeErr;                                      // Выход из алгоритма
          resetAdaptationVector(type_);                         //
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateStop() StopState + 1 -> IdleState (repeat_ = %5.0f",
                         repeat_);
          #endif
          state_ = IdleState;
        }
      }
    }
    break;
  }
}

void RegimeRunAdaptationVector::processingStatePause()          // Состояние останова и задания нового сопротивления
{
  if (vsd->checkStop()) {                                       // Если двигатель остановлен
    vsd->setMotorResistanceStator(strR_);                       // Задаём новое сопротивление
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStatePause()  PauseState setMotorResistanceStator(strR_) (strR_ = %5.4f)",
                   strR_);
    #endif
    repeat_ = 0;
    timer_ = 0;
    state_ = RestartState;                                      // Переходим на состояние запуска
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStatePause()  PauseState -> RestartState");
    #endif
  }
  else {
    if (timer_ > 2) {                                          // Задержка на конфигурирование
      timer_ = 0;
      if (repeat_ > 5) {                                        // Сделали 5 попыток адаптации
        repeat_ = 0;
        state_ = StopState;                                     // Выходим из адаптации с ошибкой
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStatePause()  PauseState -> StopState");
        #endif
      }
      else {
        repeat_++;                                              // Не сделали 5 попыток
        vsd->stop(TYPE_STOP_ALARM);                             // Пытаемся остановить ЧРП Снова
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStatePause()  PauseState vsd->stop (repeat_ = %5.0f)",
                       repeat_);
        #endif
      }
    }
  }
}

void RegimeRunAdaptationVector::processingStateRestart()        // Состояние запуска после изменения сопротивления
{
  if (vsd->checkStart()) {                                      // Если ЧРП в работе
    repeat_ = 0;
    timer_ = 0;
    state_ = WorkState;                                         // Переходим на состояние подбора сопротивления
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRestart() RestartState -> WorkState");
    #endif
  }
  else {
    if (timer_ > 5) {                                          // Задержка на конфигурирование
      timer_ = 0;
      if (repeat_ > 5) {                                        // Сделали 5 попыток адаптации
        repeat_ = 0;
        state_ = StopState;                                     // Выходим из адаптации с ошибкой
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRestart() RestartState -> StopState");
        #endif
      }
      else {
        repeat_++;                                              // Не сделали 5 попыток
        vsd->start(true);                                       // Снова пытаемся запуститься
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunAdaptationVector::processingStateRestart() RestartState vsd->start() (repeat_ = %5.0f)",
                       repeat_);
        #endif
      }
    }
  }
}

void RegimeRunAdaptationVector::automatRegime()
{
  if ((parameters.get(VSD_MOTOR_CONTROL) != VSD_MOTOR_CONTROL_VECT)       // Не векторное управление
    || (action_ == OffAction))              // Режим выключен
    return;                                 // Выходим если режим выключен или не векторный режим

  if ((state_ != IdleState) && (!vsd->isConnect() || vsd->checkAlarmVsd())) {
    state_ = StopState;                    // Состояние для выхода из режима с ошибкой
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunAdaptationVector::automatRegime() Lost connect or trip");
    #endif
  }

  if (dR_ < 0.001) {                        // Задание изменения приращения
    if (pwrTrans_ < 300001) {               // в зависимости от мощности ТМПН
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
  }

  timer_++;                                 // Счётчик таймер задержек

  switch (state_) {                         // Состояние алгоритма адаптации
  case IdleState:                           // Состояние бездействия алгоритма
    processingStateIdle();
    break;
  case RunningState:                        // Состояние задания настроек для автоадаптации
  case RunningState + 1:                    // Состояние первого пуска при автоадаптации
    processingStateRunning();
    break;
  case WorkState:                           // Состояние подбора сопротивления
  case WorkState + 1:                       // Состояние подобраного сопротивления, успешный выход
    if (ksu.isStopMotor()) {
      state_ = StopState;
    }
    processingStateWork();
    break;
  case StopState:                           // Состояние выхода из автоадаптации
  case StopState + 1:
    processingStateStop();
    break;
  case PauseState:                          // Останова в процессе работы алгоритма
    if (ksu.isStopMotor()) {
      state_ = StopState;
    }
    processingStatePause();
    break;
  case RestartState:                        // Состояние запуска в процессе работы алгоритма
    if (ksu.isStopMotor()) {
      state_ = StopState;
    }
    processingStateRestart();
    break;
  default:
    state_ = StopState;
    break;
  }
}

void RegimeRunAdaptationVector::resetAdaptationVector(uint16_t type)
{
  if (type) {
    logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationIncompleteId);
  }
  else {
    logEvent.add(OtherCode, AutoType, RegimeRunAutoAdaptationCompleteId, 0, strR_);

    if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE) == SingleAction) {
      parameters.set(CCS_RGM_RUN_AUTO_ADAPTATION_MODE, OffAction);         // Выключаем режим
      logEvent.add(SetpointCode, AutoType, RegimeRunAutoAdaptationOffId);  // Записываем данные в лог
    }
  }

  vsd->resetAdaptationVector(type);

}

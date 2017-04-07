#include "regime_run_skip_resonant_freq.h"

RegimeRunSkipResonantFreq::RegimeRunSkipResonantFreq()
{

}

RegimeRunSkipResonantFreq::~RegimeRunSkipResonantFreq()
{

}

void RegimeRunSkipResonantFreq::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE);
  state_ = parameters.get(CCS_RGM_RUN_SKIP_RESONANT_STATE);
  beginFreq_ = parameters.get(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ);
  endFreq_ = parameters.get(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ);
  time_ = parameters.get(VSD_MOTOR_FREQUENCY) / parameters.get(CCS_RGM_RUN_SKIP_RESONANT_TEMP);
  freq_ = parameters.get(VSD_FREQUENCY_NOW);
  setpointFreq_ = parameters.get(VSD_FREQUENCY);
  rememberFreq_ = parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_FREQ);
}

void RegimeRunSkipResonantFreq::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_SKIP_RESONANT_STATE, state_);
}

void RegimeRunSkipResonantFreq::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_WORK) {  // Станция в останове
      state_ = WorkState;
    }
  }
}

void RegimeRunSkipResonantFreq::processingStateRunning()
{
  ksu.start(runReason_, true);
  state_ = WorkState;
}

void RegimeRunSkipResonantFreq::processingStateWork()
{
  int16_t err = -1;
  switch (state_) {

  case WorkState:
    tempBeginFreq_ = beginFreq_;
    tempEndFreq_ = endFreq_;
    if (!(((freq_ <= beginFreq_)            // Состояние анализа, прохода через диапазон пропуска частот
           && (setpointFreq_ <= beginFreq_))
          ||((freq_ >= endFreq_)
             && (setpointFreq_ >= endFreq_)))) {
      if (freq_ < setpointFreq_) {          // Если текущая частота меньше уставки, т.е набор частоты
        if (setpointFreq_ < endFreq_) {     // Если уставка частоты лежит в диапазоне пропуска частот
          tempEndFreq_ = setpointFreq_;     // Во временную переменную конечной частоты, записываем уставку, чтобы не повышать частоту до уставки по умолчанию
        }
        if ((freq_ >= beginFreq_)           // Если текущая частота лежит в диапазоне пропуска частот
            && (freq_ <= endFreq_)) {
          tempBeginFreq_ = freq_;           // Во временную переменную начальной частоты, записываем текущую частоту TODO: А надо ли?
          state_ = WorkState + 1;           // Переходим на состояние набора частоты из диапазона пропуска частот
        }
        else {                              // Текущая частота из диапазона до диапазона пропуска частот
          state_ = WorkState + 11;          // Переходим на состояние набора частоты из диапазона до диапазона пропуска частот
        }
      }
      else if (freq_ > setpointFreq_) {     // Если текущая частота больше уставки частоты, т.е. снижение частоты
        if (setpointFreq_ > beginFreq_) {   // Если уставка частоты лежит в диапазоне пропуска частот
          tempBeginFreq_ = setpointFreq_;   // Во временную переменную начальной частоты, записываем уставку, чтобы снижать частоту до уставки по умолчанию
        }
        if ((freq_ >= beginFreq_)           // Если текущая частота лежит в диапазоне пропуска частот
            && (freq_ <= endFreq_)) {
          tempEndFreq_ = freq_;             // Во временную переменную конечной частоты, записываем текущую частоту TODO: А надо ли?
          state_ = WorkState + 21;          // Переходим на состояние снижения частоты из диапазона пропуска частот
        }
        else {                              // Текущая частота из диапазона после диапазона пропуска частот
          state_ = WorkState + 31;          // Переходим на состояние снижения частоты из диапазона после диапазона пропуска частот
        }
      }
      saveBeforeRegimeRun();                // Сохраняем настройки ЧРП
    }
    break;

  case WorkState + 1:                       // Состояние набора частоты из диапазона пропуска частот
    err = setEndFreq();                     // Задаём конечную частоту, при наборе из диапазона сначала задаём конечную частоту
    if (!err) {                             // Задали конечную частоту
      state_ = WorkState + 2;               // Переходим на состояние задания темпа разгона
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 2:                       // Состояние задания темпа разгона
    err = setTempSkip();                    // Задаём новый темп разгона
    if (!err) {                             // Задали новый темп разгона
      state_ = WorkState + 3;               // Переход на состояние задания конечной частоты
      logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 3:                       // Состояние возврата темпа разгона
    if (setpointFreq_ != tempEndFreq_) {    // Изменена уставка частоты из вне
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли конечной частоты
      err = returnTemp();                   // Вовзращаем темп разгона
      if (!err) {                           // Вернули темп разгона
        state_ = WorkState + 4;             // Переходим на состояние возврата уставки частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 4:                     // Состояние возврата уставки частоты
    err = returnFreq();                   // Возвращем уставку частоты
    if (!err) {                             // Вернули уставку частоты
      err = returnMinFreq();
      if (!err) {
        state_ = WorkState;                 // Возвращаемся в состояние ожидания изменения частоты
      }
      else if (err > 0) {                     // Ошибка
        state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
      }
    }
    else if (err > 0) {                   // Ошибка
      state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режима
    }
    break;

  case WorkState + 11:                        // Состояние набора частоты из диапазона до диапазона пропуска частот
    err = setMinFreq();
    if (!err) {
      err = setBeginFreq();                   // Меняём уставку частоты на начальную частоту
      if (!err) {                             // Если изменили уставку частоты на начальную частоту
        state_ = WorkState + 12;              // Переходим на состояние задания темпа пропуска частот
      }
      else if (err > 0) {                     // Ошибка
        state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
      }
    }
    else if (err > 0) {
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 12:                      // Состояние задания темпа пропуска частот при наборе частоты
    if (setpointFreq_ != tempBeginFreq_) {  // Во время разгона до начальной частоты изменили уставку частоты
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли начальной частоты
      err = setTempSkip();                  // Задаём новый темп разгона, при этом работаем на начальн
      if (!err) {                           // Задали новый темп разгона
        state_ = WorkState + 13;            // Переход на состояние задания конечной частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 13:                      // Состояние задания конечной частоты при наборе частоты
    err = setEndFreq();                     // Задаём конечную частоту
    if (!err) {                             // Задали конечную частоту
      state_ = WorkState + 14;              // Переходим на состояние возврата темпа разгона
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 14:                      // Состояние возврата темпа разгона
    if (setpointFreq_ != tempEndFreq_) {    // Изменена уставка частоты из вне
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли конечной частоты
      err = returnTemp();                   // Вовзращаем темп разгона
      if (!err) {                           // Вернули темп разгона
        state_ = WorkState + 15;            // Переходим на состояние возврата уставки частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 15:                      // Состояние возврата уставки частоты
    err = returnFreq();                     // Возвращем уставку частоты
    if (!err) {                             // Вернули уставку частоты
      err = returnMinFreq();
      if (!err) {
        state_ = WorkState;                 // Возвращаемся в состояние ожидания изменения частоты
      }
      else if (err > 0) {                     // Ошибка
        state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
      }
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
    }
    break;

  case WorkState + 21:                      // Состояние снижения частоты из диапазона пропуска частот
    err = setBeginFreq();                   // Задаём начальную частоту, при наборе из диапазона сначала задаём начальную частоту
    if (!err) {                             // Задали начальную частоту
      state_ = WorkState + 22;              // Переходим на состояние задания темпа разгона
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 22:                      // Состояние задания темпа разгона
    err = setTempSkip();                    // Задаём новый темп снижения
    if (!err) {                             // Задали новый темп снижения
      state_ = WorkState + 23;              // Переход на состояние задания конечной частоты
      logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 23:                      // Состояние возврата темпа разгона
    if (setpointFreq_ != tempBeginFreq_) {  // Изменена уставка частоты из вне
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли конечной частоты
      err = returnTemp();                   // Вовзращаем темп разгона
      if (!err) {                           // Вернули темп разгона
        state_ = WorkState + 24;            // Переходим на состояние возврата уставки частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 24:                      // Состояние возврата уставки частоты
    err = returnFreq();                   // Возвращем уставку частоты
    if (!err) {                             // Вернули уставку частоты
      err = returnMinFreq();
      if (!err) {
        state_ = WorkState;                 // Возвращаемся в состояние ожидания изменения частоты
      }
      else if (err > 0) {                     // Ошибка
        state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
      }
    }
    else if (err > 0) {                   // Ошибка
      state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режима
    }
    break;

  case WorkState + 31:                      // Состояние снижения частоты из диапазона после диапазона пропуска частот
    err = setEndFreq();                     // Задаём конечную частоту
    if (!err) {                             // Если задали конечную частоту
      state_ = WorkState + 32;              // Переходим на состояние задания темпа снижения частоты
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 32:                      // Состояние задания темпа снижения частоты
    if (setpointFreq_ != tempEndFreq_) {    // Изменена уставка частоты из вне
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли конечной частоты
      err = setTempSkip();                  // Задаём новый темп снижения
      if (!err) {                           // Если задали
        state_ = WorkState + 33;            // Переход на состояние задания начальной частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 33:                      // Cостояние задания начальной частоты
    err = setBeginFreq();                   // Задаём начальную частоты
    if (!err) {                             // Если задали начальную частоту
      state_ = WorkState + 34;              // Переходим на состояние возвращения темпа снижения частоты
    }
    else if (err > 0) {                     // Ошибка
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режим
    }
    break;

  case WorkState + 34:                      // Состояние возвращения темпа снижения частоты
    if (setpointFreq_ != tempBeginFreq_) {  // Изменена уставка частоты из вне
      state_ = WorkState + 41;              // Переходим на состояние переинициализации пропуска частот
    }
    else if (vsd->isSetPointFreq()) {       // Достигли начальной частоты
      err = returnTemp();                   // Возвращаем темп снижения частоты
      if (!err) {                           // Вернули темп снижения частоты
        state_ = WorkState + 35;            // Переходим на состояние возврата уставки частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {                   // Ошибка
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case WorkState + 35:                      // Состояние возврата уставки частоты
    err = returnFreq();                     // Возвращаем частоту
    if (!err) {                             // Вернули уставку частоты
      err = returnMinFreq();
      if (!err) {
        state_ = WorkState;                 // Возвращаемся в состояние ожидания изменения частоты
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {
        state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
      }
    }
    else if (err > 0) {
      state_ = WorkState + 42;              // Попытаться вернуть настройки и выйти из режима
    }
    break;

  case WorkState + 41:                      // Во время работы режима изменили уставку частоты
    err = returnTemp();                     // Возвращаем темп разгона
    if (!err) {
      tempBeginFreq_ = beginFreq_;
      tempEndFreq_ = endFreq_;
      state_ = WorkState;                   // Переходим на инициализацию
      logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantAbortId);
    }
    break;

  case WorkState + 42:                      // Состояние ошибки задания параметров в ЧРП
    returnMinFreq();                        // Возвращаем минимальную частоту
    returnTemp();                           // Возвращаем темп изменения частоты
    returnTempDown();                       // Возвращаем темп снижения частоты при торможении
    returnFreq();                           // Возвращаем уставку частоты
    logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFailId);
    state_ = IdleState;                     // Переходим в состояние Idle
    break;
  }
}

void RegimeRunSkipResonantFreq::processingStateStop()
{
  int16_t err = -1;
  switch (state_) {
  case StopState:                      // Состояние отслеживания частот пропуска частот при торможении
    if ((freq_ >= beginFreq_)
        && (freq_ <= endFreq_)) {
      err = setTempDownSkip();              // Задаём новый темп торможения
      if (!err) {                           // Задали новый темп торможения
        state_ = StopState + 1;            // Переход на состояние контроля выхода из диапазона пропуска частот
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
      }
      else if (err > 0) {
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case StopState + 1:
    if ((freq_ < beginFreq_)
        || (freq_ > endFreq_)) {
      err = returnTempDown();
      if (!err) {
        state_ = IdleState;
        logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFinishId);
      }
      else if (err > 0) {
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    break;

  case StopState + 2:
    err = returnTempDown();
    if (!err) {
      err = returnTemp();         // Возвращаем темп разгона
      if (!err) {
        err = returnMinFreq();
        if (!err) {
          err = returnFreq();
          if (!err) {
            state_ = IdleState;       // Переходим в состояние Idle
          }
          else if (err > 0) {
            state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
          }
        }
        else if (err > 0) {
          state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
        }
      }
      else if (err > 0) {
        state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
      }
    }
    else if (err > 0) {
      state_ = WorkState + 42;            // Попытаться вернуть настройки и выйти из режим
    }
    break;
  }
}

void RegimeRunSkipResonantFreq::automatRegime()
{
  if ((action_ == OffAction)
      && (state_ != IdleState)
      && (state_ != StopState)
      && (state_ != StopState + 1)
      && (state_ != StopState + 2)) {
    state_ = StopState;
  }

  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:                           // Состояние ожидания изменения частоты
  case WorkState + 1:                       // 1-4 состояние набора частоты из диапазона пропуска
  case WorkState + 2:
  case WorkState + 3:
  case WorkState + 4:
  case WorkState + 11:                      // 11-15 состояние набора частоты
  case WorkState + 12:
  case WorkState + 13:
  case WorkState + 14:
  case WorkState + 15:
  case WorkState + 21:                      // 21-24 состояния снижения частоты их диапазона пропуска
  case WorkState + 22:
  case WorkState + 23:
  case WorkState + 24:
  case WorkState + 31:                      // 31-35 состояние снижения частоты
  case WorkState + 32:
  case WorkState + 33:
  case WorkState + 34:
  case WorkState + 35:
  case WorkState + 41:                      // Состояния ошибок, прерываний и нештатных ситуаций
  case WorkState + 42:
    // Во время работы торможение
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_BREAK) {
      state_ = StopState;                   // Переходим на состояние отслеживания падения частоты
      break;
    }
    // Во время работы оказались в состоянии стоп
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
      state_ = StopState + 2;               // Переходим на состояние возврата настроек ЧРП
      break;
    }
    processingStateWork();
    break;
  case StopState:
  case StopState + 1:
  case StopState + 2:
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
      state_ = StopState + 2;
    }
    processingStateStop();
    break;
  default:
    state_ = StopState;
    break;
  }
}

void RegimeRunSkipResonantFreq::saveBeforeRegimeRun()
{
  parameters.set(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_FREQ, parameters.get(VSD_FREQUENCY));
  parameters.set(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_TIME_UP, parameters.get(VSD_TIMER_DISPERSAL));
  parameters.set(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_MIN_FREQ, parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
}

int16_t RegimeRunSkipResonantFreq::setMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, 1);
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::setMinFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setBeginFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, tempBeginFreq_);
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::setBeginFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setTempSkip()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, time_);
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::setTempSkip()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setTempDownSkip()
{
  int16_t err = setConfirmation(VSD_TIMER_DELAY, time_);
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::setTempDownSkip()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::returnTempDown()
{
  int16_t err = setConfirmation(VSD_TIMER_DELAY, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_TIME_UP));
  if (!err) {
    err = setConfirmation(VSD_TIMER_DELAY_FIX_SPEED, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_TIME_UP));
    if (err == 1) {
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnTempDown()");
#endif
    }
  }
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnTempDown()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setEndFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, tempEndFreq_);
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::setEndFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::returnTemp()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_TIME_UP));
  if (!err) {
    err = setConfirmation(VSD_TIMER_DISP_FIX_SPEED, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_TIME_UP));
    if (err == 1) {
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnTemp()");
#endif
    }
  }
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnTemp()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_FREQ));
  if (!err) {
    offRegime();
  }
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_MIN_FREQ));
  if (err == 1) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSkipResonantFreq::returnMinFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setConfirmation(uint16_t id, float value)
{
  int16_t err = -1;
  if ((parameters.getValidity(id) == ok_r)
      && (parameters.get(id) == value)) {
    repeat_ = 0;
    delay_ = 0;
    err = 0;
  }
  else {
    delay_ --;
    if (delay_ <= 0) {
      if (repeat_ >= 3) {
        repeat_ = 0;
        delay_ = 0;
        err = 1;
      }
      else {
        parameters.set(id, value, NoneType);
        delay_ = 5;
        repeat_ ++;
      }
    }
  }
  return err;
}

void RegimeRunSkipResonantFreq::offRegime()
{
  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) == SingleAction) {
    parameters.set(CCS_RGM_RUN_SKIP_RESONANT_MODE, OffAction);         // Выключаем режим
    logEvent.add(SetpointCode, AutoType, RegimeRunSkipResonantOffId); // Записываем данные в лог
  }
}

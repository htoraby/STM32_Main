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
  }
}

void RegimeRunSkipResonantFreq::processingStateRunning()
{
  ksu.start(runReason_);
  logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantStartId);
  state_ = WorkState;
}

void RegimeRunSkipResonantFreq::processingStateWork()
{
  int16_t err = -1;
  switch (state_) {
  // Состояние анализа, прохода через диапазон пропуска частот
  case WorkState:
    // Проход снизу вверх
    if ((freq_ < beginFreq_) && (setpointFreq_ > endFreq_)) {
      saveBeforeRegimeRun();                                // Сохраняем настройки ЧРП
      state_ = WorkState + 1;
    }
    // Проход сверху вниз
    else if ((freq_ > endFreq_) && (setpointFreq_ < beginFreq_)) {
      saveBeforeRegimeRun();                                // Сохраняем настройки ЧРП
      state_ = WorkState + 6;
    }
    break;
  // Проход снизу вверх, задаём начальную частоту пропуска резонансных частот
  case WorkState + 1:
    err = setBeginFreq();
    if (!err) {
      state_ = WorkState + 2;
    }
    else if (err > 0) {
      state_ = WorkState + 12;              // Попытаться вернуть настройки и выйти из режим
    }
    break;
  // Проход снизу вверх, ждём набора начальной частоты
  case WorkState + 2:
    if (setpointFreq_ != beginFreq_) {      // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      if (vsd->isSetPointFreq()) {          // Достигли начальной частоты
        err = setTempSkip();                // Задаём новый темп разгона
        if (!err) {
          state_ = WorkState + 3;           // Переход на состояние задания конечной частоты
        }
        else if (err > 0) {
          state_ = WorkState + 12;          // Попытаться вернуть настройки и выйти из режим
        }
      }
    }
    break;
  // Проход снизу вверх, задание конечной частоты
  case WorkState + 3:
    err = setEndFreq();
    if (!err) {
      state_ = WorkState + 4;
    }
    else if (err > 0) {
      state_ = WorkState + 12;              // Попытаться вернуть настройки и выйти из режим
    }
    break;
  // Проход снизу вверх, ждём набора конечной частоты
  case WorkState + 4:
    if (setpointFreq_ != endFreq_) {      // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      if (vsd->isSetPointFreq()) {        // Достигли конечной частоты
        err = returnTemp();
        if (!err) {
          state_ = WorkState + 5;
        }
        else if (err > 0) {
          state_ = WorkState + 12;        // Попытаться вернуть настройки и выйти из режим
        }
      }
    }
    break;
  // Проход снизу вверх, возвращение частоты уставки
  case WorkState + 5:
    if (setpointFreq_ != endFreq_) {      // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      err = returnFreq();
      if (!err) {
         state_ = WorkState;
      }
      else if (err > 0) {
        state_ = WorkState + 12;          // Попытаться вернуть настройки и выйти из режима
      }
    }
    break;
  // Проход сверху вниз, задаём конечную частоту пропуска резонансных частот
  case WorkState + 6:
    err = setEndFreq();
    if (!err) {
      state_ = WorkState + 7;
    }
    else if (err > 0) {
      state_ = WorkState + 15;              // Попытаться вернуть настройки и выйти из режим
    }
    break;
  // Проход сверху вниз, ждём снижения до конечной частоты
  case WorkState + 7:
    if (setpointFreq_ != endFreq_) {        // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      if (vsd->isSetPointFreq()) {          // Достигли начальной частоты
        err = setTempSkip();                // Задаём новый темп разгона
        if (!err) {
          state_ = WorkState + 8;           // Переход на состояние задания конечной частоты
        }
        else if (err > 0) {
          state_ = WorkState + 12;          // Попытаться вернуть настройки и выйти из режим
        }
      }
    }
    break;
  // Проход сверху вниз, задание начальной частоты
  case WorkState + 8:
    err = setBeginFreq();
    if (!err) {
      state_ = WorkState + 9;
    }
    else if (err > 0) {
      state_ = WorkState + 12;              // Попытаться вернуть настройки и выйти из режим
    }
    break;
  // Проход сверху вниз, ждем снижения до начальной частоты
  case WorkState + 9:
    if (setpointFreq_ != beginFreq_) {      // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      if (vsd->isSetPointFreq()) {          // Достигли конечной частоты
        err = returnTemp();
        if (!err) {
          state_ = WorkState + 10;
        }
        else if (err > 0) {
          state_ = WorkState + 12;          // Попытаться вернуть настройки и выйти из режим
        }
      }
    }
    break;
  // Проход сверху вниз, возвращение частоты уставки
  case WorkState + 10:
    if (setpointFreq_ != beginFreq_) {      // Изменена уставка частоты из вне
      state_ = WorkState + 11;
    }
    else {
      err = returnFreq();
      if (!err) {
        state_ = WorkState;
      }
      else if (err > 0) {
        state_ = WorkState + 12;            // Попытаться вернуть настройки и выйти из режима
      }
    }
    break;
  // Возвращаем темп разгона и переходим к анализу текущей частоты и уставки
  case WorkState + 11:
    err = returnTemp();
    if (!err) {
      state_ = WorkState;
    }
    break;
  case WorkState + 12:
    logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantFailId);
    state_ = IdleState;           // Переходим в состояние Idle
    break;
  }
}

void RegimeRunSkipResonantFreq::processingStateStop()
{
  int16_t err = -1;
  err = returnTemp();         // Возвращаем темп разгона
  if (!err) {                 // Вернули темп разгона
    logEvent.add(OtherCode, AutoType, RegimeRunSkipResonantAbortId);
    state_ = IdleState;     // Переходим в состояние Idle
  }
  else if (err > 0) {       // Ошибка задания частоты уставки
    state_ = WorkState + 2; // Попытаться вернуть настройки и выйти из режима
  }
}

void RegimeRunSkipResonantFreq::automatRegime()
{
  if ((action_ == OffAction) && (state_ != IdleState)) {
    state_ = StopState;
  }

  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
  case WorkState + 1:
  case WorkState + 2:
  case WorkState + 3:
  case WorkState + 4:
  case WorkState + 5:
  case WorkState + 6:
  case WorkState + 7:
  case WorkState + 8:
  case WorkState + 9:
  case WorkState + 10:
  case WorkState + 11:
  case WorkState + 12:
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
      state_ = StopState;
    }
    else {
      processingStateWork();
    }
    break;
  case StopState:
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
      logDebug.add(DebugMsg, "Run skip: setMinFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setBeginFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, beginFreq_);
  if (err == 1) {
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Run skip: setBeginFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setTempSkip()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, time_);
  if (err == 1) {
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Run skip: setTempSkip");
    #endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::setEndFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, endFreq_);
  if (err == 1) {
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Run skip: setEndFreq");
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
        logDebug.add(DebugMsg, "Run skip: returnTemp");
      #endif
    }
  }
  if (err == 1) {
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Run skip: returnTemp");
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
      logDebug.add(DebugMsg, "Run skip: returnFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSkipResonantFreq::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_SKIP_RESONANT_SETPOINT_MIN_FREQ));
  if (err == 1) {
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Run skip: returnMinFreq");
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
        parameters.set(id, value);
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

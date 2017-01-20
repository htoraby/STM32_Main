#include "regime_run_pickup.h"

RegimeRunPickup::RegimeRunPickup()
{

}

RegimeRunPickup::~RegimeRunPickup()
{

}

void RegimeRunPickup::getOtherSetpoint()
{
  // Уставки режима
  action_ = parameters.get(CCS_RGM_RUN_PICKUP_MODE);            // Активность режима
   // Текущие параметры режима
  state_ = parameters.get(CCS_RGM_RUN_PICKUP_STATE);            // Состояние режима
  nowSpeed_ = parameters.get(CCS_MOTOR_SPEED_NOW);
}

void RegimeRunPickup::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_PICKUP_STATE, state_);
}

void RegimeRunPickup::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        saveBeforeRegimeRun();                                // Сохраняем настройки ЧРП
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunPickup::processingStateRunning()
{
  int16_t err = 0;
  switch (state_) {
  case RunningState:                                  // Состояние запуска
    err = setMinFreq();                               // Пытаем задать минимальную частоту
    if (!err) {                                       // Если задали минимальную частоту
      err = setFreqWait();                            // Пытаемся задать частоту ожидания
      if (!err) {                                     // Задали частоту ожидания
        err = setTimeDispersal();                     // Пытаемся задать время изменения частоты
        if (!err) {                                   // Задали время изменения частоты
          err = setRotation();                        // Пытаемся задать направление вращения
          if (!err) {                                 // Задали направление вращения
            ksu.start(runReason_);                    // Подаём команду на пуск
            logEvent.add(OtherCode, AutoType, RegimeRunPickupStartId);
            state_ = WorkState;                       // Переходим в состояние работа
          }
          else if (err > 0) {                         // Не смогли задать направления вращения
            state_ = RunningState + 1;                // Переходим на состояние возврата времени изменения частоты
          }
        }
        else if (err > 0) {                           // Не смогли задать время изменения частоты
          state_ = RunningState + 2;                  // Переходим на состояние возврата частоты уставки
        }
      }
      else if (err > 0) {                             // Не смогли задать частоту ожидания
        state_ = RunningState + 3;                    // Переходим на состояние возврата минимальной частоты
      }
    }
    else if (err > 0) {                               // Не смогли задать минимальную частоту
      state_ = RunningState + 4;                      // Переходим на запуск
    }
    break;
  case RunningState + 1:
    err = returnTimeDispersal();
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 2;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 2:
    err = returnFreq();                               // Возвращаем частоту уставки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 3;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 3:
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 4;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 4:
    ksu.start(runReason_);                            // Подаём команду на пуск
    state_ = WorkState + 9;                           // Состояние формирования ошибки режима
    break;
  }
}

void RegimeRunPickup::processingStateWork()
{
  int16_t err = 0;
  uint32_t time;
  switch (state_) {
  case WorkState:                                     // Состояние ожидания выхода на заданную частоту
    if (vsd->isSetPointFreq()) {                      // Вышли на заданную частоту
      parameters.set(CCS_RGM_RUN_PICKUP_TIME_WAIT_BEGIN, ksu.getTime());                     // Запоминаем время выхода на частоту ожидания
      state_ = WorkState + 1;                         // Переход на состояние времени ожидания
    }
    break;
  case WorkState + 1:                                 // Состояние ожидания истечения "Времени ожидания"
    time = ksu.getSecFromCurTime(parameters.get(CCS_RGM_RUN_PICKUP_TIME_WAIT_BEGIN));
    if (time >= parameters.get(CCS_RGM_RUN_PICKUP_TIME_WAIT)) {
      state_ = WorkState + 2;                         // Истекло время ожидания
    }
    break;
  case WorkState + 2:
    err = setTimeDelay();                             // Пытаемся задать время снижения частоты
    if (!err) {                                       // Задали время снижения частоты
      err = returnRotation();                         // Пытаемся вернуть направление вращения
      if (!err) {                                     // Cмогли вернуть направления вращения
        err = returnFreq();                           // Пытаемся вернуть уставку частоты
        if (!err) {                                   // Вернули уставку частоты
          err = returnMinFreq();                      // Пытаемся вернуть уставку минимальной частоты
          if (!err) {                                 // Вернули уставку минимальной частоты
            parameters.set(CCS_RGM_RUN_PICKUP_SPEED_PREVIEW, parameters.get(CCS_MOTOR_SPEED_NOW));
            state_ = WorkState + 3;                   // Переходим на состояние ожидания прохода через 0
          }
          else if (err > 0 ) {                        // Не смогли вернуть минимальную частоту
            state_ = WorkState + 8;                   // Переход на возврат времени изменения частоты
          }
        }
        else if (err > 0) {                           // Не смогли вернуть частоту уставки
          state_ = WorkState + 7;                     // Переход на возврат минимальной частоты
        }
      }
      else if (err > 0) {                             // Не смогли вернуть направление вращения
        state_ = WorkState + 6;                       // Переход на возврат уставки частоты
      }
    }
    else if (err > 0) {                               // Не смогли задать время изменения частоты
      state_ = WorkState + 5;                         // Переход на возврат направления вращения
    }
    break;
  case WorkState + 3:                                 // Состояние отслеживания прохода через 0
    if (((parameters.get(CCS_RGM_RUN_PICKUP_SPEED_PREVIEW) > 0) && (parameters.get(CCS_MOTOR_SPEED_NOW) <= 0)) ||
        ((parameters.get(CCS_RGM_RUN_PICKUP_SPEED_PREVIEW) < 0) && (parameters.get(CCS_MOTOR_SPEED_NOW) >= 0))) {
      state_ = WorkState + 4;                         // Отловили переход через 0
    }
    parameters.set(CCS_RGM_RUN_PICKUP_SPEED_PREVIEW, parameters.get(CCS_MOTOR_SPEED_NOW));
    break;
  case WorkState + 4:
    err = returnTimeDispersal();
    if (!err) {
      state_ = WorkState + 10;
    }
    else if (err > 0) {
      state_ = WorkState + 9;
    }
    break;
  case WorkState + 5:
    err = returnRotation();                           // Возвращаем направление вращения
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 6;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case WorkState + 6:
    err = returnFreq();                               // Возвращаем частоту уставки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 7;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case WorkState + 7:
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 8;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case WorkState + 8:
    err = returnTimeDispersal();
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 9;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case WorkState + 9:
    logEvent.add(OtherCode, AutoType, RegimeRunPickupFailId); // FailId
    state_ = WorkState + 10;
    break;
  case WorkState + 10:
    if (vsd->isSetPointFreq()) {                      // Вышли на заданную частоту
      logEvent.add(OtherCode, AutoType, RegimeRunPickupFinishId);
      if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_PICKUP_MODE, OffAction);         // Выключаем режим
        logEvent.add(SetpointCode, AutoType, RegimeRunPickupOffId); // Записываем данные в лог
      }
      state_ = IdleState;
    }
    break;
  }
}

void RegimeRunPickup::processingStateStop()
{
  int16_t err = 0;
  switch (state_) {
  case StopState:
    err = returnRotation();                           // Возвращаем направление вращения
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = StopState + 1;                         // Переходим на состояние пуска с ошибкой
    }
    break;
  case StopState + 1:
    err = returnFreq();                               // Возвращаем частоту уставки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = StopState + 2;                         // Переходим на состояние пуска с ошибкой
    }
    break;
  case StopState + 2:
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = StopState + 3;                         // Переходим на состояние пуска с ошибкой
    }
    break;
  case StopState + 3:
    err = returnTimeDispersal();
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = StopState + 4;                         // Переходим на состояние пуска с ошибкой
    }
    break;
  case StopState + 4:                                // Состояния вывода ошибки задания параметров
    logEvent.add(OtherCode, AutoType, RegimeRunPickupAbortId);
    state_ = IdleState;
    break;
  }
}

void RegimeRunPickup::automatRegime()
{
  if ((action_ == OffAction) && (state_ != IdleState)) {
    state_ = StopState;
  }

  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
  case RunningState + 1:
  case RunningState + 2:
  case RunningState + 3:
  case RunningState + 4:
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
    if (ksu.isStopMotor()) {
      state_ = StopState;
    }
    else {
      processingStateWork();
    }
    break;
  case StopState:
  case StopState + 1:
  case StopState + 2:
  case StopState + 3:
  case StopState + 4:
    processingStateStop();
    break;
  default:
    state_ = StopState;
    break;
  }
}

void RegimeRunPickup::saveBeforeRegimeRun()
{
  // Сохраняем уставку частоты
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ,  parameters.get(VSD_FREQUENCY));
  // Сохраняем уставку минимальной частоты
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_LOW_LIM_FREQ,  parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  // Сохраняем уставку времени изменения частоты
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP, parameters.get(VSD_TIMER_DISPERSAL));
  // Сохраняем уставку направления вращения
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION, parameters.get(VSD_ROTATION));
}

int16_t RegimeRunPickup::setRotation()
{
  int16_t err = 0;
  err = setConfirmation(VSD_ROTATION, !parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::setRotation()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::returnRotation()
{
  int16_t err = 0;
  err = setConfirmation(VSD_ROTATION, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::returnRotation()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::setMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, 1);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::setMinFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_LOW_LIM_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::returnMinFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::setFreqWait()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_PICKUP_FREQ_WAIT));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::setFreqWait()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::returnFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::setTimeDispersal()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_PICKUP_TIME_UP));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::setTimeDispersal()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::setTimeDelay()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_PICKUP_TIME_DOWN));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::setTimeDelay()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::returnTimeDispersal()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPickup::returnTimeDispersal()");
    #endif
  }
  return err;
}

int16_t RegimeRunPickup::setConfirmation(uint16_t id, float value)
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

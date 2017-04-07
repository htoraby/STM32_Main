#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::getOtherSetpoint()
{
  // Уставки режима
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);    // Активность режима
  freqPush_ = parameters.get(CCS_RGM_RUN_PUSH_FREQ);  // Частота толчков
  quantityPush_ = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);    // Количество толчков
  voltagePush_ = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE);      // Напряжение толчков
  timePush_ = (1 / freqPush_) * 100;                  // Время толчка (счетчик * 100мс)

  // Текущие параметры режима
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);    // Состояние автомата режима
  cntPush_ = parameters.get(CCS_RGM_RUN_PUSH_COUNTER);// Счётчик толчков
  timer_ = parameters.get(CCS_RGM_RUN_PUSH_TIMER);    // Таймер-счётчик
}

void RegimeRunPush::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_PUSH_STATE, state_);
  parameters.set(CCS_RGM_RUN_PUSH_COUNTER, cntPush_);
  parameters.set(CCS_RGM_RUN_PUSH_TIMER, timer_);
}

void RegimeRunPush::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        cntPush_ = 0;                                         // Сброс счётчика толчков
        timer_ = 0;                                           // Сброс таймера счётчика
        saveBeforeRegimeRun();
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunPush::processingStateRunning()
{
  int16_t err = 0;
  switch (state_) {

  case RunningState:                                  // Состояние запуска
    err = setMinFreq();                               // Пытаем задать минимальную частоту
    if (!err) {                                       // Если задали минимальную частоту
      err = setFreqPush();                            // Пытаемся задать частоту толчка
      if (!err) {                                     // Если задали частоту толчка
        ksu.start(runReason_, true);                  // Подаём команду на пуск
        logEvent.add(OtherCode, AutoType, RegimeRunPushStartId);
        state_ = WorkState;                           // Переходим в состояние работа
      }
      else if (err > 0) {                             // Если не смогли задать частоту толчков
        state_ = RunningState + 1;                    // Переход на состояния возврата минимальной частоты
      }
    }
    else if (err > 0) {                               // Если не смогли задать минимальную частоту
      state_ = RunningState + 2;                      // Переход на состояния запуска
    }
    break;

  case RunningState + 1:                              // Состояния возврата минимальной частоты
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 2;                      // Переходим на состояние пуска с ошибкой
    }
    break;

  case RunningState + 2:
    ksu.start(runReason_, true);                      // Подаём команду на пуск
    state_ = WorkState + 7;                           // Переходим на состояние формирования ошибки задания параметров режима
    break;
  }
}

void RegimeRunPush::processingStateWork()
{
  int16_t err = 0;
  switch (state_) {

  case WorkState:                                     // Состояние ожидания выхода на частоту толчка
    if (vsd->isSetPointFreq()) {                      // Если вышли на частоту толчка
      timer_ = timePush_;                             // Инициируем таймер счётчик времени толчка
      state_ = WorkState + 1;                         // Переходим на состояние отсчёта времени
    }
    break;

  case WorkState + 1:                                 // Состояние работа (ожидания времени между толчками)
    if (timer_ <= 1) {                                // Таймер-счётчик истек, 1 на переход между состояниями
      err = setU1();                                  // Поднимаем напряжение в нижней точке диапазона
      if (!err) {                                     // Если подняли напряжение в нижней точке диапазона
        err = setU2();                                // Поднимаем напряжение в верхней точке диапазона
        if (!err) {                                   // Если подняли напряжение в верхней точке диапазона
          timer_ = 0;                                 // Таймер-счётчик в 0
          state_ = WorkState + 2;                     // Переходи на состояние (ожидание времени толчка)
        }
        else if (err > 0) {                           // Не подняли напряжение в верхней точке
          state_ = WorkState + 4;                     // Переход на состояние возврата нижней точки
        }
      }
      else if (err > 0) {                             // Не подняли напряжение в нижней точке
        state_ = WorkState + 5;                       // Переход на состояние возврата частоты уставки
      }
    }
    else {
      timer_--;                                       // Уменьшаем таймер-счётчик
    }
    break;

  case WorkState + 2:                                 // Состояние работа (ожидание времени толчка)
    if (timer_ >= timePush_ - 1) {                    // Время толчка истекло один на переход между состояними
      err = returnU2();                               // Возвращаем напряжение в нижней точке
      if (!err) {                                     // Если вернули напряжение в нижней точке
        err = returnU1();                             // Возвращаем напряжение в верхней точке
        if (!err) {                                   // Если вернули напряжение в верхней точке
          cntPush_++;                                 // Увеличиваем счётчик толчков
          if (cntPush_ >= quantityPush_) {            // Количество толчков больше равно уставке
            state_ = WorkState + 3;                   // Переход на состояние возврата настроек до режима
          }
          else {                                      // Не все толчки
            timer_ = timePush_;                       // Запускаем таймер
            state_ = WorkState + 1;
          }
        }
        else if (err > 0) {                           // Если не смогли вернуть напряжение в нижней точке
          state_ = WorkState + 5;                     // Переход на состояние возврата частоты уставки
        }
      }
      else if (err > 0) {                             // Не смогли вернуть напряжение в верхней точке
        state_ = WorkState + 4;                       // Переход на состояние возврата напряжения в нижней точке
      }
    }
    else {
      timer_++;
    }
    break;

  case WorkState + 3:                                 // Состояние возвращения настроек режима                                     // Если вернули
    err = returnFreq();                               // Возвращаем частоту уставки
    if (!err) {                                       // Если вернули уставку частоты
      err = returnMinFreq();                          // Возвращаем минимальную частоту
      if (!err) {                                     // Если вернули минимальную частоту
        state_ = WorkState + 8;                       // Переход на состояние выхода на заданную частоту
      }
      else if (err > 0) {                             // Не смогли вернуть минимальную частоту
        state_ = WorkState + 7;                       // Переход на состояние ошибки в режиме
      }
    }
    else if (err > 0) {                               // Не смогли вернуть частоту уставки
      state_ = WorkState + 6;                         // Переход на состояние возврата уставки минимальной частоты
    }
    break;

  case WorkState + 4:                                 // Состояние возврата нижней точки U/f
    err = returnU1();
    if ((!err) || (err > 0)) {
      state_ = StopState + 5;
    }
    break;
  case WorkState + 5:                                 // Состояние возврата частоты уставки
    err = returnFreq();
    if ((!err) || (err > 0)) {
      state_ = WorkState + 6;
    }
    break;
  case WorkState + 6:                                 // Состояние возврата минимальной частоты
    err = returnMinFreq();
    if ((!err) || (err > 0)) {
      state_ = WorkState + 7;
    }
    break;

  case WorkState + 7:
    logEvent.add(OtherCode, AutoType, RegimeRunPushFailId);
    state_ = WorkState + 8;
    break;

  case WorkState + 8:                                 // Состояние ожидания выхода на частоту работы
    if (vsd->isSetPointFreq()) {                      // Вышли на заданную частоту
      logEvent.add(OtherCode, AutoType, RegimeRunPushFinishId);
      if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
      }
      state_ = IdleState;
    }
    break;
  }
}

void RegimeRunPush::processingStateStop()             // Состояние остановки режима
{
  int16_t err = 0;
  switch (state_) {
  case StopState:                                     // Состояние возврата верхней точки U/f
    err = returnU2();
    if ((!err) || (err > 0)) {
      state_ = StopState + 1;
    }
    break;
  case StopState + 1:                                 // Состояние возврата нижней точки U/f
    err = returnU1();
    if ((!err) || (err > 0)) {
      state_ = StopState + 2;
    }
    break;
  case StopState + 2:                                 // Состояние возврата частоты уставки
    err = returnFreq();
    if ((!err) || (err > 0)) {
      state_ = StopState + 3;
    }
    break;
  case StopState + 3:                                 // Состояние возврата минимальной частоты
    err = returnMinFreq();
    if ((!err) || (err > 0)) {
      state_ = StopState + 4;
    }
    break;
  case StopState + 4:                                // Состояния вывода ошибки задания параметров
    logEvent.add(OtherCode, AutoType, RegimeRunPushAbortId);
    state_ = IdleState;
    break;
  }

}

void RegimeRunPush::automatRegime()
{
  // Выключили режим во время работы, переход на состояние остановки режима
  if ((action_ == OffAction) && (state_ != IdleState) && (ksu.isRunOrWorkMotor())) {
    state_ = StopState;
  }

  switch (state_) {
  case IdleState:                                     // Состояние ожидания
    processingStateIdle();
    break;
  case RunningState:                                  // Состояние запуска
  case RunningState + 1:                              //
  case RunningState + 2:                              //
    processingStateRunning();
    break;
  case WorkState:                                     // Состояние работы режима
  case WorkState + 1:
  case WorkState + 2:
  case WorkState + 3:
  case WorkState + 4:
  case WorkState + 5:
  case WorkState + 6:
  case WorkState + 7:
  case WorkState + 8:
    if (ksu.isBreakOrStopMotor()) {
      state_ = StopState;
      break;
    }
    processingStateWork();
    break;
  case StopState:                                     // Состояние остановки режима
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

void RegimeRunPush::saveBeforeRegimeRun()
{
  // Сохраняем уставку минимальной частоты
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_LOW_LIM_FREQ, parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  // Сохраняем уставку частоты
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_FREQ, parameters.get(VSD_FREQUENCY));
  // Сохраняем точки характеристики U/f по частоте
  if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_6));
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
  }
  else {
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_1));
    parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
  }
}

int16_t RegimeRunPush::setMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, 1);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::setMinFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_LOW_LIM_FREQ));
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::returnMinFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::setFreqPush()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, freqPush_);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::setFreqPush()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_FREQ));
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::returnFreq()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::setU1()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltageMax = parameters.get(VSD_UF_CHARACTERISTIC_U_6);
  float voltagePush = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U1) * voltagePush_ / 100;
  if (voltagePush > voltageMax) {
    voltagePush = voltageMax;
  }

  if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_1;
  }

  err = setConfirmation(point, voltagePush);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::setU1()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::returnU1()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltagePush = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U1);

  if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_1;
  }

  err = setConfirmation(point, voltagePush);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::returnU1()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::setU2()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltageMax = parameters.get(VSD_UF_CHARACTERISTIC_U_6);
  float voltagePush = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U2) * voltagePush_ / 100;
  if (voltagePush > voltageMax) {
    voltagePush = voltageMax;
  }

  if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_6;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }

  err = setConfirmation(point, voltagePush);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::setU2()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::returnU2()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltagePush = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U2);

  if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_6;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqPush_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }

  err = setConfirmation(point, voltagePush);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunPush::returnU2()");
#endif
  }
  return err;
}

int16_t RegimeRunPush::setConfirmation(uint16_t id, float value)
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

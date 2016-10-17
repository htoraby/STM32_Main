#include "regime_run_swing.h"

RegimeRunSwing::RegimeRunSwing()
{

}

RegimeRunSwing::~RegimeRunSwing()
{

}

void RegimeRunSwing::getOtherSetpoint()
{
  // Уставки режима
  action_ = parameters.get(CCS_RGM_RUN_SWING_MODE);   // Активность режима
  freqSwing_ = parameters.get(CCS_RGM_RUN_SWING_FREQ);// Частота качков
  quantitySwing_ = parameters.get(CCS_RGM_RUN_SWING_QUANTITY);  // Количество качков
  voltageSwing_ = parameters.get(CCS_RGM_RUN_SWING_VOLTAGE);    // Напряжение качков

  // Текущие параметры режима
  state_ = parameters.get(CCS_RGM_RUN_SWING_STATE);
  rotationNow_ = parameters.get(VSD_ROTATION);        // Текущее направления вращения
  rotationSave_ = parameters.get(CCS_RGM_RUN_SWING_ROTATION_SAVE);
  cntReverse_ = parameters.get(CCS_RGM_RUN_SWING_COUNTER);
}

void RegimeRunSwing::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_SWING_STATE, state_);
  parameters.set(CCS_RGM_RUN_SWING_ROTATION_SAVE, rotationSave_);
  parameters.set(CCS_RGM_RUN_SWING_COUNTER, cntReverse_);
}

void RegimeRunSwing::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        cntReverse_ = 0;
        saveBeforeRegimeRun();
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunSwing::processingStateRunning()
{
  int16_t err = 0;
  switch (state_) {
  case RunningState:                                  // Состояние запуска
    err = setMinFreq();                               // Пытаем задать минимальную частоту
    if (!err) {                                       // Если задали минимальную частоту
      err = setFreqSwing();                           // Пытаемся задать частоту качков
      if (!err) {                                     // Задали частоту качков
        err = setTimeDispersal();                     // Пытаемся задать время изменения частоты
        if (!err) {                                   // Задали время изменения частоты
          err = setU1();                              // Пытаемся задать напряжение в нижней точке диапазона
          if (!err) {                                 // Задали напряжение в нижней точке диапазона
            err = setU2();                            // Пытаемся задать напряжение в верхней точке диапазона
            if (!err) {                               // Задали напряжение в верхней точке диапазона
              ksu.start(runReason_);                  // Подаём команду на пуск
              logEvent.add(OtherCode, AutoType, RegimeRunSwingStartId);
              state_ = WorkState;                     // Переходим в состояние работа
            }
            else if (err > 0) {                       // Не смогли задать напряжение в верхней точке
              state_ = RunningState + 1;              // Переходим на состояние возврата в нижней точке
            }
          }
          else if (err > 0) {                         // Не смогли задать напряжение в нижней точке
            state_ = RunningState + 2;                // Переходим на состояние возврата времени изменения частоты
          }
        }
        else if (err > 0) {                           // Не смогли задать время изменения частоты
          state_ = RunningState + 3;                  // Переходим на состояние возврата частоты уставки
        }
      }
      else if (err > 0) {                             // Не смогли задать частоту качков
        state_ = RunningState + 4;                    // Переходим на состояние возврата минимальной частоты
      }
    }
    else if (err > 0) {                               // Не смогли задать минимальную частоту
      state_ = RunningState + 5;                      // Переходим на запуск
    }
    break;
  case RunningState + 1:
    err = returnU1();                                 // Возвращаем напряжение в нижней точке
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 2;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 2:
    err = returnTimeDispersal();                      // Возвращаем время набора частоты
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 3;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 3:
    err = returnFreq();                               // Возвращаем частоту уставки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 4;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 4:
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = RunningState + 5;                      // Переходим на состояние пуска с ошибкой
    }
    break;
  case RunningState + 5:
    ksu.start(runReason_);                            // Подаём команду на пуск
    state_ = WorkState + 9;                           // Состояние формирования ошибки режима
    break;
  }
}

void RegimeRunSwing::processingStateWork()
{
  int16_t err = 0;
  switch (state_) {
  case WorkState:                                     // Состояние ожидания выхода на заданную частоту
    if (vsd->isSetPointFreq()) {                      // Вышли на заданную частоту
      rotationSave_ = rotationNow_;                   // Запоминаем направление вращения
      state_ = WorkState + 1;
    }
    break;
  case WorkState + 1:
    if (rotationNow_ != rotationSave_) {              // Если направления вращения не равно сохранённому, т.е. изменилось
      cntReverse_++;                                  // Увеличиваем счётчик изменений направлений вращения
      repeat_ = 0;
      if (cntReverse_ >= quantitySwing_ * 2) {        // Если сделали все качки
        cntReverse_ = 0;
        state_ = WorkState + 2;                       // Переходим к состоянию возврата настроек
      }
      else {
        state_ = WorkState + 11;
      }
    }
    else {                                            // Меняем направление вращения на противоположное
      err = setRotation();
      if (err > 0) {
        state_ = WorkState + 3;
      }
    }
    break;
  case WorkState + 2:                                 // Состояние возврата настроек
    err = returnRotation();                           // Возвращаем направление вращения
    if (!err) {                                       // Если вернули направление вращения
      err = returnU2();                               // Возвращаем напряжение в верхней точке
      if (!err) {                                     // Если вернули напряжение в верхней точке
        err = returnU1();                             // Возвращаем напряжение в нижней точке
        if (!err) {                                   // Если вернули напряжение в нижней точке
          err = returnTimeDispersal();                // Возвращем время изменения частоты
          if (!err) {                                 // Если вернули время изменения частоты
            err = returnFreq();                       // Возвращаем уставку частоты
            if (!err) {                               // Если вернули уставку частоты
              err = returnMinFreq();                  // Возвращаем минимальную частоту
              if (!err) {                             // Вернули минимальную частоту
                state_ = WorkState + 10;              // Ожидание выхода на частоту уставки
              }
              else if (err > 0) {                     // Не смогли вернуть минимальную частоту
                state_ = WorkState + 9;               // Переход на состояние формирования ошибки
              }
            }
            else if (err > 0) {                       // Не смогли вернуть уставку частоты
              state_ = WorkState + 8;
            }
          }
          else if (err > 0) {                         // Не смогли вернуть время изменения частоты
            state_ = WorkState + 7;
          }
        }
        else if (err > 0) {                           // Не смогли вернуть напряжение в нижней точке
          state_ = WorkState + 6;
        }
      }
      else if (err > 0) {                             // Не смогли вернуть напряжение в верхней точке
        state_ = WorkState + 5;
      }
    }
    else if (err > 0) {                               // Не смогли вернуть направления вращения
      state_ = WorkState + 4;
    }
    break;
  case WorkState + 3:
    err = returnRotation();                           // Возвращаем направление вращения
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 4;                         // Переходим на состояние возврата напряжения нижней точки
    }
    break;
    break;
  case WorkState + 4:
    err = returnU2();                                 // Возвращаем напряжение верхней точки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 5;                         // Переходим на состояние возврата напряжения нижней точки
    }
    break;
  case WorkState + 5:
    err = returnU1();                                 // Возвращаем напряжение нижней точки
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 6;                         // Переходим на состояние возврата времени изменения частоты
    }
    break;
  case WorkState + 6:
    err = returnTimeDispersal();                      // Возвращаем времени изменения частоты
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 7;                         // Переходим на состояние возврата уставки частоты
    }
    break;
  case WorkState + 7:
    err = returnFreq();                               // Возвращаем уставки частоты
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 8;                         // Переходим на состояние возврата минимальной частоты
    }
    break;
  case WorkState + 8:
    err = returnMinFreq();                            // Возвращаем минимальную частоты
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = WorkState + 9;                         // Переходим на состояние формирования ошибки
    }
    break;
  case WorkState + 9:
    logEvent.add(OtherCode, AutoType, RegimeRunSwingFailId);
    state_ = WorkState + 10;
    break;
  case WorkState + 10:                                // Состояние ожидания выхода на частоту работы
    if (vsd->isSetPointFreq()) {                      // Вышли на заданную частоту
      logEvent.add(OtherCode, AutoType, RegimeRunSwingFinishId);
      if (parameters.get(CCS_RGM_RUN_SWING_MODE) == SingleAction) {
        parameters.set(CCS_RGM_RUN_SWING_MODE, OffAction);         // Выключаем режим
        logEvent.add(SetpointCode, AutoType, RegimeRunSwingOffId); // Записываем данные в лог
      }
      state_ = IdleState;
    }
    break;
  case WorkState + 11:
    delayRotation_ ++;
    if (delayRotation_ >= 10) {
      delayRotation_ = 0;
      state_ = WorkState;
    }
    break;
  }
}

void RegimeRunSwing::processingStateStop()
{
  int16_t err = 0;
  switch (state_) {
  case StopState:
    err = returnRotation();
    if ((!err) || (err > 0)) {
      state_ = StopState + 1;
    }
    break;
  case StopState + 1:
    err = returnU2();
    if ((!err) || (err > 0)) {
      state_ = StopState + 2;
    }
    break;
  case StopState + 2:                                 // Состояние возврата нижней точки U/f
    err = returnU1();
    if ((!err) || (err > 0)) {
      state_ = StopState + 3;
    }
    break;
  case StopState + 3:                                 // Состояние возврата нижней точки U/f
    err = returnTimeDispersal();                      // Возвращаем времени изменения частоты
    if ((!err) || (err > 0)) {                        // Вернули или использовали все попытки
      state_ = StopState + 4;                         // Переходим на состояние возврата уставки частоты
    }
    break;
  case StopState + 4:                                 // Состояние возврата частоты уставки
    err = returnFreq();
    if ((!err) || (err > 0)) {
      state_ = StopState + 5;
    }
    break;
  case StopState + 5:                                 // Состояние возврата минимальной частоты
    err = returnMinFreq();
    if ((!err) || (err > 0)) {
      state_ = StopState + 6;
    }
    break;
  case StopState + 6:                                // Состояния вывода ошибки задания параметров
    logEvent.add(OtherCode, AutoType, RegimeRunSwingAbortId);
    state_ = IdleState;
    break;
  }
}


void RegimeRunSwing::automatRegime()
{
  if ((action_ == OffAction) && (state_ != IdleState)) {
    state_ = StopState;
  }

  update_ ++;
  if (update_ >= 3) {
    vsd->readInDevice(VSD_ROTATION);
    update_ = 0;
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
  case RunningState + 5:
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
    if (ksu.isStopMotor()) {
      state_ = StopState;
      break;
    }
    processingStateWork();
    break;
  case StopState:
  case StopState + 1:
  case StopState + 2:
  case StopState + 3:
  case StopState + 4:
  case StopState + 5:
  case StopState + 6:
    processingStateStop();
    break;
  default:
    state_ = StopState;
    break;
  }
}

void RegimeRunSwing::saveBeforeRegimeRun()
{
  // Сохраняем уставку минимальной частоты
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_LOW_LIM_FREQ,  parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  // Сохраняем уставку частоты
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_FREQ,  parameters.get(VSD_FREQUENCY));
  // Сохраняем изменения частоты
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_TIME_UP, parameters.get(VSD_TIMER_DISPERSAL));
  // Сохраняем уставка направления вращения
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_ROTATION, parameters.get(VSD_ROTATION));
  // Сохраняем точки характеристики U/f по частоте
  if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_6));
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
  }
  else {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_1));
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
  }
}

int16_t RegimeRunSwing::setMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, 1);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setMinFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_LOW_LIM_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnMinFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setFreqSwing()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, freqSwing_);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setFreqSwing");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnFreq");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setTimeDispersal()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, (((1 / freqSwing_) * 10 * parameters.get(VSD_MOTOR_FREQUENCY)) / freqSwing_));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setTimeDispersal");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnTimeDispersal()
{
  int16_t err = setConfirmation(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_TIME_UP));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnTimeDispersal");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setU1()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltageMax = parameters.get(VSD_UF_CHARACTERISTIC_U_6);
  float voltageSwing = parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1) * voltageSwing_ / 100;
  if (voltageSwing > voltageMax) {
    voltageSwing = voltageMax;
  }

  if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_1;
  }

  err = setConfirmation(point, voltageSwing);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setU1");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnU1()
{
  int16_t err = 0;
  uint16_t point = 0;

  if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_1;
  }

  err = setConfirmation(point, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnU1");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setU2()
{
  int16_t err = 0;
  uint16_t point = 0;
  float voltageMax = parameters.get(VSD_UF_CHARACTERISTIC_U_6);
  float voltageSwing = parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2) * voltageSwing_ / 100;
  if (voltageSwing > voltageMax) {
    voltageSwing = voltageMax;
  }

  if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_6;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }

  err = setConfirmation(point, voltageSwing);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setU2");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnU2()
{
  int16_t err = 0;
  uint16_t point = 0;

  if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    point = VSD_UF_CHARACTERISTIC_U_6;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
    point = VSD_UF_CHARACTERISTIC_U_5;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
    point = VSD_UF_CHARACTERISTIC_U_4;
  }
  else if (freqSwing_ >= parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
    point = VSD_UF_CHARACTERISTIC_U_3;
  }
  else {
    point = VSD_UF_CHARACTERISTIC_U_2;
  }

  err = setConfirmation(point, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnU2");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setRotation()
{
  int16_t err = 0;
  err = setConfirmation(VSD_ROTATION, !rotationSave_);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: setRotation");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::returnRotation()
{
  int16_t err = 0;
  err = setConfirmation(VSD_ROTATION, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_ROTATION));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "Run swing: returnRotation");
    #endif
  }
  return err;
}

int16_t RegimeRunSwing::setConfirmation(uint16_t id, float value)
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




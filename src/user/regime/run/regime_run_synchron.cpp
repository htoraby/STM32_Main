#include "regime_run_synchron.h"

RegimeRunSynchron::RegimeRunSynchron()
{

}

RegimeRunSynchron::~RegimeRunSynchron()
{

}

void RegimeRunSynchron::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_SYNCHRON_MODE);
  state_ = parameters.get(CCS_RGM_RUN_SYNCHRON_STATE);
}

void RegimeRunSynchron::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_SYNCHRON_STATE, state_);
}

void RegimeRunSynchron::saveBeforeRegimeRun()
{
  parameters.set(CCS_RGM_RUN_SYNCHRON_SETPOINT_FREQ, parameters.get(VSD_FREQUENCY));
  parameters.set(CCS_RGM_RUN_SYNCHRON_SETPOINT_MIN_FREQ, parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
}

void RegimeRunSynchron::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        saveBeforeRegimeRun();
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunSynchron::processingStateRunning()
{
  int16_t err = -1;
  err = setMinFreq();                                 // Задаём минимальную частоту
  if (!err) {                                         // Задали минимальную частоту
    err = setFreqSynchron();                          // Задаём частоту синхронизации
    if (!err) {                                       // Задали частоту синхронизации
      ksu.start(runReason_);                          // Команда на запуск ЧРП
      state_ = WorkState;                             // Переход на состояние работа
    }
  }
}

void RegimeRunSynchron::processingStateWork()
{
  switch (state_) {
  case WorkState:                                     // Состояние разгона до частоты синхронизации
    if (vsd->isSetPointFreq()) {                      // Если достигли частоты синхронизации
      beginSync_ = ksu.getTime();                     // Запоминаем время выхода на частоту синхронизации
      state_ = WorkState + 1;                         // Переход на состояние отсчёта времени синхронизации
      logEvent.add(OtherCode, AutoType, RegimeRunSynchronStartId);
    }
    break;
  case WorkState + 1:                                 // Состояние времени синхронизации
    uint32_t time = ksu.getSecFromCurTime(beginSync_);// Вычисляем время с
    if (time >= parameters.get(CCS_RGM_RUN_SYNCHRON_TIME)) {                          // Если время работы на частоте синхронизаци истекло
      logEvent.add(OtherCode, AutoType, RegimeRunSynchronFinishId);
      state_ = StopState;                             // Возвращаем настройки
    }
    break;
  }
}

void RegimeRunSynchron::processingStateStop()
{
  int16_t err = -1;
  err = returnFreq();                                 // Возвращаем частоту уставки
  if (!err) {
    err = returnMinFreq();                            // Возвращаем минимальную частоту
    if (!err) {
      offRegime();                                    // Выключение режима
      state_ = IdleState;
    }
  }
}

void RegimeRunSynchron::automatRegime()
{
  if ((action_ == OffAction) && (state_ != IdleState)) {
    logEvent.add(OtherCode, AutoType, RegimeRunSynchronFailId);
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
    processingStateWork();
    break;
  case StopState:
    processingStateStop();
    break;
  }
}

void RegimeRunSynchron::offRegime()
{
  if (parameters.get(CCS_RGM_RUN_SYNCHRON_MODE) == SingleAction) {
    parameters.set(CCS_RGM_RUN_SYNCHRON_MODE, OffAction);         // Выключаем режим
    logEvent.add(SetpointCode, AutoType, RegimeRunSynchronOffId); // Записываем данные в лог
  }
}

int16_t RegimeRunSynchron::setMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, 1);
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSynchron::setMinFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunSynchron::returnMinFreq()
{
  int16_t err = setConfirmation(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_SYNCHRON_SETPOINT_MIN_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSynchron::returnMinFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunSynchron::setFreqSynchron()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_SYNCHRON_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSynchron::setFreqSynchron()");
    #endif
  }
  return err;
}

int16_t RegimeRunSynchron::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_SYNCHRON_SETPOINT_FREQ));
  if (err == err_r) {
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeRunSynchron::returnFreq()");
    #endif
  }
  return err;
}

int16_t RegimeRunSynchron::setConfirmation(uint16_t id, float value)
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


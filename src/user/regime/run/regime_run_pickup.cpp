#include "regime_run_pickup.h"

RegimeRunPickup::RegimeRunPickup()
{

}

RegimeRunPickup::~RegimeRunPickup()
{

}

void RegimeRunPickup::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PICKUP_MODE);
  state_ = parameters.get(CCS_RGM_RUN_PICKUP_STATE);
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
        onRegime();
        state_ = RunningState;
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "RegimeRunPickup::processingStateIdle() IdleState --> RunningState");
        #endif
      }
    }
  }
}

void RegimeRunPickup::processingStateRunning()
{
  switch (state_) {
  case RunningState:
    delay_ ++;
    if (delay_ >= 10) {
      delay_ = 0;
      state_ = RunningState + 1;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateRunning() RunningState  --> RunningState + 1");
      #endif
    }
    break;
  case RunningState + 1:
    if (checkOnRegime()) {
      ksu.start(runReason_);
      logEvent.add(OtherCode, AutoType, RegimeRunPickupStartId);
      state_ = WorkState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateRunning() RunningState --> WorkState");
      #endif
    }
    else {
      onRegime();
      state_ = RunningState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateRunning() RunningState + 1 --> RunningState");
      #endif
    }
  }
}

void RegimeRunPickup::processingStateWork()
{
  switch (state_) {
  case WorkState:
    if (parameters.get(VSD_ROTATION) != parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION)) {
      if (parameters.get(VSD_FREQUENCY_NOW) >= parameters.get(CCS_TURBO_ROTATION_NOW)) {
        parameters.set(VSD_ROTATION, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION), NoneType);
      }
    }
    else {
      state_ = WorkState + 1;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateWork() WorkState --> WorkState + 1");
      #endif
    }
    break;
  case WorkState + 1:
    delay_ ++;
    if (delay_ >= 10) {
      delay_ = 0;
      state_ = WorkState + 2;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateWork() WorkState + 1 --> WorkState + 2");
      #endif
    }
    break;
  case WorkState + 2:
    if ((parameters.get(VSD_FREQUENCY) == parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ))
      && (parameters.get(VSD_TIMER_DISPERSAL) == parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP))) {
      state_ = WorkState + 3;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateWork() WorkState + 2 --> WorkState + 3");
      #endif
    }
    else {
      parameters.set(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ), NoneType);
      parameters.set(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP), NoneType);
    }
    break;
  case WorkState + 3:
    if (vsd->isSetPointFreq()) {
      state_ = StopState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "RegimeRunPickup::processingStateWork() WorkState + 3 --> StopState");
      #endif
    }
    break;
  }
}

void RegimeRunPickup::processingStateStop()
{
  if (checkOffRegime()) {
    logEvent.add(OtherCode, AutoType, RegimeRunPickupFinishId);
    state_ = IdleState;
    #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "RegimeRunPickup::processingStateWork() StopState --> IdleState");
    #endif
  }
  else {
    offRegime();
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
    processingStateRunning();
    break;
  case WorkState:
  case WorkState + 1:
  case WorkState + 2:
  case WorkState + 3:
    if (ksu.isStopMotor()) {
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

void RegimeRunPickup::saveBeforeRegimeRun()
{
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ,  parameters.get(VSD_FREQUENCY));
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_LOW_LIM_FREQ,  parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP, parameters.get(VSD_TIMER_DISPERSAL));
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_DOWN, parameters.get(VSD_TIMER_DELAY));
  parameters.set(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION, parameters.get(VSD_ROTATION));
}

void RegimeRunPickup::onRegime()
{
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, 1, NoneType);
  parameters.set(VSD_TIMER_DISPERSAL, parameters.get(CCS_TURBO_ROTATION_NOW), NoneType);
  parameters.set(VSD_TIMER_DELAY, parameters.get(CCS_TURBO_ROTATION_NOW), NoneType);
  parameters.set(VSD_FREQUENCY, parameters.get(CCS_TURBO_ROTATION_NOW), NoneType);
  parameters.set(VSD_ROTATION, !parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION), NoneType);
}

void RegimeRunPickup::offRegime()
{
  ksu.setFreq(parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ), NoneType);
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_LOW_LIM_FREQ), NoneType);
  parameters.set(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP), NoneType);
  parameters.set(VSD_TIMER_DELAY, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_DOWN), NoneType);
  parameters.set(VSD_ROTATION, parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION), NoneType);

  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) == SingleAction) {
    parameters.set(CCS_RGM_RUN_PICKUP_MODE, OffAction);         // Выключаем режим
    logEvent.add(SetpointCode, AutoType, RegimeRunPickupOffId); // Записываем данные в лог
  }
}

bool RegimeRunPickup::checkOnRegime()
{
  if ((parameters.get(VSD_ROTATION) != parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION))
    && (parameters.get(VSD_LOW_LIM_SPEED_MOTOR) == 1)) {
    return true;
  }
  return false;
}

bool RegimeRunPickup::checkOffRegime()
{
  if ((parameters.get(VSD_FREQUENCY) == parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_FREQ))
    && (parameters.get(VSD_LOW_LIM_SPEED_MOTOR) == parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_LOW_LIM_FREQ))
    && (parameters.get(VSD_TIMER_DISPERSAL) ==  parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_UP))
    && (parameters.get(VSD_TIMER_DELAY) ==  parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_TIME_DOWN))
    && (parameters.get(VSD_ROTATION) == parameters.get(CCS_RGM_RUN_PICKUP_SETPOINT_ROTATION))) {
    return true;
  }
  return false;
}

#include "regime_run_novomet.h"

RegimeRunNovomet::RegimeRunNovomet()
{

}

RegimeRunNovomet::~RegimeRunNovomet()
{

}

void RegimeRunNovomet::getGeneralSetpoint()
{
  runReason_= (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
}

void RegimeRunNovomet::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_VSD_STATE, state_);
}

void RegimeRunNovomet::setGeneralSetPoint()
{

}

void RegimeRunNovomet::getOtherSetpoint()
{
  action_ = (parameters.get(CCS_RGM_RUN_PICKUP_MODE) ||
             parameters.get(CCS_RGM_RUN_PUSH_MODE) ||
             parameters.get(CCS_RGM_RUN_SWING_MODE));
  state_ = parameters.get(CCS_RGM_RUN_VSD_STATE);
}

void RegimeRunNovomet::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunNovomet::processingStateRunning()
{
  int pickupRun = 0;
  int pushRun = 0;
  int swingRun = 0;
  int autoRun = 0;

  float queue[5];
  if (parameters.get(VSD_MOTOR_CONTROL) == VSD_MOTOR_CONTROL_UF)
    for (int i = 0; i < 5; i++) {
      queue[i] = VSD_REQULATOR_QUEUE_UF;
    }
  else if (parameters.get(VSD_MOTOR_CONTROL) == VSD_MOTOR_CONTROL_VECT) {
    for (int i = 0; i < 5; i++) {
      queue[i] = VSD_REQULATOR_QUEUE_VECT;
    }
  }

  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE)) {
    queue[pickupRun] = VSD_REQULATOR_QUEUE_PICKUP;
    pushRun++;
    swingRun++;
    autoRun++;
    vsd->onRegimePickup();
  }

  if (parameters.get(CCS_RGM_RUN_PUSH_MODE)) {
    queue[pushRun] = VSD_REQULATOR_QUEUE_PUSH;
    swingRun++;
    autoRun++;
    vsd->onRegimePush();
  }

  if (parameters.get(CCS_RGM_RUN_SWING_MODE)) {
    queue[swingRun] = VSD_REQULATOR_QUEUE_PUSH;
    autoRun++;
    vsd->onRegimeSwing();
  }

  parameters.set(VSD_REGULATOR_QUEUE_1, queue[0]);
  parameters.set(VSD_REGULATOR_QUEUE_2, queue[1]);
  parameters.set(VSD_REGULATOR_QUEUE_3, queue[2]);
  parameters.set(VSD_REGULATOR_QUEUE_4, queue[3]);
  parameters.set(VSD_REGULATOR_QUEUE_5, queue[4]);

  ksu.start(runReason_);
  state_ = WorkState;
}

void RegimeRunNovomet::processingStateWork()
{
  if (vsd->getCurrentFreq() == vsd->getSetpointFreq()){         // Если режимы отработали
    if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) == SingleAction) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
      logEvent.add(SetpointCode, AutoType, RegimeRunPickupOffId); // Записываем данные в лог
    }
    if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == SingleAction) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
      logEvent.add(SetpointCode, AutoType, RegimeRunPushOffId); // Записываем данные в лог
    }
    if (parameters.get(CCS_RGM_RUN_SWING_MODE) == SingleAction) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
      logEvent.add(SetpointCode, AutoType, RegimeRunSwingOffId); // Записываем данные в лог
    }
    state_ = IdleState;
  }
}

void RegimeRunNovomet::automatRegime()
{
  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
    processingStateWork();
    break;
  default:
    break;
  }
}

void RegimeRunNovomet::processing()
{
  getGeneralSetpoint();
  getOtherSetpoint();
  automatRegime();
  setGeneralSetPoint();
  setOtherSetpoint();
}




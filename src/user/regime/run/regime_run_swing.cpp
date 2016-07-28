#include "regime_run_swing.h"

RegimeRunSwing::RegimeRunSwing()
{

}

RegimeRunSwing::~RegimeRunSwing()
{

}

void RegimeRunSwing::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_SWING_MODE);
  state_ = parameters.get(CCS_RGM_RUN_SWING_STATE);
  freq_ = parameters.get(CCS_RGM_RUN_SWING_FREQ);
  quantity_ = parameters.get(CCS_RGM_RUN_SWING_QUANTITY);
  voltage_ = parameters.get(CCS_RGM_RUN_SWING_VOLTAGE);
}

void RegimeRunSwing::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_SWING_STATE, state_);
}

void RegimeRunSwing::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
}

void RegimeRunSwing::processingStateRunning()
{
  if (checkOnRegime()) {
    ksu.start(runReason_);
    state_ = WorkState;
  }
  else {
    onRegime();
    ksu.setFreq(freq_, NoneType, false);
  }
}

void RegimeRunSwing::processingStateWork()
{
  switch (state_) {
  case WorkState:
    if (vsd->isSetPointFreq()) {
      rotation_ = parameters.get(VSD_ROTATION);
      state_ = WorkState + 1;
    }
    break;
  case WorkState + 1:
    if (rotation_ != parameters.get(VSD_ROTATION)) {
      cntReverse_++;
      if (cntReverse_ >= quantity_ * 2) {
        state_ = StopState;
      }
      else {
        state_ = WorkState;
      }
    }
    else {
      vsd->reverseRotation();
    }
    break;
  }
}

void RegimeRunSwing::processingStateStop()
{
  if (parameters.get(CCS_RGM_RUN_SWING_SETPOINT_ROTATION) == parameters.get(VSD_ROTATION)) {
    offRegime();
    state_ = IdleState;
  }
  else {
    vsd->reverseRotation();
  }
}

void RegimeRunSwing::automatRegime()
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

void RegimeRunSwing::onRegime()
{
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_FREQ,  parameters.get(VSD_FREQUENCY));
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_LOW_LIM_FREQ,  parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_TIME_UP, parameters.get(VSD_TIMER_DISPERSAL));
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_TIME_DOWN, parameters.get(VSD_TIMER_DELAY));
  parameters.set(CCS_RGM_RUN_SWING_SETPOINT_ROTATION, parameters.get(VSD_ROTATION));

  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, 1);
  float nomFreq = parameters.get(VSD_MOTOR_FREQUENCY);
  float time = ((1 / freq_) * 10 * nomFreq) / freq_;
  parameters.set(VSD_TIMER_DISPERSAL, time);
  parameters.set(VSD_TIMER_DELAY, time);

  if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_6));
    parameters.set(VSD_UF_CHARACTERISTIC_U_6, parameters.get(VSD_UF_CHARACTERISTIC_U_6) * voltage_ / 100);
    parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
    parameters.set(VSD_UF_CHARACTERISTIC_U_5, parameters.get(VSD_UF_CHARACTERISTIC_U_5) * voltage_ / 100);
  }
  else {
    if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
      parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
      parameters.set(VSD_UF_CHARACTERISTIC_U_5, parameters.get(VSD_UF_CHARACTERISTIC_U_5) * voltage_ / 100);
      parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
      parameters.set(VSD_UF_CHARACTERISTIC_U_4, parameters.get(VSD_UF_CHARACTERISTIC_U_4) * voltage_ / 100);
    }
    else {
      if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
        parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
        parameters.set(VSD_UF_CHARACTERISTIC_U_4, parameters.get(VSD_UF_CHARACTERISTIC_U_4) * voltage_ / 100);
        parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
        parameters.set(VSD_UF_CHARACTERISTIC_U_3, parameters.get(VSD_UF_CHARACTERISTIC_U_3) * voltage_ / 100);
      }
      else {
        if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
          parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
          parameters.set(VSD_UF_CHARACTERISTIC_U_3, parameters.get(VSD_UF_CHARACTERISTIC_U_3) * voltage_ / 100);
          parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
          parameters.set(VSD_UF_CHARACTERISTIC_U_2, parameters.get(VSD_UF_CHARACTERISTIC_U_2) * voltage_ / 100);
        }
        else {
          parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
          parameters.set(VSD_UF_CHARACTERISTIC_U_2, parameters.get(VSD_UF_CHARACTERISTIC_U_2) * voltage_ / 100);
          parameters.set(CCS_RGM_RUN_SWING_SETPOINT_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_1));
          parameters.set(VSD_UF_CHARACTERISTIC_U_1, parameters.get(VSD_UF_CHARACTERISTIC_U_1) * voltage_ / 100);
        }
      }
    }
  }
}

bool RegimeRunSwing::checkOnRegime()
{
  if ((parameters.get(VSD_LOW_LIM_SPEED_MOTOR) == 1)
   &&(parameters.get(VSD_FREQUENCY) == parameters.get(CCS_RGM_RUN_SWING_FREQ))) {
    return true;
  }
  return false;
}

void RegimeRunSwing::offRegime()
{
  if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    parameters.set(VSD_UF_CHARACTERISTIC_U_6, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
    parameters.set(VSD_UF_CHARACTERISTIC_U_5, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
  }
  else {
    if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
      parameters.set(VSD_UF_CHARACTERISTIC_U_5, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
      parameters.set(VSD_UF_CHARACTERISTIC_U_4, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
    }
    else {
      if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
        parameters.set(VSD_UF_CHARACTERISTIC_U_4, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
        parameters.set(VSD_UF_CHARACTERISTIC_U_3, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
      }
      else {
        if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
          parameters.set(VSD_UF_CHARACTERISTIC_U_3, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
          parameters.set(VSD_UF_CHARACTERISTIC_U_2, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
        }
        else {
          parameters.set(VSD_UF_CHARACTERISTIC_U_2, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U2));
          parameters.set(VSD_UF_CHARACTERISTIC_U_1, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_U1));
        }
      }
    }
  }
  parameters.set(VSD_TIMER_DISPERSAL, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_TIME_UP));
  parameters.set(VSD_TIMER_DELAY, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_TIME_DOWN));
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_LOW_LIM_FREQ));
  parameters.set(VSD_FREQUENCY, parameters.get(CCS_RGM_RUN_SWING_SETPOINT_FREQ));
}





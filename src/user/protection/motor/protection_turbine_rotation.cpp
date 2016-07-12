#include "protection_turbine_rotation.h"

ProtectionTurbineRotation::ProtectionTurbineRotation()
{
  idMode_= CCS_PROT_MOTOR_ASYNC_MODE;
  idPrevent_= CCS_PROT_MOTOR_ASYNC_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_ASYNC_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_ASYNC_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_ASYNC_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_ASYNC_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_ASYNC_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_ASYNC_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_MOTOR_ASYNC_DELAY_FLAG;
  idState_ = CCS_PROT_MOTOR_ASYNC_STATE;
  idTimer_ = CCS_PROT_MOTOR_ASYNC_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_ASYNC_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME;
}

ProtectionTurbineRotation::~ProtectionTurbineRotation()
{

}

bool ProtectionTurbineRotation::checkPrevent()
{
  valueParameter_ = parameters.get(CCS_TURBO_ROTATION_NOW);
  if ((parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) &&
      (valueParameter_ > tripSetpoint_)) {
    return true;
  }
  return false;
}

void ProtectionTurbineRotation::automatProtection()
{
  if (isModeOff()) {
    state_ = StateStop;
    return;
  }
  if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
    if (state_ == StateStop)
      state_ = StateRunning;
  } else {
    state_ = StateStop;
  }

  if (prevent_) {
    if (state_ == StateRunning) {
      state_ = StateRun;
      logEvent.add(ProtectCode, AutoType, TurbineRotationBeginId, tripSetpoint_, valueParameter_);
    }
  } else {
    if (state_ == StateRun) {
      state_ = StateStopping;
      logEvent.add(ProtectCode, AutoType, TurbineRotationEndId, tripSetpoint_, valueParameter_);
    }
  }
}

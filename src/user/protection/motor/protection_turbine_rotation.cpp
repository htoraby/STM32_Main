#include "protection_turbine_rotation.h"

ProtectionTurbineRotation::ProtectionTurbineRotation()
  : isStartProtect_(false)
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
  float value = parameters.get(CCS_TURBO_ROTATION_NOW);
  if ((parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) &&
      value > tripSetpoint_) {
    return true;
  }
  return false;
}

void ProtectionTurbineRotation::automatProtection()
{
  if (isModeOff()) {
    isStartProtect_ = false;
    return;
  }

  if (prevent_) {
    if (!isStartProtect_) {
      isStartProtect_ = true;
      logEvent.add(ProtectCode, AutoType, TurbineRotationBeginId);
    }
  } else {
    if (isStartProtect_) {
      isStartProtect_ = false;
      logEvent.add(ProtectCode, AutoType, TurbineRotationEndId);
    }
  }
}

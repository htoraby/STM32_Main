#include "protection_turbine_rotation.h"

ProtectionTurbineRotation::ProtectionTurbineRotation()
  : isStartProtect_(false)
{
  idMode_ = CCS_PROT_MOTOR_ASYNC_MODE;
  idReaction_= CCS_PROT_MOTOR_ASYNC_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_ASYNC_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_ASYNC_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_ASYNC_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_ASYNC_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_ASYNC_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_ASYNC_MODE;
  idTimer_ = CCS_PROT_MOTOR_ASYNC_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_ASYNC_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_ASYNC_RESTART_RESET_COUNT;
}

ProtectionTurbineRotation::~ProtectionTurbineRotation()
{

}

void ProtectionTurbineRotation::init()
{
  Protection::init("ProtTurbineRotation");
}

bool ProtectionTurbineRotation::checkBlock()
{
  bool block = false;
  float value = parameters.getValue(CCS_TURBO_ROTATION_NOW);
  if ((ksu.isStopCCS() || ksu.isWaitCCS() || ksu.isBlockCCS())
      && (value > tripSetpoint_)) {
    block = true;
  }
  return block;
}

void ProtectionTurbineRotation::automatProtection()
{
  if (mode_ == 0) {
    return;
  }

  if (block_) {
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

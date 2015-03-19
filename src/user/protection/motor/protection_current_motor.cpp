#include "protection_current_motor.h"
#include "user_main.h"

ProtectionCurrentMotor::ProtectionCurrentMotor()
{
  idReaction_= CCS_PROT_MOTOR_CURRENT_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_CURRENT_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_CURRENT_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_CURRENT_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_CURRENT_MODE;
  idTimer_ = CCS_PROT_MOTOR_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_CURRENT_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_CURRENT_RESTART_RESET_COUNT;

  protReactionEventId_ = CurrentMotorProtReactId;
  apvEventId_ = CurrentMotorApvId;
  apvDisabledEventId_ = CurrentMotorApvDisabledId;
  protBlockedEventId_ = CurrentMotorProtBlockedId;
}

ProtectionCurrentMotor::~ProtectionCurrentMotor()
{

}

bool ProtectionCurrentMotor::checkAlarm()
{
  if (vsd->checkVsdStatus(VSD_STATUS_I_LIMIT_FAST))
    return true;
  return false;
}

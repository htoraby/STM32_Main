#include "protection_current_motor.h"
#include "user_main.h"

ProtectionCurrentMotor::ProtectionCurrentMotor()
{
  idMode_= CCS_PROT_MOTOR_CURRENT_MODE;
  idPrevent_= CCS_PROT_MOTOR_CURRENT_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_CURRENT_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_CURRENT_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_CURRENT_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_CURRENT_BLOCK_FLAG;
  idState_ = CCS_PROT_MOTOR_CURRENT_STATE;
  idTimer_ = CCS_PROT_MOTOR_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_CURRENT_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_CURRENT_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvCurrentMotor;
  lastReasonStop_ = LastReasonStopCurrentMotor;

  protReactEventId_ = CurrentMotorProtReactId;
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

#include "protection_overload_motor.h"

ProtectionOverloadMotor::ProtectionOverloadMotor()
{
  idMode_= CCS_PROT_MOTOR_OVERLOAD_MODE;
  idActivDelay_ = CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_OVERLOAD_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_OVERLOAD_BLOCK_FLAG;
  idState_ = CCS_PROT_MOTOR_OVERLOAD_STATE;
  idTimer_ = CCS_PROT_MOTOR_OVERLOAD_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME;

  protReactEventId_ = OverloadMotorProtReactId;
  apvEventId_ = OverloadMotorApvId;
  apvDisabledEventId_ = OverloadMotorApvDisabledId;
  protBlockedEventId_ = OverloadMotorProtBlockedId;
}

ProtectionOverloadMotor::~ProtectionOverloadMotor()
{

}

bool ProtectionOverloadMotor::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionOverloadMotor::checkPrevent()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionOverloadMotor::calcValue()
{
  float value = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_3);

  value = max(max(value, value2), value3);

  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);
  return (value / (nominal / 100.0));
}

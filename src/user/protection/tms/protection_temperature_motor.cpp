#include "protection_temperature_motor.h"

ProtectionTemperatureMotor::ProtectionTemperatureMotor()
{
  idMode_= CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
  idActivDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE;
  idTimer_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET_COUNT;

  protReactEventId_ = TemperatureMotorProtReactId;
  apvEventId_ = TemperatureMotorApvId;
  apvDisabledEventId_ = TemperatureMotorApvDisabledId;
  protBlockedEventId_ = TemperatureMotorProtBlockedId;
}

ProtectionTemperatureMotor::~ProtectionTemperatureMotor()
{

}

bool ProtectionTemperatureMotor::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionTemperatureMotor::checkPrevent()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionTemperatureMotor::calcValue()
{
  return parameters.getValue(TMS_TEMPERATURE_WINDING);
}

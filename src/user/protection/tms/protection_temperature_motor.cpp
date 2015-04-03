#include "protection_temperature_motor.h"

ProtectionTemperatureMotor::ProtectionTemperatureMotor()
{
  idMode_= CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE;
  idPrevent_= CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT;
  idActivDelay_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_BLOCK_FLAG;
  idState_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE;
  idTimer_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME;
  idRestartCount_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvTemperatureMotor;
  lastReasonStop_ = LastReasonStopTemperatureMotor;

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
  if (restart_)
    return Protection::isHigherLimit(restartSetpoint_);
  else
    return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionTemperatureMotor::calcValue()
{
  return parameters.getValue(TMS_TEMPERATURE_WINDING);
}

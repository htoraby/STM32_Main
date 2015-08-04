#include "protection_imbalance_current_motor.h"

ProtectionImbalanceCurrentMotor::ProtectionImbalanceCurrentMotor()
{
  idMode_= CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
  idPrevent_= CCS_PROT_MOTOR_IMBALANCE_CURRENT_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_DELAY_FLAG;
  idState_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE;
  idTimer_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvImbalanceCurMotor;
  lastReasonStop_ = LastReasonStopImbalanceCurMotor;

  protReactEventId_ = ImbalanceCurMotorProtReactId;
  apvEventId_ = ImbalanceCurMotorApvId;
  apvDisabledEventId_ = ImbalanceCurMotorApvDisabledId;
  protBlockedEventId_ = ImbalanceCurMotorProtBlockedId;
}

ProtectionImbalanceCurrentMotor::~ProtectionImbalanceCurrentMotor()
{

}

bool ProtectionImbalanceCurrentMotor::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionImbalanceCurrentMotor::calcValue()
{
  return parameters.get(CCS_MOTOR_CURRENT_IMBALANCE);
}

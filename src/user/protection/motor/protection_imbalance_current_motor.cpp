#include "protection_imbalance_current_motor.h"

ProtectionImbalanceCurrentMotor::ProtectionImbalanceCurrentMotor()
{
  idMode_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
  idReaction_= CCS_PROT_MOTOR_IMBALANCE_CURRENT_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
  idTimer_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET_COUNT;

  protActivatedEventId_ = ImbalanceCurMotorProtActivId;
  apvEventId_ = ImbalanceCurMotorApvId;
  apvDisabledEventId_ = ImbalanceCurMotorApvDisabledId;
  protBlockedEventId_ = ImbalanceCurMotorProtBlockedId;
}

ProtectionImbalanceCurrentMotor::~ProtectionImbalanceCurrentMotor()
{

}

void ProtectionImbalanceCurrentMotor::init()
{
  Protection::init("ProtImbalanceCurrentMotor");
}

bool ProtectionImbalanceCurrentMotor::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionImbalanceCurrentMotor::checkBlock()
{
  return Protection::isHigherLimit(restartSetpoint_);
}

float ProtectionImbalanceCurrentMotor::calcValue()
{
  return parameters.getValue(CCS_MOTOR_CURRENT_IMBALANCE);
}

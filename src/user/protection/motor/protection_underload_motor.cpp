#include "protection_underload_motor.h"

ProtectionUnderloadMotor::ProtectionUnderloadMotor()
{
  idReaction_= CCS_PROT_MOTOR_UNDERLOAD_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_UNDERLOAD_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_UNDERLOAD_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_UNDERLOAD_MODE;
  idTimer_ = CCS_PROT_MOTOR_UNDERLOAD_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET_COUNT;

  protActivatedEventId_ = UnderloadMotorProtActivId;
  apvEventId_ = UnderloadMotorApvId;
  apvDisabledEventId_ = UnderloadMotorApvDisabledId;
  protBlockedEventId_ = UnderloadMotorProtBlockedId;
}

ProtectionUnderloadMotor::~ProtectionUnderloadMotor()
{

}

bool ProtectionUnderloadMotor::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionUnderloadMotor::checkBlock()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionUnderloadMotor::calcValue()
{
  float value = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_1_NOW);
  float value2 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_2_NOW);
  float value3 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_3_NOW);
  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);

  value = min(min(value, value2), value3);

  float cos = parameters.getValue(CCS_MOTOR_COS_PHI_NOW);
  float nominalCos = parameters.getValue(VSD_MOTOR_COS_PHI);

  return (value * cos) / (nominal * nominalCos) * 100.0;
}

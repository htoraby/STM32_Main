#include "protection_underload_motor.h"

ProtectionUnderloadMotor::ProtectionUnderloadMotor()
{
  idMode_= CCS_PROT_MOTOR_UNDERLOAD_MODE;
  idPrevent_= CCS_PROT_MOTOR_UNDERLOAD_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_UNDERLOAD_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_UNDERLOAD_BLOCK_FLAG;
  idState_ = CCS_PROT_MOTOR_UNDERLOAD_STATE;
  idTimer_ = CCS_PROT_MOTOR_UNDERLOAD_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvUnderloadMotor;

  protReactEventId_ = UnderloadMotorProtReactId;
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

float ProtectionUnderloadMotor::calcValue()
{
  float value = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_3);
  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);

  value = min(min(value, value2), value3);

  float cos = parameters.getValue(CCS_MOTOR_COS_PHI_NOW);
  float nominalCos = parameters.getValue(VSD_MOTOR_COS_PHI);

  return (value * cos) / (nominal * nominalCos) * 100.0;
}

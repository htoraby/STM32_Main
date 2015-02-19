#include "protection_overload_motor.h"

ProtectionOverloadMotor::ProtectionOverloadMotor()
{
  idMode_ = CCS_PROT_MOTOR_OVERLOAD_MODE;
  idReaction_= CCS_PROT_MOTOR_OVERLOAD_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_OVERLOAD_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_OVERLOAD_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_OVERLOAD_MODE;
  idTimer_ = CCS_PROT_MOTOR_OVERLOAD_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET_COUNT;

  protActivatedEventId_ = OverloadMotorProtActivId;
  apvEventId_ = OverloadMotorApvId;
  apvDisabledEventId_ = OverloadMotorApvDisabledId;
  protBlockedEventId_ = OverloadMotorProtBlockedId;
}

ProtectionOverloadMotor::~ProtectionOverloadMotor()
{

}

void ProtectionOverloadMotor::init()
{
  Protection::init("ProtOverloadMotor");
}

bool ProtectionOverloadMotor::checkAlarm()
{
  calcValue();
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionOverloadMotor::checkBlock()
{
  calcValue();
  return Protection::isHigherLimit(restartSetpoint_);
}

void ProtectionOverloadMotor::calcValue()
{
  float value = 0;
  for (int i = 0; i < 3; ++i) {
    float value2 = parameters.getValue(CCS_MOTOR_CURRENT_PHASE_1_NOW+i);
    if (value2 > value)
      value = value2;
  }
  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);
  valueParameter_ =  value / (nominal / 100.0);
}

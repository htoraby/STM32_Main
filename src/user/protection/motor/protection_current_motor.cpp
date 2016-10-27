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
  idDelayFlag_ = CCS_PROT_MOTOR_CURRENT_DELAY_FLAG;
  idState_ = CCS_PROT_MOTOR_CURRENT_STATE;
  idTimer_ = CCS_PROT_MOTOR_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_CURRENT_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_CURRENT_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvCurrentMotor;

  protReactEventId_ = CurrentMotorProtReactId;
  apvEventId_ = CurrentMotorApvId;
  apvDisabledEventId_ = CurrentMotorApvDisabledId;
  protBlockedEventId_ = CurrentMotorProtBlockedId;
}

ProtectionCurrentMotor::~ProtectionCurrentMotor()
{

}

void ProtectionCurrentMotor::getOtherSetpointProt()
{
  activDelay_ = 0.0;
  tripDelay_  = 0.0;
}

bool ProtectionCurrentMotor::checkAlarm()
{ 
  float alarm = vsd->checkAlarmVsdCurrentMotor();
  if (alarm) {
    lastReasonStop_ = (LastReasonStop)(int)alarm;
    return true;
  }
  else if (Protection::isHigherLimit(tripSetpoint_)) {
    lastReasonStop_ = LastReasonStopCurrentMotor;
    return true;
  }
  else {
    return false;
  }
}

float ProtectionCurrentMotor::calcValue()
{
  float value = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);

  value = max(max(value, value2), value3);

  float nominal = parameters.get(VSD_MOTOR_CURRENT);
  return (value / (nominal / 100.0));
}

bool ProtectionCurrentMotor::isProtect()
{
  return true;
}

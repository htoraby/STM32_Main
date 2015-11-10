#include "protection_analog_input_3.h"

ProtectionAnalogInput3::ProtectionAnalogInput3()
{
  idMode_= CCS_PROT_AI_3_MODE;
  idPrevent_= CCS_PROT_AI_3_PREVENT;
  idActivDelay_ = CCS_PROT_AI_3_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_AI_3_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_AI_3_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_AI_3_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_AI_3_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_AI_3_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_AI_3_RESTART_SETPOINT;
  idParam_ = CCS_PROT_AI_3_PARAMETER;
  idRestartFlag_ = CCS_PROT_AI_3_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_AI_3_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_AI_3_DELAY_FLAG;
  idState_ = CCS_PROT_AI_3_STATE;
  idTimer_ = CCS_PROT_AI_3_TIME;
  idRestartCount_ = CCS_PROT_AI_3_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_AI_3_RESTART_FIRST_TIME;

  protReactEventId_ = AnalogInput3ProtReactId;
  apvEventId_ = AnalogInput3ApvId;
  apvDisabledEventId_ = AnalogInput3ApvDisabledId;
  protBlockedEventId_ = AnalogInput3ProtBlockedId;
}

ProtectionAnalogInput3::~ProtectionAnalogInput3()
{

}

bool ProtectionAnalogInput3::checkAlarm()
{
  float min = tripSetpoint_;
  float max = restartSetpoint_;
  nominal_ = parameters.get(idParam_);
  valueParameter_ = parameters.get(CCS_AI_3_VALUE_CALC);

  if (valueParameter_ > nominal_ * max / 100) {
    lastReasonRun_ = LastReasonRunApvMaxAnalog3;
    lastReasonStop_ = LastReasonStopMaxAnalog3;
    parameters.set(CCS_PROT_AI_3_ALARM_TYPE, 1);
    return true;
  } else if (valueParameter_ < nominal_ * min / 100) {
    lastReasonRun_ = LastReasonRunApvMinAnalog3;
    lastReasonStop_ = LastReasonStopMinAnalog3;
    parameters.set(CCS_PROT_AI_3_ALARM_TYPE, 0);
    return true;
  } else {
    return false;
  }
}

bool ProtectionAnalogInput3::checkPrevent()
{
  return alarm_;
}

void ProtectionAnalogInput3::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, nominal_, valueParameter_);
}

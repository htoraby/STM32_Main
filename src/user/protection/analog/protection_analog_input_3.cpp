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
  float max = parameters.get(CCS_PROT_AI_3_PARAMETER);
  valueParameter_ = parameters.get(CCS_AI_3_VALUE_CALC);

  if (valueParameter_ > max) {
    lastReasonRun_ = LastReasonRunApvMaxAnalog3;
    lastReasonStop_ = LastReasonStopMaxAnalog3;
    protReactEventId_ = AnalogInput3MaxProtReactId;
    parameters.set(CCS_PROT_AI_3_ALARM_TYPE, 1);
    return true;
  } else if (valueParameter_ < min) {
    lastReasonRun_ = LastReasonRunApvMinAnalog3;
    lastReasonStop_ = LastReasonStopMinAnalog3;
    protReactEventId_ = AnalogInput3MinProtReactId;
    parameters.set(CCS_PROT_AI_3_ALARM_TYPE, 0);
    return true;
  } else {
    return false;
  }
}

bool ProtectionAnalogInput3::checkPrevent()
{
  if (restart_) {
    float minRestart = restartSetpoint_;
    float maxRestart = parameters.get(CCS_PROT_AI_3_RESTART_SETPOINT_MAX);
    if ((valueParameter_ > maxRestart) || (valueParameter_ < minRestart)) {
      return true;
    } else {
      return false;
    }
  }
  else {
    return alarm_;
  }
}

void ProtectionAnalogInput3::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, 0, valueParameter_);
}

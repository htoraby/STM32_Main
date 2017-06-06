#include "protection_analog_input_4.h"

ProtectionAnalogInput4::ProtectionAnalogInput4()
{
  idMode_= CCS_PROT_AI_4_MODE;
  idPrevent_= CCS_PROT_AI_4_PREVENT;
  idActivDelay_ = CCS_PROT_AI_4_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_AI_4_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_AI_4_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_AI_4_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_AI_4_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_AI_4_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_AI_4_RESTART_SETPOINT;
  idParam_ = CCS_PROT_AI_4_PARAMETER;
  idRestartFlag_ = CCS_PROT_AI_4_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_AI_4_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_AI_4_DELAY_FLAG;
  idState_ = CCS_PROT_AI_4_STATE;
  idTimer_ = CCS_PROT_AI_4_TIME;
  idRestartCount_ = CCS_PROT_AI_4_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_AI_4_RESTART_FIRST_TIME;

  apvEventId_ = AnalogInput4ApvId;
  apvDisabledEventId_ = AnalogInput4ApvDisabledId;
  protBlockedEventId_ = AnalogInput4ProtBlockedId;
}

ProtectionAnalogInput4::~ProtectionAnalogInput4()
{

}

bool ProtectionAnalogInput4::checkAlarm()
{
  float min = tripSetpoint_;
  float max = parameters.get(CCS_PROT_AI_4_PARAMETER);
  valueParameter_ = parameters.get(CCS_AI_4_VALUE_CALC);

  if (valueParameter_ > max) {
    lastReasonRun_ = LastReasonRunApvMaxAnalog4;
    lastReasonStop_ = LastReasonStopMaxAnalog4;
    protReactEventId_ = AnalogInput4MaxProtReactId;
    parameters.set(CCS_PROT_AI_4_ALARM_TYPE, 1);
    return true;
  } else if (valueParameter_ < min) {
    lastReasonRun_ = LastReasonRunApvMinAnalog4;
    lastReasonStop_ = LastReasonStopMinAnalog4;
    protReactEventId_ = AnalogInput4MinProtReactId;
    parameters.set(CCS_PROT_AI_4_ALARM_TYPE, 0);
    return true;
  } else {
    return false;
  }
}

bool ProtectionAnalogInput4::checkPrevent()
{
  if (restart_) {
    float minRestart = restartSetpoint_;
    float maxRestart = parameters.get(CCS_PROT_AI_4_RESTART_SETPOINT_MAX);
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

void ProtectionAnalogInput4::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, 0, valueParameter_);
}

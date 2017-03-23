#include "protection_analog_input_1.h"

ProtectionAnalogInput1::ProtectionAnalogInput1()
{
  idMode_= CCS_PROT_AI_1_MODE;
  idPrevent_= CCS_PROT_AI_1_PREVENT;
  idActivDelay_ = CCS_PROT_AI_1_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_AI_1_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_AI_1_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_AI_1_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_AI_1_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_AI_1_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_AI_1_RESTART_SETPOINT;
  idParam_ = CCS_PROT_AI_1_PARAMETER;
  idRestartFlag_ = CCS_PROT_AI_1_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_AI_1_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_AI_1_DELAY_FLAG;
  idState_ = CCS_PROT_AI_1_STATE;
  idTimer_ = CCS_PROT_AI_1_TIME;
  idRestartCount_ = CCS_PROT_AI_1_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_AI_1_RESTART_FIRST_TIME;

  apvEventId_ = AnalogInput1ApvId;
  apvDisabledEventId_ = AnalogInput1ApvDisabledId;
  protBlockedEventId_ = AnalogInput1ProtBlockedId;
}

ProtectionAnalogInput1::~ProtectionAnalogInput1()
{

}

bool ProtectionAnalogInput1::checkAlarm()
{
  float min = tripSetpoint_;
  float max = parameters.get(CCS_PROT_AI_1_PARAMETER);
  valueParameter_ = parameters.get(CCS_AI_1_VALUE_CALC);

  if (valueParameter_ > max) {
    lastReasonRun_ = LastReasonRunApvMaxAnalog1;
    lastReasonStop_ = LastReasonStopMaxAnalog1;
    protReactEventId_ = AnalogInput1MaxProtReactId;
    parameters.set(CCS_PROT_AI_1_ALARM_TYPE, 1);
    return true;
  } else if (valueParameter_ < min) {
    lastReasonRun_ = LastReasonRunApvMinAnalog1;
    lastReasonStop_ = LastReasonStopMinAnalog1;
    protReactEventId_ = AnalogInput1MinProtReactId;
    parameters.set(CCS_PROT_AI_1_ALARM_TYPE, 0);
    return true;
  } else {
    return false;
  }
}

bool ProtectionAnalogInput1::checkPrevent()
{
  if (restart_) {
    float minRestart = restartSetpoint_;
    float maxRestart = parameters.get(CCS_PROT_AI_1_RESTART_SETPOINT_MAX);
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

void ProtectionAnalogInput1::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, 0, valueParameter_);
}

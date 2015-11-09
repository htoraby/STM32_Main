#include "protection_analog_input_2.h"

ProtectionAnalogInput2::ProtectionAnalogInput2()
{
  idMode_= CCS_PROT_AI_2_MODE;
  idPrevent_= CCS_PROT_AI_2_PREVENT;
  idActivDelay_ = CCS_PROT_AI_2_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_AI_2_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_AI_2_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_AI_2_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_AI_2_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_AI_2_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_AI_2_RESTART_SETPOINT;
  idParam_ = CCS_PROT_AI_2_PARAMETER;
  idRestartFlag_ = CCS_PROT_AI_2_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_AI_2_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_AI_2_DELAY_FLAG;
  idState_ = CCS_PROT_AI_2_STATE;
  idTimer_ = CCS_PROT_AI_2_TIME;
  idRestartCount_ = CCS_PROT_AI_2_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_AI_2_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvMinAnalog2;
  lastReasonStop_ = LastReasonStopMinAnalog2;

  protReactEventId_ = AnalogInput2ProtReactId;
  apvEventId_ = AnalogInput2ApvId;
  apvDisabledEventId_ = AnalogInput2ApvDisabledId;
  protBlockedEventId_ = AnalogInput2ProtBlockedId;
}

ProtectionAnalogInput2::~ProtectionAnalogInput2()
{

}

bool ProtectionAnalogInput2::checkAlarm()
{
  float min = tripSetpoint_;
  float max = restartSetpoint_;
  nominal_ = parameters.get(idParam_);
  valueParameter_ = parameters.get(CCS_AI_2_VALUE_CALC);

  if (valueParameter_ > nominal_ * max / 100) {
    lastReasonRun_ = LastReasonRunApvMaxAnalog2;
    lastReasonStop_ = LastReasonStopMaxAnalog2;
    return true;
  } else if (valueParameter_ < nominal_ * min / 100) {
    return true;
  } else {
    return false;
  }
}

bool ProtectionAnalogInput2::checkPrevent()
{
  return alarm_;
}

void ProtectionAnalogInput2::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, nominal_, valueParameter_);
}

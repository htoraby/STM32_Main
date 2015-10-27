#include "protection_digital_input_3.h"

ProtectionDigitalInput3::ProtectionDigitalInput3()
{
  idMode_= CCS_PROT_DI_3_MODE;
  idPrevent_= CCS_PROT_DI_3_PREVENT;
  idActivDelay_ = CCS_PROT_DI_3_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DI_3_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DI_3_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DI_3_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DI_3_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DI_3_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DI_3_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DI_3_PARAMETER;
  idRestartFlag_ = CCS_PROT_DI_3_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DI_3_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DI_3_DELAY_FLAG;
  idState_ = CCS_PROT_DI_3_STATE;
  idTimer_ = CCS_PROT_DI_3_TIME;
  idRestartCount_ = CCS_PROT_DI_3_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DI_3_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvDigital3;
  lastReasonStop_ = LastReasonStopDigital3;

  protReactEventId_ = DigitalInput3ProtReactId;
  apvEventId_ = DigitalInput3ApvId;
  apvDisabledEventId_ = DigitalInput3ApvDisabledId;
  protBlockedEventId_ = DigitalInput3ProtBlockedId;
}

ProtectionDigitalInput3::~ProtectionDigitalInput3()
{

}

bool ProtectionDigitalInput3::isProtect()
{
  if (parameters.get(CCS_DI_3_ACTION) == DI_ACTION_PROTECTION)
    return true;
  return false;
}

bool ProtectionDigitalInput3::checkAlarm()
{
    valueParameter_ = parameters.get(CCS_DI_3_VALUE);
    if (valueParameter_ == parameters.get(CCS_DI_3_TYPE))
      return true;
    return false;
}

bool ProtectionDigitalInput3::checkPrevent()
{
  return alarm_;
}

void ProtectionDigitalInput3::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, !valueParameter_, valueParameter_);
}

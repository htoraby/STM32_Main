#include "protection_digital_input_4.h"

ProtectionDigitalInput4::ProtectionDigitalInput4()
{
  idMode_= CCS_PROT_DI_4_MODE;
  idPrevent_= CCS_PROT_DI_4_PREVENT;
  idActivDelay_ = CCS_PROT_DI_4_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DI_4_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DI_4_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DI_4_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DI_4_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DI_4_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DI_4_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DI_4_PARAMETER;
  idRestartFlag_ = CCS_PROT_DI_4_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DI_4_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DI_4_DELAY_FLAG;
  idState_ = CCS_PROT_DI_4_STATE;
  idTimer_ = CCS_PROT_DI_4_TIME;
  idRestartCount_ = CCS_PROT_DI_4_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DI_4_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvDigital4;
  lastReasonStop_ = LastReasonStopProtDigital4;

  protReactEventId_ = DigitalInput4ProtReactId;
  apvEventId_ = DigitalInput4ApvId;
  apvDisabledEventId_ = DigitalInput4ApvDisabledId;
  protBlockedEventId_ = DigitalInput4ProtBlockedId;
}

ProtectionDigitalInput4::~ProtectionDigitalInput4()
{

}

bool ProtectionDigitalInput4::isProtect()
{
  if (parameters.get(CCS_DI_4_ACTION) == DI_ACTION_PROTECTION)
    return true;
  return false;
}

bool ProtectionDigitalInput4::checkAlarm()
{
    valueParameter_ = parameters.get(CCS_DI_4_VALUE);
    if (valueParameter_ == parameters.get(CCS_DI_4_TYPE))
      return true;
    return false;
}

bool ProtectionDigitalInput4::checkPrevent()
{
  return alarm_;
}

void ProtectionDigitalInput4::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, !valueParameter_, valueParameter_);
}

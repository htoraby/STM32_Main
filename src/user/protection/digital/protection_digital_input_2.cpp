#include "protection_digital_input_2.h"

ProtectionDigitalInput2::ProtectionDigitalInput2()
{
  idMode_= CCS_PROT_DI_2_MODE;
  idPrevent_= CCS_PROT_DI_2_PREVENT;
  idActivDelay_ = CCS_PROT_DI_2_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DI_2_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DI_2_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DI_2_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DI_2_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DI_2_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DI_2_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DI_2_PARAMETER;
  idRestartFlag_ = CCS_PROT_DI_2_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DI_2_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DI_2_DELAY_FLAG;
  idState_ = CCS_PROT_DI_2_STATE;
  idTimer_ = CCS_PROT_DI_2_TIME;
  idRestartCount_ = CCS_PROT_DI_2_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DI_2_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvDigital2;
  lastReasonStop_ = LastReasonStopDigital2;

  protReactEventId_ = DigitalInput2ProtReactId;
  apvEventId_ = DigitalInput2ApvId;
  apvDisabledEventId_ = DigitalInput2ApvDisabledId;
  protBlockedEventId_ = DigitalInput2ProtBlockedId;
}

ProtectionDigitalInput2::~ProtectionDigitalInput2()
{

}

bool ProtectionDigitalInput2::isProtect()
{
  if (parameters.get(CCS_DI_2_ACTION) == DI_ACTION_PROTECTION)
    return true;
  return false;
}

bool ProtectionDigitalInput2::checkAlarm()
{
    valueParameter_ = parameters.get(CCS_DI_2_VALUE);
    if (valueParameter_ == parameters.get(CCS_DI_2_TYPE))
      return true;
    return false;
}

bool ProtectionDigitalInput2::checkPrevent()
{
  return alarm_;
}

void ProtectionDigitalInput2::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, !valueParameter_, valueParameter_);
}

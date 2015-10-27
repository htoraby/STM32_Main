#include "protection_digital_input_1.h"

ProtectionDigitalInput1::ProtectionDigitalInput1()
{
  idMode_= CCS_PROT_DI_1_MODE;
  idPrevent_= CCS_PROT_DI_1_PREVENT;
  idActivDelay_ = CCS_PROT_DI_1_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DI_1_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DI_1_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DI_1_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DI_1_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DI_1_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DI_1_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DI_1_PARAMETER;
  idRestartFlag_ = CCS_PROT_DI_1_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DI_1_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DI_1_DELAY_FLAG;
  idState_ = CCS_PROT_DI_1_STATE;
  idTimer_ = CCS_PROT_DI_1_TIME;
  idRestartCount_ = CCS_PROT_DI_1_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DI_1_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvDigital1;
  lastReasonStop_ = LastReasonStopDigital1;

  protReactEventId_ = DigitalInput1ProtReactId;
  apvEventId_ = DigitalInput1ApvId;
  apvDisabledEventId_ = DigitalInput1ApvDisabledId;
  protBlockedEventId_ = DigitalInput1ProtBlockedId;
}

ProtectionDigitalInput1::~ProtectionDigitalInput1()
{

}

bool ProtectionDigitalInput1::isProtect()
{
  if (parameters.get(CCS_DI_1_ACTION) == DI_ACTION_PROTECTION)
    return true;
  return false;
}

bool ProtectionDigitalInput1::checkAlarm()
{
    valueParameter_ = parameters.get(CCS_DI_1_VALUE);
    if (valueParameter_ == parameters.get(CCS_DI_1_TYPE))
      return true;
    return false;
}

bool ProtectionDigitalInput1::checkPrevent()
{
  return alarm_;
}

void ProtectionDigitalInput1::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, !valueParameter_, valueParameter_);
}

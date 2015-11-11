#include "protection_hardware_vsd.h"

ProtectionHardwareVsd::ProtectionHardwareVsd()
{
  idMode_= CCS_PROT_OTHER_VSD_MODE;
  idPrevent_= CCS_PROT_OTHER_VSD_PREVENT;
  idActivDelay_ = CCS_PROT_OTHER_VSD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_VSD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_VSD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_VSD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_VSD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_VSD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_VSD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_VSD_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_VSD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_VSD_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_OTHER_VSD_DELAY_FLAG;
  idState_ = CCS_PROT_OTHER_VSD_STATE;
  idTimer_ = CCS_PROT_OTHER_VSD_TIME;
  idRestartCount_ = CCS_PROT_OTHER_VSD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_OTHER_VSD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvHardwareVsd;

  protReactEventId_ = HardwareVsdProtReactId;
  apvEventId_ = HardwareVsdApvId;
  apvDisabledEventId_ = HardwareVsdApvDisabledId;
  protBlockedEventId_ = HardwareVsdProtBlockedId;
}

ProtectionHardwareVsd::~ProtectionHardwareVsd()
{

}

bool ProtectionHardwareVsd::checkAlarm()
{
  float alarm = vsd->checkAlarmVsd();
  lastReasonStop_ = (LastReasonStop)alarm;
  protReactEventId_ = (EventId)alarm;
  parameters.set(CCS_VSD_ALARM_CODE, alarm);
  if (alarm != VSD_ALARM_NONE) {
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, alarm);
    return true;
  }
  return false;
}

bool ProtectionHardwareVsd::checkPrevent()
{
  return alarm_;
}

void ProtectionHardwareVsd::addEventReactionProt()
{
  logEvent.add(ProtectVsdCode, AutoType, protReactEventId_);
}

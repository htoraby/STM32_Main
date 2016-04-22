#include "protection_overnumber_of_start.h"

ProtectionOvernumberOfStart::ProtectionOvernumberOfStart()
{
  idMode_= CCS_PROT_OTHER_LIMIT_RESTART_MODE;
  idPrevent_= CCS_PROT_OTHER_LIMIT_RESTART_PREVENT;
  idActivDelay_ = CCS_PROT_OTHER_LIMIT_RESTART_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_LIMIT_RESTART_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_LIMIT_RESTART_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_LIMIT_RESTART_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_LIMIT_RESTART_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_OTHER_LIMIT_RESTART_DELAY_FLAG;
  idState_ = CCS_PROT_OTHER_LIMIT_RESTART_STATE;
  idTimer_ = CCS_PROT_OTHER_LIMIT_RESTART_TIME;
  idRestartCount_ = CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT;

  lastReasonStop_ = LastReasonStopOvernumberOfStart;

  protReactEventId_ = OvernumberOfStartProtReactId;
  protBlockedEventId_ = OvernumberOfStartProtBlockedId;
}

ProtectionOvernumberOfStart::~ProtectionOvernumberOfStart()
{

}

void ProtectionOvernumberOfStart::getOtherSetpointProt()
{

}

void ProtectionOvernumberOfStart::setOtherParamProt()
{
  parameters.set(CCS_RESTART_COUNT_FIRST_TIME, ksu.getRestartTime());
}

bool ProtectionOvernumberOfStart::checkAlarm()
{
  if (ksu.getSecFromCurTime(ksu.getRestartTime()) < parameters.get(idParam_)) {
    if (ksu.getRestartCount() >= tripSetpoint_)
      return true;
  } else {
    ksu.decRestartCount();
  }
  return false;
}

void ProtectionOvernumberOfStart::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_);
}

void ProtectionOvernumberOfStart::processingStateRun()
{
  if (isModeBlock()) {
    if (alarm_&& !restart_ && ksu.isRestart()) {
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Защиты: Срабатывание --> блок (idMode = %d, alarm = %d, tripDelay = %d, isModeBlock = %d)",
                   idMode_, alarm_, tripDelay_, isModeBlock());
#endif
      addEventReactionProt();
      logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
      ksu.setBlock();
      ksu.stop(lastReasonStop_);
      block_ = true;
      setStateStop();
    }
  } else {
    setStateStop();
  }
  restart_ = ksu.isRestart();
}

void ProtectionOvernumberOfStart::proccessingStateStop()
{
  if (isModeBlock()) {
    if (ksu.isWorkMotor()) {
      state_ = StateRun;
    }
  }
}

void ProtectionOvernumberOfStart::automatProtection()
{
  switch (state_) {
  case StateRunning:
    state_ = StateRun;
  case StateRun:
    processingStateRun();
    break;
  case StateStop:
    proccessingStateStop();
    break;
  default:
    break;
  }
}

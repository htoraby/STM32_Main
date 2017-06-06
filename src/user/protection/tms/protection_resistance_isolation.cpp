#include "protection_resistance_isolation.h"
#include "protection_main.h"

ProtectionResistanceIsolation::ProtectionResistanceIsolation()
{
  idMode_= CCS_PROT_DHS_RESISTANCE_MODE;
  idPrevent_= CCS_PROT_DHS_RESISTANCE_PREVENT;
  idActivDelay_ = CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_RESISTANCE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_RESISTANCE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_RESISTANCE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_RESISTANCE_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_RESISTANCE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DHS_RESISTANCE_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DHS_RESISTANCE_DELAY_FLAG;
  idState_ = CCS_PROT_DHS_RESISTANCE_STATE;
  idTimer_ = CCS_PROT_DHS_RESISTANCE_TIME;
  idRestartCount_ = CCS_PROT_DHS_RESISTANCE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvResistIsolation;
  lastReasonStop_ = LastReasonStopResistIsolation;

  protReactEventId_ = ResistIsolationProtReactId;
  apvEventId_ = ResistIsolationApvId;
  apvDisabledEventId_ = ResistIsolationApvDisabledId;
  protBlockedEventId_ = ResistIsolationProtBlockedId;
}

ProtectionResistanceIsolation::~ProtectionResistanceIsolation()
{

}

bool ProtectionResistanceIsolation::checkAlarm()
{
  if (ksu.getValue(idParam_)) {
    return (protOverloadMotor.externalCheckAlarm() && Protection::isLowerLimit(tripSetpoint_));
  }
  else {
    return Protection::isLowerLimit(tripSetpoint_);
  }
}

bool ProtectionResistanceIsolation::checkPrevent()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionResistanceIsolation::calcValue()
{
  return parameters.get(CCS_RESISTANCE_ISOLATION);
}

bool ProtectionResistanceIsolation::isProtect()
{
  if (parameters.get(CCS_DHS_TYPE) == TYPE_DHS_NONE) {
    if (parameters.isValidity(CCS_RESISTANCE_ISOLATION)) {
      return true;
    }
  }
  else {
    if (tms->isConnect())
      if (parameters.isValidity(CCS_RESISTANCE_ISOLATION))
        return true;
  }
  return false;
}

#include "protection_resistance_isolation.h"

ProtectionResistanceIsolation::ProtectionResistanceIsolation()
{
  idMode_= CCS_PROT_DHS_RESISTANCE_MODE;
  idActivDelay_ = CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_RESISTANCE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_RESISTANCE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_RESISTANCE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_RESISTANCE_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_RESISTANCE_RESTART_FLAG;
  idState_ = CCS_PROT_DHS_RESISTANCE_STATE;
  idTimer_ = CCS_PROT_DHS_RESISTANCE_TIME;
  idRestartCount_ = CCS_PROT_DHS_RESISTANCE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_RESISTANCE_RESTART_RESET_COUNT;

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
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionResistanceIsolation::checkPrevent()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionResistanceIsolation::calcValue()
{
  return parameters.getValue(CCS_RESISTANCE_ISOLATION);
}

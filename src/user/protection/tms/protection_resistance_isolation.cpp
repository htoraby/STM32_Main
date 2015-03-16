#include "protection_resistance_isolation.h"

ProtectionResistanceIsolation::ProtectionResistanceIsolation()
{
  idMode_ = CCS_PROT_DHS_RESISTANCE_MODE;
  idReaction_= CCS_PROT_DHS_RESISTANCE_REACTION;
  idActivDelay_ = CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_RESISTANCE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_RESISTANCE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_DHS_RESISTANCE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_RESISTANCE_PARAMETER;
  idParam2_ = CCS_PROT_DHS_RESISTANCE_PARAMETER_2;
  idState_ = CCS_PROT_DHS_RESISTANCE_MODE;
  idTimer_ = CCS_PROT_DHS_RESISTANCE_TIME;
  idRestartCount_ = CCS_PROT_DHS_RESISTANCE_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_DHS_RESISTANCE_RESTART_RESET_COUNT;

  protActivatedEventId_ = ResistIsolationProtActivId;
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

bool ProtectionResistanceIsolation::checkBlock()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionResistanceIsolation::calcValue()
{
  return parameters.getValue(CCS_RESISTANCE_ISOLATION);
}

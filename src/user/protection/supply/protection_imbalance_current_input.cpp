#include "protection_imbalance_current_input.h"

ProtectionImbalanceCurrentInput::ProtectionImbalanceCurrentInput()
{
  idMode_= CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE;
  idPrevent_= CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PREVENT;
  idActivDelay_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_SETPOINT;
  if (1)  // По ТТ СНГ
    idRestartSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_SETPOINT;
  else
    idRestartSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PARAMETER;
  idRestartFlag_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_DELAY_FLAG;
  idState_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_STATE;
  idTimer_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvImbalanceCurIn;
  lastReasonStop_ = LastReasonStopImbalanceCurIn;

  protReactEventId_ = ImbalanceCurInProtReactId;
  apvEventId_ = ImbalanceCurInApvId;
  apvDisabledEventId_ = ImbalanceCurInApvDisabledId;
  protBlockedEventId_ = ImbalanceCurInProtBlockedId;
}

ProtectionImbalanceCurrentInput::~ProtectionImbalanceCurrentInput()
{

}

void ProtectionImbalanceCurrentInput::getOtherSetpointProt()
{
  timerDifStartFlag_ = ksu.getValue(idParam_);
}

bool ProtectionImbalanceCurrentInput::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionImbalanceCurrentInput::checkPrevent()
{
  if (restart_)
    return Protection::isHigherLimit(restartSetpoint_);
  else
    return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionImbalanceCurrentInput::calcValue()
{
  float imbalance = 0;
  if (parameters.isValidity(CCS_CURRENT_IMBALANCE_IN))
    imbalance = parameters.get(CCS_CURRENT_IMBALANCE_IN);
  return imbalance;
}

bool ProtectionImbalanceCurrentInput::isProtect()
{
#if USE_DEBUG
  return true;
#endif

  if (parameters.get(CCS_EM_TYPE) == EM_TYPE_NONE) {
    if (parameters.isValidity(CCS_CURRENT_PHASE_1) ||
        parameters.isValidity(CCS_CURRENT_PHASE_2) ||
        parameters.isValidity(CCS_CURRENT_PHASE_3)) {
      return true;
    }
  }
  else {
    if (em->isConnect()) {
      if (parameters.isValidity(CCS_CURRENT_PHASE_1) ||
          parameters.isValidity(CCS_CURRENT_PHASE_2) ||
          parameters.isValidity(CCS_CURRENT_PHASE_3)) {
        return true;
      }
    }
  }
  return false;
}

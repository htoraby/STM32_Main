#include "protection_imbalance_voltage_input.h"

ProtectionImbalanceVoltageInput::ProtectionImbalanceVoltageInput()
{
  idMode_= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE;
  idPrevent_= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT;
  idActivDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT;
  if (1)  // По ТТ СНГ
    idRestartSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT;
  else
    idRestartSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_DELAY_FLAG;
  idState_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvImbalanceVoltIn;
  lastReasonStop_ = LastReasonStopImbalanceVoltIn;

  protReactEventId_ = ImbalanceVoltInProtReactId;
  apvEventId_ = ImbalanceVoltInApvId;
  apvDisabledEventId_ = ImbalanceVoltInApvDisabledId;
  protBlockedEventId_ = ImbalanceVoltInProtBlockedId;
}

ProtectionImbalanceVoltageInput::~ProtectionImbalanceVoltageInput()
{

}

void ProtectionImbalanceVoltageInput::getOtherSetpointProt()
{
  timerDifStartFlag_ = ksu.getValue(idParam_);
}

bool ProtectionImbalanceVoltageInput::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionImbalanceVoltageInput::checkPrevent()
{
  if (restart_)
    return Protection::isHigherLimit(restartSetpoint_);
  else
    return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionImbalanceVoltageInput::calcValue()
{
  float imbalance = 0;
  if (parameters.isValidity(CCS_VOLTAGE_IMBALANCE_IN))
    imbalance = parameters.get(CCS_VOLTAGE_IMBALANCE_IN);
  return imbalance;
}

bool ProtectionImbalanceVoltageInput::isProtect()
{
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_1) ||
      parameters.isValidity(CCS_VOLTAGE_PHASE_2) ||
      parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
    return true;
  }
  return false;
}

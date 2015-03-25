#include "protection_imbalance_voltage_input.h"

ProtectionImbalanceVoltageInput::ProtectionImbalanceVoltageInput()
{
  idMode_= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE;
  idActivDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG;
  idState_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET_COUNT;

  protReactEventId_ = ImbalanceVoltInProtReactId;
  apvEventId_ = ImbalanceVoltInApvId;
  apvDisabledEventId_ = ImbalanceVoltInApvDisabledId;
  protBlockedEventId_ = ImbalanceVoltInProtBlockedId;
}

ProtectionImbalanceVoltageInput::~ProtectionImbalanceVoltageInput()
{

}

bool ProtectionImbalanceVoltageInput::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionImbalanceVoltageInput::checkPrevent()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionImbalanceVoltageInput::calcValue()
{
  return parameters.getValue(CCS_VOLTAGE_IMBALANCE_IN);
}

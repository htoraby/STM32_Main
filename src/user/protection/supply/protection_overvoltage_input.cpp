#include "protection_overvoltage_input.h"

ProtectionOverVoltageInput::ProtectionOverVoltageInput()
{
  idMode_= CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
  idActivDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER;
  idParam2_ = CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER_2;
  idState_ = CCS_PROT_SUPPLY_OVERVOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT;

  protReactEventId_ = OverVoltInProtReactId;
  apvEventId_ = OverVoltInApvId;
  apvDisabledEventId_ = OverVoltInApvDisabledId;
  protBlockedEventId_ = OverVoltInProtBlockedId;
}

ProtectionOverVoltageInput::~ProtectionOverVoltageInput()
{

}

bool ProtectionOverVoltageInput::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionOverVoltageInput::checkPrevent()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionOverVoltageInput::calcValue()
{
  float value = parameters.getValue(EM_VOLTAGE_PHASE_1);
  float value2 = parameters.getValue(EM_VOLTAGE_PHASE_2);
  float value3 = parameters.getValue(EM_VOLTAGE_PHASE_3);

  value = max(max(value, value2), value3);

  float nominal = parameters.getValue(CCS_TRANS_NOMINAL_VOLTAGE);
  return (value / (nominal / 100.0));
}

#include "protection_overvoltage_input.h"

ProtectionOverVoltageInput::ProtectionOverVoltageInput()
{
  idMode_= CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
  idPrevent_= CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT;
  idActivDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT;
  if (1)  // По ТТ СНГ
    idRestartSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT;
  else
    idRestartSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_SUPPLY_OVERVOLTAGE_BLOCK_FLAG;
  idState_ = CCS_PROT_SUPPLY_OVERVOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvOverVoltIn;

  protReactEventId_ = OverVoltInProtReactId;
  apvDisabledEventId_ = OverVoltInApvDisabledId;
  protBlockedEventId_ = OverVoltInProtBlockedId;
}

ProtectionOverVoltageInput::~ProtectionOverVoltageInput()
{

}

void ProtectionOverVoltageInput::getOtherSetpointProt()
{
  timerDifStartFlag_ = ksu.getValue(idParam_);
}

bool ProtectionOverVoltageInput::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionOverVoltageInput::checkPrevent()
{
  if (restart_)
    return Protection::isHigherLimit(restartSetpoint_);
  else
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

#include "protection_overvoltage_input.h"

ProtectionOverVoltageInput::ProtectionOverVoltageInput()
{
  idMode_ = CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
  idReaction_= CCS_PROT_SUPPLY_OVERVOLTAGE_REACTION;
  idActivDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER;
  idParam2_ = CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER_2;
  idState_ = CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
  idTimer_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT;

  protActivatedEventId_ = OverVoltInProtActivId;
  apvEventId_ = OverVoltInApvId;
  apvDisabledEventId_ = OverVoltInApvDisabledId;
  protBlockedEventId_ = OverVoltInProtBlockedId;
}

ProtectionOverVoltageInput::~ProtectionOverVoltageInput()
{

}

void ProtectionOverVoltageInput::init()
{
  Protection::init("ProtOverVoltInput");
}

bool ProtectionOverVoltageInput::checkAlarm()
{
  calcValue();
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionOverVoltageInput::checkBlock()
{
  calcValue();
  return Protection::isHigherLimit(restartSetpoint_);
}

void ProtectionOverVoltageInput::calcValue()
{
  float value = 0;
  for (int i = 0; i < 3; ++i) {
    float value2 = parameters.getValue(EM_VOLTAGE_PHASE_1+i);
    if (value2 > value)
      value = value2;
  }
  float nominal = parameters.getValue(CCS_TRANS_NOMINAL_VOLTAGE);
  valueParameter_ =  value / (nominal / 100.0);
}

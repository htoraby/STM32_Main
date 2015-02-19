#include "protection_undervoltage_input.h"

ProtectionUnderVoltageInput::ProtectionUnderVoltageInput()
{
  idMode_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
  idReaction_= CCS_PROT_SUPPLY_UNDERVOLTAGE_REACTION;
  idActivDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER;
  idParam2_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER_2;
  idState_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
  idTimer_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET_COUNT;

  protActivatedEventId_ = UnderVoltInProtActivId;
  apvEventId_ = UnderVoltInApvId;
  apvDisabledEventId_ = UnderVoltInApvDisabledId;
  protBlockedEventId_ = UnderVoltInProtBlockedId;
}

ProtectionUnderVoltageInput::~ProtectionUnderVoltageInput()
{

}

void ProtectionUnderVoltageInput::init()
{
  Protection::init("ProtUnderVoltInput");
}

bool ProtectionUnderVoltageInput::checkAlarm()
{
  calcValue();
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionUnderVoltageInput::checkBlock()
{
  calcValue();
  return Protection::isLowerLimit(restartSetpoint_);
}

void ProtectionUnderVoltageInput::calcValue()
{
  float value = 0;
  for (int i = 0; i < 3; ++i) {
    float value2 = parameters.getValue(EM_VOLTAGE_PHASE_1+i);
    if (value2 < value)
      value = value2;
  }
  float nominal = parameters.getValue(CCS_TRANS_NOMINAL_VOLTAGE);
  valueParameter_ =  value / (nominal / 100.0);
}

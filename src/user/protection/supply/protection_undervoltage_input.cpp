#include "protection_undervoltage_input.h"

ProtectionUnderVoltageInput::ProtectionUnderVoltageInput()
{
  idMode_= CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
  idPrevent_= CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT;
  idActivDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT;
  if (1)  // По ТТ СНГ
    idRestartSetpoint_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT;
  else
    idRestartSetpoint_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_BLOCK_FLAG;
  idState_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME;

  protReactEventId_ = UnderVoltInProtReactId;
  apvEventId_ = UnderVoltInApvId;
  apvDisabledEventId_ = UnderVoltInApvDisabledId;
  protBlockedEventId_ = UnderVoltInProtBlockedId;
}

ProtectionUnderVoltageInput::~ProtectionUnderVoltageInput()
{

}

void ProtectionUnderVoltageInput::getOtherSetpointProt()
{
  timerDifStartFlag_ = ksu.getValue(idParam_);
}

bool ProtectionUnderVoltageInput::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionUnderVoltageInput::checkPrevent()
{
  if (restart_)
    return Protection::isLowerLimit(restartSetpoint_);
  else
    return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionUnderVoltageInput::calcValue()
{
  float value = parameters.getValue(EM_VOLTAGE_PHASE_1);
  float value2 = parameters.getValue(EM_VOLTAGE_PHASE_2);
  float value3 = parameters.getValue(EM_VOLTAGE_PHASE_3);

  value = min(min(value, value2), value3);

  float nominal = parameters.getValue(CCS_TRANS_NOMINAL_VOLTAGE);
  return (value / (nominal / 100.0));
}

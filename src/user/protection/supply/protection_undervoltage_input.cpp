#include "protection_undervoltage_input.h"

ProtectionUnderVoltageInput::ProtectionUnderVoltageInput()
{
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
  getIdProtection(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_REACTION,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER_2,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT,
                  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET_COUNT,
                  CCS_SUPPLY_INPUT_VOLTAGE_AVERAGE);
  Protection::init("ProtectionUndervoltageInput");
}

bool ProtectionUnderVoltageInput::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionUnderVoltageInput::checkBlock()
{
  return Protection::isLowerLimit(restartSetpoint_);
}

#include "protection_overvoltage_input.h"

ProtectionOverVoltageInput::ProtectionOverVoltageInput()
{

}

ProtectionOverVoltageInput::~ProtectionOverVoltageInput()
{

}

void ProtectionOverVoltageInput::init()
{
  getIdProtection(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_REACTION,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER_2,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_MODE,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_TIME,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT,
                  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT,
                  CCS_SUPPLY_INPUT_VOLTAGE_AVERAGE);
  Protection::init("ProtectionOvervoltageInput");
}

bool ProtectionOverVoltageInput::checkTripSetPoint()
{
  return Protection::checkTripSetPoint(true);
}

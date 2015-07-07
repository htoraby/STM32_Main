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
  lastReasonStop_ = LastReasonStopOverVoltIn;

  protReactEventId_ = OverVoltInProtReactId;
  apvEventId_ = OverVoltInApvId;
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
  float value = 0;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_1) == ok_r) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = 0;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_2) == ok_r) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = 0;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_3) == ok_r) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = max(max(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.getValidity(CCS_TRANS_NOMINAL_VOLTAGE) == ok_r) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE);
  }

  return (value / ((nominal / SQRT_3) / 100.0));
}

bool ProtectionOverVoltageInput::isProtect()
{
  if (parameters.get(CCS_EM_TYPE) == EM_TYPE_NONE) {
    if (((parameters.getValidity(CCS_VOLTAGE_PHASE_1) == ok_r) ||
         (parameters.getValidity(CCS_VOLTAGE_PHASE_2) == ok_r) ||
         (parameters.getValidity(CCS_VOLTAGE_PHASE_3) == ok_r))&&
         (parameters.getValidity(CCS_TRANS_NOMINAL_VOLTAGE) == ok_r)) {
         return true;
    }
  }
  else {
    if (em->isConnect()) {
      if (((parameters.getValidity(CCS_VOLTAGE_PHASE_1) == ok_r) ||
           (parameters.getValidity(CCS_VOLTAGE_PHASE_2) == ok_r) ||
           (parameters.getValidity(CCS_VOLTAGE_PHASE_3) == ok_r))&&
           (parameters.getValidity(CCS_TRANS_NOMINAL_VOLTAGE) == ok_r)) {
           return true;
      }
    }
  }
  return false;
}

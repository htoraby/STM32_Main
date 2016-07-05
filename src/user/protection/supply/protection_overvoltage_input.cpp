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
  idDelayFlag_ = CCS_PROT_SUPPLY_OVERVOLTAGE_DELAY_FLAG;
  idState_ = CCS_PROT_SUPPLY_OVERVOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_OVERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvOverVoltIn;


  protReactEventId_ = OverVoltInProtReactId;
  apvEventId_ = OverVoltInApvId;
  apvDisabledEventId_ = OverVoltInApvDisabledId;
  protBlockedEventId_ = OverVoltInProtBlockedId;

  difStartFlag_ = true;
}

ProtectionOverVoltageInput::~ProtectionOverVoltageInput()
{

}

bool ProtectionOverVoltageInput::checkAlarm()
{
  if (vsd->checkAlarmVsdOverVoltage()) {
    lastReasonStop_ = LastReasonStopHardwareVsd;
    activDelay_ = 0;
    tripDelay_ = 0;
    return true;
  }
  else if (Protection::isHigherLimit(tripSetpoint_)) {
    lastReasonStop_ = LastReasonStopOverVoltIn;
    return true;
  }
  else {
    return false;
  }
}

bool ProtectionOverVoltageInput::checkPrevent()
{
  if (restart_)
    return (Protection::isHigherLimit(restartSetpoint_) || vsd->checkAlarmVsdOverVoltage());
  else
    return (Protection::isHigherLimit(tripSetpoint_) || vsd->checkAlarmVsdOverVoltage());
}

float ProtectionOverVoltageInput::calcValue()
{
  float value = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_1)) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_2)) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = max(max(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE_INPUT)) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT) / SQRT_3;
  }

  return (value / (nominal / 100.0));
}

bool ProtectionOverVoltageInput::isProtect()
{
  if ((parameters.isValidity(CCS_VOLTAGE_PHASE_1) ||
       parameters.isValidity(CCS_VOLTAGE_PHASE_2) ||
       parameters.isValidity(CCS_VOLTAGE_PHASE_3)) &&
      parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE_INPUT)) {
    return true;
  }
  return false;
}

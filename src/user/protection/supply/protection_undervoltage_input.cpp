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
  idDelayFlag_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_DELAY_FLAG;
  idState_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE;
  idTimer_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME;
  idRestartCount_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvUnderVoltIn;
  lastReasonStop_ = LastReasonStopUnderVoltIn;

  protReactEventId_ = UnderVoltInProtReactId;
  apvEventId_ = UnderVoltInApvId;
  apvDisabledEventId_ = UnderVoltInApvDisabledId;
  protBlockedEventId_ = UnderVoltInProtBlockedId;

  difStartFlag_ = true;
}

ProtectionUnderVoltageInput::~ProtectionUnderVoltageInput()
{

}

bool ProtectionUnderVoltageInput::checkAlarm()
{
  if (vsd->checkAlarmVsdUnderVoltage()) {
    lastReasonStop_ = LastReasonStopHardwareVsd;
    activDelay_ = 0;
    tripDelay_ = 0;
    return true;
  }
  else if (Protection::isLowerLimit(tripSetpoint_)) {
    lastReasonStop_ = LastReasonStopUnderVoltIn;
    return true;
  }
  else {
    return false;
  }
}

bool ProtectionUnderVoltageInput::checkPrevent()
{
  if (restart_)
    return (Protection::isLowerLimit(restartSetpoint_) || vsd->checkAlarmVsdUnderVoltage());
  else
    return (Protection::isLowerLimit(tripSetpoint_) || vsd->checkAlarmVsdUnderVoltage());
}

float ProtectionUnderVoltageInput::calcValue()
{
  float value = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_1)) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_2)) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = min(min(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE_INPUT)) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT) / SQRT_3;
  }

  return (value / (nominal / 100.0));
}

bool ProtectionUnderVoltageInput::isProtect()
{
#if USE_DEBUG
  return true;
#endif

  if ((parameters.isValidity(CCS_VOLTAGE_PHASE_1) ||
       parameters.isValidity(CCS_VOLTAGE_PHASE_2) ||
       parameters.isValidity(CCS_VOLTAGE_PHASE_3)) &&
      parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE_INPUT)) {
    return true;
  }
  return false;
}

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

  lastReasonRun_ = LastReasonRunApvUnderVoltIn;
  lastReasonStop_ = LastReasonStopUnderVoltIn;

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
  float value = NOM_VOLTAGE;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_1) == ok_r) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = NOM_VOLTAGE;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_2) == ok_r) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = NOM_VOLTAGE;
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_3) == ok_r) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = min(min(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.getValidity(CCS_TRANS_NOMINAL_VOLTAGE) == ok_r) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE) / SQRT_3;
  }

  return (value / (nominal / 100.0));
}

bool ProtectionUnderVoltageInput::isProtect()
{
#if USE_DEBUG
    return true;
#endif

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

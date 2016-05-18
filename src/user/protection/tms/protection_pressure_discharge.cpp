#include "protection_pressure_discharge.h"

ProtectionPressureDischarge::ProtectionPressureDischarge()
{
  idMode_= CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE;
  idPrevent_= CCS_PROT_DHS_PRESSURE_DISCHARGE_PREVENT;
  idActivDelay_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_DELAY_FLAG;
  idState_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_STATE;
  idTimer_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_TIME;
  idRestartCount_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvPressureDischarge;
  lastReasonStop_ = LastReasonStopPressureDischarge;

  protReactEventId_ = PressureDischargeProtReactId;
  apvEventId_ = PressureDischargeApvId;
  apvDisabledEventId_ = PressureDischargeApvDisabledId;
  protBlockedEventId_ = PressureDischargeProtBlockedId;
}

ProtectionPressureDischarge::~ProtectionPressureDischarge()
{

}

bool ProtectionPressureDischarge::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionPressureDischarge::checkPrevent()
{
  if (restart_)
    return Protection::isHigherLimit(restartSetpoint_);
  else
    return Protection::isHigherLimit(tripSetpoint_);
}

float ProtectionPressureDischarge::calcValue()
{
  return parameters.get(TMS_PRESSURE_DISCHARGE);
}

bool ProtectionPressureDischarge::isProtect()
{
  if (parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE)
    if (tms->isConnect())
      if (parameters.isValidity(TMS_PRESSURE_DISCHARGE))
        return true;
  return false;
}

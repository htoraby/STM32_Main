#include "protection_pressure_intake.h"

ProtectionPressureIntake::ProtectionPressureIntake()
{

  idMode_= CCS_PROT_DHS_PRESSURE_INTAKE_REACTION;
  idActivDelay_ = CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER;
  idParam2_ = CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER_2;
  idState_ = CCS_PROT_DHS_PRESSURE_INTAKE_MODE;
  idTimer_ = CCS_PROT_DHS_PRESSURE_INTAKE_TIME;
  idRestartCount_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET_COUNT;

  protModeEventId_ = PressureIntakeProtActivId;
  apvEventId_ = PressureIntakeApvId;
  apvDisabledEventId_ = PressureIntakeApvDisabledId;
  protBlockedEventId_ = PressureIntakeProtBlockedId;
}

ProtectionPressureIntake::~ProtectionPressureIntake()
{

}

bool ProtectionPressureIntake::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionPressureIntake::checkBlock()
{
  if (alarm_) {
    return true;
  } else {
    if (Protection::isLowerLimit(restartSetpoint_)) /* && (state_ == ProtectionStateRestartWait))*/
      return true;
  }

  return false;
}

float ProtectionPressureIntake::calcValue()
{
  return parameters.getValue(TMS_PRESSURE_INTAKE);
}

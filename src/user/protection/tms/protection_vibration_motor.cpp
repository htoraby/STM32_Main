#include "protection_vibration_motor.h"

ProtectionVibrationMotor::ProtectionVibrationMotor()
{
  idMode_= CCS_PROT_DHS_VIBRATION_MODE;
  idPrevent_= CCS_PROT_DHS_VIBRATION_PREVENT;
  idActivDelay_ = CCS_PROT_DHS_VIBRATION_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_VIBRATION_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_VIBRATION_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_VIBRATION_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_VIBRATION_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_VIBRATION_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_VIBRATION_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_VIBRATION_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_VIBRATION_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DHS_VIBRATION_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DHS_VIBRATION_DELAY_FLAG;
  idState_ = CCS_PROT_DHS_VIBRATION_STATE;
  idTimer_ = CCS_PROT_DHS_VIBRATION_TIME;
  idRestartCount_ = CCS_PROT_DHS_VIBRATION_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_VIBRATION_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvVibrationMotor;
  lastReasonStop_ = LastReasonStopVibrationMotor;

  protReactEventId_ = VibrationMotorProtReactId;
  apvEventId_ = VibrationMotorApvId;
  apvDisabledEventId_ = VibrationMotorApvDisabledId;
  protBlockedEventId_ = VibrationMotorProtBlockedId;
}

ProtectionVibrationMotor::~ProtectionVibrationMotor()
{

}

bool ProtectionVibrationMotor::checkAlarm()
{
  return Protection::isHigherLimit(tripSetpoint_);
}

bool ProtectionVibrationMotor::checkPrevent()
{
  return alarm_;
}

float ProtectionVibrationMotor::calcValue()
{
  float value = 0;
  if (parameters.isValidity(TMS_ACCELERATION_XY_INTAKE)) {
    value = parameters.get(TMS_ACCELERATION_XY_INTAKE);
  }
  float value2 = 0;
  if (parameters.isValidity(TMS_ACCELERATION_Z_INTAKE)) {
    value2 = parameters.get(TMS_ACCELERATION_Z_INTAKE);
  }

  value = max(value, value2);

  return value;
}

bool ProtectionVibrationMotor::isProtect()
{
  if (parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE)
    if (tms->isConnect()) {
      if (parameters.isValidity(TMS_ACCELERATION_XY_INTAKE) ||
          parameters.isValidity(TMS_ACCELERATION_Z_INTAKE))
        return true;
    }
  return false;
}

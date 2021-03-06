#include "protection_overload_motor.h"

ProtectionOverloadMotor::ProtectionOverloadMotor()
{
  idMode_= CCS_PROT_MOTOR_OVERLOAD_MODE;
  idPrevent_= CCS_PROT_MOTOR_OVERLOAD_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_OVERLOAD_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_OVERLOAD_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_MOTOR_OVERLOAD_DELAY_FLAG;
  idState_ = CCS_PROT_MOTOR_OVERLOAD_STATE;
  idTimer_ = CCS_PROT_MOTOR_OVERLOAD_TIME;

  idRestartCount_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvOverloadMotor;
  lastReasonStop_ = LastReasonStopOverloadMotor;

  protReactEventId_ = OverloadMotorProtReactId;
  apvEventId_ = OverloadMotorApvId;
  apvDisabledEventId_ = OverloadMotorApvDisabledId;
  protBlockedEventId_ = OverloadMotorProtBlockedId;
}

ProtectionOverloadMotor::~ProtectionOverloadMotor()
{

}

bool ProtectionOverloadMotor::externalCheckAlarm()
{
  return checkAlarm();
}

void ProtectionOverloadMotor::getOtherSetpointProt()
{
  // Пересчёт задержки срабатывания в зависимости от загрузки двигателя
  static uint8_t delayCalc = 0;
  float checkLoad = calcValue();
  float load = checkLoad;

  float checkTripDelay = tripDelay_;

  if (load == 0) {
    load = tripSetpoint_;
  }

  tripDelay_ = tripDelay_ * pow((tripSetpoint_ / load), 2);

  if (tripDelay_ < checkTripDelay) {
    checkTripDelay = tripDelay_;
  }

  // Вывод текущего перегруза и текущей задержки срабатывания на экран
  if (delayCalc < 5) {
    delayCalc++;
  }
  else {
    delayCalc = 0;
    parameters.set(CCS_PROT_MOTOR_OVERLOAD_CALC_TRIP_DELAY, checkTripDelay);
    parameters.set(CCS_PROT_MOTOR_OVERLOAD_CALC_LOAD, checkLoad);
  }
}

bool ProtectionOverloadMotor::checkAlarm()
{
  float alarm = vsd->checkAlarmVsdOverloadMotor();

  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE)) {
    return false;
  }

  if (alarm) {
    lastReasonStop_ = (LastReasonStop)(int)alarm;
    //lastReasonStop_ = LastReasonStopHardwareVsd;
    activDelay_ = 0;
    tripDelay_ = 0;
    return true;
  }
  else if (Protection::isHigherLimit(tripSetpoint_)) {
    lastReasonStop_ = LastReasonStopOverloadMotor;
    return true;
  }
  else {
    return false;
  }
}

float ProtectionOverloadMotor::calcValue()
{
  float value = 0;
  if (parameters.isValidity(CCS_MOTOR_CURRENT_PHASE_1)) {
    value = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  }
  float value2 = 0;
  if (parameters.isValidity(CCS_MOTOR_CURRENT_PHASE_2)) {
    value2 = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  }
  float value3 = 0;
  if (parameters.isValidity(CCS_MOTOR_CURRENT_PHASE_3)) {
    value3 = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  }

  value = max(max(value, value2), value3);

  float nominal = 100;
  if (parameters.isValidity(VSD_MOTOR_CURRENT)) {
    nominal = parameters.get(VSD_MOTOR_CURRENT);
    if (nominal == 0)
      nominal = 100;
  }

  return (value / (nominal / 100.0));
}

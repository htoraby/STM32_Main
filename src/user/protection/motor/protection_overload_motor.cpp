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

void ProtectionOverloadMotor::getOtherSetpointProt()
{
  // Пересчёт задержки срабатывания в зависимости от загрузки двигателя
  tripDelay_ = tripDelay_ * pow((tripSetpoint_ / calcValue()), 2);

  // Если включен режим работы с пониженным сопротивлением изоляции
  // и изоляция ниже уставки, сбрасываем в 0 задержки активации и срабатывания
  if (ksu.getValue(CCS_PROT_DHS_RESISTANCE_PREVENT) ||
      ksu.getValue(CCS_PROT_DHS_RESISTANCE_PARAMETER)) {
    activDelay_ = 0.0;
    tripDelay_ = 0.0;
  }
}

bool ProtectionOverloadMotor::checkAlarm()
{
  // Возвращаем аварию если превышена уставка по перегрузу или установлен бит
  // в слове состояния ЧРП
  return ((Protection::isHigherLimit(tripSetpoint_)) ||
          (vsd->checkVsdStatus(VSD_STATUS_M_I2T_ERR)));
}

float ProtectionOverloadMotor::calcValue()
{
  float value = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);

  value = max(max(value, value2), value3);

  float nominal = parameters.get(VSD_MOTOR_CURRENT);
  return (value / (nominal / 100.0));
}

#include "protection_underload_motor.h"
#include "regime.h"

ProtectionUnderloadMotor::ProtectionUnderloadMotor()
{
  idMode_= CCS_PROT_MOTOR_UNDERLOAD_MODE;
  idPrevent_= CCS_PROT_MOTOR_UNDERLOAD_PREVENT;
  idActivDelay_ = CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_UNDERLOAD_PARAMETER;
  idRestartFlag_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_MOTOR_UNDERLOAD_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_MOTOR_UNDERLOAD_DELAY_FLAG;
  idState_ = CCS_PROT_MOTOR_UNDERLOAD_STATE;
  idTimer_ = CCS_PROT_MOTOR_UNDERLOAD_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvUnderloadMotor;
  lastReasonStop_ = LastReasonStopUnderloadMotor;

  protReactEventId_ = UnderloadMotorProtReactId;
  apvEventId_ = UnderloadMotorApvId;
  apvDisabledEventId_ = UnderloadMotorApvDisabledId;
  protBlockedEventId_ = UnderloadMotorProtBlockedId;
}

ProtectionUnderloadMotor::~ProtectionUnderloadMotor()
{

}

void ProtectionUnderloadMotor::getOtherSetpointProt()
{ 
  if (ksu.getValue(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_FLAG)) {
    float progressiveRestartDelay = ksu.getValue(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_DELAY);
    progressiveRestartCount_ = ksu.getValue(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_COUNT);
    restartDelay_ = restartDelay_ + progressiveRestartDelay * progressiveRestartCount_;
  }
  else {
    progressiveRestartCount_ = 0;
  }

  // Если включен режим "Периодический"
  if (parameters.get(CCS_RGM_CHANGE_FREQ_MODE) != Regime::OffAction) {
    float beginFreq_ = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ);
    float endFreq_ = parameters.get(CCS_RGM_CHANGE_FREQ_END_FREQ);
    float beginUnderload_ = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_UNDERLOAD);
    if (endFreq_ - beginFreq_) {
      tripSetpoint_ = beginUnderload_ + (tripSetpoint_ - beginUnderload_) *
          (parameters.get(VSD_FREQUENCY) - beginFreq_) / (endFreq_ - beginFreq_);
    }
  } else {
    if (ksu.getValue(idParam_) && (vsd->getNominalFreq() > 0))
      tripSetpoint_ = tripSetpoint_ * pow(vsd->getCurrentFreq(), 2) / pow(vsd->getNominalFreq(), 2);
  }

  // Если включен режим "Чередования частот"
  if (parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE) != Regime::OffAction) {
    if (vsd->getCurrentFreq() == parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_1)) {
      tripSetpoint_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_1);
    }
    if (vsd->getCurrentFreq() == parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_2)) {
      tripSetpoint_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_2);
    }
  }
}

void ProtectionUnderloadMotor::setOtherParamProt()
{
  parameters.set(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_COUNT, progressiveRestartCount_);
}

bool ProtectionUnderloadMotor::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

float ProtectionUnderloadMotor::calcValue()
{
  float value = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  float nominal = parameters.get(VSD_MOTOR_CURRENT);

  value = min(min(value, value2), value3);

  float cos = parameters.get(CCS_MOTOR_COS_PHI_NOW);
  float nominalCos = parameters.get(VSD_MOTOR_COS_PHI);

  return (value * cos) / (nominal * nominalCos) * 100.0;
}

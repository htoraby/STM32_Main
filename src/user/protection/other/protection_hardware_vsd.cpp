#include "protection_hardware_vsd.h"

ProtectionHardwareVsd::ProtectionHardwareVsd()
{
  idMode_= CCS_PROT_OTHER_VSD_MODE;
  idPrevent_= CCS_PROT_OTHER_VSD_PREVENT;
  idActivDelay_ = CCS_PROT_OTHER_VSD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_VSD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_VSD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_VSD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_VSD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_VSD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_VSD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_VSD_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_VSD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_VSD_BLOCK_FLAG;
  idState_ = CCS_PROT_OTHER_VSD_STATE;
  idTimer_ = CCS_PROT_OTHER_VSD_TIME;
  idRestartCount_ = CCS_PROT_OTHER_VSD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_OTHER_VSD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvHardwareVsd;
  lastReasonStop_ = LastReasonStopHardwareVsd;

  protReactEventId_ = HardwareVsdProtReactId;
  apvEventId_ = HardwareVsdApvId;
  apvDisabledEventId_ = HardwareVsdApvDisabledId;
  protBlockedEventId_ = HardwareVsdProtBlockedId;
}

ProtectionHardwareVsd::~ProtectionHardwareVsd()
{

}

bool ProtectionHardwareVsd::checkAlarm()
{
  if (vsd->checkVsdStatus(VSD_STATUS_FAULT_STOPPED)) {
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_UD_LOW_FAULT)) {
    protReactEventId_ = HardwareVsdUdLowProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_UD_HIGH_FAULT)) {
    protReactEventId_ = HardwareVsdUdHighProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_UIN_ASYM)) {
    protReactEventId_ = HardwareVsdUinAsymProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_URECT_SHORTCIRCUIT)) {
    protReactEventId_ = HardwareVsdShortCirProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_FC_I2T_ERR)) {
    protReactEventId_ = HardwareVsdI2tProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_AST_ERR)) {
    protReactEventId_ = HardwareVsdAstProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_VC_ERR)) {
    protReactEventId_ = HardwareVsdVcProtReactId;
    return true;
  }
//  TODO: Прояснить вопрос с перегрузом
//  if (vsd->checkVsdStatus(VSD_STATUS_M_I2T_ERR)) {
//    return true;
//  }
//  if (vsd->checkVsdStatus(VSD_STATUS_I_FAST_ERR)) {
//    return true;
//  }

  return false;
}


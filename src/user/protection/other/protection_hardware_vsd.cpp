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
  if (!vsd->isConnect()) {
    protReactEventId_ = HardwareVsdProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_FAULT_STOPPED)) {
    protReactEventId_ = HardwareVsdProtReactId;
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
  if (vsd->checkVsdStatus(VSD_STATUS_I_LIMIT)) {
    protReactEventId_ = HardwareVsdCurrentLimitReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_UNDERLOAD)) {
    protReactEventId_ = HardwareVsdUndervoltageProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_RESISTANCE)) {
    protReactEventId_ = HardwareVsdResistanceProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_OVERVOLTAGE)) {
    protReactEventId_ = HardwareVsdOvervoltageProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_UNDERVOLTAGE)) {
    protReactEventId_ = HardwareVsdUndervoltageProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_RUN_COUNT)) {
    protReactEventId_ = HardwareVsdRunCountProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_OVERHEAT_FILTER)) {
    protReactEventId_ = HardwareVsdOverheatFilterProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_DRV0)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_DRV1)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_DRV2)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_MONOMETR)) {
    protReactEventId_ = HardwareVsdMonometrProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_AI_0)) {
    protReactEventId_ = HardwareVsdAI0ProtReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_THYR_ABC_STATE)) {
    protReactEventId_ = HardwareVsdSequencePhaseReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_OVERHEAT_MOTOR)) {
    protReactEventId_ = HardwareVsdOverheatMotorReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_OVERVIBRATION)) {
    protReactEventId_ = HardwareVsdOvervibrationReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_PRESSURE)) {
    protReactEventId_ = HardwareVsdPressureReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_FAULT_STOPPED)) {
    protReactEventId_ = HardwareVsd19ReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_IZ)) {
    protReactEventId_ = HardwareVsdImbalanceCurrentReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_CLK_MON)) {
    protReactEventId_ = HardwareVsdMemoryReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_CTR_MON)) {
    protReactEventId_ = HardwareVsdDIReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_AN_MON)) {
    protReactEventId_ = HardwareVsdADCReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_MB_MON)) {
    protReactEventId_ = HardwareVsdSensorSupplyReactId;
    return true;
  }
  if (vsd->checkVsdStatus(VSD_STATUS_CLK_MON)) {
    protReactEventId_ = HardwareVsdEepromReactId;
    return true;
  }
  return false;
}

bool ProtectionHardwareVsd::checkPrevent() {
  if (!vsd->isConnect()) {
    return true;
  }
  return false;
}

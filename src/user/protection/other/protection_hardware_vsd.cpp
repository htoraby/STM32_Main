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
  idDelayFlag_ = CCS_PROT_OTHER_VSD_DELAY_FLAG;
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
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_NO_CONNECT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_FAULT_STOPPED)) {
    protReactEventId_ = HardwareVsdProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_FAULT_STOPPED);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_UD_LOW_FAULT)) {
    protReactEventId_ = HardwareVsdUdLowProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_UD_LOW_FAULT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_UD_HIGH_FAULT)) {
    protReactEventId_ = HardwareVsdUdHighProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_UD_HIGH_FAULT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_UIN_ASYM)) {
    protReactEventId_ = HardwareVsdUinAsymProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_UIN_ASYM);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_URECT_SHORTCIRCUIT)) {
    protReactEventId_ = HardwareVsdShortCirProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_URECT_SHORTCIRCUIT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_FC_I2T_ERR)) {
    protReactEventId_ = HardwareVsdI2tProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_FC_I2T_ERR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_AST_ERR)) {
    protReactEventId_ = HardwareVsdAstProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_AST_ERR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_VC_ERR)) {
    protReactEventId_ = HardwareVsdVcProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_VC_ERR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_I_LIMIT)) {
    protReactEventId_ = HardwareVsdCurrentLimitReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_I_LIMIT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_UNDERLOAD)) {
    protReactEventId_ = HardwareVsdUndervoltageProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_UNDERLOAD);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_RESISTANCE)) {
    protReactEventId_ = HardwareVsdResistanceProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_RESISTANCE);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_OVERVOLTAGE)) {
    protReactEventId_ = HardwareVsdOvervoltageProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_OVERVOLTAGE);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_UNDERVOLTAGE)) {
    protReactEventId_ = HardwareVsdUndervoltageProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_UNDERVOLTAGE);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_RUN_COUNT)) {
    protReactEventId_ = HardwareVsdRunCountProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_RUN_COUNT);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_OVERHEAT_FILTER)) {
    protReactEventId_ = HardwareVsdOverheatFilterProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_OVERHEAT_FILTER);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_DRV0)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_DRV0);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_DRV1)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_DRV1);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_DRV2)) {
    protReactEventId_ = HardwareVsdSupplyDriversProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_DRV2);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_MONOMETR)) {
    protReactEventId_ = HardwareVsdMonometrProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_MONOMETR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_AI_0)) {
    protReactEventId_ = HardwareVsdAI0ProtReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_AI_0);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_ABC_STATE)) {
    protReactEventId_ = HardwareVsdSequencePhaseReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_ABC_STATE);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_OVERHEAT_MOTOR)) {
    protReactEventId_ = HardwareVsdOverheatMotorReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_OVERHEAT_MOTOR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_OVERVIBRATION)) {
    protReactEventId_ = HardwareVsdOvervibrationReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_OVERVIBRATION);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_PRESSURE)) {
    protReactEventId_ = HardwareVsdPressureReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_PRESSURE);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_ERR_19)) {
    protReactEventId_ = HardwareVsd19ReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_ERR_19);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_IZ)) {
    protReactEventId_ = HardwareVsdImbalanceCurrentReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_IZ);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_CLK_MON)) {
    protReactEventId_ = HardwareVsdMemoryReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_CLK_MON);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_CTR_MON)) {
    protReactEventId_ = HardwareVsdDIReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_CTR_MON);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_AN_MON)) {
    protReactEventId_ = HardwareVsdADCReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_AN_MON);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_MB_MON)) {
    protReactEventId_ = HardwareVsdSensorSupplyReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_MB_MON);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_CLK_MON)) {
    protReactEventId_ = HardwareVsdEepromReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_CLK_MON);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_I_FAST_ERR)) {
    protReactEventId_ = HardwareVsdIfastReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_I_FAST_ERR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_M_I2T_ERR)) {
    protReactEventId_ = HardwareVsdMI2TReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_M_I2T_ERR);
    return true;
  }
  if (vsd->checkStatusVsd(VSD_STATUS_ERR_31)) {
    protReactEventId_ = HardwareVsdMI2TReactId;
    parameters.set(CCS_PROT_OTHER_VSD_ALARM, VSD_STATUS_ERR_31);
    return true;
  }

  return false;
}

bool ProtectionHardwareVsd::checkPrevent()
{
  return alarm_;
}

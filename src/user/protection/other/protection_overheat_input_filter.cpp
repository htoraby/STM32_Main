#include "protection_overheat_input_filter.h"

ProtectionOverheatInputFilter::ProtectionOverheatInputFilter()
{
  idMode_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_MODE;
  idPrevent_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_PREVENT;

  idActivDelay_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_DELAY_FLAG;
  idState_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_STATE;
  idTimer_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_TIME;
  idRestartCount_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvOverheatInputFilter;
  lastReasonStop_ = LastReasonStopOverheatInputFilter;

  protReactEventId_ = OverheatInputFilterProtReactId;
  protBlockedEventId_ = OverheatInputFilterProtBlockedId;
}

ProtectionOverheatInputFilter::~ProtectionOverheatInputFilter()
{

}

bool ProtectionOverheatInputFilter::checkAlarm()
{
  if (!checkBit(parameters.get(VSD_STATUS_WORD_1), VSD_DANFOSS_STATUS_OPERATION) &&
      !checkBit(parameters.get(VSD_DINPUTS), 1)) {                                // TODO: Условие аварии пока не известно
    protReactEventId_ = OverheatInputFilterProtReactId;
    return true;
  }
  return false;
}

bool ProtectionOverheatInputFilter::checkPrevent()
{
  return alarm_;
}

bool ProtectionOverheatInputFilter::isProtect()
{
  if (parameters.get(CCS_FILTER_INPUT)) {
    if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_DANFOSS) {
      if (vsd->isConnect()) {
        return true;
      }
    }
  }
  return false;
}


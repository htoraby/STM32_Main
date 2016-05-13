#include "protection_flow_discharge.h"

ProtectionFlowDischarge::ProtectionFlowDischarge()
{
  idMode_= CCS_PROT_DHS_FLOW_DISCHARGE_MODE;
  idPrevent_= CCS_PROT_DHS_FLOW_DISCHARGE_PREVENT;
  idActivDelay_ = CCS_PROT_DHS_FLOW_DISCHARGE_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_DHS_FLOW_DISCHARGE_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_DHS_FLOW_DISCHARGE_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_SETPOINT;
  idParam_ = CCS_PROT_DHS_FLOW_DISCHARGE_PARAMETER;
  idRestartFlag_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_DHS_FLOW_DISCHARGE_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_DHS_FLOW_DISCHARGE_DELAY_FLAG;
  idState_ = CCS_PROT_DHS_FLOW_DISCHARGE_STATE;
  idTimer_ = CCS_PROT_DHS_FLOW_DISCHARGE_TIME;
  idRestartCount_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvFlowDischarge;
  lastReasonStop_ = LastReasonStopFlowDischarge;

  protReactEventId_ = FlowDischargeProtReactId;
  apvEventId_ = FlowDischargeApvId;
  apvDisabledEventId_ = FlowDischargeApvDisabledId;
  protBlockedEventId_ = FlowDischargeProtBlockedId;
}

ProtectionFlowDischarge::~ProtectionFlowDischarge()
{

}

bool ProtectionFlowDischarge::checkAlarm()
{
  return Protection::isLowerLimit(tripSetpoint_);
}

bool ProtectionFlowDischarge::checkPrevent()
{
  return alarm_;
}

float ProtectionFlowDischarge::calcValue()
{
  return parameters.get(TMS_FLOW_DISCHARGE);
}

bool ProtectionFlowDischarge::isProtect()
{
  if (parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE)
    if (tms->isConnect())
      if (parameters.isValidity(TMS_FLOW_DISCHARGE))
        return true;
  return false;
}

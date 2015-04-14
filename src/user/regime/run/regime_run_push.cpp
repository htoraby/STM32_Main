#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::processingStateRunning()
{
  /*
  switch (control) {
  case value:

    break;
  default:
    break;
  }
  */
}

void RegimeRunPush::processingStateWork()
{

}

void RegimeRunPush::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);
  freq_ = parameters.get(CCS_RGM_RUN_PUSH_FREQ);
  time_ = parameters.get(CCS_RGM_RUN_PUSH_TIME);
  quantity_ = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);
  period_ = parameters.get(CCS_RGM_RUN_PUSH_PERIOD);
  voltage_ = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE);
}

void RegimeRunPush::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_PUSH_STATE, state_);
}

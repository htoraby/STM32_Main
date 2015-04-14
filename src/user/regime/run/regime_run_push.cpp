#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::getOtherSetPoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);
  freq_ = parameters.get(CCS_RGM_RUN_PUSH_FREQ);
  time_ = parameters.get(CCS_RGM_RUN_PUSH_TIME);
  quantity_ = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);
  period_ = parameters.get(CCS_RGM_RUN_PUSH_PERIOD);
  voltage_ = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE);
}

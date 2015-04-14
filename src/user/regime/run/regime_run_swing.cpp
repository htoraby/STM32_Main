#include "regime_run_swing.h"

RegimeRunSwing::RegimeRunSwing()
{

}

RegimeRunSwing::~RegimeRunSwing()
{

}

void RegimeRunSwing::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_SWING_MODE);
  state_ = parameters.get(CCS_RGM_RUN_SWING_STATE);
  freq_ = parameters.get(CCS_RGM_RUN_SWING_FREQ);
  quantity_ = parameters.get(CCS_RGM_RUN_SWING_QUANTITY);
  voltage_ = parameters.get(CCS_RGM_RUN_SWING_VOLTAGE);
}

void RegimeRunSwing::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_SWING_STATE, state_);
}



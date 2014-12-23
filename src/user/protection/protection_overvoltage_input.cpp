#include "protection_overvoltage_input.h"

ProtectionOvervoltageInput::ProtectionOvervoltageInput()
{

}

ProtectionOvervoltageInput::~ProtectionOvervoltageInput()
{

}

unsigned char ProtectionOvervoltageInput::protection()
{
  mode_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_MODE);
  action_= KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_ACTION);
  state_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_STATE);
  delayActiv_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_ACTIV);
  delayReaction_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_REACTION);
  delayRestart_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_RESTART);
  limitReaction_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_LIMIT_REACTION);
  timer_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_TIMER);
  valueParameter_ = calcControlParameter();

  switch ((int)state_) {
  case PROTECTION_STATE_OFF:
    // Станция в работе
    if (!checkWorkKSU()) {
      // Переходим в режим активации защиты
    }

    break;
  default:
    break;
  }
}

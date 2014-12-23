#include "protection.h"

Protection::Protection()
{

}

Protection::~Protection()
{

}

unsigned char Protection::checkWorkKSU()
{
  float state = KSU.getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUN)||(state == CCS_CONDITION_DELAY))
    return 0;
  else
    return 1;
}

// Автомат работы защиты
unsigned char Protection::protection()
{

  switch((unsigned char)state_){
  case  PROTECTION_STATE_OFF:

    break;
  case  PROTECTION_STATE_ACTIV_WAIT:

    break;
  case PROTECTION_STATE_ACTIV:

    break;
  case  PROTECTION_STATE_WORK_WAIT:

    break;
  case  PROTECTION_STATE_WORK:

    break;
  case  PROTECTION_STATE_REACTION_WAIT:

    break;
  case  PROTECTION_STATE_REACTION:

    break;
  case  PROTECTION_STATE_RESTART_WAIT:

    break;
  case  PROTECTION_STATE_RESTART:

    break;
  case  PROTECTION_STATE_STOP:

    break;
  case  PROTECTION_STATE_BLOCK:

    break;
  default:

    break;
  }
  return (unsigned char)state_;
}


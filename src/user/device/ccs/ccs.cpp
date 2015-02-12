/*
 * classController.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "ccs.h"
#include "gpio.h"

Ccs::Ccs()
  : Device(CCS_BEGIN)
  , controlModeOld(0)
  , conditionOld(0)
{
  initParameters();
}

Ccs::~Ccs()
{
	// TODO Auto-generated destructor stub
}

bool Ccs::checkStopCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_STOP)
      || (state == CCS_CONDITION_WAIT)
      || (state == CCS_CONDITION_BLOCK)) {
    return 0;
  }
  else
    return 1;
}

bool Ccs::checkWorkCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUN)||(state == CCS_CONDITION_DELAY))
    return 0;
  else
    return 1;
}

bool Ccs::checkBlockCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if (state == CCS_CONDITION_BLOCK)
    return 0;
  else
    return 1;
}

bool Ccs::checkAutoControlMode()
{
  unsigned int controlMode = (unsigned int)getValue(CCS_CONTROL_MODE);
  if (controlMode == CCS_CONTROL_MODE_AUTO)
    return 0;
  else
    return 1;
}

float Ccs::getTime()
{
  return getValue(CCS_DATE_TIME);
}

void Ccs::conditionChanged()
{
  int condition = getValue(CCS_CONDITION);
  if (condition != conditionOld) {
    conditionOld = condition;
    offAllLeds();
    switch (condition) {
      case CCS_CONDITION_WAIT:
        onLed(WaitLed);
        break;
      case CCS_CONDITION_DELAY:
        onLed(WaitLed);
        onLed(WorkLed);
        break;
      case CCS_CONDITION_BLOCK:
        onLed(StopLed);
        break;
      case CCS_CONDITION_RUN:
        onLed(WorkLed);
        break;
      default:
        onLed(StopLed);
        break;
    }
  }
}

void Ccs::controlModeChanged()
{
  int controlMode = getValue(CCS_CONTROL_MODE);
  if (controlMode != controlModeOld) {
    switch (controlMode) {
      case CCS_CONTROL_MODE_MANUAL:
        // TODO: времено для проверки старта
        setValue(CCS_CONDITION, CCS_CONDITION_RUN);
        break;
      case CCS_CONTROL_MODE_AUTO:

        break;
      default:
        // TODO: времено для проверки стопа
        setValue(CCS_CONDITION, CCS_CONDITION_STOP);
        break;
    }
  }
}

void Ccs::initParameters()
{
  parameters_ = parametersArray_;
  countParameter_ = sizeof(parametersArray_)/sizeof(parameter);

  // Пустой элемент массива
  parameters_[CCS_BEGIN - CCS_BEGIN].id                = CCS_BEGIN;
  parameters_[CCS_BEGIN - CCS_BEGIN].access            = ACCESS_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].operation         = OPERATION_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].physic            = PHYSIC_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].validity          = VALIDITY_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].update            = UPDATE_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].value         = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].min           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].max           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].def           = 0.0;

  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].id          = CCS_ACCESS_LEVEL;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].update      = UPDATE_ERROR;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].value   = ACCESS_OPERATOR;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].min     = ACCESS_ERROR;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].max     = ACCESS_LAST;
  parameters_[CCS_ACCESS_LEVEL - CCS_BEGIN].def     = ACCESS_OPERATOR;

  parameters_[CCS_CONDITION - CCS_BEGIN].id          = CCS_CONDITION;
  parameters_[CCS_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CONDITION - CCS_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[CCS_CONDITION - CCS_BEGIN].update      = UPDATE_ERROR;
  parameters_[CCS_CONDITION - CCS_BEGIN].value   = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].min     = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].max     = CCS_CONDITION_RUN;
  parameters_[CCS_CONDITION - CCS_BEGIN].def     = CCS_CONDITION_STOP;

  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].id          = CCS_CONTROL_MODE;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].update      = UPDATE_ERROR;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].value   = CCS_CONTROL_MODE_STOP;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].min     = CCS_CONTROL_MODE_STOP;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].max     = CCS_CONTROL_MODE_AUTO;
  parameters_[CCS_CONTROL_MODE - CCS_BEGIN].def     = CCS_CONTROL_MODE_STOP;
}

/*
 * classController.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "ccs.h"
#include "gpio.h"
#include "user_main.h"

Ccs::Ccs()
  : Device(CCS_BEGIN, parametersArray_, CCS_END - CCS_BEGIN)
  , conditionOld(-1)
{

}

Ccs::~Ccs()
{

}

static void ccsMainTask(void *p)
{
  (static_cast<Ccs*>(p))->mainTask();
}

static void ccsLedConditionTask(void *p)
{
  (static_cast<Ccs*>(p))->ledConditionTask();
}

static void ccsVsdConditionTask(void *p)
{
  (static_cast<Ccs*>(p))->vsdConditionTask();
}

void Ccs::init()
{  
  initParameters();
  readParameters();
}

void Ccs::initTask()
{
  osMessageQDef(LedMessageQ, 5, uint8_t);
  ledMessage_ = osMessageCreate(osMessageQ(LedMessageQ), NULL);
  osThreadDef(LedCondition, ccsLedConditionTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(LedCondition), this);

  osThreadDef(VsdConditionTask, ccsVsdConditionTask, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(VsdConditionTask), this);

  osThreadDef(CcsMain, ccsMainTask, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(CcsMain), this);
}

void Ccs::mainTask()
{
  while (1) {
    osDelay(10);

    checkCmd();
    conditionChanged();
  }
}

void Ccs::setLedCondition(LedCondition condition)
{
  osMessagePut(ledMessage_, condition, 0);
}

void Ccs::ledConditionTask()
{
  int condition = LedConditionStop;
  while (1) {
    osEvent event = osMessageGet(ledMessage_, 300);
    if (event.status == osEventMessage) {
      offAllLeds();
      condition = event.value.v;
      switch (condition) {
        case LedConditionStop:
          onLed(StopLed);
          break;
        case LedConditionWaitApv:
          onLed(WaitLed);
          break;
        case LedConditionDelay:
          onLed(WaitLed);
          onLed(WorkLed);
          break;
        case LedConditionBlock:
          onLed(StopLed);
          break;
        case LedConditionRun:
          onLed(WorkLed);
          break;
        case LedConditionWaitRun:
          onLed(StopLed);
          break;
      }
    }

    switch (condition) {
      case LedConditionRunning: case LedConditionStopping:
        toggleLed(WorkLed);
        break;
      case LedConditionWaitRun:
        toggleLed(WaitLed);
        break;
    }
  }
}

void Ccs::vsdConditionTask()
{
  while (1) {
    osDelay(10);

    int vsdCondition = getValue(CCS_VSD_CONDITION);
    switch (vsdCondition) {
      case VSD_CONDITION_STOP:
        if (!isBlockCCS())
          setValue(CCS_CONDITION, CCS_CONDITION_STOP);
        break;
      case VSD_CONDITION_STOPPING:
        if (vsd->checkStop()) {
          setValue(CCS_VSD_CONDITION, VSD_CONDITION_STOP);
        }
        break;
      case VSD_CONDITION_WAIT_STOP:
          if (vsd->stop() == RETURN_OK) {
            setLedCondition(LedConditionStopping);
            setValue(CCS_VSD_CONDITION, VSD_CONDITION_STOPPING);
          } else {
            // TODO: Ошибка останова
          }
        break;
      case VSD_CONDITION_RUN:
        if (getValue(CCS_CONDITION) != CCS_CONDITION_RUN) {
          if (vsd->getValue(VSD_FREQUENCY) == vsd->getValue(VSD_FREQUENCY_NOW))
            setValue(CCS_CONDITION, CCS_CONDITION_RUN);
        }
        break;
      case VSD_CONDITION_RUNNING:
        if (vsd->checkStart()) {
          setLedCondition(LedConditionRunning);
          // Запуск сохранения пускового архива
          logRunning.start();
          setValue(CCS_VSD_CONDITION, VSD_CONDITION_RUN);
        }
        break;
      case VSD_CONDITION_WAIT_RUN:
          if (vsd->start() == RETURN_OK) {
            setValue(CCS_VSD_CONDITION, VSD_CONDITION_RUNNING);
          } else {
            // TODO: Ошибка запуска
          }
        break;
    }
  }
}

void Ccs::conditionChanged()
{
  calcCondition();
  int condition = getValue(CCS_CONDITION);
  if (condition != conditionOld) {
    conditionOld = condition;
    switch (condition) {
    case CCS_CONDITION_WAIT_APV:
      setLedCondition(LedConditionWaitApv);
      break;
    case CCS_CONDITION_DELAY:
      setLedCondition(LedConditionDelay);
      break;
    case CCS_CONDITION_RUNNING:
      setLedCondition(LedConditionWaitRun);
      break;
    case CCS_CONDITION_RUN:
      setLedCondition(LedConditionRun);
      break;
    case CCS_CONDITION_STOPPING:

      break;
    default:
      setLedCondition(LedConditionStop);
      break;
    }
  }
}

void Ccs::calcCondition()
{
  int flag = getValue(CCS_CONDITION_FLAG);
  switch (flag) {
  case CCS_CONDITION_FLAG_BLOCK:
    setValue(CCS_CONDITION, CCS_CONDITION_BLOCK);
    break;
  case CCS_CONDITION_FLAG_DELAY:
    setValue(CCS_CONDITION, CCS_CONDITION_DELAY);
    break;
  case CCS_CONDITION_FLAG_RESTART:
    setValue(CCS_CONDITION, CCS_CONDITION_WAIT_APV);
    break;
  default:
    break;
  }
}

void Ccs::start(EventType type)
{
  setValue(CCS_LAST_EVENT_TYPE, type);
  setValue(CCS_CMD_START, 1);
}

void Ccs::stop(EventType type)
{
  setValue(CCS_LAST_EVENT_TYPE, type);
  setValue(CCS_CMD_STOP, 1);
}

void Ccs::checkCmd()
{
  int start = getValue(CCS_CMD_START);
  int stop = getValue(CCS_CMD_STOP);

  if (start) {
    setValue(CCS_CMD_START, 0);
    if (checkCanStart()) {
      setValue(CCS_CONDITION, CCS_CONDITION_RUNNING);
      setValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
    }
  } else if (stop) {
    setValue(CCS_CMD_STOP, 0);
    if (checkCanStop()) {
      setValue(CCS_CONDITION, CCS_CONDITION_STOPPING);
      setValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
    }
  }
}

bool Ccs::checkCanStart()
{
  if (getValue(CCS_VSD_CONDITION) != VSD_CONDITION_STOP)
    return false;
  if (isBlockCCS())
    return false;
  return true;
}

bool Ccs::checkCanStop()
{
  if (getValue(CCS_VSD_CONDITION) == VSD_CONDITION_STOP)
    return false;
  return true;
}

bool Ccs::isStopMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_STOPPING) ||
      (state == CCS_CONDITION_STOP)) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isWorkMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUNNING) ||
      (state == CCS_CONDITION_RUN)) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isDelayCCS()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_DELAY) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isRestartCCS()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_RESTART) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isBlockCCS()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_BLOCK) {
    return true;
  }
  else {
    return false;
  }
}

void Ccs::setDelayCCS()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_DELAY)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_DELAY);
}

void Ccs::setRestartCCS()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_RESTART)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_RESTART);
}

void Ccs::setBlockCCS()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_BLOCK)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_BLOCK);
}

void Ccs::resetDelayCCS()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_DELAY)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetRestartCCS()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_RESTART)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetBlockCCS()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_BLOCK)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

bool Ccs::isAutoMode()
{
  unsigned int controlMode = (unsigned int)getValue(CCS_WORKING_MODE);
  if ((controlMode == CCS_WORKING_MODE_AUTO) ||
      (controlMode == CCS_WORKING_MODE_PROGRAM)) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isManualMode()
{
  unsigned int controlMode = (unsigned int)getValue(CCS_WORKING_MODE);
  if (controlMode == CCS_WORKING_MODE_MANUAL) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isStopMode ()
{
  unsigned int controlMode = (unsigned int)getValue(CCS_WORKING_MODE);
  if (controlMode == CCS_WORKING_MODE_STOP) {
    return true;
  }
  else {
    return false;
  }
}

float Ccs::getTime()
{
  return getValue(CCS_DATE_TIME);
}

void Ccs::initParameters()
{
  // Пустой элемент массива
  parameters_[CCS_BEGIN - CCS_BEGIN].id                = CCS_BEGIN;
  parameters_[CCS_BEGIN - CCS_BEGIN].access            = ACCESS_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].operation         = OPERATION_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].physic            = PHYSIC_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].validity          = VALIDITY_GOOD;
  parameters_[CCS_BEGIN - CCS_BEGIN].value         = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].min           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].max           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].def           = 0.0;

  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].id          = CCS_PASSWORD_LEVEL;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].value   = ACCESS_OPERATOR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].min     = ACCESS_ERROR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].max     = ACCESS_LAST;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].def     = ACCESS_OPERATOR;

  parameters_[CCS_CONDITION - CCS_BEGIN].id          = CCS_CONDITION;
  parameters_[CCS_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CONDITION - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CONDITION - CCS_BEGIN].value   = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].min     = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].max     = CCS_CONDITION_RUN;
  parameters_[CCS_CONDITION - CCS_BEGIN].def     = CCS_CONDITION_STOP;

  parameters_[CCS_WORKING_MODE - CCS_BEGIN].id          = CCS_WORKING_MODE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].value   = CCS_WORKING_MODE_STOP;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].min     = CCS_WORKING_MODE_STOP;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].max     = CCS_WORKING_MODE_PROGRAM;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].def     = CCS_WORKING_MODE_STOP;

  parameters_[CCS_CMD_STOP - CCS_BEGIN].id          = CCS_CMD_STOP;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].value   = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].min     = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].max     = 1;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].def     = 0;

  parameters_[CCS_CMD_START - CCS_BEGIN].id          = CCS_CMD_START;
  parameters_[CCS_CMD_START - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_START - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_START - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_START - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CMD_START - CCS_BEGIN].value   = 0;
  parameters_[CCS_CMD_START - CCS_BEGIN].min     = 0;
  parameters_[CCS_CMD_START - CCS_BEGIN].max     = 1;
  parameters_[CCS_CMD_START - CCS_BEGIN].def     = 0;

  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].id          = CCS_VSD_CONDITION;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].value   = VSD_CONDITION_STOP;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].min     = VSD_CONDITION_STOP;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].max     = VSD_CONDITION_WAIT_RUN;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].def     = VSD_CONDITION_STOP;
}

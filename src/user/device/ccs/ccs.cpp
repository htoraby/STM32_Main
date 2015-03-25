/*
 * classController.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "ccs.h"
#include "gpio.h"
#include "user_main.h"
#include "protection.h"

Ccs::Ccs()
  : Device(CCS_BEGIN, parametersArray_, CCS_END - CCS_BEGIN)
  , conditionOld_(-1)
  , flagOld_(-1)
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
    calcTime();
  }
}

void Ccs::setLedCondition(LedCondition condition)
{
  osMessagePut(ledMessage_, condition, 0);
}

void Ccs::ledConditionTask()
{
  int condition = OnRedLed;
  while (1) {
    osEvent event = osMessageGet(ledMessage_, 300);
    if (event.status == osEventMessage) {
      offAllLeds();
      condition = event.value.v;
      switch (condition) {
      case OnRedLed:
        onLed(StopLed);
        break;
      case OnRedOnYellowLed:
        onLed(StopLed);
        onLed(WaitLed);   
        break;
      case OnGreenToogleYellowLed:
        onLed(WorkLed);
        break;
      case OnGreenLed:
        onLed(WorkLed);
        break;
      case ToogleYellowLed:
        onLed(StopLed);
        break;
      }
    }

    switch (condition) {
    case ToogleGreenLed:
      toggleLed(WorkLed);
      break;
    case ToogleYellowLed:
      toggleLed(WaitLed);
      break;
    case OnGreenToogleYellowLed:
      toggleLed(WaitLed);
      break;
    case ToogleGreenToogleYellowLed:
      toggleLed(WorkLed);
      toggleLed(WaitLed);
      break;
    case ToogleRedLed:
      toggleLed(StopLed);
      break;
    case ToogleGreenToogleRedLed:
      toggleLed(WorkLed);
      toggleLed(StopLed);
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
      if (getValue(CCS_CONDITION) != CCS_CONDITION_STOP)
        setValue(CCS_CONDITION, CCS_CONDITION_STOP);
      break;
    case VSD_CONDITION_STOPPING:
      if (vsd->checkStop()) {
        setValue(CCS_VSD_CONDITION, VSD_CONDITION_STOP);
      }
      break;
    case VSD_CONDITION_WAIT_STOP:
      if (vsd->stop() == RETURN_OK) {
        setLedCondition(ToogleGreenLed);
        setValue(CCS_VSD_CONDITION, VSD_CONDITION_STOPPING);
      } else {
        // TODO: Ошибка останова
      }
      break;
    case VSD_CONDITION_RUN:
      if (getValue(CCS_CONDITION) != CCS_CONDITION_RUN) {
        if (vsd->getValue(VSD_FREQUENCY) == vsd->getValue(VSD_FREQUENCY_NOW))
          setValue(CCS_CONDITION, CCS_CONDITION_RUN);
#if DEBUG
        setValue(CCS_CONDITION, CCS_CONDITION_RUN);
#endif
      }
      break;
    case VSD_CONDITION_RUNNING:
      if (vsd->checkStart()) {
        setLedCondition(ToogleGreenLed);
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
  int condition = getValue(CCS_CONDITION);
  int flag = getValue(CCS_CONDITION_FLAG);
  if ((condition != conditionOld_) || (flag != flagOld_)) {
    conditionOld_ = condition;
    flagOld_ = flag;
    switch (condition) {
    case CCS_CONDITION_RUNNING:
      resetRestart();
      if (flag == CCS_CONDITION_FLAG_DELAY)
        setLedCondition(ToogleGreenToogleYellowLed);
      else
        setLedCondition(ToogleYellowLed);
      break;
    case CCS_CONDITION_RUN:
      if (flag == CCS_CONDITION_FLAG_DELAY)
        setLedCondition(OnGreenToogleYellowLed);
      else
        setLedCondition(OnGreenLed);
      break;
    case CCS_CONDITION_STOPPING:
      if (flag == CCS_CONDITION_FLAG_BLOCK)
        setLedCondition(ToogleGreenToogleRedLed);
      break;
    default:
      if (flag == CCS_CONDITION_FLAG_BLOCK)
        setLedCondition(ToogleRedLed);
      else if (flag == CCS_CONDITION_FLAG_RESTART)
        setLedCondition(OnRedOnYellowLed);
      else
        setLedCondition(OnRedLed);
      break;
    }
  }
}

void Ccs::calcTime()
{
  static int conditionOld = CCS_CONDITION_STOP;
  static uint32_t timer = HAL_GetTick();

  int condition = getValue(CCS_CONDITION);

  if ((HAL_GetTick() - timer) >= 100) {
    timer = HAL_GetTick();
    setValue(CCS_DATE_TIME, (uint32_t)rtcGetTime());
  }

  if (conditionOld != condition) {
    if ((condition != CCS_CONDITION_STOP) && (conditionOld == CCS_CONDITION_STOP))
      setValue(CCS_RUN_BEGIN_TIME, getTime());
    if ((condition == CCS_CONDITION_STOP) && (conditionOld != CCS_CONDITION_STOP))
      setValue(CCS_STOP_BEGIN_TIME, getTime());

    conditionOld = condition;
  }

  setValue(CCS_RUN_TIME, getSecFromCurTime(CCS_RUN_BEGIN_TIME));
  setValue(CCS_STOP_TIME, getSecFromCurTime(CCS_STOP_BEGIN_TIME));
}

void Ccs::start(EventType type)
{
  setValue(CCS_LAST_EVENT_TYPE, type);
  setValue(CCS_CMD_START, 1);
  checkCmd();
}

void Ccs::stop(EventType type)
{
  setValue(CCS_LAST_EVENT_TYPE, type);
  setValue(CCS_CMD_STOP, 1);
  checkCmd();
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
#if DEBUG
  return true;
#endif

  if (getValue(CCS_VSD_CONDITION) != VSD_CONDITION_STOP)
    return false;
  if (getValue(CCS_WORKING_MODE) == CCS_WORKING_MODE_STOP)
    return false;
  if (isBlock())
    return false;
  return true;
}

bool Ccs::checkCanStop()
{
#if DEBUG
  return true;
#endif

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

bool Ccs::isDelay()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_DELAY) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isRestart()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_RESTART) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isBlock()
{
  unsigned int flag = getValue(CCS_CONDITION_FLAG);
  if (flag == CCS_CONDITION_FLAG_BLOCK) {
    return true;
  }
  else {
    return false;
  }
}

void Ccs::setDelay()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_DELAY)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_DELAY);
}

void Ccs::setRestart()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_RESTART)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_RESTART);
}

void Ccs::setBlock()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_BLOCK)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_BLOCK);
}

void Ccs::resetDelay()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_DELAY)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetRestart()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_RESTART)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetBlock()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_BLOCK)
    setValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

bool Ccs::isPrevent()
{
  return getValue(CCS_PROT_PREVENT);
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

bool Ccs::isProgramMode()
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

uint32_t Ccs::getTime()
{
  return getValueUint32(CCS_DATE_TIME);
}

uint32_t Ccs::getSecFromCurTime(uint32_t time)
{
  int32_t sec = getTime() - time;
  if (sec > 0)
    return sec;
  else
    return 0;
}

uint32_t Ccs::getSecFromCurTime(enID timeId)
{
  int32_t sec = getTime() - getValueUint32(timeId);
  if (sec > 0)
    return sec;
  else
    return 0;
}

void Ccs::initParameters()
{
  for (int i = 0; i < (CCS_END - CCS_BEGIN); i++) {
    setFieldID(i, i + CCS_BEGIN);
    setFieldAccess(i, ACCESS_OPERATOR);
    setFieldOperation(i, OPERATION_WRITE);
    setFieldValidity(i, VALIDITY_GOOD);
    setFieldValue(i, 0.0);
  }

  // Пустой элемент массива
  parameters_[CCS_BEGIN - CCS_BEGIN].id                = CCS_BEGIN;
  parameters_[CCS_BEGIN - CCS_BEGIN].access            = ACCESS_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].operation         = OPERATION_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].physic            = PHYSIC_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].validity          = VALIDITY_GOOD;
  parameters_[CCS_BEGIN - CCS_BEGIN].value.float_t         = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].min           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].max           = 0.0;
  parameters_[CCS_BEGIN - CCS_BEGIN].def           = 0.0;

  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].id          = CCS_PASSWORD_LEVEL;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].validity    = VALIDITY_ERROR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].value.float_t   = ACCESS_OPERATOR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].min     = ACCESS_ERROR;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].max     = ACCESS_LAST;
  parameters_[CCS_PASSWORD_LEVEL - CCS_BEGIN].def     = ACCESS_OPERATOR;

  parameters_[CCS_CONDITION - CCS_BEGIN].id          = CCS_CONDITION;
  parameters_[CCS_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CONDITION - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CONDITION - CCS_BEGIN].value.float_t   = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].min     = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].max     = CCS_CONDITION_RUN;
  parameters_[CCS_CONDITION - CCS_BEGIN].def     = CCS_CONDITION_STOP;

  parameters_[CCS_WORKING_MODE - CCS_BEGIN].id          = CCS_WORKING_MODE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].value.float_t   = CCS_WORKING_MODE_STOP;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].min     = CCS_WORKING_MODE_STOP;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].max     = CCS_WORKING_MODE_PROGRAM;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].def     = CCS_WORKING_MODE_STOP;

  parameters_[CCS_CMD_STOP - CCS_BEGIN].id          = CCS_CMD_STOP;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].value.float_t   = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].min     = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].max     = 1;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].def     = 0;

  parameters_[CCS_CMD_START - CCS_BEGIN].id          = CCS_CMD_START;
  parameters_[CCS_CMD_START - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_START - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_START - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_START - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_CMD_START - CCS_BEGIN].value.float_t   = 0;
  parameters_[CCS_CMD_START - CCS_BEGIN].min     = 0;
  parameters_[CCS_CMD_START - CCS_BEGIN].max     = 1;
  parameters_[CCS_CMD_START - CCS_BEGIN].def     = 0;

  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].id          = CCS_VSD_CONDITION;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].validity    = VALIDITY_GOOD;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].value.float_t   = VSD_CONDITION_STOP;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].min     = VSD_CONDITION_STOP;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].max     = VSD_CONDITION_WAIT_RUN;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].def     = VSD_CONDITION_STOP;

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 150.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 120.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT, 120.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_STATE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_STATE, 3.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_STATE, 3.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_STATE, 3.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_TIME, 0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_TIME, 0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT, 3599940.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT, 86400.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT, 86400.0);

  setFieldPhysic(CCS_CONDITION_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_CONDITION_FLAG, 0.0);
  setFieldMax(CCS_CONDITION_FLAG, 4.0);
  setFieldDef(CCS_CONDITION_FLAG, 0.0);
  setFieldValue(CCS_CONDITION_FLAG, 0.0);

  setFieldPhysic(CCS_LAST_EVENT_TYPE, PHYSIC_NUMERIC);
  setFieldMin(CCS_LAST_EVENT_TYPE, NoneType);
  setFieldMax(CCS_LAST_EVENT_TYPE, LatchType);
  setFieldDef(CCS_LAST_EVENT_TYPE, AutoType);
  setFieldValue(CCS_LAST_EVENT_TYPE, (float)AutoType);

  setFieldPhysic(CCS_PROT_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_PREVENT, 0.0);
  setFieldMax(CCS_PROT_PREVENT, 1.0);
  setFieldDef(CCS_PROT_PREVENT, 0.0);
  setFieldValue(CCS_PROT_PREVENT, 0.0);

  setFieldPhysic(CCS_TIMER_DIFFERENT_START, PHYSIC_TIME);
  setFieldMin(CCS_TIMER_DIFFERENT_START, 0.0);
  setFieldMax(CCS_TIMER_DIFFERENT_START, 3600.0);
  setFieldDef(CCS_TIMER_DIFFERENT_START, 60.0);
  setFieldValue(CCS_TIMER_DIFFERENT_START, 60.0);
}

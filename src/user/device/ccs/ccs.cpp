/*
 * classController.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "ccs.h"
#include "gpio.h"
#include "user_main.h"
#include "protection_main.h"
#include "regime_main.h"

Ccs::Ccs()
  : Device(CCS_BEGIN, parametersArray_, CCS_END - CCS_BEGIN)
  , conditionOld_(-1)
  , flagOld_(-1)
  , workModeOld_(-1)
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
    changedWorkMode();
    changedCondition();

    calcTime();
    calcParameters();
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
        setNewValue(CCS_CONDITION, CCS_CONDITION_STOP);
      break;
    case VSD_CONDITION_STOPPING:
      if (vsd->checkStop()) {
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOP);
      }
      break;
    case VSD_CONDITION_WAIT_STOP:
      if (vsd->stop() == RETURN_OK) {
        setLedCondition(ToogleGreenLed);
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOPPING);
      } else {
        // TODO: Ошибка останова
      }
      break;
    case VSD_CONDITION_RUN:
      if (getValue(CCS_CONDITION) != CCS_CONDITION_RUN) {
        if (vsd->getValue(VSD_FREQUENCY) == vsd->getValue(VSD_FREQUENCY_NOW))
          setNewValue(CCS_CONDITION, CCS_CONDITION_RUN);
#if DEBUG
        setNewValue(CCS_CONDITION, CCS_CONDITION_RUN);
#endif
      }
      break;
    case VSD_CONDITION_RUNNING:
      if (vsd->checkStart()) {
        setLedCondition(ToogleGreenLed);
        // Запуск сохранения пускового архива
        logRunning.start();
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_RUN);
      }
      break;
    case VSD_CONDITION_WAIT_RUN:
      if (vsd->start() == RETURN_OK) {
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_RUNNING);
      } else {
        // TODO: Ошибка запуска
      }
      break;
    }
  }
}

void Ccs::changedCondition()
{
  int condition = getValue(CCS_CONDITION);
  int flag = getValue(CCS_CONDITION_FLAG);
  if ((condition != conditionOld_) || (flag != flagOld_)) {

    switch (condition) {
    case CCS_CONDITION_RUNNING:
      resetRestart();

      if (flag == CCS_CONDITION_FLAG_DELAY) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionDelay);
        setLedCondition(ToogleGreenToogleYellowLed);
      }
      else {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionRunning);
        setLedCondition(ToogleYellowLed);
      }
      break;
    case CCS_CONDITION_RUN:
      if (flag == CCS_CONDITION_FLAG_DELAY) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionDelay);
        setLedCondition(OnGreenToogleYellowLed);
      }
      else {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionRun);
        setLedCondition(OnGreenLed);
      }
      break;
    case CCS_CONDITION_STOPPING:
      if (flag == CCS_CONDITION_FLAG_BLOCK) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionBlock);
        setLedCondition(ToogleGreenToogleRedLed);
      }
      else {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionStopping);
      }
      break;
    default:
      if (condition != conditionOld_) {
        setNewValue(CCS_LAST_STOP_REASON, getValue(CCS_LAST_STOP_REASON_TMP));
      }

      if (flag == CCS_CONDITION_FLAG_BLOCK) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionBlock);
        setLedCondition(ToogleRedLed);
      }
      else if (flag == CCS_CONDITION_FLAG_RESTART) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionRestart);
        setLedCondition(OnRedOnYellowLed);
      }
      else {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionStop);
        setLedCondition(OnRedLed);
      }
      break;
    }

    conditionOld_ = condition;
    flagOld_ = flag;
  }
}

void Ccs::changedWorkMode()
{
  int workMode = getValue(CCS_WORKING_MODE);
  if (workMode != workModeOld_) {
    workModeOld_ = workMode;
    switch (workMode) {
    case CCS_WORKING_MODE_MANUAL:
      resetRestart();
      break;
    case CCS_WORKING_MODE_AUTO:

      break;
    case CCS_WORKING_MODE_PROGRAM:

      break;
    default:
      resetRestart();
      break;
    }
  }
}

void Ccs::start(LastReasonRun reason)
{
  setNewValue(CCS_LAST_RUN_REASON_TMP, reason);

  if (checkCanStart()) {
    setNewValue(CCS_LAST_RUN_REASON, reason);
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    setNewValue(CCS_CONDITION, CCS_CONDITION_RUNNING);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
  }
}

void Ccs::stop(LastReasonStop reason)
{
  setNewValue(CCS_LAST_STOP_REASON_TMP, reason);

  if (checkCanStop()) {
    if (reason == LastReasonStopRemote)
      setBlock();

    setNewValue(CCS_CONDITION, CCS_CONDITION_STOPPING);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
  }
}

void Ccs::checkCmd()
{
  int start = getValue(CCS_CMD_START);
  int stop = getValue(CCS_CMD_STOP);

  if (start) {
    switch (start) {
    case CmdStartRemote:
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunRemote);
      break;
    default:
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunOperator);
      break;
    }

    setNewValue(CCS_CMD_START, 0);
    if (checkCanStart()) {
      setNewValue(CCS_LAST_RUN_REASON, getValue(CCS_LAST_RUN_REASON_TMP));
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
      setNewValue(CCS_CONDITION, CCS_CONDITION_RUNNING);
      setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
    }
  } else if (stop) {
    switch (stop) {
    case CmdStopRemote:
      setNewValue(CCS_LAST_STOP_REASON_TMP, LastReasonStopRemote);
      break;
    default:
      setNewValue(CCS_LAST_STOP_REASON_TMP, LastReasonStopOperator);
      break;
    }

    setNewValue(CCS_CMD_STOP, 0);
    if (checkCanStop()) {
      setBlock();
      setNewValue(CCS_CONDITION, CCS_CONDITION_STOPPING);
      setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
    }
  }
}

bool Ccs::checkCanStart()
{
  if (getValue(CCS_VSD_CONDITION) != VSD_CONDITION_STOP)
    return false;

  if (isBlock())
    return false;

  if (isPrevent()) {
    if (getValue(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT)) {
      if (parameters.get(TMS_PRESSURE_INTAKE) < getValue(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT)) {
        addEventProtectionPrevent();
        return false;
      }
    }
    else {
      addEventProtectionPrevent();
      return false;
    }
  }

  if (!interceptionStartRegime())
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
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_DELAY);
}

void Ccs::setRestart()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_RESTART)
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_RESTART);
}

void Ccs::setBlock()
{
  if (getValue(CCS_CONDITION_FLAG) < CCS_CONDITION_FLAG_BLOCK)
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_BLOCK);
}

void Ccs::resetDelay()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_DELAY)
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetRestart()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_RESTART)
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
}

void Ccs::resetBlock()
{
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_BLOCK)
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
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

void Ccs::calcTime()
{
  static int conditionOld = CCS_CONDITION_STOP;
  static uint32_t timer = HAL_GetTick();

  int condition = getValue(CCS_CONDITION);

  if ((HAL_GetTick() - timer) >= 100) {
    timer = HAL_GetTick();
    setNewValue(CCS_DATE_TIME, (uint32_t)rtcGetTime());
  }

  if (conditionOld != condition) {
    if ((condition != CCS_CONDITION_STOP) && (conditionOld == CCS_CONDITION_STOP))
      setNewValue(CCS_LAST_RUN_DATE_TIME, getTime());
    if ((condition == CCS_CONDITION_STOP) && (conditionOld != CCS_CONDITION_STOP))
      setNewValue(CCS_LAST_STOP_DATE_TIME, getTime());

    conditionOld = condition;
  }

  setNewValue(CCS_RUN_TIME, getSecFromCurTime(CCS_LAST_RUN_DATE_TIME));
  setNewValue(CCS_STOP_TIME, getSecFromCurTime(CCS_LAST_STOP_DATE_TIME));
}

void Ccs::calcParameters()
{
  calcVoltageImbalanceIn();
}

void Ccs::calcCoefTransformation()
{
  float transVoltageTapOff = parameters.get(CCS_TRANS_VOLTAGE_TAP_OFF);
  float transNominalVoltage = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE);
  float coefTransformation;
  if (transNominalVoltage) {
    coefTransformation = transVoltageTapOff / transNominalVoltage;
  }
  else {
    coefTransformation = transVoltageTapOff / 380;
  }
  setNewValue(CCS_COEF_TRANSFORMATION, coefTransformation);
}

void Ccs::calcVoltageImbalanceIn()
{
  float imbalance = calcImbalance(parameters.get(EM_VOLTAGE_PHASE_1),
                                  parameters.get(EM_VOLTAGE_PHASE_2),
                                  parameters.get(EM_VOLTAGE_PHASE_3),
                                  0);
  setNewValue(CCS_VOLTAGE_IMBALANCE_IN, imbalance);
}

uint8_t Ccs::setNewValue(uint16_t id, float value)
{
  switch (id) {
  case CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT:
    parameters.set(VSD_M_IRMS, value);
    break;
  case CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY:
    parameters.set(VSD_T_BLANK, value);
    break;
  case CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY:
    parameters.set(VSD_M_TRMS, value);
    break;
  case CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT:
    parameters.set(VSD_M_I_FAST, value);
    break;
  case CCS_RGM_CHANGE_FREQ_PERIOD:
  case CCS_RGM_CHANGE_FREQ_BEGIN_FREQ:
  case CCS_RGM_CHANGE_FREQ_END_FREQ:
    calcRegimeChangeFreqPeriodOneStep();
    break;
  default:
    break;
  }
  return setValue(id, value);
}

uint8_t Ccs::setNewValue(uint16_t id, uint32_t value)
{
  return setValue(id, value);
}

uint8_t Ccs::setNewValue(uint16_t id, int value)
{
  return setNewValue(id, (float)value);
}

void Ccs::calcMotorCurrentImbalance()
{
  float imbalance = calcImbalance(parameters.get(CCS_MOTOR_CURRENT_PHASE_1),
                                  parameters.get(CCS_MOTOR_CURRENT_PHASE_2),
                                  parameters.get(CCS_MOTOR_CURRENT_PHASE_3),
                                  1);
  setNewValue(CCS_MOTOR_CURRENT_IMBALANCE, imbalance);
}

void Ccs::calcMotorCurrentPhase(uint16_t vsdOutCurrent, uint16_t coefCorrect, uint16_t motorCurrent)
{
  float current;
  float vsdCurrent = parameters.get(vsdOutCurrent);
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  float coefCor = parameters.get(coefCorrect);
  if (coefTrans) {
    current = vsdCurrent / coefTrans;
  }
  else {
    current = vsdCurrent;
  }
  current = current * coefCor;
  setNewValue(motorCurrent, current);
}

void Ccs::calcMotorCurrentPhase1()
{
  calcMotorCurrentPhase(VSD_CURRENT_OUT_PHASE_1,
                        CCS_COEF_OUT_CURRENT_1,
                        CCS_MOTOR_CURRENT_PHASE_1);
}

void Ccs::calcMotorCurrentPhase2()
{
  calcMotorCurrentPhase(VSD_CURRENT_OUT_PHASE_2,
                        CCS_COEF_OUT_CURRENT_2,
                        CCS_MOTOR_CURRENT_PHASE_2);
}

void Ccs::calcMotorCurrentPhase3()
{
  calcMotorCurrentPhase(VSD_CURRENT_OUT_PHASE_3,
                        CCS_COEF_OUT_CURRENT_3,
                        CCS_MOTOR_CURRENT_PHASE_3);
}

void Ccs::calcMotorCurrentAvarage()
{
  float motorCurrent = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  motorCurrent = motorCurrent + parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  motorCurrent = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  motorCurrent = motorCurrent / 3;
  setNewValue(CCS_MOTOR_CURRENT_AVARAGE, motorCurrent);
}

void Ccs::calcRegimeChangeFreqPeriodOneStep()
{
  float beginFreq = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ);
  float endFreq = parameters.get(CCS_RGM_CHANGE_FREQ_END_FREQ);
  float period = parameters.get(CCS_RGM_CHANGE_FREQ_PERIOD);
  float periodOneStep = 0;
  if ((endFreq - beginFreq) != 0)
    periodOneStep = period / (fabs(endFreq - beginFreq) * 1);
  if (periodOneStep < 5)
    periodOneStep = 5;
  setValue(CCS_RGM_CHANGE_FREQ_PERIOD_ONE_STEP, periodOneStep);
}

void Ccs::initParameters()
{
  for (int i = 0; i < (CCS_END - CCS_BEGIN); i++) {
    setFieldID(i, i + CCS_BEGIN);
    setFieldAccess(i, ACCESS_OPERATOR);
    setFieldOperation(i, OPERATION_WRITE);
    setFieldValidity(i, VALIDITY_OK);
    setFieldValue(i, 0.0);
  }

  // Пустой элемент массива
  parameters_[CCS_BEGIN - CCS_BEGIN].id                = CCS_BEGIN;
  parameters_[CCS_BEGIN - CCS_BEGIN].access            = ACCESS_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].operation         = OPERATION_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].physic            = PHYSIC_ERROR;
  parameters_[CCS_BEGIN - CCS_BEGIN].validity          = VALIDITY_OK;
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
  parameters_[CCS_CONDITION - CCS_BEGIN].validity    = VALIDITY_OK;
  parameters_[CCS_CONDITION - CCS_BEGIN].value.float_t   = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].min     = CCS_CONDITION_STOP;
  parameters_[CCS_CONDITION - CCS_BEGIN].max     = CCS_CONDITION_RUN;
  parameters_[CCS_CONDITION - CCS_BEGIN].def     = CCS_CONDITION_STOP;

  parameters_[CCS_WORKING_MODE - CCS_BEGIN].id          = CCS_WORKING_MODE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].validity    = VALIDITY_OK;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].value.float_t   = CCS_WORKING_MODE_MANUAL;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].min     = CCS_WORKING_MODE_MANUAL;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].max     = CCS_WORKING_MODE_PROGRAM;
  parameters_[CCS_WORKING_MODE - CCS_BEGIN].def     = CCS_WORKING_MODE_MANUAL;

  parameters_[CCS_CMD_STOP - CCS_BEGIN].id          = CCS_CMD_STOP;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].validity    = VALIDITY_OK;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].value.float_t   = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].min     = 0;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].max     = 1;
  parameters_[CCS_CMD_STOP - CCS_BEGIN].def     = 0;

  parameters_[CCS_CMD_START - CCS_BEGIN].id          = CCS_CMD_START;
  parameters_[CCS_CMD_START - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_CMD_START - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_CMD_START - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_CMD_START - CCS_BEGIN].validity    = VALIDITY_OK;
  parameters_[CCS_CMD_START - CCS_BEGIN].value.float_t   = 0;
  parameters_[CCS_CMD_START - CCS_BEGIN].min     = CmdStartNone;
  parameters_[CCS_CMD_START - CCS_BEGIN].max     = CmdStartRemote;
  parameters_[CCS_CMD_START - CCS_BEGIN].def     = CmdStartNone;

  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].id          = CCS_VSD_CONDITION;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].access      = ACCESS_OPERATOR;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].operation   = OPERATION_WRITE;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].physic      = PHYSIC_NUMERIC;
  parameters_[CCS_VSD_CONDITION - CCS_BEGIN].validity    = VALIDITY_OK;
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

  setFieldPhysic(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME, 86400.0);

  //-

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE, 3.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE, 3.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE, 3.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME, 0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME, 0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME, 86400.0);

  //-

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT, 20.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT, 20.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE, 3.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE, 3.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE, 3.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME, 0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME, 0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME, 86400.0);

  //-

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_MODE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_PREVENT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_PREVENT, 1.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_PREVENT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_PARAMETER, 0.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_PARAMETER, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_STATE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_STATE, 3.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_STATE, 3.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_STATE, 3.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_TIME, 0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME, 86400.0);

  //-

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_MODE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_PREVENT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_PREVENT, 1.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_PREVENT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_PARAMETER, 0.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_PARAMETER, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_STATE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_STATE, 3.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_STATE, 3.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_STATE, 3.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_TIME, 0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME, 86400.0);

  // Защита "Дисбаланс токов двигателя"
  // Защита "Дисбаланс токов двигателя" - уставка "Режим реакции защиты"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE, Protection::ModeOff);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE, (float)Protection::ModeBlock);
  // Защита "Дисбаланс токов двигателя" - уставка "Задержка активации защиты"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY, 60.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY, 15.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY, 15.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Задержка срабатывания защиты"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY, 60.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY, 20.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY, 20.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Задержка АПВ защиты"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY, 18000.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY, 3600.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Количество разрешенных АПВ"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT, 9999.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT, 5.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT, 0.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Период сброса счётчиков АПВ"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, 599940.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, 86400.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, 86400.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Граница срабатывания защиты"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT, 15.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT, 15.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Граница разрешения АПВ"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT, 0.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT, 0.0);
  // Защита "Дисбаланс токов двигателя" - уставка "Флаг работы с ВРП"
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER, 0.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER, 0.0);
  // Защита "Дисбаланс токов двигателя" - Флаг что защита в АПВ
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG, 0.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG, 0.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG, 0.0);
  // Защита "Дисбаланс токов двигателя" - состояние автомата защиты
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE, 3.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE, 3.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE, 3.0);
  // Защита "Дисбаланс токов двигателя" - значение внутреннего таймера
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME, 0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME, (uint32_t)0);
  // Защита "Дисбаланс токов двигателя" - значение счётчиков АПВ
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT, 9999.0);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT, 0.0);
  // Защита "Дисбаланс токов двигателя" - время первого АПВ
  setFieldPhysic(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME, 0.0);
  setFieldMax(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME, (uint32_t)0);

  //-

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT, 1.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER, 0.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_STATE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_STATE, 3.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_STATE, 3.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_STATE, 3.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_TIME, 0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME, 86400.0);

  //-

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_MODE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_PREVENT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_PREVENT, 1.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_PREVENT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_PARAMETER, 1.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_PARAMETER, 1.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_STATE, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_STATE, 3.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_STATE, 3.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_STATE, 3.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_TIME, 0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_TIME, 0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME, 86400.0); 
  // Защита "Температура двигателя"
  // Защита "Температура двигателя" - уставка "Режим реакции защиты"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE, Protection::ModeOff);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE, (float)Protection::ModeBlock);
  // Защита "Температура двигателя" - флаг запрещающего параметра
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT, 1.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT, 0.0);
  // Защита "Температура двигателя" - уставка "Задержка активации защиты"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, 60.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, 0.0);
  // Защита "Температура двигателя" - уставка "Задержка срабатывания защиты"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY, 60.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY, 5.0);
  // Защита "Температура двигателя" - уставка "Задержка АПВ защиты"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY, 18000.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY, 3600.0);
  // Защита "Температура двигателя" - уставка "Количество разрешенных АПВ"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT, 9999.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT, 5.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT, 5.0);
  // Защита "Температура двигателя" - уставка "Период сброса счётчиков АПВ"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, 599940.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, 86400.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, 86400.0);
  // Защита "Температура двигателя" - уставка "Граница срабатывания защиты"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT, PHYSIC_TEMPERATURE);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT, 150.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT, 115.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT, 115.0);
  // Защита "Температура двигателя" - уставка "Граница разрешения АПВ"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT, PHYSIC_TEMPERATURE);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT, 150.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT, 115.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT, 115.0);
  // Защита "Температура двигателя" - уставка "Флаг работы с ВРП"
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER, 0.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER, 0.0);
  // Защита "Температура двигателя" - Флаг что защита в АПВ
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG, 0.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG, 0.0);
  // Защита "Температура двигателя" - состояние автомата защиты
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE, 3.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE, 3.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE, 3.0);
  // Защита "Температура двигателя" - значение внутреннего таймера
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME, 0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME, 0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME, (uint32_t)0);
  // Защита "Температура двигателя" - значение счётчиков АПВ
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT, 9999.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT, 0.0);
  // Защита "Температура двигателя" - время первого АПВ
  setFieldPhysic(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME, 0.0);
  setFieldMax(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME, 4294967296.0);
  setFieldDef(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME, 0.0);
  setFieldValue(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME, (uint32_t)0);

  //-

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_MODE, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_MODE, Protection::ModeRestart);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_MODE, Protection::ModeBlock);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_MODE, (float)Protection::ModeBlock);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT, 1.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT, 0.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT, 0.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, 59999.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, 0.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY, 600.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY, 5.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY, 5.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY, 60.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY, 86400.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY, 3600.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY, 3600.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT, 99.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT, 0.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, 60.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, 86400.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, 3600.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, 3600.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT, 80.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT, 80.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT, 100.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT, 100.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT, 100.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER, 1.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER, 1.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG, 1.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG, 1.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG, 1.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_STATE, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_STATE, 3.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_STATE, 3.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_STATE, 3.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_TIME, 0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_TIME, 0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_TIME, (uint32_t)0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT, 99.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT, 0.0);

  setFieldPhysic(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME, 60.0);
  setFieldMax(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME, 3599940.0);
  setFieldDef(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME, 86400.0);
  setFieldValue(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME, 86400.0);

  /*
  // Защита "Температура двигателя"
  // Защита "Температура двигателя" - уставка "Режим реакции защиты"
  // Защита "Температура двигателя" - флаг запрещающего параметра
  // Защита "Температура двигателя" - уставка "Задержка активации защиты"
  // Защита "Температура двигателя" - уставка "Задержка срабатывания защиты"
  // Защита "Температура двигателя" - уставка "Задержка АПВ защиты"
  // Защита "Температура двигателя" - уставка "Количество разрешенных АПВ"
  // Защита "Температура двигателя" - уставка "Период сброса счётчиков АПВ"
  // Защита "Температура двигателя" - уставка "Граница срабатывания защиты"
  // Защита "Температура двигателя" - уставка "Граница разрешения АПВ"
  // Защита "Температура двигателя" - уставка "Флаг работы с ВРП"
  // Защита "Температура двигателя" - Флаг что защита в АПВ
  // Защита "Температура двигателя" - состояние автомата защиты
  // Защита "Температура двигателя" - значение внутреннего таймера
  // Защита "Температура двигателя" - значение счётчиков АПВ
  // Защита "Температура двигателя" - время первого АПВ
  */

  // Защита "Сопротивление изоляции"
  // Защита "Сопротивление изоляции" - уставка "Режим реакции защиты"
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_MODE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_MODE, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_MODE, 1.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_MODE, 1.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_MODE, 1.0);
  // Защита "Сопротивление изоляции" - флаг запрещающего параметра
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_PREVENT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_PREVENT, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_PREVENT, 1.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_PREVENT, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_PREVENT, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Задержка активации защиты" всегда 0
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Задержка срабатывания защиты" всегда 0
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Задержка АПВ защиты" всегда 0
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Количество разрешенных АПВ" всегда 0
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Период сброса счётчиков АПВ" всегда 0
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_RESET, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_RESET, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_RESET, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_RESET, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_RESET, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Граница срабатывания защиты"
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT, PHYSIC_RESISTANCE);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT, 30.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT, 500.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT, 30.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT, 30.0);
  // Защита "Сопротивление изоляции" - уставка "Граница разрешения АПВ"
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT, PHYSIC_PERCENT);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT, 0.0);
  // Защита "Сопротивление изоляции" - уставка "Флаг работы в режиме пониженной изоляции"
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_PARAMETER, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_PARAMETER, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_PARAMETER, 1.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_PARAMETER, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_PARAMETER, 0.0);
  // Защита "Сопротивление изоляции" - Флаг что защита в АПВ
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG, 0.0);
  // Защита "Сопротивление изоляции" - состояние автомата защиты
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_STATE, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_STATE, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_STATE, 3.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_STATE, 3.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_STATE, 3.0);
  // Защита "Сопротивление изоляции" - значение внутреннего таймера
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_TIME, 0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_TIME, 0xFFFFFFFF);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_TIME, 0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_TIME, (uint32_t)0);
  // Защита "Сопротивление изоляции" - значение счётчиков АПВ
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT, PHYSIC_NUMERIC);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT, 0.0);
  // Защита "Сопротивление изоляции" - время первого АПВ
  setFieldPhysic(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME, PHYSIC_TIME);
  setFieldMin(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME, 0.0);
  setFieldMax(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME, 0.0);
  setFieldDef(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME, 0.0);
  setFieldValue(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME, 0.0);

  //-

  setFieldPhysic(CCS_CONDITION_FLAG, PHYSIC_NUMERIC);
  setFieldMin(CCS_CONDITION_FLAG, 0.0);
  setFieldMax(CCS_CONDITION_FLAG, 4.0);
  setFieldDef(CCS_CONDITION_FLAG, 0.0);
  setFieldValue(CCS_CONDITION_FLAG, 0.0);

  setFieldPhysic(CCS_LAST_RUN_REASON_TMP, PHYSIC_NUMERIC);
  setFieldMin(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
  setFieldMax(CCS_LAST_RUN_REASON_TMP, LastReasonRunMaxAnalog4);
  setFieldDef(CCS_LAST_RUN_REASON_TMP, LastReasonRunOperator);
  setFieldValue(CCS_LAST_RUN_REASON_TMP, (float)LastReasonRunOperator);

  setFieldPhysic(CCS_LAST_STOP_REASON_TMP, PHYSIC_NUMERIC);
  setFieldMin(CCS_LAST_STOP_REASON_TMP, LastReasonStopNone);
  setFieldMax(CCS_LAST_STOP_REASON_TMP, LastReasonStopMaxAnalog4);
  setFieldDef(CCS_LAST_STOP_REASON_TMP, LastReasonStopOperator);
  setFieldValue(CCS_LAST_STOP_REASON_TMP, (float)LastReasonStopOperator);

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

  setFieldPhysic(CCS_COEF_OUT_CURRENT_1, PHYSIC_NUMERIC);
  setFieldMin(CCS_COEF_OUT_CURRENT_1, 0.001);
  setFieldMax(CCS_COEF_OUT_CURRENT_1, 10.0);
  setFieldDef(CCS_COEF_OUT_CURRENT_1, 1.0);
  setFieldValue(CCS_COEF_OUT_CURRENT_1, 1.0);

  setFieldPhysic(CCS_COEF_OUT_CURRENT_2, PHYSIC_NUMERIC);
  setFieldMin(CCS_COEF_OUT_CURRENT_2, 0.001);
  setFieldMax(CCS_COEF_OUT_CURRENT_2, 10.0);
  setFieldDef(CCS_COEF_OUT_CURRENT_2, 1.0);
  setFieldValue(CCS_COEF_OUT_CURRENT_2, 1.0);

  setFieldPhysic(CCS_COEF_OUT_CURRENT_3, PHYSIC_NUMERIC);
  setFieldMin(CCS_COEF_OUT_CURRENT_3, 0.001);
  setFieldMax(CCS_COEF_OUT_CURRENT_3, 10.0);
  setFieldDef(CCS_COEF_OUT_CURRENT_3, 1.0);
  setFieldValue(CCS_COEF_OUT_CURRENT_3, 1.0);

  setFieldPhysic(CCS_COEF_TRANSFORMATION, PHYSIC_NUMERIC);
  setFieldMin(CCS_COEF_TRANSFORMATION, 0.001);
  setFieldMax(CCS_COEF_TRANSFORMATION, 1000.0);
  setFieldDef(CCS_COEF_TRANSFORMATION, 1.0);
  setFieldValue(CCS_COEF_TRANSFORMATION, 1.0);
}

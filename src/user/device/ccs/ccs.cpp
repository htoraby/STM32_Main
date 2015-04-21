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

#define TIMEOUT_POWER_OFF 6000 //!< 1 минута на отключение питания ИБП

Ccs::Ccs()
  : Device(CCS_BEGIN, parametersArray_, CCS_END - CCS_BEGIN)
  , conditionOld_(-1)
  , flagOld_(-1)
  , workModeOld_(-1)
  , powerOffFlag_(false)
  , powerOffTimeout_(TIMEOUT_POWER_OFF)
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
    controlPower();
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
      case OnRedToogleYellowLed:
        onLed(StopLed);
        break;
      }
    }

    switch (condition) {
    case ToogleGreenLed:
      toggleLed(WorkLed);
      break;
    case OnRedToogleYellowLed:
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
#if USE_DEBUG
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
        setLedCondition(OnRedToogleYellowLed);
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
        setLedCondition(ToogleGreenLed);
      }
      break;
    default:
      if (condition != conditionOld_) {
        float reason = getValue(CCS_LAST_STOP_REASON_TMP);
        setNewValue(CCS_LAST_STOP_REASON, reason);
        logEvent.add(StopCode, (EventType)reason, StopId);
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
#if USE_DEBUG
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
  case CCS_RGM_RUN_PUSH_MODE:
    if (value != 1) {                       // TODO: Заменить константу
      parameters.set(CCS_RGM_RUN_SWING_MODE, 1);      // Отключаем режим раскачки
    }
    break;
  case CCS_RGM_RUN_SWING_MODE:
    if (value != 1) {                       // TODO: Заменить константу
      parameters.set(CCS_RGM_RUN_PUSH_MODE, 1);      // Отключаем режим раскачки
    }
    break;
  case CCS_CMD_LOG_COPY:
    logStartSave();
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

void Ccs::controlPower()
{
  if (!isPowerGood()) {
    if (!powerOffFlag_) {
      // Запись в журнал "Отключение питания"
      logEvent.add(PowerCode, AutoType, PowerOffId);
      powerOffFlag_ = true;
    }

    if (powerOffTimeout_) {
      powerOffTimeout_--;
    }
    else {
      turnPowerBattery(false);
    }
  } else {
    powerOffFlag_ = false;
    powerOffTimeout_ = TIMEOUT_POWER_OFF;
  }
}

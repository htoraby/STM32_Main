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
#include "math.h"

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
    checkConnectDevice();
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
      if (vsd->stop() == ok_r) {
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
      if (vsd->start() == ok_r) {
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
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
      setBlock();
      setNewValue(CCS_CONDITION, CCS_CONDITION_STOPPING);
      setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
    }
  }
}

bool Ccs::checkCanStart()
{
  if (getValue(CCS_VSD_CONDITION) != VSD_CONDITION_STOP) {
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    return false;
  }

  if (isBlock()) {
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    return false;
  }

  if (isPrevent()) {
    if (getValue(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT)) {
      if (parameters.get(TMS_PRESSURE_INTAKE) < getValue(CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT)) {
        setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
        addEventProtectionPrevent();
        return false;
      }
    }
    else {
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
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
    time_t time = rtcGetTime();
    setValue(CCS_DATE_TIME, (uint32_t)time);
    tm dateTime = *localtime(&time);
    setNewValue(CCS_DATE_TIME_SEC, dateTime.tm_sec);
    setNewValue(CCS_DATE_TIME_MIN, dateTime.tm_min);
    setNewValue(CCS_DATE_TIME_HOUR, dateTime.tm_hour);
    setNewValue(CCS_DATE_TIME_DAY, dateTime.tm_mday);
    setNewValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon);
    setNewValue(CCS_DATE_TIME_YEAR, 1900 + dateTime.tm_year);
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
  calcMotorLoad();
  calcMotorCos();
  calcMotorSpeed();
  correctVoltageIn();
  calcVoltageImbalanceIn();
  calcCurrentImbalanceIn();
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

void Ccs::correctVoltageIn()
{
  float phase = parameters.get(EM_VOLTAGE_PHASE_1);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_A);
  setNewValue(CCS_VOLTAGE_PHASE_1, phase);

  phase = parameters.get(EM_VOLTAGE_PHASE_1_2);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_A);
  setNewValue(CCS_VOLTAGE_PHASE_1_2, phase);

  phase = parameters.get(EM_VOLTAGE_PHASE_2);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_B);
  setNewValue(CCS_VOLTAGE_PHASE_2, phase);

  phase = parameters.get(EM_VOLTAGE_PHASE_2_3);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_B);
  setNewValue(CCS_VOLTAGE_PHASE_2_3, phase);

  phase = parameters.get(EM_VOLTAGE_PHASE_3);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_C);
  setNewValue(CCS_VOLTAGE_PHASE_3, phase);

  phase = parameters.get(EM_VOLTAGE_PHASE_3_1);
  phase = phase * parameters.get(CCS_COEF_VOLTAGE_IN_C);
  setNewValue(CCS_VOLTAGE_PHASE_3_1, phase);
}

void Ccs::calcVoltageImbalanceIn()
{
  float imbalance = calcImbalance(parameters.get(CCS_VOLTAGE_PHASE_1),
                                  parameters.get(CCS_VOLTAGE_PHASE_2),
                                  parameters.get(CCS_VOLTAGE_PHASE_3),
                                  0);
  setNewValue(CCS_VOLTAGE_IMBALANCE_IN, imbalance);
}

void Ccs::calcCurrentImbalanceIn()
{
  float imbalance = calcImbalance(parameters.get(EM_CURRENT_PHASE_1),
                                  parameters.get(EM_CURRENT_PHASE_2),
                                  parameters.get(EM_CURRENT_PHASE_3),
                                  1);
  setNewValue(CCS_CURRENT_IMBALANCE_IN, imbalance);
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
    if (value != Regime::OffAction) {
      parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::OffAction); // Отключаем режим раскачки
    }
    break;
  case CCS_RGM_RUN_SWING_MODE:
    if (value != Regime::OffAction) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::OffAction); // Отключаем режим раскачки
    }
    break;
  case CCS_CMD_LOG_COPY:
    logStartSave();
    break;
  case CCS_COEF_TRANSFORMATION:
    calcSystemInduct();
    break;
  case CCS_TRANS_VOLTAGE_SHORT_CIRCUIT:
    calcSystemInduct();
    break;
  case CCS_TRANS_NOMINAL_VOLTAGE:
    calcSystemInduct();
    break;
  case CCS_TRANS_NOMINAL_CURRENT:
    calcSystemInduct();
    break;
  case CCS_TRANS_NOMINAL_FREQUENCY:
    calcSystemInduct();
    break;
  case CCS_MOTOR_INDUCTANCE:
    calcSystemInduct();
    break;
  case CCS_MOTOR_INDUCTANCE_RESIST_PHASE:
    parameters.set(CCS_MOTOR_INDUCTANCE, calcMotorInductFromResistPhase(parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE)));
    calcSystemInduct();
    break;
  case CCS_FILTER_INDUCTANCE:
    calcSystemInduct();
    break;
  case CCS_DATE_TIME_SEC:
    parameters.set(VSD_TIME_SECOND, value);
    break;
  case CCS_DATE_TIME_MIN:
    parameters.set(VSD_TIME_MINUTE, value);
    break;
  case CCS_DATE_TIME_HOUR:
    parameters.set(VSD_TIME_HOUR, value);
    break;
  case CCS_DATE_TIME_DAY:
    parameters.set(VSD_TIME_DAY, value);
    break;
  case CCS_DATE_TIME_MONTH:
    parameters.set(VSD_TIME_MONTH, value);
    break;
  case CCS_DATE_TIME_YEAR:
    parameters.set(VSD_TIME_YEAR, value);
    break;
  default:
    break;
  }
  return setValue(id, value);
}

uint8_t Ccs::setNewValue(uint16_t id, uint32_t value)
{
  switch (id) {
  case CCS_DATE_TIME:
    {
      time_t time = value;
      rtcSetTime(&time);
    }
    break;
  }
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

float Ccs::calcVoltageDropCable(float lenght, float cross, float current)
{
  if (parameters.get(CCS_RGM_HEAT_CABLE_MODE) != Regime::OffAction) {
    lenght = 2 * lenght / 1000;                                         // Длина петли кабеля в кМ
    float R80 = parameters.get(CCS_RGM_HEAT_CABLE_RESISTANCE_80);       // Сопротивление кабеля при 80°С
    current = parameters.get(VSD_MOTOR_CURRENT);                        // Номинальный ток ПЭД
    float dUcl = R80 * lenght * current;
    return dUcl;
  }
  else {
    if (!cross)
      cross = 16;
    float dUcl = 38.7 * (current * lenght) / (cross * 1000);            // Падение напряжения на кабельной линии
    return dUcl;
  }
}

float Ccs::calcVoltageDropFilter(float current, float freq, float inputVoltage, float tapOff, float filter)
{
  if (!freq)
    freq = parameters.get(VSD_MOTOR_FREQUENCY);
  if (!inputVoltage)
    inputVoltage = 380.0;
  float trans = parameters.get(CCS_TRANS_VOLTAGE_TAP_OFF) / inputVoltage;
  float dUf = 2 * NUM_PI * filter * freq * current * trans / 1000.0;
  return dUf;
}

float Ccs::calcMotorResistPhaseFromInduct(float induct)
{
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float resist = 2 * NUM_PI * induct * freq / 1000;
  return resist;
}

void Ccs::calcSystemInduct()
{
  float coefTrans2 = pow(parameters.get(CCS_COEF_TRANSFORMATION), 2);
  float inductTrans = (parameters.get(CCS_TRANS_VOLTAGE_SHORT_CIRCUIT) * parameters.get(CCS_TRANS_NOMINAL_VOLTAGE) * 0.01) /
                      (parameters.get(CCS_TRANS_NOMINAL_CURRENT) * parameters.get(CCS_TRANS_NOMINAL_FREQUENCY) * 2 * NUM_PI);
  float inductCable = ((parameters.get(CCS_TRANS_CABLE_LENGHT) * 1.5) / coefTrans2) / 1000000;
  float inductMotor = (parameters.get(CCS_MOTOR_INDUCTANCE) / coefTrans2) / 1000;
  float inductFilter = parameters.get(CCS_FILTER_INDUCTANCE) / 1000;
  float induct = (inductTrans + inductCable + inductMotor + inductFilter) * 1000;
  parameters.set(CCS_SYSTEM_INDUCTANCE, induct);
  parameters.set(VSD_LOUT, parameters.get(CCS_SYSTEM_INDUCTANCE));
}

float Ccs::calcMotorInductFromResistPhase(float resist)
{
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float induct = resist * 1000 / (2 * NUM_PI * freq);
  return induct;
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

void Ccs::calcMotorLoad()
{
  float mtrCur = parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
  float mtrCos = 1;
  float nomMtrCur = parameters.get(VSD_MOTOR_CURRENT);
  if (nomMtrCur == 0) {
    return;
  }
  float nomMtrCos = parameters.get(VSD_MOTOR_COS_PHI);
  if (nomMtrCos == 0) {
    return;
  }
  float mtrLoad = ((mtrCur * mtrCos) / (nomMtrCur * nomMtrCos)) * 100;
  setNewValue(CCS_MOTOR_LOAD_NOW, mtrLoad);
}

void Ccs::calcMotorCos()
{
  float actPwr = parameters.get(VSD_POWER_ACTIVE);
  float fullPwr = parameters.get(VSD_POWER_FULL);
  float cos = actPwr;
  if (fullPwr != 0) {
    cos = actPwr / fullPwr;
  }
  if (cos < 0)
    cos = cos * (-1);
  if (cos > 1)
    cos = 1;
  setNewValue(CCS_MOTOR_COS_PHI_NOW, cos);
}

void Ccs::calcMotorSpeed()
{
  if (parameters.get(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_ASYNC) {
    parameters.set(CCS_MOTOR_SPEED_NOW, vsd->getCurrentFreq() * 60) ;
  }
  else if (parameters.get(VSD_MOTOR_TYPE) == VSD_MOTOR_TYPE_VENT) {
    parameters.set(CCS_MOTOR_SPEED_NOW, vsd->getCurrentFreq() * 30);
  }
  else {
    parameters.set(CCS_MOTOR_SPEED_NOW, 0.0);
  } 
}

void Ccs::calcRegimeChangeFreqPeriodOneStep()
{
  float beginFreq = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ);
  float endFreq = parameters.get(CCS_RGM_CHANGE_FREQ_END_FREQ);
  float period = parameters.get(CCS_RGM_CHANGE_FREQ_PERIOD);
  float periodOneStep = 0;
  if ((endFreq - beginFreq) != 0)
    periodOneStep = period / (fabs(endFreq - beginFreq)) * 0.1;

  // Проверка на минимум 1 сек
  if (periodOneStep < 1) {
    periodOneStep = 1;
  }
  setValue(CCS_RGM_CHANGE_FREQ_PERIOD_ONE_STEP, periodOneStep);
}

void Ccs::controlPower()
{
  if (!isPowerGood()) {
    if (powerOffTimeout_ == (TIMEOUT_POWER_OFF - 1000)) {
      setNewValue(CCS_CMD_AM335_POWER_OFF, 1.0);
      offLcd();
    }

    if ((powerOffTimeout_ == 1000) || !isUpsGood()) {
      if (!powerOffFlag_) {
        setNewValue(CCS_CMD_AM335_POWER_OFF, 1.0);

        // Запись в журнал "Отключение питания"
        logEvent.add(PowerCode, AutoType, PowerOffId);

        powerOffFlag_ = true;
      }
    }

    if (!powerOffTimeout_) {
      turnPowerBattery(false);
    }
    else {
      powerOffTimeout_--;
    }
  } else {
    setNewValue(CCS_CMD_AM335_POWER_OFF, 0.0);

    if (powerOffTimeout_ <= (TIMEOUT_POWER_OFF - 1000)) {
      onLcd();
      offPowerAm335x();
      osDelay(200);
      onPowerAm335x();
    }

    powerOffFlag_ = false;
    powerOffTimeout_ = TIMEOUT_POWER_OFF;
  }
}

void Ccs::checkConnectDevice()
{
  if (vsd->isConnect())
    setNewValue(CCS_VSD_CONNECTION, 1);
  else
    setNewValue(CCS_VSD_CONNECTION, 0);
  if (tms->isConnect())
    setNewValue(CCS_DHS_CONNECTION, 1);
  else
    setNewValue(CCS_DHS_CONNECTION, 0);
  if (em->isConnect())
    setNewValue(CCS_EM_CONNECTION, 1);
  else
    setNewValue(CCS_EM_CONNECTION, 0);
}

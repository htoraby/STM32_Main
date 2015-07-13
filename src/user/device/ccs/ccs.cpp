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
#define DELAY_CHECK_CONNECT_DEVICE 1000 //!< Задержка проверки подключения устройств - 20 сек

Ccs::Ccs()
  : Device(CCS_BEGIN, parametersArray_, CCS_END - CCS_BEGIN)
  , conditionOld_(-1)
  , flagOld_(-1)
  , workModeOld_(-1)
  , powerOffFlag_(false)
  , powerOffTimeout_(TIMEOUT_POWER_OFF)
  , checkConnectDeviceTimer_(DELAY_CHECK_CONNECT_DEVICE)
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

  rebootSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(rebootSemaphoreId_, 0);
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

    if (osSemaphoreWait(rebootSemaphoreId_, 0) != osEventTimeout)
      reboot();
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
      }
      else {
        char message[SIZE_MSG_DEBUG];
        sprintf(message, "Ошибка останова");
        logDebug.add(WarningMsg, message);
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
        char message[SIZE_MSG_DEBUG];
        sprintf(message, "Ошибка запуска");
        logDebug.add(WarningMsg, message);
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
        calcCountersStop(reason);
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
    calcCountersRun(reason);
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
      float reason = getValue(CCS_LAST_RUN_REASON_TMP);
      setNewValue(CCS_LAST_RUN_REASON, reason);
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
      setNewValue(CCS_CONDITION, CCS_CONDITION_RUNNING);
      setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
      calcCountersRun(reason);
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
    setValue(CCS_DATE_TIME_SEC, dateTime.tm_sec, NoneType);
    setValue(CCS_DATE_TIME_MIN, dateTime.tm_min, NoneType);
    setValue(CCS_DATE_TIME_HOUR, dateTime.tm_hour, NoneType);
    setValue(CCS_DATE_TIME_DAY, dateTime.tm_mday, NoneType);
    setValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon, NoneType);
    setValue(CCS_DATE_TIME_YEAR, 1900 + dateTime.tm_year, NoneType);
    // TODO: Синхронизация времени
  }

  if (conditionOld != condition) {
    if ((condition != CCS_CONDITION_STOP) && (conditionOld == CCS_CONDITION_STOP))
      setNewValue(CCS_LAST_RUN_DATE_TIME, getTime());   
    if ((condition == CCS_CONDITION_STOP) && (conditionOld != CCS_CONDITION_STOP))
      setNewValue(CCS_LAST_STOP_DATE_TIME, getTime());
    conditionOld = condition;
  }
  if (condition == CCS_CONDITION_STOP)
    setNewValue(CCS_STOP_TIME, (float)getSecFromCurTime(CCS_LAST_STOP_DATE_TIME));
  else
    setNewValue(CCS_RUN_TIME, (float)getSecFromCurTime(CCS_LAST_RUN_DATE_TIME));
}

void Ccs::calcParameters()
{
  calcTransCoef();
  calcMotorCurrentPhase1();
  calcMotorCurrentPhase2();
  calcMotorCurrentPhase3();
  calcMotorCurrentAverage();
  calcMotorCurrentImbalance();
  calcMotorVoltagePhase1();
  calcMotorVoltagePhase2();
  calcMotorVoltagePhase3();
  calcMotorVoltageImbalance();
  calcMotorSpeed();
  calcMotorCos();
  calcMotorLoad();
  calcInputVoltagePhase1();
  calcInputVoltagePhase2();
  calcInputVoltagePhase3();
  calcInputVoltagePhase12();
  calcInputVoltagePhase23();
  calcInputVoltagePhase31();
  calcInputVoltageImbalance();
  calcInputCurrentImbalance();
  calcRegimeRun();
}

float Ccs::calcTransCoef()
{
  float voltTapOff = parameters.get(CCS_TRANS_VOLTAGE_TAP_OFF);
  float voltInput =  parameters.get(CCS_TRANS_NOMINAL_VOLTAGE);
  float transCoef;
  if (voltInput == 0)
    transCoef = voltTapOff / 380;
  else
    transCoef = voltTapOff / voltInput;
  setValue(CCS_COEF_TRANSFORMATION, transCoef);
  return parameters.get(CCS_COEF_TRANSFORMATION);
}

float Ccs::calcMotorCurrentPhase(float vsdCurOut)
{
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (coefTrans == 0)
    return vsdCurOut;
  else
    return (vsdCurOut / coefTrans);
}

float Ccs::calcMotorCurrentPhase1()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_1,
           calcMotorCurrentPhase(applyCoef(parameters.get(VSD_CURRENT_OUT_PHASE_1),
                                           parameters.get(CCS_COEF_OUT_CURRENT_1))));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
}

float Ccs::calcMotorCurrentPhase2()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_2,
           calcMotorCurrentPhase(applyCoef(parameters.get(VSD_CURRENT_OUT_PHASE_2),
                                           parameters.get(CCS_COEF_OUT_CURRENT_2))));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
}

float Ccs::calcMotorCurrentPhase3()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_3,
           calcMotorCurrentPhase(applyCoef(parameters.get(VSD_CURRENT_OUT_PHASE_3),
                                           parameters.get(CCS_COEF_OUT_CURRENT_3))));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
}

float Ccs::calcMotorCurrentAverage()
{
  setValue(CCS_MOTOR_CURRENT_AVARAGE,
           calcAverage3Values(parameters.get(CCS_MOTOR_CURRENT_PHASE_1),
                              parameters.get(CCS_MOTOR_CURRENT_PHASE_2),
                              parameters.get(CCS_MOTOR_CURRENT_PHASE_3)));
  return parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
}

float Ccs::calcMotorCurrentImbalance()
{
  setValue(CCS_MOTOR_CURRENT_IMBALANCE,
           calcImbalance(parameters.get(CCS_MOTOR_CURRENT_PHASE_1),
                         parameters.get(CCS_MOTOR_CURRENT_PHASE_2),
                         parameters.get(CCS_MOTOR_CURRENT_PHASE_3),
                         1));
  return parameters.get(CCS_MOTOR_CURRENT_IMBALANCE);
}

float Ccs::calcDropVoltageFilter(float current, float freq, float coefTrans)
{
  float outFilter = parameters.get(CCS_FILTER_OUTPUT);
  float inductFilter = parameters.get(CCS_FILTER_INDUCTANCE);
  if (freq == -1)
    freq = parameters.get(VSD_FREQUENCY_NOW);
  if (coefTrans == -1)
    coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (outFilter == 0)
    return 0;
  if (freq == 0)
    freq = parameters.get(VSD_MOTOR_FREQUENCY);
  if (coefTrans == 0)
    coefTrans = 380.0;
  float dUf = 2 * NUM_PI * inductFilter * freq * current * coefTrans / 1000.0;
  return dUf;
}

float Ccs::calcDropVoltageCable(float current)
{
  float lenght = parameters.get(CCS_TRANS_CABLE_LENGHT);
  float cross =  parameters.get(CCS_TRANS_CABLE_CROSS);
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

float Ccs::calcMotorVoltagePhase(float dropVoltFilter, float dropVoltCable)
{
  float vsdVoltOut = parameters.get(VSD_OUT_VOLTAGE_MOTOR);
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  return (vsdVoltOut * coefTrans - (dropVoltFilter + dropVoltCable) * sqrt(3));
}

float Ccs::calcMotorVoltagePhase1()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_1, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_1);
}

float Ccs::calcMotorVoltagePhase2()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_2, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_2);
}

float Ccs::calcMotorVoltagePhase3()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_3, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_3);
}

float Ccs::calcMotorVoltageImbalance()
{
  setValue(CCS_MOTOR_VOLTAGE_IMBALANCE,
           calcImbalance(parameters.get(CCS_MOTOR_VOLTAGE_PHASE_1),
                         parameters.get(CCS_MOTOR_VOLTAGE_PHASE_2),
                         parameters.get(CCS_MOTOR_VOLTAGE_PHASE_3),
                         0));
  return parameters.get(CCS_MOTOR_VOLTAGE_IMBALANCE);
}

float Ccs::calcMotorSpeed()
{
  float mtrType = parameters.get(VSD_MOTOR_TYPE);
  float freq = parameters.get(VSD_FREQUENCY_NOW);
  float mtrSpeed;
  if (mtrType == VSD_MOTOR_TYPE_ASYNC) {
    mtrSpeed = freq * 60;
  }
  else {
    if (mtrType == VSD_MOTOR_TYPE_VENT) {
      mtrSpeed = freq * 30;
    }
    else {
      mtrSpeed = 0;
    }
  }
  setValue(CCS_MOTOR_SPEED_NOW, mtrSpeed);
  return parameters.get(CCS_MOTOR_SPEED_NOW);
}

float Ccs::calcMotorCos()
{
  float actPwr = parameters.get(VSD_POWER_ACTIVE);
  float fullPwr =  parameters.get(VSD_POWER_FULL);
  float cos = actPwr;
  if (fullPwr != 0) {
    cos = actPwr / fullPwr;
  }
  if (cos < 0)
    cos = cos * (-1);
  if (cos > 1)
    cos = 1;
  setValue(CCS_MOTOR_COS_PHI_NOW, cos);
  return parameters.get(CCS_MOTOR_COS_PHI_NOW);
}

float Ccs::calcMotorLoad()
{
  float mtrCur = parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
  float mtrCos = parameters.get(CCS_MOTOR_COS_PHI_NOW);
  float nomMtrCur = parameters.get(VSD_MOTOR_CURRENT);
  float nomMtrCos = parameters.get(VSD_MOTOR_COS_PHI);
  float mtrLoad;
  if ((nomMtrCur == 0) || (nomMtrCos == 0))
    mtrLoad = 0;
  else
    mtrLoad = ((mtrCur * mtrCos) / (nomMtrCur * nomMtrCos)) * 100;
  setValue(CCS_MOTOR_LOAD_NOW, mtrLoad);
  return parameters.get(CCS_MOTOR_LOAD_NOW);
}

float Ccs::calcInputVoltagePhase1()
{
  setValue(CCS_VOLTAGE_PHASE_1,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_1),
                     parameters.get(CCS_COEF_VOLTAGE_IN_A)));
  return parameters.get(CCS_VOLTAGE_PHASE_1);
}

float Ccs::calcInputVoltagePhase2()
{
  setValue(CCS_VOLTAGE_PHASE_2,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_2),
                     parameters.get(CCS_COEF_VOLTAGE_IN_B)));
  return parameters.get(CCS_VOLTAGE_PHASE_2);
}

float Ccs::calcInputVoltagePhase3()
{
  setValue(CCS_VOLTAGE_PHASE_3,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_3),
                     parameters.get(CCS_COEF_VOLTAGE_IN_C)));
  return parameters.get(CCS_VOLTAGE_PHASE_3);
}

float Ccs::calcInputVoltagePhase12()
{
  setValue(CCS_VOLTAGE_PHASE_1_2,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_1_2),
                     parameters.get(CCS_COEF_VOLTAGE_IN_A)));
  return parameters.get(CCS_VOLTAGE_PHASE_1_2);
}

float Ccs::calcInputVoltagePhase23()
{
  setValue(CCS_VOLTAGE_PHASE_2_3,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_2_3),
                     parameters.get(CCS_COEF_VOLTAGE_IN_B)));
  return parameters.get(CCS_VOLTAGE_PHASE_2_3);
}

float Ccs::calcInputVoltagePhase31()
{
  setValue(CCS_VOLTAGE_PHASE_3_1,
           applyCoef(parameters.get(EM_VOLTAGE_PHASE_3_1),
                     parameters.get(CCS_COEF_VOLTAGE_IN_C)));
  return parameters.get(CCS_VOLTAGE_PHASE_3_1);
}

float Ccs::calcInputVoltageImbalance()
{
  setValue(CCS_VOLTAGE_IMBALANCE_IN,
           calcImbalance(parameters.get(CCS_VOLTAGE_PHASE_1),
                         parameters.get(CCS_VOLTAGE_PHASE_2),
                         parameters.get(CCS_VOLTAGE_PHASE_3),
                         0));
  return parameters.get(CCS_VOLTAGE_IMBALANCE_IN);
}

float Ccs::calcInputCurrentImbalance()
{
  setValue(CCS_CURRENT_IMBALANCE_IN,
           calcImbalance(parameters.get(EM_CURRENT_PHASE_1),
                         parameters.get(EM_CURRENT_PHASE_2),
                         parameters.get(EM_CURRENT_PHASE_3),
                         1));
  return parameters.get(CCS_CURRENT_IMBALANCE_IN);
}

float Ccs::calcMotorInductFromResistPhase()
{
  float resist = parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE);
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float induct = resist * 1000 / (2 * NUM_PI * freq);
  setValue(CCS_MOTOR_INDUCTANCE, induct);
  return parameters.get(CCS_MOTOR_INDUCTANCE);
}

float Ccs::calcMotorResistPhaseFromInduct()
{
  float induct = parameters.get(CCS_MOTOR_INDUCTANCE);
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float resist = 2 * NUM_PI * induct * freq / 1000;
  setValue(CCS_MOTOR_INDUCTANCE_RESIST_PHASE, resist);
  return parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE);
}

float Ccs::calcSystemInduct()
{
  float coefTrans2 = pow(parameters.get(CCS_COEF_TRANSFORMATION), 2);
  float inductTrans = (parameters.get(CCS_TRANS_VOLTAGE_SHORT_CIRCUIT) * parameters.get(CCS_TRANS_NOMINAL_VOLTAGE) * 0.01) /
                      (parameters.get(CCS_TRANS_NOMINAL_CURRENT) * parameters.get(CCS_TRANS_NOMINAL_FREQUENCY_INPUT) * 2 * NUM_PI);
  float inductCable = ((parameters.get(CCS_TRANS_CABLE_LENGHT) * 1.5) / coefTrans2) / 1000000;
  float inductMotor = (parameters.get(CCS_MOTOR_INDUCTANCE) / coefTrans2) / 1000;
  float inductFilter = parameters.get(CCS_FILTER_INDUCTANCE) / 1000;
  float induct = (inductTrans + inductCable + inductMotor + inductFilter) * 1000;
  parameters.set(CCS_SYSTEM_INDUCTANCE, induct);
  parameters.set(VSD_LOUT, parameters.get(CCS_SYSTEM_INDUCTANCE));
  return parameters.get(CCS_SYSTEM_INDUCTANCE);
}

float Ccs::calcTransTapOff(float coefTrans)
{
  float nomVoltIn = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT);
  float curVoltIn = calcAverage3Values(parameters.get(CCS_VOLTAGE_PHASE_1_2),
                                       parameters.get(CCS_VOLTAGE_PHASE_2_3),
                                       parameters.get(CCS_VOLTAGE_PHASE_3_1));
  if (curVoltIn > nomVoltIn) {
    curVoltIn = nomVoltIn;
  }
  float nomCurMtr = parameters.get(VSD_MOTOR_CURRENT);
  float nomVoltMtr = parameters.get(VSD_MOTOR_VOLTAGE);
  float nomFreqMtr = parameters.get(VSD_MOTOR_FREQUENCY);
  if (nomFreqMtr == 0)
    nomFreqMtr = 50;
  float baseFreq = parameters.get(VSD_HIGH_LIM_SPEED_MOTOR);
  if (baseFreq == 0)
    baseFreq = nomFreqMtr;
  float baseVolt = (baseFreq / nomFreqMtr ) * nomVoltMtr;
  float dropVoltCable = calcDropVoltageCable(nomCurMtr);
  if (coefTrans == -1)
    coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  float dropVoltFilter = calcDropVoltageFilter(nomCurMtr, nomFreqMtr, coefTrans);
  float voltHiLim = parameters.get(CCS_VOLTAGE_HIGH_LIMIT);
  float transTapOff = (baseVolt + dropVoltCable) / (voltHiLim - dropVoltFilter);
  setValue(CCS_TRANS_NEED_VOLTAGE_TAP_OFF, transTapOff);
  return parameters.get(CCS_TRANS_NEED_VOLTAGE_TAP_OFF);
}

uint8_t Ccs::setNewValue(uint16_t id, float value, EventType eventType)
{
  float oldValue = getValue(id);

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
    if (value)
      logStartSave();
    break;
  case CCS_CMD_LOG_DELETE:
    if (value)
      logErase();
    break;
  case CCS_COEF_TRANSFORMATION:
    calcSystemInduct();
    break;
  case CCS_TRANS_VOLTAGE_SHORT_CIRCUIT:
    calcSystemInduct();
    break;
  case CCS_TRANS_NOMINAL_VOLTAGE:           // Номинальное напряжение ТМПН
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    calcTransTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_NOMINAL_CURRENT:
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    break;
  case CCS_VOLTAGE_HIGH_LIMIT:              // Максимальное входное напряжение
    calcTransTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_NOMINAL_FREQUENCY_INPUT:
    calcSystemInduct();
    break;
  case CCS_TRANS_CABLE_LENGHT:              // Длина кабеля
    calcTransTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    parameters.set(VSD_DEPTH, value);       // Записываем в ЧРП
    break;
  case CCS_TRANS_CABLE_CROSS:               // Сечение кабеля
    calcTransTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_VOLTAGE_TAP_OFF:           // Напряжение отпайки
    calcTransTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    parameters.set(VSD_TRANS_VOLTAGE_TAP_OFF, value); // Задаём в ЧРП напряжение отпайки
    break;
  case CCS_MOTOR_INDUCTANCE:
    calcSystemInduct();
    parameters.set(VSD_MOTOR_INDUCTANCE, value);      // Задаём индуктивность в ЧРП
    break;
  case CCS_MOTOR_INDUCTANCE_RESIST_PHASE:
    calcMotorInductFromResistPhase();
    calcSystemInduct();
    parameters.set(VSD_MOTOR_INDUCTANCE_RESIST_PHASE, value);   // Задаём сопротивление в ЧРП
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
  case CCS_CMD_PROT_SUPPLY_OVERVOLTAGE_SETPOINT_RESET:
    cmdProtSupplyOvervoltageSetpointReset();
    break;
  case CCS_CMD_PROT_SUPPLY_UNDERVOLTAGE_SETPOINT_RESET:
    cmdProtSupplyUndervoltageSetpointReset();
    break;
  case CCS_CMD_PROT_SUPPLY_IMBALANCE_VOLTAGE_SETPOINT_RESET:
    cmdProtSupplyImbalanceVoltageSetpointReset();
    break;
  case CCS_CMD_PROT_MOTOR_OVERLOAD_SETPOINT_RESET:
    cmdProtMotorOverloadSetpointReset();
    break;
  case CCS_CMD_PROT_MOTOR_UNDERLOAD_SETPOINT_RESET:
    cmdProtMotorUnderloadSetpointReset();
    break;
  case CCS_CMD_PROT_MOTOR_IMBALANCE_CURRENT_SETPOINT_RESET:
    cmdProtMotorImbalanceCurrentSetpointReset();
    break;
  case CCS_CMD_PROT_DHS_PRESSURE_INTAKE_SETPOINT_RESET:
    cmdProtDhsPressureIntakeSetpointReset();
    break;
  case CCS_CMD_PROT_DHS_TEMPERATURE_MOTOR_SETPOINT_RESET:
    cmdProtDhsTemperatureMotorSetpointReset();
    break;
  case CCS_CMD_PROT_DHS_RESISTANCE_SETPOINT_RESET:
    cmdProtDhsResistanceSetpointReset();
    break;
  case CCS_CMD_COUNTER_ALL_RESET:
    cmdCountersAllReset();
    break;
  case CCS_DHS_TYPE:
    if (value)
      logEvent.add(AddDeviceCode, eventType, AddDeviceDhsId, oldValue, value);
    else
      logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceDhsId, oldValue, value);
    startReboot();
    break;
  case CCS_TYPE_VSD:
    if (value)
      logEvent.add(AddDeviceCode, eventType, AddDeviceVsdId, oldValue, value);
    else
      logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceVsdId, oldValue, value);
    startReboot();
    break;
  case CCS_EM_TYPE:
    if (value)
      logEvent.add(AddDeviceCode, eventType, AddDeviceEmId, oldValue, value);
    else
      logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceEmId, oldValue, value);
    startReboot();
    break;
  case CCS_FILTER_OUTPUT:
    if (value)
      logEvent.add(AddDeviceCode, eventType, AddDeviceFiltOutId, oldValue, value);
    else
      logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceFiltOutId, oldValue, value);
    break;
  case CCS_SCADA_TYPE:
    createScada();
    break;
  default:
    break;
  }
  return setValue(id, value, eventType);
}

uint8_t Ccs::setNewValue(uint16_t id, uint32_t value, EventType eventType)
{
  switch (id) {
  case CCS_DATE_TIME:
    {
      time_t time = value;
      rtcSetTime(&time);
      tm dateTime = *localtime(&time);
      setNewValue(CCS_DATE_TIME_SEC, dateTime.tm_sec);
      setNewValue(CCS_DATE_TIME_MIN, dateTime.tm_min);
      setNewValue(CCS_DATE_TIME_HOUR, dateTime.tm_hour);
      setNewValue(CCS_DATE_TIME_DAY, dateTime.tm_mday);
      setNewValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon);
      setNewValue(CCS_DATE_TIME_YEAR, 1900 + dateTime.tm_year);
    }
    break;
  }
  return setValue(id, value, eventType);
}

uint8_t Ccs::setNewValue(uint16_t id, int value, EventType eventType)
{
  return setNewValue(id, (float)value, eventType);
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

void Ccs::calcRegimeRun()
{
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    parameters.set(CCS_RUNNING_TYPE, Regime::PushRegimeRun);
    return;
  }
  if (parameters.get(CCS_RGM_RUN_SWING_MODE) != Regime::OffAction) {
    parameters.set(CCS_RUNNING_TYPE, Regime::SwingRegimeRun);
    return;
  }
  parameters.set(CCS_RUNNING_TYPE, Regime::SmoothRegimeRun);
  return;
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
  if (checkConnectDeviceTimer_ == 0) {
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
  else {
    checkConnectDeviceTimer_--;
  }
}

void Ccs::cmdProtSupplyOvervoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtSupplyUndervoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtSupplyImbalanceVoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtMotorOverloadSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_OVERLOAD_MODE;
       i <=  CCS_PROT_MOTOR_OVERLOAD_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtMotorUnderloadSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_UNDERLOAD_MODE;
       i <= CCS_PROT_MOTOR_UNDERLOAD_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtMotorImbalanceCurrentSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
       i <= CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtDhsPressureIntakeSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_PRESSURE_INTAKE_MODE;
       i <= CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtDhsTemperatureMotorSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE;
       i <= CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtDhsResistanceSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_RESISTANCE_MODE;
       i <= CCS_PROT_DHS_RESISTANCE_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::cmdProtOtherHardwareVsdSetpointReset()
{
  for (uint16_t i = CCS_PROT_OTHER_VSD_MODE;
       i <= CCS_PROT_OTHER_VSD_PARAMETER; i++) {
    resetValue(i);
  }
}

void Ccs::calcCountersStop(float reason)
{
  switch ((uint16_t)reason) {
  case LastReasonStopOverloadMotor:
    setValue(CCS_PROT_OVERLOAD_COUNT_STOP, getValue(CCS_PROT_OVERLOAD_COUNT_STOP) + 1);
    break;
  case LastReasonStopUnderloadMotor:
    setValue(CCS_PROT_UNDERLOAD_COUNT_STOP, getValue(CCS_PROT_UNDERLOAD_COUNT_STOP) + 1);
    break;
  case LastReasonStopResistIsolation:       // "Сопротивление изоляции"
  case LastReasonStopUnderVoltIn:           // "Снижение питания сети"
  case LastReasonStopOverVoltIn:            // "Превышение питания сети"
  case LastReasonStopHackSu:                // "Взлом СУ"
  case LastReasonStopManometr:              // "Контактный манометр"
  case LastReasonStopPressureIntake:        // "Pmin на приёме насоса"
  case LastReasonStopTemperatureMotor:      // "Перегрев двигателя"
  case LastReasonStopVibrationMotor:        // "Превышение вибрации"
  case LastReasonStopImbalanceCurMotor:     // "Дисбаланс токов ПЭД"
  case LastReasonStopImbalanceVoltIn:       // "Дисбаланс напряжения"
  case LastReasonStopMaxAnalog1:            // "Ан.вх.№1 max"
  case LastReasonStopMaxAnalog2:            // "Ан.вх.№2 max"
  case LastReasonStopCurrentMotor:          // "Предел тока двигателя"
  case LastReasonStopOutOfSyncMotor:        // "Рассинхронизация ПВЭД"
  case LastReasonStopHardwareVsd:           // "Аппаратные защиты ЧРП"
    setValue(CCS_PROT_OTHER_COUNT_STOP, getValue(CCS_PROT_OTHER_COUNT_STOP) + 1);
    break;
  default:
    break;
  }
}

void Ccs::calcCountersRun(float reason)
{
  setValue(CCS_COUNT_START, getValue(CCS_COUNT_START) + 1);
  switch ((uint16_t)reason) {
  case LastReasonRunApvImbalanceVoltIn:
  case LastReasonRunApvOverVoltIn:
  case LastReasonRunApvUnderVoltIn:
    setValue(CCS_PROT_VOLTAGE_COUNT_RESTART,
             getValue(CCS_PROT_VOLTAGE_COUNT_RESTART) + 1);
    break;
  case LastReasonRunApvOverloadMotor:
    setValue(CCS_PROT_OVERLOAD_COUNT_RESTART,
             getValue(CCS_PROT_OVERLOAD_COUNT_RESTART) + 1);
    break;
  case LastReasonRunApvUnderloadMotor:
    setValue(CCS_PROT_UNDERLOAD_COUNT_RESTART,
             getValue(CCS_PROT_UNDERLOAD_COUNT_RESTART) + 1);
    break;
  case LastReasonRunApvImbalanceCurMotor:
    setValue(CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART,
             getValue(CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART) + 1);
  default:
    break;
  }
}

void Ccs::cmdCountersAllReset()
{
  setValue(CCS_COUNT_START, 0.0);
  setValue(CCS_PROT_OVERLOAD_COUNT_STOP, 0.0);
  setValue(CCS_PROT_UNDERLOAD_COUNT_STOP, 0.0);
  setValue(CCS_PROT_OTHER_COUNT_STOP, 0.0);
  setValue(CCS_PROT_VOLTAGE_COUNT_RESTART, 0.0);
  setValue(CCS_PROT_OVERLOAD_COUNT_RESTART, 0.0);
  setValue(CCS_PROT_UNDERLOAD_COUNT_RESTART, 0.0);
  setValue(CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART, 0.0);
}

void Ccs::startReboot()
{
  parameters.startSave();
  logEvent.add(PowerCode, AutoType, PowerOffId);
  osSemaphoreRelease(rebootSemaphoreId_);
}

void Ccs::reboot()
{
  osDelay(500);
  HAL_NVIC_SystemReset();
}

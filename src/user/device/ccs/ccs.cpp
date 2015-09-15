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
#include "update.h"

#define TIMEOUT_POWER_OFF 6000 //!< 1 минута на отключение питания ИБП
#define DELAY_CHECK_CONNECT_DEVICE 1000 //!< Задержка проверки подключения устройств - 20 сек

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

static void ccsCalcParametersTask(void *p)
{
  (static_cast<Ccs*>(p))->calcParametersTask();
}

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

  osThreadDef(VsdConditionTask, ccsVsdConditionTask, osPriorityNormal, 0, 3*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(VsdConditionTask), this);

  osThreadDef(CcsMain, ccsMainTask, osPriorityNormal, 0, 8*configMINIMAL_STACK_SIZE);
  ccsMainTaskId_ = osThreadCreate(osThread(CcsMain), this);

  osThreadDef(CalcParametersTask, ccsCalcParametersTask, osPriorityNormal, 0 , 4*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(CalcParametersTask), this);

  rebootSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(rebootSemaphoreId_, 0);
  updateSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(updateSemaphoreId_, 0);
  scadaSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(scadaSemaphoreId_, 0);

  setCmd(CCS_CMD_SYNC_ALL_PARAMS);
  resetCmd(CCS_CMD_REBOOT_MASTER);
  resetCmd(CCS_CMD_START_REBOOT_SLAVE);
  resetCmd(CCS_CMD_UPDATE_SOFTWARE);
  resetCmd(CCS_CMD_START_UPDATE_SW_SLAVE);
}

void Ccs::mainTask()
{
  while (1) {
    osDelay(10);

    changedWorkMode();
    changedCondition();

    calcTime();
    controlPower();
    checkConnectDevice();

    if (osSemaphoreWait(rebootSemaphoreId_, 0) != osEventTimeout)
      reboot();
    if (osSemaphoreWait(updateSemaphoreId_, 0) != osEventTimeout)
      updateSoftware();
    if (osSemaphoreWait(scadaSemaphoreId_, 0) != osEventTimeout) {
      osDelay(100);
      createScada();
    }
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
  int vsdConditionOld = -1;
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
      if (vsd->stop(checkTypeStop()) == ok_r) {
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOPPING);
      }
      else if (vsdCondition != vsdConditionOld) {
        logDebug.add(WarningMsg, "Ошибка останова");
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
      } else if (vsdCondition != vsdConditionOld) {
        logDebug.add(WarningMsg, "Ошибка запуска");
      }
      break;
    }

    vsdConditionOld = vsdCondition;
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

void Ccs::cmdStart(int value)
{
  switch (value) {
  case CmdStartRemote:
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunRemote);
    break;
  default:
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunOperator);
    break;
  }

  resetCmd(CCS_CMD_START);
  if (checkCanStart()) {
    float reason = getValue(CCS_LAST_RUN_REASON_TMP);
    setNewValue(CCS_LAST_RUN_REASON, reason);
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    setNewValue(CCS_CONDITION, CCS_CONDITION_RUNNING);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
    calcCountersRun(reason);
  }
}

void Ccs::cmdStop(int value)
{
  switch (value) {
  case CmdStopRemote:
    setNewValue(CCS_LAST_STOP_REASON_TMP, LastReasonStopRemote);
    break;
  default:
    setNewValue(CCS_LAST_STOP_REASON_TMP, LastReasonStopOperator);
    break;
  }

  resetCmd(CCS_CMD_STOP);
  if (checkCanStop()) {
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    setBlock();
    setNewValue(CCS_CONDITION, CCS_CONDITION_STOPPING);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
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

#if (USE_DEBUG == 0)
  if (!vsd->isConnect())
    return false;
#endif

  return true;
}

bool Ccs::checkCanStop()
{
#if USE_DEBUG
  return true;
#endif

  if (getValue(CCS_VSD_CONDITION) == VSD_CONDITION_STOP) {
    setBlock();
    return false;
  }
  return true;
}

float Ccs::checkTypeStop()
{
  float reason = getValue(CCS_LAST_STOP_REASON_TMP);
  if ((reason == LastReasonStopOperator) ||
      (reason == LastReasonStopProgram) ||
      (reason == LastReasonStopRemote)) {
    return parameters.get(VSD_TYPE_STOP);
  }
  else {
    return TYPE_STOP_ALARM;
  }
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

void Ccs::setFreq(float value)
{
  if (vsd->setFrequency(value) == ok_r) {
    parameters.set(CCS_PREVIEW_FREQUENCY, parameters.get(VSD_FREQUENCY));
    parameters.set(CCS_PREVIEW_FREQUENCY_DATE_TIME, parameters.getU32(CCS_DATE_TIME));
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
  if (getValue(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_BLOCK) {
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
    vsd->resetBlock();
  }
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
  if (controlMode == CCS_WORKING_MODE_PROGRAM) {
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
    setValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon+1, NoneType);
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



uint8_t Ccs::setNewValue(uint16_t id, float value, EventType eventType)
{
  uint8_t err;
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
    else {
      vsd->offRegimePush();
    }
    break;
  case CCS_RGM_RUN_SWING_MODE:
    if (value != Regime::OffAction) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::OffAction); // Отключаем режим раскачки
    }
    else {
      vsd->offRegimeSwing();
    }
    break;
  case CCS_CMD_LOG_COPY:
    err = setValue(id, value, eventType);
    if (value)
      logStartSave(eventType);
    return err;
  case CCS_CMD_LOG_DELETE: case CCS_CMD_SERVICE_LOG_DELETE:
    err = setValue(id, value, eventType);
    if (value)
      logStartDelete(eventType);
    return err;
  case CCS_COEF_TRANSFORMATION:
    calcSystemInduct();
    break;
  case CCS_TRANS_VOLTAGE_SHORT_CIRCUIT:
    calcSystemInduct();
    break;
  case CCS_TRANS_NOMINAL_VOLTAGE:           // Номинальное напряжение ТМПН
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_NOMINAL_VOLTAGE_INPUT:
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_NOMINAL_CURRENT:
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    break;
  case CCS_VOLTAGE_HIGH_LIMIT:              // Максимальное входное напряжение
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_NOMINAL_FREQUENCY_INPUT:
    calcSystemInduct();
    break;
  case CCS_TRANS_CABLE_LENGHT:              // Длина кабеля
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    parameters.set(VSD_DEPTH, value);       // Записываем в ЧРП
    break;
  case CCS_TRANS_CABLE_CROSS:               // Сечение кабеля
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
    break;
  case CCS_TRANS_VOLTAGE_TAP_OFF:           // Напряжение отпайки
    calcTransRecommendedTapOff();                      // Пересчитываем рекомендуемое напряжение отпайки
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
  case CCS_CMD_PROT_OTHER_VSD_SETPOINT_RESET:
    cmdProtOtherHardwareVsdSetpointReset();
    break;
  case CCS_CMD_COUNTER_ALL_RESET:
    cmdCountersAllReset();
    break;
  case CCS_DHS_TYPE:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceDhsId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceDhsId, oldValue, value);
      tms->initParameters();
      startReboot();
    }
    return err;
  case CCS_TYPE_VSD:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceVsdId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceVsdId, oldValue, value);
      vsd->initParameters();
      startReboot();
    }
    return err;
  case CCS_EM_TYPE:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceEmId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceEmId, oldValue, value);
      em->initParameters();
      startReboot();
    }
    return err;
  case CCS_FILTER_OUTPUT:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceFiltOutId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceFiltOutId, oldValue, value);
    }
    return err;
  case CCS_SCADA_TYPE: case CCS_SCADA_ADDRESS: case CCS_SCADA_BYTERATE:
  case CCS_SCADA_PARITY: case CCS_SCADA_DELAY:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err)
      osSemaphoreRelease(scadaSemaphoreId_);
    return err;
  case CCS_COEF_OUT_CURRENT_1:
    parameters.set(VSD_COEF_OUT_CURRENT_1, value);
    break;
  case CCS_COEF_OUT_CURRENT_2:
    parameters.set(VSD_COEF_OUT_CURRENT_2, value);
    break;
  case CCS_COEF_OUT_CURRENT_3:
    parameters.set(VSD_COEF_OUT_CURRENT_3, value);
    break;
  case CCS_CMD_REBOOT_SOFTWARE:
    err = setValue(id, 0.0, eventType);
    if (value && !err) {
      setCmd(CCS_CMD_REBOOT_MASTER);
      logEvent.add(PowerCode, AutoType, RebootSoftwareId);
      startReboot();
    }
    return err;
  case CCS_CMD_UPDATE_SOFTWARE:
    err = setValue(id, value, eventType);
    if (value)
      startUpdateSoftware();
    return err;
  case CCS_CMD_START:
    cmdStart(value);
    return ok_r;
  case CCS_CMD_STOP:
    cmdStop(value);
    return ok_r;
  case CCS_CMD_VSD_RESET_SETPOINTS:
    return vsd->resetSetpoints();
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

void Ccs::controlPower()
{
  if (!isPowerGood()) {
    if (powerOffTimeout_ == (TIMEOUT_POWER_OFF - 1000)) {
      setCmd(CCS_CMD_AM335_POWER_OFF);
      offLcd();
    }

    if ((powerOffTimeout_ == 1000) || !isUpsGood()) {
      if (!powerOffFlag_) {
        setCmd(CCS_CMD_AM335_POWER_OFF);

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
    resetCmd(CCS_CMD_AM335_POWER_OFF);

    if (powerOffTimeout_ <= (TIMEOUT_POWER_OFF - 1000)) {
      onLcd();
      resetAm335x();
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
  resetValue(CCS_TIMER_DIFFERENT_START);
}

void Ccs::cmdProtSupplyUndervoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER; i++) {
    resetValue(i);
  }
  resetValue(CCS_TIMER_DIFFERENT_START);
}

void Ccs::cmdProtSupplyImbalanceVoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER; i++) {
    resetValue(i);
  }
  resetValue(CCS_TIMER_DIFFERENT_START);
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
  setCmd(CCS_CMD_START_REBOOT_SLAVE);
  logEvent.add(PowerCode, AutoType, RebootSoftwareId);
  parameters.startSave();
  osSemaphoreRelease(rebootSemaphoreId_);
}

void Ccs::reboot()
{
  logDebug.add(WarningMsg, "Перезагрузка");
  osDelay(200);
  osThreadSuspendAll();
  HAL_NVIC_SystemReset();
}

void Ccs::startUpdateSoftware()
{
  setCmd(CCS_CMD_START_UPDATE_SW_SLAVE);
  logEvent.add(UpdateSwCode, AutoType, UpdateSoftwareId);
  osSemaphoreRelease(updateSemaphoreId_);
}

void Ccs::updateSoftware()
{
  logDebug.add(WarningMsg, "Обновление ПО");
  osDelay(200);
  if (updateFromUsb()) {
    resetCmd(CCS_CMD_UPDATE_SOFTWARE);
    startReboot();
  }
  else {
    resetCmd(CCS_CMD_UPDATE_SOFTWARE);
  }
}

void Ccs::setCmd(uint16_t id)
{
  setValue(id, 0.0);
  setValue(id, 1.0);
}

void Ccs::resetCmd(uint16_t id)
{
  setValue(id, 0.0);
}

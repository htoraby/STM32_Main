﻿/*
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
#include "adc_ext.h"
#include "usb_host.h"

#define DELAY_MAIN_TASK 1
#define TIMEOUT_POWER_OFF 60000 //!< мс на отключение питания ИБП
#define TIMEOUT_LCD_OFF 1000 //!< Время на отключение подсветки (мс)
#define TIMEOUT_MASTER_OFF 3000 //!< Время на отключение верхнего контроллера (мс)
#define TIMEOUT_SLAVE_OFF 10000 //!< Время на отключение нижнего контроллера (мс)
#define DELAY_CHECK_CONNECT_DEVICE 1000 //!< Задержка проверки подключения устройств - x*10мс

//! Массив параметров устройства
static parameter parametersArray[CCS_END - CCS_BEGIN] __attribute__((section(".extmem")));

static void ccsMainTask(void *p)
{
  (static_cast<Ccs*>(p))->mainTask();
}

static void ccsToolsTask(void *p)
{
  (static_cast<Ccs*>(p))->toolsTask();
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
  : Device(CCS_BEGIN, parametersArray, CCS_END - CCS_BEGIN)
  , conditionOld_(-1)
  , flagOld_(-1)
  , workModeOld_(-1)
  , powerOffFlag_(false)
  , powerOffTimeout_(0)
  , checkConnectDeviceTimer_(DELAY_CHECK_CONNECT_DEVICE)
  , isConnectMaster_(true)
  , countPhaseRotation_(0)
  , isParametersControl_(true)
{

}

Ccs::~Ccs()
{

}

void Ccs::init()
{  
  initParameters();
  readParameters();
  if (getValue(CCS_PARAMETERS_CONTROL) != getValueDef(CCS_PARAMETERS_CONTROL)) {
    asm("nop");
  }
}

void Ccs::initTask()
{
  conditionOld_ = getValue(CCS_CONDITION);

  osMessageQDef(LedMessageQ, 5, uint8_t);
  ledMessage_ = osMessageCreate(osMessageQ(LedMessageQ), NULL);
  osThreadDef(LedCondition, ccsLedConditionTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(LedCondition), this);

  osThreadDef(VsdCondition, ccsVsdConditionTask, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(VsdCondition), this);

  osThreadDef(CcsMain, ccsMainTask, osPriorityNormal, 0, 4*configMINIMAL_STACK_SIZE);
  ccsMainTaskId_ = osThreadCreate(osThread(CcsMain), this);

  osThreadDef(CcsTools, ccsToolsTask, osPriorityNormal, 0, 7*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(CcsTools), this);

  osThreadDef(CalcParameters, ccsCalcParametersTask, osPriorityNormal, 0 , 3*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(CalcParameters), this);

  rebootSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(rebootSemaphoreId_, 0);
  updateSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(updateSemaphoreId_, 0);
  scadaSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(scadaSemaphoreId_, 0);
  setProfileDefaultSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(setProfileDefaultSemaphoreId_, 0);
  saveConfigSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(saveConfigSemaphoreId_, 0);
  loadConfigSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(loadConfigSemaphoreId_, 0);

  setCmd(CCS_CMD_SYNC_ALL_PARAMS);
  resetCmd(CCS_CMD_REBOOT_MASTER);
  resetCmd(CCS_CMD_START_REBOOT_SLAVE);
  resetCmd(CCS_CMD_START_UPDATE_SW_SLAVE);
  resetCmd(CCS_CMD_UPDATE_SW_MASTER);
  resetCmd(CCS_CMD_TYPE_PROFILE_VSD);
  resetCmd(CCS_ERROR_SLAVE);
  resetCmd(CCS_CMD_SAVE_SETPOINT);
  resetCmd(CCS_CMD_LOAD_SETPOINT);
  resetCmd(CCS_CMD_SAVE_CONFIG_USB);
  resetCmd(CCS_CMD_LOAD_CONFIG_USB);
  setValue(CCS_PROT_SUPPLY_POWEROFF_PREVENT, 0);

  uint32_t installSw = getValueUint32(CCS_DATE_INSTALL_SW_CCS);
  if (installSw == 0) {
    installSw = getTime();
    setValue(CCS_DATE_INSTALL_SW_CCS, installSw, NoneType);
  }
  setValue(CCS_REVISION_SW_CORE, VCS_REVISION, NoneType);
  if (getValueUint32(CCS_LAST_STOP_DATE_TIME) == 0)
    setValue(CCS_LAST_STOP_DATE_TIME, getTime());

  // Выключить контактор KM1
  setValue(CCS_BYPASS_CONTACTOR_KM1_CONTROL, 0);
  // Переключение RS-485/RS-232 ТМС/Скады
  setDhsScadaInterface();

  intRestartCount();
}

void Ccs::mainTask()
{
  int time10ms = HAL_GetTick();
  int time1s = HAL_GetTick();

  while (1) {
    osDelay(DELAY_MAIN_TASK);

    controlPower();

    if (isPowerOff())
      continue;

    if ((HAL_GetTick() - time10ms) >= 10) {
      time10ms = HAL_GetTick();

      changedWorkMode();
      changedCondition();

      calcTime();
      checkConnectDevice();
      setRelayOutputs();
    }

    if ((HAL_GetTick() - time1s) >= 1000) {
      time1s = HAL_GetTick();

      indicationTurbineRotation();
    }
  }
}

void Ccs::toolsTask()
{
  while (1) {
    osDelay(50);

    if (isPowerOff())
      continue;

    if (osSemaphoreWait(rebootSemaphoreId_, 0) != osEventTimeout)
      reboot();
    if (osSemaphoreWait(updateSemaphoreId_, 0) != osEventTimeout)
      updateSoftware();
    if (osSemaphoreWait(scadaSemaphoreId_, 0) != osEventTimeout) {
      createScada();
    }

    if (osSemaphoreWait(setProfileDefaultSemaphoreId_, 0) != osEventTimeout)
      parameters.setProfileDefaultSetpoint();
    if (osSemaphoreWait(saveConfigSemaphoreId_, 0) != osEventTimeout)
      parameters.saveConfig();
    if (osSemaphoreWait(loadConfigSemaphoreId_, 0) != osEventTimeout)
      parameters.loadConfig();

    checkConnectMaster();
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
  int statusOld = 0;
  int timer = 0;

  while (1) {
    osDelay(10);

    if (isPowerOff())
      continue;

    int vsdCondition = getValue(CCS_VSD_CONDITION);
    if (vsdCondition != vsdConditionOld)
      timer = 0;

    switch (vsdCondition) {
    case VSD_CONDITION_STOP:
      if (getValue(CCS_CONDITION) != CCS_CONDITION_STOP) {
        setNewValue(CCS_CONDITION, CCS_CONDITION_STOP);
      } else {
        if (checkStartDevice()) {
          if (++timer >= 500) {
            syncStart();
          }
        } else {
          timer = 0;
        }
      }
      break;
    case VSD_CONDITION_STOPPING:
      if (checkStopDevice()) {
        resetBlockDevice();
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOP);
      }
      if (checkStartDevice()) {
        if (++timer >= 18000) {
          syncStart();
        }
      }
      break;
    case VSD_CONDITION_WAIT_STOP:
      if (stopDevice() == ok_r) {
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOPPING);
      }
      else if (vsdCondition != vsdConditionOld) {
#if (USE_LOG_WARNING == 1)
        logDebug.add(WarningMsg, "Ccs::vsdConditionTask() Error stoping (VSD = %d)", (int)getValue(CCS_TYPE_VSD));
#endif
      }
      break;
    case VSD_CONDITION_RUN:
      if (getValue(CCS_CONDITION) != CCS_CONDITION_WORK) {
        if ((parameters.get(VSD_FREQUENCY_NOW) >= parameters.get(VSD_FREQUENCY))
         && (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState))
          setNewValue(CCS_CONDITION, CCS_CONDITION_WORK);
#if USE_DEBUG
        setNewValue(CCS_CONDITION, CCS_CONDITION_RUN);
#endif
      }
      if (checkStopDevice()) {
        if (++timer >= 500) {
          syncStop();
        }
      } else {
        timer = 0;
      }
      break;
    case VSD_CONDITION_RUNNING:
      if (checkStartDevice()) {
        setLedCondition(ToogleGreenLed);
        // Запуск сохранения пускового архива
        logRunning.start();
        setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_RUN);
      }

      if (checkStopDevice()) {
        if (++timer >= 500) {
          syncStop();
        }
      }
      break;
    case VSD_CONDITION_WAIT_RUN:
      int init = (vsdCondition != vsdConditionOld);
      int status = startDevice(init);
      if (status == ok_r) {
        if ((int)getValue(CCS_VSD_CONDITION) == VSD_CONDITION_WAIT_RUN)
          setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_RUNNING);
      } else if ((status == err_r) && (init || (status != statusOld))) {
#if (USE_LOG_WARNING == 1)
        logDebug.add(WarningMsg, "Ccs::vsdConditionTask() Error running (VSD = %d)", (int)getValue(CCS_TYPE_VSD));
#endif
      }
      statusOld = status;
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
    case CCS_CONDITION_RUN:
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
    case CCS_CONDITION_WORK:
      if (flag == CCS_CONDITION_FLAG_DELAY) {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionDelay);
        setLedCondition(OnGreenToogleYellowLed);
      }
      else {
        setNewValue(CCS_GENERAL_CONDITION, GeneralConditionRun);
        setLedCondition(OnGreenLed);
      }
      break;
    case CCS_CONDITION_BREAK:
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
        logEvent.add(OtherCode, AutoType, StopMotorId);
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

void Ccs::start(LastReasonRun reason, bool force)
{
  setNewValue(CCS_LAST_RUN_REASON_TMP, reason);

  if (checkCanStart(force)) {
    initStart();
    setNewValue(CCS_LAST_RUN_REASON, reason);
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    setNewValue(CCS_CONDITION, CCS_CONDITION_RUN);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
    calcCountersRun(reason);
  }
}

void Ccs::stop(LastReasonStop reason)
{
  if (reason == LastReasonStopRemote) {
    setNewValue(CCS_LAST_STOP_REASON, reason);
    setBlock();
  }

  if (checkCanStop()) {
    logData.add();
    logEvent.add(StopCode, NoneType, (EventId)reason);
    setNewValue(CCS_LAST_STOP_REASON, reason);

    setNewValue(CCS_CONDITION, CCS_CONDITION_BREAK);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);

    calcCountersStop(reason);
  }
}

void Ccs::syncStart()
{
#if (USE_LOG_WARNING == 1)
  logDebug.add(WarningMsg, "Ccs::syncStart() Error control VSD (Synchronization = Work)");
#endif

  resetBlock();
  resetRestart();
  setNewValue(CCS_LAST_RUN_REASON, LastReasonRunApvHardwareVsd);
  setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
  setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_RUN);
  setNewValue(CCS_CONDITION, CCS_CONDITION_WORK);
  calcCountersRun(LastReasonRunApvHardwareVsd);
}

void Ccs::syncStop()
{
#if (USE_LOG_WARNING == 1)
  logDebug.add(WarningMsg, "Ccs::syncStop() Error control VSD (Synchronization = Stop)");
#endif

  setBlock();
  logEvent.add(StopCode, NoneType, (EventId)LastReasonStopVsdErrControl);
  setNewValue(CCS_LAST_STOP_REASON, LastReasonStopVsdErrControl);
  setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_STOP);
}

void Ccs::cmdStart(int value)
{
  resetCmd(CCS_CMD_START);

  switch (value) {
  case CmdStartRemote:
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunRemote);
    resetBlock();
    break;
  default:
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunOperator);
    break;
  }

  if (checkCanStart()) {
    initStart();
    float reason = getValue(CCS_LAST_RUN_REASON_TMP);
    setNewValue(CCS_LAST_RUN_REASON, reason);
    setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
    setNewValue(CCS_CONDITION, CCS_CONDITION_RUN);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_RUN);
    calcCountersRun(reason);
  }
}

void Ccs::cmdStop(int value)
{
  resetCmd(CCS_CMD_STOP);

  float reason = LastReasonStopOperator;
  if (value == CmdStopRemote) {
    setNewValue(CCS_LAST_STOP_REASON, LastReasonStopRemote);
    reason = LastReasonStopRemote;
    setBlock();
  } else if (isRestart()) {
    setBlock();
  }

  if (checkCanStop()) {
    logData.add();
    logEvent.add(StopCode, NoneType, (EventId)reason);
    setNewValue(CCS_LAST_STOP_REASON, reason);
    setBlock();

    setNewValue(CCS_CONDITION, CCS_CONDITION_BREAK);
    setNewValue(CCS_VSD_CONDITION, VSD_CONDITION_WAIT_STOP);
    calcCountersStop(reason);
  }    
}

bool Ccs::checkCanStart(bool isForce)
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
      if (parameters.get(CCS_LAST_RUN_REASON_TMP) != LastReasonRunOperator)
         return false;
    }
    else {
      setNewValue(CCS_LAST_RUN_REASON_TMP, LastReasonRunNone);
      addEventProtectionPrevent();
      return false;
    }
  }

  if (!isForce) {
    if (!interceptionStartRegime())
      return false;
  }

#if (USE_DEBUG == 0)
  if (!vsd->isConnect())
    return false;
#endif
  return true;
}

bool Ccs::checkStartDevice()
{
  bool result = false;
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) &&
      (parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON)) {                                // Если прямой пуск
    result = parameters.get(CCS_BYPASS_CONTACTOR_KM1_STATE);                    // Состояние контактора прямого пуска
  }
  else {
    result = vsd->checkStart();
  }
  return result;
}

void Ccs::initStart()
{ 
  float freqSetpoint = parameters.get(VSD_FREQUENCY);
  float freq = freqSetpoint;
  if (parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE)) {
    freq = min(freq, parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_1));
  }
  if (parameters.get(CCS_RGM_CHANGE_FREQ_MODE)) {
    freq = min(freq, parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ));
  }
  if (freq != freqSetpoint) {
    setFreq(freq, NoneType);
  }
}


bool Ccs::checkCanStop()
{
#if USE_DEBUG
  return true;
#endif

  if ((int)getValue(CCS_VSD_CONDITION) == VSD_CONDITION_STOP) {
    return false;
  }

//  if (!interceptionStopRegime())
//    return false;

  return true;
}

bool Ccs::checkStopDevice()
{
  bool result = false;
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) &&
      (parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON)) {                      // Если прямой пуск
    result = !parameters.get(CCS_BYPASS_CONTACTOR_KM1_STATE);                   // Состояние контактора прямого пуска
  }
  else {
    result = vsd->checkStop();
  }
  return result;
}

int Ccs::stopDevice()
{
  int err = ok_r;
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) &&
      (parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON)) {
    err = parameters.set(CCS_BYPASS_CONTACTOR_KM1_CONTROL, 0);
  }
  else {
    err = vsd->stop(isAlarmStop());
  }
  return err;
}

int Ccs::startDevice(bool init)
{
  int err = ok_r;
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) &&
      (parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON)) {
    err = parameters.set(CCS_BYPASS_CONTACTOR_KM1_CONTROL, 1);
  }
  else {
    err = vsd->start(init);
  }
  return err;
}

float Ccs::isAlarmStop()
{
  float reason = getValue(CCS_LAST_STOP_REASON);
  if ((reason == LastReasonStopOperator) ||
      (reason == LastReasonStopProgram) ||
      (reason == LastReasonStopRemote) ||
      (reason == LastReasonStopDigital1) ||
      (reason == LastReasonStopDigital2) ||
      (reason == LastReasonStopDigital3) ||
      (reason == LastReasonStopDigital4)) {
    return false;
  }
  else {
    return true;
  }

}

bool Ccs::isBreakOrStopMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_BREAK) ||
      (state == CCS_CONDITION_STOP)) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isStopMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if (state == CCS_CONDITION_STOP) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isRunOrWorkMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUN) ||
      (state == CCS_CONDITION_WORK)) {
    return true;
  }
  else {
    return false;
  }
}

bool Ccs::isWorkMotor()
{
  unsigned int state = getValue(CCS_CONDITION);
  if (state == CCS_CONDITION_WORK) {
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

int Ccs::setFreq(float value, EventType eventType, bool setPoint)
{
  float valueOld = parameters.get(VSD_FREQUENCY);
  if (vsd->setFrequency(value, eventType) == ok_r) {
    if (setPoint) {
      parameters.set(CCS_SETPOINT_FREQUENCY, value);
    }
    parameters.set(CCS_PREVIEW_FREQUENCY, valueOld, NoneType);
    parameters.set(CCS_PREVIEW_FREQUENCY_DATE_TIME, parameters.getU32(CCS_DATE_TIME), NoneType);
    return ok_r;
  }
  return err_r;
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
    resetRestartCount();
    setNewValue(CCS_CONDITION_FLAG, CCS_CONDITION_FLAG_NULL);
    resetBlockDevice();
  }
}

void Ccs::resetBlockDevice()
{
  if (!(parameters.get(CCS_RGM_RUN_DIRECT_MODE) &&
        (parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON))) {
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
  return rtcGetTime();
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
  int32_t sec = getTime() - parameters.getU32(timeId);
  if (sec > 0)
    return sec;
  else
    return 0;
}

void Ccs::calcTime()
{
  static int conditionOld = getValue(CCS_CONDITION);
  static uint32_t timer = HAL_GetTick();
  static float generalRunTime = getValue(CCS_GENERAL_RUN_DATE_TIME);
  static float generalStopTime = getValue(CCS_GENERAL_STOP_DATE_TIME);

  int condition = getValue(CCS_CONDITION);

  if ((HAL_GetTick() - timer) >= 100) {
    timer = HAL_GetTick();
    time_t time = getTime();
    setValue(CCS_DATE_TIME, (uint32_t)time, NoneType);
    tm dateTime = *localtime(&time);
    setValue(CCS_DATE_TIME_SEC, dateTime.tm_sec, NoneType);
    setValue(CCS_DATE_TIME_MIN, dateTime.tm_min, NoneType);
    setValue(CCS_DATE_TIME_HOUR, dateTime.tm_hour, NoneType);
    setValue(CCS_DATE_TIME_DAY, dateTime.tm_mday, NoneType);
    setValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon + 1, NoneType);
    setValue(CCS_DATE_TIME_YEAR, 1900 + dateTime.tm_year, NoneType);
    // TODO: Синхронизация времени
  }

  if (conditionOld != condition) {
    if ((condition != CCS_CONDITION_STOP) && (conditionOld == CCS_CONDITION_STOP)) {
      setNewValue(CCS_LAST_RUN_DATE_TIME, getTime());
      generalStopTime = getValue(CCS_GENERAL_STOP_DATE_TIME);
    }
    if ((condition == CCS_CONDITION_STOP) && (conditionOld != CCS_CONDITION_STOP)) {
      setNewValue(CCS_LAST_STOP_DATE_TIME, getTime());          // Сохранили время перехода в СТО
      generalRunTime = getValue(CCS_GENERAL_RUN_DATE_TIME);     // Сохранили общее время наработки
    }
    conditionOld = condition;
  }

  if (condition == CCS_CONDITION_STOP) {
    setNewValue(CCS_STOP_TIME, (float)getSecFromCurTime(CCS_LAST_STOP_DATE_TIME));
    setNewValue(CCS_GENERAL_STOP_DATE_TIME, generalStopTime + (float)getSecFromCurTime(CCS_LAST_STOP_DATE_TIME));
  }
  else {
    setNewValue(CCS_RUN_TIME, (float)getSecFromCurTime(CCS_LAST_RUN_DATE_TIME)); 
    setNewValue(CCS_GENERAL_RUN_DATE_TIME, generalRunTime + (float)getSecFromCurTime(CCS_LAST_RUN_DATE_TIME));
  }
}

uint8_t Ccs::setNewValue(uint16_t id, float value, EventType eventType)
{
  uint8_t err = ok_r;
  float oldValue = getValue(id);
  switch (id) {
  case CCS_WORKING_MODE:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value == CCS_WORKING_MODE_MANUAL) {
        logEvent.add(ModeCode, eventType, ModeCodeManualId, oldValue, value);
      }
      if (value == CCS_WORKING_MODE_AUTO) {
        logEvent.add(ModeCode, eventType, ModeCodeAutoId, oldValue, value);
      }
      if (value == CCS_WORKING_MODE_PROGRAM) {
        setValue(CCS_RGM_PERIODIC_MODE, Regime::OnAction);
        logEvent.add(ModeCode, eventType, ModeCodeProgramId, oldValue, value);
      } else {
        setValue(CCS_RGM_PERIODIC_MODE, Regime::OffAction);
      }
    }
    return err;
  case CCS_RGM_PERIODIC_MODE:
    if (value != oldValue) {                          // Если новое значение
      if (value != Regime::OffAction) {               // Если включили режим
        err = offWorkRgmExcept(CCS_RGM_PERIODIC_MODE);
        if (!err) {
          err = setValue(id, value, eventType);         // Запись в регистр
          if (!err) {
            setNewValue(CCS_WORKING_MODE, CCS_WORKING_MODE_PROGRAM);
          }
        }
      }
      else {                                          // Выключили режим
        err = setValue(id, value, eventType);         // Запись в регистр
        if (!err) {                                   // Нет ошибок записи в регистр
          setNewValue(CCS_WORKING_MODE, CCS_WORKING_MODE_AUTO);
        }
      }
    }
    return err;
  case CCS_RGM_ALTERNATION_FREQ_MODE:
  case CCS_RGM_CHANGE_FREQ_MODE:
  case CCS_RGM_MAINTENANCE_PARAM_MODE:
  case CCS_RGM_JARRING_MODE:
  case CCS_RGM_PUMP_GAS_MODE:
    if (value != oldValue) {                          // Если новое значение
      if (value != Regime::OffAction) {               // Если включили режим
        err = offWorkRgmExcept(id);
        if (!err) {
          err = setValue(id, value, eventType);       // Запись в регистр
        }
      }
      else {
        err = setValue(id, value, eventType);         // Запись в регистр
      }
    }
    return err;
  case CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT:
    err = setValue(id, value, eventType);
    vsd->setProtOverloadMotorTripSetpoint(value);
    return err;
  case CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY:
    err = setValue(id, value, eventType);
    vsd->setProtOverloadMotorActivDelay(value);
    break;
  case CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY:
    err = setValue(id, value, eventType);
    vsd->setProtOverloadMotorTripDelay(value);
    return err;
  case CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT:
    err = setValue(id, value, eventType);
    vsd->setProtCurrentMotorTripSetpoint(value);
    return err;
  case CCS_RGM_CHANGE_FREQ_PERIOD:
    err = setValue(id, value, eventType);
    calcRegimeChangeFreqPeriodOneStep();
    return err;
  case CCS_RGM_CHANGE_FREQ_BEGIN_FREQ:
    err = setValue(id, value, eventType);
    if ((!err) && (value > getMin(CCS_RGM_CHANGE_FREQ_END_FREQ)))
      setMin(CCS_RGM_CHANGE_FREQ_END_FREQ, value);
    calcRegimeChangeFreqPeriodOneStep();
    return err;
  case CCS_RGM_CHANGE_FREQ_END_FREQ:
    err = setValue(id, value, eventType);
    if ((!err) && (value < getMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ)))
      setMax(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ, value);
    calcRegimeChangeFreqPeriodOneStep();
    return err;
  case CCS_RGM_RUN_PUSH_MODE:
    if (value != Regime::OffAction) {                 // Включаем режим
      err = offRunModeExcept(CCS_RGM_RUN_PUSH_MODE);  // Выключаем все режимы кроме этого
      if (!err) {                                     // Если выключили
        err = setValue(id, value, eventType);         // Пишем в регистр
        if (!err) {                                   // Записали в регистр
          vsd->onRegimePush();                        // Функция ЧРП если она есть
        }
      }
    }
    else {                                            // Выключаем режим
      err = setValue(id, value, eventType);           //
      if (!err) {
        vsd->offRegimePush();
      }
    }
    return err;
  case CCS_RGM_RUN_SWING_MODE:
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_SWING_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
        if (!err) {
          vsd->onRegimeSwing();
        }
      }
    }
    else {
      err = setValue(id, value, eventType);
      if (!err) {
        vsd->offRegimeSwing();
      }
    }
    return err;
  case CCS_RGM_RUN_PUSH_FREQ: case CCS_RGM_RUN_PUSH_TIME:
  case CCS_RGM_RUN_PUSH_PERIOD: case CCS_RGM_RUN_PUSH_QUANTITY:
    err = setValue(id, value, eventType);
    vsd->onRegimePush();
    return err;
  case CCS_RGM_RUN_PUSH_VOLTAGE:
    err = setValue(id, value, eventType);
    vsd->onRegimePush();
    vsd->onRegimeSwing();
    return err;
  case CCS_RGM_RUN_SWING_FREQ: case CCS_RGM_RUN_SWING_QUANTITY:
    err = setValue(id, value, eventType);
    vsd->onRegimeSwing();
    return err;
  case CCS_RGM_RUN_PICKUP_MODE:
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_PICKUP_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
        if (!err) {
          parameters.set(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ProtModeOff); // Отключаем защиту турбин. вращен.
          vsd->onRegimePickup();
        }
      }
    }
    else {
      err = setValue(id, value, eventType);
      if (!err) {
        vsd->offRegimePickup();
      }
    }
    return err;
  case CCS_RGM_RUN_SKIP_RESONANT_MODE:
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_SKIP_RESONANT_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
        if (!err) {
          vsd->onRegimeSkipFreq();
        }
      }
    }
    else {
      err = setValue(id, value, eventType);
      if (!err) {
        vsd->offRegimeSkipFreq();
      }
    }
    return err;
  case CCS_RGM_RUN_AUTO_ADAPTATION_MODE:
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_AUTO_ADAPTATION_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
      }
    }
    else {
      err = setValue(id, value, eventType);
    }
    return err;
  case CCS_RGM_RUN_SYNCHRON_MODE:
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_SYNCHRON_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
      }
    }
    else {
      err = setValue(id, value, eventType);
    }
    return err;
  case CCS_RGM_OPTIM_VOLTAGE_MODE:
    if (value != oldValue) {
      if (value != Regime::OffAction) {
        err = offWorkRgmExcept(CCS_RGM_OPTIM_VOLTAGE_MODE);
        if (!err) {
          err = setValue(id, value, eventType);
        }
      }
      else {
        err = setValue(id, value, eventType);
      }
    }
    return err;
  case CCS_RGM_CURRENT_LIMIT_MODE:
    if (value != oldValue) {
      if (value != Regime::OffAction) {
        //err = offWorkRgmExcept(CCS_RGM_CURRENT_LIMIT_MODE);
        //if (!err) {
          err = setValue(id, value, eventType);
          if (!err) {
            vsd->onRegimeCurrentLimitation();
          }
      }
      else {
        err = setValue(id, value, eventType);
        if (!err) {
          vsd->offRegimeCurrentLimitation();
        }
      }
    }
    return err;
  case CCS_RGM_CURRENT_LIMIT_SETPOINT:
  case CCS_RGM_CURRENT_LIMIT_STEPDOWN:
  case CCS_RGM_CURRENT_LIMIT_DELAY_REACTION:
  case CCS_RGM_CURRENT_LIMIT_DELAY_RESTART:
    err = setValue(id, value, eventType);
//    vsd->configRegimeCurrentLimitation();
    return err;
  case CCS_VSD_DECEL:
    err = setValue(id, value, eventType);
    vsd->setDischarge(value);
    return err;
  case CCS_PROT_MOTOR_ASYNC_MODE:
    err = setValue(id, value, eventType);
    if (value != Protection::ProtModeOff) {
      parameters.set(CCS_RGM_RUN_PICKUP_MODE, Regime::OffAction); // Отключаем режим подхвата
    }
    return err;
  case CCS_CMD_LOG_COPY:
    err = setValue(id, value, eventType);
    if (value == 1)
      logStartSave(eventType);
    return err;
  case CCS_CMD_LOG_DELETE: case CCS_CMD_SERVICE_LOG_DELETE:
    err = setValue(id, value, eventType);
    if (value)
      logStartDelete(eventType);
    return err;
  case CCS_CMD_UNBLOCK:
    err = setValue(id, 0.0, eventType);
    if (value)
      resetBlock();
    return err;
  case CCS_COEF_TRANSFORMATION:
    err = setValue(id, value, eventType);
    calcSystemInduct();
    vsd->setLimitsMotor();
    vsd->setMotorCurrent(parameters.get(VSD_MOTOR_CURRENT), eventType);
    vsd->setMotorVoltage(parameters.get(VSD_MOTOR_VOLTAGE), eventType);
    calcTransRecommendedTapOff();
    return err;
  case CCS_TRANS_VOLTAGE_SHORT_CIRCUIT:
    err = setValue(id, value, eventType);
    calcSystemInduct();
    return err;
  case CCS_TRANS_NOMINAL_VOLTAGE:           // Номинальное напряжение ТМПН
    err = setValue(id, value, eventType);
    calcTransCoef();
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    return err;
  case CCS_TRANS_NOMINAL_VOLTAGE_INPUT:     // Номинальное напряжение питающей сети
    err = setValue(id, value, eventType);
    calcTransRecommendedTapOff();           // Пересчитываем рекомендуемое напряжение отпайки
    return err;
  case CCS_TRANS_NOMINAL_CURRENT:
    err = setValue(id, value, eventType);
    calcSystemInduct();                     // Пересчитываем индуктивность системы
    return err;
  case CCS_BASE_VOLTAGE:                    // Максимальное рабочее напряжение (базовое)
    err = setValue(id, value, eventType);
    calcTransRecommendedTapOff();           // Пересчитываем рекомендуемое напряжение отпайки
    vsd->calcUfCharacteristicU(value);
    return err;
  case CCS_BASE_FREQUENCY:                  // Максимальная рабочая частота (базовая)
    err = setValue(id, value, eventType);
    if (!err) {
      parameters.set(VSD_HIGH_LIM_SPEED_MOTOR, value, eventType);
      calcTransRecommendedTapOff();
      vsd->calcUfCharacteristicF(value);
    }
    return err;
  case CCS_TRANS_NOMINAL_FREQUENCY_INPUT:
    err = setValue(id, value, eventType);
    calcSystemInduct();
    return err;
  case CCS_TRANS_CABLE_LENGHT:              // Длина кабеля
    err = setValue(id, value, eventType);
    calcTransRecommendedTapOff();           // Пересчитываем рекомендуемое напряжение отпайки
    parameters.set(VSD_DEPTH, value, eventType);       // Записываем в ЧРП
    return err;
  case CCS_TRANS_CABLE_CROSS:               // Сечение кабеля
    err = setValue(id, value, eventType);
    calcTransRecommendedTapOff();           // Пересчитываем рекомендуемое напряжение отпайки
    parameters.set(VSD_TRANS_CABLE_CROSS, value, eventType);
    return err;
  case CCS_TRANS_VOLTAGE_TAP_OFF:           // Напряжение отпайки
    err = setValue(id, value, eventType);
    calcTransCoef();
    parameters.set(VSD_TRANS_VOLTAGE_TAP_OFF, value, eventType); // Задаём в ЧРП напряжение отпайки
    return err;
  case CCS_MOTOR_INDUCTANCE:
    err = setValue(id, value, eventType);
    calcMotorResistPhaseFromInduct();
    calcSystemInduct();
    parameters.set(VSD_MOTOR_INDUCTANCE, value, eventType);      // Задаём индуктивность в ЧРП
    return err;
  case CCS_MOTOR_INDUCTANCE_RESIST_PHASE:
    err = setValue(id, value, eventType);
    calcMotorInductFromResistPhase();
    calcSystemInduct();
    return err;
  case CCS_FILTER_INDUCTANCE:
    err = setValue(id, value, eventType);
    parameters.set(VSD_LF, value, eventType);
    calcSystemInduct();
    return err;
  case CCS_FILTER_CAPACITY:
    err = setValue(id, value, eventType);
    parameters.set(VSD_CF, value, eventType);
    return err;
  case CCS_DATE_TIME_SEC:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_SECOND, value, eventType);
    return err;
  case CCS_DATE_TIME_MIN:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_MINUTE, value, eventType);
    return err;
  case CCS_DATE_TIME_HOUR:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_HOUR, value, eventType);
    return err;
  case CCS_DATE_TIME_DAY:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_DAY, value, eventType);
    return err;
  case CCS_DATE_TIME_MONTH:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_MONTH, value);
    return err;
  case CCS_DATE_TIME_YEAR:
    err = setValue(id, value, eventType);
    parameters.set(VSD_TIME_YEAR, value);
    return err;
  case CCS_CMD_PROT_SUPPLY_OVERVOLTAGE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtSupplyOvervoltageSetpointReset();
    return err;
  case CCS_CMD_PROT_SUPPLY_UNDERVOLTAGE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtSupplyUndervoltageSetpointReset();
    return err;
  case CCS_CMD_PROT_SUPPLY_IMBALANCE_VOLTAGE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtSupplyImbalanceVoltageSetpointReset();
    return err;
  case CCS_CMD_PROT_SUPPLY_IMBALANCE_CURRENT_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtSupplyImbalanceCurrentSetpointReset();
    return err;
  case CCS_CMD_PROT_SUPPLY_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtSupplySetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_OVERLOAD_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorOverloadSetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_CURRENT_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorCurrentSetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_UNDERLOAD_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorUnderloadSetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_IMBALANCE_CURRENT_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorImbalanceCurrentSetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_ASYNC_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorAsyncModeSetpointReset();
    return err;
  case CCS_CMD_PROT_MOTOR_OUT_OF_SYNC_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtMotorOutOfSyncSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_PRESSURE_INTAKE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsPressureIntakeSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_PRESSURE_DISCHARGE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsPressureDischargeSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_TEMPERATURE_MOTOR_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsTemperatureMotorSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_RESISTANCE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsResistanceSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_VIBRATION_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsVibrationSetpointReset();
    return err;
  case CCS_CMD_PROT_DHS_FLOW_DISCHARGE_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDhsFlowDischargeSetpointReset();
    return err;
  case CCS_PROT_OTHER_LIMIT_RESTART_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtOvernumberOfStartSetpointReset();
    return err;
  case CCS_CMD_PROT_OTHER_VSD_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtOtherHardwareVsdSetpointReset();
    return err;
  case CCS_CMD_PROT_OTHER_VSD_NO_CONNECT_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtOtherVsdNoConnectSetpointReset();
    return err;
  case CCS_CMD_PROT_OTHER_OVERHEAT_INPUT_FILTER_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtOtherOverheatInputFilterSetpointReset();
    return err;
  case CCS_CMD_PROT_DI_1_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDigitalInput1SetpointReset();
    return err;
  case CCS_CMD_PROT_DI_2_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDigitalInput2SetpointReset();
    return err;
  case CCS_CMD_PROT_DI_3_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDigitalInput3SetpointReset();
    return err;
  case CCS_CMD_PROT_DI_4_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtDigitalInput4SetpointReset();
    return err;
  case CCS_CMD_PROT_AI_1_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtAnalogInput1SetpointReset();
    return err;
  case CCS_CMD_PROT_AI_2_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtAnalogInput2SetpointReset();
    return err;
  case CCS_CMD_PROT_AI_3_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtAnalogInput3SetpointReset();
    return err;
  case CCS_CMD_PROT_AI_4_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    cmdProtAnalogInput4SetpointReset();
    return err;
  case CCS_CMD_PROT_ALL_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    if (!err) {
      cmdProtSetpointReset();
      logEvent.add(ResetCountsCode, eventType, CounterAllResetId);
    }
    return err;
  case CCS_CMD_COUNTER_ALL_RESET:
    err = setValue(id, value, eventType);
    if (!err) {
      cmdCountersAllReset();
      logEvent.add(ResetCountsCode, eventType, CounterAllResetId);
    }
    return err;
  case CCS_DHS_TYPE:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceDhsId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceDhsId, oldValue, value);
      tms->resetAllDefault();
      startReboot();
    }
    return err;
  case CCS_TYPE_VSD:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceVsdId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceVsdId, oldValue, value);

      resetAllDefault();
      vsd->resetAllDefault();
      setValue(id, value, NoneType);

      startReboot();
    }
    return err;
  case CCS_EM_TYPE:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceEmId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceEmId, oldValue, value);
      em->resetAllDefault();
      startReboot();
    }
    return err;
  case CCS_FILTER_OUTPUT:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value)
        logEvent.add(AddDeviceCode, eventType, AddDeviceFiltOutId, oldValue, value);
      else
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceFiltOutId, oldValue, value);
      parameters.set(VSD_OUT_FILTER, value, eventType);
    }
    return err;
  case CCS_FILTER_INPUT:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value) {
        logEvent.add(AddDeviceCode, eventType, AddDeviceFiltInputId, oldValue, value);
        parameters.set(VSD_DI_33, value * 2 );
      }
      else {
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceFiltInputId, oldValue, value);
        if (!parameters.get(CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_SENSOR)) {     // Выключен датчик в выходном фильтре
          parameters.set(VSD_DI_33, value * 2 );
        }
      }
    }
    return err;
  case CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_SENSOR:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value) {
        parameters.set(VSD_DI_33, value * 2 );
      }
      else {
        if (!parameters.get(CCS_FILTER_INPUT)) {
           parameters.set(VSD_DI_33, value * 2 );
        }
      }
    }
    return err;
  case CCS_SCADA_TYPE:
  case CCS_SCADA_ADDRESS:
  case CCS_SCADA_BYTERATE:
  case CCS_SCADA_PARITY:
  case CCS_SCADA_STOPBIT:
  case CCS_SCADA_DELAY:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err)
      osSemaphoreRelease(scadaSemaphoreId_);
    return err;
  case CCS_COEF_OUT_CURRENT_1:
    err = setValue(id, value, eventType);
    parameters.set(VSD_COEF_OUT_CURRENT_1, value, eventType);
    return err;
  case CCS_COEF_OUT_CURRENT_2:
    err = setValue(id, value, eventType);
    parameters.set(VSD_COEF_OUT_CURRENT_2, value, eventType);
    return err;
  case CCS_COEF_OUT_CURRENT_3:
    err = setValue(id, value, eventType);
    parameters.set(VSD_COEF_OUT_CURRENT_3, value, eventType);
    return err;
  case CCS_CMD_REBOOT_SOFTWARE:
    err = setValue(id, 0.0, eventType);
    if (value && !err) {
      setCmd(CCS_CMD_REBOOT_MASTER);
      startReboot();
    }
    return err;
  case CCS_CMD_UPDATE_SOFTWARE:
    err = setValue(id, value, eventType);
    if (value == 1)
      startUpdateSoftware();
    return err;
  case CCS_CMD_START:
    cmdStart(value);
    return ok_r;
  case CCS_CMD_STOP:
    cmdStop(value);
    return ok_r;
  case CCS_CMD_VSD_RESET_SETPOINTS:
    logEvent.add(SetpointResetCode, OperatorType, VsdResetSetpointId);
    return vsd->resetSetpoints();
  case CCS_PROT_OTHER_VSD_NO_CONNECT_MODE:
    err = setValue(id, value, eventType);
    if (!err) {
      if (value)
        return vsd->onProtConnect();
      else
        return vsd->offProtConnect();
    }
    return err;
  case CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY:
    err = setValue(id, value, eventType);
    parameters.set(VSD_PROT_NO_CONNECT_TRIP_DELAY, value);
    return err;
  case CCS_MOTOR_TYPE:
    err = setValue(id, value, eventType);
    if (!err) {
      parameters.set(VSD_MOTOR_TYPE, value, eventType);
      if (value == VSD_MOTOR_TYPE_VENT) {                                       // Переключили на вентильный двигатель
        if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) == Regime::OnAction) {      // Был включен прямой пуск
          parameters.set(CCS_RGM_RUN_DIRECT_MODE, Regime::OffAction);           // Выключаем прямой пуск
        }
        if (parameters.get(CCS_RGM_RUN_SOFT_MODE) == Regime::OnAction) {      // Был включен "мягкий" пуск
          parameters.set(CCS_RGM_RUN_SOFT_MODE, Regime::OffAction);           // Выключаем "мягкий" пуск
        }
      }
    }
    return err;
  case CCS_MOTOR_TYPE_PROFILE_VSD:
    err = setValue(id, value, eventType);
    vsd->setMotorTypeProfile();
    return err;
  case CCS_PROT_DI_1_MODE:
    err = setValue(id, value, eventType);
    if (value != Protection::ProtModeOff) {
      parameters.set(CCS_DI_1_ACTION, DI_ACTION_PROTECTION); // Вкл Действие по сигналу "Защита"
    }
    return err;
  case CCS_PROT_DI_2_MODE:
    err = setValue(id, value, eventType);
    if (value != Protection::ProtModeOff) {
      parameters.set(CCS_DI_2_ACTION, DI_ACTION_PROTECTION); // Вкл Действие по сигналу "Защита"
    }
    return err;
  case CCS_PROT_DI_3_MODE:
    err = setValue(id, value, eventType);
    if (value != Protection::ProtModeOff) {
      parameters.set(CCS_DI_3_ACTION, DI_ACTION_PROTECTION); // Вкл Действие по сигналу "Защита"
    }
    return err;
  case CCS_PROT_DI_4_MODE:
    err = setValue(id, value, eventType);
    if (value != Protection::ProtModeOff)
      parameters.set(CCS_DI_4_ACTION, DI_ACTION_PROTECTION); // Вкл Действие по сигналу "Защита"
    return err;
  case CCS_DI_1_ACTION:
    err = setValue(id, value, eventType);
    if (value != DI_ACTION_PROTECTION)
      parameters.set(CCS_PROT_DI_1_MODE, Protection::ProtModeOff); // Отключаем защиту по цифромому входу
    return err;
  case CCS_DI_2_ACTION:
    err = setValue(id, value, eventType);
    if (value != DI_ACTION_PROTECTION)
      parameters.set(CCS_PROT_DI_2_MODE, Protection::ProtModeOff); // Отключаем защиту по цифромому входу
    return err;
  case CCS_DI_3_ACTION:
    err = setValue(id, value, eventType);
    if (value != DI_ACTION_PROTECTION)
      parameters.set(CCS_PROT_DI_3_MODE, Protection::ProtModeOff); // Отключаем защиту по цифромому входу
    return err;
  case CCS_DI_4_ACTION:
    err = setValue(id, value, eventType);
    if (value != DI_ACTION_PROTECTION)
      parameters.set(CCS_PROT_DI_4_MODE, Protection::ProtModeOff); // Отключаем защиту по цифромому входу
    return err;
  case CCS_AI_1_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI1, value);
    return err;
  case CCS_AI_2_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI2, value);
    return err;
  case CCS_AI_3_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI3, value);
    return err;
  case CCS_AI_4_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI4, value);
    return err;
  case CCS_AI_6_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI6, value);
    return err;
  case CCS_AI_7_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI7, value);
    return err;
  case CCS_AI_8_TYPE:
    err = setValue(id, value, eventType);
    if (!err)
      setModeAnalogInExt(AI8, value);
    return err;
  case CCS_PROT_SUPPLY_RESTART_DELAY:
  case CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY:
  case CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY:
  case CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY:
  case CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_DELAY:
    err = setValue(CCS_PROT_SUPPLY_RESTART_DELAY, value, eventType);
    if (!err) {
      setValue(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY, value, NoneType);
      setValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY, value, NoneType);
      setValue(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY, value, NoneType);
      setValue(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_DELAY, value, NoneType);
    }
    return err;
  case CCS_CMD_DHS_CONNECTION_RESET:
    err = setValue(id, value, NoneType);
    checkConnectDeviceTimer_ = DELAY_CHECK_CONNECT_DEVICE;
    tms->resetConnect();
    logEvent.add(OtherCode, eventType, DhsConnectionCountersResetId);
    return err;
  case CCS_CMD_VSD_CONNECTION_RESET:
    err = setValue(id, value, NoneType);
    checkConnectDeviceTimer_ = DELAY_CHECK_CONNECT_DEVICE;
    vsd->resetConnect();
    logEvent.add(OtherCode, eventType, VsdConnectionCountersResetId);
    return err;
  case CCS_CMD_EM_CONNECTION_RESET:
    err = setValue(id, value, NoneType);
    checkConnectDeviceTimer_ = DELAY_CHECK_CONNECT_DEVICE;
    em->resetConnect();
    logEvent.add(OtherCode, eventType, EmConnectionCountersResetId);
    return err;
  case CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ:
    err = setValue(id, value, eventType);
    if (!err) {
      parameters.setMin(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ, value);
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ) < value) {
        parameters.set(CCS_RGM_RUN_SKIP_RESONANT_END_FREQ, value);
      }
      vsd->onRegimeSkipFreq();
    }
    return err;
  case CCS_RGM_RUN_SKIP_RESONANT_END_FREQ:
    err = setValue(id, value, eventType);
    if (!err) {
      parameters.setMax(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ, value);
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ) > value) {
        parameters.set(CCS_RGM_RUN_SKIP_RESONANT_BEGIN_FREQ, value);
      }
      vsd->onRegimeSkipFreq();
    }
    return err;
  case CCS_RGM_RUN_SKIP_RESONANT_TEMP:
    err = setValue(id, value, eventType);
    if (!err)
      vsd->onRegimeSkipFreq();
    return err;
  case CCS_SW_STARTUP_FREQUENCY_PUSH: case CCS_SW_STARTUP_FREQUENCY_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_FREQUENCY, value);
    return err;
  case CCS_SW_STARTUP_ANGLE_OSC_PUSH: case CCS_SW_STARTUP_ANGLE_OSC_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_ANGLE_OSC, value);
    return err;
  case CCS_SW_STARTUP_OSC_COUNT_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_OSC_COUNT, value);
    return err;
  case CCS_SW_STARTUP_ROTATIONS_PUSH: case CCS_SW_STARTUP_ROTATIONS_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_ROTATIONS, value);
    return err;
  case CCS_SW_STARTUP_U_PULSE_PUSH: case CCS_SW_STARTUP_U_PULSE_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_U_PULSE, value);
    return err;
  case CCS_SW_STARTUP_I_LIM_PUSH: case CCS_SW_STARTUP_I_LIM_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_I_LIM, value);
    return err;
  case CCS_SW_STARTUP_I_LIM_PULSE_PUSH: case CCS_SW_STARTUP_I_LIM_PULSE_SWING:
    err = setValue(id, value, eventType);
    parameters.set(VSD_SW_STARTUP_I_LIM_PULSE, value);
    return err;
  case CCS_DHS_STATE_SENSOR_PRESSURE_INTAKE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsPressureIntakeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsPressureIntakeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_TEMPERATURE_INTAKE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureIntakeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureIntakeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_TEMPERATURE_WINDING:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureWindingSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureWindingSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_TEMPERATURE_MOTOR:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureMotorSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureMotorSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_ACCELERATION_X_INTAKE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationXIntakeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationXIntakeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_ACCELERATION_Y_INTAKE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationYIntakeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationYIntakeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_ACCELERATION_Z_INTAKE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationZIntakeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsAccelerationZIntakeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_PRESSURE_DISCHARGE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsPressureDischargeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsPressureDischargeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_PRESSURE_DISCHARGE_ANNULUS:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsPressureDischargeZTSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsPressureDischargeZTSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_TEMPERATURE_DISCHARGE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureDischargeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsTemperatureDischargeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_FLOW_DISCHARGE:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsFlowDischargeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsFlowDischargeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_DHS_STATE_SENSOR_HOWMIDITY:
    if ((value != oldValue) && (value == 0)) {
      logEvent.add(OtherCode, AutoType, DhsHowmidityDischargeSensorOkId);
    }
    if ((value != oldValue) && (value == 2)) {
      logEvent.add(OtherCode, AutoType, DhsHowmidityDischargeSensorFailId);
    }
    err = setValue(id, value, eventType);
    return err;
  case CCS_EMF_MOTOR:
    err = setValue(id, value, eventType);
    if (!err) {
      parameters.set(VSD_BACK_EMF, value / parameters.get(CCS_COEF_TRANSFORMATION) * 1000);
    }
    return err;                                                                 //
  case CCS_BYPASS_CONTACTORS:
    err = setValue(id, value, NoneType);
    if ((value != oldValue) && !err) {
      if (value) {
        logEvent.add(AddDeviceCode, eventType, AddDeviceBypassContactorsInputId, oldValue, value);
        parameters.set(CCS_BYPASS_CONTACTOR_KM2_CONTROL, value);
      }
      else {
        logEvent.add(RemoveDeviceCode, eventType, RemoveDeviceBypassContactorsInputId, oldValue, value);
        parameters.set(CCS_RGM_RUN_DIRECT_MODE, value);                         // Отключаем прямой пуск если нет контакторов
      }
    }
    return err;
  case CCS_BYPASS_CONTACTOR_KM1_CONTROL:                                        // Контактор ПП
    if ((value) && (getValue(CCS_BYPASS_CONTACTOR_KM2_STATE))) {                // Если включить контактор ПП и включен контактор ЧРП
      err = err_r;                                                              // Выход с ошибкой
    }
    else {                                                                      // Включить контактор ПП и выключен контактор ЧРП или выключить ПП
      err = setValue(id, value, eventType);                                     // Записываем в регистр
    }
    return err;
  case CCS_BYPASS_CONTACTOR_KM2_CONTROL:                                        // Контактор ЧРП
    if ((value) && (getValue(CCS_BYPASS_CONTACTOR_KM1_STATE))) {                // Если включить контактор ЧРП и включен контактор ПП                                                                // Если включаем контактор ЧРП
      err = err_r;
    }
    else {
      err = setValue(id, value, eventType);
    }
    return err;
  case CCS_RGM_RUN_DIRECT_MODE:                                                 // Прямой пуск
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_DIRECT_MODE);
      if (!err) {
        if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_ETALON) {
          err = setValue(id, value, eventType);
          if (!err) {
            parameters.set(VSD_ETALON_DIRECT_RUN_MODE, 1);
          }
        } else {
          err = parameters.set(CCS_BYPASS_CONTACTOR_KM2_CONTROL, !value);
          if (!err) {
            err = setValue(id, value, eventType);
          }
        }
      }
    }
    else {
      if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_ETALON) {
        err = setValue(id, value, eventType);
        if (!err) {
          parameters.set(VSD_ETALON_DIRECT_RUN_MODE, 0);
        }
      } else {
        err = parameters.set(CCS_BYPASS_CONTACTOR_KM2_CONTROL, !value);
        if (!err) {
          err = setValue(id, value, eventType);
        }
      }
    }
    return err;
  case CCS_RGM_RUN_SOFT_MODE:                                                 // "Мягкий" пуск
    if (value != Regime::OffAction) {
      err = offRunModeExcept(CCS_RGM_RUN_SOFT_MODE);
      if (!err) {
        err = setValue(id, value, eventType);
        if (!err) {
          parameters.set(VSD_ETALON_DIRECT_RUN_SOFT_MODE, 1);
        }
      }
    }
    else {
      err = setValue(id, value, eventType);
      if (!err) {
        parameters.set(VSD_ETALON_DIRECT_RUN_SOFT_MODE, 0);
      }
    }
    return err;
  case CCS_SU_NOMINAL_CURRENT:
    err = setValue(id, value, eventType);
    if (!err) {
      if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_DANFOSS) {
        vsd->setLimitsMotor();
        switch ((uint16_t)value) {
        case 160: value = 195; break;
        case 250: value = 286; break;
        case 400: value = 435; break;
        case 630: value = 724; break;
        case 800: value = 880; break;
        default:  break;
        }
        parameters.set(CCS_SU_MAX_CURRENT, value);
      }
    }
    return err;
  case CCS_PROFILE_DEFAULT_SETPOINT:
    err = setValue(id, value, eventType);
    if (!err && (value != oldValue)) {
      osSemaphoreRelease(setProfileDefaultSemaphoreId_);
    }
    return err;
  case CCS_CMD_SAVE_SETPOINT: case CCS_CMD_SAVE_CONFIG_USB:
    err = setValue(id, value, eventType);
    if (!err && value) {
      osSemaphoreRelease(saveConfigSemaphoreId_);
    }
    return err;
  case CCS_CMD_LOAD_SETPOINT: case CCS_CMD_LOAD_CONFIG_USB:
    err = setValue(id, value, eventType);
    if (!err && value) {
      osSemaphoreRelease(loadConfigSemaphoreId_);
    }
    return err;
  case CCS_CMD_ALL_SETPOINT_RESET:
    err = setValue(id, value, eventType);
    if (!err && value) {
      parameters.setAllDefault();
    }
    return err;
  case CCS_NUM_PRODUCTION_CCS_2:
    err = setValue(id, value, NoneType);
    if (!err) {
      logEvent.add(MultiSetpointCode, eventType, (EventId)id,
                   getValue(CCS_NUM_PRODUCTION_CCS_2), getValue(CCS_NUM_PRODUCTION_CCS));
      logDebug.add(WarningMsg, "Ccs::setNewValue() CCS_NUM_PRODUCTION_CCS (%d, %d)",
                   getValueUint32(CCS_NUM_PRODUCTION_CCS_2),
                   getValueUint32(CCS_NUM_PRODUCTION_CCS));
    }
    return err;
  case CCS_NUM_PRODUCTION_SU_2:
    err = setValue(id, value, NoneType);
    if (!err) {
      logEvent.add(MultiSetpointCode, eventType, (EventId)id,
                   getValue(CCS_NUM_PRODUCTION_SU_2), getValue(CCS_NUM_PRODUCTION_SU));
      logDebug.add(WarningMsg, "Ccs::setNewValue() CCS_NUM_PRODUCTION_SU (%d, %d)",
                   getValueUint32(CCS_NUM_PRODUCTION_SU_2),
                   getValueUint32(CCS_NUM_PRODUCTION_SU));
    }
    return err;
  case CCS_IP_ADDRESS_4:
    err = setValue(id, value, NoneType);
    if (!err) {
      logEvent.add(MultiSetpointCode, eventType, (EventId)id,
                   getValue(CCS_IP_ADDRESS_1)*1000+getValue(CCS_IP_ADDRESS_2),
                   getValue(CCS_IP_ADDRESS_3)*1000+getValue(CCS_IP_ADDRESS_4));
      logDebug.add(WarningMsg, "Ccs::setNewValue() CCS_IP_ADDRESS (%f, %f, %f, %f)",
                   getValue(CCS_IP_ADDRESS_1),
                   getValue(CCS_IP_ADDRESS_2),
                   getValue(CCS_IP_ADDRESS_3),
                   getValue(CCS_IP_ADDRESS_4));
    }
    return err;
  case CCS_NETMASK_4:
    err = setValue(id, value, NoneType);
    if (!err) {
      logEvent.add(MultiSetpointCode, eventType, (EventId)id,
                   getValue(CCS_NETMASK_1)*1000+getValue(CCS_NETMASK_2),
                   getValue(CCS_NETMASK_3)*1000+getValue(CCS_NETMASK_4));
      logDebug.add(WarningMsg, "Ccs::setNewValue() CCS_NETMASK (%f, %f, %f, %f)",
                   getValue(CCS_NETMASK_1),
                   getValue(CCS_NETMASK_2),
                   getValue(CCS_NETMASK_3),
                   getValue(CCS_NETMASK_4));
    }
    return err;
  case CCS_DHS_INTERFACE:
    err = setValue(id, value, eventType);
    if (!err) {
      setDhsScadaInterface();
      checkConnectDeviceTimer_ = DELAY_CHECK_CONNECT_DEVICE;
      tms->resetConnect();
    }
    return err;
  case CCS_AI_1_PARAMETER:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      setValue(CCS_AI_1_PARAM_UNITS, 0.0, NoneType);
    }
    return err;
  case CCS_AI_2_PARAMETER:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      setValue(CCS_AI_2_PARAM_UNITS, 0.0, NoneType);
    }
    return err;
  case CCS_AI_3_PARAMETER:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      setValue(CCS_AI_3_PARAM_UNITS, 0.0, NoneType);
    }
    return err;
  case CCS_AI_4_PARAMETER:
    err = setValue(id, value, eventType);
    if ((value != oldValue) && !err) {
      setValue(CCS_AI_4_PARAM_UNITS, 0.0, NoneType);
    }
    return err;

  default:
    return setValue(id, value, eventType);
  }

  return err;
}

uint8_t Ccs::setNewValue(uint16_t id, uint32_t value, EventType eventType)
{
  uint8_t err = ok_r;
  switch (id) {
  case CCS_DATE_TIME:
    {
      time_t time = value;
      tm dateTime = *localtime(&time);
      err = setNewValue(CCS_DATE_TIME_SEC, dateTime.tm_sec, NoneType);
      if (err == ok_r)
        err = setNewValue(CCS_DATE_TIME_MIN, dateTime.tm_min, NoneType);
      if (err == ok_r)
        err = setNewValue(CCS_DATE_TIME_HOUR, dateTime.tm_hour, NoneType);
      if (err == ok_r)
        err = setNewValue(CCS_DATE_TIME_DAY, dateTime.tm_mday, NoneType);
      if (err == ok_r)
        err = setNewValue(CCS_DATE_TIME_MONTH, dateTime.tm_mon + 1, NoneType);
      if (err == ok_r)
        err = setNewValue(CCS_DATE_TIME_YEAR, 1900 + dateTime.tm_year, NoneType);
      if (err == ok_r)
        rtcSetTime(&time);
      if (err != ok_r)
        return err;
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
  // Питание не в норме
  if (!isPowerGood()) {
    // При первом определении питания не в норме
    // Формируем запись работы от ИБП
    if (!powerOffTimeout_)
      logEvent.add(PowerCode, AutoType, WorkUpsId);

    // Если питания нет в течении заданного числа мс
    // Выключаем подсветку дисплея для экономии
    if (powerOffTimeout_ == TIMEOUT_LCD_OFF/DELAY_MAIN_TASK) {
      offLcd();
    }

    // Если питания нет в течении заданного числа мс
    // Посылаем команду на выключение AM335
    if (powerOffTimeout_ == TIMEOUT_MASTER_OFF/DELAY_MAIN_TASK) {
      setCmd(CCS_CMD_AM335_POWER_OFF);
    }

    // Если питания нет в течении заданного числа мс или нет(не работает) ИБП
    if ((powerOffTimeout_ == TIMEOUT_SLAVE_OFF/DELAY_MAIN_TASK) || !isUpsGood()) {
      // Если не посылали команду на выключение
      if (!powerOffFlag_) {
        offLcd();
        setCmd(CCS_CMD_AM335_POWER_OFF);

        // Запись в журнал "Отключение питания"
        logEvent.add(PowerCode, AutoType, PowerOffId);
        logDebug.add(CriticalMsg, "*** Power Off ***");
        powerOffFlag_ = true;
      }
    }

    if (powerOffTimeout_ == TIMEOUT_POWER_OFF/DELAY_MAIN_TASK) {
      turnPowerBattery(false);
    }
    powerOffTimeout_++;
  }
  // Питание в норме
  else {

    resetCmd(CCS_CMD_AM335_POWER_OFF);

    if ((powerOffTimeout_ > TIMEOUT_LCD_OFF/DELAY_MAIN_TASK) || powerOffFlag_) {
      onLcd();
    }

    if (powerOffTimeout_ > TIMEOUT_MASTER_OFF/DELAY_MAIN_TASK) {
      resetAm335x();
    }

    powerOffFlag_ = false;
    powerOffTimeout_ = 0;
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

    if (vsd->log() && vsd->log()->isConnect())
      setNewValue(CCS_VSD_LOG_CONNECTION, 1, NoneType);
    else
      setNewValue(CCS_VSD_LOG_CONNECTION, 0, NoneType);

    if (usbIsReady())
      setNewValue(CCS_USB_CONNECTION, 1);
    else
      setNewValue(CCS_USB_CONNECTION, 0);
  }
  else {
    checkConnectDeviceTimer_--;
  }
}

void Ccs::cmdProtSupplyOvervoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_OVERVOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtSupplyUndervoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtSupplyImbalanceVoltageSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE;
       i <= CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtSupplyImbalanceCurrentSetpointReset()
{
  for (uint16_t i = CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE;
       i <= CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtSupplySetpointReset()
{
  cmdProtSupplyOvervoltageSetpointReset();
  cmdProtSupplyUndervoltageSetpointReset();
  cmdProtSupplyImbalanceVoltageSetpointReset();
  cmdProtSupplyImbalanceCurrentSetpointReset();
}

void Ccs::cmdProtMotorOverloadSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_OVERLOAD_MODE;
       i <=  CCS_PROT_MOTOR_OVERLOAD_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtMotorCurrentSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_CURRENT_MODE;
       i <=  CCS_PROT_MOTOR_CURRENT_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtMotorUnderloadSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_UNDERLOAD_MODE;
       i <= CCS_PROT_MOTOR_UNDERLOAD_PARAMETER; i++) {
    parameters.setDefault(i);
  }
  parameters.setDefault(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_MOTOR_UNDERLOAD_PROGRES_RESTART_FLAG);

}

void Ccs::cmdProtMotorImbalanceCurrentSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE;
       i <= CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtMotorAsyncModeSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_ASYNC_MODE;
       i <= CCS_PROT_MOTOR_ASYNC_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtMotorOutOfSyncSetpointReset()
{
  for (uint16_t i = CCS_PROT_MOTOR_OUT_OF_SYNC_MODE;
       i <= CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtMotorSetpointReset()
{
  cmdProtMotorOverloadSetpointReset();
  cmdProtMotorCurrentSetpointReset();
  cmdProtMotorUnderloadSetpointReset();
  cmdProtMotorImbalanceCurrentSetpointReset();
  cmdProtMotorAsyncModeSetpointReset();
  cmdProtMotorOutOfSyncSetpointReset();
}

void Ccs::cmdProtDhsPressureIntakeSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_PRESSURE_INTAKE_MODE;
       i <= CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtDhsPressureDischargeSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE;
       i <= CCS_PROT_DHS_PRESSURE_DISCHARGE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtDhsTemperatureMotorSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE;
       i <= CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtDhsResistanceSetpointReset()
{
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_MODE);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_PREVENT);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_TRIP_DELAY);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_RESET);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_PARAMETER);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_FLAG);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_STATE);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_TIME);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_COUNT);
  parameters.setDefault(CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME);
  parameters.setDefault(CCS_SOURCE_RESISTANCE_ISOLATION);
  parameters.setDefault(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
  parameters.setDefault(CCS_SHIFT_RESISTANCE_ISOLATION);
  parameters.setDefault(CCS_COEF_RESISTANCE_ISOLATION);
}

void Ccs::cmdProtDhsVibrationSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_VIBRATION_MODE;
       i <= CCS_PROT_DHS_VIBRATION_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtDhsFlowDischargeSetpointReset()
{
  for (uint16_t i = CCS_PROT_DHS_FLOW_DISCHARGE_MODE;
       i <= CCS_PROT_DHS_FLOW_DISCHARGE_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtDhsSetpointReset()
{
  cmdProtDhsPressureIntakeSetpointReset();
  cmdProtDhsPressureDischargeSetpointReset();
  cmdProtDhsTemperatureMotorSetpointReset();
  cmdProtDhsResistanceSetpointReset();
  cmdProtDhsVibrationSetpointReset();
  cmdProtDhsFlowDischargeSetpointReset();
}

void Ccs::cmdProtOvernumberOfStartSetpointReset()
{
  for (uint16_t i = CCS_PROT_OTHER_LIMIT_RESTART_MODE;
       i <= CCS_PROT_OTHER_LIMIT_RESTART_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtOtherHardwareVsdSetpointReset()
{
  for (uint16_t i = CCS_PROT_OTHER_VSD_MODE;
       i <= CCS_PROT_OTHER_VSD_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtOtherVsdNoConnectSetpointReset()
{
  for (uint16_t i = CCS_PROT_OTHER_VSD_NO_CONNECT_MODE;
       i <= CCS_PROT_OTHER_VSD_NO_CONNECT_TRIP_DELAY; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtOtherOverheatInputFilterSetpointReset()
{
  for (uint16_t i = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_MODE;
       i <= CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_PARAMETER; i++) {
    parameters.setDefault(i);
  }
}

void Ccs::cmdProtOtherSetpointReset()
{
  cmdProtOvernumberOfStartSetpointReset();
  cmdProtOtherHardwareVsdSetpointReset();
  cmdProtOtherVsdNoConnectSetpointReset();
  cmdProtOtherOverheatInputFilterSetpointReset();
}

void Ccs::cmdProtDigitalInput1SetpointReset()
{
  for (uint16_t i = CCS_PROT_DI_1_MODE;
       i <= CCS_PROT_DI_1_PARAMETER; i++) {
    parameters.setDefault(i);
  }
  parameters.setDefault(CCS_DI_1_TYPE);
}

void Ccs::cmdProtDigitalInput2SetpointReset()
{
  for (uint16_t i = CCS_PROT_DI_2_MODE;
       i <= CCS_PROT_DI_2_PARAMETER; i++) {
    parameters.setDefault(i);
  }
  parameters.setDefault(CCS_DI_2_TYPE);
}

void Ccs::cmdProtDigitalInput3SetpointReset()
{
  for (uint16_t i = CCS_PROT_DI_3_MODE;
       i <= CCS_PROT_DI_3_PARAMETER; i++) {
    parameters.setDefault(i);
  }
  parameters.setDefault(CCS_DI_3_TYPE);
}

void Ccs::cmdProtDigitalInput4SetpointReset()
{
  for (uint16_t i = CCS_PROT_DI_4_MODE;
       i <= CCS_PROT_DI_4_PARAMETER; i++) {
    parameters.setDefault(i);
  }
  parameters.setDefault(CCS_DI_4_TYPE);
}

void Ccs::cmdProtDigitalSetpointReset()
{
  cmdProtDigitalInput1SetpointReset();
  cmdProtDigitalInput2SetpointReset();
  cmdProtDigitalInput3SetpointReset();
  cmdProtDigitalInput4SetpointReset();
}

void Ccs::cmdProtAnalogInput1SetpointReset()
{
  parameters.setDefault(CCS_PROT_AI_1_MODE);
  parameters.setDefault(CCS_PROT_AI_1_PREVENT);
  parameters.setDefault(CCS_PROT_AI_1_ACTIV_DELAY);
  parameters.setDefault(CCS_PROT_AI_1_TRIP_DELAY);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_LIMIT);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_RESET);
  parameters.setDefault(CCS_PROT_AI_1_TRIP_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_1_PARAMETER);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_FLAG);
  parameters.setDefault(CCS_PROT_AI_1_STATE);
  parameters.setDefault(CCS_PROT_AI_1_TIME);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_COUNT);
  parameters.setDefault(CCS_PROT_AI_1_RESTART_FIRST_TIME);
  parameters.setDefault(CCS_AI_1_TYPE);
  parameters.setDefault(CCS_AI_1_AXIS_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_1_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_1_COEF);
  parameters.setDefault(CCS_AI_1_PARAMETER);
  parameters.setDefault(CCS_AI_1_PARAM_UNITS);
  parameters.setDefault(CCS_AI_1_MINIMUM);
  parameters.setDefault(CCS_AI_1_MAXIMUM);
}

void Ccs::cmdProtAnalogInput2SetpointReset()
{
  parameters.setDefault(CCS_PROT_AI_2_MODE);
  parameters.setDefault(CCS_PROT_AI_2_PREVENT);
  parameters.setDefault(CCS_PROT_AI_2_ACTIV_DELAY);
  parameters.setDefault(CCS_PROT_AI_2_TRIP_DELAY);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_LIMIT);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_RESET);
  parameters.setDefault(CCS_PROT_AI_2_TRIP_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_2_PARAMETER);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_FLAG);
  parameters.setDefault(CCS_PROT_AI_2_STATE);
  parameters.setDefault(CCS_PROT_AI_2_TIME);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_COUNT);
  parameters.setDefault(CCS_PROT_AI_2_RESTART_FIRST_TIME);
  parameters.setDefault(CCS_AI_2_TYPE);
  parameters.setDefault(CCS_AI_2_AXIS_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_2_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_2_COEF);
  parameters.setDefault(CCS_AI_2_PARAMETER);
  parameters.setDefault(CCS_AI_2_PARAM_UNITS);
  parameters.setDefault(CCS_AI_2_MINIMUM);
  parameters.setDefault(CCS_AI_2_MAXIMUM);
}

void Ccs::cmdProtAnalogInput3SetpointReset()
{
  parameters.setDefault(CCS_PROT_AI_3_MODE);
  parameters.setDefault(CCS_PROT_AI_3_PREVENT);
  parameters.setDefault(CCS_PROT_AI_3_ACTIV_DELAY);
  parameters.setDefault(CCS_PROT_AI_3_TRIP_DELAY);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_LIMIT);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_RESET);
  parameters.setDefault(CCS_PROT_AI_3_TRIP_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_3_PARAMETER);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_FLAG);
  parameters.setDefault(CCS_PROT_AI_3_STATE);
  parameters.setDefault(CCS_PROT_AI_3_TIME);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_COUNT);
  parameters.setDefault(CCS_PROT_AI_3_RESTART_FIRST_TIME);
  parameters.setDefault(CCS_AI_3_TYPE);
  parameters.setDefault(CCS_AI_3_AXIS_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_3_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_3_COEF);
  parameters.setDefault(CCS_AI_3_PARAMETER);
  parameters.setDefault(CCS_AI_3_PARAM_UNITS);
  parameters.setDefault(CCS_AI_3_MINIMUM);
  parameters.setDefault(CCS_AI_3_MAXIMUM);
}

void Ccs::cmdProtAnalogInput4SetpointReset()
{
  parameters.setDefault(CCS_PROT_AI_4_MODE);
  parameters.setDefault(CCS_PROT_AI_4_PREVENT);
  parameters.setDefault(CCS_PROT_AI_4_ACTIV_DELAY);
  parameters.setDefault(CCS_PROT_AI_4_TRIP_DELAY);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_DELAY);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_LIMIT);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_RESET);
  parameters.setDefault(CCS_PROT_AI_4_TRIP_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_SETPOINT);
  parameters.setDefault(CCS_PROT_AI_4_PARAMETER);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_FLAG);
  parameters.setDefault(CCS_PROT_AI_4_STATE);
  parameters.setDefault(CCS_PROT_AI_4_TIME);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_COUNT);
  parameters.setDefault(CCS_PROT_AI_4_RESTART_FIRST_TIME);
  parameters.setDefault(CCS_AI_4_TYPE);
  parameters.setDefault(CCS_AI_4_AXIS_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_4_ZERO_SHIFT);
  parameters.setDefault(CCS_AI_4_COEF);
  parameters.setDefault(CCS_AI_4_PARAMETER);
  parameters.setDefault(CCS_AI_4_PARAM_UNITS);
  parameters.setDefault(CCS_AI_4_MINIMUM);
  parameters.setDefault(CCS_AI_4_MAXIMUM);
}

void Ccs::cmdProtAnalogSetpointReset()
{
  cmdProtAnalogInput1SetpointReset();
  cmdProtAnalogInput2SetpointReset();
  cmdProtAnalogInput3SetpointReset();
  cmdProtAnalogInput4SetpointReset();
}

void Ccs::cmdProtSetpointReset()
{
  cmdProtSupplySetpointReset();
  cmdProtMotorSetpointReset();
  cmdProtDhsSetpointReset();
  cmdProtOtherSetpointReset();
  cmdProtDigitalSetpointReset();
  cmdProtAnalogSetpointReset();
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
  case LastReasonStopOperator: case LastReasonStopRemote: case LastReasonStopProgram:
    break;
  default:
    setValue(CCS_PROT_OTHER_COUNT_STOP, getValue(CCS_PROT_OTHER_COUNT_STOP) + 1);
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

  if ((reason == LastReasonRunOperator) || (reason == LastReasonRunRemote)) {
    if (protImbalanceVoltIn.isRestart() || protOverVoltIn.isRestart() ||
        protUnderVoltIn.isRestart())
      setValue(CCS_PROT_VOLTAGE_COUNT_RESTART, getValue(CCS_PROT_VOLTAGE_COUNT_RESTART) + 1);
    if (protOverloadMotor.isRestart())
      setValue(CCS_PROT_OVERLOAD_COUNT_RESTART, getValue(CCS_PROT_OVERLOAD_COUNT_RESTART) + 1);
    if (protUnderloadMotor.isRestart())
      setValue(CCS_PROT_UNDERLOAD_COUNT_RESTART, getValue(CCS_PROT_UNDERLOAD_COUNT_RESTART) + 1);
    if (protImbalanceCurrentMotor.isRestart())
      setValue(CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART, getValue(CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART) + 1);
  }
  if ((reason != LastReasonRunOperator) && (reason != LastReasonRunRemote) &&
      (reason != LastReasonRunProgram)) {
    incRestartCount();
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

void Ccs::startReboot(bool isSaveParameters)
{
  setCmd(CCS_CMD_START_REBOOT_SLAVE);
  logEvent.add(PowerCode, AutoType, RebootSoftwareId);
  if (isSaveParameters) {
    parameters.startSave();
    osDelay(200);
  }
  osSemaphoreRelease(rebootSemaphoreId_);
}

void Ccs::reboot()
{
#if (USE_LOG_WARNING == 1)
  logDebug.add(WarningMsg, "Ccs::reboot() Reboot");
#endif
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
#if (USE_LOG_WARNING == 1)
  logDebug.add(WarningMsg, "Ccs::updateSoftware() Update");
#endif
  osDelay(200);
  if (updateFromUsb()) {
    setValue(CCS_DATE_INSTALL_SW_CCS, getTime(), NoneType);
    setValue(CCS_CMD_UPDATE_SOFTWARE, 2);
    startReboot();
  }
  else {
    resetCmd(CCS_CMD_UPDATE_SOFTWARE);
  }
}

void Ccs::checkConnectMaster()
{
  bool isConnect = novobusSlave.isConnect();
  if (!isConnect && (isConnect != isConnectMaster_)) {
    logDebug.add(CriticalMsg, "Ccs::checkConnectMaster() Master reboot");
    resetAm335x();
  }
  isConnectMaster_ = isConnect;
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

void Ccs::setMaxBaseFrequency()
{
  setMax(CCS_BASE_FREQUENCY, vsd->getMaxBaseFrequency());
}

void Ccs::setError(int error)
{
  setValue(CCS_ERROR_SLAVE, 0.0);
  setValue(CCS_ERROR_SLAVE, error);
}

void Ccs::intRestartCount()
{
  for (int i = 0; i < RESTART_TIME_MAX+1; ++i) {
    restartTime_[i] = 0;
  }
  int restartCount = getValue(CCS_RESTART_COUNT_ALL);
  for (int i = 0; i < restartCount; ++i) {
    restartTime_[i] = getValueUint32(CCS_RESTART_COUNT_FIRST_TIME);
  }
}

int Ccs::getRestartCount()
{
  return getValue(CCS_RESTART_COUNT_ALL);
}

uint32_t Ccs::getRestartTime()
{
  return restartTime_[0];
}

void Ccs::incRestartCount()
{
  int restartCount = getValue(CCS_RESTART_COUNT_ALL);
  if (restartCount < RESTART_TIME_MAX) {
    restartTime_[restartCount] = getTime();
    restartCount++;
    setValue(CCS_RESTART_COUNT_ALL, restartCount);
  } else {
    for (int i = 0; i < RESTART_TIME_MAX; ++i) {
      restartTime_[i] = restartTime_[i+1];
    }
    restartTime_[RESTART_TIME_MAX] = 0;
  }
}

void Ccs::decRestartCount()
{
  int restartCount = getValue(CCS_RESTART_COUNT_ALL);
  if (restartCount) {
    restartCount--;
    setValue(CCS_RESTART_COUNT_ALL, restartCount);
    for (int i = 0; i < restartCount; ++i) {
      restartTime_[i] = restartTime_[i+1];
    }
    restartTime_[restartCount] = 0;
  }
}

void Ccs::resetRestartCount()
{
  int restartCount = getValue(CCS_RESTART_COUNT_ALL);
  restartCount = 0;
  setValue(CCS_RESTART_COUNT_ALL, restartCount);
  for (int i = 0; i < (RESTART_TIME_MAX + 1); ++i) {
    restartTime_[i] = 0;
  }
}

void Ccs::setRelayOutputs()
{
  static PinState valueOld[4] = {PinReset};
  PinState value;

  for (int i = 0; i < 4; ++i) {
    int action = getValue(CCS_RO_1_ACTION + i);
    if (((action == DO_ACTION_STOP) && isBreakOrStopMotor()) ||
        ((action == DO_ACTION_RUN) && isRunOrWorkMotor()) ||
        ((action == DO_ACTION_RESTART) && isRestart()) ||
        ((action == DO_ACTION_BLOCK) && isBlock())) {
      value = PinSet;
    } else {
      value = PinReset;
    }
    if (value != valueOld[i])
      setRelayOutput(DO1 + i, value);
    valueOld[i] = value;
  }

  setRelayOutput(RO5, (PinState)parameters.get(CCS_BYPASS_CONTACTOR_KM1_CONTROL));
  setRelayOutput(RO6, (PinState)!parameters.get(CCS_BYPASS_CONTACTOR_KM2_CONTROL));
}

void Ccs::setDhsScadaInterface()
{
  int dhsInterface = getValue(CCS_DHS_INTERFACE);
  if (dhsInterface) {
    clrPinOut(DHS_SCADA_RS_1_PIN);
    setPinOut(DHS_SCADA_RS_2_PIN);
  } else {
    clrPinOut(DHS_SCADA_RS_1_PIN);
    clrPinOut(DHS_SCADA_RS_2_PIN);
  }
}

void Ccs::indicationTurbineRotation()
{
  if (!isStopMotor()) {
    onLed(TurboLed);
  } else {
    if (getValue(CCS_TURBO_ROTATION_NOW)) {
      toggleLed(TurboLed);
    } else {
      offLed(TurboLed);
    }
  }
}

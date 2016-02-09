#include "protection_power_off.h"
#include "regime_main.h"
#include "protection_main.h"

int timer;
ProtectionPowerOff::ProtectionPowerOff()
  : isInit_(false)
{
  idPrevent_= CCS_PROT_SUPPLY_POWEROFF_PREVENT;
  idRestartFlag_ = CCS_PROT_SUPPLY_POWEROFF_RESTART_FLAG;
  idState_ = CCS_PROT_SUPPLY_POWEROFF_STATE;

  lastReasonRun_ = LastReasonRunApvPower;
  lastReasonStop_ = LastReasonStopNoVoltage;

  protReactEventId_ = PowerOffProtReactId;
  apvEventId_ = PowerOffApvId;
  apvDisabledEventId_ = PowerOffApvDisabledId;

  timer = -1;
}

ProtectionPowerOff::~ProtectionPowerOff()
{

}

void ProtectionPowerOff::processing()
{
  if (timer == -1)
    timer = HAL_GetTick();
  getCurrentParamProt();
  alarm_ = checkAlarm();    // Определяем выполняется ли условие срабатывания защиты
  prevent_ = alarm_;
  automatProtection();
  setCurrentParamProt();
}

void ProtectionPowerOff::getCurrentParamProt()
{
  state_   = ksu.getValue(idState_);
  restart_ = ksu.getValue(idRestartFlag_);
  restartDelay_ = ksu.getValue(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY);
}

void ProtectionPowerOff::setCurrentParamProt()
{
  parameters.set(idState_, state_);
  parameters.set(idRestartFlag_, restart_);
  parameters.set(idPrevent_, prevent_);
}

bool ProtectionPowerOff::checkAlarm()
{
  bool alarm = false;
  if (parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) == ModeOff)
    alarm = Protection::isLowerLimit(50);
  else
    alarm = Protection::isLowerLimit(1);

  if (!isInit_) {
    isInit_ = true;
    alarm = true;
  } else {
    alarm = false;
  }

  return alarm;
}

float ProtectionPowerOff::calcValue()
{
  float value = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_1)) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_2)) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = 0;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = min(min(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE)) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE) / SQRT_3;
  }

  return (value / (nominal / 100.0));
}

void ProtectionPowerOff::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, 1, valueParameter_);
}

void ProtectionPowerOff::processingStateRun()       // Состояние работа
{
  if (ksu.isProgramMode() && (parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction)) {
    if (alarm_ && !protUnderVoltIn.isRestart()) {
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Защиты: Срабатывание --> АПВ (PowerOff)");
#endif
      addEventReactionProt();
      parameters.set(CCS_RESTART_COUNT, 0);
      ksu.setRestart();
      if (ksu.isWorkMotor()) {
        ksu.stop(lastReasonStop_);
      }
      else if (ksu.isStopMotor()) {
        parameters.set(CCS_LAST_STOP_REASON_TMP, lastReasonStop_);
      }
      timer_ = 0;
      restart_ = true;
      state_ = StateStop;
      return;
    }
  }
  restart_ = false;
}

void ProtectionPowerOff::proccessingStateStop()
{
  if (ksu.isWorkMotor()) {
    restart_ = false;
    state_ = StateRun;
  }
  else if (ksu.isStopMotor()) {               // Двигатель - стоп;
    if (ksu.isAutoMode() && !ksu.isBlock()) { // Двигатель - стоп; Режим - авто; Нет блокировки;
      if (restart_) {                         // Двигатель - стоп; Режим - авто; Флаг - АПВ;
        float restartTimer = restartDelay_;
        if (!prevent_) {                      // Параметр защиты в норме
          if (timer_ == 0) {
            timer_ = ksu.getTime();           // Зафиксировали время начала отсёта АПВ
            logDebug.add(DebugMsg, "prot: time restart (PowerOff)");
            ksu.setRestart();
          }
          else {
            restartTimer = restartDelay_ - ksu.getSecFromCurTime(timer_);
            if (restartTimer <= 0) {
              if (!ksu.isPrevent()) {
                restart_ = false;
                ksu.start(lastReasonRun_);
                state_ = StateRun;
              }
            }
          }
        }
        else {
          timer_ = 0;
        }
        if (restartTimer < 0)
          restartTimer = 0;
        parameters.set(CCS_RESTART_TIMER, restartTimer);
      }
    }
    else {
      restart_ = false;
    }
  } else {
    restart_ = false;
  }
}

void ProtectionPowerOff::automatProtection()
{
  switch (state_) {
  case StateRunning:
    state_ = StateRun;
  case StateRun:
    processingStateRun();
    break;
  case StateStop:
    proccessingStateStop();
    break;
  default:
    break;
  }
}

#include "protection_power_off.h"
#include "regime_main.h"
#include "protection_main.h"

#define MIN_VOLTAGE 2  // Процент от номинала

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
}

ProtectionPowerOff::~ProtectionPowerOff()
{

}

void ProtectionPowerOff::processing()
{
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
  alarm = Protection::isLowerLimit(MIN_VOLTAGE);

#if (USE_POWER_OFF == 1)
  alarm = false;
#endif

  return alarm;
}

float ProtectionPowerOff::calcValue()
{
  float value = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_1)) {
    value = parameters.get(CCS_VOLTAGE_PHASE_1);
  }
  float value2 = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_2)) {
    value2 = parameters.get(CCS_VOLTAGE_PHASE_2);
  }
  float value3 = NOM_VOLTAGE;
  if (parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
    value3 = parameters.get(CCS_VOLTAGE_PHASE_3);
  }

  value = min(min(value, value2), value3);

  float nominal = NOM_VOLTAGE;
  if  (parameters.isValidity(CCS_TRANS_NOMINAL_VOLTAGE_INPUT)) {
    nominal = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT) / SQRT_3;
  }

  return (value / (nominal / 100.0));
}

void ProtectionPowerOff::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, MIN_VOLTAGE, valueParameter_);
}

void ProtectionPowerOff::processingStateRun()       // Состояние работа
{
  if (ksu.isRunOrWorkMotor() ||
      (ksu.isProgramMode() && (parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction))) {
    if (alarm_ && !protUnderVoltIn.isRestart()) {
#if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "ProtectionPowerOff::processingStateRun() Trigger -> AR");
#endif
      addEventReactionProt();
      if ((ksu.isProgramMode() && (parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction)) ||
          (ksu.isAutoMode() && (parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) == Protection::ProtModeRestart))) {
        restart_ = true;
        parameters.set(CCS_RESTART_COUNT, 0);
        ksu.setRestart();
      }
      else {
        restart_ = false;
      }

      if (ksu.isRunOrWorkMotor()) {
        if (!restart_)
          ksu.setBlock();
        ksu.stop(lastReasonStop_);
      }
      else if (ksu.isBreakOrStopMotor() && (parameters.get(CCS_LAST_STOP_REASON_TMP) == LastReasonStopProgram)) {
        parameters.set(CCS_LAST_STOP_REASON_TMP, lastReasonStop_);
      }

      timer_ = 0;
      state_ = StateStop;
      return;
    }
  }
  restart_ = false;
}

void ProtectionPowerOff::proccessingStateStop()
{
  if (ksu.isRunOrWorkMotor()) {
    restart_ = false;
    state_ = StateRun;
  }
  else if (ksu.isBreakOrStopMotor()) {               // Двигатель - стоп;
    if (ksu.isAutoMode() && !ksu.isBlock()) { // Двигатель - стоп; Режим - авто; Нет блокировки;
      if (restart_) {                         // Двигатель - стоп; Режим - авто; Флаг - АПВ;
        float restartTimer = restartDelay_;
        if (!prevent_) {                      // Параметр защиты в норме
          if (timer_ == 0) {
            timer_ = ksu.getTime();           // Зафиксировали время начала отсёта АПВ
#if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "ProtectionPowerOff::proccessingStateStop() Time restart");
#endif
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

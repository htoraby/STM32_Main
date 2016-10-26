#include "protection_hardware_vsd.h"

ProtectionHardwareVsd::ProtectionHardwareVsd()
{
  idMode_= CCS_PROT_OTHER_VSD_MODE;
  idPrevent_= CCS_PROT_OTHER_VSD_PREVENT;
  idActivDelay_ = CCS_PROT_OTHER_VSD_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_VSD_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_VSD_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_VSD_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_VSD_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_VSD_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_VSD_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_VSD_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_VSD_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_VSD_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_OTHER_VSD_DELAY_FLAG;
  idState_ = CCS_PROT_OTHER_VSD_STATE;
  idTimer_ = CCS_PROT_OTHER_VSD_TIME;
  idRestartCount_ = CCS_PROT_OTHER_VSD_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_OTHER_VSD_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvHardwareVsd;

  protReactEventId_ = HardwareVsdProtReactId;
  apvEventId_ = HardwareVsdApvId;
  apvDisabledEventId_ = HardwareVsdApvDisabledId;
  protBlockedEventId_ = HardwareVsdProtBlockedId;
}

ProtectionHardwareVsd::~ProtectionHardwareVsd()
{

}

void ProtectionHardwareVsd::processing()
{
  getGeneralSetpointProt();
  getOtherSetpointProt();
  getCurrentParamProt();
  if (!isModeOff() && isProtect()) {
    alarm_ = checkAlarm();
    prevent_ = checkPrevent();
    warning_ = checkWarning();
  }
  else {
    alarm_ = false;
    prevent_ = false;
    warning_ = false;
  }
  delay_ = warning_ ? delay_ : false;
  checkRestartResetCount();                 // Проверяем и сбрасываем количество АПВ если нужно
  automatProtection();                      // Выполняем шаг автомата защиты
  setCurrentParamProt();                    // Сохраняем текущие параметры защиты
  setOtherParamProt();
}

void ProtectionHardwareVsd::getOtherSetpointProt()
{
  activDelay_ = 0.0;
  tripDelay_ = 0.0;
}

bool ProtectionHardwareVsd::checkAlarm()
{
  float alarm = vsd->checkAlarmVsd();
  lastReasonStop_ = (LastReasonStop)(int)alarm;
  protReactEventId_ = (EventId)alarm;
  float alarmOld = parameters.get(CCS_VSD_ALARM_CODE);
  parameters.set(CCS_VSD_ALARM_CODE, alarm);
  if (alarm != VSD_ALARM_NONE) {
    if (alarm != alarmOld)
      logDebug.add(WarningMsg, "VSD: Error № (%d)", (int)alarm);
    return true;
  }
  return false;
}

bool ProtectionHardwareVsd::checkPrevent()
{
  return vsd->checkPreventVsd();
}

bool ProtectionHardwareVsd::checkWarning()
{
  float warning = vsd->checkWarningVsd();
  float warningOld = parameters.get(CCS_VSD_WARNING_CODE);
  parameters.set(CCS_VSD_WARNING_CODE, warning);
  if (warning != VSD_WARNING_NONE) {
    if (warning != warningOld) {
      logEvent.add(ProtectVsdCode, AutoType, (EventId)warning);
      logDebug.add(WarningMsg, "VSD: Warning № (%d)", (int)warning);
    }
    return true;
  }
  return false;
}

void ProtectionHardwareVsd::addEventReactionProt()
{
  logEvent.add(ProtectVsdCode, AutoType, protReactEventId_);
}

void ProtectionHardwareVsd::processingStateRun()
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;
      if (isModeOff()) {                    // Двигатель - работа; Режим - авто; Защита - выкл;
        setStateStop();
      }
      else if (isModeBlock()) {             // Двигатель - работа; Режим - авто; Защита - блок;
        if (alarm_) {
          delay_ = true;
          addEventReactionProt();
          logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
          ksu.setBlock();
          ksu.stop(lastReasonStop_);
          block_ = true;
          setStateStop();
        }
        else {
          if (warning_) {
            delay_ = true;
          }
          setStateRun();
        }
      }
      else if (isModeRestart()) {
        if (alarm_) {
          delay_ = true;
          if (restartCount_ >= restartLimit_) {
            addEventReactionProt();
            logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
            ksu.setBlock();
            ksu.stop(lastReasonStop_);
            block_ = true;
            setStateStop();
          }
          else {
            addEventReactionProt();
            parameters.set(CCS_RESTART_COUNT, restartCount_);
            ksu.setRestart();
            ksu.stop(lastReasonStop_);
            restart_ = true;
            state_ = StateStopping;
          }
        }
        else {
          if (warning_) {
            delay_ = true;
          }
          setStateRun();
        }
      }
      else if (isModeOn()) {
        if (alarm_) {
          delay_ = true;
          addEventReactionProt();
          ksu.resetDelay();
          ksu.stop(lastReasonStop_);
          setStateStop();
        }
        else {
          if (warning_) {
            delay_ = true;
          }
          setStateRun();
        }
      }
    }
    else if (ksu.isManualMode()) {
      if (isModeOff()) {                    // Двигатель - работа; Режим - авто; Защита - выкл;
        setStateStop();
      }
      else {
        if (alarm_) {
          delay_ = true;
          addEventReactionProt();
          logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
          ksu.setBlock();
          ksu.stop(lastReasonStop_);
          block_ = true;
          setStateStop();
        }
        else {
          if (warning_) {
            delay_ = true;
          }
          setStateRun();
        }
      }
    }
    else {
      setStateStop();
    }
  }
  else {
    setStateStop();
  }
}

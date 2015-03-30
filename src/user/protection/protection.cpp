#include "protection.h"
#include "protection_main.h"

Protection::Protection()
  : attempt_(false)
  , delay_(false)
{

}

Protection::~Protection()
{

}

void Protection::processing()
{
  // Получаем уставки защиты
  getSetpointProt();
  // Получаем текущие параметры защиты
  getCurrentParamProt();
  // Определяем выполняется ли условие срабатывания защиты
  alarm_ = checkAlarm();
  delay_ = alarm_ ? delay_ : false;
  // Определяем есть ли запрещаюший параметр
  prevent_ = checkPrevent();
  // Проверяем и сбрасываем количество АПВ если нужно
  checkRestartResetCount();
  // Выполняем шаг автомата защиты
  automatProtection();
  // Сохраняем текущие параметры защиты
  setCurrentParamProt();
}

void Protection::getSetpointProt()
{
  mode_           = ksu.getValue(idMode_);
  activDelay_     = ksu.getValue(idActivDelay_);
  tripDelay_      = ksu.getValue(idTripDelay_);
  restartDelay_   = ksu.getValue(idRestartDelay_);
  restartLimit_   = ksu.getValue(idRestartLimit_);
  restartResetTime_ = ksu.getValue(idRestartResetTime_);
  tripSetpoint_   = ksu.getValue(idTripSetpoint_);
  restartSetpoint_= ksu.getValue(idRestartSetpoint_);
  timerDifStart_  = ksu.getValue(idParam_);
}

void Protection::getCurrentParamProt()
{
  state_                = ksu.getValue(idState_);
  restart_              = ksu.getValue(idRestartFlag_);
  block_                = ksu.getValue(idBlockFlag_);
  restartCount_         = ksu.getValue(idRestartCount_);
  restartFirstTime_     = ksu.getValueUint32(idRestartFirstTime_);
}

void Protection::setCurrentParamProt()
{
  ksu.setValue(idState_, state_);
  ksu.setValue(idRestartFlag_, restart_);
  ksu.setValue(idBlockFlag_, block_);
  ksu.setValue(idRestartCount_, restartCount_);
  ksu.setValue(idRestartFirstTime_, restartFirstTime_);
}

bool Protection::checkAlarm()
{
  return false;
}

bool Protection::isHigherLimit(float setpoint)
{
  valueParameter_ = calcValue();
  // Если текущий параметр больше уставки
  if (valueParameter_ > setpoint)
    return true;
  else
    return false;
}

bool Protection::isLowerLimit(float setpoint)
{
  valueParameter_ = calcValue();
  // Если текущий параметр меньше уставки
  if (valueParameter_ < setpoint)
    return true;
  else
    return false;
}

void Protection::checkRestartResetCount()
{
  // Если разница между текущим временем и временем первого АПВ больше уставки
  // "Время сброса АПВ" и счётчик АПВ не равен 0
  if ((ksu.getSecFromCurTime(restartFirstTime_) >= restartResetTime_) && restartCount_) {
    restartCount_ = 0;                      // Сбрасываем количество АПВ
    restartFirstTime_ = ksu.getTime();          // Фиксируем время сброса
  }
}

bool Protection::checkPrevent()
{
  return false;
}

float Protection::calcValue()
{
  return valueParameter_;
}

void Protection::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactEventId_, valueParameter_, tripSetpoint_,
               parameters.getPhysic(idTripSetpoint_));
}

bool Protection::isModeOff()
{
  if (parameters.getValue(idMode_) == ModeOff)
    return true;
  else
    return false;
}

bool Protection::isModeBlock()
{
  if (parameters.getValue(idMode_) == ModeBlock)
    return true;
  else
    return false;
}

bool Protection::isModeRestart()
{
  if (parameters.getValue(idMode_) == ModeRestart)
    return true;
  else
    return false;
}

bool Protection::isModeOn()
{
  if (parameters.getValue(idMode_) == ModeOn)
    return true;
  else
    return false;
}

void Protection::setStateStop()
{
  timer_ = 0;
  state_ = StateStop;
}

void Protection::setStateRun()
{
  timer_ = 0;
  state_ = StateRun;
}

void Protection::processingStateRunning()
{  
  if (ksu.isWorkMotor()) {
    if (ksu.isAutoMode() || ksu.isManualMode()) {
      if (isModeOff()) {
        state_ = StateStop;
      }
      else if (ksu.getValueUint32(CCS_RUN_TIME) >= activDelay_) {
        logDebug.add(DebugMsg, "ProtActiv");
        setStateRun();
      }
    }
    else {
      state_ = StateStop;
    }
  }
  else {
    state_ = StateStop;
  }
}

void Protection::processingStateRun()       // Состояние работа
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;
      if (isModeOff()) {                    // Двигатель - работа; Режим - авто; Защита - выкл;
        state_ = StateStop;
      }
      else if (isModeBlock()) {             // Двигатель - работа; Режим - авто; Защита - блок;
        if (alarm_) {                       // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме
          if (timer_ == 0) {                // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - начало;
            timer_ = ksu.getTime();             // Зафиксировали время начала задержки срабатывания
            logDebug.add(DebugMsg, "Reaction - Begin");
            delay_ = true;
          }
          else {
            if (ksu.getSecFromCurTime(timer_) >= tripDelay_) {   // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - конец;
              logDebug.add(DebugMsg, "Reaction - Block");
              addEventReactionProt();
              logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
              ksu.setBlock();
              ksu.stop(lastReasonStop_);
              block_ = true;
              state_ = StateStop;
            }
          }
        }
        else {                              // Двигатель - работа; Режим - авто; Защита - блок; Параметр - в норме
          setStateRun();
        }
      }
      else if (isModeRestart()) {           // Двигатель - работа; Режим - авто; Защита - АПВ;
        if (alarm_) {                       // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме
          if (timer_ == 0) {                // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме; Срабатывание - начало;
            timer_ = ksu.getTime();             // Зафиксировали время начала задержки срабатывания
            logDebug.add(DebugMsg, "Reaction - Begin");
            delay_ = true;
          }
          else if (ksu.getSecFromCurTime(timer_) >= tripDelay_) { // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме; Срабатывание - конец;
            if (restartCount_ >= restartLimit_) {
              logDebug.add(DebugMsg, "Reaction - Block");
              addEventReactionProt();
              logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
              ksu.setBlock();
              ksu.stop(lastReasonStop_);
              block_ = true;
              state_ = StateStop;
            }
            else {
              logDebug.add(DebugMsg, "Reaction - Restart");
              addEventReactionProt();
              ksu.setRestart();
              ksu.stop(lastReasonStop_);
              restart_ = true;
              state_ = StateStopping;
            }
          }
        }
        else {                              // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - в норме
          setStateRun();
        }
      }
      else if (isModeOn()) {                // Двигатель - работа; Режим - авто; Защита - Вкл;
        if (alarm_) {                       // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме
          if (timer_ == 0) {                // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме; Срабатывание - начало;
            timer_ = ksu.getTime();             // Зафиксировали время начала задержки срабатывания
            logDebug.add(DebugMsg, "Reaction - Begin");
            delay_ = true;
          }
          else if (timer_ >= tripDelay_) {  // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме; Срабатывание - конец;
            logDebug.add(DebugMsg, "Reaction - Stop");
            addEventReactionProt();
            ksu.resetDelay();
            ksu.stop(lastReasonStop_);
            state_ = StateStop;
          }
        }
        else {                              // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - в норме
          setStateRun();
        }
      }
    }
    else if (ksu.isManualMode()) {          // Двигатель - работа; Режим - ручной;
      if (isModeOff()) {                    // Двигатель - работа; Режим - авто; Защита - выкл;
        state_ = StateStop;
      }
      else {                                // Двигатель - работа; Режим - авто; Защита - вкл;
        if (alarm_) {                       // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме
          if (timer_ == 0) {                // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - начало;
            timer_ = ksu.getTime();             // Зафиксировали время начала задержки срабатывания
            logDebug.add(DebugMsg, "Reaction - Begin");
            delay_ = true;
          }
          else if (ksu.getSecFromCurTime(timer_) >= tripDelay_) {   // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - конец;
            logDebug.add(DebugMsg, "Reaction - Block");
            addEventReactionProt();
            logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
            ksu.setBlock();
            ksu.stop(lastReasonStop_);
            block_ = true;
            state_ = StateStop;
          }
        }
        else {                              // Двигатель - работа; Режим - авто; Защита - блок; Параметр - в норме
          setStateRun();
        }
      }
    }
    else {
      state_ = StateStop;
    }
  }
  else {
    state_ = StateStop;
  }
}

void Protection::proccessingStateStopping()
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;
      if (restart_) {                       // Двигатель - работа; Режим - авто; Защита - Апв
        incRestartCount();
        state_ = StateRunning;
      }
      state_ = StateRunning;                // TODO: А точно?
    }
    else if (ksu.isManualMode()) {          // Двигатель - работа; Режим - ручной;
      if (restart_) {                       // Двигатель - работа; Режим - авто; Защита - Апв
        incRestartCount();
        state_ = StateRunning;
      }
      state_ = StateRunning;                // TODO: А точно?
    }
  }
  else if (ksu.isStopMotor()) {             // Двигатель - стоп;
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {
      state_ = StateStop;
    }
  }
  else {
    logDebug.add(CriticalMsg, "Prot: StateMotor - Unknown!");
  }
}

void Protection::proccessingStateStop()
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;
      if (isModeOff()) {
        restart_ = false;
        restartCount_ = 0;
      }
      else {
        if (restart_) {                     // Двигатель - работа; Режим - авто; Защита - Апв
          incRestartCount();
        }
        state_ = StateRunning;
      }
    }
    else if (ksu.isManualMode()) {          // Двигатель - работа; Режим - ручной;
      if (isModeOff()) {
        restart_ = false;
        restartCount_ = 0;
      }
      else {
        if (restart_) {                       // Двигатель - работа; Режим - авто; Защита - Апв
          incRestartCount();
        }
        state_ = StateRunning;
      }
    }
    else {                                  // Двигатель - работа; Режим - неизвестный
      logDebug.add(CriticalMsg, "Prot: ControlMode - Unknown!");
    }
  }
  else if (ksu.isStopMotor()) {             // Двигатель - стоп;
    if (ksu.isAutoMode() && !ksu.isBlock()) { // Двигатель - стоп; Режим - авто; Нет блокировки;
      if (restart_) {                         // Двигатель - стоп; Режим - авто; Флаг - АПВ;
        if (ksu.getValueUint32(CCS_STOP_TIME) >= restartDelay_) {
          if (timerDifStart_) {               // Защита с отсчётом АПВ после нормализации параметра (ВРП)
            if (!prevent_) {                  // Параметр защиты в норме
              if (timer_ == 0) {              //
                timer_ = ksu.getTime();       // Зафиксировали время начала отсёта АПВ
                logDebug.add(DebugMsg, "Time restart - begin");
                ksu.setRestart();
              }
              else if (ksu.getSecFromCurTime(timer_) >= parameters.getValue(CCS_TIMER_DIFFERENT_START)) {
                if (ksu.isPrevent()) {
                  if (!attempt_) {                // Первая попытка запуска по АПВ
                    attempt_ = true;
                    addEventProtectionPrevent();  // Сообщение неудачной попытке пуска
                  }
                }
                else {
                  incRestartCount();
                  logEvent.add(ProtectCode, AutoType, apvEventId_);
                  ksu.start(lastReasonRun_);
                  state_ = StateRunning;
                }
              }
            }
            else {
              timer_ = 0;
            }
          }
          else {
            if (ksu.isPrevent() || prevent_) {// Есть запрещающий параметр
              if (!attempt_) {                // Первая попытка АПВ
                attempt_ = true;
                addEventProtectionPrevent();  // Сообщение неудачной попытке пуска
              }
            }
            else {
              incRestartCount();
              logEvent.add(ProtectCode, AutoType, apvEventId_);
              ksu.start(lastReasonRun_);
              state_ = StateRunning;
            }
          }
        }
      }
    }
    else {
      if (isModeOff()) {
        restartCount_ = 0;
      }
      restart_ = false;
    }
  }
  else {
    logDebug.add(CriticalMsg, "Prot: StateMotor - Unknown!");
  }

  if (block_ && !ksu.isBlock()) {
    block_ = false;
    restartCount_ = 0;
  }

  delay_ = false;
}

void Protection::incRestartCount()
{
  restartCount_++;                    // Увеличиваем счётчик АПВ
  if (restartCount_ == 1) {           // Первое АПВ
    restartFirstTime_ = ksu.getTime();   // Запоминаем время первого АПВ по защите
  }
  attempt_ = false;
  restart_ = false;
}

void Protection::automatProtection()
{
  switch (state_) {
  case StateRunning:
    processingStateRunning();
    break;
  case StateRun:
    processingStateRun();
    break;
  case StateStopping:
    proccessingStateStopping();
    break;
  case StateStop:
    proccessingStateStop();
    break;
  default:
    break;
  }
}

#include "protection.h"

Protection::Protection()
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
  // Определяем есть ли блокирующий параметр
  block_ = checkBlock();
  // Выполняем шаг автомата защиты
  automatProtection();
  // Сохраняем текущие параметры защиты
  setCurrentParamProt();
}

void Protection::getSetpointProt()
{
  reaction_       = ksu.getValue(idReaction_);
  activDelay_     = ksu.getValue(idActivDelay_);
  tripDelay_      = ksu.getValue(idTripDelay_);
  restartDelay_   = ksu.getValue(idRestartDelay_);
  restartLimit_   = ksu.getValue(idRestartLimit_);
  restartReset_   = ksu.getValue(idRestartReset_);
  tripSetpoint_   = ksu.getValue(idTripSetpoint_);
  restartSetpoint_= ksu.getValue(idRestartSetpoint_);
  param_          = ksu.getValue(idParam_);
  param2_         = ksu.getValue(idParam2_);
}

void Protection::getCurrentParamProt()
{
  state_                = ksu.getValue(idState_);
  timer_                = ksu.getValue(idTimer_);
  restartCount_         = ksu.getValue(idRestartCount_);
  restartResetCount_    = ksu.getValue(idRestartResetCount_);
}

void Protection::setCurrentParamProt()
{
  ksu.setValue(idState_, state_);
  ksu.setValue(idTimer_, timer_);
  ksu.setValue(idRestartCount_, restartCount_);
  ksu.setValue(idRestartResetCount_, restartResetCount_);
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
  // Проверяем если уставка времени сброса больше чем разница между текущим
  // временем и временем первого АПВ
  if (restartReset_ >= (ksu.getTime() - restartResetCount_)) {
    // Сбрасываем количество АПВ
    restartCount_ = 0;
    // Фиксируем время сброса
    restartResetCount_ = ksu.getTime();
  }
}

bool Protection::checkBlock()
{
  return false;
}

float Protection::calcValue()
{
  return valueParameter_;
}

void Protection::addEventReactionProt()
{
  logEvent.add(ProtectCode, AutoType, protReactionEventId_, valueParameter_, tripSetpoint_,
               parameters.getPhysic(idTripSetpoint_));
}

bool Protection::isReactionOff()
{
  if (parameters.getValue(idReaction_) == ProtReactionOff)
    return true;
  else
    return false;
}

bool Protection::isReactionBlock()
{
  if (parameters.getValue(idReaction_) == ProtReactionBlock)
    return true;
  else
    return false;
}

bool Protection::isReactionRestart()
{
  if (parameters.getValue(idReaction_) == ProtReactionRestart)
    return true;
  else
    return false;
}

bool Protection::isReactionOn()
{
  if (parameters.getValue(idReaction_) == ProtReactionOn)
    return true;
  else
    return false;
}


void Protection::processingStateIdle()
{
  if (ksu.isWorkMotor()) {

  }
  else {
    state_ = ProtStateIdle;
  }
}

void Protection::processingStateRunning()   // Задержка активации
{
  if (ksu.isWorkMotor()) {

  }
  else {
    state_ = ProtStateIdle;
  }
}

void Protection::processingStateRun()       // Состояние работа
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;
      if (isReactionOff()) {                // Двигатель - работа; Режим - авто; Защита - выкл;
        state_ = ProtStateIdle;
      }
      else {
        if (isReactionBlock()) {            // Двигатель - работа; Режим - авто; Защита - блок;
          if (alarm_) {                     // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме
            if (timer_ == 0) {              // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - начало;
              ksu.setDelayCCS();
              logDebug.add(DebugMsg, "Reaction - Begin");
              state_ = ProtStateRun;
            }
            else {
              if (timer_ >= tripDelay_) {   // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - конец;
                timer_ = 0;
                ksu.setBlockCCS();
                vsd->stop();
                logDebug.add(DebugMsg, "Reaction - Block");
                state_ = ProtStateStop;
              }
              else {                        // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - процесс;
                state_ = ProtStateRun;
              }
            }
          }
          else {                            // Двигатель - работа; Режим - авто; Защита - блок; Параметр - в норме
            timer_ = 0;
            state_ = ProtStateRun;
          }
        }
        else {
          if (isReactionRestart()) {        // Двигатель - работа; Режим - авто; Защита - АПВ;
            if (alarm_) {                   // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме
              if (timer_ == 0) {            // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме; Срабатывание - начало;
                ksu.setDelayCCS();
                logDebug.add(DebugMsg, "Reaction - Begin");
                state_ = ProtStateRun;
              }
              else {
                if (timer_ >= tripDelay_) { // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме; Срабатывание - конец;
                  timer_ = 0;
                  vsd->stop();
                  logDebug.add(DebugMsg, "Reaction - Block");
                  state_ = ProtStateStopping;
                }
                else {                      // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - не в норме; Срабатывание - процесс;
                  state_ = ProtStateRun;
                }
              }
            }
            else {                          // Двигатель - работа; Режим - авто; Защита - АПВ; Параметр - в норме
              timer_ = 0;
              state_ = ProtStateRun;
            }
          }
          else {
            if (isReactionOn()) {           // Двигатель - работа; Режим - авто; Защита - Вкл;
              if (alarm_) {                 // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме
                if (timer_ == 0) {          // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме; Срабатывание - начало;
                  ksu.setDelayCCS();
                  logDebug.add(DebugMsg, "Reaction - Begin");
                  state_ = ProtStateRun;
                }
                else {
                  if (timer_ >= tripDelay_) {// Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме; Срабатывание - конец;
                    timer_ = 0;
                    vsd->stop();
                    logDebug.add(DebugMsg, "Reaction - Block");
                    state_ = ProtStateStopping;
                  }
                  else {                    // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - не в норме; Срабатывание - процесс;
                    state_ = ProtStateRun;
                  }
                }
              }
              else {                        // Двигатель - работа; Режим - авто; Защита - Вкл; Параметр - в норме
                timer_ = 0;
                state_ = ProtStateRun;
              }
            }
          }
        }
      }
    }
    else {
      if (ksu.isManualMode()) {             // Двигатель - работа; Режим - ручной;
        if (isReactionOff()) {              // Двигатель - работа; Режим - авто; Защита - выкл;
          state_ = ProtStateIdle;
        }
        else {                              // Двигатель - работа; Режим - авто; Защита - вкл;
          if (alarm_) {                     // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме
            if (timer_ == 0) {              // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - начало;
              ksu.setDelayCCS();
              logDebug.add(DebugMsg, "Reaction - Begin");
              state_ = ProtStateRun;
            }
            else {
              if (timer_ >= tripDelay_) {   // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - конец;
                timer_ = 0;
                ksu.setBlockCCS();
                vsd->stop();
                logDebug.add(DebugMsg, "Reaction - Block");
                state_ = ProtStateStop;
              }
              else {                        // Двигатель - работа; Режим - авто; Защита - блок; Параметр - не в норме; Срабатывание - процесс;
                state_ = ProtStateRun;
              }
            }
          }
          else {                            // Двигатель - работа; Режим - авто; Защита - блок; Параметр - в норме
            timer_ = 0;
            state_ = ProtStateRun;
          }
        }
      }
      else {
        timer_ = 0;
        state_ = ProtStateIdle;
      }
    }
  }
  else {
    timer_ = 0;
    state_ = ProtStateIdle;
  }
}

void Protection::proccessingStateStopping()
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;

    }
    else {
      if (ksu.isManualMode()) {             // Двигатель - работа; Режим - ручной;

      }
      else {
        if (ksu.isStopMode()) {             // Двигатель - работа; Режим - стоп;

        }
        else {                              // Двигатель - работа; Режим - неизвестный

        }
      }
    }
  }
  else {
    if (ksu.isStopMotor()) {                // Двигатель - стоп;
      if (ksu.isAutoMode()) {               // Двигатель - стоп; Режим - авто;

      }
      else {
        if (ksu.isManualMode()) {           // Двигатель - стоп; Режим - ручной;

        }
        else {
          if (ksu.isStopMode()) {           // Двигатель - стоп; Режим - стоп;

          }
          else {

          }
        }
      }
    }
    else {                                  // Двигатель - неизвестно;
      if (ksu.isAutoMode()) {               // Двигатель - неизвестно; Режим - авто;

      }
      else {
        if (ksu.isManualMode()) {           // Двигатель - неизвестно; Режим - ручной;

        }
        else {
          if (ksu.isStopMode()) {           // Двигатель - неизвестно; Режим - стоп;

          }
          else {                            // Двигатель - неизвестно; Режим - неизвестный

          }
        }
      }
    }
  }
}

void Protection::proccessingStateStop()
{
  if (ksu.isWorkMotor()) {                  // Двигатель - работа;
    if (ksu.isAutoMode()) {                 // Двигатель - работа; Режим - авто;

    }
    else {
      if (ksu.isManualMode()) {             // Двигатель - работа; Режим - ручной;

      }
      else {
        if (ksu.isStopMode()) {             // Двигатель - работа; Режим - стоп;

        }
        else {                              // Двигатель - работа; Режим - неизвестный

        }
      }
    }
  }
  else {
    if (ksu.isStopMotor()) {                // Двигатель - стоп;
      if (ksu.isAutoMode()) {               // Двигатель - стоп; Режим - авто;

      }
      else {
        if (ksu.isManualMode()) {           // Двигатель - стоп; Режим - ручной;

        }
        else {
          if (ksu.isStopMode()) {           // Двигатель - стоп; Режим - стоп;

          }
          else {

          }
        }
      }
    }
    else {                                  // Двигатель - неизвестно;
      if (ksu.isAutoMode()) {               // Двигатель - неизвестно; Режим - авто;

      }
      else {
        if (ksu.isManualMode()) {           // Двигатель - неизвестно; Режим - ручной;

        }
        else {
          if (ksu.isStopMode()) {           // Двигатель - неизвестно; Режим - стоп;

          }
          else {                            // Двигатель - неизвестно; Режим - неизвестный

          }
        }
      }
    }
  }
}


void Protection::automatProtection()
{
  switch (state_) {
  case ProtStateIdle:
    processingStateIdle();
    break;
  case ProtStateRunning:
    processingStateRunning();
    break;
  case ProtStateRun:
    processingStateRun();
    break;
  case ProtStateStopping:
    proccessingStateStopping();
    break;
  case ProtStateStop:
    proccessingStateStop();
    break;
  default:
    break;
  }
}

/*
void Protection::processingStateIdle()
{
  if (ksu.isWorkMotor()) {
    if (ksu.isBlockCCS()) {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          timer_ = 0;
          state_ = ProtStateIdle;
        }
        else {
          timer_ = 0;
          state_ = ProtStateIdle;
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isStopMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
    }
    else
    {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          timer_ = 0;
          state_ = ProtStateActiv;
        }
        else {
          timer_ = 0;
          state_ = ProtStateActiv;
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateActiv;
          }
          else {
            timer_ = 0;
            state_ = ProtStateActiv;
          }
        }
        else {
          if (ksu.isStopMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
    }
  }
  else {
    if (ksu.isStopMotor()) {
      if (ksu.isBlockCCS()) {
        if (ksu.isAutoMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isManualMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            if (ksu.isStopMode()) {
              if (reaction_) {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
        }
      }
      else
      {
        if (ksu.isAutoMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isManualMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            if (ksu.isStopMode()) {
              if (reaction_) {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
        }
      }
    }
    else {
      timer_ = 0;
      state_ = ProtStateIdle;
    }
  }
}

void Protection::processingStateActiv()
{
  if (ksu.isWorkMotor()) {
    if (ksu.isBlockCCS()) {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          timer_ = 0;
          state_ = ProtStateIdle;
        }
        else {
          timer_ = 0;
          state_ = ProtStateIdle;
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isStopMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
    }
    else
    {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          if (ksu.getValue(CCS_RUN_DATE_TIME) >= activDelay_) {
            logDebug.add(DebugMsg, "ProtectionStateWork");
            timer_ = 0;
            state_ = ProtStateWork;
          }
          else {
            timer_ = 0;
            state_ = ProtStateActiv;
          }
        }
        else {
          if (ksu.getValue(CCS_RUN_DATE_TIME) >= activDelay_) {
            logDebug.add(DebugMsg, "ProtectionStateWork");
            timer_ = 0;
            state_ = ProtStateWork;
          }
          else {
            timer_ = 0;
            state_ = ProtStateActiv;
          }
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            if (ksu.getValue(CCS_RUN_DATE_TIME) >= activDelay_) {
              logDebug.add(DebugMsg, "ProtectionStateWork");
              timer_ = 0;
              state_ = ProtStateWork;
            }
            else {
              timer_ = 0;
              state_ = ProtStateActiv;
            }
          }
          else {
            if (ksu.getValue(CCS_RUN_DATE_TIME) >= activDelay_) {
              logDebug.add(DebugMsg, "ProtectionStateWork");
              timer_ = 0;
              state_ = ProtStateWork;
            }
            else {
              timer_ = 0;
              state_ = ProtStateActiv;
            }
          }
        }
        else {
          if (ksu.isStopMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
    }
  }
  else {
    if (ksu.isStopMotor()) {
      if (ksu.isBlockCCS()) {
        if (ksu.isAutoMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isManualMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            if (ksu.isStopMode()) {
              if (reaction_) {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
        }
      }
      else
      {
        if (ksu.isAutoMode()) {
          if (reaction_) {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {
          if (ksu.isManualMode()) {
            if (reaction_) {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {
            if (ksu.isStopMode()) {
              if (reaction_) {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
            else {
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
        }
      }
    }
    else {                                  // Неизвестное состояние
      timer_ = 0;
      state_ = ProtStateIdle;
    }
  }
}

void Protection::processingStateWork()
{
  if (ksu.isWorkMotor()) {                  // Работа
    if (ksu.isBlockCCS()) {                 // Работа и Блок
      if (ksu.isAutoMode()) {               // Работа и Блок и Авто
        if (reaction_) {                    // Работа и Блок и Авто и Вкл
          if (alarm_) {                     // Работа и Блок и Авто и Вкл и Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {                            // Работа и Блок и Авто и Вкл и Не Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
        else {                              // Работа и Блок и Авто и Выкл
          if (alarm_) {                     // Работа и Блок и Авто и Выкл и Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {                            // Работа и Блок и Авто и Выкл и Не Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
      else {                                // Работа и Блок и Не Авто
        if (ksu.isManualMode()) {           // Работа и Блок и Ручной
          if (reaction_) {                  // Работа и Блок и Ручной и Вкл
            if (alarm_) {                   // Работа и Блок и Ручной и Вкл и Авария
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {                          // Работа и Блок и Ручной и Вкл и Не Авария
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
          else {                            // Работа и Блок и Ручной и Выкл
            if (alarm_) {                   // Работа и Блок и Ручной и Выкл и Авария
              timer_ = 0;
              state_ = ProtStateIdle;
            }
            else {                          // Работа и Блок и Ручной и Выкл и Не Авария
              timer_ = 0;
              state_ = ProtStateIdle;
            }
          }
        }
        else {                              // Работа и Блок и Не Авто и не Ручной
          if (ksu.isStopMode()) {           // Работа и Блок и Стоп
            if (reaction_) {                // Работа и Блок и Стоп и Вкл
              if (alarm_) {                 // Работа и Блок и Стоп и Вкл и Авария
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {                        // Работа и Блок и Стоп и Вкл и Не Авария
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
            else {                          // Работа и Блок и Стоп и Выкл
              if (alarm_) {                 // Работа и Блок и Стоп и Выкл и Авария
                timer_ = 0;
                state_ = ProtStateIdle;
              }
              else {                        // Работа и Блок и Стоп и Выкл и Не Авария
                timer_ = 0;
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Работа и Блок и Неизвестный режим
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
    }
    else                                    // Работа и Не блок
    {
      if (ksu.isAutoMode()) {               // Работа и Не блок и Авто
        if (reaction_) {                    // Работа и Не блок и Авто и Вкл
          if (alarm_) {                     // Работа и Не блок и Авто и Вкл и Авария
            if (timer_ == 0) {
              ksu.setDelayCCS();
              logDebug.add(DebugMsg, "BeginReaction");
            }
            else {
              if (timer_ >= tripDelay_) {
                if (reaction_ == ProtReactionBlock) {
                  timer_ = 0;
                  ksu.setBlockCCS();
                  vsd->stop();
                  logDebug.add(DebugMsg, "Reaction");
                  state_ = ProtStateReaction;
                }
                else {
                  if (reaction_ == ProtReactionRestart) {
                    if (restartCount_ >= restartLimit_) {
                      timer_ = 0;
                      ksu.setBlockCCS();
                      vsd->stop();
                      logDebug.add(DebugMsg, "Reaction");
                      state_ = ProtStateReaction;
                    }
                    else {
                      timer_ = 0;
                      vsd->stop();
                      logDebug.add(DebugMsg, "Reaction");
                      state_ = ProtStateReaction;
                    }
                  }
                  else {
                    if (reaction_ == ProtReactionOn) {
                      timer_ = 0;
                      vsd->stop();
                      logDebug.add(DebugMsg, "Reaction");
                      state_ = ProtStateReaction;
                    }
                    else {
                      timer_ = 0;
                      state_ = ProtStateWork;
                    }
                  }
                }
              }
              else {
                state_ = ProtStateWork;
              }
            }
          }
          else {                            // Работа и Не блок и Авто и Вкл и Не Авария
            ksu.resetDelayCCS();
            timer_ = 0;
            state_ = ProtStateWork;
          }
        }
        else {                              // Работа и Не блок и Авто и Выкл
          if (alarm_) {                     // Работа и Не блок и Авто и Выкл и Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
          else {                            // Работа и Не блок и Авто и Выкл и Не Авария
            timer_ = 0;
            state_ = ProtStateIdle;
          }
        }
      }
      else {
        if (ksu.isManualMode()) {           // Работа и Не блок и Ручной
          if (reaction_) {                  // Работа и Не блок и Ручной и Вкл
            if (alarm_) {                   // Работа и Не блок и Ручной и Вкл и Авария
              state_ = ProtStateReaction;
            }
            else {                          // Работа и Не блок и Ручной и Вкл и Не Авария
              state_ = ProtStateWork;
            }
          }
          else {                            // Работа и Не блок и Ручной и Выкл
            if (alarm_) {                   // Работа и Не блок и Ручной и Выкл и Авария
              state_ = ProtStateWork;
            }
            else {                          // Работа и Не блок и Ручной и Выкл и Не Авария
              state_ = ProtStateWork;
            }
          }
        }
        else {                              // Работа и Не блок и Не Авто и Не Ручной
          if (ksu.isStopMode()) {           // Работа и Не блок и Стоп
            if (reaction_) {                // Работа и Не блок и Стоп и Вкл
              if (alarm_) {                 // Работа и Не блок и Стоп и Вкл и Авария
                state_ = ProtStateIdle;
              }
              else {                        // Работа и Не блок и Стоп и Вкл и Не Авария
                state_ = ProtStateIdle;
              }
            }
            else {                          // Работа и Не блок и Стоп и Выкл
              if (alarm_) {                 // Работа и Не блок и Стоп и Выкл и Авария
                state_ = ProtStateIdle;
              }
              else {                        // Работа и Не блок и Стоп и Выкл и Не Авария
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Работа и Не блок и Не Авто и Не Ручной и Не Ручной
            state_ = ProtStateIdle;
          }
        }
      }
    }
  }
  else {                                    // Не Работа
    if (ksu.isStopMotor()) {                // Стоп
      if (ksu.isBlockCCS()) {               // Стоп и Блок
        if (ksu.isAutoMode()) {             // Стоп и Блок и Авто
          if (reaction_) {                  // Стоп и Блок и Авто и Вкл
            if (alarm_) {                   // Стоп и Блок и Авто и Вкл и Авария
              state_ = ProtStateIdle;
            }
            else {                          // Стоп и Блок и Авто и Вкл и Не Авария
              state_ = ProtStateIdle;
            }
          }
          else {                            // Стоп и Блок и Авто и Выкл
            if (alarm_) {                   // Стоп и Блок и Авто и Выкл и Авария
              state_ = ProtStateIdle;
            }
            else {                          // Стоп и Блок и Авто и Выкл и Не Авария
              state_ = ProtStateIdle;
            }
          }
        }
        else {                              // Стоп и Блок и Не Авто
          if (ksu.isManualMode()) {         // Стоп и Блок и Ручной
            if (reaction_) {                // Стоп и Блок и Ручной и Вкл
              if (alarm_) {                 // Стоп и Блок и Ручной и Вкл и Авария
                state_ = ProtStateIdle;
              }
              else {                        // Стоп и Блок и Ручной и Вкл и Не Авария
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Блок и Ручной и Выкл
              if (alarm_) {                 // Стоп и Блок и Ручной и Выкл и Авария
                state_ = ProtStateIdle;
              }
              else {                        // Стоп и Блок и Ручной и Выкл и Не Авария
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Стоп и Блок и Не Авто и Не Ручной
            if (ksu.isStopMode()) {         // Стоп и Блок и Останов
              if (reaction_) {              // Стоп и Блок и Останов и Вкл
                if (alarm_) {               // Стоп и Блок и Останов и Вкл и Авария
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Блок и Останов и Вкл и Не Авария
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Блок и Останов и Выкл
                if (alarm_) {               // Стоп и Блок и Останов и Выкл и Авария
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Блок и Останов и Выкл и Не Авария
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Стоп и Блок и Не Авто и Не Ручной и не Останов
              if (reaction_) {              // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл
                if (alarm_) {               // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл и Авария
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл и Не Авария
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл
                if (alarm_) {               // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл и Авария
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл и Не Авария
                  state_ = ProtStateIdle;
                }
              }
            }
          }
        }
      }
      else {                                // Стоп и Не Блок
        if (ksu.isAutoMode()) {             // Стоп и Не Блок
          if (reaction_) {                  // Стоп и Не Блок
            if (alarm_) {                   // Стоп и Не Блок
              state_ = ProtStateIdle;
            }
            else {                          // Стоп и Не Блок
              state_ = ProtStateIdle;
            }
          }
          else {                            // Стоп и Не Блок
            if (alarm_) {                   // Стоп и Не Блок
              state_ = ProtStateIdle;
            }
            else {                          // Стоп и Не Блок
              state_ = ProtStateIdle;
            }
          }
        }
        else {                              // Стоп и Не Блок
          if (ksu.isManualMode()) {         // Стоп и Не Блок
            if (reaction_) {                // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
              else {                        // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
              else {                        // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Стоп и Не Блок
            if (ksu.isStopMode()) {         // Стоп и Не Блок
              if (reaction_) {              // Стоп и Не Блок
                if (alarm_) {               // Стоп и Не Блок
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Не Блок
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Не Блок
                if (alarm_) {               // Стоп и Не Блок
                  state_ = ProtStateIdle;
                }
                else {                      // Стоп и Не Блок
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
              else {                        // Стоп и Не Блок
                state_ = ProtStateIdle;
              }
            }
          }
        }
      }
    }
    else {                                  // Не Стоп
      state_ = ProtStateIdle;
    }
  }
}

void Protection::proccessingStateReactionWait()
{
  if (ksu.isWorkMotor()) {
    if (ksu.isBlockCCS()) {
      state_ = ProtStateIdle;
    }
    else {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          if (alarm_) {
            if (timer_ >= tripDelay_) {
              state_ = ProtStateReaction;
            }
            else {
              state_ = ProtStateReactionWait;
            }
          }
          else {
            state_ = ProtStateWork;
          }
        }
        else {
          state_ = ProtStateIdle;
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            if (alarm_) {
              if (timer_ >= tripDelay_) {
                state_ = ProtStateFailureBegin;
              }
              else {
                state_ = ProtStateReaction;
              }
            }
            else {
              state_ = ProtStateWork;
            }
          }
          else {
            state_ = ProtStateIdle;
          }
        }
        else {
          state_ = ProtStateIdle;
        }
      }
    }
  }
  else {
    state_ = ProtStateIdle;
  }
}

void Protection::proccessingStateReaction()
{
  if (ksu.isWorkMotor()) {
    if (ksu.isBlockCCS()) {
      state_ = ProtStateIdle;
    }
    else {
      if (ksu.isAutoMode()) {
        if (reaction_) {
          if (alarm_) {
            if (reaction_ == ProtReactionBlock) {
              ksu.setBlockCCS();
              vsd->stop();
              state_ = ProtStateFailureWait;
            }
            else {
              if (reaction_ == ProtReactionRestart) {
                if (restartCount_ >= restartLimit_) {
                  ksu.setBlockCCS();
                  vsd->stop();
                  state_ = ProtStateFailureWait;
                }
                else {
                  vsd->stop();
                  state_ = ProtStateFailureWait;
                }
              }
              else {
                if (reaction_ == ProtReactionOn) {
                  vsd->stop();
                  state_ = ProtStateFailureWait;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
          else {
            state_ = ProtStateWork;
          }
        }
        else {
          state_ = ProtStateIdle;
        }
      }
      else {
        if (ksu.isManualMode()) {
          if (reaction_) {
            if (alarm_) {
              ksu.setBlockCCS();
              vsd->stop();
              state_ = ProtStateFailureWait;
            }
            else {
              state_ = ProtStateWork;
            }
          }
          else {
            state_ = ProtStateIdle;
          }
        }
        else {
          state_ = ProtStateIdle;
        }
      }
    }
  }
  else {
    state_ = ProtStateIdle;
  }
}

void Protection::proccessingStateFailureWait()
{
  if (ksu.isWorkMotor()) {
    state_ = ProtStateFailureWait;
  }
  else {
    state_ = ProtStateFailure;
  }
}

void Protection::proccessingStateFailure()
{
  if (ksu.isWorkMotor()) {
    state_ = ProtStateIdle;
  }
  else {
    if (ksu.isAutoMode()) {
      if (reaction_) {
        if (alarm_) {
          if (reaction_ == ProtReactionBlock) {
            ksu.setBlockCCS();
            vsd->stop();
            state_ = ProtStateFailureWait;
          }
          else {
            if (reaction_ == ProtReactionRestart) {
              if (restartCount_ >= restartLimit_) {
                ksu.setBlockCCS();
                vsd->stop();
                state_ = ProtStateFailureWait;
              }
              else {
                vsd->stop();
                state_ = ProtStateFailureWait;
              }
            }
            else {
              if (reaction_ == ProtReactionOn) {
                vsd->stop();
                state_ = ProtStateFailureWait;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
        }
        else {
          state_ = ProtStateWork;
        }
      }
      else {
        state_ = ProtStateIdle;
      }
    }
    else {
      if (ksu.isManualMode()) {
        if (reaction_) {
          if (alarm_) {
            ksu.setBlockCCS();
            vsd->stop();
            state_ = ProtStateFailureWait;
          }
          else {
            state_ = ProtStateWork;
          }
        }
        else {
          state_ = ProtStateIdle;
        }
      }
      else {
        state_ = ProtStateIdle;
      }
    }
  }
}

void Protection::automatProtection()
{
  switch (state_) {
  case ProtStateIdle:
    processingStateIdle();
    break;
  case ProtStateActiv:
    processingStateActiv();
    break;
  case ProtStateWork:
    processingStateWork();
    break;
  case ProtStateReaction:
    proccessingStateReactionWait();
    break;
  case ProtStateReaction:
    proccessingStateReaction();
    break;
  case ProtStateFailureWait:
    proccessingFailureWait();
    break;
  case ProtStateFailure:
    proccessingFailure();
    break;

  default:
    break;
  }
}

void Protection::proccessingStateReaction()
{
  if (ksu.isWorkMotor()) {                  // Работа
    if (ksu.isBlockCCS()) {                 // Работа и Блок
      if (ksu.isAutoMode()) {               // Работа и Блок и Авто
        if (reaction_) {                    // Работа и Блок и Авто и Вкл
          if (alarm_) {                     // Работа и Блок и Авто и Вкл и Авария
            if (timer_ >= tripDelay_) {     // Работа и Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
          else {                            // Работа и Блок и Авто и Вкл и Не Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
        }
        else {                              // Работа и Блок и Авто и Выкл
          if (alarm_) {                     // Работа и Блок и Авто и Выкл и Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
          else {                            // Работа и Блок и Авто и Выкл и Не Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
        }
      }
      else {                                // Работа и Блок и Не Авто
        if (ksu.isManualMode()) {           // Работа и Блок и Ручной
          if (reaction_) {                  // Работа и Блок и Ручной и Вкл
            if (alarm_) {                   // Работа и Блок и Ручной и Вкл и Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Работа и Блок и Ручной и Вкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Работа и Блок и Ручной и Выкл
            if (alarm_) {                   // Работа и Блок и Ручной и Выкл и Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Работа и Блок и Ручной и Выкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
        }
        else {                              // Работа и Блок и Не Авто и не Ручной
          if (ksu.isStopMode()) {           // Работа и Блок и Стоп
            if (reaction_) {                // Работа и Блок и Стоп и Вкл
              if (alarm_) {                 // Работа и Блок и Стоп и Вкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Работа и Блок и Стоп и Вкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Работа и Блок и Стоп и Выкл
              if (alarm_) {                 // Работа и Блок и Стоп и Выкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Работа и Блок и Стоп и Выкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
          else {                            // Работа и Блок и Неизвестный режим
            state_ = ProtStateIdle;
          }
        }
      }
    }
    else                                    // Работа и Не блок
    {
      if (ksu.isAutoMode()) {               // Работа и Не блок и Авто
        if (reaction_) {                    // Работа и Не блок и Авто и Вкл
          if (alarm_) {                     // Работа и Не блок и Авто и Вкл и Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateFailureBegin;
            }
            else {
              state_ = ProtStateReaction;
            }
          }
          else {                            // Работа и Не блок и Авто и Вкл и Не Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateWork;
            }
            else {
              state_ = ProtStateWork;
            }
          }
        }
        else {                              // Работа и Не блок и Авто и Выкл
          if (alarm_) {                     // Работа и Не блок и Авто и Выкл и Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
          else {                            // Работа и Не блок и Авто и Выкл и Не Авария
            if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
              state_ = ProtStateIdle;
            }
            else {
              state_ = ProtStateIdle;
            }
          }
        }
      }
      else {
        if (ksu.isManualMode()) {           // Работа и Не блок и Ручной
          if (reaction_) {                  // Работа и Не блок и Ручной и Вкл
            if (alarm_) {                   // Работа и Не блок и Ручной и Вкл и Авария
              if (timer_ >= tripDelay_) {   // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateFailureBegin;
              }
              else {
                state_ = ProtStateReaction;
              }
            }
            else {                          // Работа и Не блок и Ручной и Вкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateWork;
              }
              else {
                state_ = ProtStateWork;
              }
            }
          }
          else {                            // Работа и Не блок и Ручной и Выкл
            if (alarm_) {                   // Работа и Не блок и Ручной и Выкл и Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Работа и Не блок и Ручной и Выкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
        }
        else {                              // Работа и Не блок и Не Авто и Не Ручной
          if (ksu.isStopMode()) {           // Работа и Не блок и Стоп
            if (reaction_) {                // Работа и Не блок и Стоп и Вкл
              if (alarm_) {                 // Работа и Не блок и Стоп и Вкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Работа и Не блок и Стоп и Вкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Работа и Не блок и Стоп и Выкл
              if (alarm_) {                 // Работа и Не блок и Стоп и Выкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Работа и Не блок и Стоп и Выкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
          else {                            // Работа и Не блок и Не Авто и Не Ручной и Не Ручной
            state_ = ProtStateIdle;
          }
        }
      }
    }
  }
  else {                                    // Не Работа
    if (ksu.isStopMotor()) {                // Стоп
      if (ksu.isBlockCCS()) {               // Стоп и Блок
        if (ksu.isAutoMode()) {             // Стоп и Блок и Авто
          if (reaction_) {                  // Стоп и Блок и Авто и Вкл
            if (alarm_) {                   // Стоп и Блок и Авто и Вкл и Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Блок и Авто и Вкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Стоп и Блок и Авто и Выкл
            if (alarm_) {                   // Стоп и Блок и Авто и Выкл и Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Блок и Авто и Выкл и Не Авария
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
        }
        else {                              // Стоп и Блок и Не Авто
          if (ksu.isManualMode()) {         // Стоп и Блок и Ручной
            if (reaction_) {                // Стоп и Блок и Ручной и Вкл
              if (alarm_) {                 // Стоп и Блок и Ручной и Вкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Блок и Ручной и Вкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Стоп и Блок и Ручной и Выкл
              if (alarm_) {                 // Стоп и Блок и Ручной и Выкл и Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Блок и Ручной и Выкл и Не Авария
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
          else {                            // Стоп и Блок и Не Авто и Не Ручной
            if (ksu.isStopMode()) {         // Стоп и Блок и Останов
              if (reaction_) {              // Стоп и Блок и Останов и Вкл
                if (alarm_) {               // Стоп и Блок и Останов и Вкл и Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Блок и Останов и Вкл и Не Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
              else {                        // Стоп и Блок и Останов и Выкл
                if (alarm_) {               // Стоп и Блок и Останов и Выкл и Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Блок и Останов и Выкл и Не Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
            }
            else {                          // Стоп и Блок и Не Авто и Не Ручной и не Останов
              if (reaction_) {              // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл
                if (alarm_) {               // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл и Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Блок и Не Авто и Не Ручной и не Останов и Вкл и Не Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
              else {                        // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл
                if (alarm_) {               // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл и Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Блок и Не Авто и Не Ручной и не Останов и Выкл и Не Авария
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
            }
          }
        }
      }
      else {                                // Стоп и Не Блок
        if (ksu.isAutoMode()) {             // Стоп и Не Блок
          if (reaction_) {                  // Стоп и Не Блок
            if (alarm_) {                   // Стоп и Не Блок
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Не Блок
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
          else {                            // Стоп и Не Блок
            if (alarm_) {                   // Стоп и Не Блок
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
            else {                          // Стоп и Не Блок
              if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                state_ = ProtStateIdle;
              }
              else {
                state_ = ProtStateIdle;
              }
            }
          }
        }
        else {                              // Стоп и Не Блок
          if (ksu.isManualMode()) {         // Стоп и Не Блок
            if (reaction_) {                // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
            else {                          // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
          else {                            // Стоп и Не Блок
            if (ksu.isStopMode()) {         // Стоп и Не Блок
              if (reaction_) {              // Стоп и Не Блок
                if (alarm_) {               // Стоп и Не Блок
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Не Блок
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
              else {                        // Стоп и Не Блок
                if (alarm_) {               // Стоп и Не Блок
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
                else {                      // Стоп и Не Блок
                  if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                    state_ = ProtStateIdle;
                  }
                  else {
                    state_ = ProtStateIdle;
                  }
                }
              }
            }
            else {                          // Стоп и Не Блок
              if (alarm_) {                 // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
              else {                        // Стоп и Не Блок
                if (timer_ >= tripDelay_) {     // Работа и Не Блок и Авто и Вкл и Авария и Время
                  state_ = ProtStateIdle;
                }
                else {
                  state_ = ProtStateIdle;
                }
              }
            }
          }
        }
      }
    }
    else {                                  // Не Стоп
      state_ = ProtStateIdle;
    }
  }
}
*/

/*
void Protection::automatProtection1()
{
  if ((mode_ == 0) && (state_ != ProtStateRestartWait)) {
    state_ = ProtStateIdle;
    return;
  }

  switch (state_) {

  // Состояние "ничегонеделанья"
  case ProtStateIdle:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Остаёмся в этом же состоянии
      state_ = ProtStateIdle;
    }
    // Станция в работе
    else {
      // Если станция в работе
      if (ksu.isWorkMotor()) {
        // Переход на активацию
        state_ = ProtStateActivBegin;
      }
    }
    break;

    // Состояние начала ожидания активации
  case ProtStateActivBegin:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    // Станция в работе
    else {
      // Фиксируем текущее время
      timer_ = ksu.getTime();
      // Переходим в состояние ожидания активации защиты
      state_ = ProtStateActivWait;
    }
    break;

    // Состояние ожидания активации
  case ProtStateActivWait:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    // Станция в работе
    else {
      // Если задано время активации и время активации истекло
      if ((activDelay_) && (activDelay_ >= (ksu.getTime() - timer_))) {
        // Переходим в состояние активированно
        state_ =  ProtStateActiv;
      }
      else {
        // Остаёмся в этом состоянии
        state_ = ProtStateActivWait;
      }
    }
    break;

    // Состояние активации
  case ProtStateActiv:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    else {
      // TODO: Message Активирована защита
      state_ = ProtStateWork;
    }
    break;

    // Состояние работы защиты
  case ProtStateWork:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    else {
      // Если контролируемый параметр не в норме
      if (alarm_) {
        // Переходим в задержку срабатывания
        state_ = ProtStateReactionBegin;
      }
      else {
        // Продолжаем работать
        state_ = ProtStateWork;
      }
    }
    break;

    // Состояние начала задержки срабатывания
  case  ProtStateReactionBegin:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    else {
      // Если контролируемый параметр не в норме
      if (alarm_) {
        // Фиксируем текущее время
        timer_ = ksu.getTime();
        // Переходим в состояние ожидания срабатывания
        state_ = ProtStateReactionWait;
      }
      else {
        // Продолжаем работать
        state_ = ProtStateWork;
      }
    }

    // Состояние ожидания задержки срабатывания
  case  ProtStateReactionWait:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    // Станция в работе
    else {
      // Если контролируемый параметр не в норме
      if (alarm_) {
        // Если задано время срабатывания и время срабатывания истекло
        if ((tripDelay_) && (tripDelay_ >= (ksu.getTime() - timer_))) {
          // Переходим в состояние сработало
          state_ =  ProtStateReaction;
        }
        else {
          // Остаёмся в этом состоянии
          state_ = ProtStateReactionWait;
        }
      }
      else {
        // Продолжаем работать
        state_ = ProtStateWork;
      }
    }
    break;

    // Состояние срабатывания
  case  ProtStateReaction:
    // Если станция остановлена
    if (ksu.isStopMotor()) {
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtStateIdle;
    }
    else {
      // Если контролируемый параметр не в норме
      if (alarm_) {
        // TODO: Message Срабатывания защиты
        addEventReactionProt();
        state_ = ProtStateFailure;
      }
      else {
        state_ = ProtStateWork;
      }
    }
    break;

    // Состояние формирования команды останова
  case ProtStateFailureBegin:
    // TODO: Message Команда останов
    state_ = ProtStateFailureWait;
    break;

    // Состояние ожидания останова
  case  ProtStateFailureWait:
    // Переходим в состояние остановились, принимаем дальнейшие решения
    state_ = ProtStateFailure;
    break;


    // Состояние принятия решения дальнейших действий
  case  ProtStateFailure:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else {
      // Если действие защиты блокировка или превышено количество АПВ
      if ((reaction_ == ProtReactionBlock) || (restartCount_ >= restartLimit_)) {
        state_ =  ProtStateBlock;
      }
      else
        state_ = ProtStateRestartBegin;
    }
    break;


    // Состояние начала отсчёта АПВ
    // В это состояние попадаем если разрешены АПВ по защите
  case  ProtStateRestartBegin:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {
        // TODO: Message Пуск
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
      else {
        // Фиксируем текущее время
        timer_ = ksu.getTime();
        state_ = ProtStateRestartWait;
      }
    }
    break;

    // Состояние ожидания АПВ
    // Здесь происходит отсчёт времени таймера АПВ
  case  ProtStateRestartWait:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || !ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {
        // TODO: Message Пуск
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
      else {
        // Если задано время АПВ и время АПВ истекло
        if ((restartDelay_) && (restartReset_ >= (ksu.getTime() - timer_))) {
          // Переходим в состояние проверки блокирующего параметра
          state_ =  ProtStateRestartBlock;
        }
        // Время АПВ не истекло, остаёмся в этом состоянии
        else {
          state_ = ProtStateRestartWait;
        }
      }
    }
    break;

    // Состояние проверки блокирующего параметра
  case ProtStateRestartBlock:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || !ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {
        // TODO: Message Пуск
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
      else {
        // Если есть блокирующий параметр
        if (block_) {
          // TODO: Message АПВ запрещен
          logEvent.add(ProtectCode, AutoType, apvDisabledEventId_);
          state_ = ProtStateRestartBlockWait;
        }
        else {
          state_ = ProtStateRestart;
        }
      }
    }
    break;

    // Состояние ожидания пока исчезнет блокирующий параметр
  case  ProtStateRestartBlockWait:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || !ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if (ksu.isWorkMotor() && ksu.isAutoMode()) {
        // TODO: Message Пуск
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
      else {
        // Если есть блокирующий параметр
        if (block_) {
          // Остаёмся в этом состоянии
          state_ = ProtStateRestartBlockWait;
        }
        else {
          state_ = ProtStateRestart;
        }
      }
    }
    break;

    // Состояние АПВ
  case  ProtStateRestart:
    // Если СУ в блокировке или не автоматическом режиме
    if (ksu.isBlockCCS() || !ksu.isAutoMode()) {
      // Переходим в состояние блокировка
      state_ = ProtStateBlock;
    }
    else
    {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.isWorkMotor()) && (!ksu.isAutoMode())) {
        // TODO: Message Пуск
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
      else {
        // TODO: Message Пуск по АПВ
        logEvent.add(ProtectCode, AutoType, apvEventId_);
        // Переходим в состояние активации защиты
        state_ = ProtStateActivBegin;
      }
    }
    break;

    // Состояние стоп
  case  ProtStateStop:

    break;

    // Состояние блокировка
  case  ProtStateBlock:
    // TODO: Message Блокировка по защите
    logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
    // Переходим в состояние "ничегонеделанья"
    state_ = ProtStateIdle;
    break;

    // Неизвестное состояние
  default:

    break;
  }
}
*/


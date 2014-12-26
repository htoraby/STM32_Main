#include "protection.h"

Protection::Protection()
{

}

Protection::~Protection()
{

}

void Protection::getCurrentParam(int count)
{

}

bool Protection::checkWorkKSU()
{
  float state = KSU.getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUN)||(state == CCS_CONDITION_DELAY))
    return 0;
  else
    return 1;
}

// Универсальный автомат работы защиты
unsigned char Protection::protection()
{

  switch((unsigned char)state_){
  // Состояние "ничегонеделанья"
  case PROTECTION_STATE_OFF:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Переход на активацию
      state_ = PROTECTION_STATE_ACTIV_BEGIN;
    }
    break;

  // Состояние начала ожидания активации
  case PROTECTION_STATE_ACTIV_BEGIN:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // TODO: Фиксируем время начала активации
      state_ = PROTECTION_STATE_ACTIV_WAIT;
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние ожидания активации
  case PROTECTION_STATE_ACTIV_WAIT:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если истёк таймер активации
      if ((delayActiv_) && (delayActiv_ >= timer_)) {
        state_ =  PROTECTION_STATE_ACTIV;
      }
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние активации
  case PROTECTION_STATE_ACTIV:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // TODO: Message Активация защиты
      state_ = PROTECTION_STATE_WORK;
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние работы защиты
  case PROTECTION_STATE_WORK:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если параметр не в норме
      if(flagReaction_) {
        state_ = PROTECTION_STATE_REACTION_BEGIN;
      }
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние начала задержки срабатывания
  case  PROTECTION_STATE_REACTION_BEGIN:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если параметр не в норме
      if (flagReaction_) {
        // TODO: Фиксируем время начала задержки срабатывания
        state_ = PROTECTION_STATE_REACTION_WAIT;
      }
      else {
        state_ = PROTECTION_STATE_WORK;
      }
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние ожидания задержки срабатывания
  case  PROTECTION_STATE_REACTION_WAIT:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если параметр не в норме
      if (flagReaction_) {
        // Если истекла задержка срабатывания
        if ((delayReaction_) && (delayReaction_>= timer_)) {
          state_ = PROTECTION_STATE_REACTION;
        }
      }
      else {
        state_ = PROTECTION_STATE_WORK;
      }
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние срабатывания
  case  PROTECTION_STATE_REACTION:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // TODO: Message Срабатывания защиты
      state_ = PROTECTION_STATE_FAILURE;
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние формирования команды останова
  case PROTECTION_STATE_FAILURE_BEGIN:
    // TODO: Фиксируем время команды останова
    state_ = PROTECTION_STATE_FAILURE_WAIT;
    break;

  // Состояние ожидания останова
  case  PROTECTION_STATE_FAILURE_WAIT:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если истекло время ожидания останова
      if ((delayFailure_) && (delayFailure_>= timer_)) {
        state_ = PROTECTION_STATE_FAILURE_BEGIN;
      }
    }
    else {
      state_ = PROTECTION_STATE_FAILURE;
    }
    break;

  // Состояние принятия решения дальнейших действий
  case  PROTECTION_STATE_FAILURE:
    // Если станция в работе
    if (!checkWorkKSU()) {

    }
    else {
      // TODO: Message Стоп защита
      // Если флаг блокировки или превышено количество АПВ
      if ((flagBlocking_)
          || (numberRestart_ == 0)
          || (numberRestart_ <= countRestart_)) {
        // Переход на состояние блокировки
        state_ = PROTECTION_STATE_BLOCK;
      }
      else {
        state_ = PROTECTION_STATE_RESTART_BEGIN;
      }
    }
    break;

  // Состояние начала отсчёта АПВ
  case  PROTECTION_STATE_RESTART_BEGIN:

    break;

  // Состояние ожидания АПВ
  case  PROTECTION_STATE_RESTART_WAIT:

    break;

  // Состояние АПВ
  case  PROTECTION_STATE_RESTART:

    break;

  // Состояние стоп
  case  PROTECTION_STATE_STOP:

    break;

  // Состояние блокировка
  case  PROTECTION_STATE_BLOCK:
    // TODO: Message Блокировка защита
    break;

  // Неизвестное состояние
  default:

    break;
  }
  return 1;
}


#include "protection_overvoltage_input.h"

ProtectionOvervoltageInput::ProtectionOvervoltageInput()
{

}

ProtectionOvervoltageInput::~ProtectionOvervoltageInput()
{

}

// Метод вычисления контролируемого параметра
float ProtectionOvervoltageInput::calcControlParameter()
{
  return (em.getValue(EM_VOLTAGE_PHASE_1_2)
          + em.getValue(EM_VOLTAGE_PHASE_2_3)
          + em.getValue(EM_VOLTAGE_PHASE_3_1))/3;
}

unsigned char ProtectionOvervoltageInput::protection()
{
  // Текущее состояние защиты
  mode_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_MODE);
  // Текущее действие защиты
  action_= KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_ACTION);
  // Текущее состояние автомата защиты
  state_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_STATE);
  // Уставка задержка активации
  delayActiv_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_ACTIV);
  // Уставка задержка срабатывания
  delayReaction_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_REACTION);
  // Задержка АПВ
  delayRestart_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_DELAY_RESTART);
  // Уставка АПВ
  limitReaction_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_LIMIT_REACTION);
  // Текущее значение таймера
  timer_ = KSU.getValue(CCS_PROT_OVERVOLTAGE_IN_TIMER);
  // Текущее значение контролируемого параметра
  valueParameter_ = calcControlParameter();





  switch ((int)state_) {
  // Состояние "ничегонеделанья"
  case PROTECTION_STATE_OFF:
    // Станция в работе
    if (!checkWorkKSU()) {
      // Переходим в режим активации защиты
      state_ = PROTECTION_STATE_ACTIV;
    }
    break;

  // Состояние начала активации защиты
  case  PROTECTION_STATE_ACTIV:
    // TODO: Зафиксировать время timer_
    state_ = PROTECTION_STATE_ACTIV_WAIT;
    break;

  // Состояние ожидания активации защиты
  case  PROTECTION_STATE_ACTIV_WAIT:
    // Станция в работе
    if (!checkWorkKSU()) {
      if ((delayActiv_) && (timer_ >= delayActiv_)) {
        // TODO: Сообщение активирована защита
        state_ = PROTECTION_STATE_WORK;
      }
    }
    else {
      state_ = PROTECTION_STATE_OFF;
    }
    break;

  // Состояние начала работы
  case PROTECTION_STATE_WORK:
    // Если станция в работе
    if (!checkWorkKSU()) {
      state_ = PROTECTION_STATE_WORK_WAIT;
    }
    else
      state_ = PROTECTION_STATE_OFF;
    break;

  // Состояние работы
  case  PROTECTION_STATE_WORK_WAIT:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // Если авария
      if (flagReaction_) {
        // Переходим в состояние реакция
        state_ = PROTECTION_STATE_REACTION;
      }
    }
    else
      state_ = PROTECTION_STATE_OFF;
    break;

  case  PROTECTION_STATE_REACTION:
    // Если станция в работе
    if (!checkWorkKSU()) {
      // TODO: Сообщение задержка срабатывания защиты
      // TODO: Фиксируем время

    }

    break;
  default:
    break;
  }
}

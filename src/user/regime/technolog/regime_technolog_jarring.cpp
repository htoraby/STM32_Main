#include "regime_technolog_jarring.h"

RegimeTechnologJarring::RegimeTechnologJarring()
{

}

RegimeTechnologJarring::~RegimeTechnologJarring()
{

}

void RegimeTechnologJarring::processing()
{
  // Получение уставок режима
  action_ = parameters.get(CCS_RGM_JARRING_MODE);
  firstFreq_ = parameters.get(CCS_RGM_JARRING_FREQ_1);
  secondFreq_ = parameters.get(CCS_RGM_JARRING_FREQ_2);
  periodJar_ = parameters.get(CCS_RGM_JARRING_PERIOD);
  countJar_ = parameters.get(CCS_RGM_JARRING_COUNT);
  upTemp_ = parameters.get(CCS_RGM_JARRING_UPTEMP);
  downTemp_ = parameters.get(CCS_RGM_JARRING_DOWNTEMP);

  // Получение текущих параметров режима
  state_ = parameters.get(CCS_RGM_JARRING_STATE);
  beginTime_ = parameters.get(CCS_RGM_JARRING_TIMER);

  uint32_t time = 0;

  static float count_ = 0;
//  count_ = parameters.get(CCS_RGM_JARRING_CURRENT_COUNT);

  // Режим выключен
  if (action_ == OffAction)  {
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:                           // Режим не активен

    break;
  case RunningState:                        // Активация таймера
    beginTime_ = ksu.getTime();             // Запоминаем время когда начался отсчёт периода встряхивания
    break;
  case WorkState:                           // Отсчёт таймера
    time = ksu.getSecFromCurTime(beginTime_);
    if ((time > periodJar_) && periodJar_) {// Время паузы истекло
      state_ = WorkState + 1;               // Переход в состояние задания частоты 1
    }
    break;
  case WorkState + 1:                       // Задание настроек режима встряхивания
    // Задание минимума частоты
    vsd->setMinFrequency(max(parameters.get(VSD_HIGH_LIM_SPEED_MOTOR), max(firstFreq_, secondFreq_)));
    // Задание максимума частоты
    vsd->setMaxFrequency(max(parameters.get(VSD_LOW_LIM_SPEED_MOTOR), max(firstFreq_, secondFreq_)));
    // Задание темпа набора частоты
    if (upTemp_)
      vsd->setTimeSpeedUp(parameters.get(CCS_BASE_FREQUENCY) / upTemp_);
    // Задание темпа снижения частот
    if (downTemp_)
      vsd->setTimeSpeedDown(parameters.get(CCS_BASE_FREQUENCY) / downTemp_);
    // Переход к изменению частоты
    state_ = WorkState + 2;
    break;
  case WorkState + 2:                       // Задание частоты 1
    ksu.setFreq(firstFreq_);
    state_ = WorkState + 3;
    break;
  case WorkState + 3:                       // Ожидание перехода на частоту
    if (vsd->checkFreq())
      state_ = WorkState + 4;
    break;
  case WorkState + 4:                       // Выход на частоту 2
    ksu.setFreq(secondFreq_);
    state_ = WorkState + 5;
    break;
  case WorkState + 5:                       // Ожидание перехода на частоту 2
    if (vsd->checkFreq())
      state_ = WorkState + 6;
    break;
  case WorkState + 6:                       // Проверка что выполнено нужное количество встряхиваний
    count_++;
    if (count_ >= countJar_) {
      state_ = WorkState + 7;               // Выход из цикла встряхивания
    }
    else {
      state_ = WorkState + 2;
    }
    break;
  case WorkState + 7:                       // Возвращение настроек и запуск таймера

    state_ = RunningState;
    break;
  default:
    break;
  }
}

void RegimeTechnologJarring::saveBeforeJarring()
{

}

void RegimeTechnologJarring::loadAfterJarring()
{

}


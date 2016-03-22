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
  if (action_ == OffAction)  {              // Выключен режим
    if (state_ != IdleState)                // Режим выключили во время работы
      state_ = WorkState + 7;               // Возвращаем настройки работы установки
  }

  switch (state_) {
  case IdleState:                           // Режим не активен
    if ((action_ != OffAction) && ksu.isWorkMotor() ) {
      state_ = RunningState;
    }
    break;
  case RunningState:                        // Активация таймера
    beginTime_ = ksu.getTime();             // Запоминаем время когда начался отсчёт периода встряхивания
    state_ = WorkState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: начало встряхивания");
#endif
    break;
  case WorkState:                           // Отсчёт таймера
    time = ksu.getSecFromCurTime(beginTime_);
    if ((time > periodJar_) && periodJar_) {// Время паузы истекло
      state_ = WorkState + 1;               // Переход в состояние задания частоты 1
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: начало встряхивания (time = %5.0f, periodJar_ = %5.0f)",
                     time, periodJar_);
#endif
    }
    break;
  case WorkState + 1:                       // Задание настроек режима встряхивания
    saveBeforeJarring();                    // Сохраняем настройки перед включением режима
    configJarring();                        // Задаём настройки режима
    state_ = WorkState + 2;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: задание настроек встряхивания",
                     secondFreq_);
#endif
    break;
  case WorkState + 2:                       // Задание частоты 1
    ksu.setFreq(firstFreq_);
    state_ = WorkState + 3;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: установили частоту f1 (firstFreq_ = %5.0f)",
                     secondFreq_);
#endif
    break;
  case WorkState + 3:                       // Ожидание перехода на частоту
    if (vsd->isSetPointFreq()) {
      state_ = WorkState + 4;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: достигли частоты f1");
#endif
    }
    break;
  case WorkState + 4:                       // Выход на частоту 2
    ksu.setFreq(secondFreq_);
    state_ = WorkState + 5;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: установили частоту f2 (secondFreq_ = %5.0f)",
                     secondFreq_);
#endif
    break;
  case WorkState + 5:                       // Ожидание перехода на частоту 2
    if (vsd->isSetPointFreq()) {
      state_ = WorkState + 6;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: достигли частоты f2");
#endif
    }
    break;
  case WorkState + 6:                       // Проверка что выполнено нужное количество встряхиваний
    count_++;
    if (count_ >= countJar_) {
      state_ = WorkState + 7;               // Выход из цикла встряхивания
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: конец циклов встряхивания (count_ = %5.0f, countJar_ = %5.0f)",
                     count_, countJar_);
#endif
    }
    else {
      state_ = WorkState + 2;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: конец одного цикла встряхивания (count_ = %5.0f, countJar_ = %5.0f)",
                     count_, countJar_);
#endif
    }
    break;
  case WorkState + 7:                       // Возвращение настроек
    loadAfterJarring();
    state_ = IdleState;
#if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Встряхив.: конец встряхивания");
#endif
    break;
  default:
    break;
  }
}

void RegimeTechnologJarring::saveBeforeJarring()
{
  parameters.set(CCS_SAVE_VSD_LOW_LIM_SPEED_MOTOR, parameters.get(VSD_LOW_LIM_SPEED_MOTOR));
  parameters.set(CCS_SAVE_VSD_HIGH_LIM_SPEED_MOTOR, parameters.get(VSD_HIGH_LIM_SPEED_MOTOR));
  parameters.set(CCS_SAVE_VSD_TIMER_DISPERSAL, parameters.get(VSD_TIMER_DISPERSAL));
  parameters.set(CCS_SAVE_VSD_TIMER_DELAY, parameters.get(VSD_TIMER_DELAY));
  parameters.set(CCS_SAVE_VSD_FREQUENCY, parameters.get(VSD_FREQUENCY));
}

void RegimeTechnologJarring::loadAfterJarring()
{
  vsd->setMinFrequency(parameters.get(CCS_SAVE_VSD_LOW_LIM_SPEED_MOTOR));
  vsd->setMaxFrequency(parameters.get(CCS_SAVE_VSD_HIGH_LIM_SPEED_MOTOR));
  vsd->setTimeSpeedUp(parameters.get(CCS_SAVE_VSD_TIMER_DISPERSAL));
  vsd->setTimeSpeedDown(parameters.get(CCS_SAVE_VSD_TIMER_DELAY));
  vsd->setFrequency(parameters.get(CCS_SAVE_VSD_FREQUENCY));
}

void RegimeTechnologJarring::configJarring()
{
  vsd->setMinFrequency(max(parameters.get(VSD_HIGH_LIM_SPEED_MOTOR), max(firstFreq_, secondFreq_)));
  // Задание максимума частоты
  vsd->setMaxFrequency(max(parameters.get(VSD_LOW_LIM_SPEED_MOTOR), max(firstFreq_, secondFreq_)));
  // Задание темпа набора частоты
  if (upTemp_)
    vsd->setTimeSpeedUp(parameters.get(CCS_BASE_FREQUENCY) / upTemp_);
  // Задание темпа снижения частот
  if (downTemp_)
    vsd->setTimeSpeedDown(parameters.get(CCS_BASE_FREQUENCY) / downTemp_);
}


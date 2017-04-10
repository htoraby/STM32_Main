#include "regime_technolog_pumping_gas.h"

RegimeTechnologPumpingGas::RegimeTechnologPumpingGas()
{

}

RegimeTechnologPumpingGas::~RegimeTechnologPumpingGas()
{

}

void RegimeTechnologPumpingGas::processing()
{
  // Получение уставок режима
  action_ = parameters.get(CCS_RGM_PUMP_GAS_MODE);          // Действие режима
  firstFreq_ = parameters.get(CCS_RGM_PUMP_GAS_F1);         // F1 пркч выходная верхняя частота СУ ЧР в режиме прокачки газа
  secondFreq_ = parameters.get(CCS_RGM_PUMP_GAS_F2);        // F2 пркч выходная нижняя частота СУ ЧР в режиме прокачки газа
  time_ = parameters.get(CCS_RGM_PUMP_GAS_TIME);            // T пркч время работы на частоте прокачки в режиме прокачки газа
  cicle_ = parameters.get(CCS_RGM_PUMP_GAS_CICLE);          // Количество циклов прокачки газа

  // Получение скртытых уставок режима
  activDelay_ = parameters.get(CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY);           // Время активации режима = времени активации защиты по недогрузу
  protTripSetpoint_ = parameters.get(CCS_PROT_MOTOR_UNDERLOAD_TRIP_CALC);       // Уставка срабатывания защиты по недогрузу
  incTripSetpoint_ = parameters.get(CCS_RGM_PUMP_GAS_UNDERLOAD);                // Прибавка к уставке срабатывания защиты по недогрузу
  tripDelay_ = parameters.get(CCS_RGM_PUMP_GAS_DELAY);                          // Задержка на включение режима (защита от ложных срабатываний)

  // Получение текущих параметров режима
  state_ = parameters.get(CCS_RGM_PUMP_GAS_STATE);                              // Состояние автомата
  tripTime_ = parameters.getU32(CCS_RGM_PUMP_GAS_POINT_DELAY);                  // Время начала недогруза
  beginTime_ = parameters.getU32(CCS_RGM_PUMP_GAS_POINT_TIME);                  // Время начала T пркч
  countCicle_ = parameters.get(CCS_RGM_PUMP_GAS_COUNT_CICLE);                   // Текущее количество циклов прокачки газа

  int16_t err = 0;

  // Если режим выключили или остановили двигатель во время работы режима
  if ((action_ == OffAction) || ksu.isBreakOrStopMotor()) {
    if (state_ != IdleState) {
      state_ = StopState;                   // Cостояние выхода из режима
    }
  }

  switch (state_) {
  case IdleState:                           // Пустое состояние
    if ((action_ != OffAction) &&           // Режим включен
        ksu.isRunOrWorkMotor() &&           // Двигатель в работе
        (parameters.get(CCS_RUN_TIME) >= activDelay_)) {  // Время активации режима истекло
      tripTime_ = 0;                        // Сброс переменной времени срабатывания
      state_ = RunningState;                // Переход на состояние ожидания недогруза
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "PumpingGas::processing() Idle -> Running (state_=%d, action_=%d, CCS_RUN_TIME=%d, activDelay_=%d)",
                   (int)state_, (int)action_, (int)parameters.get(CCS_RUN_TIME), (int)activDelay_);
      #endif
    }
    break;
  case RunningState:                        // Состояние ожидания недогруза
    if (isNeedPumping()) {                  // Если недогруз
      if (tripTime_ == 0) {                 // Первый раз определили недогруз
        parameters.set(CCS_RGM_PUMP_GAS_SAVE_SETPOINT_FREQ, parameters.get(VSD_FREQUENCY));
        tripTime_ = ksu.getTime();          // Запоминаем время недогруза  
      }
      else if (ksu.getSecFromCurTime(tripTime_) >= tripDelay_) {
        if (countCicle_ >= cicle_) {        // Сделали необходимое количество циклов
          state_ = PauseState;              // Переходим на состояние ожидания останова
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "PumpingGas::processing() Running -> Pause (state_=%d, action_=%d, tripTime=%d, tripDelay_=d, countCicle_=%d, cicle_=%d)",
                       (int)state_, (int)action_, ksu.getSecFromCurTime(tripTime_), (int)tripDelay_, (int)countCicle_, (int)cicle_);
          #endif
        }
        else {
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "PumpingGas::processing() Running -> Work (state_=%d, action_=%d, tripTime=%d, tripDelay_=%d, countCicle_=%d, cicle_=%d)",
                       (int)state_, (int)action_, ksu.getSecFromCurTime(tripTime_), (int)tripDelay_, (int)countCicle_, (int)cicle_);
          #endif
          state_ = WorkState;             // Переходим на состояние прокачки
        }
      }
    }
    else {
      countCicle_ = 0;
      tripTime_ = 0;    
    }
    break;
  case WorkState:                           // Состояние задания F1 пркч
    err = setF1();                          // Пытаемся задать частоту F1 пркч
    if (!err) {                             // Задали частоту F1 пркч 
      if (vsd->isSetPointFreq() && (parameters.get(VSD_FREQUENCY_NOW) == firstFreq_)) {          // Если вышли на частоту F1 пркч
        if (beginTime_ == 0) {              // Если первый раз определили что вышли на частоту F1 пркч
          beginTime_ = ksu.getTime();       // Запоминаем время выхода на F1 пркч
          logEvent.add(OtherCode, AutoType, RgmPumpingGasF1Id);
        }
        else {                              // Работаем на частоте прокачки
          if (ksu.getSecFromCurTime(beginTime_) >= time_) { 
            state_ = WorkState + 1;         // Переходим на сосотояние задания F2 пркч
            #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "PumpingGas::processing() Work -> Work + 1 (state_=%d, action_=%d, beginTime_=%d, time_=%d)",
                         (int)state_, (int)action_, ksu.getSecFromCurTime(beginTime_), (int)time_);
            #endif
          }
        }
      }
      else {
        beginTime_ = 0;  
      }
    }
    else if (err > 0) {                     // Не смогли задать F1 пркч
      state_ = WorkState + 1;               // Переходим на состояние задания частоты F1
      #if (USE_LOG_WARNING == 1)
      logDebug.add(WarningMsg, "PumpingGas::processing() Work -> Work + 1 (state_=%d, action_=%d, err=%d)",
                   (int)state_, (int)action_, (int)err);
      #endif
    }
    break;
  case WorkState + 1:                       // Состояние задания F2 пркч
    err = setF2();                          // Пытаемся задать частоту F2 пркч
    if (!err) {                             // Задали частоту F2 пркч
      if (vsd->isSetPointFreq() && (parameters.get(VSD_FREQUENCY_NOW) == secondFreq_)) {          // Если вышли на частоту F2 пркч
        if (beginTime_ == 0) {              // Если первый раз определили что вышли на частоту F1 пркч
          beginTime_ = ksu.getTime();       // Запоминаем время выхода на F2 пркч
          logEvent.add(OtherCode, AutoType, RgmPumpingGasF2Id);
        }
        else {                              // Работаем на частоте прокачки
          if (ksu.getSecFromCurTime(beginTime_) >= time_) {
            state_ = WorkState + 2;         // Переходим на сосотояние возврата частоты
            #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "PumpingGas::processing() Work + 1 -> Work + 2 (state_=%d, action_=%d, beginTime_=%d, time_=%d)",
                         (int)state_, (int)action_, ksu.getSecFromCurTime(beginTime_), (int)time_);
            #endif
          }
        }
      }
      else {
        beginTime_ = 0;
      }
    }
    else if (err > 0) {                     // Не смогли задать F2 пркч
      state_ = WorkState + 2;               // Переходим на состояние возврата частоты уставки
      #if (USE_LOG_WARNING == 1)
      logDebug.add(WarningMsg, "PumpingGas::processing() Work + 1 -> Work + 2 (state_=%d, action_=%d, err=%d)",
                   (int)state_, (int)action_, (int)err);
      #endif
    }
    break;
  case WorkState + 2:                       // Состояние возврата частоты уставки
    err = returnFreq();                     // Пытаемся вернуть частоту уставки
    if (!err) {                             // Задали частоту уставки
      if (vsd->isSetPointFreq() && (parameters.get(VSD_FREQUENCY_NOW) == parameters.get(CCS_RGM_PUMP_GAS_SAVE_SETPOINT_FREQ))) {          // Если вышли на частоту уставки
        tripTime_ = 0;
        countCicle_ ++;
        state_ = RunningState;
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "PumpingGas::processing() Work + 2 -> Running (state_=%d, action_=%d, countCicle_=%d)",
                     (int)state_, (int)action_, (int)countCicle_);
        #endif
      }
    }
    else if (err > 0) {                     // Не смогли вернуть частоту уставки
      tripTime_ = 0;
      countCicle_ ++;
      state_ = RunningState;
      #if (USE_LOG_WARNING == 1)
      logDebug.add(WarningMsg, "PumpingGas::processing() Work + 2 -> Running (state_=%d, action_=%d, countCicle_=%d, err=%d)",
                   (int)state_, (int)action_, (int)countCicle_, (int)err);
      #endif
    }
    break;
  case PauseState:
    if (ksu.isBreakOrStopMotor()) {
      state_ = StopState;
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "PumpingGas::processing() Pause -> Stop (state_=%d, action_=%d)",
                   (int)state_, (int)action_);
      #endif
    }
    break;
  case StopState:
    countCicle_ = 0;
    tripTime_ = 0;
    beginTime_ = 0;
    state_ = IdleState;
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "PumpingGas::processing() Stop -> Idle (state_=%d, action_=%d)",
                 (int)state_, (int)action_);
    #endif
    break;
  default:
    state_ = IdleState;
    #if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "PumpingGas::processing() default -> Idle (state_=%d, action_=%d)",
                 (int)state_, (int)action_);
    #endif
    break;
  }
  parameters.set(CCS_RGM_PUMP_GAS_STATE, state_);
  parameters.set(CCS_RGM_PUMP_GAS_POINT_DELAY, tripTime_);
  parameters.set(CCS_RGM_PUMP_GAS_POINT_TIME, beginTime_);
  parameters.set(CCS_RGM_PUMP_GAS_COUNT_CICLE, countCicle_);
}

int16_t RegimeTechnologPumpingGas::setF1()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, firstFreq_);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeTechnologPumpingGas::setF1()");
#endif
  }
  return err;
}

int16_t RegimeTechnologPumpingGas::setF2()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, secondFreq_);
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeTechnologPumpingGas::setF2()");
#endif
  }
  return err;
}

int16_t RegimeTechnologPumpingGas::returnFreq()
{
  int16_t err = setConfirmation(VSD_FREQUENCY, parameters.get(CCS_RGM_PUMP_GAS_SAVE_SETPOINT_FREQ));
  if (err == err_r) {
#if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "RegimeTechnologPumpingGas::returnFreq()");
#endif
  }
  return err;
}

int16_t RegimeTechnologPumpingGas::setConfirmation(uint16_t id, float value)
{
  int16_t err = -1;
  if ((parameters.getValidity(id) == ok_r)
      && (parameters.get(id) == value)) {
    repeat_ = 0;
    delay_ = 0;
    err = 0;
  }
  else {
    delay_ --;
    if (delay_ <= 0) {
      if (repeat_ >= 3) {
        repeat_ = 0;
        delay_ = 0;
        err = 1;
      }
      else {
        parameters.set(id, value, NoneType);
        delay_ = 5;
        repeat_ ++;
      }
    }
  }
  return err;
}

bool RegimeTechnologPumpingGas::isNeedPumping()
{
  float value = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  float value2 = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  float value3 = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  float nominal = parameters.get(VSD_MOTOR_CURRENT);

  value = min(min(value, value2), value3);

  float cos = parameters.get(CCS_MOTOR_COS_PHI_NOW);
  float nominalCos = parameters.get(VSD_MOTOR_COS_PHI);

  float load = (value * cos) / (nominal * nominalCos) * 100.0;

  if (load < (protTripSetpoint_ + incTripSetpoint_))
    return true;
  else
    return false;
}


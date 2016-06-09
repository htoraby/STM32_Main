#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);    // Активность режима
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);    // Состояние автомата режима
  freq_ = parameters.get(CCS_RGM_RUN_PUSH_FREQ);      // Частота толчка
  quantity_ = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);  // Количество толчков
  voltage_ = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE);// Напряжение толчка
  time_ = (1 / freq_) * 100;                          // Время толчка (счетчик * 100мс)

  setPointFreq_ = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_FREQ);
  setPointLowLimFreq_ = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_LOW_LIM_FREQ);
  setPointU1_ = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U1);
  setPointU2_ = parameters.get(CCS_RGM_RUN_PUSH_SETPOINT_U2);
}

void RegimeRunPush::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_PUSH_STATE, state_);
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_FREQ, setPointFreq_);
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_LOW_LIM_FREQ, setPointLowLimFreq_);
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U1, setPointU1_);
  parameters.set(CCS_RGM_RUN_PUSH_SETPOINT_U2, setPointU2_);
}

void RegimeRunPush::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
//  if ((parameters.get(VSD_LOW_LIM_SPEED_MOTOR) != setPointLowLimFreq_)
//    &&(parameters.get(VSD_FREQUENCY) != setPointFreq_)) {
//    offRegime();
//  }
}

void RegimeRunPush::processingStateRunning()
{
  if ((parameters.get(VSD_LOW_LIM_SPEED_MOTOR) == 1)
   &&(parameters.get(VSD_FREQUENCY) == parameters.get(CCS_RGM_RUN_PUSH_FREQ))) {
    ksu.start(runReason_);
    state_ = WorkState;
  }
  else {
    setPointLowLimFreq_ = parameters.get(VSD_LOW_LIM_SPEED_MOTOR);
    setPointFreq_ = parameters.get(VSD_FREQUENCY);
    parameters.set(VSD_LOW_LIM_SPEED_MOTOR, 1);
    ksu.setFreq(freq_, NoneType, false);
  }
}

void RegimeRunPush::processingStateWork()
{
  switch (state_) {
  case WorkState:
    if (vsd->isSetPointFreq()) {
      timer_ = time_;
      state_ = WorkState + 1;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState-2 --> WorkState - 1");
      #endif
    }
    break;
  case WorkState + 1:
    if (timer_ <= 0) {
      makePush(true);
      timer_ = 0;
      state_ = WorkState + 2;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState-1 --> WorkState");
      #endif
    }
    else {
      timer_--;
    }
    break;
  case WorkState + 2:
    if (timer_ >= time_) {
      makePush(false);
      cntPush_++;
      if (cntPush_ >= quantity_) {
        state_ = WorkState + 3;
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Толчковый: WorkState --> WorkState+1");
        #endif
      }
      else {
        timer_ = time_;
        state_ = WorkState + 1;
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Толчковый: WorkState --> WorkState-1");
        #endif
      }
    }
    else {
      timer_++;
    }
    break;
  case WorkState + 3:
    if ((parameters.get(VSD_LOW_LIM_SPEED_MOTOR) == setPointLowLimFreq_)
      &&(parameters.get(VSD_FREQUENCY) == setPointFreq_)) {
      state_ = IdleState;
    }
    else {
      offRegime();
    }
    break;
  }
}

void RegimeRunPush::automatRegime()
{
  if (action_ == OffAction) {
    if (state_ != IdleState) {
      state_ = WorkState + 3;
    }
  }
  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
  case WorkState + 1:
  case WorkState + 2:
  case WorkState + 3:
    processingStateWork();
    break;
  default:
    break;
  }
}

void RegimeRunPush::offRegime()
{
  makePush(false);
  ksu.setFreq(setPointFreq_, NoneType);
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, setPointLowLimFreq_);
  freq_ = 0;
  cntPush_ = 0;                                       // Счётчик толчков
  timer_ = 0;                                         // Счётчик времени
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) == SingleAction) {
    parameters.set(CCS_RGM_RUN_PUSH_MODE, OffAction);         // Выключаем режим
    logEvent.add(SetpointCode, AutoType, RegimeRunPushOffId); // Записываем данные в лог
  }
}

void RegimeRunPush::makePush(bool push)
{
  if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_5)) {
    if (push) {
      setPointU2_ = parameters.get(VSD_UF_CHARACTERISTIC_U_6);
      parameters.set(VSD_UF_CHARACTERISTIC_U_6, setPointU2_ * voltage_ / 100);
      setPointU1_ = parameters.get(VSD_UF_CHARACTERISTIC_U_5);
      parameters.set(VSD_UF_CHARACTERISTIC_U_5, setPointU1_ * voltage_ / 100);
    }
    else {
      parameters.set(VSD_UF_CHARACTERISTIC_U_6, setPointU2_);
      parameters.set(VSD_UF_CHARACTERISTIC_U_5, setPointU1_);
    }
  }
  else {
    if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_4)) {
      if (push) {
        setPointU2_ = parameters.get(VSD_UF_CHARACTERISTIC_U_5);
        parameters.set(VSD_UF_CHARACTERISTIC_U_5, setPointU2_ * voltage_ / 100);
        setPointU1_ = parameters.get(VSD_UF_CHARACTERISTIC_U_4);
        parameters.set(VSD_UF_CHARACTERISTIC_U_4, setPointU1_ * voltage_ / 100);
      }
      else {
        parameters.set(VSD_UF_CHARACTERISTIC_U_5, setPointU2_);
        parameters.set(VSD_UF_CHARACTERISTIC_U_4, setPointU1_);
      }
    }
    else {
      if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_3)) {
        if (push) {
          setPointU2_ = parameters.get(VSD_UF_CHARACTERISTIC_U_4);
          parameters.set(VSD_UF_CHARACTERISTIC_U_4, setPointU2_ * voltage_ / 100);
          setPointU1_ = parameters.get(VSD_UF_CHARACTERISTIC_U_3);
          parameters.set(VSD_UF_CHARACTERISTIC_U_3, setPointU1_ * voltage_ / 100);
        }
        else {
          parameters.set(VSD_UF_CHARACTERISTIC_U_4, setPointU2_);
          parameters.set(VSD_UF_CHARACTERISTIC_U_3, setPointU1_);
        }
      }
      else {
        if (freq_ > parameters.get(VSD_UF_CHARACTERISTIC_F_2)) {
          if (push) {
            setPointU2_ = parameters.get(VSD_UF_CHARACTERISTIC_U_3);
            parameters.set(VSD_UF_CHARACTERISTIC_U_3, setPointU2_ * voltage_ / 100);
            setPointU1_ = parameters.get(VSD_UF_CHARACTERISTIC_U_2);
            parameters.set(VSD_UF_CHARACTERISTIC_U_2, setPointU1_ * voltage_ / 100);
          }
          else {
            parameters.set(VSD_UF_CHARACTERISTIC_U_3, setPointU2_);
            parameters.set(VSD_UF_CHARACTERISTIC_U_2, setPointU1_);
          }
        }
        else {
          if (push) {
            setPointU2_ = parameters.get(VSD_UF_CHARACTERISTIC_U_2);
            parameters.set(VSD_UF_CHARACTERISTIC_U_2, setPointU2_ * voltage_ / 100);
            setPointU1_ = parameters.get(VSD_UF_CHARACTERISTIC_U_1);
            parameters.set(VSD_UF_CHARACTERISTIC_U_1, setPointU1_ * voltage_ / 100);
          }
          else {
            parameters.set(VSD_UF_CHARACTERISTIC_U_2, setPointU2_);
            parameters.set(VSD_UF_CHARACTERISTIC_U_1, setPointU1_);
          }
        }
      }
    }
  }
}

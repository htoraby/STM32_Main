#include "regime_run_push.h"

RegimeRunPush::RegimeRunPush()
{

}

RegimeRunPush::~RegimeRunPush()
{

}

void RegimeRunPush::getGeneralSetpoint()
{
  runReason_= (LastReasonRun)parameters.get(CCS_LAST_RUN_REASON_TMP);
}

void RegimeRunPush::setOtherSetpoint()
{
  parameters.set(CCS_RGM_RUN_PUSH_STATE, state_);
}



void RegimeRunPush::getOtherSetpoint()
{
  action_ = parameters.get(CCS_RGM_RUN_PUSH_MODE);    // Активность режима
  state_ = parameters.get(CCS_RGM_RUN_PUSH_STATE);    // Состояние автомата режима
  freq_ = parameters.get(CCS_RGM_RUN_PUSH_FREQ);      // Частота толчка
  quantity_ = parameters.get(CCS_RGM_RUN_PUSH_QUANTITY);  // Количество толчков
  voltage_ = parameters.get(CCS_RGM_RUN_PUSH_VOLTAGE);// Напряжение толчка
  time_ = (1 / freq_) * 100;                          // Время толчка (счетчик * 100мс)
}

void RegimeRunPush::processing()
{
  getGeneralSetpoint();
  getOtherSetpoint();
  automatRegime();
  setGeneralSetPoint();
  setOtherSetpoint();
}

  //! TODO: Возможно здесь потребуется добавить блок чтения текущей частоты, тока и слова состояния ЧРП
/*
  switch (state_) {
  case IdleState:
    if (!checkOffRegime())
      offRegime();
    if (ksu.isBlock()) {                              // Если блокировка
      state_ = IdleState;
    }
    else {
      if (ksu.isWorkMotor()) {                        // Если можем работать
        if (!ksu.isPrevent()) {                       // Если нет запрещающих параметров
          flag_ = true;
          setPointLowLimFreq_ = parameters.get(VSD_LOW_LIM_SPEED_MOTOR);
          parameters.set(VSD_LOW_LIM_SPEED_MOTOR, 1);
          state_ = WorkState - 3;
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Толчковый: Idle --> WorkState-3 ");
          #endif
        }
      }
    }
    break;
  case WorkState - 3:
    if (!ksu.isWorkMotor()) {
      state_ = IdleState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState-3 --> Idle");
      #endif
    }
    else {
      if (checkOnRegime()) {
        flag_ = true;
        cntPush_ = 0;
        setPointFreq_ = parameters.get(VSD_FREQUENCY);
        parameters.set(VSD_FREQUENCY, freq_);
        state_ = WorkState - 2;
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Толчковый: WorkState-3 --> WorkState-2 (freq = %f5.2)",
                                 freq_);
        #endif
      }
    }
    break;
  case WorkState - 2:
    if (!ksu.isWorkMotor()) {
      state_ = IdleState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState-2 --> Idle");
      #endif
    }
    else {
      flag_ = true;
      if (vsd->isControl()) {
        timer_ = time_;
        state_ = WorkState - 1;
        #if (USE_LOG_DEBUG == 1)
                logDebug.add(DebugMsg, "Толчковый: WorkState-2 --> WorkState - 1");
        #endif
      }
    }
    break;
  case WorkState - 1:
    if (!ksu.isWorkMotor()) {
      state_ = IdleState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState-2 --> Idle");
      #endif
    }
    else {
      flag_ = true;
      if (timer_ < 1) {
        makePush(true);
        timer_ = 0;
        state_ = WorkState;
        #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Толчковый: WorkState-1 --> WorkState");
        #endif
      }
      else {
        timer_--;
      }
    }
    break;
  case WorkState:
    if (!ksu.isWorkMotor()) {
      state_ = IdleState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState --> Idle");
      #endif
    }
    else {
      flag_ = true;
      if (timer_ > time_) {
        makePush(false);
        cntPush_++;
        if (cntPush_ >= quantity_) {
          if (!checkOffRegime()) {
            offRegime();
          }
          state_ = WorkState + 1;
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Толчковый: WorkState --> WorkState+1");
          #endif
        }
        else {
          timer_ = time_;
          state_ = WorkState - 1;
          #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Толчковый: WorkState --> WorkState-1");
          #endif
        }
      }
      else {
        timer_++;
      }
    }
    break;
  case WorkState + 1:
    if (!ksu.isWorkMotor()) {
      state_ = IdleState;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState+1 --> Idle");
      #endif
    }
    else {
      flag_ = true;
      state_ = WorkState + 2;
      #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Толчковый: WorkState+1 --> WorkState+2");
      #endif
    }
    break;
  case WorkState + 2:
    if (ksu.isStopMotor()) {
      state_ = IdleState;
    }
    if (!checkOffRegime()) {
      parameters.set(VSD_LOW_LIM_SPEED_MOTOR, setPointLowLimFreq_);
    }
    break;
  default:
    state_ = IdleState;
  }
}
*/

void RegimeRunPush::processingStateIdle()
{
  if (action_) {
    if (ksu.getValue(CCS_CONDITION) == CCS_CONDITION_STOP) {  // Станция в останове
      if (runReason_ != LastReasonRunNone) {                  // Попытка пуска
        state_ = RunningState;
      }
    }
  }
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
      parameters.set(VSD_LOW_LIM_SPEED_MOTOR, setPointLowLimFreq_);
      parameters.set(VSD_FREQUENCY, setPointFreq_);
    }
    break;
  }
}

void RegimeRunPush::automatRegime()
{
  if (action_ == OffAction) {
    state_ = IdleState;
  }
  switch (state_) {
  case IdleState:
    processingStateIdle();
    break;
  case RunningState:
    processingStateRunning();
    break;
  case WorkState:
    processingStateWork();
    break;
  default:
    break;
  }
}

void RegimeRunPush::offRegime()
{
  makePush(false);
  parameters.set(VSD_FREQUENCY, setPointFreq_);
  parameters.set(VSD_LOW_LIM_SPEED_MOTOR, setPointLowLimFreq_);
  freq_ = 0;
  cntPush_ = 0;                                       // Счётчик толчков
  timer_ = 0;                                         // Счётчик времени
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

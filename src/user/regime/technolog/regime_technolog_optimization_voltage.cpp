#include "regime_technolog_optimization_voltage.h"
#include "protection_main.h"

RegimeTechnologOptimizationVoltage::RegimeTechnologOptimizationVoltage()
{

}

RegimeTechnologOptimizationVoltage::~RegimeTechnologOptimizationVoltage()
{

}

void RegimeTechnologOptimizationVoltage::processing()
{
  action_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_MODE);
  state_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_STATE);

  delay_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELAY);
  period_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_PERIOD);
  scale_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_SCALE);
  scale_ = scale_ / 100.0;
  step_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_STEP);

  maxUfPoint_ = parameters.get(CCS_BASE_VOLTAGE);

  if (action_ == OffAction) { // Режим - выключен
    state_ = IdleState;
  }

  if (ksu.isBreakOrStopMotor()) {
    state_ = IdleState;
  }

  switch (state_) {
  //! Состояние в котором ничего не происходит
  case IdleState:
    //! Если двигатель в работе, переходим в состояние ожидания задержки после запуска
    if (ksu.isRunOrWorkMotor()) {
      state_ = RunningState;
    }
    break;
  //! Состояние в котором ожидается время после запуска
  case RunningState:
    //! Если задержка после запуска истекла
    if ((ksu.getSecFromCurTime(CCS_LAST_RUN_DATE_TIME) > delay_)
      && (parameters.get(VSD_FREQUENCY) == parameters.get(VSD_FREQUENCY_NOW))) {
      //! Находим верхную точку отрезка U/f которой принадлежит текущая частота
      idUfHiPoint_ = vsd->findUfHiPoint(parameters.get(VSD_FREQUENCY_NOW));
      //! Если точка существует
      if (idUfHiPoint_) {
        //! Получаем текущее значение напряжения в этой точке
        valueUfHiPoint_ = parameters.get(idUfHiPoint_);
        //! Вычисляем границы регулирования напряжения в этой точке
        limUpUfHiPoint_ = valueUfHiPoint_ + valueUfHiPoint_ * scale_;
        limDownUfHiPoint_ = valueUfHiPoint_ - valueUfHiPoint_ * scale_;
        limUpUfHiPoint_ = (limUpUfHiPoint_ > maxUfPoint_) ? maxUfPoint_ : limUpUfHiPoint_;
        limDownUfHiPoint_ = (limDownUfHiPoint_ < 0) ? 0 : limDownUfHiPoint_;
        //! Находим нижную точку отрезка U/f которой принадлежит текущая частота
        idUfLowPoint_ = vsd->findUfLowPoint(idUfHiPoint_);
        //! Получаем текущее значение напряжения в этой точке
        valueUfLowPoint_ = parameters.get(idUfLowPoint_);
        //! Вычисляем границы регулирования напряжения в этой точке
        limUpUfLowPoint_ = valueUfLowPoint_ + valueUfLowPoint_ * scale_;
        limDownUfLowPoint_ = valueUfLowPoint_ - valueUfLowPoint_ * scale_;
        limUpUfLowPoint_ = (limUpUfLowPoint_ > maxUfPoint_) ? maxUfPoint_ : limUpUfLowPoint_;
        limDownUfLowPoint_ = (limDownUfLowPoint_ < 0) ? 0 : limDownUfLowPoint_;
        //! Переходим на состояние регулирования
        state_ = WorkState;
      }
    }
    break;
  //! Состояние понижения напряжения
  case WorkState:
    if ((valueUfHiPoint_ <= limDownUfHiPoint_) && (valueUfLowPoint_ <= limDownUfLowPoint_))
    {
      state_ = PauseState;
      beginPeriod_ = ksu.getTime();
    }
    else {
      oldCurrent_ = parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
      //! Задаём новую верхнию точку напряжения
      valueUfHiPoint_ = valueUfHiPoint_ - step_;
      if (valueUfHiPoint_ < limDownUfHiPoint_) {
        valueUfHiPoint_ = limDownUfHiPoint_;
      }
      parameters.set(idUfHiPoint_, valueUfHiPoint_, NoneType);
      //! Задаём новую нижнию точку напряжения
      valueUfLowPoint_ = valueUfLowPoint_ - step_;
      if (valueUfLowPoint_ < limDownUfLowPoint_) {
        valueUfLowPoint_ = limDownUfLowPoint_;
      }
      parameters.set(idUfLowPoint_, valueUfLowPoint_, NoneType);
      state_ = WorkState + 1;
    }
    break;
  case WorkState + 1:
    cntCurrent_ = 0;
    newCurrent_ = 0;
    state_ = WorkState + 2;
    break;
  case WorkState + 2:
    if (cntCurrent_ < 30) {
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
      cntCurrent_++;
    }
    else {
      newCurrent_ = newCurrent_ / cntCurrent_;
      if ((newCurrent_ > oldCurrent_)
        || (newCurrent_ > parameters.get(VSD_MOTOR_CURRENT))) {
        state_ = WorkState + 3;
      }
      else {
        state_ = WorkState;
      }
    }
    break;
  case WorkState + 3:
    valueUfHiPoint_ = valueUfHiPoint_ + step_;
    if (valueUfHiPoint_ > limUpUfHiPoint_) {
      valueUfHiPoint_ = limUpUfHiPoint_;
    }
    parameters.set(idUfHiPoint_, valueUfHiPoint_, NoneType);
    valueUfLowPoint_ = valueUfLowPoint_ + step_;
    if (valueUfLowPoint_ > limUpUfLowPoint_) {
      valueUfLowPoint_ = limUpUfLowPoint_;
    }
    parameters.set(idUfLowPoint_, valueUfLowPoint_, NoneType);
    state_ = PauseState;
    beginPeriod_ = ksu.getTime();
    break;
  case PauseState:
    if (ksu.getSecFromCurTime(beginPeriod_) > period_) {
      state_ = WorkState;
    }
    break;
  default:
      state_ = IdleState;
    break;
  }
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_STATE, state_);
}


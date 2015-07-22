#include "regime_technolog_maintenance_param.h"

RegimeTechnologMaintenanceParam::RegimeTechnologMaintenanceParam()
  : timer_(0)
  , delta_(0)
{

}

RegimeTechnologMaintenanceParam::~RegimeTechnologMaintenanceParam()
{

}

void RegimeTechnologMaintenanceParam::processing()
{
  action_ = parameters.get(CCS_RGM_MAINTENANCE_PARAM_MODE);
  state_ = parameters.get(CCS_RGM_MAINTENANCE_PARAM_STATE);

  if ((action_ == OffAction) ||                                // Режим - выключен
      (parameters.get(CCS_CONDITION) != CCS_CONDITION_RUN)) {  // Двигатель не в работе
    state_ = IdleState;
  }

  switch (state_) {
  case IdleState:
    timer_ = 0;
    delta_ = 0;
    if (action_ != OffAction) { // Режим - включен
      if (parameters.get(CCS_CONDITION) == CCS_CONDITION_RUN) {  // Двигатель - работа;
        state_ = WorkState;
      }
    }
    break;
  case WorkState:
    {
      if (timer_ == 0) {
        timer_ = ksu.getTime();
      }

      float curValue;
      float setpoint;
      int type = parameters.get(CCS_RGM_MAINTENANCE_PARAM_TYPE);
      switch (type) {
      case PressureParam:
        curValue = parameters.get(TMS_PRESSURE_INTAKE);
        setpoint = parameters.get(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_PRESSURE);
        break;
      case TemperatureParam:
        curValue = parameters.get(TMS_TEMPERATURE_WINDING);
        setpoint = parameters.get(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_TEMPERATURE);
        break;
      case AnalogIn1Param: case AnalogIn2Param:
      case AnalogIn3Param: case AnalogIn4Param:
        curValue = parameters.get(CCS_AI_1_VALUE + (type - AnalogIn1Param));
        setpoint = parameters.get(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_ANALOG_IN);
        break;
      default:
        curValue = parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
        setpoint = parameters.get(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_CURRENT);
        break;
      }

      // Накопление площади для интегрального слагаемого
      delta_ += curValue - setpoint;

      float period = parameters.get(CCS_RGM_MAINTENANCE_PARAM_PERIOD);
      if ((ksu.getSecFromCurTime(timer_) >= period) && (period > 0)) {
        // Посчёт пропорционального и интегрального слагаемого
        float delta = curValue - setpoint;
        float propSum = parameters.get(CCS_RGM_MAINTENANCE_PARAM_PROP) * delta;
        float intSum = parameters.get(CCS_RGM_MAINTENANCE_PARAM_INT) * delta_ / period;
        float freq = propSum + intSum;

        // Регулировка частоты
        float minFreq = parameters.get(CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ);
        float maxFreq = parameters.get(CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ);
        freq = min((int)freq, (int)(0.1 * maxFreq));
        if (fabs(freq) > (0.1 * maxFreq)) {
          if (freq > 0)
            freq = 0.1 * maxFreq;
          else
            freq = -0.1 * maxFreq;
        }
        freq = min((int)freq, (int)(0.1 * maxFreq));

        // Расчёт новой частоты с учётом изменения
        bool depend = parameters.get(CCS_RGM_MAINTENANCE_PARAM_DEPENDENCE);
        freq = parameters.get(VSD_FREQUENCY) - (depend * freq) + (!depend * freq);

        // Проверка частоты на допустимые границы
        freq = max(freq, minFreq);
        freq = min(freq, maxFreq);

        ksu.setFreq(freq);

        timer_ = 0;
        delta_ = 0;
        state_ = PauseState;
      }
    }
    break;
  case PauseState:
    if (parameters.get(VSD_FREQUENCY) == parameters.get(VSD_FREQUENCY_NOW)) {
      state_ = WorkState;
    }
    break;
  default:
    state_ = IdleState;
    break;
  }

  parameters.set(CCS_RGM_MAINTENANCE_PARAM_STATE, state_);
}

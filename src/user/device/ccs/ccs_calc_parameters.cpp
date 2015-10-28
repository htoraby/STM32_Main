#include "ccs.h"
#include "user_main.h"
#include "regime_main.h"
#include "gpio.h"
#include "adc_ext.h"

#if USE_EXT_MEM
static uint16_t uValue[ADC_CNANNELS_NUM*HC_POINTS_NUM*2] __attribute__((section(".extmem")));
#else
static uint16_t uValue[ADC_CNANNELS_NUM*HC_POINTS_NUM*2];
#endif

void Ccs::calcParametersTask()
{
  while (1) {
    osDelay(100);
    calcMotorVoltagePhase1();
    calcMotorVoltagePhase2();
    calcMotorVoltagePhase3();
    calcMotorVoltageImbalance();
    calcMotorCos();
    calcMotorLoad();

// TODO: Возможно нужно перенести вычисления в счётчик после получения данных
//    calcInputVoltagePhase1();
//    calcInputVoltagePhase2();
//    calcInputVoltagePhase3();
//    calcInputVoltagePhase12();
//    calcInputVoltagePhase23();
//    calcInputVoltagePhase31();
    calcInputVoltageFromAdc();

    calcInputVoltageImbalance();
    calcInputCurrentImbalance();
    calcResistanceIsolation();
    calcRegimeRun();

    calcAnalogInputs();
  }
}

float Ccs::calcTransCoef()
{
  float voltTapOff = parameters.get(CCS_TRANS_VOLTAGE_TAP_OFF);
  float voltNom =  parameters.get(CCS_TRANS_NOMINAL_VOLTAGE);
  float transCoef;
  if (voltNom == 0)
    transCoef = voltTapOff / 380;
  else
    transCoef = voltTapOff / voltNom;
  setValue(CCS_COEF_TRANSFORMATION, transCoef);
  return parameters.get(CCS_COEF_TRANSFORMATION);
}

float Ccs::calcMotorCurrentPhase(float vsdCurOut)
{
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (coefTrans == 0)
    return vsdCurOut;
  else
    return (vsdCurOut / coefTrans);
}

float Ccs::calcMotorCurrentPhase1()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_1,
           calcMotorCurrentPhase(parameters.get(VSD_CURRENT_OUT_PHASE_1)));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
}

float Ccs::calcMotorCurrentPhase2()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_2,
           calcMotorCurrentPhase(parameters.get(VSD_CURRENT_OUT_PHASE_2)));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
}

float Ccs::calcMotorCurrentPhase3()
{
  setValue(CCS_MOTOR_CURRENT_PHASE_3,
           calcMotorCurrentPhase(parameters.get(VSD_CURRENT_OUT_PHASE_3)));
  return parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
}

float Ccs::calcMotorCurrentAverage()
{
  setValue(CCS_MOTOR_CURRENT_AVARAGE,
           calcAverage3Values(parameters.get(CCS_MOTOR_CURRENT_PHASE_1),
                              parameters.get(CCS_MOTOR_CURRENT_PHASE_2),
                              parameters.get(CCS_MOTOR_CURRENT_PHASE_3)));
  return parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
}

float Ccs::calcMotorCurrentImbalance()
{
  setValue(CCS_MOTOR_CURRENT_IMBALANCE,
           calcImbalance(parameters.get(CCS_MOTOR_CURRENT_PHASE_1),
                         parameters.get(CCS_MOTOR_CURRENT_PHASE_2),
                         parameters.get(CCS_MOTOR_CURRENT_PHASE_3),
                         1));
  return parameters.get(CCS_MOTOR_CURRENT_IMBALANCE);
}

float Ccs::calcDropVoltageFilter(float current, float freq, float coefTrans)
{
  float outFilter = parameters.get(CCS_FILTER_OUTPUT);
  float inductFilter = parameters.get(CCS_FILTER_INDUCTANCE);
  if (freq == -1)
    freq = parameters.get(VSD_FREQUENCY_NOW);
  if (coefTrans == -1)
    coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (outFilter == 0)
    return 0;
  if (freq == 0)
    freq = parameters.get(VSD_MOTOR_FREQUENCY);
  if (coefTrans == 0)
    coefTrans = 380.0;
  float dUf = 2 * NUM_PI * inductFilter * freq * current * coefTrans / 1000.0;
  return dUf;
}

float Ccs::calcDropVoltageCable(float current)
{
  float lenght = parameters.get(CCS_TRANS_CABLE_LENGHT);
  float cross =  parameters.get(CCS_TRANS_CABLE_CROSS);
  if (parameters.get(CCS_RGM_HEAT_CABLE_MODE) != Regime::OffAction) {
    lenght = 2 * lenght / 1000;                                         // Длина петли кабеля в кМ
    float R80 = parameters.get(CCS_RGM_HEAT_CABLE_RESISTANCE_80);       // Сопротивление кабеля при 80°С
    current = parameters.get(VSD_MOTOR_CURRENT);                        // Номинальный ток ПЭД
    float dUcl = R80 * lenght * current;
    return dUcl;
  }
  else {
    if (!cross)
      cross = 16;
    float dUcl = 38.7 * (current * lenght) / (cross * 1000);            // Падение напряжения на кабельной линии
    return dUcl;
  }
}

float Ccs::calcMotorVoltagePhase(float dropVoltFilter, float dropVoltCable)
{
  float vsdVoltOut = parameters.get(VSD_OUT_VOLTAGE_MOTOR);
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  return (vsdVoltOut * coefTrans - (dropVoltFilter + dropVoltCable) * sqrt(3));
}

float Ccs::calcMotorVoltagePhase1()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_1, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_1);
}

float Ccs::calcMotorVoltagePhase2()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_2, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_2);
}

float Ccs::calcMotorVoltagePhase3()
{
  float current = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_3, calcMotorVoltagePhase(calcDropVoltageFilter(current),
                                                            calcDropVoltageCable(current)));
  return parameters.get(CCS_MOTOR_VOLTAGE_PHASE_3);
}

float Ccs::calcMotorVoltageImbalance()
{
  setValue(CCS_MOTOR_VOLTAGE_IMBALANCE,
           calcImbalance(parameters.get(CCS_MOTOR_VOLTAGE_PHASE_1),
                         parameters.get(CCS_MOTOR_VOLTAGE_PHASE_2),
                         parameters.get(CCS_MOTOR_VOLTAGE_PHASE_3),
                         0));
  return parameters.get(CCS_MOTOR_VOLTAGE_IMBALANCE);
}

float Ccs::calcMotorSpeed()
{
  float mtrSpeed = NAN;
  if (parameters.getValidity(VSD_FREQUENCY_NOW) == ok_r) {
    float mtrType = parameters.get(CCS_MOTOR_TYPE);
    float freq = parameters.get(VSD_FREQUENCY_NOW);
    if (mtrType == VSD_MOTOR_TYPE_ASYNC) {
      mtrSpeed = freq * 60;
    }
    else {
      if (mtrType == VSD_MOTOR_TYPE_VENT) {
        mtrSpeed = freq * 30;
      }
      else {
        mtrSpeed = 0;
      }
    }
  }
  setValue(CCS_MOTOR_SPEED_NOW, mtrSpeed);
  return parameters.get(CCS_MOTOR_SPEED_NOW);
}

float Ccs::calcMotorCos()
{
  setValue(CCS_MOTOR_COS_PHI_NOW, parameters.get(VSD_MOTOR_COS_PHI_NOW));
  return parameters.get(CCS_MOTOR_COS_PHI_NOW);
}

float Ccs::calcMotorLoad()
{
  float mtrCur = parameters.get(CCS_MOTOR_CURRENT_AVARAGE);
  float mtrCos = parameters.get(CCS_MOTOR_COS_PHI_NOW);
  float nomMtrCur = parameters.get(VSD_MOTOR_CURRENT);
  float nomMtrCos = parameters.get(VSD_MOTOR_COS_PHI);
  float mtrLoad;
  if ((nomMtrCur == 0) || (nomMtrCos == 0))
    mtrLoad = 0;
  else
    mtrLoad = ((mtrCur * mtrCos) / (nomMtrCur * nomMtrCos)) * 100;
  setValue(CCS_MOTOR_LOAD_NOW, mtrLoad);
  return parameters.get(CCS_MOTOR_LOAD_NOW);
}

float Ccs::calcInputVoltagePhase1()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_1)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_A))) {
    setNewValue(CCS_VOLTAGE_PHASE_1, (float)NAN);
  }
  else {
    setNewValue(CCS_VOLTAGE_PHASE_1,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_1),
                       parameters.get(CCS_COEF_VOLTAGE_IN_A)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_1);
}

float Ccs::calcInputVoltagePhase2()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_2)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_B))) {
    setNewValue(CCS_VOLTAGE_PHASE_2, (float)NAN);
  }
  else {
    setValue(CCS_VOLTAGE_PHASE_2,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_2),
                       parameters.get(CCS_COEF_VOLTAGE_IN_B)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_2);
}

float Ccs::calcInputVoltagePhase3()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_3)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_C))) {
    setNewValue(CCS_VOLTAGE_PHASE_3, (float)NAN);
  }
  else {
    setValue(CCS_VOLTAGE_PHASE_3,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_3),
                       parameters.get(CCS_COEF_VOLTAGE_IN_C)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_3);
}

float Ccs::calcInputVoltagePhase12()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_1_2)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_A))) {
    setNewValue(CCS_VOLTAGE_PHASE_1_2, (float)NAN);
  }
  else {
    setValue(CCS_VOLTAGE_PHASE_1_2,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_1_2),
                       parameters.get(CCS_COEF_VOLTAGE_IN_A)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_1_2);
}

float Ccs::calcInputVoltagePhase23()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_2_3)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_B))) {
    setNewValue(CCS_VOLTAGE_PHASE_2_3, (float)NAN);
  }
  else {
    setValue(CCS_VOLTAGE_PHASE_2_3,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_2_3),
                       parameters.get(CCS_COEF_VOLTAGE_IN_B)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_2_3);
}

float Ccs::calcInputVoltagePhase31()
{
  if ((parameters.getValidity(EM_VOLTAGE_PHASE_3_1)) ||
      (parameters.getValidity(CCS_COEF_VOLTAGE_IN_C))) {
    setNewValue(CCS_VOLTAGE_PHASE_3_1, (float)NAN);
  }
  else {
    setValue(CCS_VOLTAGE_PHASE_3_1,
             applyCoef(parameters.get(EM_VOLTAGE_PHASE_3_1),
                       parameters.get(CCS_COEF_VOLTAGE_IN_C)));
  }
  return parameters.get(CCS_VOLTAGE_PHASE_3_1);
}

float Ccs::calcInputVoltageImbalance()
{
  if (parameters.getValidity(CCS_VOLTAGE_PHASE_1) ||
      parameters.getValidity(CCS_VOLTAGE_PHASE_2) ||
      parameters.getValidity(CCS_VOLTAGE_PHASE_3)) {
    setNewValue(CCS_VOLTAGE_IMBALANCE_IN, (float)NAN);
  }
  else {
  setValue(CCS_VOLTAGE_IMBALANCE_IN,
           calcImbalance(parameters.get(CCS_VOLTAGE_PHASE_1),
                         parameters.get(CCS_VOLTAGE_PHASE_2),
                         parameters.get(CCS_VOLTAGE_PHASE_3),
                         0));
  }
  return parameters.get(CCS_VOLTAGE_IMBALANCE_IN);
}

float Ccs::calcInputCurrentImbalance()
{
  setValue(CCS_CURRENT_IMBALANCE_IN,
           calcImbalance(parameters.get(EM_CURRENT_PHASE_1),
                         parameters.get(EM_CURRENT_PHASE_2),
                         parameters.get(EM_CURRENT_PHASE_3),
                         1));
  return parameters.get(CCS_CURRENT_IMBALANCE_IN);
}

float Ccs::calcMotorInductFromResistPhase()
{
  float resist = parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE);
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float induct = resist * 1000 / (2 * NUM_PI * freq);
  setValue(CCS_MOTOR_INDUCTANCE, induct);
  return parameters.get(CCS_MOTOR_INDUCTANCE);
}

float Ccs::calcMotorResistPhaseFromInduct()
{
  float induct = parameters.get(CCS_MOTOR_INDUCTANCE);
  float freq = parameters.get(VSD_MOTOR_FREQUENCY);
  float resist = 2 * NUM_PI * induct * freq / 1000;
  setValue(CCS_MOTOR_INDUCTANCE_RESIST_PHASE, resist);
  return parameters.get(CCS_MOTOR_INDUCTANCE_RESIST_PHASE);
}

float Ccs::calcSystemInduct()
{
  float coefTrans2 = pow(parameters.get(CCS_COEF_TRANSFORMATION), 2);
  float inductTrans = (parameters.get(CCS_TRANS_VOLTAGE_SHORT_CIRCUIT) * parameters.get(CCS_TRANS_NOMINAL_VOLTAGE) * 0.01) /
                      (parameters.get(CCS_TRANS_NOMINAL_CURRENT) * parameters.get(CCS_TRANS_NOMINAL_FREQUENCY_INPUT) * 2 * NUM_PI);
  float inductCable = ((parameters.get(CCS_TRANS_CABLE_LENGHT) * 1.5) / coefTrans2) / 1000000;
  float inductMotor = (parameters.get(CCS_MOTOR_INDUCTANCE) / coefTrans2) / 1000;
  float inductFilter = parameters.get(CCS_FILTER_INDUCTANCE) / 1000;
  float induct = (inductTrans + inductCable + inductMotor + inductFilter) * 1000;
  parameters.set(CCS_SYSTEM_INDUCTANCE, induct);
  vsd->setSumInduct(parameters.get(CCS_SYSTEM_INDUCTANCE));
  return parameters.get(CCS_SYSTEM_INDUCTANCE);
}

float Ccs::calcTransRecommendedTapOff()
{
  // Вычисляем коэффициент трансформации без учёта потери в фильтре
  float nomVoltIn = parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT);
  float curVoltIn = calcAverage3Values(parameters.get(CCS_VOLTAGE_PHASE_1_2),
                                       parameters.get(CCS_VOLTAGE_PHASE_2_3),
                                       parameters.get(CCS_VOLTAGE_PHASE_3_1));
  if (curVoltIn > nomVoltIn) {
    curVoltIn = nomVoltIn;
  }
  float nomCurMtr = parameters.get(VSD_MOTOR_CURRENT);
  float nomVoltMtr = parameters.get(VSD_MOTOR_VOLTAGE);
  float nomFreqMtr = parameters.get(VSD_MOTOR_FREQUENCY);
  if (nomFreqMtr == 0)
    nomFreqMtr = 50;
  float baseFreq = parameters.get(CCS_BASE_FREQUENCY);
  if (baseFreq == 0)
    baseFreq = nomFreqMtr;
  float baseVolt = (baseFreq / nomFreqMtr ) * nomVoltMtr;
  float dropVoltCable = calcDropVoltageCable(nomCurMtr);
  float voltHiLim = parameters.get(CCS_BASE_VOLTAGE);
  float transCoef = (baseVolt + dropVoltCable) / (voltHiLim);

  // Вычисляем коэффициент трансформации с учётом потери в фильтре
  float dropVoltFilter = calcDropVoltageFilter(nomCurMtr, nomFreqMtr, transCoef);
  transCoef = (baseVolt + dropVoltCable) / (voltHiLim - dropVoltFilter);

  // Вычисляем рекомендуемое напряжение отпайки
  float voltNom =  parameters.get(CCS_TRANS_NOMINAL_VOLTAGE);
  float transTapOff = voltNom;
  if ((voltNom == 0) || (transCoef == 0))
    transTapOff = 380;
  else
    transTapOff = transCoef * voltNom;

  setValue(CCS_MOTOR_VOLTAGE_CALC, voltHiLim * getValue(CCS_COEF_TRANSFORMATION) - dropVoltCable - dropVoltFilter);
  setValue(CCS_DPOR_VOLTAGE_CABLE, dropVoltCable);
  setValue(CCS_DROP_VOLTAGE_FILTER, dropVoltFilter);
  setValue(CCS_TRANS_NEED_VOLTAGE_TAP_OFF, transTapOff);
  return parameters.get(CCS_TRANS_NEED_VOLTAGE_TAP_OFF);
}

float Ccs::calcResistanceIsolation()
{
  if ((parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE) &&
      (!parameters.getValidity(TMS_RESISTANCE_ISOLATION))) {
    setValue(CCS_RESISTANCE_ISOLATION, parameters.get(TMS_RESISTANCE_ISOLATION));
  }
  else {
    if ((parameters.get(CCS_TYPE_VSD) == VSD_TYPE_ETALON) &&
        (!parameters.getValidity(VSD_ETALON_RESISTANCE_ISOLATION))) {
      setValue(CCS_RESISTANCE_ISOLATION, parameters.get(VSD_ETALON_RESISTANCE_ISOLATION));
    }
  }
  return parameters.get(CCS_RESISTANCE_ISOLATION);
}

void Ccs::calcRegimeChangeFreqPeriodOneStep()
{
  float beginFreq = parameters.get(CCS_RGM_CHANGE_FREQ_BEGIN_FREQ);
  float endFreq = parameters.get(CCS_RGM_CHANGE_FREQ_END_FREQ);
  float period = parameters.get(CCS_RGM_CHANGE_FREQ_PERIOD);
  float periodOneStep = 0;
  if ((endFreq - beginFreq) != 0)
    periodOneStep = period / (fabs(endFreq - beginFreq)) * 0.1;

  // Проверка на минимум 0.1 сек
  if (periodOneStep < 0.1) {
    periodOneStep = 0.1;
  }
  setValue(CCS_RGM_CHANGE_FREQ_PERIOD_ONE_STEP, periodOneStep);
}

void Ccs::calcRegimeRun()
{
  if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE) != Regime::OffAction) {
    parameters.set(CCS_RUNNING_TYPE, Regime::AutoAdaptationRegimeRun);
    return;
  }
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    parameters.set(CCS_RUNNING_TYPE, Regime::PushRegimeRun);
    return;
  }
  if (parameters.get(CCS_RGM_RUN_SWING_MODE) != Regime::OffAction) {
    parameters.set(CCS_RUNNING_TYPE, Regime::SwingRegimeRun);
    return;
  }
  parameters.set(CCS_RUNNING_TYPE, Regime::SmoothRegimeRun);
  return;
}

void Ccs::calcInputVoltageFromAdc()
{
  float uaValue;
  float ubValue;
  float ucValue;
  float valueOld[3] = { 1 };
  int checkPhase[3] = { 0 };
  int count[3] = { 0 };

  getAdcDataInPeriod(uValue);

  for (int i = 0; i < HC_POINTS_NUM*2; ++i) {
    if (((uValue[0 + i*3] - 2048) >= 0) && (valueOld[0] < 0)) {
      if (!checkPhase[0])
        checkPhase[0] = 1;
      else
        checkPhase[0] = 2;
    }
    if (checkPhase[0] == 1) {
      uaValue += (uValue[0 + i*3] - 2048)*(uValue[0 + i*3] - 2048);
      count[0]++;
    }

    if (((uValue[1 + i*3] - 2048) >= 0) && (valueOld[1] < 0)) {
      if (!checkPhase[1]) {
        if ((uValue[1 + i*3] - 2048) < (uValue[0 + i*3] - 2048))
          countPhaseRotation_++;
        else if ((uValue[1 + i*3] - 2048) > (uValue[0 + i*3] - 2048))
          countPhaseRotation_--;

        checkPhase[1] = 1;
      } else {
        checkPhase[1] = 2;
      }
    }

    if (checkPhase[1] == 1) {
      ubValue += (uValue[1 + i*3] - 2048)*(uValue[1 + i*3] - 2048);
      count[1]++;
    }
    if (((uValue[2 + i*3] - 2048) >= 0) && (valueOld[2] < 0)) {
      if (!checkPhase[2])
        checkPhase[2] = 1;
      else
        checkPhase[2] = 2;
    }
    if (checkPhase[2] == 1) {
      ucValue += (uValue[2 + i*3] - 2048)*(uValue[2 + i*3] - 2048);
      count[2]++;
    }
    valueOld[0] = (uValue[0 + i*3] - 2048);
    valueOld[1] = (uValue[1 + i*3] - 2048);
    valueOld[2] = (uValue[2 + i*3] - 2048);
  }
  uaValue = (sqrt(uaValue/count[0]) * 627.747 * 2.5) / 0xFFF;
  ubValue = (sqrt(ubValue/count[1]) * 627.747 * 2.5) / 0xFFF;
  ucValue = (sqrt(ucValue/count[2]) * 627.747 * 2.5) / 0xFFF;

  if (parameters.getValidity(CCS_COEF_VOLTAGE_IN_A)) {
    setValue(CCS_VOLTAGE_PHASE_1, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_1_2, (float)NAN);
  }
  else {
    uaValue = applyCoef(uaValue, parameters.get(CCS_COEF_VOLTAGE_IN_A));
    setValue(CCS_VOLTAGE_PHASE_1, uaValue);
    setValue(CCS_VOLTAGE_PHASE_1_2, uaValue*SQRT_3);
  }

  if (parameters.getValidity(CCS_COEF_VOLTAGE_IN_B)) {
    setValue(CCS_VOLTAGE_PHASE_2, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_2_3, (float)NAN);
  }
  else {
    ubValue = applyCoef(ubValue, parameters.get(CCS_COEF_VOLTAGE_IN_B));
    setValue(CCS_VOLTAGE_PHASE_2, ubValue);
    setValue(CCS_VOLTAGE_PHASE_2_3, ubValue*SQRT_3);
  }

  if (parameters.getValidity(CCS_COEF_VOLTAGE_IN_C)) {
    setValue(CCS_VOLTAGE_PHASE_3, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_3_1, (float)NAN);
  }
  else {
    ucValue = applyCoef(ucValue, parameters.get(CCS_COEF_VOLTAGE_IN_C));
    setValue(CCS_VOLTAGE_PHASE_3, ucValue);
    setValue(CCS_VOLTAGE_PHASE_3_1, ucValue*SQRT_3);
  }

  if (countPhaseRotation_ >= 10) {
    countPhaseRotation_ = 0;
    setValue(CCS_PHASE_ROTATION, PHASE_ABC);
  } else if (countPhaseRotation_ <= -10) {
    countPhaseRotation_ = 0;
    setValue(CCS_PHASE_ROTATION, PHASE_CBA);
  }
}

void Ccs::calcDigitalInputs()
{
  uint8_t value[4];
  static uint8_t valueOld[4];
  static uint8_t count[4];

  for (int i = 0; i <= DI4; ++i) {
    value[i] = !getDigitalInput(i);
    if (value[i] == valueOld[i]) {
      count[i]++;
      if (count[i] >= 10) {
        count[i] = 0;
        if (getValue(CCS_DI_1_VALUE + i) != value[i]) {
          if (setValue(CCS_DI_1_VALUE + i, value[i]) == ok_r)
            changedDigitalInput(i);
        }
      }
    } else {
      count[i] = 0;
    }
    valueOld[i] = value[i];
  }
}

void Ccs::changedDigitalInput(int num)
{
  int action = getValue(CCS_DI_1_ACTION + num);
  if ((action == DI_ACTION_NONE) || (action == DI_ACTION_PROTECTION))
    return;

  uint8_t value = getValue(CCS_DI_1_VALUE + num);
  if (value == getValue(CCS_DI_1_TYPE + num)) {
    switch (action) {
    case DI_ACTION_STOP:
      stop(LastReasonStop(LastReasonStopDigital1 + num));
      break;
    case DI_ACTION_START:
      start(LastReasonRun(LastReasonRunDigital1 + num));
      break;
    case DI_ACTION_REVERSE:
      logEvent.add(OtherCode, AutoType, EventId(DigitalInput1LogId + num), !value, value);
      vsd->setRotation(VSD_ROTATION_REVERSE);
      break;
    case DI_ACTION_BUTTON:
      logEvent.add(OtherCode, OperatorType, EventId(DigitalInput1LogId + num), !value, value);
      break;
    case DI_ACTION_WARNING:
      logEvent.add(OtherCode, AutoType, EventId(DigitalInput1LogId + num), !value, value);
      break;
    }
  }
}

void Ccs::calcAnalogInputs()
{
  setValue(CCS_AI_1_VALUE, getValueAnalogInExt(AI1));
  setValue(CCS_AI_2_VALUE, getValueAnalogInExt(AI2));
  setValue(CCS_AI_3_VALUE, getValueAnalogInExt(AI3));
  setValue(CCS_AI_4_VALUE, getValueAnalogInExt(AI4));
}

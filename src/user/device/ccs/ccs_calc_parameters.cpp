#include "ccs.h"
#include "user_main.h"
#include "regime_main.h"
#include "gpio.h"
#include "adc_ext.h"
#include "temp_sensor.h"


#if USE_EXT_MEM
static uint16_t uValue[ADC_CNANNELS_NUM*HC_POINTS_NUM*2] __attribute__((section(".extmem")));
#else
static uint16_t uValue[ADC_CNANNELS_NUM*HC_POINTS_NUM*2];
#endif

void Ccs::calcParametersTask()
{
  int time10ms = HAL_GetTick();
  int time100ms = HAL_GetTick();
  int time500ms = HAL_GetTick();
  int time1s = HAL_GetTick();

  setModeAnalogInExt(AI1, getValue(CCS_AI_1_TYPE));
  setModeAnalogInExt(AI2, getValue(CCS_AI_2_TYPE));
  setModeAnalogInExt(AI3, getValue(CCS_AI_3_TYPE));
  setModeAnalogInExt(AI4, getValue(CCS_AI_4_TYPE));
  setModeAnalogInExt(AI6, getValue(CCS_AI_6_TYPE));

  while (1) {
    osDelay(1);

    if (isPowerOff())
      continue;

    calcDigitalInputs();

    calcTest();                           // Тестовая функция для проверки

    if ((HAL_GetTick() - time10ms) >= 10) {
      time10ms = HAL_GetTick();
    }

    if ((HAL_GetTick() - time100ms) >= 100) {
      time100ms = HAL_GetTick();

      calcInputVoltageFromAdc();
      calcInputVoltageImbalance();
      calcInputCurrentPhase1();
      calcInputCurrentPhase2();
      calcInputCurrentPhase3();
      calcInputCurrentImbalance();

      calcCurrentParam();
//      calcAsyncMotor();               // Расчёт по модели
      calcResistanceIsolation();
      calcAnalogInputs();
      calcRegimeRun();
    }

    if ((HAL_GetTick() - time500ms) >= 500) {
      time500ms = HAL_GetTick();

    }

    if ((HAL_GetTick() - time1s) >= 1000) {
      time1s = HAL_GetTick();

      calcTemperatureSTM32();
      calcTemperatureCCS();
      tms->getConnect();
      vsd->getConnect();
      em->getConnect();
    }
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
  parameters.set(CCS_COEF_TRANSFORMATION, transCoef);
  return parameters.get(CCS_COEF_TRANSFORMATION);
}

float Ccs::applyCoefTransForCurrent(float vsdCurOut)
{
  float coefTrans = parameters.get(CCS_COEF_TRANSFORMATION);
  if (coefTrans == 0)
    return vsdCurOut;
  else
    return (vsdCurOut / coefTrans);
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
  setValue(CCS_DROP_VOLTAGE_FILTER, dUf);
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

float Ccs::calcMotorSpeed()
{
  float mtrSpeed = NAN;
  if (parameters.isValidity(VSD_FREQUENCY_NOW)) {
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

float Ccs::calcInputVoltageImbalance()
{
  if (!parameters.isValidity(CCS_VOLTAGE_PHASE_1) ||
      !parameters.isValidity(CCS_VOLTAGE_PHASE_2) ||
      !parameters.isValidity(CCS_VOLTAGE_PHASE_3)) {
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

float Ccs::calcInputCurrentPhase1()
{
  if ((!parameters.isValidity(EM_CURRENT_PHASE_1)) ||
      (!parameters.isValidity(CCS_COEF_CURRENT_IN_A))) {
    setNewValue(CCS_CURRENT_PHASE_1, (float)NAN);
  }
  else {
    setValue(CCS_CURRENT_PHASE_1,
             applyCoef(parameters.get(EM_CURRENT_PHASE_1),
                       parameters.get(CCS_COEF_CURRENT_IN_A)));
  }
  return parameters.get(CCS_CURRENT_PHASE_1);
}

float Ccs::calcInputCurrentPhase2()
{
  if ((!parameters.isValidity(EM_CURRENT_PHASE_2)) ||
      (!parameters.isValidity(CCS_COEF_CURRENT_IN_B))) {
    setNewValue(CCS_CURRENT_PHASE_2, (float)NAN);
  }
  else {
    setValue(CCS_CURRENT_PHASE_2,
             applyCoef(parameters.get(EM_CURRENT_PHASE_2),
                       parameters.get(CCS_COEF_CURRENT_IN_B)));
  }
  return parameters.get(CCS_CURRENT_PHASE_2);
}

float Ccs::calcInputCurrentPhase3()
{
  if ((!parameters.isValidity(EM_CURRENT_PHASE_3)) ||
      (!parameters.isValidity(CCS_COEF_CURRENT_IN_C))) {
    setNewValue(CCS_CURRENT_PHASE_3, (float)NAN);
  }
  else {
    setValue(CCS_CURRENT_PHASE_3,
             applyCoef(parameters.get(EM_CURRENT_PHASE_3),
                       parameters.get(CCS_COEF_CURRENT_IN_C)));
  }
  return parameters.get(CCS_CURRENT_PHASE_3);
}


float Ccs::calcInputCurrentImbalance()
{
  if (!parameters.isValidity(CCS_CURRENT_PHASE_1) ||
      !parameters.isValidity(CCS_CURRENT_PHASE_2) ||
      !parameters.isValidity(CCS_CURRENT_PHASE_3)) {
    setNewValue(CCS_CURRENT_IMBALANCE_IN, (float)NAN);
  }
  else {
  setValue(CCS_CURRENT_IMBALANCE_IN,
           calcImbalance(parameters.get(CCS_CURRENT_PHASE_1),
                         parameters.get(CCS_CURRENT_PHASE_2),
                         parameters.get(CCS_CURRENT_PHASE_3),
                         1));
  }
  return parameters.get(CCS_CURRENT_IMBALANCE_IN);
}

void Ccs::calcCurrentParam()
{
  float transCoef = parameters.get(CCS_COEF_TRANSFORMATION);
  float directRun = parameters.get(CCS_RGM_RUN_DIRECT_MODE);
  float typeVsd = parameters.get(CCS_TYPE_VSD);
  float nomCurMtr = parameters.get(VSD_MOTOR_CURRENT);
  float nomCosMtr = parameters.get(VSD_MOTOR_COS_PHI);
  float cosVsd = parameters.get(VSD_MOTOR_COS_PHI_NOW);

  //! Расчёт параметров на выходе ЧРП
  // Расчёт токов на выходе ЧРП
  float vsdCurPh1 = parameters.get(VSD_CURRENT_OUT_PHASE_1);
  float vsdCurPh2 = parameters.get(VSD_CURRENT_OUT_PHASE_2);
  float vsdCurPh3 = parameters.get(VSD_CURRENT_OUT_PHASE_3);
//  float vsdCurPh  = calcAverage3Values(vsdCurPh1, vsdCurPh2, vsdCurPh3);
  float vsdVoltLn = parameters.get(VSD_VOLTAGE_LINE);

  //! Расчёт параметров на выходе СУ
  // Расчёт токов на выходе СУ
  float outCurPh1 = vsdCurPh1;
  float outCurPh2 = vsdCurPh2;
  float outCurPh3 = vsdCurPh3;
  if ((directRun) && (typeVsd != VSD_TYPE_ETALON)) {
    outCurPh1 = parameters.get(CCS_CURRENT_PHASE_1);
    outCurPh2 = parameters.get(CCS_CURRENT_PHASE_2);
    outCurPh3 = parameters.get(CCS_CURRENT_PHASE_3);
  }
  float outCurPh = calcAverage3Values(outCurPh1, outCurPh2, outCurPh3);
  // Расчёт фазных напряжений на выходе СУ
  float outVoltPh1 = vsdVoltLn / SQRT_3 - calcDropVoltageFilter(outCurPh1, -1, 1);
  float outVoltPh2 = vsdVoltLn / SQRT_3 - calcDropVoltageFilter(outCurPh2, -1, 1);
  float outVoltPh3 = vsdVoltLn / SQRT_3 - calcDropVoltageFilter(outCurPh3, -1, 1);
  if (typeVsd == VSD_TYPE_ETALON) {
    float outVoltVsd = parameters.get(VSD_ETALON_SU_OUTPUT_VOLTAGE_AVARAGE);
    outVoltPh1 = outVoltVsd / SQRT_3;
    outVoltPh2 = outVoltVsd / SQRT_3;
    outVoltPh3 = outVoltVsd / SQRT_3;
  }
  float outVoltPh  = calcAverage3Values(outVoltPh1, outVoltPh2, outVoltPh3);
  // Расчёт линейных напряжений на выходе СУ
  float outVoltLn1 = outVoltPh1 * SQRT_3;
  float outVoltLn2 = outVoltPh2 * SQRT_3;
  float outVoltLn3 = outVoltPh3 * SQRT_3;
  float outVoltLn  =  calcAverage3Values(outVoltLn1, outVoltLn2, outVoltLn3);

  //! Расчёт параметров на выходе ТМПН
  // Расчёт токов на выходе ТМПН
  float transCurPh1 = (transCoef != 0) ? outCurPh1 / transCoef : float(NAN);
  float transCurPh2 = (transCoef != 0) ? outCurPh2 / transCoef : float(NAN);
  float transCurPh3 = (transCoef != 0) ? outCurPh3 / transCoef : float(NAN);
  float transCurPh = calcAverage3Values(transCurPh1, transCurPh2, transCurPh3);
  // Расчёт фазных напряжений на выходе ТМПН
  float transVoltPh1 = outVoltPh1 * transCoef;
  float transVoltPh2 = outVoltPh2 * transCoef;
  float transVoltPh3 = outVoltPh3 * transCoef;
  float transVoltPh = calcAverage3Values(transVoltPh1, transVoltPh2, transVoltPh3);
  // Расчёт линейных напряжений на выходе ТМПН
  float transVoltLn1 = transVoltPh1 * SQRT_3;
  float transVoltLn2 = transVoltPh2 * SQRT_3;
  float transVoltLn3 = transVoltPh3 * SQRT_3;
  float transVoltLn = calcAverage3Values(transVoltLn1, transVoltLn2, transVoltLn3);

  //! Расчёт параметров двигателя
  // Расчёт токов двигателя
  float mtrCurPh1 = transCurPh1;
  float mtrCurPh2 = transCurPh2;
  float mtrCurPh3 = transCurPh3;
  float mtrCurPh = calcAverage3Values(mtrCurPh1, mtrCurPh2, mtrCurPh3);
  float mtrCurPhDisb = calcImbalance(mtrCurPh1, mtrCurPh2, mtrCurPh3, 0);
  // Расчёт фазных напряжений двигателя
  float mtrVoltPh1 = transVoltPh1 - calcDropVoltageCable(transCurPh1);
  float mtrVoltPh2 = transVoltPh2 - calcDropVoltageCable(transCurPh2);
  float mtrVoltPh3 = transVoltPh3 - calcDropVoltageCable(transCurPh3);
  float mtrVoltPh = calcAverage3Values(mtrVoltPh1, mtrVoltPh2, mtrVoltPh3);
  float mtrVoltPhDisb = calcImbalance(mtrVoltPh1, mtrVoltPh2, mtrVoltPh3, 0);
  // Расчёт линейных напряжений двигателя
  float mtrVoltLn1 = mtrVoltPh1 * SQRT_3;
  float mtrVoltLn2 = mtrVoltPh2 * SQRT_3;
  float mtrVoltLn3 = mtrVoltPh3 * SQRT_3;
  float mtrVoltLn = calcAverage3Values(mtrVoltLn1, mtrVoltLn2, mtrVoltLn3);
  // Расчёт прочих параметров двигателя
  float cosMtr = cosVsd;
  float mtrActPwr = SQRT_3 * mtrVoltLn * mtrCurPh * cosMtr;
  float loadCurPh1 = (nomCurMtr != 0) ? mtrCurPh1 / nomCurMtr * 100 : float(NAN);
  float loadCurPh2 = (nomCurMtr != 0) ? mtrCurPh2 / nomCurMtr * 100 : float(NAN);
  float loadCurPh3 = (nomCurMtr != 0) ? mtrCurPh3 / nomCurMtr * 100 : float(NAN);
  float loadCurPh = ((nomCurMtr != 0) && (nomCosMtr != 0)) ? ((mtrCurPh * cosMtr) / (nomCurMtr * nomCosMtr)) * 100 : float(NAN);

  setValue(CCS_OUTPUT_CURRENT_PHASE_1, outCurPh1);
  setValue(CCS_OUTPUT_CURRENT_PHASE_2, outCurPh2);
  setValue(CCS_OUTPUT_CURRENT_PHASE_3, outCurPh3);
  setValue(CCS_OUTPUT_CURRENT_PHASE, outCurPh);
  setValue(CCS_OUTPUT_VOLTAGE_PHASE_1, (outVoltPh1 < 0) ? 0 : outVoltPh1);
  setValue(CCS_OUTPUT_VOLTAGE_PHASE_2, (outVoltPh2 < 0) ? 0 : outVoltPh2);
  setValue(CCS_OUTPUT_VOLTAGE_PHASE_3, (outVoltPh3 < 0) ? 0 : outVoltPh3);
  setValue(CCS_OUTPUT_VOLTAGE_PHASE, (outVoltPh < 0) ? 0 : outVoltPh);
  setValue(CCS_OUTPUT_VOLTAGE_LINE_1_2, (outVoltLn1 < 0) ? 0 : outVoltLn1);
  setValue(CCS_OUTPUT_VOLTAGE_LINE_2_3, (outVoltLn2 < 0) ? 0 : outVoltLn2);
  setValue(CCS_OUTPUT_VOLTAGE_LINE_3_1, (outVoltLn3 < 0) ? 0 : outVoltLn3);
  setValue(CCS_OUTPUT_VOLTAGE_LINE, (outVoltLn < 0) ? 0 : outVoltLn);

  setValue(CCS_TRANS_OUTPUT_CURRENT_PHASE_1, transCurPh1);
  setValue(CCS_TRANS_OUTPUT_CURRENT_PHASE_2, transCurPh2);
  setValue(CCS_TRANS_OUTPUT_CURRENT_PHASE_3, transCurPh3);
  setValue(CCS_TRANS_OUTPUT_CURRENT_PHASE, transCurPh);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_PHASE_1, (transVoltPh1 < 0) ? 0 : transVoltPh1);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_PHASE_2, (transVoltPh2 < 0) ? 0 : transVoltPh2);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_PHASE_3, (transVoltPh3 < 0) ? 0 : transVoltPh3);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_PHASE, (transVoltPh < 0) ? 0 : transVoltPh);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_LINE_1_2, (transVoltLn1 < 0) ? 0 : transVoltLn1);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_LINE_2_3, (transVoltLn2 < 0) ? 0 : transVoltLn2);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_LINE_3_1, (transVoltLn3 < 0) ? 0 : transVoltLn3);
  setValue(CCS_TRANS_OUTPUT_VOLTAGE_LINE, (transVoltLn < 0) ? 0 : transVoltLn);

  setValue(CCS_MOTOR_CURRENT_PHASE_1, mtrCurPh1);
  setValue(CCS_MOTOR_CURRENT_PHASE_2, mtrCurPh2);
  setValue(CCS_MOTOR_CURRENT_PHASE_3, mtrCurPh3);
  setValue(CCS_MOTOR_CURRENT_AVARAGE, mtrCurPh);
  setValue(CCS_MOTOR_CURRENT_IMBALANCE, mtrCurPhDisb);

  setValue(CCS_MOTOR_VOLTAGE_PHASE_1, (mtrVoltPh1 < 0) ? 0 : mtrVoltPh1);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_2, (mtrVoltPh2 < 0) ? 0 : mtrVoltPh2);
  setValue(CCS_MOTOR_VOLTAGE_PHASE_3, (mtrVoltPh3 < 0) ? 0 : mtrVoltPh3);
  setValue(CCS_MOTOR_VOLTAGE_PHASE, (mtrVoltPh < 0) ? 0 : mtrVoltPh);
  setValue(CCS_MOTOR_VOLTAGE_IMBALANCE, (mtrVoltPhDisb < 0) ? 0 : mtrVoltPhDisb);

  setValue(CCS_MOTOR_VOLTAGE_LINE_1_2, (mtrVoltLn1 < 0) ? 0 : mtrVoltLn1);
  setValue(CCS_MOTOR_VOLTAGE_LINE_2_3, (mtrVoltLn2 < 0) ? 0 : mtrVoltLn2);
  setValue(CCS_MOTOR_VOLTAGE_LINE_3_1, (mtrVoltLn3 < 0) ? 0 : mtrVoltLn3);
  setValue(CCS_MOTOR_VOLTAGE_LINE, (mtrVoltLn < 0) ? 0 : mtrVoltLn);

  setValue(CCS_MOTOR_COS_PHI_NOW, cosMtr);
  setValue(CCS_MOTOR_ACTIVE_POWER, mtrActPwr);
  setValue(CCS_RGM_RUN_DIRECT_LOAD_PHASE_1, loadCurPh1);
  setValue(CCS_RGM_RUN_DIRECT_LOAD_PHASE_2, loadCurPh2);
  setValue(CCS_RGM_RUN_DIRECT_LOAD_PHASE_3, loadCurPh3);
  setValue(CCS_MOTOR_LOAD_NOW, loadCurPh);
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
  if ((parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON))
    setValue(CCS_TRANS_NEED_VOLTAGE_TAP_OFF, transTapOff);
  return parameters.get(CCS_TRANS_NEED_VOLTAGE_TAP_OFF);
}

void Ccs::calcResistanceIsolation()
{
  float resIso = 9999000;
  uint16_t source = (uint16_t)parameters.get(CCS_SOURCE_RESISTANCE_ISOLATION);
  switch (source) {
  // Автоматическое определение источника сопротивления изоляции
  case R_AUTO:
    if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_ETALON) {                      // Если в системе ЧРП Эталон
      if (vsd->isConnect() && parameters.isValidity(VSD_ETALON_RESISTANCE_ISOLATION)) { // Если есть связь с ЧРП и Параметр сопротивления изоляции валиден
        resIso = parameters.get(VSD_ETALON_RESISTANCE_ISOLATION);
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
        parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
        if (parameters.get(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION) != R_VSD) {
          parameters.set(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION, R_VSD);
        }
        return;
      }
    }
    if (parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE)  {                 // Если выбрана какая то система телеметрии
      if (tms->isConnect() &&  parameters.isValidity(TMS_RESISTANCE_ISOLATION)) {
        resIso = parameters.get(TMS_RESISTANCE_ISOLATION);
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
        parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
        if (parameters.get(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION) != R_DHS) {
          parameters.set(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION, R_DHS);
        }
        return;
      }
    }
    if (parameters.isValidity(CCS_AI_5_VALUE)) {
      float volt = getValue(CCS_AI_5_VALUE);
      if (volt > 0.0142) {// Граница "разумности" 9990 кОм, если это подставить в формулу то получим U 0.0142
        resIso = 142 / volt  - 35.42;
        resIso = resIso * 1000;
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
      }
      else {
        resIso = 9999 * 1000;
      }
      parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
      if (parameters.get(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION) != R_UKI) {
        parameters.set(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION, R_UKI);
      }
      return;
    }
    parameters.set(CCS_RESISTANCE_ISOLATION, NAN);
    parameters.setValidity(CCS_RESISTANCE_ISOLATION, err_r);
    if (parameters.get(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION) != R_NONE) {
      parameters.set(CCS_AUTO_SOURCE_RESISTANCE_ISOLATION, R_NONE);
    }
    break;
  // Источником сопротивления изоляции выбран ЧРП
  case R_VSD:
    if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_ETALON) {
      if (vsd->isConnect() && parameters.isValidity(VSD_ETALON_RESISTANCE_ISOLATION)) {
        resIso = parameters.get(VSD_ETALON_RESISTANCE_ISOLATION);
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
        parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
        return;
      }
    }
    parameters.set(CCS_RESISTANCE_ISOLATION, NAN);
    parameters.setValidity(CCS_RESISTANCE_ISOLATION, err_r);
    return;
    break;
  // Источником сопротивления изоляции выбрано ТМС
  case R_DHS:
    if (parameters.get(CCS_DHS_TYPE) != TYPE_DHS_NONE) {
      if (tms->isConnect() &&  parameters.isValidity(TMS_RESISTANCE_ISOLATION)) {
        resIso = parameters.get(TMS_RESISTANCE_ISOLATION);
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
        parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
        return;
      }
    }
    parameters.set(CCS_RESISTANCE_ISOLATION, NAN);
    parameters.setValidity(CCS_RESISTANCE_ISOLATION, err_r);
    return;
    break;
  // Источником сопротивления изоляции выбрано УКИ
  case R_UKI:
    if (parameters.isValidity(CCS_AI_5_VALUE)) {
      float volt = getValue(CCS_AI_5_VALUE);
      if (volt > 0.0142) {// Граница "разумности" 9990 кОм, если это подставить в формулу то получим U 0.0142
        resIso = 142 / volt  - 35.42;
        resIso = resIso * 1000;
        resIso = resIso + parameters.get(CCS_SHIFT_RESISTANCE_ISOLATION) * parameters.get(CCS_AXIS_SHIFT_RESISTANCE_ISOLATION);
        resIso = resIso * parameters.get(CCS_COEF_RESISTANCE_ISOLATION);
        resIso = (resIso < 0) ? 0 : resIso;
      }
      else {
        resIso = 9999 * 1000;
      }
      parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
      return;
    }
    parameters.set(CCS_RESISTANCE_ISOLATION, NAN);
    parameters.setValidity(CCS_RESISTANCE_ISOLATION, err_r);
    return;
    break;
  default:
    parameters.set(CCS_RESISTANCE_ISOLATION, resIso);
    return;
    break;
  }
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
  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction) {
     parameters.set(CCS_RUNNING_TYPE, Regime::PickupRegimeRun);
     return;
  }
  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) != Regime::OffAction) {
     parameters.set(CCS_RUNNING_TYPE, Regime::SkipFreqRegimeRun);
     return;
  }
  if (parameters.get(CCS_RGM_RUN_SYNCHRON_MODE) != Regime::OffAction) {
     parameters.set(CCS_RUNNING_TYPE, Regime::SyncRegimeRun);
     return;
  }
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) != Regime::OffAction) {
     parameters.set(CCS_RUNNING_TYPE, Regime::DirectRegimeRun);
     return;
  }
  if (parameters.get(CCS_RGM_RUN_SOFT_MODE) != Regime::OffAction) {
     parameters.set(CCS_RUNNING_TYPE, Regime::SoftRegimeRun);
     return;
  }
  parameters.set(CCS_RUNNING_TYPE, Regime::SmoothRegimeRun);
  return;
}

void Ccs::calcInputVoltageFromAdc()
{
  float uaValue = 0;
  float ubValue = 0;
  float ucValue = 0;
  float uabValue = 0;
  float ubcValue = 0;
  float ucaValue = 0;
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
      uabValue += ((uValue[0 + i*3] - 2048) - (uValue[1 + i*3] - 2048))*((uValue[0 + i*3] - 2048) - (uValue[1 + i*3] - 2048));
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
      ubcValue += ((uValue[1 + i*3] - 2048) - (uValue[2 + i*3] - 2048))*((uValue[1 + i*3] - 2048) - (uValue[2 + i*3] - 2048));
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
      ucaValue += ((uValue[2 + i*3] - 2048) - (uValue[0 + i*3] - 2048))*((uValue[2 + i*3] - 2048) - (uValue[0 + i*3] - 2048));
      count[2]++;
    }

    valueOld[0] = (uValue[0 + i*3] - 2048);
    valueOld[1] = (uValue[1 + i*3] - 2048);
    valueOld[2] = (uValue[2 + i*3] - 2048);
  }

  if (count[0]) {
    uaValue = (sqrt(uaValue/count[0]) * 627.747 * 2.5) / 0xFFF;
    uabValue = (sqrt(uabValue/count[0]) * 627.747 * 2.5) / 0xFFF;
  }
  else {
    uaValue = 0;
    uabValue = 0;
  }
  if (count[1]) {
    ubValue = (sqrt(ubValue/count[1]) * 627.747 * 2.5) / 0xFFF;
    ubcValue = (sqrt(ubcValue/count[1]) * 627.747 * 2.5) / 0xFFF;
  }
  else {
    ubValue = 0;
    ubcValue = 0;
  }
  if (count[2]) {
    ucValue = (sqrt(ucValue/count[2]) * 627.747 * 2.5) / 0xFFF;
    ucaValue = (sqrt(ucaValue/count[2]) * 627.747 * 2.5) / 0xFFF;
  }
  else {
    ucValue = 0;
    ucaValue = 0;
  }

  if (!parameters.isValidity(CCS_COEF_VOLTAGE_IN_A)) {
    setValue(CCS_VOLTAGE_PHASE_1, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_1_2, (float)NAN);
  }
  else {
    uaValue = applyCoef(uaValue, parameters.get(CCS_COEF_VOLTAGE_IN_A));
    uabValue = applyCoef(uabValue, parameters.get(CCS_COEF_VOLTAGE_IN_A));
    setValue(CCS_VOLTAGE_PHASE_1, uaValue);
    setValue(CCS_VOLTAGE_PHASE_1_2, uabValue);
  }

  if (!parameters.isValidity(CCS_COEF_VOLTAGE_IN_B)) {
    setValue(CCS_VOLTAGE_PHASE_2, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_2_3, (float)NAN);
  }
  else {
    ubValue = applyCoef(ubValue, parameters.get(CCS_COEF_VOLTAGE_IN_B));
    ubcValue = applyCoef(ubcValue, parameters.get(CCS_COEF_VOLTAGE_IN_B));
    setValue(CCS_VOLTAGE_PHASE_2, ubValue);
    setValue(CCS_VOLTAGE_PHASE_2_3, ubcValue);
  }

  if (!parameters.isValidity(CCS_COEF_VOLTAGE_IN_C)) {
    setValue(CCS_VOLTAGE_PHASE_3, (float)NAN);
    setValue(CCS_VOLTAGE_PHASE_3_1, (float)NAN);
  }
  else {
    ucValue = applyCoef(ucValue, parameters.get(CCS_COEF_VOLTAGE_IN_C));
    ucaValue = applyCoef(ucaValue, parameters.get(CCS_COEF_VOLTAGE_IN_B));
    setValue(CCS_VOLTAGE_PHASE_3, ucValue);
    setValue(CCS_VOLTAGE_PHASE_3_1, ucaValue);
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
  uint8_t value[14];
  static uint8_t valueOld[14] = {0};
  static uint16_t count[14] = {0};
  static uint16_t impulse = 0;

  for (int i = 0; i <= DI5; ++i) {
    value[i] = !getDigitalInput(i);
    if (value[i] == valueOld[i]) {
      count[i]++;
      if (count[i] >= 100) {
        count[i] = 0;
        if (i < DI5) {
          if (getValue(CCS_DI_1_VALUE + i) != value[i]) {
            if (setValue(CCS_DI_1_VALUE + i, value[i]) == ok_r)
              changedDigitalInput(i);
          }
        } else if (i == DI5) {
          setValue(CCS_DI_5_VALUE, value[i]);
          if ((parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON))
            setValue(CCS_DOOR_VALUE, !value[i]);
        }
      }
    } else {
      count[i] = 0;
    }
    valueOld[i] = value[i];
  }

  value[DI6] = !getDigitalInput(DI6);       // Получаем текущее состояние дискретного входа
  if (count[DI6] < 940) {                   // Счётчик сколько раз попадём в функцию за 1 секунду
    if (valueOld[DI6] != value[DI6])        // Если предыдущее состояние не равно текущему
      impulse++;                            // Увеличиваем счётчик переходов
    valueOld[DI6] = value[DI6];             // Запоминаем текущее состояние
    count[DI6]++;                           // Увеличиваем счётчик мс
  }
  else {                                     // Прошла 1 секунда
    if (impulse > 400)
      impulse = 400;
    if ((parameters.get(CCS_TYPE_VSD) != VSD_TYPE_ETALON)) {
      if (!isRunOrWorkMotor() || (parameters.get(CCS_RGM_RUN_PICKUP_STATE) != Regime::IdleState))
        setValue(CCS_TURBO_ROTATION_NOW, impulse / 2);
      else
        setValue(CCS_TURBO_ROTATION_NOW, 0);
    }
    setValue(CCS_DI_6_VALUE, value[DI6]);
    impulse = 0;                            // Сбрасываем количество переходов
    count[DI6] = 0;                         // Сбрасывам счётчик
  }

  setValue(CCS_DI_7_VALUE, !getDigitalInput(DI7));
  setValue(CCS_DI_8_VALUE, !getDigitalInput(DI8));
  setValue(CCS_DI_9_VALUE, !getDigitalInput(DI9));
  setValue(CCS_DI_10_VALUE, !getDigitalInput(DI10));
  setValue(CCS_DI_11_VALUE, !getDigitalInput(DI11));
  setValue(CCS_DI_12_VALUE, !getDigitalInput(DI12));
  setValue(CCS_DI_13_VALUE, !getDigitalInput(DI13));
  setValue(CCS_DI_14_VALUE, !getDigitalInput(DI14));

  setValue(CCS_BYPASS_CONTACTOR_KM1_STATE, !getValue(CCS_DI_7_VALUE));
  setValue(CCS_BYPASS_CONTACTOR_KM2_STATE, !getValue(CCS_DI_8_VALUE));
}

void Ccs::calcTest()
{
//  float a = 70.0;
//  float b = 9.869232667;
//  float c = a/b;
//  float d = c * b;
//  float e;
//  e = 2 * d;
//  e = e * e;
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
      parameters.set(VSD_ROTATION, !parameters.get(VSD_ROTATION));
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
  float value;
  float range;
  float offset;
  float minRange;
  float maxRange;

  value = (getValueAnalogInExt(AI1) + parameters.get(CCS_AI_1_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_1_ZERO_SHIFT)) * parameters.get(CCS_AI_1_COEF);
  if (value > getMax(CCS_AI_1_VALUE))
    value = getMax(CCS_AI_1_VALUE);
  setValue(CCS_AI_1_VALUE, value);

  value = (getValueAnalogInExt(AI2) + parameters.get(CCS_AI_2_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_2_ZERO_SHIFT)) * parameters.get(CCS_AI_2_COEF);
  if (value > getMax(CCS_AI_2_VALUE))
    value = getMax(CCS_AI_2_VALUE);
  setValue(CCS_AI_2_VALUE, value);

  value = (getValueAnalogInExt(AI3) + parameters.get(CCS_AI_3_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_3_ZERO_SHIFT)) * parameters.get(CCS_AI_3_COEF);
  if (value > getMax(CCS_AI_3_VALUE))
    value = getMax(CCS_AI_3_VALUE);
  setValue(CCS_AI_3_VALUE, value);

  value = (getValueAnalogInExt(AI4) + parameters.get(CCS_AI_4_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_4_ZERO_SHIFT)) * parameters.get(CCS_AI_4_COEF);
  if (value > getMax(CCS_AI_4_VALUE))
    value = getMax(CCS_AI_4_VALUE);
  setValue(CCS_AI_4_VALUE, value);

  value = (getValueAnalogInExt(AI5) + parameters.get(CCS_AI_5_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_5_ZERO_SHIFT)) * parameters.get(CCS_AI_5_COEF);
  if (value > getMax(CCS_AI_5_VALUE))
    value = getMax(CCS_AI_5_VALUE);
  setValue(CCS_AI_5_VALUE, value);

  value = (getValueAnalogInExt(AI6) + parameters.get(CCS_AI_6_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_6_ZERO_SHIFT)) * parameters.get(CCS_AI_6_COEF);
  if (value > getMax(CCS_AI_6_VALUE))
    value = getMax(CCS_AI_6_VALUE);
  setValue(CCS_AI_6_VALUE, value);

  value = (getValueAnalogInExt(AI7) + parameters.get(CCS_AI_7_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_7_ZERO_SHIFT)) * parameters.get(CCS_AI_7_COEF);
  if (value > getMax(CCS_AI_7_VALUE))
    value = getMax(CCS_AI_7_VALUE);
  setValue(CCS_AI_7_VALUE, value);

  value = (getValueAnalogInExt(AI8) + parameters.get(CCS_AI_8_AXIS_ZERO_SHIFT) * parameters.get(CCS_AI_8_ZERO_SHIFT)) * parameters.get(CCS_AI_8_COEF);
  if (value > getMax(CCS_AI_8_VALUE))
    value = getMax(CCS_AI_8_VALUE);
  setValue(CCS_AI_8_VALUE, value);

  if (getValue(CCS_AI_1_TYPE) == AnalogInModeCurrent) {
    range = 16;
    offset = 4;
  }
  else {
    range = 10;
    offset = 0;
  }
  minRange = getValue(CCS_AI_1_MINIMUM);
  maxRange = getValue(CCS_AI_1_MAXIMUM);
  value = getValue(CCS_AI_1_VALUE);
  value = (minRange + (maxRange - minRange) * ((value - offset) / range));
  setValue(CCS_AI_1_VALUE_CALC, value);

  if (getValue(CCS_AI_2_TYPE) == AnalogInModeCurrent) {
    range = 16;
    offset = 4;
  }
  else {
    range = 10;
    offset = 0;
  }
  minRange = getValue(CCS_AI_2_MINIMUM);
  maxRange = getValue(CCS_AI_2_MAXIMUM);
  value = getValue(CCS_AI_2_VALUE);
  value = (minRange + (maxRange - minRange) * ((value - offset) / range));
  setValue(CCS_AI_2_VALUE_CALC, value);

  if (getValue(CCS_AI_3_TYPE) == AnalogInModeCurrent) {
    range = 16;
    offset = 4;
  }
  else {
    range = 10;
    offset = 0;
  }
  minRange = getValue(CCS_AI_3_MINIMUM);
  maxRange = getValue(CCS_AI_3_MAXIMUM);
  value = getValue(CCS_AI_3_VALUE);
  value = (minRange + (maxRange - minRange) * ((value - offset) / range));
  setValue(CCS_AI_3_VALUE_CALC, value);

  if (getValue(CCS_AI_4_TYPE) == AnalogInModeCurrent) {
    range = 16;
    offset = 4;
  }
  else {
    range = 10;
    offset = 0;
  }
  minRange = getValue(CCS_AI_4_MINIMUM);
  maxRange = getValue(CCS_AI_4_MAXIMUM);
  value = getValue(CCS_AI_4_VALUE);
  value = (minRange + (maxRange - minRange) * ((value - offset) / range));
  setValue(CCS_AI_4_VALUE_CALC, value);
}

void Ccs::calcTemperatureSTM32()
{
  float temp = -1;
  StatusType status;
  status = getCoreTemperature(&temp);
  if (status == StatusOk)
    setValue(CCS_TEMPERATURE_STM32, temp);
}

void Ccs::calcTemperatureCCS()
{
  float temp = tempSensorReadData();
  setValue(CCS_TEMPERATURE_CCS, temp);
}

void Ccs::calcTurboRotation()
{

}

void Ccs::calcAsyncMotor()
{
  float tPiInF = 2 * NUM_PI * parameters.get(CCS_TRANS_NOMINAL_VOLTAGE_INPUT); if (tPiInF == 0) return;
  float cTrans = parameters.get(CCS_COEF_TRANSFORMATION); if (cTrans == 0) return;
  float sCable = parameters.get(CCS_TRANS_CABLE_CROSS); if (sCable == 0) return;
  float xfTran = parameters.get(CCS_TRANS_FULL_RESISTANCE_PHASE);
  float rfTran = parameters.get(CCS_TRANS_ACTIVE_RESISTANCE_PHASE);
  float xlTran = sqrt(xfTran * xfTran - rfTran * rfTran);
  float lTrans = xlTran / tPiInF;
  float cTran2 = cTrans * cTrans;
  float iTrans = lTrans * cTran2;
  float cosPhi = parameters.get(CCS_MOTOR_COS_PHI_NOW);
  float sinPhi = sqrt(1 - cosPhi * cosPhi);
  float twoPiF = 2 * NUM_PI * parameters.get(VSD_FREQUENCY_NOW);
  float lCable = parameters.get(CCS_TRANS_CABLE_LENGHT);
  float iCable = lCable * parameters.get(CCS_CABLE_INDUCTANCE) / 1000000.0;
  float emfMtr = parameters.get(CCS_EMF_MOTOR);
  float indFtr = parameters.get(CCS_FILTER_INDUCTANCE);
  float indMtr = parameters.get(CCS_MOTOR_INDUCTANCE) / 1000.0;
  float rTrans = rfTran * cTran2;
  float rMotor = parameters.get(VSD_MOTOR_ACTIVE_RESIST) * (1 + 0.0038 * (20 - 20));
  float rCable = lCable / (sCable * 58);
  float rTempr = rCable * (1 + 0.0038 * (parameters.get(CCS_TEMP_WATER_OUT_OF_SYNC) - 20));

  for (int i = 0; i < 3; i++) {
    calcAsync_.Ut       = calcAsync_.UVSD * cosPhi;  if (calcAsync_.Ut == 0) return;
    calcAsync_.Un       = calcAsync_.UVSD * sinPhi;
    calcAsync_.IVSD[i]  = parameters.get(VSD_CURRENT_OUT_PHASE_1 + i);
    calcAsync_.ULn      = SQRT_3 * NUM_PI * (indFtr / 1000) * calcAsync_.IVSD[i];
    calcAsync_.dUn      = calcAsync_.Un - calcAsync_.ULn;
    calcAsync_.Usu[i]   = sqrt(calcAsync_.Ut * calcAsync_.Ut + calcAsync_.dUn * calcAsync_.dUn);
    calcAsync_.dFi      = atan(calcAsync_.dUn / calcAsync_.Ut);
    calcAsync_.FiL      = acos(cosPhi) - calcAsync_.dFi;
    calcAsync_.Cf       = parameters.get(CCS_FILTER_CAPACITY) / 1000000;
    calcAsync_.Isf      = (calcAsync_.Usu[i] / SQRT_3) * twoPiF * calcAsync_.Cf;
    calcAsync_.In[i]    = calcAsync_.IVSD[i] * sin(calcAsync_.dFi);
    calcAsync_.dIn      = calcAsync_.In[i] + calcAsync_.Isf;
    calcAsync_.It       = calcAsync_.IVSD[i] * cos(calcAsync_.dFi);  if (calcAsync_.It == 0) return;
    calcAsync_.IH[i]    = sqrt(calcAsync_.It * calcAsync_.It + calcAsync_.dIn * calcAsync_.dIn);
    calcAsync_.FiH      = atan(calcAsync_.dIn / calcAsync_.It);
    calcAsync_.dFiH     = calcAsync_.FiH - calcAsync_.dFi;
    calcAsync_.Un       = calcAsync_.Usu[i] * cTrans * sin(calcAsync_.FiH);
    calcAsync_.LH       = calcSystemInduct();
    calcAsync_.LH       = calcAsync_.LH - indFtr;
    calcAsync_.ULn      = SQRT_3 * twoPiF * (iTrans + iCable + indMtr) * calcAsync_.IH[i] / cTrans;
    calcAsync_.dUn      = calcAsync_.Un - calcAsync_.ULn;
    calcAsync_.Ut       = calcAsync_.Usu[i] * cTrans * cos(calcAsync_.FiH);
    calcAsync_.Rn       = (38.7 * lCable / (sCable * 1000));
    calcAsync_.Rn       = rTrans + (rCable + rTempr) / 2 + rMotor;
    calcAsync_.ULped[i] = SQRT_3 * twoPiF * indMtr * calcAsync_.IH[i] / cTrans;
    calcAsync_.URn      = SQRT_3 * calcAsync_.Rn * calcAsync_.IH[i] / cTrans;
    calcAsync_.dUt      = calcAsync_.Ut - calcAsync_.URn; if (calcAsync_.dUt == 0) return;
    calcAsync_.Psi      = atan(calcAsync_.dUn / calcAsync_.dUt) * 180 / NUM_PI;
    calcAsync_.EMF[i]   = sqrt(calcAsync_.dUn * calcAsync_.dUn + calcAsync_.dUt * calcAsync_.dUt);
    calcAsync_.Uped[i]  = sqrt((calcAsync_.ULped[i] + calcAsync_.dUn) * (calcAsync_.ULped[i] + calcAsync_.dUn) + (calcAsync_.dUt * calcAsync_.dUt));
    calcAsync_.flEMF    = (emfMtr > 0.0001) && (parameters.get(CCS_TRANS_VOLTAGE_SHORT_CIRCUIT) > 0.001) && (parameters.get(CCS_TRANS_NOMINAL_CURRENT) > 0.1);
  }
  calcAsync_.EMFaver = (calcAsync_.EMF[0] + calcAsync_.EMF[1] + calcAsync_.EMF[2]) / 3;
  parameters.set(CCS_EMF_MODEL, calcAsync_.EMFaver);
  calcAsync_.EMFModel = emfMtr * parameters.get(VSD_SPEED_RPM_NOW);
  parameters.set(CCS_OUTPUT_VOLTAGE_LINE_1_2, calcAsync_.Usu[0]);
  parameters.set(CCS_OUTPUT_VOLTAGE_LINE_2_3, calcAsync_.Usu[1]);
  parameters.set(CCS_OUTPUT_VOLTAGE_LINE_3_1, calcAsync_.Usu[2]);
  parameters.set(CCS_OUTPUT_VOLTAGE_LINE, (calcAsync_.Usu[0] + calcAsync_.Usu[1] + calcAsync_.Usu[2]) / 3);
  parameters.set(CCS_OUTPUT_CURRENT_PHASE_1, calcAsync_.IH[0]);
  parameters.set(CCS_OUTPUT_CURRENT_PHASE_2, calcAsync_.IH[1]);
  parameters.set(CCS_OUTPUT_CURRENT_PHASE_3, calcAsync_.IH[2]);
  parameters.set(CCS_TRANS_OUTPUT_VOLTAGE_LINE_1_2, calcAsync_.Usu[0] * cTrans);
  parameters.set(CCS_TRANS_OUTPUT_VOLTAGE_LINE_2_3, calcAsync_.Usu[1] * cTrans);
  parameters.set(CCS_TRANS_OUTPUT_VOLTAGE_LINE_3_1, calcAsync_.Usu[2] * cTrans);
  parameters.set(CCS_TRANS_OUTPUT_VOLTAGE_LINE, (calcAsync_.Usu[0] + calcAsync_.Usu[1] + calcAsync_.Usu[2]) / 3 * cTrans);
  parameters.set(CCS_TRANS_OUTPUT_CURRENT_PHASE_1, calcAsync_.IH[0] / cTrans);
  parameters.set(CCS_TRANS_OUTPUT_CURRENT_PHASE_2, calcAsync_.IH[1] / cTrans);
  parameters.set(CCS_TRANS_OUTPUT_CURRENT_PHASE_3, calcAsync_.IH[2] / cTrans);
  parameters.set(CCS_TRANS_OUTPUT_COS, (float)cos(calcAsync_.FiH));
}



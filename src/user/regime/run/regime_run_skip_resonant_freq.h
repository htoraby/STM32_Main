#ifndef REGIMERUNSKIPRESONANTFREQ_H
#define REGIMERUNSKIPRESONANTFREQ_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "Пропуск резонансных частот"
 */

class RegimeRunSkipResonantFreq : public RegimeRun
{
public:
  RegimeRunSkipResonantFreq();
  virtual ~RegimeRunSkipResonantFreq();

private:
  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();
  void saveBeforeRegimeRun();
  int16_t setBeginFreq();
  int16_t setMinFreq();
  int16_t setTempSkip();
  int16_t setEndFreq();
  int16_t returnTemp();
  int16_t returnFreq();
  int16_t returnMinFreq();
  int16_t setConfirmation(uint16_t id, float value);
  void offRegime();
  float delay_;
  float repeat_;
  float beginFreq_;
  float endFreq_;
  float time_;
  float freq_;
  float setpointFreq_;
  float rememberFreq_;
  float tempBeginFreq_;
  float tempEndFreq_;

};

#endif // REGIMERUNSKIPRESONANTFREQ_H

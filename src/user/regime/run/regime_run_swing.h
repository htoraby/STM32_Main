#ifndef REGIMERUNSWING_H
#define REGIMERUNSWING_H

#include "regime_run.h"

class RegimeRunSwing : public RegimeRun
{
public:
  RegimeRunSwing();
  virtual ~RegimeRunSwing();

  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();

private:
  void onRegime();
  bool checkOnRegime();
  void offRegime();

  float freq_;                                        // Частота качка
  float quantity_;                                    // Количество качков
  float voltage_;                                     // Напряжение качков
  float rotation_;                                    // Направление вращения
  float cntReverse_;
};

#endif // REGIMERUNSWING_H

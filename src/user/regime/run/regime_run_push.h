#ifndef REGIMERUNPUSH_H
#define REGIMERUNPUSH_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "Толчковый"
 */
class RegimeRunPush: public RegimeRun
{
public:
  RegimeRunPush();
  virtual ~RegimeRunPush();

  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void automatRegime();

private:
  void offRegime();
  bool checkOnRegime();
  void onRegime();

  void makePush(bool push);

  float freq_;                                        // Частота толчка
  float time_;                                        // Время толчка и между толчками
  float quantity_;                                    // Количество толчков
  float voltage_;                                     // Напряжение толчка

  bool flag_;                                         // Флаг
  uint16_t cntPush_;                                  // Счётчик толчков
  float timer_;                                       // Счётчик времени
  float setPointFreq_;                                // Уставка частоты
  float setPointLowLimFreq_;                          // Уставка минимальной частоты
  float setPointU1_;                                  // Уставка напряжения в нижней точке Uf
  float setPointU2_;                                  // Уставка напряжения в верхней точке Uf
};

#endif // REGIMERUNPUSH_H

#ifndef REGIMETECHNOLOGOPTIMIZATIONVOLTAGE_H
#define REGIMETECHNOLOGOPTIMIZATIONVOLTAGE_H

#include "regime_technolog.h"

/*!
 * \brief The RegimeTechnologOptimizationVoltage class
 * Технологический режим работы, предназначен для оптимизации выходного напряжения
 * преобразователя частоты с целью уменьшения непроизводительных потерь электрической
 * энергии.
 */
class RegimeTechnologOptimizationVoltage : public RegimeTechnolog
{
public:
  RegimeTechnologOptimizationVoltage();
  ~RegimeTechnologOptimizationVoltage();

  void processing();

private:

  uint32_t delay_;                                    //! Уставка "Первый цикл после запуска"
  uint32_t period_;                                   //! Периодичность оптимизации
  uint32_t beginPeriod_;                              //! Время начала периода оптимизации
  float scale_;                                       //! Максимальное отклонение
  float step_;                                        //! Шаг оптимизации

  uint16_t idUfLowPoint_;                             //! Точка U/f верхняя
  uint16_t idUfHiPoint_;                              //! Точка U/f нижняя

  float valueUfLowPoint_;                             //!
  float valueUfHiPoint_;

  float limUpUfLowPoint_;
  float limDownUfLowPoint_;
  float limUpUfHiPoint_;
  float limDownUfHiPoint_;

  float maxUfPoint_;

  float oldCurrent_;
  float newCurrent_;
  uint16_t cntCurrent_;


};

#endif // REGIMETECHNOLOGOPTIMIZATIONVOLTAGE_H

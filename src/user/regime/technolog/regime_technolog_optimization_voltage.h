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
  void saveUfBeforeOptim();
  void loadUfAfterOptim();
  void createOptim(bool first);

private:
  // Уставки режима
  uint32_t delay_;                                    //! Уставка "Первый цикл после запуска"
  float scale_;                                       //! Максимальное отклонение
  float step_;                                        //! Шаг оптимизации

  // Скрытые настройки
  float recalcLimits_;                                //! Пересчитывать границы от текущей характеристики U/f
  float beginUp_;                                     //! Первый шаг 0-вниз, 1-вверх
  float delta_;                                       //! Дельта изменения тока для принятия решения
  uint32_t timeCur_;                                  //! Время усреднения тока

  // Прочие уставки станции
  float maxUfPoint_;                                  //! Максимальное рабочее напряжение

  // Флаги и переменные
  uint32_t beginPause_;                               //! Время начала периода паузы между оптимизациями
  uint32_t beginCurrent_;                             //! Время начала усреднения тока

  float oldCurrent_;                                  //! Значение тока до шага оптимизации
  float newCurrent_;                                  //! Значения тока после шага оптимизации
  uint16_t cntCurrent_;                               //! Счётчик выборок для усреднения тока

  /*!
   * \brief Массив с параметрами на время работы режима строки точки U/f,
   * столбцы: id f(U/f), id U(U/f), f, U, minU, maxU
   */
  float optim_[6][6];
};

#endif // REGIMETECHNOLOGOPTIMIZATIONVOLTAGE_H

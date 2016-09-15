#ifndef REGIMERUNSYNCHRON_H
#define REGIMERUNSYNCHRON_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "С синхронизацией"
 */
class RegimeRunSynchron : public RegimeRun
{
public:
  RegimeRunSynchron();
  virtual ~RegimeRunSynchron();

private:
  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();
  void saveBeforeRegimeRun();
  void offRegime();

  /*!
   * \brief setMinFreq функция задания минимальной границы уставки частоты для
   * обеспечения возможности задания частоты синхронизации как уставки частоты
   * даже если она меньше обычной уставки минимальной частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setMinFreq();

  /*!
   * \brief returnMinFreq функция возвращения значения минимальной границы уставки
   * частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnMinFreq();

  /*!
   * \brief setFreqSynchron функция задания в уставку частоты значения частоты
   * синхронизации из режима пуска с синхронизацией
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setFreqSynchron();

  /*!
   * \brief returnFreq функция возвращения значения уставки частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnFreq();

  /*!
   * \brief setConfirmation
   * \param id
   * \param value
   * \return
   */
  int16_t setConfirmation(uint16_t id, float value);

  float delay_;                             // Задержка между попытками записать настройки режима в ЧРП
  float repeat_;                            // Количество попыток записать настройки режима в ЧРП
  uint32_t beginSync_;                      // Время выхода на частоту синхронизации


};

#endif // REGIMERUNSYNCHRON_H

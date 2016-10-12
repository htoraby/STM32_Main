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
  void processingStateStop();
  void processingStateError();
  void automatRegime();

private:
  /*!
   * \brief saveBeforeRegimeRun
   * Функция сохранения настроек работы ЧРП перед запуском режима
   */
  void saveBeforeRegimeRun();

  /*!
   * \brief setMinFreq функция задания минимальной границы уставки частоты для
   * обеспечения возможности задания частоты синхронизации как уставки частоты
   * даже если она меньше обычной уставки минимальной частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setMinFreq();

  /*!
   * \brief returnMinFreq функция возвращения минимальной уставки частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnMinFreq();

  /*!
   * \brief setFreqPush функция задания частоты толчков
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setFreqPush();

  /*!
   * \brief returnFreq функция возвращения уставки частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnFreq();

  /*!
   * \brief setU1
   * \return
   */
  int16_t setU1();

  /*!
   * \brief returnU1
   */
  int16_t returnU1();

  /*!
   * \brief setU2
   * \return
   */
  int16_t setU2();

  /*!
   * \brief returnU2
   * \return
   */
  int16_t returnU2();

  /*!
   * \brief setConfirmation
   * \param id
   * \param value
   * \return
   */
  int16_t setConfirmation(uint16_t id, float value);

  // Уставки режима
  float freqPush_;                                    // Частота толчка
  float quantityPush_;                                // Количество толчков
  float voltagePush_;                                 // Напряжение толчка
  float timePush_;                                    // Время толчка и между толчками

  uint16_t cntPush_;                                  // Счётчик толчков
  float timer_;                                       // Счётчик времени

  // Запись параметров в ЧРП
  float delay_;                                       // Задержка между попытками записать настройки режима в ЧРП
  float repeat_;                                      // Количество попыток записать настройки режима в ЧРП
};

#endif // REGIMERUNPUSH_H

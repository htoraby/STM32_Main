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
   * \brief setFreqSwing функция задания частоты толчков
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setFreqSwing();

  /*!
   * \brief returnFreq функция возвращения уставки частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnFreq();

  /*!
   * \brief setTimeDispersal функция задания времени набора частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t setTimeDispersal();

  /*!
   * \brief returnTimeDispersal функция возврата времени набора частоты
   * \return 0 - нет ошибок; 1 - ошибка задания
   */
  int16_t returnTimeDispersal();

  /*!
   * \brief setU1
   * \return
   */
  int16_t setU1();

  /*!
   * \brief returnU1
   * \return
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
   * \brief setRotation
   * \return
   */
  int16_t setRotation();

  /*!
   * \brief returnRotation
   * \return
   */
  int16_t returnRotation();

  /*!
   * \brief setConfirmation
   * \param id
   * \param value
   * \return
   */
  int16_t setConfirmation(uint16_t id, float value);

  // Уставки режима
  float freqSwing_;                                   // Частота качка
  float quantitySwing_;                               // Количество качков
  float voltageSwing_;                                // Напряжение качков

  // Текущие параметры
  float cntReverse_;
  float rotationNow_;                                 // Текущее направление вращения
  float rotationSave_;

  // Cчётчики
  float update_;                                      // Таймер счётчик обновления параметров
  float delayRotation_;                               // Таймер на изменение направления вращения
  // Сохраненное направление вращения
  // Запись параметров в ЧРП
  float delay_;                                       // Задержка между попытками записать настройки режима в ЧРП
  float repeat_;                                      // Количество попыток записать настройки режима в ЧРП


};

#endif // REGIMERUNSWING_H

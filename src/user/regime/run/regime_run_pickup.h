#ifndef REGIMERUNPICKUP_H
#define REGIMERUNPICKUP_H

#include "regime_run.h"

/*!
 * \brief Класс режима пуска "Подхват"
 */
class RegimeRunPickup: public RegimeRun
{
public:
  RegimeRunPickup();
  virtual ~RegimeRunPickup();

  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateRunning();
  void processingStateWork();
  void processingStateStop();
  void automatRegime();

private:
  void saveBeforeRegimeRun();
  // Задание направления вращения, равного направлению турбинного вращения
  int16_t setRotation();
  // Задание направления вращения, до турбинного вращения
  int16_t returnRotation();
  // Задание минимальной частоты, в 1
  int16_t setMinFreq();
  // Задание минимальной частоты,
  int16_t returnMinFreq();
  // Задание уставки частоты, равной "Частоте ожидания"
  int16_t setFreqWait();
  // Заданиче уставки частоты, равной сохранённой частоте уставки
  int16_t returnFreq();
  // Задание времени изменения частоты, на "Время разгона" параметра подхвата
  int16_t setTimeDispersal();
  // Задание времени изменения частоты, на "Время торможения" параметра подхвата
  int16_t setTimeDelay();
  // Задание времени изменения частоты, на "Время разгона до частоты уставки"
  int16_t returnTimeDispersal();

  int16_t setConfirmation(uint16_t id, float value);
  // Запись параметров в ЧРП
  float delay_;                                       // Задержка между попытками записать настройки режима в ЧРП
  float repeat_;                                      // Количество попыток записать настройки режима в ЧРП
  float oldSpeed_;
  float nowSpeed_;
};

#endif // REGIMERUNPICKUP_H

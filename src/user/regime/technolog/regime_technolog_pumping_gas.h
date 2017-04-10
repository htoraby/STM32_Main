#ifndef REGIMETECHNOLOGPUMPINGGAS_H
#define REGIMETECHNOLOGPUMPINGGAS_H

#include "regime_technolog.h"

class RegimeTechnologPumpingGas : public RegimeTechnolog
{
public:
  RegimeTechnologPumpingGas();
  ~RegimeTechnologPumpingGas();

private:
  /*!
   * \brief Основной цикл работы режима
   */
  void processing();

  /*!
   * \brief Функция сохранения уставок перед началом работы режима
   */
  void saveBeforePumping();

  /*!
   * \brief Функция возвращения уставок после окончания работы режима
   */
  void loadAfterPumping();

  /*!
   * \brief setF1
   * \return
   */
  int16_t setF1();

  /*!
   * \brief setF2
   * \return
   */
  int16_t setF2();

  /*!
   * \brief returnFreq
   * \return
   */
  int16_t returnFreq();

  int16_t setConfirmation(uint16_t id, float value);

  bool isNeedPumping();

  // Уставки режима - визуальные входные данные
  float firstFreq_;                         //!< F1 пркч выходная верхняя частота СУ ЧР в режиме прокачки газа
  float secondFreq_;                        //!< F2 пркч выходная нижняя частота СУ ЧР в режиме прокачки газа
  float time_;                              //!< T пркч время работы на частоте прокачки в режиме прокачки газа
  float cicle_;                             //!< Количество циклов прокачки газа
  // Уставки режима - скрытые настройки
  float activDelay_;                        //!< Время активации режима = времени активации защиты по недогрузу
  float tripDelay_;                         //!< Задержка на включение режима (защита от ложных срабатываний)
  float protTripSetpoint_;                  //!< Уставка срабатывания защиты по недогрузу
  float incTripSetpoint_;                   //!< Прибавка к уставке срабатывания защиты по недогрузу

  // Текущие параметры
  uint32_t tripTime_;                      //!< Время начала недогруза
  uint32_t beginTime_;                     //!< Время начала T пркч
  float countCicle_;                       //!< Текущее количество циклов прокачки газа

  // Запись параметров в ЧРП
  float delay_;                            //!< Задержка между попытками записать настройки режима в ЧРП
  float repeat_;                           //!< Количество попыток записать настройки режима в ЧРП

};

#endif // REGIMETECHNOLOGPUMPINGGAS_H

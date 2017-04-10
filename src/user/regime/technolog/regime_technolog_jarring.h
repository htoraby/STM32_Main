#ifndef REGIMETECHNOLOGJARRING_H
#define REGIMETECHNOLOGJARRING_H

#include "regime_technolog.h"

class RegimeTechnologJarring : public RegimeTechnolog
{
public:
  RegimeTechnologJarring();
  ~RegimeTechnologJarring();

private:
  void processing();

  /*!
   * \brief Функция сохранения параметров перед встряхиванием
   */
  void saveBeforeJarring();

  /*!
   * \brief Функция загрузки параметров после встряхивания
   */
  void loadAfterJarring();

  /*!
   * \brief Функция изменения дополнительных параметров режима встряхивания
   */
  void configJarring();

  float firstFreq_;           //!< Первая частота до которой будет изменение в режиме встрахивания
  float secondFreq_;          //!< Вторая частота до которой будет изменение в режиме встрахивания
  float period_;              //!< Период встряхивания
  float countJar_;            //!< Количество встряхиваний
  float upTemp_;              //!< Темп набора частоты
  float downTemp_;            //!< Темп снижения частоты

  uint32_t beginPeriod_;      //!< Время начала периода
  float count_;               //!< Текущее количество встряхиваний
  uint16_t delay_;

};

#endif // REGIMETECHNOLOGJARRING_H

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

  float firstFreq_;           //!< Первая частота до которой будет изменение в режиме встрахивания
  float secondFreq_;          //!< Вторая частота до которой будет изменение в режиме встрахивания
  float periodJar_;           //!< Период встряхивания
  float countJar_;            //!< Количество встряхиваний
  float upTemp_;              //!< Темп набора частоты
  float downTemp_;            //!< Темп снижения частоты

  uint32_t beginTime_;        //!< Время начала периода

};

#endif // REGIMETECHNOLOGJARRING_H

#ifndef REGIMETECHNOLOGALTERNATIONFREQ_H
#define REGIMETECHNOLOGALTERNATIONFREQ_H

#include "regime_technolog.h"

class RegimeTechnologAlternationFreq : public RegimeTechnolog
{
public:
  RegimeTechnologAlternationFreq();
  ~RegimeTechnologAlternationFreq();

private:
  void processing();

  float firstFreq_;                         //!< Первая рабочая частота
  float secondFreq_;                        //!< Вторая рабочая частота
  float timeFirstFreq_;                     //!< Время работы на первой частоте
  float timeSecondFreq_;                    //!< Время работы на второй частоте

  uint32_t beginTime_;

};

#endif // REGIMETECHNOLOGALTERNATIONFREQ_H

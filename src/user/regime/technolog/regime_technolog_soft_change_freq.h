#ifndef REGIMETECHNOLOGSOFTCHANGEFREQ_H
#define REGIMETECHNOLOGSOFTCHANGEFREQ_H

#include "regime_technolog.h"

/*!
 * \brief Класс режима "Программное изменение частоты"
 */
class RegimeTechnologSoftChangeFreq : public RegimeTechnolog
{
public:
  RegimeTechnologSoftChangeFreq();
  ~RegimeTechnologSoftChangeFreq();

  void processing();

private:
  float beginFreq_;
  float endFreq_;
  float period_one_step_;

  uint32_t beginTime_;

};

#endif // REGIMETECHNOLOGSOFTCHANGEFREQ_H

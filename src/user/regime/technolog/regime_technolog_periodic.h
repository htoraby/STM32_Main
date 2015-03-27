#ifndef REGIMETECHNOLOGPERIODIC_H
#define REGIMETECHNOLOGPERIODIC_H

#include "regime_technolog.h"

/*!
 * \brief Класс режима "Периодический"
 */
class RegimeTechnologPeriodic : public RegimeTechnolog
{
public:
  RegimeTechnologPeriodic();
  ~RegimeTechnologPeriodic();

  void processing();

private:
  float workPeriod_;       //!< Уставка: Время работы
  float stopPeriod_;       //!< Уставка: Время останова
  uint32_t workBeginTime_; //!< Время начала работы
  uint32_t stopBeginTime_; //!< Время начала останова
  float workTimeToEnd_;    //!< Время до конца работы
  float stopTimeToEnd_;    //!< Время до конца останова

};

#endif // REGIMETECHNOLOGPERIODIC_H

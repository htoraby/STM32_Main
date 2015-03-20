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
  float workTime_;      //!< Уставка: Время работы
  float stopTime_;      //!< Уставка: Время останова
  float workTimer_;     //!< Таймер работы
  float stopTimer_;     //!< Таймер останова
  float workTimeEnd_;   //!< Время до конца работы
  float stopTimeEnd_;   //!< Время до конца останова

};

#endif // REGIMETECHNOLOGPERIODIC_H

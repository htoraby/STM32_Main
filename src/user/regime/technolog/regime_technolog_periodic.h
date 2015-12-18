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
  uint32_t workPeriod_;       //!< Уставка: Время работы
  uint32_t stopPeriod_;       //!< Уставка: Время останова
  uint32_t workBeginTime_;    //!< Время начала работы
  uint32_t stopBeginTime_;    //!< Время начала останова
  float workTimeToEnd_;       //!< Время до конца работы
  float stopTimeToEnd_;       //!< Время до конца останова

  bool isInit_;               //!< Флаг инициализации
  bool attempt_;              //!< Флаг первой попытки запуска
  int addTime_;               //!< Добавочное время к паузе

};

#endif // REGIMETECHNOLOGPERIODIC_H

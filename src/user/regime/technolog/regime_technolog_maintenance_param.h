#ifndef REGIMETECHNOLOGMAINTENANCEPARAM_H
#define REGIMETECHNOLOGMAINTENANCEPARAM_H

#include "regime_technolog.h"

/*!
 * \brief Класс режима "Поддержание параметра"
 */
class RegimeTechnologMaintenanceParam : public RegimeTechnolog
{
public:
  /*!
   * \brief Тип поддерживаемого параметра
  */
  typedef enum {
    CurrentParam,     //!< Ток
    PressureParam,    //!< Давление
    TemperatureParam, //!< Температура
    AnalogIn1Param,   //!< Аналоговый вход 1
    AnalogIn2Param,   //!< Аналоговый вход 2
    AnalogIn3Param,   //!< Аналоговый вход 3
    AnalogIn4Param,   //!< Аналоговый вход 4
  } TypeParam;

  RegimeTechnologMaintenanceParam();
  ~RegimeTechnologMaintenanceParam();

  void processing();

private:
  uint32_t timer_;
  float delta_;

};

#endif // REGIMETECHNOLOGMAINTENANCEPARAM_H

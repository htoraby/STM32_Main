#ifndef REGIMETECHNOLOGMAINTENANCEPARAM_H
#define REGIMETECHNOLOGMAINTENANCEPARAM_H

#include "regime_technolog.h"

/*!
 * \brief Класс режима "Поддержание параметра"
 */
class RegimeTechnologMaintenanceParam : public RegimeTechnolog
{
public:
  RegimeTechnologMaintenanceParam();
  ~RegimeTechnologMaintenanceParam();

  void processing();

private:
  uint32_t timer_;
  float delta_;
  int deltaCount_;

};

#endif // REGIMETECHNOLOGMAINTENANCEPARAM_H

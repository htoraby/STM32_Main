#ifndef SCADATELESCOP_H
#define SCADATELESCOP_H

#include "scada.h"

class ScadaTelescop : public Scada
{
public:
  ScadaTelescop();
  ~ScadaTelescop();

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADATELESCOP_H

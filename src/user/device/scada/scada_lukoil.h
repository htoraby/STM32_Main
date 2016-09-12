#ifndef SCADALUKOIL_H
#define SCADALUKOIL_H

#include "scada.h"

class ScadaLukoil : public Scada
{
public:
  ScadaLukoil();
  ~ScadaLukoil();

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);
  void calcDateTime(time_t time, ScadaParameter *params);

};

#endif // SCADALUKOIL_H

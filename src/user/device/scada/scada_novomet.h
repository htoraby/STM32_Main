#ifndef SCADANOVOMET_H
#define SCADANOVOMET_H

#include "scada.h"

class ScadaNovomet : public Scada
{
public:
  ScadaNovomet();
  ~ScadaNovomet();

private:
  /*!
   * \brief Инициализация параметров скады Новомет
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADANOVOMET_H

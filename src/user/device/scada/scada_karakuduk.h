#ifndef SCADAKARAKUDUK_H
#define SCADAKARAKUDUK_H

#include "scada.h"

class ScadaKarakuduk : public Scada
{
public:
  ScadaKarakuduk();
  ~ScadaKarakuduk();

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADAKARAKUDUK_H

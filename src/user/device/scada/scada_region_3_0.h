#ifndef SCADAREGION30_H
#define SCADAREGION30_H

#include "scada.h"

class ScadaRegion30 : public Scada
{
public:
  ScadaRegion30();
  ~ScadaRegion30();

private:
  /*!
   * \brief Инициализация параметров скады Регион 3.0
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADAREGION30_H

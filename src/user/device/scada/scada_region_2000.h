#ifndef SCADAREGION2000_H
#define SCADAREGION2000_H

#include "scada.h"

class ScadaRegion2000 : public Scada
{
public:
  ScadaRegion2000();
  ~ScadaRegion2000();

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADAREGION2000_H

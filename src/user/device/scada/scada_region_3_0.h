#ifndef SCADAREGION30_H
#define SCADAREGION30_H

#include "scada.h"

class ScadaRegion30 : public Scada
{
public:
  ScadaRegion30();
  ~ScadaRegion30();

private:
  void initParameters();
  void calcParametersTask();


};

#endif // SCADAREGION30_H

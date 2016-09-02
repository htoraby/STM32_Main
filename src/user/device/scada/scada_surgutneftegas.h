#ifndef SCADASURGUTNEFTEGAS_H
#define SCADASURGUTNEFTEGAS_H

#include "scada.h"

class ScadaSurgutneftegas : public Scada
{
public:
  ScadaSurgutneftegas();
  ~ScadaSurgutneftegas();

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

};

#endif // SCADASURGUTNEFTEGAS_H

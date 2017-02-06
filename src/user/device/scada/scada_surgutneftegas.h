#ifndef SCADASURGUTNEFTEGAS_H
#define SCADASURGUTNEFTEGAS_H

#include "scada.h"

class ScadaSurgutneftegas : public Scada
{
public:
  ScadaSurgutneftegas();
  ~ScadaSurgutneftegas();

  uint16_t getParam(uint16_t address, uint8_t *pucFrame, uint16_t *usLen);

  int setNewValue(ScadaParameter *param);

private:
  /*!
   * \brief Инициализация параметров скады
   */
  void initParameters();
  void calcParamsTask();


};

#endif // SCADASURGUTNEFTEGAS_H

#ifndef SCADAREGION30_H
#define SCADAREGION30_H

#include "scada.h"

#define SIZE_RECORD_DHS_LOG 6

class ScadaRegion30 : public Scada
{
public:
  ScadaRegion30();
  ~ScadaRegion30();

  virtual eMBErrorCode readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);

  /*!
   * \brief функция формирования данных из логов ТМС в архивы ГДИ Роснефть
   * \param buffer уазатель на массив куда будем заносить данные
   * \param address адресс регистра с которого начинаем чтение
   * \param numRegs количество читаемых регистров
   * \return
   */
  eMBErrorCode readDhsLog(uint8_t *buffer, uint16_t address, uint16_t numRegs);

  uint8_t *logDhs_;

private:
  /*!
   * \brief Инициализация параметров скады Регион 3.0
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);

  /*!
   * \brief Функция чтения данных архива ТМС
   * \param address номер регистра с которого начинаем читать
   * \param numRegs количество читамых регистров
   * \param buffer буфер куда копируются данные архивов ТМС с flash памяти
   * \return
   */


  float firstAddrDhsLog_;
  float lastAddrDhsLog_;

};

#endif // SCADAREGION30_H

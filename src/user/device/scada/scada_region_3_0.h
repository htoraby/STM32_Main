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

private:
  /*!
   * \brief Инициализация параметров скады Регион 3.0
   */
  void initParameters();
  void calcParamsTask();
  int setNewValue(ScadaParameter *param);
  /*!
   * \brief функция формирования данных из логов ТМС в архивы ГДИ Роснефть
   * \param buffer уазатель на массив куда будем заносить данные
   * \param address адресс регистра с которого начинаем чтение
   * \param numRegs количество читаемых регистров
   * \return
   */
  eMBErrorCode readRegDhsLog(uint8_t *buffer, uint16_t address, uint16_t numRegs);

  uint8_t *logDhs_;                         //!< Указатель на буфер с данными ТМС
  float firstAddrDhsLog_;                   //!< Адрес первого регистра архива ГДИ
  float lastAddrDhsLog_;                    //!< Адрес последнего регистра архива ГДИ

};

#endif // SCADAREGION30_H

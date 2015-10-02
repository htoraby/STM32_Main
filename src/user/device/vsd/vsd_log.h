#ifndef VSDFASTLOG_H
#define VSDFASTLOG_H

#include "modbus_master_serial.h"

class VsdLog
{
public:
  VsdLog(int numPort, long baudRate, int dataBits, int stopBits, int parity, int address);
  virtual ~VsdLog();

  bool isConnect();

  /*!
   * \brief Проверка на наличие аварии в ЧРП.
   * Чтение флага готовности архива из специального регистра
   * \return
   */
  virtual bool checkAlarm();

  /*!
   * \brief Чтение аварийного архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   */
  virtual void readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic, uint16_t *ud);

  /*!
   * \brief Чтение пускового архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   * \param cos
   */
  virtual void readRunningLog(uint16_t *ia, uint16_t *ib, uint16_t *ic, uint16_t *ud, uint16_t *cos);

protected:
  /*!
   * \brief Запись в регистр значения
   */
  void writeReg(uint32_t addr, int16_t value);

  /*!
   * \brief Чтение архива
   * \param addr
   * \param buf
   * \param size
   */
  void readLog(uint32_t addr, uint16_t *buf, uint32_t size);

private:
  int numPort_;
  uint8_t devAdrs_;
  ModbusMasterSerial *mms_;
  bool prevConnect_;

  //! Идентификатор семафора захвата порта
  osSemaphoreId semaphoreId_;

};

#endif // VSDFASTLOG_H

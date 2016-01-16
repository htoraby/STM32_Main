#ifndef VSDFASTLOG_H
#define VSDFASTLOG_H

#include "modbus_master_serial.h"
#include "gpio.h"

class VsdLog
{
public:
  VsdLog(int numPort, long baudRate, int dataBits, int stopBits, int parity, int address);
  virtual ~VsdLog();

  bool isConnect();

  /*!
   * \brief Проверка на наличие аварии в ЧРП
   * \return
   */
  virtual bool checkAlarm();

  /*!
   * \brief Проверка на готовность архивов в ЧРП
   * \return
   */
  virtual bool checkReady();

  /*!
   * \brief Сброс флага готовности архивов ЧРП
   */
  virtual void resetReady();

  /*!
   * \brief setAlarm
   */
  virtual void setAlarm();

  /*!
   * \brief resetAlarm
   */
  virtual void resetAlarm();

  /*!
   * \brief Чтение аварийного архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   */
  virtual void readAlarmLog(int16_t *ia, int16_t *ib, int16_t *ic, int16_t *ud);

  /*!
   * \brief Чтение пускового архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   * \param cos
   */
  virtual void readRunningLog(int16_t *ia, int16_t *ib, int16_t *ic, int16_t *ud, int16_t *cos);

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

  /*!
   * \brief readNovometLog
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   */
  void readNovometLog(int16_t *ia, int16_t *ib, int16_t *ic,
                                   int16_t *ud);

private:
  int numPort_;
  uint8_t devAdrs_;
  ModbusMasterSerial *mms_;
  bool prevConnect_;

  //! Идентификатор семафора захвата порта
  osSemaphoreId semaphoreId_;

};

#endif // VSDFASTLOG_H

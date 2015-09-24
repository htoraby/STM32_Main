#ifndef VSDFASTLOG_H
#define VSDFASTLOG_H

#include "modbus_master_serial.h"

class VsdLog
{
public:
  VsdLog(int numPort, long baudRate, int dataBits, int stopBits, int parity, int address);
  ~VsdLog();

  /*!
   * \brief Метод создания задачи
   * \param threadName - имя задачи
   */
  void createThread(const char *threadName);

  /*!
   * \brief Задача чтения архивов с устройства по Modbus
   */
  void task();

  bool isConnect();

  uint16_t regArr_[125];

private:
  int numPort_;
  uint8_t devAdrs_;
  ModbusMasterSerial *mms_;
  bool prevConnect;

  //! Идентификатор задачи
  osThreadId threadId_;

};

#endif // VSDFASTLOG_H

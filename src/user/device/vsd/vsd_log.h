#ifndef VSDFASTLOG_H
#define VSDFASTLOG_H

#include "modbus_master_serial.h"

class VsdLog
{
public:
  VsdLog(int numPort, long baudRate, int dataBits, int stopBits, int parity, int address);
  ~VsdLog();

  bool isConnect();

private:
  int numPort_;
  uint8_t devAdrs_;
  ModbusMasterSerial *mms_;
  bool prevConnect;

};

#endif // VSDFASTLOG_H

#include "vsd_log.h"
#include "user_main.h"

#define MAX_COUNT 120

VsdLog::VsdLog(int numPort,
               long baudRate,
               int dataBits,
               int stopBits,
               int parity,
               int address)
  : numPort_(numPort)
  , devAdrs_(address)
  , prevConnect_(true)
{
  mms_ = new ModbusMasterSerial();
  // Открываем порт
  mms_->openProtocol(numPort_, baudRate, dataBits, stopBits, parity);
}

VsdLog::~VsdLog()
{
  mms_->closeProtocol(numPort_);
  delete mms_;
}

bool VsdLog::isConnect()
{
  bool curConnect = mms_->isConnect();

  if (prevConnect_ && !curConnect) {
    logDebug.add(WarningMsg, "Connect lost port: %d, device: %d", numPort_, devAdrs_);
  }

  if (!prevConnect_ && curConnect) {
    logDebug.add(WarningMsg, "Connect restored port: %d, device: %d", numPort_, devAdrs_);
  }

  prevConnect_ = curConnect;
  return curConnect;
}

void VsdLog::readAlarmLog(uint16_t */*ia*/, uint16_t */*ib*/, uint16_t */*ic*/,
                          uint16_t */*ud*/)
{

}

void VsdLog::readRunningLog(uint16_t */*ia*/, uint16_t */*ib*/, uint16_t */*ic*/,
                            uint16_t */*ud*/, uint16_t */*cos*/)
{

}

void VsdLog::readLog(uint32_t addr, uint16_t *buf, uint32_t size)
{
  uint16_t res = 1;
  uint16_t address = addr;
  uint32_t count = MAX_COUNT;
  uint32_t readAll = 0;
  while (1) {
    if ((readAll + MAX_COUNT) > size)
      count = size - readAll;
    res = mms_->readMultipleRegisters(devAdrs_, address, &buf[readAll], count);
    if (res != MODBUS_OK) {
      break;
    }
    readAll += count;
    address += count;
    if (readAll >= size)
      break;
  }
}

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

  semaphoreId_ = osSemaphoreCreate(NULL, 1);
}

VsdLog::~VsdLog()
{
  mms_->closeProtocol(numPort_);
  delete mms_;

  osSemaphoreDelete(semaphoreId_);
}

bool VsdLog::isConnect()
{
  bool curConnect = mms_->isConnect();

  if (prevConnect_ && !curConnect) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Архивы ЧРП: Связь потеряна (numPort = %d, devAdrs = %d)", numPort_, devAdrs_);
#endif
  }

  if (!prevConnect_ && curConnect) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Архивы ЧРП: Связь восстановлена (numPort = %d, devAdrs = %d)", numPort_, devAdrs_);
#endif
  }

  prevConnect_ = curConnect;
  return curConnect;
}

bool VsdLog::checkAlarm()
{
  return false;
}

bool VsdLog::checkReady()
{
  return false;
}

void VsdLog::resetReady()
{

}

void VsdLog::setAlarm()
{

}

void VsdLog::resetAlarm()
{

}

void VsdLog::readAlarmLog(uint16_t */*ia*/, uint16_t */*ib*/, uint16_t */*ic*/,
                          uint16_t */*ud*/)
{

}

void VsdLog::readRunningLog(uint16_t */*ia*/, uint16_t */*ib*/, uint16_t */*ic*/,
                            uint16_t */*ud*/, uint16_t */*cos*/)
{

}

void VsdLog::writeReg(uint32_t addr, int16_t value)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  uint16_t res = 1;
  res = mms_->writeSingleRegister(devAdrs_, addr, value);
  if (res != MODBUS_OK) {
    asm("nop");
  }

  osSemaphoreRelease(semaphoreId_);
}

void VsdLog::readLog(uint32_t addr, uint16_t *buf, uint32_t size)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

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

  osSemaphoreRelease(semaphoreId_);
}

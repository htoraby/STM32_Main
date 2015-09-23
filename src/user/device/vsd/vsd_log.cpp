#include "vsd_log.h"
#include "user_main.h"

VsdLog::VsdLog(int numPort,
               long baudRate,
               int dataBits,
               int stopBits,
               int parity,
               int address)
  : numPort_(numPort)
  , devAdrs_(address)
  , prevConnect(true)
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

  if (prevConnect && !curConnect) {
    logDebug.add(WarningMsg, "Connect lost port: %d, device: %d", numPort_, devAdrs_);
  }

  if (!prevConnect && curConnect) {
    logDebug.add(WarningMsg, "Connect restored port: %d, device: %d", numPort_, devAdrs_);
  }

  prevConnect = curConnect;
  return curConnect;
}

#include "vsd_log.h"
#include "user_main.h"

static void vsdLogTask(void *p)
{
  (static_cast<VsdLog*>(p))->task();
}

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
  osThreadTerminate(threadId_);
  mms_->closeProtocol(numPort_);
  delete mms_;
}

void VsdLog::createThread(const char *threadName)
{
  osThreadDef_t t = {threadName, vsdLogTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
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

void VsdLog::task()
{
  int count = 0;
  uint16_t res = 1;
  while (1) {
    osDelay(30000);
//    parameters.set(VSD_ETALON_FAST_OFF, 1);
//    osDelay(30000);
//    count = 125;
//    int address = 0x9800;
//    res = mms_->readMultipleRegisters(devAdrs_, address, regArr_, count);
//    asm("nop");
  }
}

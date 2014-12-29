#include "log_main.h"

LogEvent logEvent;
LogData logData;
LogRunning logRunning;
LogTms logTms;

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);

  logEvent.init();
  logData.init();
  logRunning.init();
  logTms.init();
}

void logErase()
{
  Log::id_ = 0;
  framWriteData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);

  logEvent.deInit();
  logData.deInit();
  logRunning.deInit();
  logTms.deInit();

  flashExtChipErase(FlashSpi5);
}

StatusType logRead(uint32_t address, uint8_t *data, uint32_t size)
{
  return flashExtRead(FlashSpi5, address, data, size);
}


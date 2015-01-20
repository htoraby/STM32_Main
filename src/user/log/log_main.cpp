#include "log_main.h"

LogEvent logEvent;
LogData logData;
LogRunning logRunning;
LogAlarm logAlarm;
LogTms logTms;

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);

  logEvent.init();
  logRunning.init();
  logAlarm.init();

#ifndef DEBUG
  logData.init();
  logTms.init();
#endif
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

  logInit();
}

StatusType logRead(uint32_t address, uint8_t *data, uint32_t size)
{
  return flashExtRead(FlashSpi5, address, data, size);
}


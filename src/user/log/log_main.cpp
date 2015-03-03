#include "log_main.h"

LogEvent logEvent;
LogData logData;
LogRunning logRunning;
LogAlarm logAlarm;
LogTms logTms;

LogDebug logDebug;

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);
  framReadData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logEvent.init();
  logRunning.init();
  logAlarm.init();

#ifndef DEBUG
  logData.init();
  logTms.init();
#endif

  logDebug.init();
}

void logErase()
{
  Log::id_ = 0;
  framWriteData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);
  Log::idDebug_ = 0;
  framWriteData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logEvent.deInit();
  logData.deInit();
  logRunning.deInit();
  logTms.deInit();
  logDebug.deInit();

  flashExtChipErase(FlashSpi5);
  flashExtChipErase(FlashSpi1);

  logInit();
}

StatusType logRead(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  return flashExtRead(num, address, data, size);
}


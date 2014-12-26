#include "log_main.h"

LogEvent *logEvent;

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);

  logEvent = new LogEvent();
}


#include "log_debug.h"
#include "string.h"
#include "user_main.h"

#include <stdio.h>
#include <stdarg.h>

LogDebug::LogDebug() : Log(DebugTypeLog)
{
  semaphoreId_ = osSemaphoreCreate(NULL, 1);
}

LogDebug::~LogDebug()
{

}

uint32_t LogDebug::add(MsgType type, const char *msg, ...)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  memset(msg_, 0, sizeof(msg_));
  memset(buffer, 0, sizeof(buffer));

  va_list args;
  va_start(args, msg);
  vsprintf(msg_, msg, args);
  va_end(args);

  time_t time = ksu.getTime();

  *(uint32_t*)(buffer) = ++idDebug_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = type;

  int size = strlen(msg_);
  if (size > SIZE_MSG_DEBUG)
    size = SIZE_MSG_DEBUG;
  memcpy(&buffer[9], msg_, size);

  write(buffer, SIZE_BUF_LOG);

#ifdef USE_RTT
  tm *dateTime = localtime(&time);
  if ((type == CriticalMsg) || (type == FatalMsg))
    SEGGER_RTT_printf(0, "\033[31m");
  else
    SEGGER_RTT_printf(0, "\033[0m");
  SEGGER_RTT_printf(0, "LOG(%d): %02d.%02d.%d %02d:%02d:%02d  %s\n",
                    type,
                    dateTime->tm_mday, dateTime->tm_mon + 1, dateTime->tm_year + 1900,
                    dateTime->tm_hour, dateTime->tm_min, dateTime->tm_sec,
                    msg_);
  if ((type == CriticalMsg) || (type == FatalMsg))
    SEGGER_RTT_printf(0, "\033[0m");
#endif

  osSemaphoreRelease(semaphoreId_);
  return idDebug_;
}

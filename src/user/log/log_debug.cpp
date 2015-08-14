#include "log_debug.h"
#include "string.h"
#include "user_main.h"

#include <stdio.h>
#include <stdarg.h>

LogDebug::LogDebug() : Log(DebugTypeLog)
{

}

LogDebug::~LogDebug()
{

}

uint32_t LogDebug::add(MsgType type, const char *msg, ...)
{
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
  memset(msg1251_, 0, sizeof(msg1251_));
  convert_utf8_to_windows1251(msg_, msg1251_, size);
  SEGGER_RTT_printf(0, "LogDebug (%d): %02d.%02d.%d %02d:%02d:%02d  %s\n",
                    type,
                    dateTime->tm_mday, dateTime->tm_mon + 1, dateTime->tm_year + 1900,
                    dateTime->tm_hour, dateTime->tm_min, dateTime->tm_sec,
                    msg1251_);
#endif

  return idDebug_;
}

#include "log_debug.h"
#include "string.h"
#include "user_main.h"

LogDebug::LogDebug() : Log(DebugTypeLog)
{

}

LogDebug::~LogDebug()
{

}

uint32_t LogDebug::add(MsgType type, const char* msg)
{
  memset(buffer, 0, sizeof(buffer));

  time_t time = ksu.getTime();

  *(uint32_t*)(buffer) = ++idDebug_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = type;

  int size = strlen((char*)msg);
  if (size > SIZE_MSG_DEBUG)
    size = SIZE_MSG_DEBUG;
  memcpy(&buffer[9], msg, size);

  write(buffer, SIZE_BUF_LOG);

#ifdef USE_RTT
  static char buf1251[sizeof(buffer)];
  tm *dateTime = localtime(&time);
  convert_utf8_to_windows1251(msg, buf1251, size);
  SEGGER_RTT_printf(0, "LogDebug (%d): %02d.%02d.%d %02d:%02d:%02d  %s\n",
                    type,
                    dateTime->tm_mday, dateTime->tm_mon + 1, dateTime->tm_year + 1900,
                    dateTime->tm_hour, dateTime->tm_min, dateTime->tm_sec,
                    buf1251);
#endif

  return idDebug_;
}

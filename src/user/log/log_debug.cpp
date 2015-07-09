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

  return idDebug_;
}

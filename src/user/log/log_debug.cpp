#include "log_debug.h"
#include "string.h"

LogDebug::LogDebug() : Log(DebugTypeLog)
{

}

LogDebug::~LogDebug()
{

}

uint32_t LogDebug::add(MsgType type, const char* msg)
{
  memset(buffer, 0, sizeof(buffer));

  time_t time = getTime();

  *(uint32_t*)(buffer) = ++idDebug_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = type;

  int size = strlen((char*)msg);
  if (size > (256 - 9))
    size = 256 - 9;
  memcpy(&buffer[9], msg, size);

  write(buffer, 256);

  return idDebug_;
}

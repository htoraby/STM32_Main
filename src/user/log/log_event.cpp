#include "log_event.h"
#include <string.h>

LogEvent::LogEvent() : Log(EventTypeLog)
{

}

LogEvent::~LogEvent()
{

}

void LogEvent::add(uint8_t code, uint8_t type, uint16_t id,
                   uint32_t oldValue, uint32_t newValue)
{
  time_t time = getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(uint8_t*)(buffer+9) = type;
  *(uint16_t*)(buffer+10) = id;
  *(uint32_t*)(buffer+12) = oldValue;
  *(uint32_t*)(buffer+16) = newValue;
  write(buffer, 20);
}


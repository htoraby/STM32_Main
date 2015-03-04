#include "log_event.h"
#include "user_main.h"
#include <string.h>

LogEvent::LogEvent() : Log(EventTypeLog)
{

}

LogEvent::~LogEvent()
{

}

uint32_t LogEvent::add(uint8_t code, uint8_t type, uint16_t id,
                       float oldValue, float newValue, uint8_t units)
{
  memset(buffer, 0, sizeof(buffer));

  time_t time = getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(uint8_t*)(buffer+9) = type;
  *(uint16_t*)(buffer+10) = id;
  *(float*)(buffer+12) = oldValue;
  *(float*)(buffer+16) = newValue;
  *(uint8_t*)(buffer+20) = units;
  write(buffer, 21);

  // Сообщить контроллеру визуализации о новом событии
  novobusSlave.putMessageEvents(id);

  return id_;
}


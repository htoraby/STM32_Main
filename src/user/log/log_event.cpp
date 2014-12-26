#include "log_event.h"

LogEvent::LogEvent() : Log(EventTypeLog)
{

}

LogEvent::~LogEvent()
{

}

void LogEvent::add(uint8_t code, uint8_t type, uint16_t id,
                   uint32_t oldValue, uint32_t newValue)
{
  tm dateTime;
  getDateTime(&dateTime);

  *(uint32_t*)(buffer) = ++id_;

//  *(uint16_t*)(buffer+4) = 2000 + dateTime.year;
//  *(uint8_t*)(buffer+6) = dateTime.month;
//  *(uint8_t*)(buffer+7) = dateTime.date;
//  *(uint8_t*)(buffer+8) = dateTime.hours;
//  *(uint8_t*)(buffer+9) = dateTime.minutes;
//  *(uint8_t*)(buffer+10) = dateTime.seconds;

  *(uint8_t*)(buffer+11) = code;
  *(uint8_t*)(buffer+12) = type;
  *(uint16_t*)(buffer+13) = id;
  *(uint32_t*)(buffer+15) = oldValue;
  *(uint32_t*)(buffer+19) = newValue;
  write(buffer, 23);
}


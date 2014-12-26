#ifndef LOGEVENT_H
#define LOGEVENT_H

#include "log.h"

class LogEvent : public Log
{
public:
  LogEvent();
  ~LogEvent();

  void add(uint8_t code, uint8_t type, uint16_t id, uint32_t oldValue, uint32_t newValue);
};

#endif // LOGEVENT_H

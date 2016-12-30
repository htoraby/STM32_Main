#include "log_event.h"
#include "user_main.h"
#include <string.h>

LogEvent::LogEvent() : Log(EventTypeLog)
{
  semaphoreId_ = osSemaphoreCreate(NULL, 1);
}

LogEvent::~LogEvent()
{

}

uint32_t LogEvent::add(EventCode code, EventType type, EventId id,
                       float oldValue, float newValue, uint8_t units)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  memset(buffer, 0, sizeof(buffer));

  const uint32_t addr = address();
  time_t time = ksu.getTime();

  ++id_;
  *(uint32_t*)(buffer) = id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(uint8_t*)(buffer+9) = type;
  *(uint16_t*)(buffer+10) = id;
  *(float*)(buffer+12) = oldValue;
  *(float*)(buffer+16) = newValue;
  *(uint8_t*)(buffer+20) = units;
  StatusType status = write(buffer, LOG_EVENT_SIZE);

  // Сообщить контроллеру визуализации о новом событии
  if (status == StatusOk)
    novobusSlave.putMessageEvents(addr);

  osSemaphoreRelease(semaphoreId_);
  return id_;
}


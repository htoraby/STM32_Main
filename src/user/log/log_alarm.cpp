#include "log_alarm.h"

#if USE_EXT_MEM
static float uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM] __attribute__((section(".extmem")));
#else
static float uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM];
#endif

LogAlarm::LogAlarm() : LogRunning(AlarmTypeLog)
{

}

LogAlarm::~LogAlarm()
{

}

void LogAlarm::init()
{
  uValue_ = uValue;

  LogRunning::init("LogAlarm");
}

void LogAlarm::start(EventType type, uint32_t eventId)
{
  eventId_ = eventId;
  LogRunning::start(type);
}

void LogAlarm::task()
{
  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    osDelay(500);
    add();
  }
}


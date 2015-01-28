#include "log_running.h"
#include "user_main.h"
#include <string.h>

#if USE_EXT_MEM
static float uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM] __attribute__((section(".extmem")));
#else
static float uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM];
#endif

LogRunning::LogRunning(TypeLog type) : Log(type)
{

}

LogRunning::~LogRunning()
{

}

static void logRunningTask(void *p)
{
  (static_cast<LogRunning*>(p))->task();
}

void LogRunning::init(const char *threadName)
{
  if (type_ == RunTypeLog)
    uValue_ = uValue;

  Log::init();

  osThreadDef_t t = {threadName, logRunningTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);

  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);
}

void LogRunning::start(EventType type)
{
  eventType_ = type;
  osSemaphoreRelease(semaphoreId_);
}

void LogRunning::task()
{
  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    eventId_ = logEvent.add(RunCode, eventType_, RunId);

    osDelay(ARCHIVE_TIME);
    add();
  }
}

void LogRunning::add()
{
  // Получения занчений Ua, Ub, Uc
  copyAdcData();
  for (int i = 0; i < ADC_CNANNELS_NUM*ADC_POINTS_NUM; ++i) {
    // V = (value - 2048) * 627.747 * 2.5 / 0xFFF
    uValue_[i] = ((adcData[i] - 2048) * 627.747 * 2.5) / 0xFFF;
  }

  memset(buffer, 0, sizeof(buffer));
  *(uint32_t*)(buffer) = eventId_;
  *(float*)(buffer+4) = ksu.getValue(CCS_RESISTANCE_ISOLATION);
  write(buffer, 256);

  for (int i = 0; i < ADC_POINTS_NUM/8; ++i) {
    memset(buffer, 0, sizeof(buffer));
    for (int j = 0; j < 8; ++j) {
      //TODO: Нет информации об архивах ЧРП
      //  *(float*)(buffer+j*32) = ;
      //  *(float*)(buffer+4+j*32) = ;
      //  *(float*)(buffer+8+j*32) = ;
      //  *(float*)(buffer+12+j*32) = ;
      *(float*)(buffer+16+j*32) = ksu.getValue(VSD_COS_PHI_MOTOR);
      *(float*)(buffer+20+j*32) = uValue[0 + j*3 + i*24];
      *(float*)(buffer+24+j*32) = uValue[1 + j*3 + i*24];
      *(float*)(buffer+28+j*32) = uValue[2 + j*3 + i*24];
    }
    if (i == (ADC_POINTS_NUM/8 - 1))
      write(buffer, 256, false, true);
    else
      write(buffer, 256, false);
  }
}



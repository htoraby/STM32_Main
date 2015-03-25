#include "log_tms.h"
#include "user_main.h"
#include <string.h>

LogTms::LogTms() : Log(TmsTypeLog)
{

}

LogTms::~LogTms()
{

}

static void logTmsTask(void *p)
{
  (static_cast<LogTms*>(p))->task();
}

void LogTms::init()
{
  Log::init();

  osThreadDef_t t = {"LogTms", logTmsTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void LogTms::task()
{
  while (1) {
    osDelay(1000);
    add();
  }
}

void LogTms::add()
{
  memset(buffer, 0, sizeof(buffer));

  time_t time = ksu.getTime();
  uint8_t code = TmsCode;

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(float*)(buffer+9) = tms->getValue(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+13) = tms->getValue(TMS_TEMPERATURE_WINDING);

  write(buffer, 17);
}

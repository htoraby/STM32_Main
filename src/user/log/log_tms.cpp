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

  time_t time = getTime();
  uint8_t code = TmsCode;

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint32_t*)(buffer+8) = code;
  *(uint32_t*)(buffer+9) = KSU.getValue(TMS_PRESSURE_INTAKE);
  *(uint32_t*)(buffer+13) = KSU.getValue(TMS_TEMPERATURE_WINDING);

  write(buffer, 17);
}
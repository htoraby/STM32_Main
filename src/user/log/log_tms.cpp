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

void LogTms::deInit()
{
  osThreadTerminate(threadId_);
  Log::deInit();
}

void LogTms::task()
{
  int timeCnt = 0;
  while (1) {
    osDelay(1000);

    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
      int period = parameters.get(CCS_LOG_PERIOD_DHS);
      if (++timeCnt >= period) {
        timeCnt = 0;
        add();
      }
    } else {
      timeCnt = parameters.get(CCS_LOG_PERIOD_DHS);
    }
  }
}

void LogTms::add()
{
  memset(buffer, 0, sizeof(buffer));

  time_t time = ksu.getTime();
  uint8_t code = TmsCode;

  ++id_;
  *(uint32_t*)(buffer) = id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(float*)(buffer+9) = parameters.get(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+13) = parameters.get(TMS_TEMPERATURE_WINDING);

  write(buffer, 17);
  calcAddrLastRecordRosneft(calcRecordLogTms());
}

uint32_t LogTms::calcRecordLogTms()
{
  uint32_t count = 0;
  // Адрес с которого начнётся следующая запись - первый адрес архивов, деленное
  // на размер сектора = количество записанных секторов
  div_t x = div(address() - startAddr(), 0x1000);
  // последний сектор записан целиком
  if (x.rem == 0) {
    // Количество записей = количество записей на сектор * на количество секторов
    count = (x.quot * 240);
  }
  // Последний сектор записан частично
  else {
    // Количество записей = количество записей на сектор * на количество секторов
    // + часть сектора * умноженная на размер сектора / на размер одной записи
    count = (x.quot * 240) + ((x.rem * 0x1000) / 17);
  }
  return count;
}

void  LogTms::calcAddrLastRecordRosneft(uint32_t countRecord)
{
  // Адрес первого регистра всегда 0x1000
  parameters.set(CCS_DHS_LOG_ROSNEFT_FIRST_REGISTER, 0x1000);
  // Адрес последней записи = Адрес первого регистра + количество записей * на размер записи - последняя запись
  parameters.set(CCS_DHS_LOG_ROSNEFT_LAST_RECORD, 0x1000 + countRecord * 6 - 6);
  // Адрес последнего регистра  = Адрес первого регистра + количество записей * на размер записи
  parameters.set(CCS_DHS_LOG_ROSNEFT_LAST_REGISTER, 0x1000 + countRecord * 6);

}



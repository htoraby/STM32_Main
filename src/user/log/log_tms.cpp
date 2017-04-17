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
  uint8_t codeErr = 0;

  ++id_;
  *(uint32_t*)(buffer) = id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(uint8_t*)(buffer+9) = codeErr;
  *(float*)(buffer+10) = parameters.get(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+14) = parameters.get(TMS_TEMPERATURE_WINDING);

  write(buffer, LOG_DHS_SIZE);
  calcAddrLastRecordRosneft();
}

void  LogTms::calcAddrLastRecordRosneft()
{
  // Получаем счётчик записей в архиве для чтения по ТТ Роснефть
  float cntRecord = parameters.get(CCS_DHS_LOG_ROSNEFT_COUNT_RECORD);

  // Если количество записей в архиве не достигло максимального по ТТ Роснефть
  if (cntRecord < 10239) {
    cntRecord = (address() - startAddr()) / LOG_DHS_SIZE;
    // Сохраняем количество записей в регистре
    parameters.set(CCS_DHS_LOG_ROSNEFT_COUNT_RECORD, cntRecord);
    // Адрес первого регистра всегда 0x1000
    parameters.set(CCS_DHS_LOG_ROSNEFT_FIRST_REGISTER, 0x1000);
    // Адрес последней записи = Адрес первого регистра + количество записей * на размер записи - последняя запись
    parameters.set(CCS_DHS_LOG_ROSNEFT_LAST_RECORD, 0x1000 + cntRecord * 6 - 6);
    // Адрес последнего регистра  = Адрес первого регистра + количество записей * на размер записи
    parameters.set(CCS_DHS_LOG_ROSNEFT_LAST_REGISTER, 0x1000 + cntRecord * 6);
  }
}

StatusType LogTms::readLogRequestedRosneft(uint32_t shiftFromEnd, uint8_t *buffer, uint32_t quantity)
{
  uint32_t addr = address();                // Текущий адрес в flash памяти
  uint32_t startAddr_ = startAddr();        //
  uint32_t endAddr_ = endAddr();
  for (uint16_t i = 0; i < shiftFromEnd; i++) {
    if ((addr - LOG_DHS_SIZE) < startAddr_) {
      addr = endAddr_;
    }
    addr = addr - LOG_DHS_SIZE;
  }

  if ((addr + quantity * LOG_DHS_SIZE) <= endAddr_) {
    return logRead(addr, buffer, quantity * LOG_DHS_SIZE);
  }
  else {
    // TODO: обработка чтения сначала до конца архива, а потом с начала
  }


}









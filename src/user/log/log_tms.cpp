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

    // Если станция в останове и останов меньше часа
    if ((parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) && (parameters.get(CCS_STOP_TIME) <= 60 * 60)) {
      int period = parameters.get(CCS_LOG_PERIOD_DHS_FIRST_HOUR_AFTER_STOP);
      if (++timeCnt >= period) {
        timeCnt = 0;
        add();
      }
    }
    else {
      int period = parameters.get(CCS_LOG_PERIOD_DHS);
      if (++timeCnt >= period) {
        timeCnt = 0;
        add();
      }
    }

    /*
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP) {
      int period = parameters.get(CCS_LOG_PERIOD_DHS);
      if (++timeCnt >= period) {
        timeCnt = 0;
        add();
      }
    } else {
      timeCnt = parameters.get(CCS_LOG_PERIOD_DHS);
    }
    */
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
  *(uint8_t*)(buffer+17) = calcCodeErrLogRosneft();
  *(float*)(buffer +18) = parameters.get(TMS_TEMPERATURE_INTAKE);
  write(buffer, LOG_DHS_SIZE);  
  incCountRecordLogDhs();
}

void LogTms::incCountRecordLogDhs()
{
  float cntRecord = parameters.get(CCS_DHS_LOG_COUNT_RECORD);
  if (cntRecord < ((endAddr() - startAddr()) / LOG_DHS_SIZE)) {
    parameters.set(CCS_DHS_LOG_COUNT_RECORD, cntRecord + 1);
  }
}

StatusType LogTms::readLogRequestedRosneft(uint32_t shiftFromEnd, uint8_t *buffer, uint32_t quantity)
{
  uint32_t addr = address();
  uint32_t startAddr_ = startAddr();
  uint32_t endAddr_ = endAddr();
  // Вычисляем адрес в flash с которого начинаются запрашиваемые данные
  addr = addr - shiftFromEnd * LOG_DHS_SIZE;
  // Если адрес оказался за пределами области хранения архивов ТМС
  if (addr < startAddr_) {
    addr = endAddr_ - (startAddr_ - addr);
  }
  if ((addr + quantity * LOG_DHS_SIZE) <= endAddr_) {
    return logRead(addr, buffer, quantity * LOG_DHS_SIZE);
  }
  else {
    // Читаем до конца области памяти с ТМС
    return logRead(addr, buffer, endAddr_ - addr);
    // TODO: По идее после этого надо читать с начала до конца количества записей, но х.з. как
  }
}

uint8_t LogTms::calcCodeErrLogRosneft()
{
  // Формирование кода ошибки архива ГДИ
  uint8_t codeErr = 0;
  if (!parameters.get(CCS_DHS_CONNECTION)) {
    codeErr = codeErr | 0x0001;
  }
  if (parameters.get(TMS_FAIL_LINK_TMSP))  {
    codeErr = codeErr | 0x0001;
  }
  if (parameters.get(CCS_DHS_STATE_SENSOR_TEMPERATURE_INTAKE)) {
    codeErr = codeErr | 0x0002;
  }
  if (parameters.get(CCS_DHS_STATE_SENSOR_PRESSURE_INTAKE)) {
    codeErr = codeErr | 0x0004;
  }
  parameters.set(CCS_DHS_LOG_ROSNEFT_CODE_ERROR, (float)codeErr);
  return codeErr;
}









#include "log_running.h"
#include "user_main.h"
#include "adc_ext.h"

#include <string.h>

#if USE_EXT_MEM
static uint16_t uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t iaValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t ibValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t icValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t udValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t cosValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
#else
static uint16_t uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM];
static uint16_t iaValue[ADC_POINTS_NUM];
static uint16_t ibValue[ADC_POINTS_NUM];
static uint16_t icValue[ADC_POINTS_NUM];
static uint16_t udValue[ADC_POINTS_NUM];
static uint16_t cosValue[ADC_POINTS_NUM];
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
  Log::init();

  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);

  osThreadDef_t t = {threadName, logRunningTask, osPriorityNormal, 0, 3 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void LogRunning::deInit()
{
  osThreadTerminate(threadId_);
  osSemaphoreDelete(semaphoreId_);
  Log::deInit();
}

void LogRunning::start()
{
  osSemaphoreRelease(semaphoreId_);
}

void LogRunning::task()
{
  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    EventId eventId = (EventId)ksu.getValue(CCS_LAST_RUN_REASON);
    eventId_ = logEvent.add(RunCode, NoneType, eventId);

    osDelay(ARCHIVE_TIME);

    if (vsd->log()) {
      add();
    }
  }
}

void LogRunning::add()
{
  int time = HAL_GetTick();
  uint16_t typeVsd = parameters.get(CCS_TYPE_VSD);

  // Получение значений Ua, Ub, Uc
  copyAdcData(uValue);

  // Получение значений с ЧРП Ia, Ib, Ic, Ud, cos
  vsd->log()->readRunningLog(iaValue, ibValue, icValue, udValue, cosValue);

  memset((uint8_t *)buffer, 0, sizeof(buffer));
  *(uint32_t*)(buffer) = eventId_;
  *(float*)(buffer+4) = parameters.get(CCS_RESISTANCE_ISOLATION);
  write((uint8_t *)buffer, SIZE_BUF_LOG, false);

  int idxU = 0;
  int idxI = 0;
  int idxVsd = 0;
  int shiftVsd = 0;
  int shiftIa = 0;
  float current = 0;
  float maxCurrent = parameters.get(CCS_SU_MAX_CURRENT);
  int pauseCount = 0;

  for (int i = 0; i < ADC_POINTS_NUM; ) {
    memset((uint8_t *)buffer, 0xFF, sizeof(buffer));
    for (int j = 0; j < 4; ++j) {
      switch (typeVsd) {
      case VSD_TYPE_ETALON:
        *(float*)(buffer + j*64) = (int16_t)iaValue[idxVsd];
        *(float*)(buffer + 4 + j*64) = (int16_t)ibValue[idxVsd];
        *(float*)(buffer + 8 + j*64) = (int16_t)icValue[idxVsd];
        *(float*)(buffer + 12 + j*64) = udValue[idxVsd];
        *(float*)(buffer + 16 + j*64) = cosValue[idxVsd]*0.01;
        shiftVsd++;
        if (shiftVsd >= 10) {
          shiftVsd = 0;
          idxVsd++;
        }
        break;
      case VSD_TYPE_NOVOMET:
        *(float*)(buffer + j*64) = (int16_t)iaValue[idxVsd];
        *(float*)(buffer + 4 + j*64) = (int16_t)ibValue[idxVsd];
        *(float*)(buffer + 8 + j*64) = (int16_t)icValue[idxVsd];
        *(float*)(buffer + 12 + j*64) = udValue[idxVsd];
        *(float*)(buffer + 16 + j*64) = cosValue[idxVsd];
        idxVsd++;
        break;
      case VSD_TYPE_DANFOSS:
        if (shiftIa == 0) {                                           // Если новая точка тока
          current = (float)iaValue[idxI] * 10000.0 / 0xFFF / 493;              // Вычисляем милиА на входе
          if (current < 4.1) {                                       // Граница "разумности" параметра 4100 милиапмер
            current = 0;                                              // Ток ЧРП 0
          }
          else {
            current = (maxCurrent * (current - 4.0)) / 16.0;        // Вычисляем ток ЧРП
          }
        }

        *(float*)(buffer + j*64) = current;                           // Переписываем токи
        *(float*)(buffer + 4 + j*64) = current;
        *(float*)(buffer + 8 + j*64) = current;

        shiftIa++;
        if (shiftIa >= (ADC_POINTS_NUM / ADC_EXT_INPUTS_6_POINTS)) {
          shiftIa = 0;
          idxI++;
        }
        break;
      }

      *(float*)(buffer + 20 + j*64) = ((uValue[0 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      *(float*)(buffer + 24 + j*64) = ((uValue[1 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      *(float*)(buffer + 28 + j*64) = ((uValue[2 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      idxU += 3;
      i++;
    }
    if (i == ADC_POINTS_NUM)
      write((uint8_t *)buffer, SIZE_BUF_LOG, true, true);
    else
      write((uint8_t *)buffer, SIZE_BUF_LOG, false);

    if (pauseCount < 20) {
      pauseCount++;
    }
    else {
      pauseCount = 0;
      osDelay(1);
    }
  }

  time = HAL_GetTick() - time;
  logDebug.add(WarningMsg, "LogRunning::add() Recording time %d", time);

  vsd->log()->resetAlarm();
  osDelay(50);
}



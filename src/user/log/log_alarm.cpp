#include "log_alarm.h"
#include "user_main.h"

#include <string.h>

#if USE_EXT_MEM
static uint16_t uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t iaValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t ibValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t icValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
static uint16_t udValue[ADC_POINTS_NUM] __attribute__((section(".extmem")));
#else
static uint16_t uValue[ADC_CNANNELS_NUM*ADC_POINTS_NUM];
static uint16_t iaValue[ADC_POINTS_NUM];
static uint16_t ibValue[ADC_POINTS_NUM];
static uint16_t icValue[ADC_POINTS_NUM];
static uint16_t udValue[ADC_POINTS_NUM];
#endif

LogAlarm::LogAlarm() : LogRunning(AlarmTypeLog)
{

}

LogAlarm::~LogAlarm()
{

}

void LogAlarm::init()
{
  LogRunning::init("LogAlarm");
}

void LogAlarm::start(uint32_t eventId)
{
  eventId_ = eventId;
  LogRunning::start();
}

int timeTest;

void LogAlarm::task()
{
  while (1) {
    osDelay(1);

    if (vsd->log()) {
      if (vsd->log()->checkAlarm()) {
        add();
      }
    }
  }
}

void LogAlarm::add()
{
  eventId_ = logEvent.add(AlarmCode, AutoType, WriteAlarmLogId);

  // Получение значений Ua, Ub, Uc
  copyAdcData(uValue);

  while (!vsd->log()->checkReady()) {
    osDelay(10);
  }

  uint16_t typeVsd = parameters.get(CCS_TYPE_VSD);

  // Получение значений с ЧРП Ia, Ib, Ic, Ud
  vsd->log()->readAlarmLog(iaValue, ibValue, icValue, udValue);

  memset(buffer, 0, sizeof(buffer));
  *(uint32_t*)(buffer) = eventId_;
  *(float*)(buffer+4) = parameters.get(CCS_RESISTANCE_ISOLATION);
  write(buffer, SIZE_BUF_LOG);

  int idxU = 0;
  int idxI = 0;
  int idxUd = 0;
  int shiftUd = 0;
  for (int i = 0; i < ADC_POINTS_NUM; ) {
    memset(buffer, 0xFF, sizeof(buffer));
    for (int j = 0; j < 4; ++j) {
      switch (typeVsd) {
      case VSD_TYPE_ETALON:
        if (i >= ADC_POINTS_NUM/2) {
          *(float*)(buffer + j*64) = (int16_t)iaValue[idxI];
          *(float*)(buffer + 4 + j*64) = (int16_t)ibValue[idxI];
          *(float*)(buffer + 8 + j*64) = (int16_t)icValue[idxI];
          *(float*)(buffer + 12 + j*64) = udValue[idxUd];
          shiftUd++;
          if (shiftUd >= 10) {
            shiftUd = 0;
            idxUd++;
          }
          idxI++;
        }
        break;
      case VSD_TYPE_NOVOMET:
        *(float*)(buffer + j*64) = (int16_t)iaValue[idxI];
        *(float*)(buffer + 4 + j*64) = (int16_t)ibValue[idxI];
        *(float*)(buffer + 8 + j*64) = (int16_t)icValue[idxI];
        *(float*)(buffer + 12 + j*64) = udValue[idxI];
        idxI++;
        break;
      }

      *(float*)(buffer + 20 + j*64) = ((uValue[0 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      *(float*)(buffer + 24 + j*64) = ((uValue[1 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      *(float*)(buffer + 28 + j*64) = ((uValue[2 + idxU] - 2048) * 627.747 * 2.5) / 0xFFF;
      idxU += 3;
      i++;
    }
    if (i == ADC_POINTS_NUM)
      write(buffer, SIZE_BUF_LOG, false, true);
    else
      write(buffer, SIZE_BUF_LOG, false);
  }

  vsd->log()->resetReady();
  osDelay(10000);
}

#include "rtc.h"

RTC_HandleTypeDef hrtc;

void rtcInit()
{
  HAL_PWR_EnableBkUpAccess();

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&hrtc);

  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2) {
    TimeTypeDef time;
    //! Установка времени по умолчанию
    time.hours = 0;
    time.minutes = 0;
    time.seconds = 0;
    time.mseconds = 0;
    setTime(time);

    //! Установка даты по умолчанию
    DateTypeDef date;
    date.month = 1;
    date.date = 1;
    date.year = 0;
    setDate(date);

    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x32F2);
  }
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  (void)hrtc;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
      asm("nop"); //! error
    }
  }

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
    asm("nop"); //! error
  }

  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  (void)hrtc;
  __HAL_RCC_RTC_DISABLE();
}

void setDateTime(const tm &time)
{
  if (time.tm_year < 2000)
    return;

  RTC_TimeTypeDef timeRtc;
  timeRtc.Hours = time.tm_hour;
  timeRtc.Minutes = time.tm_min;
  timeRtc.Seconds = time.tm_sec;
  timeRtc.TimeFormat = RTC_HOURFORMAT12_AM;
  timeRtc.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  timeRtc.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &timeRtc, FORMAT_BIN);

  RTC_DateTypeDef dateRtc;
  dateRtc.Date = time.tm_mday;
  dateRtc.Month = time.tm_mon;
  dateRtc.Year = time.tm_year - 2000;
  dateRtc.WeekDay = RTC_WEEKDAY_MONDAY;
  HAL_RTC_SetDate(&hrtc, &dateRtc, FORMAT_BIN);
}

void setTime(const TimeTypeDef &time)
{
  RTC_TimeTypeDef timeRtc;
  timeRtc.Hours = time.hours;
  timeRtc.Minutes = time.minutes;
  timeRtc.Seconds = time.seconds;
  timeRtc.SubSeconds = time.mseconds;
  timeRtc.TimeFormat = RTC_HOURFORMAT12_AM;
  timeRtc.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  timeRtc.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &timeRtc, FORMAT_BIN);
}

void setDate(const DateTypeDef &date)
{
  RTC_DateTypeDef dateRtc;
  dateRtc.Date = date.date;
  dateRtc.Month = date.month;
  dateRtc.Year = date.year;
  dateRtc.WeekDay = RTC_WEEKDAY_MONDAY;
  HAL_RTC_SetDate(&hrtc, &dateRtc, FORMAT_BIN);
}

void getDateTime(tm *time)
{
  RTC_TimeTypeDef timeRtc;
  HAL_RTC_GetTime(&hrtc, &timeRtc, FORMAT_BIN);
  time->tm_hour = timeRtc.Hours;
  time->tm_min = timeRtc.Minutes;
  time->tm_sec = timeRtc.Seconds;

  RTC_DateTypeDef dateRtc;
  HAL_RTC_GetDate(&hrtc, &dateRtc, FORMAT_BIN);
  time->tm_mday = dateRtc.Date;
  time->tm_mon = dateRtc.Month;
  time->tm_year = dateRtc.Year + 2000;
  if((time->tm_mday == 0) || (time->tm_mon == 0)) {
    time->tm_mday = time->tm_mon = 1;
  }
}

void getTime(TimeTypeDef *time)
{
  RTC_TimeTypeDef timeRtc;
  HAL_RTC_GetTime(&hrtc, &timeRtc, FORMAT_BIN);
  time->hours = timeRtc.Hours;
  time->minutes = timeRtc.Minutes;
  time->seconds = timeRtc.Seconds;
  time->mseconds = timeRtc.SubSeconds;
}

void getDate(DateTypeDef *date)
{
  RTC_DateTypeDef dateRtc;
  HAL_RTC_GetDate(&hrtc, &dateRtc, FORMAT_BIN);
  date->date = dateRtc.Date;
  date->month = dateRtc.Month;
  date->year = dateRtc.Year;
  if((date->date == 0) || (date->month == 0)) {
    date->date = date->month = 1;
  }
}

void backupSaveParameter(uint32_t address, uint32_t data)
{
  if (address == RTC_BKP_DR0)
    return;
  HAL_RTCEx_BKUPWrite(&hrtc, address, data);
}

uint32_t backupRestoreParameter(uint32_t address)
{
  if (address == RTC_BKP_DR0)
    return 0x00;
  return HAL_RTCEx_BKUPRead(&hrtc, address);
}

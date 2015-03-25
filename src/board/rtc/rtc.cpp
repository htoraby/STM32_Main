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
    tm dateTime;
    //! Установка времени по умолчанию
    dateTime.tm_year = 2000 - 1900;
    dateTime.tm_mon = 0;
    dateTime.tm_mday = 1;
    dateTime.tm_hour = 0;
    dateTime.tm_min = 0;
    dateTime.tm_sec = 0;

    time_t time = mktime(&dateTime);
    rtcSetTime(&time);

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
      // Error
      asm("nop");
    }
  }

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
    // Error
    asm("nop");
  }

  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  (void)hrtc;
  __HAL_RCC_RTC_DISABLE();
}

void rtcSetTime(const time_t *time)
{
  tm *dateTime = localtime(time);

  RTC_TimeTypeDef timeRtc;
  timeRtc.Hours = dateTime->tm_hour;
  timeRtc.Minutes = dateTime->tm_min;
  timeRtc.Seconds = dateTime->tm_sec;
  timeRtc.TimeFormat = RTC_HOURFORMAT12_AM;
  timeRtc.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  timeRtc.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &timeRtc, FORMAT_BIN);

  RTC_DateTypeDef dateRtc;
  dateRtc.Date = dateTime->tm_mday;
  dateRtc.Month = dateTime->tm_mon;
  dateRtc.Year = dateTime->tm_year - 100;
  dateRtc.WeekDay = RTC_WEEKDAY_MONDAY;
  HAL_RTC_SetDate(&hrtc, &dateRtc, FORMAT_BIN);
}

time_t rtcGetTime()
{
  tm dateTime;
  RTC_TimeTypeDef timeRtc;
  HAL_RTC_GetTime(&hrtc, &timeRtc, FORMAT_BIN);
  dateTime.tm_hour = timeRtc.Hours;
  dateTime.tm_min = timeRtc.Minutes;
  dateTime.tm_sec = timeRtc.Seconds;

  RTC_DateTypeDef dateRtc;
  HAL_RTC_GetDate(&hrtc, &dateRtc, FORMAT_BIN);
  dateTime.tm_mday = dateRtc.Date;
  dateTime.tm_mon = dateRtc.Month;
  dateTime.tm_year = dateRtc.Year + 100;
  if((dateTime.tm_mday == 0) || (dateTime.tm_mon == 0)) {
    dateTime.tm_mday = dateTime.tm_mon = 1;
  }

  return mktime(&dateTime);
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

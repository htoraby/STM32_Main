#ifndef RTC_H
#define RTC_H

#include "board.h"
#include <time.h>

/*!
 \brief Структура времени

*/
typedef struct {
  uint8_t hours;   //! Часы: 0-23
  uint8_t minutes; //! Минуты: 0-59
  uint8_t seconds; //! Секунды: 0-59
  uint32_t mseconds; //! Миллисекунды: 0
} TimeTypeDef;

/*!
 \brief Структура даты

*/
typedef struct {
  uint8_t month; //! Месяц: 1-12 (@ref RTC_Month_Date_Definitions)
  uint8_t date;  //! Дата: 1-31
  uint8_t year;  //! Год: 0-99
} DateTypeDef;

/*!
 \brief Инициализация RTC

*/
void rtcInit();

/*!
 \brief Установка даты и времени

 \param dateTime - структура даты и времени @ref tm
*/
void setDateTime(const tm &time);

/*!
 \brief Установка времени

 \param time - структура времени @ref TimeTypeDef
*/
void setTime(const TimeTypeDef &time);

/*!
 \brief Установка даты

 \param date - структура даты @ref DateTypeDef
*/
void setDate(const DateTypeDef &date);

/*!
 \brief Получение даты и времени

 \param dateTime - структура даты и времени @ref tm
*/
void getDateTime(tm *time);

/*!
 \brief Получение времени
  К осжалению, функция работает некорректно из-за проблем с процессором.
 \param time - структура времени @ref TimeTypeDef
*/
void getTime(TimeTypeDef *time);

/*!
 \brief Получение даты

 \param date - структура даты @ref DateTypeDef
*/
void getDate(DateTypeDef *date);

/*!
 \brief Запись в энергонезависимую статическую память

 \param address: RTC_BKP_DRx 1-19 @ref RTCEx_Backup_Registers_Definitions
 \param data: данные для записи
*/
void backupSaveParameter(uint32_t address, uint32_t data);

/*!
 \brief Чтение из энергонезависимой статическую память

 \param address: RTC_BKP_DRx 1-19 @ref RTCEx_Backup_Registers_Definitions
 \return uint32_t - считанные данные
*/
uint32_t backupRestoreParameter(uint32_t address);

#endif // RTC_H

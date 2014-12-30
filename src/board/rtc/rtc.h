#ifndef RTC_H
#define RTC_H

#include "board.h"
#include <time.h>

/*!
 \brief Инициализация RTC

*/
void rtcInit();

/*!
 \brief Установка даты и времени

 \param dateTime - структура даты и времени @ref tm
*/
void setTime(const time_t *time);

/*!
 \brief Получение даты и времени в секундах

 \return time_t - возвращаемое значение
*/
time_t getTime();

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

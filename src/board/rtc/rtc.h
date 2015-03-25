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

 \param time - время в секундах от 1970
*/
void rtcSetTime(const time_t *time);

/*!
 \brief Получение даты и времени в секундах

 \return time_t - возвращаемое значение в секундах от 1970
*/
time_t rtcGetTime();

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

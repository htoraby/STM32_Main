#ifndef LOG_MAIN_H
#define LOG_MAIN_H

#include "log_event.h"
#include "log_data.h"
#include "log_running.h"
#include "log_alarm.h"
#include "log_tms.h"
#include "log_debug.h"

extern LogEvent logEvent;
extern LogData logData;
extern LogRunning logRunning;
extern LogAlarm logAlarm;
extern LogTms logTms;

/*!
 \brief Инициализация архивов

*/
void logInit();

/*!
 \brief Стирание архивов (в т.ч. стирание всей Flash)

*/
void logErase();

/*!
 \brief Чтение архивов

 \param address - адрес данных
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType logRead(uint32_t address, uint8_t *data, uint32_t size);

#endif // LOG_MAIN_H

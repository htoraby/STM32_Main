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

extern LogDebug logDebug;

#pragma pack(1)

typedef struct {
  unsigned int size;
  unsigned short codeProduction;
  unsigned char codeEquip;
  unsigned char subCodeEquip;
  unsigned short version;
  unsigned int date;
  unsigned int mainLogSize;
  unsigned int debugLogSize;
  unsigned int parametersSize;
} LOG_FILE_HEADER;

typedef struct {
  uint8_t type;
  uint16_t crc;
  uint32_t addr;
  uint32_t inLen;
  uint32_t outLen;
} LOG_HEADER_PKT;

#pragma pack()

/*!
 * \brief Тип области архива
*/
typedef enum {
  HeaderLogType,
  MainLogType,
  DebugLogType,
  ParamsLogType,
} MapLogType;

/*!
 * \brief Инициализация архивов
*/
void logInit();

/*!
 * \brief Запуск сохранения архивов на USB flash
*/
void logStartSave(EventType type);

/*!
 * \brief Запуск стирания архивов (в т.ч. стирание всей Flash)
 * type - Тип события
*/
void logStartDelete(EventType type);

/*!
 * \brief Метод получения заголовка
 * \param header
 */
void logHeader(LOG_FILE_HEADER *header);

/*!
 * \brief Чтение архивов
 * \param address - адрес данных
 * \param data - указатель на данные
 * \param size - размер данных
 * \return StatusType - ошибка или ок
*/
StatusType logRead(uint32_t address, uint8_t *data, uint32_t size);

/*!
 * \brief Чтение программистких архивов
 * \param address - адрес данных
 * \param data - указатель на данные
 * \param size - размер данных
 * \return StatusType - ошибка или ок
*/
StatusType logDebugRead(uint32_t address, uint8_t *data, uint32_t size);

/*!
 * \brief Чтение параметров
 * \param address - адрес данных
 * \param data - указатель на данные
 * \param size - размер данных
 * \return StatusType - ошибка или ок
 */
StatusType logParamsRead(uint32_t address, uint8_t *data, uint32_t size);

bool logCompressRead(uint8_t *data);

#endif // LOG_MAIN_H

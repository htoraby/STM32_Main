#ifndef LOG_H
#define LOG_H

#include "flash_ext.h"
#include "rtc.h"
#include "fram.h"

/*!
 * \brief Список типов журналов
 *
*/
typedef enum {
  EventTypeLog,
  DataTypeLog,
  RunTypeLog,
  AlarmTypeLog,
  TmsTypeLog,
  DebugTypeLog,
} TypeLog;

/*!
 * \brief Начальные адреса журналов на FLASH
 *
*/
typedef enum {
  //! FlashSpi5
  StartAddrEventLog = 0x00000000,  //! 4+1 сектора
  StartAddrDataLog  = 0x00005000,  //! 2357+1 сектора
  StartAddrRunLog   = 0x009AC000,  //! 640+1 секторов
  StartAddrAlarmLog = 0x00C2C000,  //! 640+1 секторов
  StartAddrTmsLog   = 0x00EAC000,  //! 450+1 секторов
  //! FlashSpi1
  StartAddrDebugLog = 0x00000000,  //! все сектора
} StartAddrLog;

/*!
 * \brief Конечные адреса журналов на FLASH
 *
*/
typedef enum {
  EndAddrEventLog = 0x005000,
  EndAddrDataLog  = 0x9ABFFF,
  EndAddrRunLog   = 0xC2BFFF,
  EndAddrAlarmLog = 0xEABFFF,
  EndAddrTmsLog   = 0xEAC000,
  EndAddrDebugLog = 0x000000,
} EndAddrLog;

/*!
 * \brief Базовый класс журналов
 *
 */
class Log
{
public:
  Log(TypeLog type);
  ~Log();

  /*!
   * \brief Инициализация журнала
   *
   */
  void init();

  /*!
   * \brief Деинициализация журнала
   * Установка адреса в начало @ref StartAddrLog
   */
  void deInit();

  /*!
   * \brief Глобальный индекс записей всех журналов
   * Установка адреса в начало @ref StartAddrLog
   */
  static uint32_t id_;

protected:
  /*!
   * \brief Метод записи данных в журнал
   *
   */
  void write(uint8_t *data, uint32_t size);

  /*!
   * \brief Буфер для формирования записи журнала
   *
   */
  uint8_t buffer[256];

private:
  TypeLog type_;
  FlashSpiNum flashSpiNum_;
  uint32_t addrFram_;

  uint32_t address_;
  uint32_t startAddr_;
  uint32_t endAddr_;
  uint32_t sectorSize_;

};

#endif // LOG_H

#ifndef LOG_H
#define LOG_H

#include "flash_ext.h"
#include "rtc.h"
#include "fram.h"

/*!
 * \brief Тип журнала
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
 * \brief Код события
 *
*/
typedef enum {
  NormModeCode = 28,
  FastModeCode = 29,
  RunCode      = 30,
  TmsCode      = 32,
} EventCode;

/*!
 * \brief Вид события
 *
*/
typedef enum {
  NoneType,
  OperatorType,
  AutoType,
  RemoteType,
  ProtType,
  LatchType,
} EventType;

/*!
 * \brief Id события
 *
*/
typedef enum {
  RunId,
} EventId;

/*!
 * \brief Начальные адреса журналов на FLASH
 *
*/
typedef enum {
  //! FlashSpi5
  StartAddrEventLog = 0x00000000,  //!< 4+1 сектора
  StartAddrDataLog  = 0x00005000,  //!< 2357+1 сектора
  StartAddrRunLog   = 0x0093B000,  //!< 640+1 секторов
  StartAddrAlarmLog = 0x00BBC000,  //!< 640+1 секторов
  StartAddrTmsLog   = 0x00E3D000,  //!< 450+1 секторов
  //! FlashSpi1
  StartAddrDebugLog = 0x00000000,  //!< все сектора
} StartAddrLog;

/*!
 * \brief Конечные адреса журналов на FLASH
 *
*/
typedef enum {
  EndAddrEventLog = 0x00005000,
  EndAddrDataLog  = 0x0093B000,
  EndAddrRunLog   = 0x00BBC000,
  EndAddrAlarmLog = 0x00E3D000,
  EndAddrTmsLog   = 0x01000000,
  EndAddrDebugLog = 0x00000000,
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
   * \param data - указатель на записываемые данные
   * \param size - размер данных
   * \param saveId - флаг сохранения Id события, по умолчанию true
   * \param endLog - флаг завершения архива (необходим для архива пусковых характеристик)
   */
  void write(uint8_t *data, uint32_t size, bool saveId = true, bool endLog = false);

  /*!
   * \brief Буфер для формирования записи журнала
   *
   */
  uint8_t buffer[256];

  TypeLog type_;

private:
  FlashSpiNum flashSpiNum_;
  uint32_t addrFram_;

  uint32_t address_;
  uint32_t startAddr_;
  uint32_t endAddr_;
  uint32_t sectorSize_;
  uint32_t addrSectorOld_;

};

#endif // LOG_H

#ifndef LOG_H
#define LOG_H

#include "flash_ext.h"
#include "rtc.h"
#include "fram.h"
#include "log_idlist.h"

#define SIZE_BUF_LOG 256

/*!
 * \brief Тип журнала
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
*/
typedef enum {
  RunCode              = 1,           //!< Пуск
  StopCode             = 2,           //!< Стоп
  ModeCode             = 3,           //!< Изменение режима работы СУ
  BlockCode            = 4,           //!< Блокировка СУ
  UnblockCode          = 5,           //!< Деблокировка СУ
  TripDelayCode        = 8,           //!< Задержка срабатывания
  RestartDelayCode     = 9,           //!< Задержка АПВ
  OpenDoorOperatorCode = 12,          //!< ОткрытаДверь
  SetpointCode         = 13,          //!< ИзмПарам
  DelLogCode           = 14,          //!< Удаление хронологии
  CopyLogCode          = 15,          //!< Считывание хронологии
  UpdateSwCode         = 16,          //!< Перепрошивка КСУ
  ResetCountsCode      = 18,          //!< Сброс счётчиков наработки
  AddDeviceCode        = 20,          //!< Подключение устройства
  RemoveDeviceCode     = 21,          //!< Отключение устройства
  FastModeCode         = 28,          //!< ЗаписьУскРеж
  NormModeCode         = 29,          //!< ЗаписьНормРеж
  TmsCode              = 32,          //!< ЗаписьТМС
  ConnectTmsCode       = 33,          //!< Связь ТМС
  ConnectVsdCode       = 37,          //!< Связь ЧРП
  ConnectEmCode        = 38,          //!< Связь ИМ1
  TurbineRotationCode  = 44,          //!< Турб.Вращ.
  SequencePhaseCode    = 45,          //!< Чередования фаз
  HackSuCode           = 49,          //!< Взлом СУ
  OpenDoorAutoCode     = 66,          //!< Дверь открыт

  ProtectCode          = 100,         //!< Событие защиты
  PowerCode,                          //!< Событие откл/вкл питания
  AlarmCode,                          //!< Запись аварийного архива
  OtherCode,                          //!< Другие события
} EventCode;

/*!
 * \brief Вид события
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
 * \brief Начальные адреса журналов на FLASH
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
*/
typedef enum {
  //! FlashSpi5
  EndAddrEventLog = 0x00005000,
  EndAddrDataLog  = 0x0093B000,
  EndAddrRunLog   = 0x00BBC000,
  EndAddrAlarmLog = 0x00E3D000,
  EndAddrTmsLog   = 0x01000000,
  //! FlashSpi1
  EndAddrDebugLog = 0x00800000,
} EndAddrLog;

/*!
 * \brief Базовый класс журналов
 */
class Log
{
public:
  Log(TypeLog type);
  ~Log();

  /*!
   * \brief Инициализация журнала
   */
  void init();

  /*!
   * \brief Деинициализация журнала
   * Установка адреса в начало @ref StartAddrLog
   */
  void deInit();

  /*!
   * \brief Глобальный индекс записей всех журналов,
   * кроме отладочного
   */
  static uint32_t id_;

  /*!
   * \brief Глобальный индекс записей отладочного архива
   */
  static uint32_t idDebug_;

  uint32_t address() const {
    return address_;
  }

protected:
  /*!
   * \brief Метод записи данных в журнал
   *
   * \param data - указатель на записываемые данные
   * \param size - размер данных
   * \param saveId - флаг сохранения Id события, по умолчанию true
   * \param endLog - флаг завершения архива (необходим для архива пусковых характеристик)
   */
  StatusType write(uint8_t *data, uint32_t size, bool saveId = true, bool endLog = false);

  /*!
   * \brief Буфер для формирования записи журнала
   */
  uint8_t buffer[SIZE_BUF_LOG];

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

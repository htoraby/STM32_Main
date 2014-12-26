#ifndef LOG_H
#define LOG_H

#include "flash_ext.h"
#include "rtc.h"
#include "fram.h"

/*!
 \brief Список типов журналов

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
 \brief Начальные адреса журналов на FLASH

*/
enum {
  //! FlashSpi5
  StartAddrEventLog = 0x000000,  //! 4+1 сектора
  StartAddrDataLog  = 0x005000,  //! 2357+1 сектора
  StartAddrRunLog   = 0x9AC000,  //! 640+1 секторов
  StartAddrAlarmLog = 0xC2C000,  //! 640+1 секторов
  StartAddrTmsLog   = 0xEAC000,  //! 450+1 секторов
  //! FlashSpi1
  StartAddrDebugLog = 0x000000,  //! все сектора
};

/*!
 \brief Конечные адреса журналов на FLASH

*/
enum {
  EndAddrEventLog = 0x005000,
  EndAddrDataLog  = 0x9ABFFF,
  EndAddrRunLog   = 0xC2BFFF,
  EndAddrAlarmLog = 0xEABFFF,
  EndAddrTmsLog   = 0xEAC000,
  EndAddrDebugLog = 0x000000,
};

class Log
{
public:
  Log(TypeLog type);
  ~Log();

  static uint32_t id_;

protected:
  uint8_t buffer[256];
  void write(uint8_t *data, uint32_t size);
  void read(uint32_t idxRec, uint8_t *data, uint32_t count);

private:
  TypeLog type_;
  FlashSpiNum flashSpiNum_;
  uint32_t addrFram_;

  int32_t address_;
  int32_t startAddr_;
  int32_t endAddr_;
  uint32_t sectorSize_;

};

#endif // LOG_H

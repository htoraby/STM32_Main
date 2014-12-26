#include "log.h"

uint32_t Log::id_ = 0;

Log::Log(TypeLog type)
  : type_(type)
{
  if (type_ != DebugTypeLog)
    flashSpiNum_ = FlashSpi5;
  else
    flashSpiNum_ = FlashSpi1;
  sectorSize_ = flashExts[flashSpiNum_].sectorSize;

  switch (type_) {
    case EventTypeLog:
      startAddr_ = StartAddrEventLog;
      addrFram_ = EventLogAddrFram;
      break;
    case DataTypeLog:
      startAddr_ = StartAddrDataLog;
      addrFram_ = DataLogAddrFram;
      break;
    case RunTypeLog:
      startAddr_ = StartAddrRunLog;
      addrFram_ = RunLogAddrFram;
      break;
    case AlarmTypeLog:
      startAddr_ = StartAddrAlarmLog;
      addrFram_ = AlarmLogAddrFram;
      break;
    case TmsTypeLog:
      startAddr_ = StartAddrTmsLog;
      addrFram_ = TmsLogAddrFram;
      break;
    case DebugTypeLog:
      startAddr_ = StartAddrDebugLog;
      addrFram_ = DebugLogAddrFram;
      break;
  }

  framReadData(addrFram_, (uint8_t*)&address_, 4);
  if (address_ == 0)
    address_ = startAddr_;

}

Log::~Log()
{

}

void Log::write(uint8_t *data, uint32_t size)
{
  static uint32_t addrSectorOld = 0;

  flashExtWrite(flashSpiNum_, address_, data, size);

  //! Вычисление адреса для следующей записи
  address_ = address_ + size;
  if (address_ >= endAddr_) {
    address_ = startAddr_;
  }

  //! Проверка на начало нового сектора
  uint32_t addrSector = address_ / sectorSize_ * sectorSize_;
  if (addrSector != addrSectorOld) {
    flashEraseSector4k(flashSpiNum_, address_);
    addrSectorOld = addrSector;
  }

  framWriteData(addrFram_, (uint8_t*)&address_, 4);
}

void Log::read(uint32_t idxRec, uint8_t *data, uint32_t count)
{
//  // address_ = 5
//  uint32_t address;
//  uint32_t size;
//  if ((address_ - incAddr_) < StartAddrEventLog) {
//    address = address_;
//    size = (address_ - StartAddrEventLog)
//    flashExtRead(flashSpiNum_, address, data, incAddr_*count);
//    address = EndAddrEventLog;
//    flashExtRead(flashSpiNum_, address, data, incAddr_*count);
//    asm("nop");
//  } else {

//  }
//  asm("nop");
}

#include "log.h"
#include "user_main.h"

uint32_t Log::id_ = 0;
uint32_t Log::idDebug_ = 0;

Log::Log(TypeLog type)
  : type_(type)
{

}

Log::~Log()
{

}

void Log::init()
{
  if (type_ != DebugTypeLog)
    flashSpiNum_ = FlashSpi5;
  else
    flashSpiNum_ = FlashSpi1;
  sectorSize_ = flashExts[flashSpiNum_].sectorSize;

  switch (type_) {
    case EventTypeLog:
      startAddr_ = StartAddrEventLog;
      endAddr_ = EndAddrEventLog;
      addrFram_ = EventLogAddrFram;
      break;
    case DataTypeLog:
      startAddr_ = StartAddrDataLog;
      endAddr_ = EndAddrDataLog;
      addrFram_ = DataLogAddrFram;
      break;
    case RunTypeLog:
      startAddr_ = StartAddrRunLog;
      endAddr_ = EndAddrRunLog;
      addrFram_ = RunLogAddrFram;
      break;
    case AlarmTypeLog:
      startAddr_ = StartAddrAlarmLog;
      endAddr_ = EndAddrAlarmLog;
      addrFram_ = AlarmLogAddrFram;
      break;
    case TmsTypeLog:
      startAddr_ = StartAddrTmsLog;
      endAddr_ = EndAddrTmsLog;
      addrFram_ = TmsLogAddrFram;
      break;
    case DebugTypeLog:
      startAddr_ = StartAddrDebugLog;
      endAddr_ = EndAddrDebugLog;
      addrFram_ = DebugLogAddrFram;
      break;
  }

  // Получение адреса с которого начнётся следующая запись
  framReadData(addrFram_, (uint8_t*)&address_, 4);
  if (address_ == 0)
    address_ = startAddr_;
  addrSectorOld_ = address_ / sectorSize_ * sectorSize_;
}

void Log::deInit()
{
  address_ = 0;
  framWriteData(addrFram_, (uint8_t*)&address_, 4);
}

StatusType Log::write(uint8_t *data, uint32_t size, bool saveId, bool endLog)
{
  // Проверка отключения питания платы
  if (ksu.isPowerOff())
    return StatusError;

  flashExtWrite(flashSpiNum_, address_, data, size);

  // Вычисление адреса для следующей записи
  address_ = address_ + size;
  if ((address_ + size) > endAddr_) {
    address_ = startAddr_;
  }

  // Проверка на начало нового сектора
  uint32_t addrSector = (address_ + size - 1) / sectorSize_ * sectorSize_;
  if (endLog) {
    addrSector = addrSector + sectorSize_;
    if ((addrSector + sectorSize_*64) >= endAddr_)
      addrSector = startAddr_;
  }

  if (addrSector != addrSectorOld_) {
    address_ = addrSector;
    flashExtEraseSector4k(flashSpiNum_, address_);
    addrSectorOld_ = addrSector;
  }

  // Сохранение глабального индекса записей
  if (saveId) {   
    if (type_ != DebugTypeLog) {
      if (id_ == 0xFFFFFFFF)
        id_ = 0;
      framWriteData(IdLogAddrFram, (uint8_t*)&id_, 4);
    }
    else {
      if (idDebug_ == 0xFFFFFFFF)
        idDebug_ = 0;
      framWriteData(IdDebugLogAddrFram, (uint8_t*)&idDebug_, 4);
    }
  }
  // Сохранение адреса с которого начнётся следующая запись
  framWriteData(addrFram_, (uint8_t*)&address_, 4);

  return StatusOk;
}

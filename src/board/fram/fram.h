#ifndef FRAM_H
#define FRAM_H

#include "board.h"

#define FRAM_SIZE  0x20000 // 128KB
#define FRAM_END   0x0001FFFF

/*!
 \brief Список занятых адресов FRAM

*/
typedef enum {
  IdDebugLogAddrFram = 0x01FFC4,
  IdLogAddrFram      = 0x01FFC8,
  EventLogAddrFram   = 0x01FFCC,
  DataLogAddrFram    = 0x01FFD0,
  RunLogAddrFram     = 0x01FFD4,
  AlarmLogAddrFram   = 0x01FFD8,
  TmsLogAddrFram     = 0x01FFDC,
  DebugLogAddrFram   = 0x01FFE0,
  RcauseAddrFram     = 0x01FFE4,
} AddrFram;

/*!
 \brief Инициализация FRAM FM25V10 128KB (SPI3)
*/
void framInit();

/*!
 \brief Запись данных во FRAM

 \param address - адрес первого байта
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType framWriteData(uint32_t address, uint8_t *data, uint32_t size);

/*!
 \brief Чтение данных из FRAM

 \param address - адрес первого байта
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType framReadData(uint32_t address, uint8_t *data, uint32_t size);

#endif // FRAM_H

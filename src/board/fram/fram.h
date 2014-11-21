#ifndef FRAM_H
#define FRAM_H

#include "board.h"

#define FRAM_SIZE  131072 // 128KB
#define FRAM_END   0x0001FFFF

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

#ifndef FLASH_H
#define FLASH_H

#include "board.h"

/*!
 \brief Список SPI каналов задействованых для Flash памяти

*/
typedef enum {
  FlashSpi1,
  FlashSpi5,
  FlashSpiMax,
} FlashSpiNum;

/*!
 \brief Инициализация внешней Flash:
 SPI1 - W25Q64BV, SPI5 - MX25L128
 \param num - номер spi @ref FlashSpiNum
*/
void flashExtInit(FlashSpiNum num);

/*!
 \brief Запись данных во Flash
 Желательно писать данные размером с сектор 4096
 \param num - номер spi @ref FlashSpiNum
 \param address - адрес данных
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType flashExtWrite(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size);

/*!
 \brief Чтение данных из Flash

 \param num - номер spi @ref FlashSpiNum
 \param address - адрес данных
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType flashExtRead(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size);

/*!
 \brief Стирание всей Flash памяти
 Длительная операция! Занимает около 3 секунд
 \param num - номер spi @ref FlashSpiNum
 \return StatusType - ошибка или ок
*/
StatusType flashExtChipErase(FlashSpiNum num);

#endif // FLASH_H

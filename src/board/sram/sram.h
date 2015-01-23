#ifndef SRAM_H
#define SRAM_H

#include "board.h"

#define SRAM_ADDR_START ((uint32_t)0x60000000)
#define SRAM_SIZE       1024*1024      //!< Размер ОЗУ 1MB

typedef enum {
  bit8,
  bit16,
  bit32,
} NumberBits;

/*!
 \brief Инициализация внешней SRAM CY62157EV30
  Пример объявления переменной: uint8_t tmp __attribute__((section(".extmem")));
*/
void sramInit();

/*!
 \brief Запись данных во внешнею SRAM

 \param address - начальный адрес в памяти начиная от @ref SRAM_ADDR_START
 \param data - указатель на данные
 \param size - размер данных
 \param numberBits - количество битов в слове данных (@ref NumberBits)
 \return StatusSram - ошибка или ок
*/
StatusType sramWriteData(uint32_t address, uint32_t *data,
                         uint32_t size, NumberBits bits = bit32);

/*!
 \brief Чтение данных из внешней SRAM

 \param address - начальный адрес в памяти начиная от @ref SRAM_ADDR_START
 \param data - указатель на данные
 \param size - размер данных
 \param bits - количество битов в слове данных (@ref NumberBits)
 \return StatusSram - ошибка или ок
*/
StatusType sramReadData(uint32_t address, uint32_t *data,
                        uint32_t size, NumberBits bits = bit32);

#endif // SRAM_H

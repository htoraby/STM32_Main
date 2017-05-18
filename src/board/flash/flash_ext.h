#ifndef FLASH_H
#define FLASH_H

#include "board.h"

#define FLASH_TIMEOUT 5000
#define FLASH_SECTOR_SIZE 4096

/*!
 * \brief Список SPI каналов задействованых для Flash памяти
*/
typedef enum {
  FlashSpi1,
  FlashSpi5,
  FlashSpiMax,
} FlashSpiNum;

typedef struct {
  SPI_HandleTypeDef spi;  //!< Структура SPI
  osSemaphoreId cmdSemaphoreId;
  volatile uint8_t spiReady;
  GPIO_TypeDef* nss_port; //!< Порт вывода NSS
  uint16_t nss_pin;       //!< Номер вывода NSS
  uint8_t manufacturer;   //!< Производитель
  uint32_t size;          //!< Размер общей памяти
  uint32_t pageSize;      //!< Размер страницы
  uint32_t sectorSize;    //!< Размер сектора
  uint32_t blockSize;     //!< Размер блока
  volatile uint8_t *data;
} FlashTypeDef;

extern FlashTypeDef flashExts[];

#ifdef __cplusplus

/*!
 \brief Инициализация внешней Flash: SPI1, SPI5
 \param num - номер spi @ref FlashSpiNum
*/
void flashExtInit(FlashSpiNum num);

/*!
 \brief Метод обработки прерывания Flash

 \param num - номер spi @ref FlashSpiNum
*/
void flashTxRxCpltCallback(FlashSpiNum num);

/*!
 \brief Запись данных во Flash для архивов
 Запись без стирания сектора и с учётом перехода на новый сектор
 \param num - номер spi @ref FlashSpiNum
 \param address - адрес данных
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType flashExtWrite(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size);

/*!
 \brief Запись данных во Flash (универсальная)
 Желательно писать данные размером с сектор 4096
 \param num - номер spi @ref FlashSpiNum
 \param address - адрес данных
 \param data - указатель на данные
 \param size - размер данных
 \return StatusType - ошибка или ок
*/
StatusType flashExtWriteEx(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size);

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
 \brief Стирание сектора (4k) Flash
 Операция занимает около 17 мс
 \param num - номер spi @ref FlashSpiNum
 \param address - адрес сектора
 \return StatusType - ошибка или ок
*/
StatusType flashExtEraseSector4k(FlashSpiNum num, uint32_t address);

/*!
 * \brief Стирание блока (64k) Flash
 * \param num
 * \param address
 * \return
 */
StatusType flashExtEraseBlock(FlashSpiNum num, uint32_t address);

/*!
 \brief Стирание всей Flash памяти
 Длительная операция! Занимает около 3-60 секунд
 \param num - номер spi @ref FlashSpiNum
 \return StatusType - ошибка или ок
*/
StatusType flashExtChipErase(FlashSpiNum num);

#endif

#endif // FLASH_H

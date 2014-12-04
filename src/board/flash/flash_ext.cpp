#include "flash_ext.h"
#include "gpio.h"
#include "common.h"

#include <string.h>

#define CMD_JEDEC_ID 0x9F

#if USE_EXT_MEM
  static uint8_t buffer[4096] __attribute__((section(".extmem")));
#else
  static uint8_t buffer[4096];
#endif

/*!
 \brief Спискок возможных производителей
*/
enum {
  VENDOR_ATMEL	  =	0x1F,
  VENDOR_WINBOND	=	0xEF,
  VENDOR_MACRONIX	=	0xC2,
};

/*!
 \brief Команды для Flash ATMEL
*/
enum {
  CMD_CONT_ARRAY_READ = 0x03,
  CMD_STATUS_REGISTER_READ = 0xD7,
  CMD_MAIN_MEM_PAGE_TO_BUF_TRANSFER = 0x53,
  CMD_MAIN_MEM_PAGE_PROGRAM_THROUGH_BUF = 0x85,
  CMD_AUTO_PAGE_REWRITE = 0x58,
};

/*!
 \brief Команды для Flash WINBOND и MACRONIX
*/
enum {
  CMD_W_FAST_READ          = 0x0B,
  CMD_W_GET_ID             = 0x90,
  CMD_W_WRITE_ENABLE       = 0x06,
  CMD_W_WRITE_DISABLE      = 0x04,
  CMD_W_PAGE_PROGRAM       = 0x02,
  CMD_W_READ_SR            = 0x05,
  CMD_W_READ_DATA          = 0x03,
  CMD_W_CHIP_ERASE         = 0xC7,
  CMD_W_SECTOR_ERASE       = 0x20,
  CMD_W_BLOCK64k_ERASE     = 0xD8,
  CMD_W_BLOCK32k_ERASE     = 0x52,
  CMD_W_POWER_DOWN         = 0xB9,
  CMD_W_WAKE_UP            = 0xAB,
  CMD_W_WRITE_SR           = 0x01,
  CMD_W_FAST_READ_DUAL_OUT = 0x3B,
};

typedef struct {
  SPI_HandleTypeDef spi;  //! Структура SPI
  GPIO_TypeDef* nss_port; //! Порт вывода NSS
  uint16_t nss_pin;       //! Номер вывода NSS
  uint8_t manufacturer;   //! Производитель
  uint32_t size;          //! Размер общей памяти
  uint32_t pageSize;      //! Размер страницы
  uint32_t sectorSize;    //! Размер сектора
  uint32_t blockSize;     //! Размер блока
} FlashTypeDef;

FlashTypeDef flashExts[FlashSpiMax];

static StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData,
                                     uint8_t *rxData, uint16_t txSize,
                                     uint16_t rxSize);
static void flashWriteEnable(FlashSpiNum num);
static void flashWriteDisable(FlashSpiNum num);
static StatusType flashEraseSector4k(FlashSpiNum num, uint32_t address);

void flashExtInit(FlashSpiNum num)
{
  uint8_t buf[10];

  SPI_HandleTypeDef *spiX = &flashExts[num].spi;
  switch (num) {
    case FlashSpi1:
      spiX->Instance = SPI1;
      // PA4 ------> SPI1_NSS
      flashExts[num].nss_port = GPIOA;
      flashExts[num].nss_pin  = GPIO_PIN_4;
      break;
    case FlashSpi5:
      spiX->Instance = SPI5;
      // PC9 ------> SPI5_NSS
      flashExts[num].nss_port = GPIOC;
      flashExts[num].nss_pin  = GPIO_PIN_9;
      break;
    default:
      break;
  }

  initPinOut(flashExts[num].nss_port, flashExts[num].nss_pin, GPIO_PIN_SET);

  spiX->Init.Mode = SPI_MODE_MASTER;
  spiX->Init.Direction = SPI_DIRECTION_2LINES;
  spiX->Init.DataSize = SPI_DATASIZE_8BIT;
  spiX->Init.CLKPolarity = SPI_POLARITY_LOW;
  spiX->Init.CLKPhase = SPI_PHASE_1EDGE;
  spiX->Init.NSS = SPI_NSS_SOFT;
  spiX->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  spiX->Init.FirstBit = SPI_FIRSTBIT_MSB;
  spiX->Init.TIMode = SPI_TIMODE_DISABLED;
  spiX->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  spiX->Init.CRCPolynomial = 7;
  HAL_SPI_Init(spiX);

  //! Чтение данных из JEDEC ID регистров для определения производителя
  buf[0] = CMD_JEDEC_ID;
  spiTransmitReceive(num, &buf[0], &buf[0], 1, 4);

  //! Определение производителя
  flashExts[num].manufacturer = buf[0];
  switch (flashExts[num].manufacturer) {
    case VENDOR_ATMEL:
      flashExts[num].size = 0;
      flashExts[num].pageSize = 256;
      flashExts[num].sectorSize = 4096;
      flashExts[num].blockSize = 65536;
      break;
    case VENDOR_WINBOND:
      flashExts[num].size = 0x00800000;
      flashExts[num].pageSize = 256;
      flashExts[num].sectorSize = 4096;
      flashExts[num].blockSize = 65536;
      break;
    case VENDOR_MACRONIX:
      flashExts[num].size = 0x01000000;
      flashExts[num].pageSize = 256;
      flashExts[num].sectorSize = 4096;
      flashExts[num].blockSize = 65536;
      break;
    default:
      break;
  }

  asm("nop");
}

StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData, uint8_t *rxData,
                             uint16_t txSize, uint16_t rxSize)
{
  StatusType status = StatusError;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, txData, txSize, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&flashExts[num].spi, rxData, rxData, rxSize, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  return status;
}

uint8_t flashReady(FlashSpiNum num)
{
  uint8_t statusReg = CMD_W_READ_SR;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &statusReg, 1, 1000);
  HAL_SPI_TransmitReceive(&flashExts[num].spi, &statusReg, &statusReg, 1, 1000);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if(!(statusReg & 0x01))
    return 1;
  return 0;
}

void flashWriteEnable(FlashSpiNum num)
{
  uint8_t cmd = CMD_W_WRITE_ENABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, 1000);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

void flashWriteDisable(FlashSpiNum num)
{
  uint8_t cmd = CMD_W_WRITE_DISABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, 1000);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

StatusType flashEraseSector4k(FlashSpiNum num, uint32_t address)
{
  StatusType status = StatusError;
  uint8_t buf[4];
  address &= ~(flashExts[num].sectorSize - 1);

  flashWriteEnable(num);

  buf[0] = CMD_W_SECTOR_ERASE;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 4, 1000) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  //! Ожидание завершения операции
  while(!flashReady(num));
  flashWriteDisable(num);

  return status;
}

StatusType flashExtRead(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;
  uint8_t buf[5];

  if (address > flashExts[num].size)
    return status;

  buf[0] = CMD_W_FAST_READ;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  buf[4] = 0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 5, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&flashExts[num].spi, data, data, size, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  return status;
}

StatusType flashWritePage(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;
  uint8_t buf[4];

  if (address > flashExts[num].size)
    return status;

  flashWriteEnable(num);

  buf[0] = CMD_W_PAGE_PROGRAM;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 4, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_Transmit(&flashExts[num].spi, data, size, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  //! Ожидание завершения операции
  while(!flashReady(num));
  flashWriteDisable(num);

  return status;
}

StatusType flashWriteSector(FlashSpiNum num, uint32_t address,
                            uint8_t *data, uint32_t size)
{
  uint32_t offSetPage = 0;
  uint32_t len = 0;

  flashEraseSector4k(num, address);

  while(size) {
    if(address > flashExts[num].size)
      break;

    offSetPage = address % flashExts[num].pageSize;
    len = flashExts[num].pageSize - offSetPage;
    if(size < len)
      len = size;

    if (flashWritePage(num, address, data, len) != StatusOk)
      return StatusError;

    address += len;
    data += len;
    size -= len;
  }

  return StatusOk;
}

StatusType flashExtWrite(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  uint32_t offSetPage = 0;
  uint32_t len = 0;
  uint32_t addrSector;

  while(size) {
    if(address > flashExts[num].size)
      break;

    addrSector = address/flashExts[num].sectorSize*flashExts[num].sectorSize;
    offSetPage = address % flashExts[num].sectorSize;
    len = flashExts[num].sectorSize - offSetPage;
    if(size < len)
      len = size;
    memset(buffer, 0, sizeof(buffer));

    if (flashExtRead(num, addrSector, buffer, flashExts[num].sectorSize) != StatusOk)
      return StatusError;
    for(uint32_t i = 0; i < len; i++) {
      buffer[offSetPage + i] = data[i];
    }

    if (flashWriteSector(num, addrSector, buffer, flashExts[num].sectorSize) != StatusOk)
      return StatusError;

    address += len;
    data += len;
    size -= len;
  }

  return StatusOk;
}

StatusType flashExtChipErase(FlashSpiNum num)
{
  StatusType status = StatusError;

  flashWriteEnable(num);

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  uint8_t cmd = CMD_W_CHIP_ERASE;
  if (HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, 1000) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  //! Ожидание завершения операции
  while(!flashReady(num));
  flashWriteDisable(num);

  return status;
}

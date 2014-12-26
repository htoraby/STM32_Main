#include "flash_ext.h"
#include "gpio.h"
#include "common.h"

#include <string.h>

#define CMD_JEDEC_ID 0x9F

#if USE_EXT_MEM
  static uint8_t bufData[4096] __attribute__((section(".extmem")));
#else
  static uint8_t bufData[4096];
#endif
  static uint8_t buf[10];

#define TIMEOUT 1000

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

DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi5_tx;
DMA_HandleTypeDef hdma_spi5_rx;
FlashTypeDef flashExts[FlashSpiMax];

static StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData,
                                     uint8_t *rxData, uint16_t txSize,
                                     uint16_t rxSize);
static void flashWriteEnable(FlashSpiNum num);
static void flashWriteDisable(FlashSpiNum num);

void flashExtInit(FlashSpiNum num)
{
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

  initPinOut(flashExts[num].nss_port, flashExts[num].nss_pin, PinSet);

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


  /* Peripheral DMA init*/
  if (num == FlashSpi1) {
    hdma_spi1_tx.Instance = DMA2_Stream5;
    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_spi1_tx);

    __HAL_LINKDMA(spiX, hdmatx, hdma_spi1_tx);

    hdma_spi1_rx.Instance = DMA2_Stream0;
    hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_rx.Init.Mode = DMA_NORMAL;
    hdma_spi1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_spi1_rx);

    __HAL_LINKDMA(spiX, hdmarx, hdma_spi1_rx);

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  }
  else if (num == FlashSpi5) {
    hdma_spi5_tx.Instance = DMA2_Stream4;
    hdma_spi5_tx.Init.Channel = DMA_CHANNEL_2;
    hdma_spi5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_tx.Init.Mode = DMA_NORMAL;
    hdma_spi5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_spi5_tx);

    __HAL_LINKDMA(spiX, hdmatx, hdma_spi5_tx);

    hdma_spi5_rx.Instance = DMA2_Stream3;
    hdma_spi5_rx.Init.Channel = DMA_CHANNEL_2;
    hdma_spi5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_rx.Init.Mode = DMA_NORMAL;
    hdma_spi5_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_spi5_rx);

    __HAL_LINKDMA(spiX, hdmarx, hdma_spi5_rx);

    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  }

  flashExts[num].cmdSemaphoreId = osSemaphoreCreate(NULL, 1);
  flashExts[num].operSemaphoreId = osSemaphoreCreate(NULL, 1);


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

void flashTxRxCpltCallback(FlashSpiNum num)
{
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);
  osSemaphoreRelease(flashExts[num].operSemaphoreId);
}

StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData, uint8_t *rxData,
                             uint16_t txSize, uint16_t rxSize)
{
  StatusType status = StatusError;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, txData, txSize, TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&flashExts[num].spi, rxData, rxData, rxSize, TIMEOUT) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  return status;
}

uint8_t flashReady(FlashSpiNum num)
{
  uint8_t statusReg = CMD_W_READ_SR;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &statusReg, 1, TIMEOUT);
  HAL_SPI_TransmitReceive(&flashExts[num].spi, &statusReg, &statusReg, 1, TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if(!(statusReg & 0x01))
    return 1;
  return 0;
}

static void flashWriteEnable(FlashSpiNum num)
{
  uint8_t cmd = CMD_W_WRITE_ENABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

static void flashWriteDisable(FlashSpiNum num)
{
  uint8_t cmd = CMD_W_WRITE_DISABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

StatusType flashEraseSector4k(FlashSpiNum num, uint32_t address)
{
  StatusType status = StatusError;
  address &= ~(flashExts[num].sectorSize - 1);

  flashWriteEnable(num);

  buf[0] = CMD_W_SECTOR_ERASE;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 4, TIMEOUT) == HAL_OK)
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

  if (address > flashExts[num].size)
    return status;

  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, TIMEOUT) == osEventTimeout)
    return status;

  buf[0] = CMD_W_FAST_READ;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  buf[4] = 0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 5, TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    osSemaphoreWait(flashExts[num].operSemaphoreId, 0);

    if (HAL_SPI_Receive_DMA(&flashExts[num].spi, data, size) == HAL_OK)
      status = StatusOk;

    if (osSemaphoreWait(flashExts[num].operSemaphoreId, TIMEOUT) == osEventTimeout) {
      flashTxRxCpltCallback(num);
      status = StatusError;
    }
  }

  return status;
}

StatusType flashWritePage(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;

  if (address > flashExts[num].size)
    return status;

  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, TIMEOUT) == osEventTimeout)
    return status;

  flashWriteEnable(num);

  buf[0] = CMD_W_PAGE_PROGRAM;
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, &buf[0], 4, TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    osSemaphoreWait(flashExts[num].operSemaphoreId, 0);

    if (HAL_SPI_Transmit_DMA(&flashExts[num].spi, data, size) == HAL_OK)
      status = StatusOk;

    if (osSemaphoreWait(flashExts[num].operSemaphoreId, TIMEOUT) == osEventTimeout) {
      flashTxRxCpltCallback(num);
      status = StatusError;
    }
  }

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
    memset(bufData, 0, sizeof(bufData));

    if (flashExtRead(num, addrSector, bufData, flashExts[num].sectorSize) != StatusOk)
      return StatusError;
    for(uint32_t i = 0; i < len; i++) {
      bufData[offSetPage + i] = data[i];
    }

    if (flashWriteSector(num, addrSector, bufData, flashExts[num].sectorSize) != StatusOk)
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
  if (HAL_SPI_Transmit(&flashExts[num].spi, &cmd, 1, TIMEOUT) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  //! Ожидание завершения операции
  while(!flashReady(num));
  flashWriteDisable(num);

  return status;
}

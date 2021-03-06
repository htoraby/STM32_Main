#include "flash_ext.h"
#include "gpio.h"
#include "common.h"

#include <string.h>

#define CMD_JEDEC_ID 0x9F

static volatile uint8_t bufData[FlashSpiMax][FLASH_SECTOR_SIZE];

/*!
 \brief Спискок возможных производителей
*/
enum {
  VENDOR_ATMEL	    =	0x1F,
  VENDOR_WINBOND	  =	0xEF,
  VENDOR_MACRONIX	  =	0xC2,
  VENDOR_GIGADEVICE	=	0xC8,
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
 \brief Команды для Flash WINBOND, MACRONIX и GIGADEVICE
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
  CMD_W_ADDRESS_MODE       = 0xB7,
};

DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi5_tx;
DMA_HandleTypeDef hdma_spi5_rx;
FlashTypeDef flashExts[FlashSpiMax];

static StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData,
                                     uint8_t *rxData, uint16_t txSize,
                                     uint16_t rxSize);
static void flashSetAddressMode(FlashSpiNum num);
static void flashWriteEnable(FlashSpiNum num);
static void flashWriteDisable(FlashSpiNum num);

void flashExtInit(FlashSpiNum num)
{
  volatile uint8_t buf[10];

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
  spiX->Init.TIMode = SPI_TIMODE_DISABLE;
  spiX->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
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

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, FLASH_IRQ_PREPRIO, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, FLASH_IRQ_PREPRIO, 0);
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

    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, FLASH_IRQ_PREPRIO, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, FLASH_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  }

  flashExts[num].cmdSemaphoreId = osSemaphoreCreate(NULL, 1);
  flashExts[num].data = bufData[num];

  // Чтение данных из JEDEC ID регистров для определения производителя
  buf[0] = CMD_JEDEC_ID;
  spiTransmitReceive(num, (uint8_t *)&buf[0], (uint8_t *)&buf[0], 1, 4);

  // Определение производителя
  flashExts[num].manufacturer = buf[0];
  switch (flashExts[num].manufacturer) {
  case VENDOR_ATMEL:
    flashExts[num].size = 0;
    flashExts[num].pageSize = 256;
    flashExts[num].sectorSize = FLASH_SECTOR_SIZE;
    flashExts[num].blockSize = 65536;
    break;
  case VENDOR_WINBOND:
    flashExts[num].size = 0x00800000;
    flashExts[num].pageSize = 256;
    flashExts[num].sectorSize = FLASH_SECTOR_SIZE;
    flashExts[num].blockSize = 65536;
    break;
  case VENDOR_MACRONIX:
    flashExts[num].size = 0x01000000;
    flashExts[num].pageSize = 256;
    flashExts[num].sectorSize = FLASH_SECTOR_SIZE;
    flashExts[num].blockSize = 65536;
    break;
  case VENDOR_GIGADEVICE:
    flashExts[num].size = 0x02000000;
    flashExts[num].pageSize = 256;
    flashExts[num].sectorSize = FLASH_SECTOR_SIZE;
    flashExts[num].blockSize = 65536;

    flashSetAddressMode(num);
    break;
  default:
    flashExts[num].size = 0x02000000;
    flashExts[num].pageSize = 256;
    flashExts[num].sectorSize = FLASH_SECTOR_SIZE;
    flashExts[num].blockSize = 65536;

    flashSetAddressMode(num);
    break;
  }
}

void flashTxRxCpltCallback(FlashSpiNum num)
{
  flashExts[num].spiReady = true;
}

static StatusType spiTransmitReceive(FlashSpiNum num, uint8_t *txData, uint8_t *rxData,
                             uint16_t txSize, uint16_t rxSize)
{
  StatusType status = StatusError;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, txData, txSize, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&flashExts[num].spi, rxData, rxData, rxSize, FLASH_TIMEOUT) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  return status;
}

static void flashSetAddressMode(FlashSpiNum num)
{
  volatile uint8_t buf[10];

#if (HARDWARE_VERSION >= 0x0200)
  buf[0] = CMD_W_ADDRESS_MODE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, 1, FLASH_TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
#endif

  buf[0] = 0x35;
  buf[1] = 0xFF;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_TransmitReceive(&flashExts[num].spi, (uint8_t *)buf, (uint8_t *)buf, 2, FLASH_TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

static StatusType flashDmaWait(FlashSpiNum num)
{
  int timeOut = FLASH_TIMEOUT;
  int attempt = 0;
  while (!flashExts[num].spiReady) {
    if (++attempt > 500) {
      attempt = 0;
      osDelay(1);
      if (--timeOut <= 0)
        return StatusError;
    }
  }
  return StatusOk;
}

static void flashReady(FlashSpiNum num)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  int attempt = 0;
  while (1) {
    buf[0] = CMD_W_READ_SR;
    buf[1] = 0xFF;
    clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
    if (HAL_SPI_TransmitReceive(&flashExts[num].spi, (uint8_t *)buf, (uint8_t *)buf, 2, FLASH_TIMEOUT) == HAL_OK)
      status = StatusOk;
    setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

    if (status == StatusError)
      return;
    if(!(buf[1] & 0x01)) {
      return;
    }
    if (++attempt > 500) {
      attempt = 0;
      osDelay(1);
    }
  }
}

static void flashWriteEnable(FlashSpiNum num)
{
  volatile uint8_t buf[10];
  buf[0] = CMD_W_WRITE_ENABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, 1, FLASH_TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

static void flashWriteDisable(FlashSpiNum num)
{
  volatile uint8_t buf[10];
  buf[0] = CMD_W_WRITE_DISABLE;
  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, 1, FLASH_TIMEOUT);
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
}

static StatusType flashEraseSector(FlashSpiNum num, uint32_t address)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  address &= ~(flashExts[num].sectorSize - 1);

  flashWriteEnable(num);

  uint8_t sizeTx = 5;
  buf[0] = CMD_W_SECTOR_ERASE;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  sizeTx = 4;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  // Ожидание завершения операции
  osDelay(20);
  flashReady(num);
  flashWriteDisable(num);

  return status;
}

static StatusType flashRead(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (address > flashExts[num].size)
    return status;

  uint8_t sizeTx = 6;
  buf[0] = CMD_W_FAST_READ;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
  buf[5] = 0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  buf[4] = 0xFF;
  sizeTx = 5;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    flashExts[num].spiReady = false;
    if (HAL_SPI_Receive_DMA(&flashExts[num].spi, data, size) != HAL_OK)
      status = StatusError;
    else
      status = flashDmaWait(num);
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  return status;
}

static StatusType flashWritePage(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (address > flashExts[num].size)
    return status;

  flashWriteEnable(num);

  uint8_t sizeTx = 5;
  buf[0] = CMD_W_PAGE_PROGRAM;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  sizeTx = 4;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    flashExts[num].spiReady = false;
    if (HAL_SPI_Transmit_DMA(&flashExts[num].spi, data, size) != HAL_OK)
      status = StatusError;
    else
      status = flashDmaWait(num);
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  // Ожидание завершения операции
  flashReady(num);
  flashWriteDisable(num);

  return status;
}

static StatusType flashWriteSector(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  uint32_t offSetPage = 0;
  uint32_t len = 0;

  flashEraseSector(num, address);

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

StatusType flashExtRead(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, 3*FLASH_TIMEOUT) != osOK)
    return StatusError;

  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (address > flashExts[num].size)
    return status;

  osDelay(1);

  uint8_t sizeTx = 6;
  buf[0] = CMD_W_FAST_READ;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
  buf[5] = 0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  buf[4] = 0xFF;
  sizeTx = 5;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    flashExts[num].spiReady = false;
    if (HAL_SPI_Receive_DMA(&flashExts[num].spi, data, size) != HAL_OK)
      status = StatusError;
    else
      status = flashDmaWait(num);
  }
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

StatusType flashExtWrite(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, 3*FLASH_TIMEOUT) != osOK)
    return StatusError;

  StatusType status = StatusOk;
  uint32_t offSetPage = 0;
  uint32_t len = 0;

  while(size) {
    if(address > flashExts[num].size) {
      status = StatusError;
      break;
    }

    offSetPage = address % flashExts[num].pageSize;
    len = flashExts[num].pageSize - offSetPage;
    if(size < len)
      len = size;

    status = flashWritePage(num, address, data, len);
    if (status != StatusOk)
      break;

    address += len;
    data += len;
    size -= len;
  }

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

StatusType flashExtWriteEx(FlashSpiNum num, uint32_t address, uint8_t *data, uint32_t size)
{
  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, 3*FLASH_TIMEOUT) != osOK)
    return StatusError;

  StatusType status = StatusOk;
  uint32_t offSetPage = 0;
  uint32_t len = 0;
  uint32_t addrSector;

  while(size) {
    if(address > flashExts[num].size) {
      status = StatusError;
      break;
    }

    addrSector = address/flashExts[num].sectorSize*flashExts[num].sectorSize;
    offSetPage = address % flashExts[num].sectorSize;
    len = flashExts[num].sectorSize - offSetPage;
    if(size < len)
      len = size;
    memset((uint8_t*)flashExts[num].data, 0, sizeof(bufData[0]));

    status = flashRead(num, addrSector, (uint8_t*)flashExts[num].data, flashExts[num].sectorSize);
    if (status != StatusOk)
      break;
    for(uint32_t i = 0; i < len; i++) {
      flashExts[num].data[offSetPage + i] = data[i];
    }

    status = flashWriteSector(num, addrSector, (uint8_t*)flashExts[num].data, flashExts[num].sectorSize);
    if (status != StatusOk)
      break;

    address += len;
    data += len;
    size -= len;
  }

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

StatusType flashExtEraseSector4k(FlashSpiNum num, uint32_t address)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, FLASH_TIMEOUT) != osOK)
    return status;

  address &= ~(flashExts[num].sectorSize - 1);

  flashWriteEnable(num);

  uint8_t sizeTx = 5;
  buf[0] = CMD_W_SECTOR_ERASE;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  sizeTx = 4;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  // Ожидание завершения операции
  osDelay(20);
  flashReady(num);
  flashWriteDisable(num);

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

StatusType flashExtEraseBlock(FlashSpiNum num, uint32_t address)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, FLASH_TIMEOUT) != osOK)
    return status;

  address &= ~(flashExts[num].sectorSize - 1);

  flashWriteEnable(num);

  uint8_t sizeTx = 5;
  buf[0] = CMD_W_BLOCK64k_ERASE;
#if (HARDWARE_VERSION >= 0x0200)
  buf[1] = address>>24;
  buf[2] = address>>16;
  buf[3] = address>>8;
  buf[4] = address&0xFF;
#else
  buf[1] = address>>16;
  buf[2] = address>>8;
  buf[3] = address&0xFF;
  sizeTx = 4;
#endif

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, sizeTx, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  // Ожидание завершения операции
  osDelay(200);
  flashReady(num);
  flashWriteDisable(num);

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

StatusType flashExtChipErase(FlashSpiNum num)
{
  StatusType status = StatusError;
  volatile uint8_t buf[10];

  if (osSemaphoreWait(flashExts[num].cmdSemaphoreId, FLASH_TIMEOUT) != osOK)
    return status;

  flashWriteEnable(num);

  clrPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);
  buf[0] = CMD_W_CHIP_ERASE;
  if (HAL_SPI_Transmit(&flashExts[num].spi, (uint8_t *)buf, 1, FLASH_TIMEOUT) == HAL_OK)
    status = StatusOk;
  setPinOut(flashExts[num].nss_port, flashExts[num].nss_pin);

  // Ожидание завершения операции
  osDelay(100000);
  flashReady(num);
  flashWriteDisable(num);

  osSemaphoreRelease(flashExts[num].cmdSemaphoreId);

  return status;
}

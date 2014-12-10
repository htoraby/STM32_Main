#include "fram.h"
#include "gpio.h"

#define FRAM_NSS_PIN   GPIO_PIN_15
#define FRAM_NSS_PORT  GPIOA

/*!
 \brief Команды для FRAM FM25V10

*/
enum {
  CMD_WREN = 0x06,
  CMD_WRDI = 0x04,
  CMD_RDSR = 0x05,
  CMD_WRSR = 0x01,
  CMD_READ = 0x03,
  CMD_FSTRD = 0x0b,
  CMD_WRITE = 0x02,
  CMD_SLEEP = 0xb9,
  CMD_RDID = 0x9f,
  CMD_SNR = 0xc3,
};

SPI_HandleTypeDef hspi3;

static StatusType spiTransmitReceive(uint8_t *txData, uint8_t *rxData,
                                    uint16_t txSize, uint16_t rxSize);
static StatusType spiTransmit(uint8_t *data, uint16_t size);

void framInit()
{
  uint8_t buffer[10];

  // PA15     ------> SPI3_NSS
  initPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN, PinSet);

  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi3.Init.CRCPolynomial = 7;
  HAL_SPI_Init(&hspi3);

  //! Разрешение записи
  buffer[0] = CMD_WREN;
  spiTransmit(buffer, 1);
  //! Установка статусного регистра - разрешить запись в любую область
  buffer[0] = CMD_WRSR;
  buffer[1] = 0x00;
  spiTransmit(buffer, 2);

  //! Проверка на чтение данных из Device ID регистра
  buffer[0] = CMD_RDID;
  spiTransmitReceive(&buffer[0], &buffer[0], 1, 9);
  if ((buffer[0] != 0x7F) || (buffer[1] != 0x7F) || (buffer[5] != 0x7F) ||
      (buffer[6] != 0xC2) || (buffer[7] != 0x24) || (buffer[8] != 0x00))
    asm("nop"); // Ошибка
}

StatusType framWriteData(uint32_t address, uint8_t *data, uint32_t size)
{  
  StatusType status = StatusError;
  uint8_t buffer[4];

  if (address > FRAM_END)
    return status;

  buffer[0] = CMD_WREN;
  spiTransmit(buffer, 1);

  buffer[0] = CMD_WRITE;
  buffer[1] = address>>16;
  buffer[2] = address>>8;
  buffer[3] = address&0xFF;

  clrPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  if (HAL_SPI_Transmit(&hspi3, &buffer[0], 4, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_Transmit(&hspi3, data, size, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  return status;
}

StatusType framReadData(uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = StatusError;
  uint8_t buffer[5];

  if (address > FRAM_END)
    return status;

  buffer[0] = CMD_FSTRD;
  buffer[1] = address>>16;
  buffer[2] = address>>8;
  buffer[3] = address&0xFF;
  buffer[4] = 0xFF;

  clrPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  if (HAL_SPI_Transmit(&hspi3, &buffer[0], 5, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&hspi3, data, data, size, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  return status;
}

StatusType spiTransmitReceive(uint8_t *txData, uint8_t *rxData,
                             uint16_t txSize, uint16_t rxSize)
{
  StatusType status = StatusError;
  clrPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  if (HAL_SPI_Transmit(&hspi3, txData, txSize, 1000) == HAL_OK)
    status = StatusOk;
  if (status == StatusOk) {
    if (HAL_SPI_TransmitReceive(&hspi3, rxData, rxData, rxSize, 1000) == HAL_OK)
      status = StatusOk;
  }
  setPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  return status;
}

StatusType spiTransmit(uint8_t *data, uint16_t size)
{
  StatusType status = StatusError;
  clrPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  if (HAL_SPI_Transmit(&hspi3, data, size, 1000) == HAL_OK)
    status = StatusOk;
  setPinOut(FRAM_NSS_PORT, FRAM_NSS_PIN);
  return status;
}

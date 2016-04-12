#ifndef HOST_H
#define HOST_H

#include "board.h"

#define HOST_BUF_SIZE	4096+25

typedef struct {
  uint32_t txGood;
  uint32_t rxGood;
  uint32_t ovrFlagError;
  uint32_t crcError;
  uint32_t sizeError;
  uint32_t timeoutError;
  uint32_t txError;
} StatHostDef;

extern StatHostDef statHost;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief Функция обработки прерывания по приёму SPI4

*/
void hostRxIRQHandler(void);

#ifdef __cplusplus
}
#endif

/*!
 \brief Инициализация драйвера работы с контроллером визуализации (SPI4-Slave)

*/
void hostInit();

/*!
 \brief Создание семафора драйвера работы с контроллером визуализации
 Семафор необходим для сообщения задачи о получении пакета данных
 \return osSemaphoreId - id созданного семафора
*/
osSemaphoreId getHostSemaphore();

/*!
 * \brief Включение драйвера работы с контроллером визуализации
 */
void hostEnable();

/*!
 \brief Чтение данных из буфера приёма от контроллера визуализации

 \param data - указатель на буфер
 \return int - количество принятых байт
*/
int hostReadData(uint8_t *data);

/*!
 \brief Отправка данных контроллеру визуализации

 \param data  - указатель на буфер
 \param size - количество отправляемых байт
 \param timeout - таймаут отправки (по умолчанию: 1000 мс)
 \return StatusType - ошибка или ок
*/
StatusType hostWriteData(uint8_t *data, int size, uint32_t timeout = 1000);

#endif // HOST_H

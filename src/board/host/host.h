#ifndef HOST_H
#define HOST_H

#include "board.h"

#define HOST_BUF_SIZE	256

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

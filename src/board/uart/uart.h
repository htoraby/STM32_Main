#ifndef UART_H
#define UART_H

#include "board.h"

#define UART_BUF_SIZE	1024

typedef enum {
  uart1,
  uart2,
  uart3,
  uart4,
  uart6,
  uart7,
  uartMax,
} uartNum;

#define HOST_UART_TEST uart3

typedef struct {
  UART_HandleTypeDef uart;
  osSemaphoreId semaphoreId;
  uint8_t txBuffer[UART_BUF_SIZE];
  uint8_t rxBuffer[UART_BUF_SIZE];
} UART_Def;

extern UART_Def uarts[];

#ifdef __cplusplus

/*!
 \brief Инициализация UART

 \param num      - номер порта (@ref uartNum)
 \param baudRate - скорость (115200)
 \param parity   - чётность ()
 \param stopBits - стоп бит (UART_STOPBITS_1)
 \return StatusType - ошибка или ок
*/
StatusType uartInit(uartNum num, uint32_t baudRate, uint32_t parity = UART_PARITY_NONE,
                    uint32_t stopBits = UART_STOPBITS_1);


/*!
 \brief Закрытие порта (запрет прерываний)

 \param num - номер порта (@ref uartNum)
 \return HAL_StatusTypeDef: HAL_OK - удачно
*/
HAL_StatusTypeDef uartClose(uartNum num);

/*!
 \brief Получение конкретного семафора по номеру UART.
 Семафор необходим для сообщения задачи о получении байта данных
 \param num         - номер порта (@ref uartNum)
 \return osSemaphoreId - id созданного семафора
*/
osSemaphoreId uartGetSemaphoreId(uartNum num);

/*!
 \brief Получение номера порта UART по указателю

 \param huart    - Указатель на порт UART
 \return uartNum - номер порта
*/
uartNum uartGetNum(UART_HandleTypeDef *huart);

/*!
 \brief Получение количества принятых байт

 \param num  - номер порта (@ref uartNum)
 \return int - количество принятых байт
*/
int uartGetRxCount(uartNum num);

/*!
 \brief Чтение данных из буфера UART

 \param num  - номер порта (@ref uartNum)
 \param data - указатель на буфер
 \return int - количество принятых байт
*/
int uartReadData(uartNum num, uint8_t *data);

/*!
 \brief Отправка данных в порт UART

 \param num   - номер порта (@ref uartNum)
 \param data  - указатель на буфер
 \param count - количество отправляемых байт
 \return StatusType - ошибка или ок
*/
StatusType uartWriteData(uartNum num, uint8_t *data, int count,
                         uint32_t timeout = 100);

#endif

#endif // UART_H

#ifndef UART_H
#define UART_H

#include "board.h"

#define UART_BUF_SIZE	256

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
 \return int : -1 - неверный порт, 0 - ok, >0 - ошибки
*/
int uart_init(uartNum num, uint32_t baudRate, uint32_t parity = UART_PARITY_NONE,
             uint32_t stopBits = UART_STOPBITS_1);


/*!
 \brief Закрытие порта (запрет прерываний)

 \param num - номер порта (@ref uartNum)
 \return HAL_StatusTypeDef : HAL_OK - удачно
*/
HAL_StatusTypeDef uart_close(uartNum num);

/*!
 \brief Привязка конкретного семафора к номеру UART.
 Семафор необходим для сообщения задачи о получении байта данных
 \param num         - номер порта (@ref uartNum)
 \param semaphoreId - id семафора
*/
void uart_setSemaphoreId(uartNum num, osSemaphoreId semaphoreId);

/*!
 \brief Получение номера порта UART по указателю

 \param huart    - Указатель на порт UART
 \return uartNum - номер порта
*/
uartNum uart_getNum(UART_HandleTypeDef *huart);

/*!
 \brief Получение количества принятых байт

 \param num  - номер порта (@ref uartNum)
 \return int - количество принятых байт
*/
int uart_getRxCount(uartNum num);

/*!
 \brief Чтение данных из буфера UART

 \param num  - номер порта (@ref uartNum)
 \param data - указатель на буфер
 \return int - количество принятых байт
*/
int uart_readData(uartNum num, uint8_t *data);

/*!
 \brief Отправка данных в порт UART

 \param num   - номер порта (@ref uartNum)
 \param data  - указатель на буфер
 \param count - количество отправляемых байт
 \return HAL_StatusTypeDef
*/
HAL_StatusTypeDef uart_writeData(uartNum num, uint8_t *data, int count,
                                 uint32_t timeout = 100);

#endif

#endif // UART_H

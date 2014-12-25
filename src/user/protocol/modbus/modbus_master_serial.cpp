/*
 * modbus_master_serial.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master_serial.h"

ModbusMasterSerial::ModbusMasterSerial(int Com)
{
	// TODO Auto-generated constructor stub
  numberComPort_ = Com;
}

ModbusMasterSerial::~ModbusMasterSerial()
{
	// TODO Auto-generated destructor stub
}

// МЕТОД ОТКРЫТИЯ ПРОТОКОЛА
// Открывает протокол и связанный и с ним порт
// Параметры:
// PortName - Идентификатор Последовательного порта (например, "COM1", "/ dev/ser1" или "/ dev/ttyS0")
// BaudRate - Скорость передачи данных
// DataBits - 7 бит данных для протокол ASCII, 8 бит данных для RTU
// StopBits - стоп-бит
// Parity - контроль четности
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMasterSerial::openProtocol( int PortName,
                    						long BaudRate,
                    						int DataBits,
                    						int StopBits,
                    						int Parity)
{
	int Result = RETURN_ERROR;
	try	{
		// Вызываем функцию UART
    uart_init((uartNum)PortName, BaudRate, Parity, StopBits);
		// Возвращаем что операция выполнена
		Result = RETURN_OK;
    // Семафор для ожидания ответов по Modbus
    osSemaphoreId semaphoreAnswer_ = osSemaphoreCreate(NULL, 1);
    // Передаём имя семафора
    transmissionSemaphore(semaphoreAnswer_);
    osSemaphoreWait(semaphoreAnswer_, 0);
	}
	catch(...)
	{
		// Формируем сообщение упали в функции openProtocol
	}
	return Result;
};

// МЕТОД ЗАКРЫТИЯ ПРОТОКОЛА
// Закрывает последовательного порта и освобождает все системные ресурсы,
// связанные с портом.
int ModbusMasterSerial::closeProtocol(int PortName)
{
	int Result = RETURN_ERROR;
	try
	{
		// Вызываем функцию UART
    uart_close((uartNum)PortName);
		Result = RETURN_OK;
	}
	catch(...)
	{
		// Формируем сообщение упали в функции closeProtocol
	}
	return Result;
}

// МЕТОД ПОСЫЛКИ ДАННЫХ ИЗ БУФЕРА В COM ПОРТ
int ModbusMasterSerial::transmitQuery(unsigned char *Buf, int Count)
{
  uart_writeData((uartNum)numberComPort_, txBuffer_, Count);
  return 1;
}

// Метод получения данных из uart порта
int ModbusMasterSerial::reseiveAnswer(unsigned char *Buf)
{
  // Если истек таймаут ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, MODBUS_ANSWER_TIMEOUT) == osEventTimeout) {
    // Возвращаем ошибку что нет ответа от устройства
    return 0;
  }
  // Получили первый байт
  else {
    // Крутимся пока время между байтами не станет больше MODBUS_TIME_END_PACKAGE
    while (1) {
      if (osSemaphoreWait(semaphoreAnswer_, MODBUS_TIME_END_PACKAGE) == osEventTimeout) {
        // Получаем количество полученных байт и массив байт
        return uart_readData((uartNum)numberComPort_, Buf);
      }
    }
  }
}

void ModbusMasterSerial::transmissionSemaphore(osSemaphoreId semaphoreId)
{
  uart_setSemaphoreId((uartNum)numberComPort_, semaphoreId);
};


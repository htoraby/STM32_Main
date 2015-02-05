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

int ModbusMasterSerial::openProtocol(int portName,
                                     long baudRate,
                                     int dataBits,
                                     int stopBits,
                                     int parity)
{
	int Result = RETURN_ERROR;
	try	{
    // Вызываем функцию инициализации UART
    uartInit((uartNum)portName, baudRate, parity, stopBits);
    // Семафор для ожидания ответов по Modbus
    semaphoreAnswer_ = uartGetSemaphoreId((uartNum)portName);
		// Возвращаем что операция выполнена
		Result = RETURN_OK;
	}
	catch(...)
	{
		// Формируем сообщение упали в функции openProtocol
	}
	return Result;
}

int ModbusMasterSerial::closeProtocol(int PortName)
{
	int Result = RETURN_ERROR;
	try
	{
		// Вызываем функцию UART
    uartClose((uartNum)PortName);
		Result = RETURN_OK;
	}
	catch(...)
	{
		// Формируем сообщение упали в функции closeProtocol
	}
	return Result;
}

int ModbusMasterSerial::transmitQuery(unsigned char *Buf, int Count)
{
  uartWriteData((uartNum)numberComPort_, txBuffer_, Count);
  return 1;
}

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
        return uartReadData((uartNum)numberComPort_, Buf);
      }
    }
  }
}

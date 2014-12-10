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
	NumberComPort = Com;
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
int ModbusMasterSerial::transmitQuery(	unsigned char *Buf, int Count)
{
	int Result = RETURN_ERROR;
	try
	{
    uart_writeData((uartNum)NumberComPort, Buffer, Count);
		Result = RETURN_OK;
	}
	catch(...)
	{

	}
	return Result;
}

// МЕТОД ЧТЕНИЕ ДАННЫХ ИЗ ПОРТА
// Buf - массив байт считываемый из порта
// Count - ожидаемое количество байт для считывания
int ModbusMasterSerial::reseiveAnswer(	unsigned char *Buf,
											unsigned char Count)
{
	int Result = RETURN_ERROR;
	//
	int CurrentCountByte = 0;
	static int PrevCountByte = 0;
	static int TimerByte = 0;
	static int TimerAnswer = 0;
	try
	{

		// Бесконечный цикл
		while(1)
		{
			// Узнаём количество байт в буфере приёма
      //CurrentCountByte = Com_GetReadCount(NumberComPort);
			// 1-e условие выхода из цикла, получена предполагаемая длина пакета
			if(CurrentCountByte == Count)
			{
				break;
			}
			// 2-e условие выхода из цикла, пауза между байтами в ответе
			if(CurrentCountByte != 0)
			{
				// Если текущее количество байт не равно предыдущему
				if(CurrentCountByte != PrevCountByte)
				{
					TimerByte = 0;
				}
				else
				{
					TimerByte++;
					if(TimerByte >= MODBUS_TIME_END_PACKAGE)
					{
						break;
					}
				}
				PrevCountByte = CurrentCountByte;
			}
			// 3-е условие выхода из цикла
			if( TimerAnswer > getTimeout())
			{
				Result = RETURN_MODBUS_TIMEOUT;
				break;
			}
		}
		// Выпали из цикла, считываем данные из буфера
		// Если получен пакет минимальной длины
		// Вероятно пакет с ошибкой
		if(CurrentCountByte == MODBUS_MIN_LENGHT_PACKAGE)
		{
      //Com_ReadData(NumberComPort, Buffer, CurrentCountByte);
			// Проверяем контрольную сумму
			if(((Buffer[CurrentCountByte - 1] << 8) + Buffer[CurrentCountByte - 2]) == calcCRC16((CurrentCountByte-2), Buffer))
			{
				Result = RETURN_MODBUS_ERROR;
			}
			else
			{
				Result = RETURN_MODBUS_ERROR_CRC;
			}
		}
		else
		{
			if(CurrentCountByte == Count)
			{
        //Com_ReadData(NumberComPort, Buffer, CurrentCountByte);
				// Проверяем контрольную сумму
				if(((Buffer[CurrentCountByte - 1] << 8) + Buffer[CurrentCountByte - 2]) == calcCRC16((CurrentCountByte-2), Buffer))
				{
					Result = RETURN_MODBUS_OK;
				}
				else
				{
					Result = RETURN_MODBUS_ERROR_CRC;
				}
			}
		}
	}
	catch(...)
	{

	}
	return Result;
}


/*
 * modbus_master_serial.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_MASTER_SERIAL_H_
#define MODBUS_MASTER_SERIAL_H_

#include "modbus_master.h"
#include "uart.h"

// КЛАСС MODBUS MASTER ДЛЯ ПОСЛЕДОВАТЕЛЬНЫХ ПОРТОВ
// Реализует некоторые настройки последовательного протокола
class ModbusMasterSerial: public ModbusMaster
{
	public:

    ModbusMasterSerial(int Com);
    virtual ~ModbusMasterSerial();

		// МЕТОД ЗАКРЫТИЯ ПРОТОКОЛА
		// Закрывает последовательного порта и освобождает все системные ресурсы,
		// связанные с портом.
		int closeProtocol(int PortName);

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
		int openProtocol(	int PortName,
		                    long BaudRate,
		                    int DataBits,
		                    int StopBits,
		                    int Parity);

		// МЕТОД ПРОВЕРКИ ОТКРЫТ ЛИ ПРОТОКОЛ
		// Наследуется из classModbusMaster
		// Возвращает:
		// 1 - открыт
		// 0 - закрыт
		int isOpen();

		// МЕТОД ЗАПИСИ ДАННЫХ В ПОРТ
		// Buf - массив байт записываемый в порт
		// Count - количество записываемых байт
		int transmitQuery(	unsigned char *Buf,
              int Count);

		// МЕТОД ЧТЕНИЕ ДАННЫХ ИЗ ПОРТА
		// Buf - массив байт считываемый из порта
		// Count - ожидаемое количество байт для считывания
		int reseiveAnswer(	unsigned char *Buf,
							unsigned char Count);

	private:
		int NumberComPort;
};

#endif /* MODBUS_MASTER_SERIAL_H_ */

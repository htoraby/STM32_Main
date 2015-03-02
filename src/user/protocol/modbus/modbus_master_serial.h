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

/*!
 * \brief Класс modbus master для последовательных портов
 *  Реализует некоторые настройки последовательного протокола
 */
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
    int openProtocol(int portName, long baudRate, int dataBits,
                     int stopBits, int parity);

		// МЕТОД ПРОВЕРКИ ОТКРЫТ ЛИ ПРОТОКОЛ
		// Наследуется из classModbusMaster
		// Возвращает:
		// 1 - открыт
		// 0 - закрыт
		int isOpen();

		// МЕТОД ЗАПИСИ ДАННЫХ В ПОРТ
		// Buf - массив байт записываемый в порт
		// Count - количество записываемых байт
    int transmitQuery(unsigned char *Buf, int Count);


    /*!
     * \brief receiveAnswer Метод получения данных из uart порта
     * \param Buf Массив принятых данных байт
     * \return Количество принятых байт
     */
    int receiveAnswer(unsigned char *Buf);

	private:
    int numberComPort_;

    // Семафор
    osSemaphoreId semaphoreAnswer_;
};

#endif /* MODBUS_MASTER_SERIAL_H_ */

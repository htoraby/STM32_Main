﻿/*
 * modbus_master.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_MASTER_H_
#define MODBUS_MASTER_H_

#include "define.h"
#include "stdlib.h"

#define MODBUS_OK                             0
#define MODBUS_ERROR_TIMEOUT                  1
#define MODBUS_ERROR_CRC                      2
#define MODBUS_ERROR_TRASH                    3
// Максимальное время ожидания ответа от устройства в мс
#define MODBUS_ANSWER_TIMEOUT                 500
// Максимальное время между байтами в ответе в мс
#define MODBUS_TIME_END_PACKAGE               5
// Минимальная возможная длина "корректного" ответа по Modbus
#define MODBUS_MIN_LENGHT_PACKAGE             5
// Длина modbus запроса по спецификации
#define MODBUS_PDU_SIZE                       253
#define MODBUS_MAX_DATA_SIZE                  MODBUS_PDU_SIZE - 1
#define MODBUS_MAX_FC03_WORDS                 (MODBUS_MAX_DATA_SIZE - 1) / 2
#define MODBUS_MAX_FC01_COILS                 MODBUS_MAX_FC03_WORDS * 16
#define MODBUS_MAX_FC16_WORDS                 (MODBUS_MAX_DATA_SIZE - 5) / 2
#define MODBUS_MAX_FC15_COILS                 MODBUS_MAX_FC16_WORDS * 16
#define MODBUS_READ_COILS_0x01                1
#define MODBUS_READ_DISCRETE_INPUTS_0x02      2
#define MODBUS_READ_HOLDING_REGISTERS_0x03    3
#define MODBUS_READ_INPUT_REGISTERS_0x04      4
#define MODBUS_WRITE_SINGLE_COIL_0x05         5
#define MODBUS_WRITE_SINGLE_REGISTER_0x06     6
#define MODBUS_READ_EXCEPTION_STATUS_0x07     7
#define MODBUS_DIAGNOSTICS_0x08               8
#define MODBUS_WRITE_MULTIPLE_COILS_0x0F      15
#define MODBUS_WRITE_MULTIPLE_REGISTERS_0x10  16
#define MODBUS_ERROR_0x80                     128
#define MODBUS_ILLEGAL_FUNCTION_0x01          1
#define MODBUS_ILLEGAL_DATA_ADDRESS_0x02      2
#define MODBUS_ILLEGAL_DATA_VALUE_0x03        3
#define MODBUS_SLAVE_DEVICE_FAILURE_0x04      4
#define MODBUS_ACKNOWLEDGE_0x05               5
#define MODBUS_SLAVE_DEVICE_BUSY_0x06         6
#define MODBUS_MEMORY_PARITY_ERROR_0x08       8
#define MODBUS_GATEWAY_PATH_UNAVAILABLE_0x0A  10
#define MODBUS_GATEWAY_TARGET_DEVICE_0x0B     11


// БАЗОВЫЙ КЛАСС MODBUS MASTER
// Релизует функции Modbus и некоторые другие функции
// Методы этого класс применяются ко всем классам типов обмена через наследование
class ModbusMaster
{
	public:
    ModbusMaster();
    virtual ~ModbusMaster();

		///////////////////////////////////////////////////////////////////////
		// МЕТОДЫ РЕАЛИЗУЮЩИЙ ФУНКЦИИ ПРОТОКОЛА MODBUS
		///////////////////////////////////////////////////////////////////////

		// ПОБИТОВЫЕ МЕТОДЫ ///////////////////////////////////////////////////

		// МЕТОД ЧТЕНИЯ СОСТОЯНИЯ КАТУШЕК
		// Modbus функция 1, Считывание состояния катушки
		// Читает содержимое катушек
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Первая катушка
		// RefCnt Количество считываемых катушек (Диапазон: 1-2000)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readCoils(				int SlaveAddr,
									int StartRef,
									bool BitArr[],
									int RefCnt);

		// МЕТОД ЧТЕНИЯ СОСТОЯНИЯ ДИСКРЕТНЫХ ВХОДОВ
		// Modbus функция 2, Чтение дискретных входов
		// Считывает содержимое дискретных входов
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Первый вход
		// BitArr буфера, который будет содержать считывать данные
		// RefCnt Количество считываемых входов (Диапазон: 1-2000)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readInputDiscretes(		int SlaveAddr,
									int StartRef,
									bool BitArr[],
									int RefCnt);

		// МЕТОД ЗАПИСИ КАТУШКИ
		// Modbus функция 5, Записать катушку.
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (Диапазон: 0 - 255)
		// BitAddr катушки адрес
		// BitVal записываемое состояние
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int writeCoil(				int SlaveAddr,
									int BitAddr,
									int BitVal);

		// МЕТОД ЗАПИСИ КАТУШЕК
		// Modbus функция 15 (0F Hex), Воздействие на несколько катушек
		// Записывает двоичные значения в последовательность дискретных выходов или катушек
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Первая ссылка
		// BitArr буфера, который содержит отправляемые данные
		// RefCnt Количество катушек, которые будут записаны (Диапазон: 1-1968)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int forceMultipleCoils(		int SlaveAddr,
									int StartRef,
									const bool BitArr[],
									int RefCnt);

		// МЕТОДЫ ДЛЯ 16-ТИ БИТОВЫХ РЕГИСТРОВ /////////////////////////////////

		// МЕТОД ЧТЕНИЯ РЕГИСТРОВ УСТРОЙТВА
		// Modbus функция 3,  Читать нескольких регистров.
		// Читает содержимое регистров
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Начните регистр (диапазон: 1 - 0x10000)
		// RegArr буфера, который будет наполнен прочитанныйми данными
		// RefCnt Количество считываемых регистров
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
    int readMultipleRegisters(int slaveAddr,
                                int startRef,
                                short *regArr,
                                int refCnt);

		// МЕТОД ЧТЕНИЯ ВНУТРЕННИХ РЕГИСТРОВ УСТРОЙСТВА
		// Modbus функция 4, Чтение входных регистров.
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Начните регистр
		// RegArr буфера, который будет наполнен данными читать.
		// RefCnt Количество считываемых регистров (Диапазон: 1-125)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readInputRegisters(		int SlaveAddr,
									int StartRef,
									short RegArr[],
									int RefCnt);

		// МЕТОД ЗАПИСИ РЕГИСТРА
		// Modbus функция 6, Запись одного регистра.
		// Записывает значение в один регистр выходной
		// Параметры:
		// SlaveAddr - Modbus Адрес ведомого устройства (Диапазон: 0 - 255)
		// RegAddr - Адрес регистра (диапазон: 1 - 0x10000)
		// Regval - данные для отправки
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int writeSingleRegister(	int SlaveAddr,
									int RegAddr,
                  short RegVal);

		// МЕТОД ЗАПИСИ НЕСКОЛЬКИХ РЕГИСТРОВ
		// Modbus функция 16 (10 Hex) Запись нескольких регистров.
		// Записывает значения в последовательность регистров
		// Параметры:
		// SlaveAddr - Modbus Адрес ведомого устройства
		// StartRef - Первый регистр (диапазон: 1 - 0x10000)
		// RegArr - буфера с данными, которые будут отправлены.
		// RefCnt - Количество регистров, которые будут записаны (Диапазон: 1-123)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int writeMultipleRegisters(	int SlaveAddr,
		                            int StartRef,
                                short *RegArr,
		                            int RefCnt);

		// МЕТОДЫ РАБОТЫ С 32-УХ БИТОВЫМИ РЕГИСТРАМИ //////////////////////////

		// МЕТОД ЧТЕНИЯ РЕГИСТРОВ УСТРОЙТВА
		// Функция MModbus (03 Hex) для 32-разрядных типов данных Int,
		// Пишет длинные десятичного значения в пар выходных регистров
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
		// StartRef Первый регистр
		// Int32Arr буфера с данными для отправки
		// RefCnt Количество длинных целых чисел, которые будут отправлены (Диапазон: 1-50)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readMultipleLongInts(	int SlaveAddr,
		                            int StartRef,
                                int *Int32Arr,
		                            int RefCnt);

		// МЕТОД ЧТЕНИЯ ВНУТРЕННИХ РЕГИСТРОВ УСТРОЙСТВА
		// Modbus функция 4 для 32-битных типов данных Int
		// Считывает содержимое пар последовательных регистров
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы
		// StartRef Первый регистр (диапазон: 1 - 0x10000)
		// Int32Arr буфера, который будет наполнен чтения данных
		// RefCnt Количество длинных целых чисел, которые будут читать (диапазон: 1-62)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readInputLongInts(		int SlaveAddr,
		                         	int StartRef,
		                         	int Int32Arr[],
		                         	int RefCnt);
		// МЕТОД ЗАПИСИ РЕГИСТРОВ
		// Modbus функция 16 (10 Hex) для 32-разрядных типов данных Int, Запись нескольких регистров с
		// Пишет длинные десятичного значения в пары регистров
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (Диапазон: 0 - 255)
		// StartRef Первый регистр
		// Int32Arr буфера с данными для отправки
		// RefCnt Количество длинных целых чисел, которые будут отправлены (Диапазон: 1-50)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int writeMultipleLongInts(	int SlaveAddr,
		                            int StartRef,
                                int *Int32Arr,
		                            int RefCnt);

		// МЕТОД ЧТЕНИЯ FLOAT РЕГИСТРОВ
		// Modbus функция 3 для 32-битных типов данных с плавающей точкой
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Первый регистр
		// Float32Arr буфер, который будет наполнен прочитаными данными
		// RefCnt Количество поплавков значений, которые будут читать (диапазон: 1-62)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readMultipleFloats(		int SlaveAddr,
		                          	int StartRef,
		                          	float Float32Arr[],
		                          	int RefCnt);

		// МЕТОД ЧТЕНИЯ ВНУТРЕННИХ РЕГИСТРОВ
		// Modbus функция 4 для 32-битных типов данных с плавающей точкойй.
		// Считывает содержимое пар последовательных регистров ввода
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
		// StartRef Первая ссылка
		// Float32Arr буфера, который будет наполнен прочитанными данных
		// RefCnt Количество регистров (диапазон: 1-62)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readInputFloats(		int SlaveAddr,
		                       	   	int StartRef,
		                       	   	float Float32Arr[],
		                       	   	int RefCnt);

		// МЕТОД ЗАПИСИ FLOAT РЕГИСТРОВ
		// Modbus функция 16 (10 Hex) для 32-разрядных типов данных с плавающей точкой, запись нескольких регистров данных с плавающей точкой.
		// Записывает значения с плавающей точкой на пары регистров выходных
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
		// StartRef Первая ссылка
		// Float32Arr буфера с данными для отправки
		// RefCnt Количество значений с плавающей точкой, которые будут отправлены (Диапазон: 1-50)
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int writeMultipleFloats(	int SlaveAddr,
		                           	int StartRef,
                                float *Float32Arr,
		                           	int RefCnt);

		///////////////////////////////////////////////////////////////////////
		// МЕТОДЫ ДИАГНОСТИКИ ПРОТОКОЛА
		///////////////////////////////////////////////////////////////////////

		// МЕТОД ЧТЕНИЯ ИСКЛЮЧЕНИЯ
		// Modbus функция 7 чтение cтатуса исключений
		// Считывание состояния катушки исключение внутри ведомого устройства.
		// Параметры:
		// slaveAddr Modbus Адрес ведомого устройства
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int readExceptionStatus(	int SlaveAddr,
									unsigned char *StatusBytePtr);

		// МЕТОД ЭХО ЗАПРОСА
		// Функция Modbus код 8, подфункция 00, Вернуться Запрос данных.
		// Параметры:
		// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы (диапазон: 1 - 255)
		// QueryArr буфер с данными для отправки
		// EchoArr буфера, который будет содержать считывать данные
		// Len Количество байт Написать отправлено и читать назад
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int returnQueryData(		int SlaveAddr,
		                       	   	const unsigned char QueryArr[],
		                       	   	unsigned char EchoArr[],
		                       	   	int Len);

		// МЕТОД СБРОСА НАСТРОЕКТ ОПРОСА
		// Функция Modbus код 8, подфункция 01, сброс настроек
		// Параметры:
		// slaveAddr Modbus Адрес ведомого устройства  (диапазон: 1 - 255)
		// clearEventLog Флаг
		// Возвращает:
		// RETURN_OK в случае успешного завершения или код ошибки
		int restartCommunicationsOption( int SlaveAddr,
		                                 int ClearEventLog);

		///////////////////////////////////////////////////////////////////////
		// МЕТОДЫ КОНФИГУРИРОВАНИЯ ПРОТОКОЛА
		///////////////////////////////////////////////////////////////////////

		// МЕТОД НАСТРОЙКИ ВРЕМЕНИ ОЖИДАНИЯ ОТВЕТА
		// Настраивает тайм-аут
		// Протокол должен быть закрыт
		// Параметры:
		// TimeOut значение  в мс (диапазон: 1 - 100000)
		// Возвращает:
		// RETURN_OK в случае успешного завершения
		// Ошибку вне диапазона
		// Ошибку протокол уже открыт
		int setTimeout(int Time);

		// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ТАЙМАУТА ОТВЕТА
		// Возвращает значение таймаута
		int getTimeout();

		// МЕТОД ЗАДАНИЯ ЗАДЕРЖКИ МЕЖДУ ЗАПРОСАМИ
		// Настраивает задержку между запросами,
		// Протокол должен быть закрыт
		// Параметры:
		// PollDelay значение  в мс (диапазон: 1 - 100000)
		// Возвращает:
		// RETURN_OK в случае успешного завершения
		// Ошибку вне диапазона
		// Ошибку протокол уже открыт
		int setPollDelay(int Delay);

		// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ЗАДЕРЖКИ МЕЖДУ ЗАПРОСАМИ
		// Возвращает значение задержки между опросами
		int getPollDelay();

		// МЕТОД УСТАНОВКИ ПОЛИЧЕСТВА ПОВТОРОВ ЗАПРОСОВ
		// Настраивает количество автоматических повторов запросов
		// Параметры:
		// RetryCnt	(0 - 10)
		// RETURN_OK в случае успешного завершения
		// Ошибку вне диапазона
		// Ошибку протокол уже открыт
		int setRetryCnt(int Retry);

		// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ АВТОМАТИЧЕСКИХ ПОВТОРОВ ЗАПРОСОВ
		// Возвращает количество автоматических запросов
		int getRetryCnt();

		///////////////////////////////////////////////////////////////////////
		// МЕТОДЫ СТАТИСТИЧЕСКОЙ ИНФОРМАЦИИ ОБ ОБМЕНЕ
		///////////////////////////////////////////////////////////////////////

		// МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВА ЗАПРОСОВ
		// Вовзращает значение счётчика общего количества запросов
		long getTotalCounter();

		// МЕТОД СБРОСА СЧЁТЧИКА ОБЩЕГО КОЛИЧЕСТВА ЗАПРОСОВ
		// Сбрасывает значение счётчика общего количества запросов в 0
		void resetTotalCounter();

		// МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВ ОТВЕТОВ НА ЗАПРОСЫ
		// Вовзращает значение счётчика корректных ответов на запросы
		long getSuccessCounter();

		// МЕТОД СБРОСА СЧЁТЧИКА ОТВЕТОВ НА ЗАПРОСЫ
		//Сбрасывает значение счётчика корректных ответов в 0
		void resetSuccessCounter();

		///////////////////////////////////////////////////////////////////////
		// МЕТОДЫ НАСТРОЙКИ ПОД SLAVE
		///////////////////////////////////////////////////////////////////////

		// МЕТОД КОНФИГУРИРОВАНИЯ BIG ENDIAN
		// По умолчанию Modbus использует little-endian порядок передачи байт
		// сначала младший потом старший, функция настраивает что другой порядок
		// сначала старший, потом младший
		void configureBigEndianInts();

		// МЕТОД КОНФИГУРИРОВАНИЯ LITTLE ENDIAN
		// По умолчанию Modbus использует little-endian порядок передачи байт
		// сначала младший потом старший
		void configureLittleEndianInts();

		void closeProtocol();

		// МЕТОД ПРОВЕРКИ ОТКРЫТИЯ ПРОТОКОЛА
		int isOpen();

		// МЕТОД ПОСЫЛКИ ДАННЫХ
		int transmitQuery(	unsigned char *Buf,
							unsigned char Count);

		// МЕТОД ЧТЕНИЕ ДАННЫХ
		// Buf - массив байт считываемый из порта
    // Byte - ожидаемое количество байт для считывания
    int reseiveAnswer(unsigned char *Buf);

	///////////////////////////////////////////////////////////////////////////
	// ЗАЩИЩЕННЫЕ ЧЛЕНЫ КЛАССА
	///////////////////////////////////////////////////////////////////////////
	protected:

		// Общий счётчик запросов
		unsigned long TotalCounter;
		// Общий счётчик корректных ответов
		unsigned long SuccessCounter;
		// Число автоматических повторений запроса
		int RetryCnt;
		// Время ожидания ответа
		int TimeOut;
		// Время между запросами
		int PollDelay;
		// Порядок байт
		int Endian;
		// Текущий таймер ожидания ответа
		int Timer;
    // Количество байт в ответе
    int countBytesAnswer_;


		// МЕТОД ПРОВЕРКИ КОРРЕКТНОСТИ АДРЕСА УСТРОЙТВА
		bool checkDeviceAddress(int Address);

		// МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА КАТУШЕК
		bool checkCntCoils(int Cnt);

		// МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА РЕГИСТРОВ
		bool checkCntReg(int Cnt);

		// МЕТОД ВЫЧИСЛЕНИЯ КОНТРОЛЬНОЙ СУММЫ
		unsigned short calcCRC16(char Size, unsigned char* Buf);

    unsigned char bufferTx_[MODBUS_MAX_DATA_SIZE];
    unsigned char bufferRx_[MODBUS_MAX_DATA_SIZE];

	///////////////////////////////////////////////////////////////////////////
	// ЗАКРЫТЫЕ ЧЛЕНЫ КЛАССА
	///////////////////////////////////////////////////////////////////////////
	private:

};

#endif /* MODBUS_MASTER_H_ */
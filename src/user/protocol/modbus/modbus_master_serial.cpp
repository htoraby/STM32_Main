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
  uartClose((uartNum)PortName);
  resetTotalCounter();
  resetSuccessCounter();
  Result = RETURN_OK;
  return Result;
}

int ModbusMasterSerial::transmitQuery(unsigned char *Buf, int Count)
{
  int result = RETURN_ERROR;
  uartWriteData((uartNum)numberComPort_, txBuffer_, Count);
  incTotalCounter();
  result = RETURN_OK;
  return result;
}

int ModbusMasterSerial::receiveAnswer(uint8_t *Buf, uint8_t count)
{
  // Если истек таймаут ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, MODBUS_ANSWER_TIMEOUT) == osEventTimeout) {
    incLostCounter();
    // Возвращаем ошибку что нет ответа от устройства
    return MODBUS_ERROR_TIMEOUT;
  }
  // Получили первый байт
  else {
    // Крутимся пока время между байтами не станет больше MODBUS_TIME_END_PACKAGE
    while (1) {
      if (osSemaphoreWait(semaphoreAnswer_, MODBUS_TIME_END_PACKAGE) == osEventTimeout) {
        // Получаем принятое количество байт
        int rxCnt = uartReadData((uartNum)numberComPort_, Buf);
        // Условия корректного ответа с данными
        if ((rxCnt == count) &&                       // Получили ожидаемое количество байт
            (rxBuffer_[0] == txBuffer_[0]) &&         // Ответ от нужного устройства
            (rxBuffer_[1] == txBuffer_[1]) &&         // Ответ на нужную команду
            (rxBuffer_[2] == rxCnt - MODBUS_MIN_LENGHT_PACKAGE)) {        // Нужное количество данных
          if (((rxBuffer_[rxCnt - 1] << 8) + rxBuffer_[rxCnt - 2]) == crc16_ibm(rxBuffer_, (rxCnt - 2))) {
            incSuccessCounter();
            resetLostCounter();
            return MODBUS_OK;
          }
          else {
            return MODBUS_ERROR_CRC;
          }
        }
        // Приняли 5 байт, проверяем на ошибку
        else if ((rxCnt == MODBUS_MIN_LENGHT_PACKAGE) &&        // Получили ожидаемое количество байт
                 (rxBuffer_[0] == txBuffer_[0]) &&              // Ответ от нужного устройства
                 (rxBuffer_[1] == txBuffer_[1] + MODBUS_ERROR_0x80) &&    // Ответ с документированной ошибкой
                 (rxBuffer_[2] >= MODBUS_ILLEGAL_FUNCTION_0x01) &&
                 (rxBuffer_[2] <= MODBUS_GATEWAY_TARGET_DEVICE_0x0B)) {
          return MODBUS_ERROR_0x80 + rxBuffer_[2];
        }
        // Неизвестная ошибка
        else {
          return MODBUS_ERROR_TRASH;
        }
      }
    }
  }
}

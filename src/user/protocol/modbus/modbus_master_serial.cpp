/*
 * modbus_master_serial.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master_serial.h"
#include "user_main.h"


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
  int Result = err_r;
	try	{
    // Вызываем функцию инициализации UART
    uartInit((uartNum)portName, baudRate, parity, stopBits);
    // Семафор для ожидания ответов по Modbus
    semaphoreAnswer_ = uartGetSemaphoreId((uartNum)portName);
		// Возвращаем что операция выполнена
    Result = ok_r;
	}
	catch(...)
	{
		// Формируем сообщение упали в функции openProtocol
	}
	return Result;
}

int ModbusMasterSerial::closeProtocol(int PortName)
{
  int Result = err_r;
  uartClose((uartNum)PortName);
  resetTotalCounter();
  resetSuccessCounter();
  Result = ok_r;
  return Result;
}

uint8_t ModbusMasterSerial::txBuf(uint8_t *buf, uint8_t num)
{
  uint16_t res = err_r;
  if (uartWriteData((uartNum)numberComPort_, txBuffer_, num) == ok_r) {
    incTotalCounter();
    res = ok_r;
  }
  else {
    logDebug.add(WarningMsg, "txQuery");
  }
  return res;
}

uint8_t ModbusMasterSerial::rxBuf(uint8_t *buf, uint8_t num)
{
  uint16_t res = MODBUS_ERROR_TRASH;
  // Если истек таймаут ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, MODBUS_ANSWER_TIMEOUT) == osEventTimeout) {
    incLostCounter();
    res = MODBUS_ERROR_TIMEOUT;                      // Возвращаем ошибку что нет ответа от устройства
  }
  else {                                              // Получили первый байт
    while (1) {                                       // Крутимся пока время между байтами не станет больше MODBUS_TIME_END_PACKAGE
      if (osSemaphoreWait(semaphoreAnswer_, MODBUS_TIME_END_PACKAGE) == osEventTimeout) {       
        int rxNum = uartReadData((uartNum)numberComPort_, buf);
        if ((rxNum == num) &&                         // Получили ожидаемое количество байт
            (rxBuffer_[0] == txBuffer_[0]) &&         // Ответ от нужного устройства
            (rxBuffer_[1] == txBuffer_[1]) &&         // Ответ на нужную команду
            (rxBuffer_[2] == rxNum - MODBUS_MIN_LENGHT_PACKAGE)) {        // Нужное количество данных
          if (((rxBuffer_[rxNum - 1] << 8) + rxBuffer_[rxNum - 2]) == crc16_ibm(rxBuffer_, (rxNum - 2))) {
            incSuccessCounter();
            resetLostCounter();
            res =  MODBUS_OK;
          }
          else {
            res =  MODBUS_ERROR_CRC;
          }
        }
        // Приняли 5 байт, проверяем на ошибку
        else if ((rxNum == MODBUS_MIN_LENGHT_PACKAGE) &&        // Получили ожидаемое количество байт
                 (rxBuffer_[0] == txBuffer_[0]) &&              // Ответ от нужного устройства
                 (rxBuffer_[1] == txBuffer_[1] + MODBUS_ERROR_0x80) &&    // Ответ с документированной ошибкой
                 (rxBuffer_[2] >= MODBUS_ILLEGAL_FUNCTION_0x01) &&
                 (rxBuffer_[2] <= MODBUS_GATEWAY_TARGET_DEVICE_0x0B)) {
          res =  MODBUS_ERROR_0x80 + rxBuffer_[2];
        }
        // Неизвестная ошибка
        else {
          res =  MODBUS_ERROR_TRASH;
        }
      }
    }
  }
  return res;
}

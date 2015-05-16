/*
 * modbus_master_serial.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master_serial.h"
#include "user_main.h"


ModbusMasterSerial::ModbusMasterSerial(int com)
{
  numberComPort_ = com;
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
    resetTotalCounter();
    resetSuccessCounter();
    resetCrcCounter();
    resetErrCounter();
    resetFailCounter();
    resetLostCounter();
    resetTrashCounter();
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
  resetCrcCounter();
  resetErrCounter();
  resetFailCounter();
  resetLostCounter();
  resetTrashCounter();
  Result = ok_r;
  return Result;
}

uint8_t ModbusMasterSerial::txBuf(uint8_t *buf, uint8_t num)
{
  uint16_t res = err_r;
  if (uartWriteData((uartNum)numberComPort_, txBuffer_, num, timeOut_) == ok_r) {
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
  uint32_t callTest = 0;
  // Если истек таймаут ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, timeOut_) == osEventTimeout) {
    incLostCounter();
    return MODBUS_ERROR_TIMEOUT;                      // Возвращаем ошибку что нет ответа от устройства
  }
  else {                                              // Получили первый байт
    callTest++;
    while (1) {                                       // Крутимся пока время между байтами не станет больше MODBUS_TIME_END_PACKAGE
      if (osSemaphoreWait(semaphoreAnswer_, MODBUS_TIME_END_PACKAGE) == osEventTimeout) {
        int rxNum = uartReadData((uartNum)numberComPort_, buf);
        if ((rxNum == MODBUS_MIN_LENGHT_PACKAGE) &&
            (rxBuffer_[0] == txBuffer_[0]) &&              // Ответ от нужного устройства
            (rxBuffer_[1] == txBuffer_[1] + MODBUS_ERROR_0x80) &&    // Ответ с документированной ошибкой
            (rxBuffer_[2] >= MODBUS_ILLEGAL_FUNCTION_0x01) &&
            (rxBuffer_[2] <= MODBUS_GATEWAY_TARGET_DEVICE_0x0B)) {
              incErrCounter();
              return  MODBUS_ERROR_0x80 + rxBuffer_[2];
        }
        else if ((rxNum >= num) &&                    // Получили ожидаемое количество байт
                 (rxBuffer_[0] == txBuffer_[0]) &&         // Ответ от нужного устройства
                 (rxBuffer_[1] == txBuffer_[1])) {        // Нужное количество данных
                  if (((rxBuffer_[num - 1] << 8) + rxBuffer_[num - 2]) == crc16_ibm(rxBuffer_, (num - 2))) {
                    incSuccessCounter();
                    return  MODBUS_OK;
                  }
                  else {
                    incCrcCounter();
                    return  MODBUS_ERROR_CRC;
                  }
        }
        else {
          incTrashCounter();
          return  MODBUS_ERROR_TRASH;
        }
      }
      callTest++;
    }
  }
  return res;
}

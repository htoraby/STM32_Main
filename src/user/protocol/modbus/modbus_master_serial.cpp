/*
 * modbus_master_serial.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master_serial.h"
#include "user_main.h"
//#include "stdlib.h"

ModbusMasterSerial::ModbusMasterSerial()
{

}

ModbusMasterSerial::~ModbusMasterSerial()
{

}

int ModbusMasterSerial::openProtocol(int portName,
                                     long baudRate,
                                     int dataBits,
                                     int stopBits,
                                     int parity)
{
  numberComPort_ = portName;
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
  int Result = ok_r;
  uartClose((uartNum)PortName);
  resetTotalCounter();
  resetSuccessCounter();
  resetCrcCounter();
  resetErrCounter();
  resetFailCounter();
  resetLostCounter();
  resetTrashCounter();
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
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Modbus Serial: ошибка записи данных в порт (numberComPort_= %d)",
                 numberComPort_);
#endif
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

uint8_t ModbusMasterSerial::rxLogNovomet(uint8_t *buf)
{
  // Если истек таймаут ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, timeOut_) == osEventTimeout) {
    incLostCounter();
    return 0;                                       // Возвращаем ошибку что нет ответа от устройства
  }
  else {                                              // Получили первый байт
    while (1) {                                       // Крутимся пока время между байтами не станет больше MODBUS_TIME_END_PACKAGE
      if (osSemaphoreWait(semaphoreAnswer_, MODBUS_TIME_END_PACKAGE) == osEventTimeout) {
        return uartReadData((uartNum)numberComPort_, buf);
      }
    }
  }
  return 0;
}

uint8_t ModbusMasterSerial::readLogNovomet(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  unTypeData value;
  uint16_t crc;
  uint8_t res = 0;
  uint8_t retry  = 0;
  int i = 0;
  div_t fieldRead;                                        // Количество целых записей
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                           // Адрес устройства
        txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;  // Команды
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);          // Старший байт адреса первого регистра
        txBuffer_[3] = startRef & 0x00ff;                   // Младший байт адреса первого регистра
        txBuffer_[4] = 0;                                   // Старший байт количества регистров
        txBuffer_[5] = refCnt & 0x00ff;                     // Младший байт количества регистров
        crc = crc16_ibm(txBuffer_, 6);                      // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                        // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);               // Старший байт контрольной суммы
        if (txBuf(txBuffer_, 8) == ok_r) {                  // Если отправили данные
          res = rxLogNovomet(rxBuffer_);
          if ((res == 2) && (rxBuffer_[0] == 0xff) && (rxBuffer_[1] == 0xff)) {
            return 1;
          }
          else {
            if (res >= 10) {
              fieldRead = div(res - 2,8);
              if ((fieldRead.rem == 0) && (((rxBuffer_[res - 1] << 8) + rxBuffer_[res - 2]) == crc16_ibm(rxBuffer_, (res - 2)))) {
                for (i = 0; i < (res - 2) / 2; i++) {
                  value.char_t[0] = rxBuffer_[1 + 2*i];
                  value.char_t[1] = rxBuffer_[0 + 2*i];
                  regArr[i] = value.uint16_t[0];
                }
                return i;
              }
            }
          }
        }
        else {
          asm("nop");
        }
        if (retry >= retryCnt_) {
          return 0;
        }
        osDelay(100);
      }
    }
  }
  return 0;
}

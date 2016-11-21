/*
 * modbus_master.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master.h"



ModbusMaster::ModbusMaster()
  : retryCnt_(5)
  , timeOut_(MODBUS_ANSWER_TIMEOUT)
{
  // TODO Auto-generated constructor stub

}

ModbusMaster::~ModbusMaster()
{
  // TODO Auto-generated destructor stub
}

uint8_t ModbusMaster::readCoils(uint8_t slaveAddr, uint16_t startRef, bool *bitArr, uint16_t refCnt)
{
  uint16_t crc;
  uint8_t res = MODBUS_ERROR_TRASH;
  uint8_t retry = 0;
  div_t Div;
  int i;
  int j;
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntCoils(refCnt)) {                      // Проверяем корректность количества катушек
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                       // Адрес устройства
        txBuffer_[1] = MODBUS_READ_COILS_0x01;          // Команды
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первой катушки
        txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первой катушки
        txBuffer_[4] = ((refCnt >> 8) & 0x00ff);        // Старший байт количества катушек
        txBuffer_[5] = refCnt & 0x00ff;                 // Младший байт количества катушек
        crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
                                                        // Вычисляем сколько байт мы ожидаем в ответе
                                                        // Количество катушек / 8, если остаток != 0 то целое увеличиваем на 1
                                                        // Получаем Целое и остаток от деления
        Div = div(refCnt,8);
        if(Div.rem != 0)                                // Если остаток не равен 0
          Div.quot++;                                   // Увеличиваем целую часть
        if(txBuf(txBuffer_, 8) == ok_r) {                       // Посылаем данные
          res = rxBuf(rxBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);    // Функция приёма данных
          if (res == MODBUS_OK) {
            for(i = 0; i < Div.quot; i++) {            // Цикл по байтам данных
              for(j = 0; j < 8; j++) {                 // Цикл по битам данных
                bitArr[i * 8 + j] = ((rxBuffer_[3 + i] >> j) & 0x01);
              }
            }
            return res;
          }
        }
        if (retry >= retryCnt_) {
          return res;
        }
        osDelay(100);
      }
    }
  }
  return res;
}

int ModbusMaster::readInputDiscretes(uint8_t SlaveAddr, uint16_t StartRef, bool BitArr[], uint16_t RefCnt)
{
  int result = MODBUS_ERROR_TRASH;
  div_t Div;                                          // Структура для вычисления байт с данными
  int I;                                              // Переменны счётчики
  int J;
  uint16_t crc = 0;                                   // Переменная для вычисления контрольной суммы
  if (checkDeviceAddress(SlaveAddr)) {
    if (checkCntCoils(RefCnt)) {                      // Проверяем корректность количества входов
      txBuffer_[0] = SlaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_READ_DISCRETE_INPUTS_0x02;// Команды
      txBuffer_[2] = ((StartRef >> 8) & 0x00ff);      // Старший байт адреса первого входа
      txBuffer_[3] = StartRef & 0x00ff;               // Младший байт адреса первого входа
      txBuffer_[4] = ((RefCnt >> 8) & 0x00ff);        // Старший байт количества входов
      txBuffer_[5] = RefCnt & 0x00ff;                 // Младший байт количества входов
      crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
      txBuffer_[7] = crc & 0x00ff;                    // Младший байт контрольной суммы
      Div = div(RefCnt,8);                            // Вычисляем сколько байт мы ожидаем в ответе
                                                      // Количество катушек / 8, если остаток != 0 то целое увеличиваем на 1
                                                      // Получаем Целое и остаток от деления

      if (Div.rem != 0)                               // Если остаток не равен 0
        Div.quot++;                                   // Увеличиваем целую часть
      if (txBuf(txBuffer_, 8)) {
        result = rxBuf(rxBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);
        switch (result) {                             // Анализируем ответ
        case MODBUS_OK:                               // Получен корректный ответ
          for (I = 0; I < Div.quot; I++) {           // Цикл по байтам данных
            for(J = 0; J < 8; J++) {                  // Цикл по битам данных
              BitArr[I * 8 + J] = ((rxBuffer_[3 + I] >> J) & 0x01);// Сохраняем данные в выходной буфер
            }
          }
          break;
        case MODBUS_ERROR_TRASH:                      // Получен ответ с ошибкой
          break;
        case MODBUS_ERROR_CRC:                        // Ответ с ошибкой CRC
          break;
        case MODBUS_ERROR_TIMEOUT:                    // Ответ не получен
          break;
        default:
          break;
        }
      }
    }
  }
  return result;
}

uint8_t ModbusMaster::readMultipleRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  unTypeData value;
  uint16_t crc;
  uint8_t res = MODBUS_ERROR_TRASH;
  uint8_t retry  = 0;
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
          res = rxBuf(rxBuffer_, refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {                           // Получен корректный ответ
            for (int i = 0; i < refCnt; i++) {
              value.char_t[0] = rxBuffer_[4 + 2*i];
              value.char_t[1] = rxBuffer_[3 + 2*i];
              regArr[i] = value.uint16_t[0];
            }
            return res;
          }
        } else {
          asm("nop");
        }
        if (retry >= retryCnt_) {
          return res;
        }
        osDelay(100);
      }
    }
  }
  return res;
}

int ModbusMaster::readMultipleLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  int res = MODBUS_ERROR_TRASH;
  unTypeData Value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;                             // Переменная для проверки CRC
  uint8_t retry  = 0;
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {                        // Проверяем корректность количества регистров
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                       // Адрес устройства
        txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
        txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
        txBuffer_[4] = 0;                               // Старший байт количества регистров
        txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
        crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
        if (txBuf(txBuffer_, 8) == ok_r) {
          res = rxBuf(rxBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {
            for (int i = 0; i < refCnt; i++) {
              Value.char_t[0] = rxBuffer_[6 + 4*i];
              Value.char_t[1] = rxBuffer_[5 + 4*i];
              Value.char_t[2] = rxBuffer_[4 + 4*i];
              Value.char_t[3] = rxBuffer_[3 + 4*i];
              int32Arr[i] = Value.uint32_t;
            }
            return res;
          }
          if (retry >= retryCnt_) {
            return res;
          }
          osDelay(100);
        }                                               // Не смог отправить запрос
      }                                                 // Некорректное число регистров
    }
  }
  return res;
}

int ModbusMaster::readMultipleFloats(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt)
{
  int res = MODBUS_ERROR_TRASH;
  unTypeData value;                                   // Переменна для формирования массива прочитанных данных
  uint16_t crc = 0;
  uint8_t retry = 0;
  if (checkDeviceAddress(slaveAddr)) {                // Проверяем корректность количества регистров
    if (checkCntReg(refCnt * 4)) {
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                       // Адрес устройства
        txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
        txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
        txBuffer_[4] = 0;                               // Старший байт количества регистров
        txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
        crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
        if (txBuf(txBuffer_, 8) == ok_r) {
          res = rxBuf(rxBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {
            for (int i = 0; i < refCnt; i++) {
              value.char_t[0] = rxBuffer_[6 + 4*i];
              value.char_t[1] = rxBuffer_[5 + 4*i];
              value.char_t[2] = rxBuffer_[4 + 4*i];
              value.char_t[3] = rxBuffer_[3 + 4*i];
              float32Arr[i] = value.float_t;
            }
            return res;
          }
        }
        if (retry >= retryCnt_) {
          return res;
        }
        osDelay(100);
      }
    }
  }
  return res;
}

uint8_t ModbusMaster::readInputRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  unTypeData value;
  uint16_t crc;
  uint8_t res = MODBUS_ERROR_TRASH;
  uint8_t retry  = 0;
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                           // Адрес устройства
        txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;  // Команды
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);          // Старший байт адреса первого регистра
        txBuffer_[3] = startRef & 0x00ff;                   // Младший байт адреса первого регистра
        txBuffer_[4] = 0;                                   // Старший байт количества регистров
        txBuffer_[5] = refCnt & 0x00ff;                     // Младший байт количества регистров
        crc = crc16_ibm(txBuffer_, 6);                      // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                        // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);               // Старший байт контрольной суммы
        if (txBuf(txBuffer_, 8) == ok_r) {                  // Если отправили данные
          res = rxBuf(rxBuffer_, refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {                           // Получен корректный ответ
            for (int i = 0; i < refCnt; i++) {
              value.char_t[0] = rxBuffer_[4 + 2*i];
              value.char_t[1] = rxBuffer_[3 + 2*i];
              regArr[i] = value.uint16_t[0];
            }
            return res;
          }
        } else {
          asm("nop");
        }
        if (retry >= retryCnt_) {
          return res;
        }
        osDelay(100);
      }
    }
  }
  return res;
}

int ModbusMaster::readInputLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  int res = MODBUS_ERROR_TRASH;
  unTypeData Value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;                             // Переменная для проверки CRC
  uint8_t retry  = 0;
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {                        // Проверяем корректность количества регистров
      while (1) {
        retry++;
        txBuffer_[0] = slaveAddr;                       // Адрес устройства
        txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;
        txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
        txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
        txBuffer_[4] = 0;                               // Старший байт количества регистров
        txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
        crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
        txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
        txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
        if (txBuf(txBuffer_, 8) == ok_r) {
          res = rxBuf(rxBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {
            for (int i = 0; i < refCnt; i++) {
              Value.char_t[0] = rxBuffer_[6 + 4*i];
              Value.char_t[1] = rxBuffer_[5 + 4*i];
              Value.char_t[2] = rxBuffer_[4 + 4*i];
              Value.char_t[3] = rxBuffer_[3 + 4*i];
              int32Arr[i] = Value.uint32_t;
            }
            return res;
          }
          if (retry >= retryCnt_) {
            return res;
          }
          osDelay(100);
        }                                               // Не смог отправить запрос
      }                                                 // Некорректное число регистров
    }
  }
  return res;
}

int ModbusMaster::readInputFloats(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt)
{
  int result = MODBUS_ERROR_TRASH;
  int i = 0;                                          // Счётчик, используется при формировании массива прочитанных данных
  unTypeData value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;
  if (checkDeviceAddress(slaveAddr)) {                // Проверяем корректность количества регистров
    if (checkCntReg(refCnt)) {
      txBuffer_[0] = slaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;
      txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
      txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
      txBuffer_[4] = 0;                               // Старший байт количества регистров
      txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
      crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
      txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
      if (txBuf(txBuffer_, 8)) {
        result = rxBuf(rxBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
        switch(result) {                              // Анализируем ответ
        case MODBUS_OK:                               // Получен корректный ответ
          for(i = 0; i < refCnt; i++) {
            value.char_t[0] = rxBuffer_[6 + 4*i];
            value.char_t[1] = rxBuffer_[5 + 4*i];
            value.char_t[2] = rxBuffer_[4 + 4*i];
            value.char_t[3] = rxBuffer_[3 + 4*i];
            float32Arr[i] = value.float_t;
          }
          break;
        case MODBUS_ERROR_TRASH:                      // Получен ответ с ошибкой
          break;
        case MODBUS_ERROR_CRC:                        // Ответ с ошибкой CRC
          break;
        case MODBUS_ERROR_TIMEOUT:                    // Ответ не получен
          break;
        default:
          break;
        }
      }
    }
  }
  return result;
}

int ModbusMaster::writeCoil(uint8_t slaveAddr, int bitAddr, int bitVal)
{
  int res = err_r;
  uint16_t crc = 0;
  uint8_t retry = 0;
  if (checkDeviceAddress(slaveAddr)) {
    while (1) {
      retry++;
      txBuffer_[0] = slaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_WRITE_SINGLE_COIL_0x05;   // Команды
      txBuffer_[2] = ((bitAddr >> 8) & 0x00ff);       // Старший байт адреса катушки
      txBuffer_[3] = bitAddr & 0x00ff;                // Младший байт адреса катушки
      if (bitVal) {
        txBuffer_[4] = 0xff;                          // Старший байт значения
        txBuffer_[5] = 0x00;                          // Младший байт значения
      }
      else {
        txBuffer_[4] = 0x00;                          // Старший байт значения
        txBuffer_[5] = 0x00;                          // Младший байт значения
      }
      crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
      txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
      if (txBuf(txBuffer_, 8) == ok_r) {
        res = rxBuf(rxBuffer_, 8);
        if (res == MODBUS_OK) {
          return res;                                 // Возвращаем что запись выполнена
        }
      }
      if (retry >= retryCnt_) {
        return res;                                   // Возвращаем код ошибки Modbus
      }
      osDelay(100);
    }
  }
  return res;
}

int ModbusMaster::writeSingleRegister(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal)
{
  int res = err_r;
  uint16_t crc = 0;
  uint8_t retry = 0;
  if (checkDeviceAddress(slaveAddr)) {
    while (1) {
      retry++;
      txBuffer_[0] = slaveAddr;                         // Адрес устройства
      txBuffer_[1] = MODBUS_WRITE_SINGLE_REGISTER_0x06; // Команды
      txBuffer_[2] = ((regAddr >> 8) & 0x00ff);         // Старший байт адреса катушки
      txBuffer_[3] = regAddr & 0x00ff;                  // Младший байт адреса катушки
      txBuffer_[4] = ((regVal >> 8) & 0x00ff);          // Старший байт значения
      txBuffer_[5] = regVal & 0x00ff;                   // Младший байт значения
      crc = crc16_ibm(txBuffer_, 6);                    // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;
      txBuffer_[7] = ((crc >> 8) & 0x00ff);
      if (txBuf(txBuffer_, 8) == ok_r) {
        res = rxBuf(rxBuffer_, 8);
        if (res == MODBUS_OK) {
          return res;                                   // Возвращаем что запись выполнена
        }
      }
      if (retry >= retryCnt_) {
        return res;                                     // Возвращаем код ошибки Modbus
      }
      osDelay(100);
    }
  }
  return res;
}

int ModbusMaster::readExceptionStatus(uint8_t slaveAddr, unsigned char *statusBytePtr)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::returnQueryData(int slaveAddr, const unsigned char queryArr[], unsigned char echoArr[], int len)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::restartCommunicationsOption(int slaveAddr, int clearEventLog)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::forceMultipleCoils(uint8_t slaveAddr, uint16_t startRef, const bool bitArr[], uint16_t refCnt)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::writeMultipleRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::writeMultipleLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  int res = err_r;
  uint16_t crc = 0;
  uint8_t retry = 0;
  if (checkDeviceAddress(slaveAddr)) {
    while (1) {
      retry++;
      txBuffer_[0] = slaveAddr;                                 // Адрес устройства
      txBuffer_[1] = MODBUS_WRITE_MULTIPLE_REGISTERS_0x10;      // Команды
      txBuffer_[2] = ((startRef >> 8) & 0x00ff);                // Старший байт первого адреса
      txBuffer_[3] = startRef & 0x00ff;                         // Младший байт первого адреса
      txBuffer_[4] = ((refCnt*2 >> 8) & 0x00ff);                // Старший байт количества 16-ти битных регистров
      txBuffer_[5] = refCnt*2 & 0x00ff;                         // Младший байт количества 16-ти битных регистров
      txBuffer_[6] = refCnt*2*2;                                // Количество байт данных
      txBuffer_[7] = (int32Arr[0] >> 24) & 0x000000ff;
      txBuffer_[8] = (int32Arr[0] >> 16) & 0x000000ff;
      txBuffer_[9] = (int32Arr[0] >> 8) & 0x000000ff;
      txBuffer_[10] = int32Arr[0] & 0x000000ff;
      crc = crc16_ibm(txBuffer_, 11);
      txBuffer_[11] = crc & 0x00ff;
      txBuffer_[12] = ((crc >> 8) & 0x00ff);
      if (txBuf(txBuffer_, 13) == ok_r) {
        res = rxBuf(rxBuffer_, 8);
        if (res == MODBUS_OK) {
          return res;                                   // Возвращаем что запись выполнена
        }
      }
      if (retry >= retryCnt_) {
        return res;                                     // Возвращаем код ошибки Modbus
      }
      osDelay(100);
    }
  }
  return res;
}

int ModbusMaster::writeMultipleFloats(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt)
{
  /*
  int res = err_r;
  uint16_t crc = 0;
  uint8_t retry = 0;
  if (checkDeviceAddress(slaveAddr)) {
    while (1) {
      retry++;
      txBuffer_[0] = slaveAddr;                                 // Адрес устройства
      txBuffer_[1] = MODBUS_WRITE_MULTIPLE_REGISTERS_0x10;      // Команды
      txBuffer_[2] = ((startRef >> 8) & 0x00ff);                // Старший байт первого адреса
      txBuffer_[3] = startRef & 0x00ff;                         // Младший байт первого адреса
      txBuffer_[4] = ((refCnt*2 >> 8) & 0x00ff);                // Старший байт количества 16-ти битных регистров
      txBuffer_[5] = refCnt*2 & 0x00ff;                         // Младший байт количества 16-ти битных регистров
      txBuffer_[6] = refCnt*2*2;                                // Количество байт данных
      txBuffer_[7] = (float32Arr[0] >> 24) & 0x000000ff;
      txBuffer_[8] = (float32Arr[0] >> 16) & 0x000000ff;
      txBuffer_[9] = (float32Arr[0] >> 8) & 0x000000ff;
      txBuffer_[10] = float32Arr[0] & 0x000000ff;
      crc = crc16_ibm(txBuffer_, 11);
      txBuffer_[11] = crc & 0x00ff;
      txBuffer_[12] = ((crc >> 8) & 0x00ff);
      if (txBuf(txBuffer_, 13) == ok_r) {
        res = rxBuf(rxBuffer_, 8);
        if (res == MODBUS_OK) {
          return res;                                   // Возвращаем что запись выполнена
        }
      }
      if (retry >= retryCnt_) {
        return res;                                     // Возвращаем код ошибки Modbus
      }
      osDelay(100);
    }
  }
  */
  return MODBUS_ERROR_TRASH;
}

bool ModbusMaster::checkDeviceAddress(int address)
{
  if ((address > 0) && (address < 256))
    return true;
  else
    return false;
}

// МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА КАТУШЕК
bool ModbusMaster::checkCntCoils(int Cnt)
{
  if (Cnt <= MODBUS_MAX_FC01_COILS)
    return true;
  return false;
}

bool ModbusMaster::checkCntReg(int Cnt)
{
  bool res = false;
  if (Cnt <= MODBUS_MAX_FC03_WORDS)
    res = true;
  return res;
}

// МЕТОД ПРОВЕРКИ ОТКРЫТИЯ ПРОТОКОЛА
int ModbusMaster:: isOpen()
{
  int Result = err_r;
  try
  {

  }
  catch(...)
  {

  }
  return Result;
}

bool ModbusMaster::isConnect()
{
  if (counters_.failure > MODBUS_COUNTER_LOST_CONNECT)
    return false;
  else
    return true;
}

int  ModbusMaster::setTimeout(uint32_t time)
{
  int res = err_r;
  res = checkRange(time,1,100000,true);
  if (res == ok_r)
    timeOut_ = time;
  return res;
}

int ModbusMaster::setRetryCnt(int retry)
{
  int res = err_r;
  res = checkRange(retry,0,10,true);
  if (res == ok_r)
    retryCnt_ = retry;
  return res;
}

void ModbusMaster::calcConnect()
{
  counters_.quality = counters_.resive / counters_.transmite * 100;
  if (counters_.transmite > 999999.0) {
    counters_ = {counters_.quality,0,0,0,0,0,0,0};
  }
}

uint32_t ModbusMaster::getTimeout()
{
  return timeOut_;
}

int ModbusMaster::getRetryCnt()
{
  return (retryCnt_);
}

stConnectQuality* ModbusMaster::getCounters()
{
  return &counters_;
}

void ModbusMaster::resetCounters()
{
  counters_ = {0,0,0,0,0,0,0,0};
}

void ModbusMaster::configureBigEndianInts()
{
  endian_ = 0;
}

void ModbusMaster::configureLittleEndianInts()
{
  endian_ = 1;
}

uint8_t ModbusMaster::txBuf(uint8_t *buf, uint8_t num)
{
  return err_r;
}

uint8_t ModbusMaster::rxBuf(uint8_t *buf, uint8_t num)
{
  return err_r;
}

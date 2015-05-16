/*
 * modbus_master.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master.h"



ModbusMaster::ModbusMaster()
  : retryCnt_(MODBUS_COUNTER_LOST_CONNECT)
  , timeOut_(MODBUS_ANSWER_TIMEOUT)
{
  // TODO Auto-generated constructor stub

}

ModbusMaster::~ModbusMaster()
{
  // TODO Auto-generated destructor stub
}

int ModbusMaster::readCoils(int SlaveAddr, int StartRef, bool BitArr[], int RefCnt)
{
  int result = MODBUS_ERROR_TRASH;
  div_t Div;
  int I;
  int J;
  uint16_t Crc = 0;
  if (checkDeviceAddress(SlaveAddr)) {
    if (checkCntCoils(RefCnt)) {                      // Проверяем корректность количества катушек
      txBuffer_[0] = SlaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_READ_COILS_0x01;          // Команды
      txBuffer_[2] = ((StartRef >> 8) & 0x00ff);      // Старший байт адреса первой катушки
      txBuffer_[3] = StartRef & 0x00ff;               // Младший байт адреса первой катушки
      txBuffer_[4] = ((RefCnt >> 8) & 0x00ff);        // Старший байт количества катушек
      txBuffer_[5] = RefCnt & 0x00ff;                 // Младший байт количества катушек
      Crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = Crc & 0x00ff;                    // Младший байт контрольной суммы
      txBuffer_[7] = ((Crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
                                                      // Вычисляем сколько байт мы ожидаем в ответе
                                                      // Количество катушек / 8, если остаток != 0 то целое увеличиваем на 1
                                                      // Получаем Целое и остаток от деления
      Div = div(RefCnt,8);
      if(Div.rem != 0)                                // Если остаток не равен 0
        Div.quot++;                                   // Увеличиваем целую часть
      if(txBuf(txBuffer_, 8)) {                       // Посылаем данные
        result = rxBuf(txBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);    // Функция приёма данных
        switch(result) {
        case MODBUS_OK:                               // Получен корректный ответ
          for(I = 0; I <= Div.quot; I++) {            // Цикл по байтам данных
            for(J = 0; J <= 7; J++) {                 // Цикл по битам данных
              BitArr[I * 8 + J] = ((txBuffer_[3 + I] >> J) & 0x01);
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

int ModbusMaster::readInputDiscretes(int SlaveAddr, int StartRef, bool BitArr[], int RefCnt)
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
        result = rxBuf(txBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);
        switch (result) {                             // Анализируем ответ
        case MODBUS_OK:                               // Получен корректный ответ
          for (I = 0; I <= Div.quot; I++) {           // Цикл по байтам данных
            for(J = 0; J <= 7; J++) {                 // Цикл по битам данных
              BitArr[I * 8 + J] = ((txBuffer_[3 + I] >> J) & 0x01);// Сохраняем данные в выходной буфер
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
            for (int i = 0; i <= refCnt; i++) {
              value.char_t[0] = rxBuffer_[4 + 2*i];
              value.char_t[1] = rxBuffer_[3 + 2*i];
              regArr[i] = value.uint16_t[0];
            }
            return res;
          }
        }
        if (retry > retryCnt_) {
          return res;
        }
      }
    }
  }
  return res;
}

int ModbusMaster::readMultipleLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt)
{
  int res = MODBUS_ERROR_TRASH;
  int i = 0;                                          // Счётчик, используется при формировании массива прочитанных данных
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
            for (i = 0; i <= refCnt; i++) {
              Value.char_t[0] = rxBuffer_[6 + 4*i];
              Value.char_t[1] = rxBuffer_[5 + 4*i];
              Value.char_t[2] = rxBuffer_[4 + 4*i];
              Value.char_t[3] = rxBuffer_[3 + 4*i];
              int32Arr[i] = Value.uint32_t;
            }
            return res;
          }
          if (retry > retryCnt_) {
            return res;
          }
        }                                               // Не смог отправить запрос
      }                                                 // Некорректное число регистров
    }
  }
  return res;
}

int ModbusMaster::readMultipleFloats(int slaveAddr, int startRef, float *float32Arr, int refCnt)
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
            for (int i = 0; i <= refCnt; i++) {
              value.char_t[0] = rxBuffer_[6 + 4*i];
              value.char_t[1] = rxBuffer_[5 + 4*i];
              value.char_t[2] = rxBuffer_[4 + 4*i];
              value.char_t[3] = rxBuffer_[3 + 4*i];
              float32Arr[i] = value.float_t;
            }
            return res;
          }
        }
        if (retry > retryCnt_) {
          return res;
        }
      }
    }
  }
  return res;
}

int ModbusMaster::readInputRegisters(int slaveAddr, int startRef, short regArr[], int refCnt)
{
  int result = MODBUS_ERROR_TRASH;
  unTypeData value;
  uint16_t crc;

  txBuffer_[0] = slaveAddr;                           // Адрес устройства
  txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;    // Команды
  txBuffer_[2] = ((startRef >> 8) & 0x00ff);          // Старший байт адреса первого регистра
  txBuffer_[3] = startRef & 0x00ff;                   // Младший байт адреса первого регистра
  txBuffer_[4] = 0;                                   // Старший байт количества регистров
  txBuffer_[5] = refCnt & 0x00ff;                     // Младший байт количества регистров
  crc = crc16_ibm(txBuffer_, 6);                      // Вычисляем контрольную сумму
  txBuffer_[6] = crc & 0x00ff;                        // Младший байт контрольной суммы
  txBuffer_[7] = ((crc >> 8) & 0x00ff);               // Старший байт контрольной суммы
  if (txBuf(txBuffer_, 8)) {                  // Если отправили данные
    result = rxBuf(rxBuffer_, refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE);
    switch (result) {
    case MODBUS_OK:                                   // Получен корректный ответ
      for (int I = 0; I <= refCnt; I++) {
        value.char_t[0] = rxBuffer_[4 + 2*I];
        value.char_t[1] = rxBuffer_[3 + 2*I];
        regArr[I] = value.uint16_t[0];
      }
      break;
    case MODBUS_ERROR_TRASH:                          // Получен ответ с ошибкой
      break;
    case MODBUS_ERROR_CRC:                            // Ответ с ошибкой CRC
      break;
    case MODBUS_ERROR_TIMEOUT:                        // Ответ не получен
      break;
    default:
      break;
    }
  }
  return result;
}

int ModbusMaster::readInputLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt)
{
  int result = MODBUS_ERROR_TRASH;
  int i = 0;                                          // Счётчик, используется при формировании массива прочитанных данных
  unTypeData Value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;                             // Переменная для проверки CRC
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {                        // Проверяем корректность количества регистров
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
        switch (result) {
        case MODBUS_OK:                               // Получен корректный ответ
          for (i = 0; i <= refCnt; i++) {
            Value.char_t[0] = rxBuffer_[6 + 4*i];
            Value.char_t[1] = rxBuffer_[5 + 4*i];
            Value.char_t[2] = rxBuffer_[4 + 4*i];
            Value.char_t[3] = rxBuffer_[3 + 4*i];
            int32Arr[i] = Value.uint32_t;
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
      }                                               // Не смог отправить запрос
    }                                                 // Некорректное число регистров
  }                                                   // Некорректный адрес
  return result;
}

int ModbusMaster::readInputFloats(int slaveAddr, int startRef, float *float32Arr, int refCnt)
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
          for(i = 0; i <= refCnt; i++) {
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

int ModbusMaster::writeCoil(int slaveAddr, int bitAddr, int bitVal)
{
  int result = MODBUS_ERROR_TRASH;
  unsigned short crc = 0;
  if (checkDeviceAddress(slaveAddr)) {
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
  }
  return result;
}

int ModbusMaster::writeSingleRegister(int slaveAddr, int regAddr, short regVal)
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
      if (retry > retryCnt_) {
        return res;                                     // Возвращаем код ошибки Modbus
      }
    }
  }
  return res;
}

int ModbusMaster::readExceptionStatus(int slaveAddr, unsigned char *statusBytePtr)
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

int ModbusMaster::forceMultipleCoils(int slaveAddr, int startRef, const bool bitArr[], int refCnt)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::writeMultipleRegisters(int slaveAddr, int startRef, short *regArr, int refCnt)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::writeMultipleLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt)
{
  //TODO: Написать функцию
  return MODBUS_ERROR_TRASH;
}

int ModbusMaster::writeMultipleFloats(int slaveAddr, int startRef, float *float32Arr, int refCnt)
{
  //TODO: Написать функцию
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
  bool Result = err_r;
  try
  {
    if(Cnt <=MODBUS_MAX_FC01_COILS)
    {
      Result = ok_r;
    }
  }
  catch(...)
  {
    // Упали в функции checkCntCoils
  }
  return Result;
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
  if (getFailCounter() >= MODBUS_COUNTER_LOST_CONNECT)
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

void ModbusMaster::resetTotalCounter()
{
  totalCounter_ = 0;
}

void ModbusMaster::incTotalCounter()
{
  calcConnect();
  totalCounter_++;
}

void ModbusMaster::resetSuccessCounter()
{
  successCounter_ = 0;
}

void ModbusMaster::incSuccessCounter()
{
  successCounter_++;
  resetFailCounter();
}

uint32_t ModbusMaster::getTotalCounter()
{
  return (totalCounter_);
}

void ModbusMaster::incLostCounter()
{
  lostCounter_++;
  incFailCounter();
}

void ModbusMaster::resetLostCounter()
{
  lostCounter_ = 0;
}

void ModbusMaster::incCrcCounter()
{
  crcCounter_++;
  incFailCounter();
}

uint32_t ModbusMaster::getCrcCounter()
{
  return crcCounter_;
}

void ModbusMaster::resetCrcCounter()
{
  crcCounter_ = 0;
}

void ModbusMaster::incErrCounter()
{
  errCounter_++;
  incFailCounter();
}

uint32_t ModbusMaster::getErrCounter()
{
  return errCounter_;
}

void ModbusMaster::resetErrCounter()
{
  errCounter_ = 0;
}

void ModbusMaster::incTrashCounter()
{
  trashCounter_++;
  incFailCounter();
}

uint32_t ModbusMaster::getTrashCounter()
{
  return trashCounter_;
}

void ModbusMaster::resetTrashCounter()
{
  trashCounter_ = 0;
}

void ModbusMaster::incFailCounter()
{
  failCounter_++;
}

uint32_t ModbusMaster::getFailCounter()
{
  return failCounter_;
}

void ModbusMaster::resetFailCounter()
{
  failCounter_ = 0;
}

void ModbusMaster::calcConnect()
{
  uint32_t total = getTotalCounter();
  uint32_t success = getSuccessCounter();
  if (total > 1000000) {
    resetTotalCounter();
    resetSuccessCounter();
    resetLostCounter();
    resetCrcCounter();
    resetErrCounter();
    resetTrashCounter();
  }
  if (total)
    calcConnect_ = (success * 100) / total;
}

uint32_t ModbusMaster::getLostCounter()
{
  return (lostCounter_);
}

uint32_t ModbusMaster::getTimeout()
{
  return timeOut_;
}

uint32_t ModbusMaster::getSuccessCounter()
{
  return (successCounter_);
}

int ModbusMaster::getRetryCnt()
{
  return (retryCnt_);
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

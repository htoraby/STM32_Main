/*
 * modbus_master.cpp
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#include "modbus_master.h"



ModbusMaster::ModbusMaster()
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
        if (retry > RetryCnt) {
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
          res = rxBuf(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
          if (res == MODBUS_OK) {
            for (i = 0; i <= refCnt; i++) {
              Value.char_t[0] = txBuffer_[6 + 4*i];
              Value.char_t[1] = txBuffer_[5 + 4*i];
              Value.char_t[2] = txBuffer_[4 + 4*i];
              Value.char_t[3] = txBuffer_[3 + 4*i];
              int32Arr[i] = Value.uint32_t;
            }
            return res;
          }
          if (retry > RetryCnt) {
            return res;
          }
        }                                           // Не смог отправить запрос
      }                                                 // Некорректное число регистров
    }
  }
  return res;
}

int ModbusMaster::readMultipleFloats(int slaveAddr, int startRef, float *float32Arr, int refCnt)
{
  int result = MODBUS_ERROR_TRASH;
  int i = 0;                                          // Счётчик, используется при формировании массива прочитанных данных
  unTypeData value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;
  if (checkDeviceAddress(slaveAddr)) {                // Проверяем корректность количества регистров
    if (checkCntReg(refCnt)) {
      txBuffer_[0] = slaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;
      txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
      txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
      txBuffer_[4] = 0;                               // Старший байт количества регистров
      txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
      crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
      txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
      if (txBuf(txBuffer_, 8)) {
        result = rxBuf(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
        switch(result) {                              // Анализируем ответ
        case MODBUS_OK:                               // Получен корректный ответ
          for(i = 0; i <= refCnt; i++) {
            value.char_t[0] = txBuffer_[6 + 4*i];
            value.char_t[1] = txBuffer_[5 + 4*i];
            value.char_t[2] = txBuffer_[4 + 4*i];
            value.char_t[3] = txBuffer_[3 + 4*i];
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

int ModbusMaster::readInputRegisters(int slaveAddr, int startRef, short regArr[], int refCnt)
{
  unTypeData value;
  uint16_t crc;
  int result = MODBUS_ERROR_TRASH;
  txBuffer_[0] = slaveAddr;                           // Адрес устройства
  txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;  // Команды
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
        result = rxBuf(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
        switch (result) {
        case MODBUS_OK:                               // Получен корректный ответ
          for (i = 0; i <= refCnt; i++) {
            Value.char_t[0] = txBuffer_[6 + 4*i];
            Value.char_t[1] = txBuffer_[5 + 4*i];
            Value.char_t[2] = txBuffer_[4 + 4*i];
            Value.char_t[3] = txBuffer_[3 + 4*i];
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
        result = rxBuf(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
        switch(result) {                              // Анализируем ответ
        case MODBUS_OK:                               // Получен корректный ответ
          for(i = 0; i <= refCnt; i++) {
            value.char_t[0] = txBuffer_[6 + 4*i];
            value.char_t[1] = txBuffer_[5 + 4*i];
            value.char_t[2] = txBuffer_[4 + 4*i];
            value.char_t[3] = txBuffer_[3 + 4*i];
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
  int result = err_r;
  unsigned short crc = 0;
  if (checkDeviceAddress(slaveAddr)) {
    txBuffer_[0] = slaveAddr;                         // Адрес устройства
    txBuffer_[1] = MODBUS_WRITE_SINGLE_REGISTER_0x06; // Команды
    txBuffer_[2] = ((regAddr >> 8) & 0x00ff);         // Старший байт адреса катушки
    txBuffer_[3] = regAddr & 0x00ff;                  // Младший байт адреса катушки
    txBuffer_[4] = ((regVal >> 8) & 0x00ff);          // Старший байт значения
    txBuffer_[5] = regVal & 0x00ff;                   // Младший байт значения
    crc = crc16_ibm(txBuffer_, 6);                    // Вычисляем контрольную сумму
    txBuffer_[6] = crc & 0x00ff;
    txBuffer_[7] = ((crc >> 8) & 0x00ff);
    if (txBuf(txBuffer_, 8)) {
      result = rxBuf(rxBuffer_, 8);
    }
  }
  return result;
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

// МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА РЕГИСТРОВ
bool ModbusMaster::checkCntReg(int Cnt)
{
  bool Result = err_r;
  try
  {
    if(Cnt <= MODBUS_MAX_FC03_WORDS)
    {
      Result = ok_r;
    }
  }
  catch(...)
  {
    // Упали в функции checkCntReg
  }
  return Result;
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

// МЕТОД НАСТРОЙКИ ВРЕМЕНИ ОЖИДАНИЯ ОТВЕТА
// Настраивает тайм-аут
// Протокол должен быть закрыт
// Параметры:
// TimeOut значение  в мс (диапазон: 1 - 100000)
// Возвращает:
// RETURN_OK в случае успешного завершения
// Ошибку вне диапазона
// Ошибку протокол уже открыт
int  ModbusMaster::setTimeout(int Time)
{
  int Result = err_r;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введённого значения
      if((Time >=1 )&&(Time <= 100000))
      {
        TimeOut = Time;                                // Присваиваем время ожидания
        Result = ok_r;                            // Возвращаем ОК
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

// МЕТОД УСТАНОВКИ ПОЛИЧЕСТВА ПОВТОРОВ ЗАПРОСОВ
// Настраивает количество автоматических повторов запросов
// Параметры:
// RetryCnt    (0 - 10)
// RETURN_OK в случае успешного завершения
// Ошибку вне диапазона
// Ошибку протокол уже открыт
int ModbusMaster::setRetryCnt(int Retry)
{
  int Result = err_r;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введенного значения
      if((Retry >=0 )&&(Retry <= 10))
      {
        RetryCnt = Retry;                            // Присваиваем количества повторений
        Result = ok_r;                            // Возвращаем ОК
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

// МЕТОД ЗАДАНИЯ ЗАДЕРЖКИ МЕЖДУ ЗАПРОСАМИ
// Настраивает задержку между запросами,
// Протокол должен быть закрыт
// Параметры:
// PollDelay значение  в мс (диапазон: 1 - 100000)
// Возвращает:
// RETURN_OK в случае успешного завершения
// Ошибку вне диапазона
// Ошибку протокол уже открыт
int ModbusMaster::setPollDelay(int Delay)
{
  int Result = err_r;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введенного значения
      if((Delay >=1 )&&(Delay <= 100000))
      {
        PollDelay = Delay;                            // Присваиваем таймер задержки опроса
        Result = ok_r;                            // Возвращаем ОК
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

// МЕТОД СБРОСА СЧЁТЧИКА ОБЩЕГО КОЛИЧЕСТВА ЗАПРОСОВ
// Сбрасывает значение счётчика общего количества запросов в 0
void ModbusMaster::resetTotalCounter()
{
  totalCounter_ = 0;
};

void ModbusMaster::incTotalCounter()
{
  totalCounter_++;
};

// МЕТОД СБРОСА СЧЁТЧИКА ОТВЕТОВ НА ЗАПРОСЫ
//Сбрасывает значение счётчика корректных ответов в 0
void ModbusMaster::resetSuccessCounter()
{
  successCounter_ = 0;
}

void ModbusMaster::incSuccessCounter()
{
  successCounter_++;
};

// МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВА ЗАПРОСОВ
// Вовзращает значение счётчика общего количества запросов
long ModbusMaster::getTotalCounter()
{
  return (totalCounter_);
};

void ModbusMaster::incLostCounter()
{
  lostCounter_++;
}

void ModbusMaster::resetLostCounter()
{
  lostCounter_ = 0;
}

int ModbusMaster::getLostCounter()
{
  return (lostCounter_);
};

// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ТАЙМАУТА ОТВЕТА
// Возвращает значение таймаута
int ModbusMaster::getTimeout()
{
  return (TimeOut);
};

// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ЗАДЕРЖКИ МЕЖДУ ЗАПРОСАМИ
// Возвращает значение задержки между опросами
int ModbusMaster::getPollDelay()
{
  return (PollDelay);
};

// МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВ ОТВЕТОВ НА ЗАПРОСЫ
// Вовзращает значение счётчика корректных ответов на запросы
long ModbusMaster::getSuccessCounter()
{
  return (successCounter_);
}


// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ АВТОМАТИЧЕСКИХ ПОВТОРОВ ЗАПРОСОВ
// Возвращает количество автоматических запросов
int ModbusMaster::getRetryCnt()
{
  return (RetryCnt);
};

// МЕТОД КОНФИГУРИРОВАНИЯ BIG ENDIAN
// По умолчанию Modbus использует little-endian порядок передачи байт
// сначала младший потом старший, функция настраивает что другой порядок
// сначала старший, потом младший
void ModbusMaster::configureBigEndianInts()
{
  Endian = 0;
};

// МЕТОД КОНФИГУРИРОВАНИЯ LITTLE ENDIAN
// По умолчанию Modbus использует little-endian порядок передачи байт
// сначала младший потом старший
void ModbusMaster::configureLittleEndianInts()
{
  Endian = 1;
};

uint8_t ModbusMaster::txBuf(uint8_t *buf, uint8_t num)
{
  return err_r;
}

uint8_t ModbusMaster::rxBuf(uint8_t *buf, uint8_t num)
{
  return err_r;
}



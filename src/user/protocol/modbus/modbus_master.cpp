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
      if(transmitQuery(txBuffer_, 8)) {               // Посылаем данные
        result = receiveAnswer(txBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);    // Функция приёма данных
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
      if (transmitQuery(txBuffer_, 8)) {
        result = receiveAnswer(txBuffer_, Div.quot + MODBUS_MIN_LENGHT_PACKAGE);
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

int ModbusMaster::readMultipleRegisters(int slaveAddr, int startRef, short *regArr, int refCnt)
{
  unTypeData value;
  uint16_t crc;
  int result = MODBUS_ERROR_TRASH;
  txBuffer_[0] = slaveAddr;                           // Адрес устройства
  txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;  // Команды
  txBuffer_[2] = ((startRef >> 8) & 0x00ff);          // Старший байт адреса первого регистра
  txBuffer_[3] = startRef & 0x00ff;                   // Младший байт адреса первого регистра
  txBuffer_[4] = 0;                                   // Старший байт количества регистров
  txBuffer_[5] = refCnt & 0x00ff;                     // Младший байт количества регистров
  crc = crc16_ibm(txBuffer_, 6);                      // Вычисляем контрольную сумму
  txBuffer_[6] = crc & 0x00ff;                        // Младший байт контрольной суммы
  txBuffer_[7] = ((crc >> 8) & 0x00ff);               // Старший байт контрольной суммы
  if (transmitQuery(txBuffer_, 8)) {                  // Если отправили данные
    result = receiveAnswer(rxBuffer_, refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE);
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

int ModbusMaster::readMultipleLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt)
{
  int result = MODBUS_ERROR_TRASH;
  int i = 0;                                          // Счётчик, используется при формировании массива прочитанных данных
  unTypeData Value;                                   // Переменна для формирования массива прочитанных данных
  unsigned short crc = 0;                             // Переменная для проверки CRC
  if (checkDeviceAddress(slaveAddr)) {
    if (checkCntReg(refCnt)) {                        // Проверяем корректность количества регистров
      txBuffer_[0] = slaveAddr;                       // Адрес устройства
      txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;
      txBuffer_[2] = ((startRef >> 8) & 0x00ff);      // Старший байт адреса первого регистра
      txBuffer_[3] = startRef & 0x00ff;               // Младший байт адреса первого регистра
      txBuffer_[4] = 0;                               // Старший байт количества регистров
      txBuffer_[5] = (refCnt * 2) & 0x00ff;           // Младший байт количества регистров
      crc = crc16_ibm(txBuffer_, 6);                  // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;                    // Младший байт контрольной суммы
      txBuffer_[7] = ((crc >> 8) & 0x00ff);           // Старший байт контрольной суммы
      if (transmitQuery(txBuffer_, 8)) {
        result = receiveAnswer(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
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
      if (transmitQuery(txBuffer_, 8)) {
        result = receiveAnswer(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
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
  if (transmitQuery(txBuffer_, 8)) {                  // Если отправили данные
    result = receiveAnswer(rxBuffer_, refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE);
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
      if (transmitQuery(txBuffer_, 8)) {
        result = receiveAnswer(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
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
      if (transmitQuery(txBuffer_, 8)) {
        result = receiveAnswer(txBuffer_, refCnt*4 + MODBUS_MIN_LENGHT_PACKAGE);
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
  int result = RETURN_ERROR;
  unsigned short crc = 0;
  if (!checkDeviceAddress(slaveAddr)) {
    txBuffer_[0] = slaveAddr;                         // Адрес устройства
    txBuffer_[1] = MODBUS_WRITE_SINGLE_REGISTER_0x06; // Команды
    txBuffer_[2] = ((regAddr >> 8) & 0x00ff);         // Старший байт адреса катушки
    txBuffer_[3] = regAddr & 0x00ff;                  // Младший байт адреса катушки
    txBuffer_[4] = ((regVal >> 8) & 0x00ff);          // Старший байт значения
    txBuffer_[5] = regVal & 0x00ff;                   // Младший байт значения
    crc = crc16_ibm(txBuffer_, 6);                    // Вычисляем контрольную сумму
    txBuffer_[6] = crc & 0x00ff;
    txBuffer_[7] = ((crc >> 8) & 0x00ff);
    if (transmitQuery(txBuffer_, 8)) {
      result = receiveAnswer(rxBuffer_, 8);
    }
  }
  return result;
}

// МЕТОД ЧТЕНИЯ ИСКЛЮЧЕНИЯ
// Modbus функция 7 чтение cтатуса исключений
// Считывание состояния катушки исключение внутри ведомого устройства.
// Параметры:
// slaveAddr Modbus Адрес ведомого устройства
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::readExceptionStatus(    int SlaveAddr,
                                          unsigned char *StatusBytePtr)
{
  int Result = RETURN_ERROR;
  unsigned short Crc = 0;
  try
  {
    // ФОРМИРУЕМ ПОСЫЛКУ
    // Проверяем корректность адреса
    if(checkDeviceAddress(SlaveAddr))
    {
      txBuffer_[0] = SlaveAddr;                                // Адрес устройства
      txBuffer_[1] = MODBUS_READ_EXCEPTION_STATUS_0x07;        // Команды
      Crc = crc16_ibm(txBuffer_, 2);                            // Вычисляем контрольную сумму
      txBuffer_[2] = Crc & 0x00ff;                            // Младший байт контрольной суммы
      txBuffer_[3] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы

    }
  }
  catch(...)
  {

  }
  return Result;
}

// МЕТОД ЭХО ЗАПРОСА
// Функция Modbus код 8, подфункция 00, Вернуться Запрос данных.
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы (диапазон: 1 - 255)
// QueryArr буфер с данными для отправки
// EchoArr буфера, который будет содержать считывать данные
// Len Количество байт Написать отправлено и читать назад
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::returnQueryData(        int SlaveAddr,
                                          const unsigned char QueryArr[],
                                          unsigned char EchoArr[],
                                          int Len)
{
  int Result = RETURN_ERROR;
  unsigned char I = 0;                                                // Временный счётчик
  unsigned short Crc = 0;                                                // Переменная для контрольной суммы

  try
  {
    // ФОРМИРУЕМ ПОСЫЛКУ
    // Проверяем корректность адреса
    if(checkDeviceAddress(SlaveAddr))
    {
      txBuffer_[0] = SlaveAddr;                                    // Адрес устройства
      txBuffer_[1] = MODBUS_DIAGNOSTICS_0x08;                        // Команды
      txBuffer_[2] = 0x00;                                        // Запрос эхо
      txBuffer_[3] = 0x00;                                        // Запрос эхо
      for(I = 0; I <= Len; I++)
      {
        txBuffer_[I + 4] = QueryArr[I];
      }
      Crc = crc16_ibm(txBuffer_, Len + 4);
      txBuffer_[Len + 5] = Crc & 0x00ff;                            // Младший байт контрольной суммы
      txBuffer_[Len + 6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
    }
  }
  catch(...)
  {

  }
  return Result;
}

// МЕТОД СБРОСА НАСТРОЕКТ ОПРОСА
// Функция Modbus код 8, подфункция 01, сброс настроек
// Параметры:
// slaveAddr Modbus Адрес ведомого устройства  (диапазон: 1 - 255)
// clearEventLog Флаг
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::restartCommunicationsOption( int SlaveAddr,
                                               int ClearEventLog)
{
  int Result = RETURN_ERROR;
  try
  {

  }
  catch(...)
  {

  }
  return Result;
}

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
int ModbusMaster::forceMultipleCoils( int SlaveAddr,
                                      int StartRef,
                                      const bool BitArr[],
                                      int RefCnt)
{
  int Result = RETURN_ERROR;
  div_t Div;
  unsigned char CntByte = 0;             // Счётчик байт данных
  unsigned char I = 0;                   // Временный счётчик
  unsigned short Crc = 0;                // Переменная для контрольной суммы

  try
  {
    // ФОРМИРУЕМ ПОСЫЛКУ
    // Проверяем корректность адреса
    if(checkDeviceAddress(SlaveAddr))
    {
      // Проверяем корректность количества записываемых катушек
      if(checkCntReg(RefCnt))
      {
        // Адрес устройства
        txBuffer_[0] = SlaveAddr;
        // Команды
        txBuffer_[1] = MODBUS_WRITE_MULTIPLE_COILS_0x0F;
        // Старший байт адреса первой катушки
        txBuffer_[2] = ((StartRef >> 8) & 0x00ff);
        // Младший байт адреса катушки
        txBuffer_[3] = StartRef & 0x00ff;
        // Заполнение данных
        // Вычисляем сколько у нас байт данных
        // Получаем Целое и остаток от деления
        Div = div(RefCnt,8);
        // Если остаток не равен 0
        if(Div.rem != 0)
        {
          // Увеличиваем целую часть
          Div.quot++;
        }
        // Получаем количество байт
        CntByte = Div.quot;
        // Проверяем количество байт данных
        if(checkDeviceAddress(CntByte))
        {
          // Количество байт с данными
          txBuffer_[4] = CntByte;
          for(I = 0; I <= CntByte; I++)
          {
            // Младший байт данных
            txBuffer_[I + 5] = BitArr[I] & 0x00ff;
            // Старший байт данных
            txBuffer_[I + 6] = ((BitArr[I] >> 8) & 0x00ff);
          }
          // Вычисляем контрольную сумму
          Crc = crc16_ibm(txBuffer_, CntByte + 5);
          // Старший байт контрольной суммы
          txBuffer_[CntByte + 6] = ((Crc >> 8) & 0x00ff);
          // Младший байт контрольной суммы
          txBuffer_[CntByte + 7] = Crc & 0x00ff;
        }
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

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
int ModbusMaster::writeMultipleRegisters(    int SlaveAddr,
                                             int StartRef,
                                             short *RegArr,
                                             int RefCnt)
{
  int Result = RETURN_ERROR;
  unsigned char I = 0;                                                // Временный счётчик
  unsigned short Crc = 0;                                                // Переменная для контрольной суммы
  try
  {
    // ФОРМИРУЕМ ПОСЫЛКУ
    // Проверяем корректность адреса
    if(checkDeviceAddress(SlaveAddr))
    {
      // Проверяем корректность количества регистров
      if(checkCntReg(RefCnt))
      {
        txBuffer_[0] = SlaveAddr;                                // Адрес устройства
        txBuffer_[1] = MODBUS_WRITE_MULTIPLE_REGISTERS_0x10;    // Команды
        txBuffer_[2] = ((StartRef >> 8) & 0x00ff);                // Старший байт адреса первого регистра
        txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
        txBuffer_[4] = ((RefCnt >> 8) & 0x00ff);                // Старший байт адреса первого регистра
        txBuffer_[5] = RefCnt & 0x00ff;                            // Младший байт адреса первого регистра
        txBuffer_[6] = RefCnt*2;                                // Количество
        for(I = 0; I <= RefCnt; I++)
        {
          txBuffer_[I * 2 + 7] = RegArr[I] & 0x00ff;            // Младший байт данных
          txBuffer_[I * 2 + 8] = ((RegArr[I] >> 8) & 0x00ff);    // Старший байт данных
        }
        Crc = crc16_ibm(txBuffer_, RefCnt*2 + 7);                // Вычисляем контрольную сумму
        txBuffer_[RefCnt*2 + 8] = ((Crc >> 8) & 0x00ff);        // Старший байт контрольной суммы
        txBuffer_[RefCnt*2 + 9] = Crc & 0x00ff;                    // Младший байт контрольной суммы
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

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
int ModbusMaster::writeMultipleLongInts(    int SlaveAddr,
                                            int StartRef,
                                            int *Int32Arr,
                                            int RefCnt)
{
  int Result = RETURN_ERROR;
  unsigned char I = 0;                                                // Временный счётчик
  //unsigned short Crc = 0;                                                // Переменная для контрольной суммы
  short RegArr[RefCnt*2+9];
  try
  {
    // Переводим в формат 16-ти битовых регистров
    for(I = 0; I<= RefCnt; I++)
    {
      RegArr[I * 4] = (Int32Arr[I] >> 24) & 0x000000ff;
      RegArr[I * 4 + 1] = (Int32Arr[I] >> 16) & 0x000000ff;
      RegArr[I * 4 + 2] = (Int32Arr[I] >> 8) & 0x000000ff;
      RegArr[I * 4 + 3] = (Int32Arr[I]) & 0x000000ff;
    }
    // Вызываем запись данных
    writeMultipleRegisters(SlaveAddr, StartRef, RegArr, RefCnt*2);
  }
  catch(...)
  {

  }
  return Result;
}

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
int ModbusMaster::writeMultipleFloats(    int SlaveAddr,
                                          int StartRef,
                                          float *Float32Arr,
                                          int RefCnt)
{
  int Result = RETURN_ERROR;
  unsigned char I = 0;                                                // Временный счётчик
  //unsigned short Crc = 0;                                                // Переменная для контрольной суммы
  short RegArr[RefCnt*2 + 9];
  unTypeData Arr;
  try
  {
    // Переводим в формат 16-ти битовых регистров
    for(I = 0; I<= RefCnt; I++)
    {
      Arr.float_t = Float32Arr[I];
      RegArr[I * 4] = Arr.char_t[0];
      RegArr[I * 4 + 1] = Arr.char_t[1];
      RegArr[I * 4 + 2] = Arr.char_t[2];
      RegArr[I * 4 + 3] = Arr.char_t[3];
    }
    // Вызываем запись данных
    writeMultipleRegisters(SlaveAddr, StartRef, RegArr, RefCnt*2);
  }
  catch(...)
  {

  }
  return Result;
}


// МЕТОД ПРОВЕРКИ КОРРЕКТНОСТИ АДРЕСА УСТРОЙТВА
bool ModbusMaster::checkDeviceAddress(int Address)
{
  bool Result = RETURN_ERROR;
  try
  {
    if((Address > 0)&&(Address < 256))
    {
      Result = RETURN_OK;
    }
  }
  catch(...)
  {
    // Упали в функции checkDeviceAddress
  }
  return Result;
}

// МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА КАТУШЕК
bool ModbusMaster::checkCntCoils(int Cnt)
{
  bool Result = RETURN_ERROR;
  try
  {
    if(Cnt <=MODBUS_MAX_FC01_COILS)
    {
      Result = RETURN_OK;
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
  bool Result = RETURN_ERROR;
  try
  {
    if(Cnt <= MODBUS_MAX_FC03_WORDS)
    {
      Result = RETURN_OK;
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
  int Result = RETURN_ERROR;
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
  int Result = RETURN_ERROR;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введённого значения
      if((Time >=1 )&&(Time <= 100000))
      {
        TimeOut = Time;                                // Присваиваем время ожидания
        Result = RETURN_OK;                            // Возвращаем ОК
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
  int Result = RETURN_ERROR;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введенного значения
      if((Retry >=0 )&&(Retry <= 10))
      {
        RetryCnt = Retry;                            // Присваиваем количества повторений
        Result = RETURN_OK;                            // Возвращаем ОК
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
  int Result = RETURN_ERROR;
  try
  {
    // Проверяем если у нас уже открыт порт
    if(isOpen())
    {
      // Проверяем корректность введенного значения
      if((Delay >=1 )&&(Delay <= 100000))
      {
        PollDelay = Delay;                            // Присваиваем таймер задержки опроса
        Result = RETURN_OK;                            // Возвращаем ОК
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

// МЕТОД ПОСЫЛКИ ДАННЫХ ИЗ БУФЕРА

int ModbusMaster::transmitQuery(unsigned char *Buf, int Count)
{
  return RETURN_ERROR;
}

// МЕТОД ЧТЕНИЕ ДАННЫХ ИЗ ПОРТА
// Buf - массив байт считываемый из порта
int ModbusMaster::receiveAnswer(unsigned char *Buf, uint8_t count)
{
  return RETURN_ERROR;
}



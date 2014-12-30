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

// МЕТОД ЧТЕНИЯ СОСТОЯНИЯ КАТУШЕК
// Modbus функция 1, Считывание состояния катушки
// Читает содержимое катушек
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
// StartRef Первая катушка
// RefCnt Количество считываемых катушек (Диапазон: 1-2000)
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::readCoils(   int SlaveAddr,
                                    int StartRef,
                                    bool BitArr[],
                                    int RefCnt)
{
    int Result = RETURN_ERROR;
    div_t Div;
    int I;
    int J;
    uint16_t Crc = 0;
    try
    {
        // ФОРМИРУЕМ ПОСЫЛКУ
        // Проверяем корректность адреса
        if(checkDeviceAddress(SlaveAddr))
        {
            // Проверяем корректность количества катушек
            if(checkCntCoils(RefCnt))
            {
            	// Адрес устройства
                txBuffer_[0] = SlaveAddr;
                // Команды
                txBuffer_[1] = MODBUS_READ_COILS_0x01;
                // Старший байт адреса первой катушки
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);
                // Младший байт адреса первой катушки
                txBuffer_[3] = StartRef & 0x00ff;
                // Старший байт количества катушек
                txBuffer_[4] = ((RefCnt >> 8) & 0x00ff);
                // Младший байт количества катушек
                txBuffer_[5] = RefCnt & 0x00ff;
                // Вычисляем контрольную сумму
                Crc = crc16_ibm(txBuffer_, 6);
                // Старший байт контрольной суммы
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);
                // Младший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;
                // Вычисляем сколько байт мы ожидаем в ответе
                // Количество катушек / 8, если остаток != 0 то целое увеличиваем на 1
                // Получаем Целое и остаток от деления
                Div = div(RefCnt,8);
                // Если остаток не равен 0
                if(Div.rem != 0)
                {
                	// Увеличиваем целую часть
                	Div.quot++;
                }
                // ПЕРЕДАЧА И ПРИЁМ ДАННЫХ
                if(transmitQuery(txBuffer_, 8))
                {
                    // Функция приёма данных
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                // Цикл по байтам данных
                                for(I = 0; I <= Div.quot; I++)
                                {
                                    // Цикл по битам данных
                                    for(J = 0; J <= 7; J++)
                                    {
                                        // Сохраняем данные в выходной буфер
                                        BitArr[I * 8 + J] = ((txBuffer_[3 + I] >> J) & 0x01);
                                    }
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                    }
                }
            }
        }
    }
    catch(...)
    {

    }
    return Result;
}

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
int ModbusMaster::readInputDiscretes( int SlaveAddr,
                                           int StartRef,
                                           bool BitArr[],
                                           int RefCnt)
{
    int Result = RETURN_ERROR;
    // Структура для вычисления байт с данными
    div_t Div;
    // Переменны счётчики
    int I;
    int J;
    // Переменная для вычисления контрольной суммы
    uint16_t Crc = 0;
    try
    {
        // ФОРМИРУЕМ ПОСЫЛКУ
        // Проверяем корректность адреса
        if(checkDeviceAddress(SlaveAddr))
        {
            // Проверяем корректность количества входов
            if(checkCntCoils(RefCnt))
            {
                // Адрес устройства
                txBuffer_[0] = SlaveAddr;
                // Команды
                txBuffer_[1] = MODBUS_READ_DISCRETE_INPUTS_0x02;
                // Старший байт адреса первого входа
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);
                // Младший байт адреса первого входа
                txBuffer_[3] = StartRef & 0x00ff;
                // Старший байт количества входов
                txBuffer_[4] = ((RefCnt >> 8) & 0x00ff);
                // Младший байт количества входов
                txBuffer_[5] = RefCnt & 0x00ff;
                // Вычисляем контрольную сумму
                Crc = crc16_ibm(txBuffer_, 6);
                // Старший байт контрольной суммы
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);
                // Младший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;
                // Вычисляем сколько байт мы ожидаем в ответе
                // Количество катушек / 8, если остаток != 0 то целое увеличиваем на 1
                // Получаем Целое и остаток от деления
                Div = div(RefCnt,8);
                // Если остаток не равен 0
                if(Div.rem != 0)
                {
                	// Увеличиваем целую часть
                	Div.quot++;
                }
                // ПЕРЕДАЧА И ПРИЁМ ДАННЫХ
                if(transmitQuery(txBuffer_, 8))
                {
                	// Функция приёма данных
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                // Цикл по байтам данных
                                for(I = 0; I <= Div.quot; I++)
                                {
                                    // Цикл по битам данных
                                    for(J = 0; J <= 7; J++)
                                    {
                                        // Сохраняем данные в выходной буфер
                                        BitArr[I * 8 + J] = ((txBuffer_[3 + I] >> J) & 0x01);
                                    }
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                    }
                }
            }
        }
    }
    catch(...)
    {

    }
    return Result;
}

// Метод чтения регистров устройства функций 0х03
int ModbusMaster::readMultipleRegisters( int slaveAddr,
                                         int startRef,
                                         short *regArr,
                                         int refCnt)
{
  unTypeData Value;
  uint16_t Crc;
  int Count;
  int Result = MODBUS_ERROR_TRASH;
  // Формирование посылки из данных в формат Modbus запроса
  // Проверка адреса устройства
  txBuffer_[0] = slaveAddr;                                // Адрес устройства
  txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;       // Команды
  txBuffer_[2] = ((startRef >> 8) & 0x00ff);               // Старший байт адреса первого регистра
  txBuffer_[3] = startRef & 0x00ff;                        // Младший байт адреса первого регистра
  txBuffer_[4] = 0;                                        // Старший байт количества регистров
  txBuffer_[5] = refCnt & 0x00ff;                          // Младший байт количества регистров
  Crc = crc16_ibm(txBuffer_, 6);                           // Вычисляем контрольную сумму
  txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
  txBuffer_[7] = Crc & 0x00ff;                             // Младший байт контрольной суммы
  // Если отправили данные
  if (transmitQuery(txBuffer_, 8)){
    // Принимаем данные получаем количество принятых байт
    Count = reseiveAnswer(bufferRx_);
    // Если хоть что-то получили
    if (Count) {
      // Корректная длина пакета
      if ((Count == refCnt*2 + MODBUS_MIN_LENGHT_PACKAGE)
          // Совпадает адрес устройства
          &&(bufferRx_[0] == slaveAddr)
          // Совпадает функция на которую отвечаем
          &&(bufferRx_[1] == MODBUS_READ_HOLDING_REGISTERS_0x03)
          // Совпадает количество байт данных
          &&(bufferRx_[2] == refCnt*2)) {
        // Совпадает контрольная сумма
        if (((bufferRx_[Count - 1] << 8) + bufferRx_[Count - 2])
            == crc16_ibm(bufferRx_, (Count - 2))) {
          for(int I = 0; I <= refCnt; I++)
          {
            Value.tdChar[0] = bufferRx_[4 + 2*I];
            Value.tdChar[1] = bufferRx_[3 + 2*I];
            regArr[I] = Value.tdUint16[0];
          }
          Result = MODBUS_OK;
        }
        else
          Result = MODBUS_ERROR_CRC;
      }
      else {
        // Длина пакета равна длине с ошибкой
        if ((Count == MODBUS_MIN_LENGHT_PACKAGE)
            // Адрес совпадает
            &&(bufferRx_[0] == slaveAddr)
            // Код функции совпадает
            &&(bufferRx_[1] == MODBUS_READ_HOLDING_REGISTERS_0x03 + MODBUS_ERROR_0x80)
            &&((bufferRx_[2] >= MODBUS_ILLEGAL_FUNCTION_0x01)
               &&(bufferRx_[2] <= MODBUS_GATEWAY_TARGET_DEVICE_0x0B))) {
          if (((bufferRx_[Count - 1] << 8) + bufferRx_[Count - 2])
              == crc16_ibm(bufferRx_, (Count - 2))) {
            Result = bufferRx_[2];
          }
          else
            Result = MODBUS_ERROR_CRC;
        }
        else
          Result = MODBUS_ERROR_TRASH;
      }
    }
    else {
      Result = MODBUS_ERROR_TIMEOUT;
    }
  }
  return Result;
}

// МЕТОД ЧТЕНИЯ РЕГИСТРОВ УСТРОЙТВА
// Функция Modbus (03 Hex) для 32-разрядных типов данных Int,
// Пишет длинные десятичного значения в пар выходных регистров
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
// StartRef Первый регистр
// Int32Arr буфера с данными для отправки
// RefCnt Количество длинных целых чисел, которые будут отправлены (Диапазон: 1-50)
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::readMultipleLongInts(    int SlaveAddr,
                                                int StartRef,
                                                int *int32Arr,
                                                int RefCnt)
{
    int Result = RETURN_ERROR;
    int I = 0;                                                        // Счётчик, используется при формировании массива прочитанных данных
    unTypeData Value;                                                    // Переменна для формирования массива прочитанных данных
    unsigned short Crc = 0;                                            // Переменная для проверки CRC
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
                txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;       // Команды
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);               // Старший байт адреса первого регистра
                txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
                txBuffer_[4] = 0;                                        // Старший байт количества регистров
                txBuffer_[5] = (RefCnt * 2) & 0x00ff;                    // Младший байт количества регистров
                Crc = crc16_ibm(txBuffer_, 6);                           // Вычисляем контрольную сумму
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;                             // Младший байт контрольной суммы
                // ПЕРЕДАЧА И ПРИЁМ ДАННЫХ
                if(transmitQuery(txBuffer_, 8))
                {
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                for(I = 0; I <= RefCnt; I++)
                                {
                                    Value.tdChar[0] = txBuffer_[6 + 4*I];
                                    Value.tdChar[1] = txBuffer_[5 + 4*I];
                                    Value.tdChar[2] = txBuffer_[4 + 4*I];
                                    Value.tdChar[3] = txBuffer_[3 + 4*I];
                                    int32Arr[I] = Value.tdUint32;
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                    }
                }// Не смог отправить запрос
            }// Некорректное число регистров
        }// Некорректный адрес
    }
    catch(...)
    {
        // Формируем сообщение упали в функции readMultipleRegisters
    }
    return Result;
};

// МЕТОД ЧТЕНИЯ FLOAT РЕГИСТРОВ
// Modbus функция 3 для 32-битных типов данных с плавающей точкой
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
// StartRef Первый регистр
// Float32Arr буфер, который будет наполнен прочитаными данными
// RefCnt Количество поплавков значений, которые будут читать (диапазон: 1-62)
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::readMultipleFloats(        int SlaveAddr,
                                                  int StartRef,
                                                  float *Float32Arr,
                                                  int RefCnt)
{
    int Result = RETURN_ERROR;
    int I = 0;                                                        // Счётчик, используется при формировании массива прочитанных данных
    unTypeData Value;                                                   // Переменна для формирования массива прочитанных данных
    unsigned short Crc = 0;
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
                txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;       // Команды
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);               // Старший байт адреса первого регистра
                txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
                txBuffer_[4] = 0;                                        // Старший байт количества регистров
                txBuffer_[5] = (RefCnt * 2) & 0x00ff;                    // Младший байт количества регистров
                Crc = crc16_ibm(txBuffer_, 6);                           // Вычисляем контрольную сумму
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;                             // Младший байт контрольной суммы
                if(transmitQuery(txBuffer_, 8))
                {
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                for(I = 0; I <= RefCnt; I++)
                                {
                                    Value.tdChar[0] = txBuffer_[6 + 4*I];
                                    Value.tdChar[1] = txBuffer_[5 + 4*I];
                                    Value.tdChar[2] = txBuffer_[4 + 4*I];
                                    Value.tdChar[3] = txBuffer_[3 + 4*I];
                                    Float32Arr[I] = Value.tdFloat;
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                        }
                 }// Не смог отправить запрос
            }// Некорректное число регистров
        }// Некорректный адрес
    }
    catch(...)
    {
        // Формируем сообщение упали в функции readMultipleRegisters
    }
    return Result;
}

// МЕТОД ЧТЕНИЯ ВНУТРЕННИХ РЕГИСТРОВ УСТРОЙСТВА
// Modbus функция 4, Чтение входных регистров.
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства (диапазон: 1 - 255)
// StartRef Начните регистр
// RegArr буфера, который будет наполнен данными читать.
// RefCnt Количество считываемых регистров (Диапазон: 1-125)
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::readInputRegisters(      int SlaveAddr,
                                                int StartRef,
                                                short RegArr[],
                                                int RefCnt)
{
    int Result = RETURN_ERROR;
    int I = 0;                                                        // Счётчик, используется при формировании массива прочитанных данных
    unTypeData Value;                                                   // Переменна для формирования массива прочитанных данных
    unsigned short Crc = 0;
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
                txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;         // Команды
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);               // Старший байт адреса первого регистра
                txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
                txBuffer_[4] = 0;                                        // Старший байт количества регистров
                txBuffer_[5] = RefCnt & 0x00ff;                          // Младший байт количества регистров
                Crc = crc16_ibm(txBuffer_, 6);                           // Вычисляем контрольную сумму
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;                             // Младший байт контрольной суммы
                // ПЕРЕДАЧА И ПРИЁМ ДАННЫХ
                if(transmitQuery(txBuffer_, 8))
                {
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                for(I = 0; I <= RefCnt; I++)
                                {
                                    Value.tdChar[0] = txBuffer_[4 + 2*I];
                                    Value.tdChar[1] = txBuffer_[3 + 2*I];
                                    RegArr[I] = Value.tdUint16[0];
                                }
                                break;
                                // Получен ответ с ошибкой
                                case    RETURN_MODBUS_ERROR:

                                break;
                                // Ответ с ошибкой CRC
                                case    RETURN_MODBUS_ERROR_CRC:

                                break;
                                // Ответ не получен
                                case    RETURN_MODBUS_TIMEOUT:

                                break;
                                default:

                                break;
                    }
                }// Не смог отправить запрос
            }// Некорректное число регистров
        }// Некорректный адрес
    }
    catch(...)
    {
        // Формируем сообщение упали в функции readMultipleRegisters
    }
    return Result;
}

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
int ModbusMaster::readInputLongInts(    int SlaveAddr,
                                             int StartRef,
                                             int Int32Arr[],
                                             int RefCnt)
{
    int Result = RETURN_ERROR;
    unsigned short Crc = 0;
    int I = 0;                                                        // Счётчик, используется при формировании массива прочитанных данных
    unTypeData Value;                                                   // Переменна для формирования массива прочитанных данных
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
                txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;        // Команды
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);                // Старший байт адреса первого регистра
                txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
                txBuffer_[4] = 0;                                        // Старший байт количества регистров
                txBuffer_[5] = (RefCnt * 2) & 0x00ff;                    // Младший байт количества регистров
                Crc = crc16_ibm(txBuffer_, 6);                            // Вычисляем контрольную сумму
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;                            // Младший байт контрольной суммы
                // ПЕРЕДАЧА И ПРИЁМ ДАННЫХ
                if(transmitQuery(txBuffer_, 8))
                {
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                for(I = 0; I <= RefCnt; I++)
                                {
                                    Value.tdChar[0] = txBuffer_[6 + 4*I];
                                    Value.tdChar[1] = txBuffer_[5 + 4*I];
                                    Value.tdChar[2] = txBuffer_[4 + 4*I];
                                    Value.tdChar[3] = txBuffer_[3 + 4*I];
                                    Int32Arr[I] = Value.tdUint32;
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                    }
                }// Не смог отправить запрос
            }// Некорректное число регистров
        }// Некорректный адрес
    }
    catch(...)
    {
        // Формируем сообщение упали в функции readMultipleRegisters
    }
    return Result;
};

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
int ModbusMaster::readInputFloats(    int SlaveAddr,
                                              int StartRef,
                                              float Float32Arr[],
                                              int RefCnt)
{
    int Result = RETURN_ERROR;
    int I = 0;                                                        // Счётчик, используется при формировании массива прочитанных данных
    unTypeData Value;                                                   // Переменна для формирования массива прочитанных данных
    unsigned short Crc = 0;
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
                txBuffer_[1] = MODBUS_READ_INPUT_REGISTERS_0x04;        // Команды
                txBuffer_[2] = ((StartRef >> 8) & 0x00ff);                // Старший байт адреса первого регистра
                txBuffer_[3] = StartRef & 0x00ff;                        // Младший байт адреса первого регистра
                txBuffer_[4] = 0;                                        // Старший байт количества регистров
                txBuffer_[5] = (RefCnt * 2) & 0x00ff;                    // Младший байт количества регистров
                Crc = crc16_ibm(txBuffer_, 6);                            // Вычисляем контрольную сумму
                txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
                txBuffer_[7] = Crc & 0x00ff;                            // Младший байт контрольной суммы
                if(transmitQuery(txBuffer_, 8))
                {
                    Result = reseiveAnswer(txBuffer_);
                    // Анализируем ответ
                    switch(Result)
                    {
                        // Получен корректный ответ
                        case    RETURN_MODBUS_OK:
                                for(I = 0; I <= RefCnt; I++)
                                {
                                    Value.tdChar[0] = txBuffer_[6 + 4*I];
                                    Value.tdChar[1] = txBuffer_[5 + 4*I];
                                    Value.tdChar[2] = txBuffer_[4 + 4*I];
                                    Value.tdChar[3] = txBuffer_[3 + 4*I];
                                    Float32Arr[I] = Value.tdFloat;
                                }
                        break;
                        // Получен ответ с ошибкой
                        case    RETURN_MODBUS_ERROR:

                        break;
                        // Ответ с ошибкой CRC
                        case    RETURN_MODBUS_ERROR_CRC:

                        break;
                        // Ответ не получен
                        case    RETURN_MODBUS_TIMEOUT:

                        break;
                        default:

                        break;
                        }
                 }// Не смог отправить запрос
            }// Некорректное число регистров
        }// Некорректный адрес
    }
    catch(...)
    {
        // Формируем сообщение упали в функции readMultipleRegisters
    }
    return Result;
}

// МЕТОД ЗАПИСИ КАТУШКИ
// Modbus функция 5, Записать катушку.
// Параметры:
// SlaveAddr Modbus Адрес ведомого устройства (Диапазон: 0 - 255)
// BitAddr катушки адрес
// BitVal записываемое состояние
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::writeCoil(    int SlaveAddr,
                                    int BitAddr,
                                    int BitVal)
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
            txBuffer_[1] = MODBUS_WRITE_SINGLE_COIL_0x05;            // Команды
            txBuffer_[2] = ((BitAddr >> 8) & 0x00ff);                // Старший байт адреса катушки
            txBuffer_[3] = BitAddr & 0x00ff;                        // Младший байт адреса катушки
            if(BitVal)
            {
                txBuffer_[4] = 0xff;                                // Старший байт значения
                txBuffer_[5] = 0x00;                                // Младший байт значения
            }
            else
            {
                txBuffer_[4] = 0x00;                                // Старший байт значения
                txBuffer_[5] = 0x00;                                // Младший байт значения
            }
            Crc = crc16_ibm(txBuffer_, 6);                            // Вычисляем контрольную сумму
            txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
            txBuffer_[7] = Crc & 0x00ff;                            // Младший байт контрольной суммы
        }
    }
    catch(...)
    {

    }
    return Result;
}

// МЕТОД ЗАПИСИ РЕГИСТРА
// Modbus функция 6, Запись одного регистра.
// Записывает значение в один регистр выходной
// Параметры:
// SlaveAddr - Modbus Адрес ведомого устройства (Диапазон: 0 - 255)
// RegAddr - Адрес регистра (диапазон: 1 - 0x10000)
// Regval - данные для отправки
// Возвращает:
// RETURN_OK в случае успешного завершения или код ошибки
int ModbusMaster::writeSingleRegister(    int SlaveAddr,
                                            int RegAddr,
                                            short RegVal)
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
            txBuffer_[1] = MODBUS_WRITE_SINGLE_REGISTER_0x06;        // Команды
            txBuffer_[2] = ((RegAddr >> 8) & 0x00ff);                // Старший байт адреса катушки
            txBuffer_[3] = RegAddr & 0x00ff;                        // Младший байт адреса катушки
            txBuffer_[4] = ((RegVal >> 8) & 0x00ff);                // Старший байт значения
            txBuffer_[5] = RegVal & 0x00ff;                            // Младший байт значения
            Crc = crc16_ibm(txBuffer_, 6);                            // Вычисляем контрольную сумму
            txBuffer_[6] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
            txBuffer_[7] = Crc & 0x00ff;                            // Младший байт контрольной суммы
        }
    }
    catch(...)
    {

    }
    return Result;
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
            txBuffer_[2] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
            txBuffer_[3] = Crc & 0x00ff;                            // Младший байт контрольной суммы
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
            txBuffer_[Len + 5] = ((Crc >> 8) & 0x00ff);                    // Старший байт контрольной суммы
            txBuffer_[Len + 6] = Crc & 0x00ff;                            // Младший байт контрольной суммы
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
                    	// Старший байт данных
                        txBuffer_[I + 5] = ((BitArr[I] >> 8) & 0x00ff);
                        // Младший байт данных
                        txBuffer_[I + 6] = BitArr[I] & 0x00ff;
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
                    txBuffer_[I * 2 + 7] = ((RegArr[I] >> 8) & 0x00ff);    // Старший байт данных
                    txBuffer_[I * 2 + 8] = RegArr[I] & 0x00ff;            // Младший байт данных
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
            Arr.tdFloat = Float32Arr[I];
            RegArr[I * 4] = Arr.tdChar[0];
            RegArr[I * 4 + 1] = Arr.tdChar[1];
            RegArr[I * 4 + 2] = Arr.tdChar[2];
            RegArr[I * 4 + 3] = Arr.tdChar[3];
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
        if((Address > 1)&&(Address < 256))
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
    TotalCounter = 0;
};

// МЕТОД СБРОСА СЧЁТЧИКА ОТВЕТОВ НА ЗАПРОСЫ
//Сбрасывает значение счётчика корректных ответов в 0
void ModbusMaster::resetSuccessCounter()
{
    SuccessCounter = 0;
};

// МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВА ЗАПРОСОВ
// Вовзращает значение счётчика общего количества запросов
long ModbusMaster::getTotalCounter()
{
    return (TotalCounter);
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
    return (SuccessCounter);
};

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
int ModbusMaster::transmitQuery(unsigned char *Buf, unsigned char Count)
{
    return RETURN_ERROR;
}

// МЕТОД ЧТЕНИЕ ДАННЫХ ИЗ ПОРТА
// Buf - массив байт считываемый из порта
int ModbusMaster::reseiveAnswer(unsigned char *Buf)
{
    return RETURN_ERROR;
}



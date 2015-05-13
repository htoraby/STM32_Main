/*
 * modbus_master.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_MASTER_H_
#define MODBUS_MASTER_H_

#include "define.h"
#include "stdlib.h"
#include "board.h"

#define MODBUS_OK                             0
#define MODBUS_ERROR_TIMEOUT                  1
#define MODBUS_ERROR_CRC                      2
#define MODBUS_ERROR_TRASH                    3
// Максимальное время ожидания ответа от устройства в мс
#define MODBUS_ANSWER_TIMEOUT                 100
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

#define MODBUS_COUNTER_LOST_CONNECT           5

/*!
 * \brief Класс Modbus master
 * Релизует функции Modbus и некоторые другие функции
 */
class ModbusMaster
{
public:
  ModbusMaster();
  virtual ~ModbusMaster();

  /*! Функции реализующий протокол Modbus */
  /*! Битовые методы */
  /*!
   * \brief Функция чтения состояния катушек
   * Функция 1, считывание состояния катушек
   * \param SlaveAddr - modbus адрес ведомого устройства (диапазон: 1 - 255)
   * \param StartRef - первая катушка
   * \param BitArr - массив возвращаемых значений
   * \param RefCnt - количество считываемых катушек (Диапазон: 1-2000)
   * \return 0 - выполнено или код ошибки
   */
  int readCoils(int SlaveAddr, int StartRef, bool BitArr[], int RefCnt);

  /*!
   * \brief Функция чтения состояния дискретных входов
   * Modbus функция 2, Чтение дискретных входов
   * \param SlaveAddr - modbus адрес ведомого устройства (диапазон: 1 - 255)
   * \param StartRef - первый вход
   * \param BitArr - массив возвращаемых значений
   * \param RefCnt - количество считываемых входов (Диапазон: 1-2000)
   * \return 0 - выполнено или код ошибки
   */
  int readInputDiscretes(int SlaveAddr, int StartRef, bool BitArr[], int RefCnt);

  /*!
   * \brief Функция чтения регистров
   * Modbus функция 3,  Читать нескольких регистров.
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef  - начните регистр (диапазон: 1 - 0x10000)
   * \param regArr - буфера, который будет наполнен прочитанныйми данными
   * \param refCnt - количество считываемых регистров
   * \return код результата операции
   */
  uint8_t readMultipleRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Функция Modbus (03 Hex) для 32-разрядных типов данных Int
   * \param slaveAddr - modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
   * \param startRef - Первый регистр
   * \param int32Arr - буфера с данными
   * \param refCnt - количество длинных целых регистров
   * \return 0 - выполнено или код ошибки
   */
  int readMultipleLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Modbus функция 3 для 32-битных типов данных с плавающей точкой
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первый регистр
   * \param float32Arr - буфер, который будет наполнен прочитаными данными
   * \param refCnt - количество поплавков значений, которые будут читать (диапазон: 1-62)
   * \return 0 - выполнено или код ошибки
   */
  int readMultipleFloats(int slaveAddr, int startRef, float float32Arr[], int refCnt);

  /*!
   * \brief Функция чтения регистров
   * Modbus функция 4,  Читать нескольких регистров.
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - начните регистр (диапазон: 1 - 0x10000)
   * \param regArr - буфера, который будет наполнен прочитанныйми данными
   * \param refCnt - количество считываемых регистров
   * \return 0 - выполнено или код ошибки
   */
  int readInputRegisters(int slaveAddr, int startRef, short regArr[], int refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Функция Modbus (04 Hex) для 32-разрядных типов данных Int
   * \param slaveAddr - modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
   * \param startRef - Первый регистр
   * \param int32Arr - буфера с данными
   * \param refCnt - количество длинных целых регистров
   * \return 0 - выполнено или код ошибки
   */
  int readInputLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Modbus функция 3 для 32-битных типов данных с плавающей точкой
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первый регистр
   * \param float32Arr - буфер, который будет наполнен прочитаными данными
   * \param refCnt - количество поплавков значений, которые будут читать (диапазон: 1-62)
   * \return 0 - выполнено или код ошибки
   */
  int readInputFloats(int slaveAddr, int startRef, float float32Arr[], int refCnt);

  /*!
   * \brief Функция записи катушки
   * modbus функция 5, Записать катушку.
   * \param slaveAddr - modbus Адрес ведомого устройства (Диапазон: 0 - 255)
   * \param bitAddr - катушки адрес
   * \param bitVal - записываемое состояние
   * \return 0 - выполнено или код ошибки
   */
  int writeCoil(int slaveAddr, int bitAddr, int bitVal);

  /*!
   * \brief Функция записи значения в регистр
   * Modbus функция 6, Запись одного регистра. Записывает значение в один регистр выходной
   * \param slaveAddr -modbus Адрес ведомого устройства (Диапазон: 0 - 255)
   * \param regAddr - адрес регистра (диапазон: 1 - 0x10000)
   * \param regVal - данные для отправки
   * \return 0 - выполнено или код ошибки
   */
  int writeSingleRegister(int slaveAddr, int regAddr, short regVal);

  /*!
   * \brief Функция чтения исключения
   * Modbus функция 7 чтение cтатуса исключений. Считывание состояния катушки
   * исключение внутри ведомого устройства.
   * \param slaveAddr - modbus Адрес ведомого устройства
   * \param statusBytePtr - указатель на буфер
   * \return 0 - выполнено или код ошибки
   */
  int readExceptionStatus(int slaveAddr, unsigned char *statusBytePtr);

  /*!
   * \brief Функция эхо-запроса
   * Функция Modbus код 8, подфункция 00, Вернуться Запрос данных.
   * \param slaveAddr - modbus Адрес ведомого устройства или идентификатор единицы (диапазон: 1 - 255)
   * \param queryArr - буфер с данными для отправки
   * \param echoArr - буфер, который будет содержать считывать данные
   * \param len - количество байт Написать отправлено и читать назад
   * \return 0 - выполнено или код ошибки
   */
  int returnQueryData(int slaveAddr, const unsigned char queryArr[], unsigned char echoArr[], int len);

  /*!
   * \brief Функция сброса настрок опроса
   * Функция Modbus код 8, подфункция 01, сброс настроек
   * \param slaveAddr - modbus Адрес ведомого устройства  (диапазон: 1 - 255)
   * \param clearEventLog - Флаг
   * \return 0 - выполнено или код ошибки
   */
  int restartCommunicationsOption(int slaveAddr, int clearEventLog);

  /*!
   * \brief Функция записи катушек
   * Modbus функция 15 (0F Hex), Воздействие на несколько катушек, записывает
   * двоичные значения в последовательность дискретных выходов или катушек
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первая катушка
   * \param bitArr - буфер, который содержит отправляемые данные
   * \param refCnt - количество катушек, которые будут записаны (Диапазон: 1-1968)
   * \return 0 - выполнено или код ошибки
   */
  int forceMultipleCoils(int slaveAddr, int startRef, const bool bitArr[], int refCnt);

  int writeMultipleRegisters(int slaveAddr, int startRef, short *regArr, int refCnt);

  int writeMultipleLongInts(int slaveAddr, int startRef, int *int32Arr, int refCnt);

  int writeMultipleFloats(int slaveAddr, int startRef, float *float32Arr, int refCnt);

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

  /*!
   * \brief Метод увеличения счётчика запросов
   */
  void incTotalCounter();

  // МЕТОД ПОЛУЧЕНИЯ ОБЩЕГО КОЛИЧЕСТВ ОТВЕТОВ НА ЗАПРОСЫ
  // Вовзращает значение счётчика корректных ответов на запросы
  long getSuccessCounter();

  /*!
   * \brief Метод увеличения счётчика принятых ответов
   */
  void incSuccessCounter();

  // МЕТОД СБРОСА СЧЁТЧИКА ОТВЕТОВ НА ЗАПРОСЫ
  //Сбрасывает значение счётчика корректных ответов в 0
  void resetSuccessCounter();

  /*!
   * \brief Метод увеличения счётчика принятых пакетов
   */
  void incLostCounter();

  /*!
   * \brief Метод сброса счётчика потерянных пакетов
   */
  void resetLostCounter();

  /*!
   * \brief getLostCounter
   * \return
   */
  int getLostCounter();

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

  /*!
   * \brief Функция записи данных
   * \param buf - массив байт записываемый
   * \param num - количество байт
   * \return код выполнения операции
   */
  virtual uint8_t txBuf(uint8_t *buf, uint8_t num);

  /*!
   * \brief Функция чтения данных из порта
   * \param buf - массив принятых байт
   * \param num - ожидаемое количество байт
   * \return код выполнения операции
   */
  virtual uint8_t rxBuf(uint8_t *buf, uint8_t num);

  ///////////////////////////////////////////////////////////////////////////
  // ЗАЩИЩЕННЫЕ ЧЛЕНЫ КЛАССА
  ///////////////////////////////////////////////////////////////////////////
protected:
  unsigned long totalCounter_;              /// Общий счётчик запросов
  unsigned long successCounter_;            /// Общий счётчик корректных ответов
  unsigned long lostCounter_;               /// Счётчик потерянных пакетов

  // Число автоматических повторений запроса
  int retryCnt_;
  // Время ожидания ответа
  int timeOut_;
  // Время между запросами
  int pollDelay_;
  // Порядок байт
  int endian_;

  /*!
   * \brief Функция проверки корректности адреса устройства
   * \param address - проверяемый адрес
   * \return true - корректный адрес, false - не корректный
   */
  bool checkDeviceAddress(int address);

  // МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА КАТУШЕК
  bool checkCntCoils(int Cnt);

  // МЕТОД ПРОВЕРКИ КОЛИЧЕСТВА РЕГИСТРОВ
  bool checkCntReg(int Cnt);

  unsigned char txBuffer_[MODBUS_MAX_DATA_SIZE];
  unsigned char rxBuffer_[MODBUS_MAX_DATA_SIZE];

  ///////////////////////////////////////////////////////////////////////////
  // ЗАКРЫТЫЕ ЧЛЕНЫ КЛАССА
  ///////////////////////////////////////////////////////////////////////////
private:

};

#endif /* MODBUS_MASTER_H_ */

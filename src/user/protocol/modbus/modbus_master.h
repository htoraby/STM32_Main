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

#define MODBUS_ANSWER_TIMEOUT                 100     //!< Максимальное время ожидания ответа от устройства в мс
#define MODBUS_TIME_END_PACKAGE               20      //!< Максимальное время между байтами в ответе в мс
#define MODBUS_MIN_LENGHT_PACKAGE             5       //!< Минимальная возможная длина "корректного" ответа по Modbus

#define MODBUS_PDU_SIZE                       253     //!< Длина modbus запроса по спецификации
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

#define MODBUS_COUNTER_LOST_CONNECT           50



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
   * \param slaveAddr - modbus адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первая катушка
   * \param bitArr - массив возвращаемых значений
   * \param refCnt - количество считываемых катушек (Диапазон: 1-2000)
   * \return 0 - выполнено или код ошибки
   */
  uint8_t readCoils(uint8_t slaveAddr, uint16_t startRef, bool *bitArr, uint16_t refCnt);

  /*!
   * \brief Функция чтения состояния дискретных входов
   * Modbus функция 2, Чтение дискретных входов
   * \param SlaveAddr - modbus адрес ведомого устройства (диапазон: 1 - 255)
   * \param StartRef - первый вход
   * \param BitArr - массив возвращаемых значений
   * \param RefCnt - количество считываемых входов (Диапазон: 1-2000)
   * \return 0 - выполнено или код ошибки
   */
  int readInputDiscretes(uint8_t SlaveAddr, uint16_t StartRef, bool BitArr[], uint16_t RefCnt);

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
  int readMultipleLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Modbus функция 3 для 32-битных типов данных с плавающей точкой
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первый регистр
   * \param float32Arr - буфер, который будет наполнен прочитаными данными
   * \param refCnt - количество поплавков значений, которые будут читать (диапазон: 1-62)
   * \return 0 - выполнено или код ошибки
   */
  int readMultipleFloats(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt);

  /*!
   * \brief Функция чтения регистров
   * Modbus функция 4,  Читать нескольких регистров.
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - начните регистр (диапазон: 1 - 0x10000)
   * \param regArr - буфера, который будет наполнен прочитанныйми данными
   * \param refCnt - количество считываемых регистров
   * \return 0 - выполнено или код ошибки
   */
  uint8_t readInputRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Функция Modbus (04 Hex) для 32-разрядных типов данных Int
   * \param slaveAddr - modbus Адрес ведомого устройства или идентификатор единицы (Диапазон: 0 - 255)
   * \param startRef - Первый регистр
   * \param int32Arr - буфера с данными
   * \param refCnt - количество длинных целых регистров
   * \return 0 - выполнено или код ошибки
   */
  int readInputLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief Функция чтения регистров 32 битных регистров
   * Modbus функция 3 для 32-битных типов данных с плавающей точкой
   * \param slaveAddr - modbus Адрес ведомого устройства (диапазон: 1 - 255)
   * \param startRef - первый регистр
   * \param float32Arr - буфер, который будет наполнен прочитаными данными
   * \param refCnt - количество поплавков значений, которые будут читать (диапазон: 1-62)
   * \return 0 - выполнено или код ошибки
   */
  int readInputFloats(uint8_t slaveAddr, uint16_t startRef, float float32Arr[], uint16_t refCnt);

  /*!
   * \brief Функция записи катушки
   * modbus функция 5, Записать катушку.
   * \param slaveAddr - modbus Адрес ведомого устройства (Диапазон: 0 - 255)
   * \param bitAddr - катушки адрес
   * \param bitVal - записываемое состояние
   * \return 0 - выполнено или код ошибки
   */
  int writeCoil(uint8_t slaveAddr, int bitAddr, int bitVal);

  /*!
   * \brief Функция записи значения в регистр
   * Modbus функция 6, Запись одного регистра. Записывает значение в один регистр выходной
   * \param slaveAddr -modbus Адрес ведомого устройства (Диапазон: 0 - 255)
   * \param regAddr - адрес регистра (диапазон: 1 - 0x10000)
   * \param regVal - данные для отправки
   * \return 0 - выполнено или код ошибки
   */
  int writeSingleRegister(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal);

  /*!
   * \brief Функция чтения исключения
   * Modbus функция 7 чтение cтатуса исключений. Считывание состояния катушки
   * исключение внутри ведомого устройства.
   * \param slaveAddr - modbus Адрес ведомого устройства
   * \param statusBytePtr - указатель на буфер
   * \return 0 - выполнено или код ошибки
   */
  int readExceptionStatus(uint8_t slaveAddr, unsigned char *statusBytePtr);

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
  int forceMultipleCoils(uint8_t slaveAddr, uint16_t startRef, const bool bitArr[], uint16_t refCnt);

  /*!
   * \brief writeMultipleRegisters
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   * \return
   */
  int writeMultipleRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief writeMultipleLongInts
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   * \return
   */
  int writeMultipleLongInts(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief writeMultipleFloats
   * \param slaveAddr
   * \param startRef
   * \param float32Arr
   * \param refCnt
   * \return
   */
  int writeMultipleFloats(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt);

  /*!
   * \brief Функция задания времени ожидания ответа на запрос
   * \param time - значение  в мс (диапазон: 1 - 100000)
   * \return код выполнения операции
   */
  int setTimeout(uint32_t time);

  /*!
   * \brief Функция получения текущего значения времени ожидания ответа на запрос
   * \return текущее значение
   */
  uint32_t getTimeout();

  /*!
   * \brief Функция задания количества автоматических повторов запроса в случае ошибки
   * \param retry - количество запросов (диапазон: 0 - 10)
   * \return код выполнения операции
   */
  int setRetryCnt(int retry);

  /*!
   * \brief Функция получения текущего значения автоматических повторов
   * \return текущее значение
   */
  int getRetryCnt();

  stConnectQuality *getCounters();
  void resetCounters();

  void configureBigEndianInts();

  void configureLittleEndianInts();

  void closeProtocol();

  // МЕТОД ПРОВЕРКИ ОТКРЫТИЯ ПРОТОКОЛА
  int isOpen();

  /*!
   * \brief Функция проверки наличия связи с устройством
   * \return true - связь есть; false - связи нет
   */
  bool isConnect();

  /*!
   * \brief Функция вычисления качества связи с устройством
   */
  void calcConnect();

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
  stConnectQuality counters_;

  int retryCnt_;                            //!< Число автоматических повторений запроса
  uint32_t timeOut_;                        //!< Время ожидания ответа
  int pollDelay_;                           //!< Время между запросами
  int endian_;                              //!< Порядок байт

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

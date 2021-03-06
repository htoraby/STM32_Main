﻿/*
 * device_modbus.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef DEVICE_MODBUS_H_
#define DEVICE_MODBUS_H_

#include "device.h"
#include "define.h"
#include "modbus_master_serial.h"

//! Структура для хранения полей параметра из карты устройства
struct ModbusParameter
{  
  uint16_t id;           //!< Уникальный идентификатор параметра
  uint16_t address;      //!< Адрес регистра в устройстве
  uint8_t operation;     //!< Операции с параметром
  uint8_t physic;        //!< Физическая величина параметра
  uint8_t unit;          //!< Единицы измерения параметра
  uint8_t typeData;      //!< Тип данных
  uint8_t index;         //!< Индекс для массивного параметра
  float coefficient;     //!< Коэффициент преобразования параметра
  float min;             //!< Минимальное значение параметра
  //TODO: Может лучше минимум и максимум сделать uint32?
  float max;             //!< Максимально значение параметра
  float def;             //!< Значение по умолчанию
  uint16_t freqExchange; //!< Количество раз во сколько циклов нужно опрашивать параметр
  uint16_t cntExchange;  //!< Количество циклов когда проверяли нужно ли опрашивать параметр
  uint8_t command;       //!< Команда читать или писать
  uint8_t validity;      //!< Флаг получено ли значение параметра
  unTypeData value;      //!< Значение, записываемое или считанное
};

/*!
 * \class DeviceModbus
 * \brief Класс для устройств работающих по протоколу Modbus.
 * Содержит функции организации цикла чтения и записи параметров устройства
*/ 
class DeviceModbus
{
public:
  /*!
   * \brief Конструктор класса
   *
   * \param modbusParameters - указатель на карту регистров
   * \param countParameter Количество регистров в массиве
   * \param numPort - номер имя порта uart
   * \param baudRate - скорость обмена
   * \param dataBits - количество бит данных
   * \param stopBits - количество стоп бит
   * \param parity - чётность
   * \param address - адресс устройства
   */
  DeviceModbus(ModbusParameter *modbusParameters,
               int countParameter,
               int numPort,
               long baudRate,
               int dataBits,
               int stopBits,
               int parity,
               int address);
  virtual ~DeviceModbus();

  /*!
   * \brief Метод создания задачи
   * \param threadName - имя задачи
   * \param getValueDeviceQId - Очередь параметров со значениями от устройства
   */
  void createThread(const char *threadName, osMessageQId getValueDeviceQId);

  void setAddrIndexReg(uint16_t addr);

  // Базовые методы работы со структурой ModbusParameter
  /*!
   * \brief Метод получения ID параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле ID
   */
  int getFieldID(int index);

  /*!
   * \brief Метод получения Address параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Address
   */
  int getFieldAddress(int index);

  /*!
   * \brief Метод получения Operation параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Operation
   */
  int getFieldOperation(int index);

  /*!
   * \brief Метод получения Physic параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Physic
   */
  int getFieldPhysic(int index);

  /*!
   * \brief Метод получения Unit параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Unit
   */
  int getFieldUnit(int index);

  /*!
   * \brief Метод получения TypeData параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле TypeData
   */
  int getFieldTypeData(int index);

  int getFieldIndex(int index);

  /*!
   * \brief Метод получения Coefficient параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Coefficient
   */
  float getFieldCoefficient(int index);

  /*!
   * \brief Метод получения Minimum параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Minimum
   */
  float getFieldMinimum(int index);

  /*!
   * \brief Метод получения Maximum параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Maximum
   */
  float getFieldMaximum(int index);

  float getFieldDefault(int index);

  /*!
   * \brief Метод получения FreqExchange параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле FreqExchange
   */
  int getFieldFreqExchange(int index);

  /*!
   * \brief Метод получения CntExchange параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле CntExchange
   */
  int getFieldCntExchange(int index);

  /*!
   * \brief Метод задания CntExchange параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле CntExchange
   */
  int setFieldCntExchange(int index, int cntExchange);

  /*!
   * \brief Метод получения Command параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Command
   */
  int getFieldCommand(int index);

  /*!
   * \brief Метод присвоения Command параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \param Command Присваиваемая команда
   */
  int setFieldCommand(int index, int command);

    /*!
   * \brief Метод получения Validity параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Validity
   */
  int getFieldValidity(int index);

  /*!
   * \brief Метод задания Validity параметра по индексу в массиве
   * \param index - индекс параметра в массиве
   * \param значение validity
   * \return
   */
  int setFieldValidity(int index, int validity);

  /*!
   * \brief Метод получения Value параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return поле Value
   */
  unTypeData getFieldValue(int index);


  /*!
   * \brief Метод получения указателя на параметр по индексу
   * \param index - индекс параметра в массиве
   * \return Указатель
   */
  ModbusParameter *getFieldAll(int index);

  /*!
   * \brief Метод поиска и получения индекса по ID параметра
   * \param ID идентификатор параметра
   * \return Index
  */
  int getIndexAtId(int id);

  /*!
   * \brief Метод поиска и получения индекса по Address параметра
   * \param address - идентификатор параметра
   * \return Index
  */
  int getIndexAtAddress(int address, int typeData, uint8_t index = 0);

  /*!
   * \brief Метод записи параметра в устройство
   * \param ID идентификатор параметра
   * \param Value значение
   * \param operation
  */
  void writeModbusParameter(int id, float value, enOperation operation = OPERATION_WRITE);

  /*!
   * \brief readModbusParameter
   * \param id
   */
  void readModbusParameter(int id);

  // Методы для работы цикла опроса и записи параметров
  /*!
   \brief Метод циклического опроса устройства по Modbus и/или внеочередной
    записи и/или чтения параметра
  */
  void exchangeTask();

  /*!
   * \brief putTurn метод добавления элемента в очередь
   * \param Index Индекс параметра
   * \return добавили элемент в очередь или нет
   */
  int getMessageOutOfTurn();

  /*!
    * \brief putTurn метод добавления элемента в очередь
    * \param Index Индекс параметра
    * \return добавили элемент в очередь или нет
    */
  int putMessageOutOfTurn(int element);

  /*!
     * \brief getMessageReadyParam получить элемент
     * из очереди готовых параметров
     * \return 0 - нет элементов или элемент
     */
  int getMessageReadyParam();

  /*!
     * \brief putMessagereadyParam положить элемент
     * в очередь готовых параметров
     * \param Element - элемент
     * \return
     */
  int putMessageUpdateId(int id);

  int searchExchangeParameters();

  bool isConnect();

  ModbusMasterSerial * getMms() const { return mms_; }

  bool bitArr_[125]; 
  uint16_t uint16Arr_[125];
  uint32_t uint32Arr_[62];
  float floatArr_[62];

private:
  /*!
   * \brief Метод получения адреса устройства
   * \return Адрес устройства
  */
  int getDeviceAddress();

  /*!
   * \brief Метод записи адреса устройства
   * \param Адрес устройства
  */
  void setDeviceAddress(int address);

  /*!
   * \brief readCoils
   * \param slaveAddr
   * \param startRef
   * \param bitArr
   * \param refCnt
   */
  void readCoils(uint8_t slaveAddr, uint16_t startRef, bool *bitArr, uint16_t refCnt);

  /*!
   * \brief readInt16Registers
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   */
  void readInt16Registers(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief readInt16InputRegisters
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   */
  void readInt16InputRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief readUint16Registers
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   */
  void readUint16Registers(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief readUint16InputRegisters
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   */
  void readUint16InputRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief readInt32Registers
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   */
  void readInt32Registers(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief readUint32Registers
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   */
  void readUint32Registers(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief readUint32InputRegisters
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   */
  void readUint32InputRegisters(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief readFloatsRegisters
   * \param slaveAddr
   * \param startRef
   * \param float32Arr
   * \param refCnt
   */
  void readFloatsRegisters(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt);

  /*!
   * \brief readArrayInt16Registers
   * \param slaveAddr
   * \param startRef
   * \param indexArray
   * \param regArr
   * \param refCnt
   */
  void readArrayInt16Registers(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief readArrayInt32Registers
   * \param slaveAddr
   * \param startRef
   * \param indexArray
   * \param int32Arr
   * \param refCnt
   */
  void readArrayInt32Registers(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief readStrRegisters
   * \param slaveAddr
   * \param startRef
   * \param regArr
   * \param refCnt
   */
  void readStrRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt);

  /*!
   * \brief writeCoil
   * \param slaveAddr
   * \param bitAddr
   * \param bitVal
   */
  void writeCoil(uint8_t slaveAddr, int bitAddr, int bitVal);

  /*!
   * \brief writeInt16Register
   * \param slaveAddr
   * \param regAddr
   * \param regVal
   */
  void writeInt16Register(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal);

  /*!
   * \brief writeInt16Register
   * \param slaveAddr
   * \param regAddr
   * \param regVal
   */
  void writeUint16Register(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal);

  /*!
   * \brief writeInt32Register
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   */
  void writeInt32Register(uint8_t slaveAddr, uint16_t startRef, int32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief writeUint32Register
   * \param slaveAddr
   * \param startRef
   * \param int32Arr
   * \param refCnt
   */
  void writeUint32Register(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt);

  /*!
   * \brief writeFloatRegister
   * \param slaveAddr
   * \param startRef
   * \param float32Arr
   * \param refCnt
   */
  void writeFloatRegister(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt);

  /*!
   * \brief writeInt16Register
   * \param slaveAddr
   * \param regAddr
   * \param regVal
   */
  void writeArrayInt16Register(uint8_t slaveAddr, uint16_t regAddr, uint8_t indexArray, uint16_t regVal);

  /*!
   * \brief writeArrayInt32Register
   * \param slaveAddr
   * \param startRef
   * \param indexArray
   * \param int32Arr
   * \param refCnt
   */
  void writeArrayInt32Register(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, int32_t *int32Arr, uint16_t refCnt);

  //! Указатель на массив параметров устройства
  ModbusParameter *mbParams_;
  //! Количество параметров в массиве
  uint16_t countParameter_;

  int numPort_;
  uint8_t devAdrs_;
  int indexExchange_;

  uint16_t addrIndexReg_;

  //! Идентификатор задачи
  osThreadId threadId_;
  //! Идентификатор очереди параметров опроса вне основного цикла
  osMessageQId messageOutOfTurn_;
  //! Идентификатор очереди параметров с новыми значениями от устройства
  osMessageQId getValueDeviceQId_;

  ModbusMasterSerial *mms_;

  bool prevConnect;

};

#endif /* DEVICE_MODBUS_H_ */

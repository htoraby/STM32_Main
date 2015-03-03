/*
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

/// Структура для хранения полей параметра из карты устройства
struct ModbusParameter
{  
  int ID;                       /// Уникальный идентификатор параметра
  int Address;                  /// Адрес регистра в устройстве
  int Operation;                /// Операции с параметром
  int Physic;                   /// Физическая величина параметра
  int Unit;                     /// Единицы измерения параметра
  int TypeData;                 /// Тип данных
  float Coefficient;            /// Коэффициент преобразования параметра
  float Minimum;                /// Минимальное значение параметра
  float Maximum;                /// Максимально значение параметра
  float Default;                /// Значение по умолчанию
  int FreqExchange;             /// Количество раз во сколько циклов нужно опрашивать параметр
  int CntExchange;              /// Количество циклов когда проверяли нужно ли опрашивать параметр
  int Command;                  /// Команда читать или писать
  int Validity;                 /// Флаг получено ли значение параметра
  unTypeData Value;             /// Значение, записываемое или считанное
};

/*!
 \class DeviceModbus
 \brief Класс для устройств работающих по протоколу Modbus.
  Содержит функции организации цикла чтения и записи параметров устройства
*/ 
class DeviceModbus
{
public:
  /*!
   * \brief DeviceModbus
   * Конструктор класса
   * \param MapRegisters - указатель на карту регистров
   * \param Quantity Длина массива карты регистров
   * \param PortName - номер имя порта uart
   * \param BaudRate - скорость обмена
   * \param DataBits - количество бит данных
   * \param StopBits - количество стоп бит
   * \param Parity - чётность
   * \param DeviceAddress - адресс устройства
   * \return Код результата операции
   */
  DeviceModbus(ModbusParameter *MapRegisters,
               int Quantity,
               int PortName,
               long BaudRate,
               int DataBits,
               int StopBits,
               int Parity,
               int Address,
               const char *threadName,
               osMessageQId messageUpdateID);
  /// Деструктор по умолчанию
  virtual ~DeviceModbus();

  // Базовые методы работы со структурой ModbusParameter
  /*!
   * \brief Метод получения ID параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле ID
   */
  int getFieldID(int Index);

  /*!
   * \brief Метод получения Address параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Address
   */
  int getFieldAddress(int Index);

  /*!
   * \brief Метод получения Operation параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Operation
   */
  int getFieldOperation(int Index);

  /*!
   * \brief Метод получения Physic параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Physic
   */
  int getFieldPhysic(int Index);

  /*!
   * \brief Метод получения Unit параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Unit
   */
  int getFieldUnit(int Index);

  /*!
   * \brief Метод получения TypeData параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле TypeData
   */
  int getFieldTypeData(int Index);

  /*!
   * \brief Метод получения Coefficient параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Coefficient
   */
  float getFieldCoefficient(int Index);

  /*!
   * \brief Метод получения Minimum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Minimum
   */
  float getFieldMinimum(int Index);

  /*!
   * \brief Метод получения Maximum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Maximum
   */
  float getFieldMaximum(int Index);

  float getFieldDefault(int Index);

  /*!
   * \brief Метод получения FreqExchange параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле FreqExchange
   */
  int getFieldFreqExchange(int Index);

  /*!
   * \brief Метод получения CntExchange параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле CntExchange
   */
  int getFieldCntExchange(int Index);

  /*!
   * \brief Метод задания CntExchange параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле CntExchange
   */
  int setFieldCntExchange(int Index, int CntExchange);

  /*!
   * \brief Метод получения Command параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Command
   */
  int getFieldCommand(int Index);

  /*!
   * \brief Метод присвоения Command параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \param Command Присваиваемая команда
   */
  int setFieldCommand(int Index, int Command);

    /*!
   * \brief Метод получения Validity параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Validity
   */
  int getFieldValidity(int Index);

  /*!
   * \brief Метод задания Validity параметра по индексу в массиве
   * \param Index индекс параметра в массиве
   * \param значение Validity
   * \return
   */
  int setFieldValidity(int Index, int Validity);

  /*!
   * \brief Метод получения Value параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Value
   */
  unTypeData getFieldValue(int Index);


  /*!
   * \brief Метод получения указателя на параметр по индексу
   * \param Index индекс параметра в массиве
   * \return Указатель
   */
  ModbusParameter *getFieldAll(int Index);

  /*!
   * \brief Метод поиска и получения индекса по ID параметра
   * \param ID идентификатор параметра
   * \return Index
  */
  int getIndexAtID(int ID);

  /*!
   * \brief Метод поиска и получения индекса по Address параметра
   * \param Address идентификатор параметра
   * \return Index
  */
  int getIndexAtAddress(int Address);

  /*!
   * \brief Метод записи параметра в устройство
   * \param ID идентификатор параметра
   * \param Value значение
  */
  void writeModbusParameter(int ID, float Value);

  // Методы для работы цикла опроса и записи параметров
  /*!
   \brief Метод циклического опроса устройства по Modbus и/или внеочередной
    записи и/или чтения параметра
  */
  void exchangeTask(void);

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
  int putMessageOutOfTurn(int Element);


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
  int putMessageUpdateID(int id);

  int searchExchangeParameters();

  short int regArr_[125];
  int int32Arr_[62];
  float float32Arr_[62];

private:
  int quantityParam_;
  int deviceAddress_;
  int indexExchange_;

  // Идентификатор задачи
  osThreadId threadId_;

  // Идентификатор очереди параметров опроса вне основного цикла
  osMessageQId messageOutOfTurn_;

  osMessageQId messageUpdateID_;

  ModbusParameter *modbusParameters_;

  /*!
   * \brief Метод получения количества регистров в карте регистров
   * \return Количество врегистров
  */
  int getQuantityParam();

  /*!
   * \brief Метод записи количества регистров в карте регистров
   * \param Quantity количество регистров
  */
  void setQuantityParam(int Quantity);

  /*!
   * \brief Метод получения адреса устройства
   * \return Адрес устройства
  */
  int getDeviceAddress();

  /*!
   * \brief Метод записи адреса устройства
   * \param Адрес устройства
  */
  void setDeviceAddress(int Address);

  ModbusMasterSerial *mms_;

};

#endif /* DEVICE_MODBUS_H_ */

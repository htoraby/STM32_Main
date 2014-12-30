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
  // Уникальный идентификатор параметра
  int ID;
  // Адрес регистра в устройстве
  int Address;
  // Операции с параметром
  int Operation;
  // Физическая величина параметра
  int Physic;
  // Единицы измерения параметра
  int Unit;
  // Поле состояния параметра
  int Validity;
  // Тип данных
  int TypeData;
  // Масштаб (коэффициент для приведения к физической единице измерения)
  float Scale;
  // Коэффициент преобразования параметра
  float Coefficient;
  // Минимальное значение параметра
  float Minimum;
  // Максимально значение параметра
  float Maximum;
  // Частота опроса параметра
  int FreqExchange;
  // Количество запросов к параметру
  int CntExchange;
  // Приоритет
  int Priority;
  // Флаг
  int Flag;
  // Считываемое значение
  unTypeData Value;
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
                osMessageQId *messageUpdateID);
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
   * \brief Метод получения Validity параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Validity
   */
  int getFieldValidity(int Index);

  /*!
   * \brief Метод получения TypeData параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле TypeData
   */
  int getFieldTypeData(int Index);

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
   * \brief Метод получения Priority параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Priority
   */
  int getFieldPriority(int Index);

  /*!
   * \brief Метод присвоения Priority параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \param Priority Присваиваемый приоритет
   */
  void setFieldPriority(int Index, int Priority);

  /*!
   * \brief Метод получения Flag параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Flag
   */
  int getFieldFlag(int Index);

  /*!
   * \brief Метод присвоения Flag параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \param Flag Присваиваемый флаг
   */
  void setFieldFlag(int Index, int Flag);

  /*!
   * \brief Метод получения Scale параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Scale
   */
  float getFieldScale(int Index);

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

  /*!
   * \brief Метод получения Value параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Value
   */
  unTypeData getFieldValue(int Index);

  /*!
   * \brief Метод получения параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return все поля
   */
  ModbusParameter getFieldAll(int Index);

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
  void exchangeCycle(void);

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
    int putMessageUpdateID(int Element);

   int searchExchangeParameters();

   short int regArr_[125];
   int int32Arr_[62];
   float float32Arr_[62];

private:
   int quantityParam_;
   int deviceAddress_;

   // Идентификатор задачи
   osThreadId threadId_;

   // Идентификатор очереди параметров опроса вне основного цикла
   osMessageQId messageOutOfTurn_;

   osMessageQId messageUpdateID_;

   ModbusParameter modbusParameters_[];

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

  ModbusMasterSerial *MMS;

};

#endif /* DEVICE_MODBUS_H_ */

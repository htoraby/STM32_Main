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

//! Структура для хранения полей параметра из карты устройства
struct ModbusParameter
{  
  int id;              //!< Уникальный идентификатор параметра
  int address;         //!< Адрес регистра в устройстве
  int operation;       //!< Операции с параметром
  int physic;          //!< Физическая величина параметра
  int unit;            //!< Единицы измерения параметра
  int typeData;        //!< Тип данных
  float coefficient;   //!< Коэффициент преобразования параметра
  float min;           //!< Минимальное значение параметра
  float max;           //!< Максимально значение параметра
  float def;           //!< Значение по умолчанию
  int freqExchange;    //!< Количество раз во сколько циклов нужно опрашивать параметр
  int cntExchange;     //!< Количество циклов когда проверяли нужно ли опрашивать параметр
  int command;         //!< Команда читать или писать
  int validity;        //!< Флаг получено ли значение параметра
  unTypeData value;    //!< Значение, записываемое или считанное
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
   * \param portName - номер имя порта uart
   * \param baudRate - скорость обмена
   * \param dataBits - количество бит данных
   * \param stopBits - количество стоп бит
   * \param parity - чётность
   * \param address - адресс устройства
   */
  DeviceModbus(ModbusParameter *modbusParameters,
               int countParameter,
               int portName,
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
   * \param значение Validity
   * \return
   */
  int setFieldValidity(int Index, int Validity);

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
  int getIndexAtAddress(int address);

  /*!
   * \brief Метод записи параметра в устройство
   * \param ID идентификатор параметра
   * \param Value значение
  */
  void writeModbusParameter(int id, float value);

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

  /*!
   * \brief Метод проверяющий наличие связи с устройством
   * \return
   */
  bool isConnect();

  uint16_t regArr_[125];
  int int32Arr_[62];
  float float32Arr_[62];

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

  //! Указатель на массив параметров устройства
  ModbusParameter *mbParams_;
  //! Количество параметров в массиве
  uint16_t countParameter_;

  uint8_t devAdrs_;
  int indexExchange_;

  //! Идентификатор задачи
  osThreadId threadId_;
  //! Идентификатор очереди параметров опроса вне основного цикла
  osMessageQId messageOutOfTurn_;
  //! Идентификатор очереди параметров с новыми значениями от устройства
  osMessageQId getValueDeviceQId_;

  ModbusMasterSerial *mms_;

};

#endif /* DEVICE_MODBUS_H_ */

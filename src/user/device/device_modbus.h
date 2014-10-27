/*
 * device_modbus.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef DEVICE_MODBUS_H_
#define DEVICE_MODBUS_H_

#include "define.h"

/*!
 \class DeviceModbus
 \brief Класс для устройств работающих по протоколу Modbus.
  Содержит структуру для составления и хранения Modbus карты устройства и
  набор методов для работы с её полями.
  Содержит функции организации цикла чтения и записи параметров устройства
*/ 
class DeviceModbus
{
public:
  /// Конструктор по умолчанию
  DeviceModbus(int Address, int Quantity);
  /// Деструктор по умолчанию
  virtual ~DeviceModbus();

protected:
  /// Структура для хранения полей параметра из карты устройства
  struct ModbusParameter
  {
    /// Уникальный идентификатор параметра
    int ID;
    /// Адрес регистра в устройстве
    int Address;
    /// Операции с параметром
    int Operation;
    /// Физическая величина параметра
    int Physic;
    /// Единицы измерения параметра
    int Unit;
    /// Поле состояния параметра
    int Validity;
    /// Тип данных
    int TypeData;
    /// Масштаб (коэффициент для приведения к физической единице измерения)
    float Scale;
    /// Коэффициент преобразования параметра
    float Coefficient;
    /// Минимальное значение параметра
    float Minimum;
    /// Максимально значение параметра
    float Maximum;
    /// Частота опроса параметра
    int FreqExchange;
    /// Количество запросов к параметру
    int CntExchange;
    /// Приоритет
    int Priority;
    /// Флаг
    int Flag;
    /// Считываемое значение
    unTypeData Value;
  };

  //! --------------------------------------------------------------------------
  //! Методы для работы с полями массива структур Modbus параметров (картой)
  //! через индекс параметра в массиве
  //! --------------------------------------------------------------------------

  /*!
   \brief Метод получения ID параметра по индексу в массиве параметров
   \param Index индекс параметра в массиве
   \return int ID параметра
  */
  int getModbusIDAtIndex(int Index);

  /*!
   \brief Метод получения адреса параметра по индексу в массиве параметров
   \param Index индекс параметра в массиве
   \return int Адрес параметра
  */
  int getModbusAddressAtIndex(int Index);

  /*!
   \brief Метод получения операций над параметром по индексу в массиве параметров
   \param Index индекс параметра в массиве
   \return int код операций над параметром
  */
  int getModbusOperationAtIndex(int Index);

  /*!
   \brief Метод получения физического смысла параметра
   \param Index Индекс параметра
   \return int Код физической величины параметра или 0 если ошибка
  */
  int getModbusPhysicAtIndex(int Index);

  /*!
   \brief Метод получения единиц измерения параметра
   \param Index Индекс параметра
   \return int Код единиц измерения параметра или 0 если ошибка
  */
  int getModbusUnitAtIndex(int Index);

  /*!
   \brief Метод получения флага валидности параметра
   \param Index Индекс параметра
   \return int Код валидности параметра
  */
  int getModbusValidityAtIndex(int Index);

  /*!
   \brief Метод получения типа данных параметра по индексу в карте регистров
   \param Index Индекс параметра
   \return int Тип данных параметра или 0 если ошибка
  */
  int getModbusTypeDataAtIndex(int Index);

  /*!
   \brief Метод получения коэффциента масштабирования по индексу в карте регистров
   \param Index Индекс параметра
   \return float Коэффициент масштабирования параметра
  */
  float getModbusScaleAtIndex(int Index);

  /*!
   \brief Метод получения коэффициента преобразования параметра
   \param Index Индекс параметра
   \return float Коэффициент преобразования или 0 если ошибка
  */
  float getModbusCoefficientAtIndex(int Index);

  /*!
   \brief Метод получения минимального значения параметра
   \param Index Индекс параметра
   \return float минимальное значение
  */
  float getModbusMinAtIndex(int Index);

  /*!
   \brief Метод получения максимального значения параметра
   \param Index Индекс параметра
   \return float максимальное значение
  */
  float getModbusMaxAtIndex(int Index);

  /*!
   \brief Метод получения задержки опроса параметра
   \param Index Индекс параметра
   \return int задержка опроса параметра
  */
  int getModbusFreqExchangeAtIndex(int Index);

  /*!
   \brief Метод получения текущей задержки обработки параметра
   \param Index Индекс параметра
   \return int Текущая задержка обработки параметра
  */
  int getModbusCntExchangeAtIndex(int Index);

  /*!
   \brief Метод получения приоритета обработки параметра
   \param Index Индекс параметра
   \return int Приоритет обработки параметра
  */
  int getModbusPriorityAtIndex(int Index);

  /*!
   \brief Метод получения флага обработки параметра
   \param Index Индекс параметра
   \return int Флаг обработки параметра
  */
  int getModbusFlagAtIndex(int Index);

  //! --------------------------------------------------------------------------
  //! Методы для работы с полями массива структур Modbus параметров (картой)
  //! через адрес параметра в массиве
  //! --------------------------------------------------------------------------

  /*!
   \brief Метод получения ID параметра по адресу в карте регистров
   \param Address Адрес параметра
   \return int ID параметра или 0 если ошибка
  */
  int getModbusIDAtAddress(int Address);

  /*!
   \brief Метод получения типа данных параметра по адресу в карте регистров
   \param Address Адрес параметра
   \return int Тип данных параметра или 0 если ошибка
  */
  int getModbusTypeDataAtAddress(int Address);

  /*!
   \brief Метод получения коэффициента масштабирования параметра по адресу
   \param Address Адрес параметра
   \return float Коэффициент масштабирования параметра или 0 если ошибка
  */
  float getModbusScaleAtAddress(int Address);

  /*!
   \brief Метод получения коэффициента преобразования параметра
   \param Address Адрес параметра
   \return float Коэффициент преобразования или 0 если ошибка
  */
  float getModbusCoefficientAtAddress(int Address);

  /*!
   \brief Метод получения физического смысла параметра
   \param Address Адрес параметра
   \return int Код физической величины параметра или 0 если ошибка
  */
  int getModbusPhysicAtAddress(int Address);

  /*!
   \brief Метод получения единиц измерения параметра
   \param Address Адрес параметра
   \return int Код единиц измерения параметра или 0 если ошибка
  */
  int getModbusUnitAtAddress(int Address);

  /*!
   \brief Метод получения индекса параметра в карте регистров по адресу
   \param Address Адрес параметра
   \return int Индекс в массиве или 0 если ошибка
  */
  int getIndexInArrayAtAddress(int Address);

  //! --------------------------------------------------------------------------
  //! Методы для организации и работы цикла опроса устройства
  //! --------------------------------------------------------------------------

  /*!
   \brief Метод поиска среди параметров устройства параметра с наибольшим приоритетом
   \return int Если найден параметр с повышенным приоритетом возвращает индекс
    параметра или 0 если нет таких параметров
  */
  int searchPriorityModbusParameters(void);

  /*!
   \brief Метод проверки предназначен ли параметр для работы в цикле и если он
    читается/записывается периодически истёк ли период
   \param indexParam - индекс параметра в карте регистров
   \return int 0 - не нужно, 1 - нужно
  */
  int checkExchangModbusParameters(int indexParam);

  /*!
   \brief Метод поиска параметра обычного приоритета для обработки в цикле
   \return int Индекс параметра для работы
  */
  int searchNextModbusParameters(void);

  /*!
   \brief Метод поиска и выбора параметра для обработки в цикле опроса
    в методе вызываются методы searchPriorityModbusParameters и
    searchNextModbusParameters
   \return int Индекс параметра для обработки
  */
  int searchExchangModbusParameters(void);

  /*!
   \brief Метод циклического опроса устройства по Modbus и/или внеочередной
    записи и/или чтения параметра
  */
  void excangeCycle(void);

  //! --------------------------------------------------------------------------
  //! Методы для преобразования значения полученного по modbus в значение
  //! для банка параметров
  //! --------------------------------------------------------------------------

  /*!
   \brief Метод преобразует значение полученное по Modbus в тип данных float
   \param Index Индекс параметра
   \param ModbusValue Значение полученное по Modbus
   \return float Значение в формате float
  */
  float convModbusTypeToFloatAtIndex(int Index, unTypeData ModbusValue);

  /*!
   \brief Метод применяет к значению полученному по Modbus коэффициент
    масштабирования параметра
   \param Index Индекс параметра
   \param Value Значение полученное по Modbus
   \return float Значение после применения коэффициента масштабирования
  */
  float applyModbusScaleAtIndex(int Index, float Value);

  /*!
   \brief Метод применяет к значению полученному по Modbus коэффициент
    преобразования параметра
   \param Index Индекс параметра
   \param Value Значение полученное по Modbus
   \return float Значение после применения коэффициента
  */
  float applyModbusCoefficientAtIndex(int Index, float Value);

  /*!
   \brief Метод преобразует значение полученное по Modbus в единицы измерения
    системы СИ
   \param Index Индекс параметра
   \param Value Значение полученное по Modbus
   \return float Значение после преобразования
  */
  float convModbusPhysicUnit(int Index, float Value);

  /*!
   \brief Метод преобразования значения полученного по Modbus в значение для
    банка параметров устройства
   \param Index Индекс в массиве параметров
   \param ModbusValue Значение по Modbus
   \return float Преобразованное значение
  */
  float convModbusToParameters(int Index, unTypeData ModbusValue);

  /// МЕТОД ПОЛУЧЕНИЯ АДРЕСА УСТРОЙСТВА
  int getDeviceAddress();

public:
  ModbusParameter ModbusParameters[];

private:
  /// Адрес устройства
  int  DeviceAddress;

  /// Количество параметров в устройстве
  int QuantityParam;

};

#endif /* DEVICE_MODBUS_H_ */

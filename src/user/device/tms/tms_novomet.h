/*
 * classTmsNovomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef TMS_NOVOMET_H_
#define TMS_NOVOMET_H_

#include "tms.h"
#include "device_modbus.h"

/*!
 * \brief Класс системы телеметрической "Новомет"
 */
class TmsNovomet: public Tms
{
public:
  TmsNovomet();
  virtual ~TmsNovomet();

  /*!
   * \brief Функция инициализации ТМС
   * в функции создаётся задача для работы с устройством и создаётся
   * modbus протокол для работы с устройством
   */
  void init();

  /*!
   * \brief Функция заполнения массива параметров modbus карты
   */
  void initModbusParameters();

  /*!
   * \brief Функция заполнения массива параметров по массиву modbus параметров
   */
  void initParameters();

  /*!
   * \brief Функция проверки и обновления параметра устройства
   * \param id - уникальный идентификатор параметра
   */
  void getNewValue(uint16_t id);

  /*!
   * \brief Функция записи нового значения в устройство
   * \param id - уникальный идентификатор параметра
   * \param value - значение параметра
   * \return 0 - выполнено, код ошибки
   */
  uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод записи параметра в устройство
   * \param id - индекс параметра
   * \param value - значение
   */
  void writeToDevice(int id, float value);

  int setUnitPressure(float unit);
  int setUnitTemperature(float unit);

  bool isConnect();

private:
  ModbusParameter modbusParameters_[21];
  DeviceModbus *dm_;

};

#endif /* TMS_NOVOMET_H_ */

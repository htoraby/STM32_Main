#ifndef TMSSCAD_H
#define TMSSCAD_H

#include "tms.h"
#include "device_modbus.h"

/*!
 * \brief Класс системы телеметрической "Скад"
 */
class TmsScad : public Tms
{
public:
  TmsScad();
  virtual ~TmsScad();

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
  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  /*!
   * \brief Метод записи параметра в устройство
   * \param id - индекс параметра
   * \param value - значение
   */
  void writeToDevice(int id, float value);

  bool isConnect();
  void getConnect();
  void resetConnect();

private:
  ModbusParameter modbusParameters_[7];
  DeviceModbus *dm_;

};

#endif // TMSSCAD_H

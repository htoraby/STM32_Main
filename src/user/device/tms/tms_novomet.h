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

  void init();
  void initModbusParameters();
  void initParameters();
  void getNewValue(uint16_t id);

  uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод записи параметра в устройство
   * \param id - индекс параметра
   * \param value - значение
   */
  void writeToDevice(int id, float value);

private:
  ModbusParameter modbusParameters_[74];
  DeviceModbus *dm_;

};

#endif /* TMS_NOVOMET_H_ */

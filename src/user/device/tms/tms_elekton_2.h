/*
 * classTmsElekton2.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef TMS_ELEKTON_2_H_
#define TMS_ELEKTON_2_H_

#include "tms.h"
#include "device_modbus.h"

/*!
 * \brief Класс системы телеметрической "Электон ТМСН-2"
 */
class TmsElekton2: public Tms
{
public:
  TmsElekton2();
  virtual ~TmsElekton2();

  void initModbusParameters();
  void initParameters();
  void init();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  void writeToDevice(int id, float value);

  int setUnitPressure(float unit);
  int setUnitTemperature(float unit);

  bool isConnect();

private:
  ModbusParameter modbusParameters_[12];
  DeviceModbus *dm_;
};

#endif /* TMS_ELEKTON_2_H_ */

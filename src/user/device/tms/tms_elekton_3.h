/*
 * classTmsElekton3.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef TMS_ELEKTON_3_H_
#define TMS_ELEKTON_3_H_

#include "tms.h"
#include "device_modbus.h"

enum enElekton3Status
{
  DHS_ELEKTON_3_STATUS_A12 = 12
};

enum enElektonTypeTMSP
{
  DHS_ELEKTON_TMSP_UNKNOW,
  DHS_ELEKTON_TMSP_1,
  DHS_ELEKTON_TMSP_2,
  DHS_ELEKTON_TMSP_3
};

class TmsElekton3: public Tms
{
public:
  TmsElekton3();
  virtual ~TmsElekton3();

  void initModbusParameters();
  void initParameters();
  void init();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  void writeToDevice(int id, float value);

  /*!
   * \brief Функция определения типа ТМСН
   * \param id
   */
  void checkTypeTMSP(int id);

  /*!
   * \brief checkDiscretePressure
   * \param id
   */
  void checkDiscretePressure(int id);

  int setUnitPressure(float unit);
  int setUnitTemperature(float unit);

  bool isConnect();

private:
  ModbusParameter modbusParameters_[15];
  DeviceModbus *dm_;
  uint16_t ElektonTypeTMSP_;
};

#endif /* TMS_ELEKTON_3_H_ */

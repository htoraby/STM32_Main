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


enum enElekton3Status {
  DHS_ELEKTON_3_STATUS_A12 = 12
};

enum enElektonSensor {
  DHS_ELEKTON_3_SENSOR_PRESSURE_HI_RES = 8
};

enum enElektonTypeTMSP {
  DHS_ELEKTON_TMSP_UNKNOW,
  DHS_ELEKTON_TMSP_1,
  DHS_ELEKTON_TMSP_2,
  DHS_ELEKTON_TMSP_3
};

enum enElektonUnitPressure {
  DHS_ELEKTON_UNIT_PRESSURE_ATM,
  DHS_ELEKTON_UNIT_PRESSURE_BAR,
  DHS_ELEKTON_UNIT_PRESSURE_AT,
  DHS_ELEKTON_UNIT_PRESSURE_kPA,
  DHS_ELEKTON_UNIT_PRESSURE_PSI,
  DHS_ELEKTON_UNIT_PRESSURE_KGS,
  DHS_ELEKTON_UNIT_PRESSURE_LAST
};

//! Структура для приведения соответствия между единицами измерения давления Электон ТМСН-3 и КСУ
struct stUnitsPressure {
  enElektonUnitPressure unitElekton;
  enPhysicPressure unitCCS;
};

class TmsElekton3: public Tms
{
public:
  TmsElekton3();
  virtual ~TmsElekton3();

  void initModbusParameters();
  void initParameters();
  void initUnitsElekton();
  void init();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  void writeToDevice(int id, float value);

  /*!
   * \brief configElekton3
   * \param typeTMSP
   * \param unit
   * \param hiRes
   * \return
   */
  bool config();

  int setUnitPressure(float unit);
  int setUnitTemperature(float unit);

  bool isConnect();

private:
  ModbusParameter modbusParameters_[15];
  DeviceModbus *dm_;
  stUnitsPressure unitPressure_[6];
  bool isConfigurated_;
};

#endif /* TMS_ELEKTON_3_H_ */

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

class TmsElekton2: public Tms
{
public:
  TmsElekton2();
  virtual ~TmsElekton2();

  void initModbusParameters();
  void initParameters();
  void init();


private:
  ModbusParameter modbusParameters_[15];
  DeviceModbus *dm_;
};

#endif /* TMS_ELEKTON_2_H_ */

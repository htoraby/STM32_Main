/*
 * modbus_slave_serial.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_SLAVE_SERIAL_H_
#define MODBUS_SLAVE_SERIAL_H_

#include "modbus_slave.h"

class ModbusSlaveSerial: public ModbusSlave
{
public:
  ModbusSlaveSerial();
  virtual ~ModbusSlaveSerial();
};

#endif /* MODBUS_SLAVE_SERIAL_H_ */

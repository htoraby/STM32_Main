/*
 * modbus_slave_tcp.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_SLAVE_TCP_H_
#define MODBUS_SLAVE_TCP_H_

#include "modbus_slave.h"

class ModbusSlaveTcp: public ModbusSlave
{
public:
  ModbusSlaveTcp();
  virtual ~ModbusSlaveTcp();
};

#endif /* MODBUS_SLAVE_TCP_H_ */

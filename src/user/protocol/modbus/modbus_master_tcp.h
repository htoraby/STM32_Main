/*
 * modbus_master_tcp.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_MASTER_TCP_H_
#define MODBUS_MASTER_TCP_H_

#include "modbus_master.h"

class ModbusMasterTcp: public ModbusMaster
{
	public:
    ModbusMasterTcp();
    virtual ~ModbusMasterTcp();

		// МЕТОД ЗАКРЫТИЯ ПРОТОКОЛА ОБМЕНА
		// Закрывает TCP / IP соединение и высвобождает все системные ресурсы,
		// связанные с соединением.
		virtual void closeProtocol();
};

#endif /* MODBUS_MASTER_TCP_H_ */

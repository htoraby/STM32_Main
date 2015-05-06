/*
 * modbus_master_serial.h
 *
 *  Created on: 03.07.2014
 *      Author: trofimenko
 */

#ifndef MODBUS_MASTER_SERIAL_H_
#define MODBUS_MASTER_SERIAL_H_

#include "modbus_master.h"
#include "uart.h"

/*!
 * \brief Класс modbus master для последовательных портов
 *  Реализует некоторые настройки последовательного протокола
 */
class ModbusMasterSerial: public ModbusMaster
{
public:
  ModbusMasterSerial(int Com);
  virtual ~ModbusMasterSerial();

  /*!
   * \brief Функция закрытия протокола обмена
   * Закрывает последовательного порта и освобождает все системные ресурсы,
   * связанные с портом.
   * \param PortName - номер UART порта
   * \return 0 - выполнено, 1 - не выполнено
   */
  int closeProtocol(int PortName);

  /*!
   * \brief Функция открытия протокола обмена
   * Открывает протокол и связанный и с ним порт
   * \param portName - Идентификатор Последовательного порта (например, "COM1", "/ dev/ser1" или "/ dev/ttyS0")
   * \param baudRate - Скорость передачи данных
   * \param dataBits - 7 бит данных для протокол ASCII, 8 бит данных для RTU
   * \param stopBits - стоп-бит
   * \param parity - контроль четности
   * \return 0 - выполнено, 1 - не выполнено
   */
  int openProtocol(int portName, long baudRate, int dataBits, int stopBits, int parity);

  /*!
   * \brief Метод проверки открыт ли протоколо
   * \return 0 - закрыт, 1 - открыт
   */
  int isOpen();

  /*!
   * \brief Функция записи данных в порт
   * \param Buf - массив байт записываемый в порт
   * \param Count - количество записываемых байт
   * \return 0 - выполнено, 1 - не выполнено
   */
  int transmitQuery(unsigned char *Buf, int Count);

  int receiveAnswer(uint8_t *Buf, uint8_t count);

	private:
    int numberComPort_;

    // Семафор
    osSemaphoreId semaphoreAnswer_;
};

#endif /* MODBUS_MASTER_SERIAL_H_ */

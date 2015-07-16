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
  ModbusMasterSerial();
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
   * \brief Функция записи данных в serial port
   * \param buf - массив байт записываемый в порт
   * \param num - количество записываемых байт
   * \return код выполнения операции
   */
  uint8_t txBuf(uint8_t *buf, uint8_t num);

  /*!
   * \brief Функция чтения данных из serial port
   * \param buf - массив полученных байт
   * \param num - ожидаемое количество полученных байт
   * \return код выполнения операции
   */
  uint8_t rxBuf(uint8_t *buf, uint8_t num);

	private:
    int numberComPort_;

    // Семафор
    osSemaphoreId semaphoreAnswer_;
};

#endif /* MODBUS_MASTER_SERIAL_H_ */

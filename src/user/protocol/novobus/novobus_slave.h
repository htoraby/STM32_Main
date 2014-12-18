#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "device.h"
#include "board.h"

#define NOVOBUS_COMMAND_DIAGNOSTIC  10
#define NOVOBUS_COMMAND_READ        20
#define NOVOBUS_COMMAND_WRITE       30
#define NOVOBUS_COMMAND_MESSAGE     40


/*!
 * \brief The novobus_master class
 *
 */
class NovobusSlave
{

public:
  NovobusSlave();
  ~NovobusSlave();

  /*!
   * \brief initNovobus
   * \return 0 Результат инициализации
   */
  int initNovobus();

  /*!
   * \brief calcCRC16
   * Метод вычисления контрольной суммы
   * \param begin С какого элемента массива начинать считать контрольную сумму
   * \param Size Количество элементов
   * \param Buf Массива с данными
   * \return Контрольную сумму
   */
  unsigned short calcCRC16(unsigned char begin, unsigned char Size, unsigned char *Buf);

  /*!
   * \brief reseivePackage
   * Функция обработки запросов по Novobus
   */
  void reseivePackage();

  int getMessageEventSPI();

  int getMessageParamSPI();

  /*!
   * \brief Очередь на посылку сообщений по SPI
   */
  osMessageQId messageEventSPI_;

  /*!
   * \brief messageParamSPI_ очередь на запись данных по SPI
   */
  osMessageQId messageParamSPI_;

  unsigned char txBuffer_[];
  unsigned char rxBuffer_[];
};

#endif // NOVOBUS_SLAVE_H

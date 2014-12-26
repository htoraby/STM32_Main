#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "device.h"
#include "board.h"

#define NOVOBUS_COMMAND_DIAGNOSTIC  1
#define NOVOBUS_COMMAND_READ        2
#define NOVOBUS_COMMAND_WRITE       3
#define NOVOBUS_COMMAND_MESSAGE     4


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
   * \brief exchangeCycle
   * Метод вызываемая как задача FreeRTOS
   */
  void exchangeCycle(void);

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

  /*!
   * \brief threadId_
   * Идентификатор задачи Novobus
   */
  osThreadId threadId_;

  unsigned char txBuffer_[];
  unsigned char rxBuffer_[];
};

#endif // NOVOBUS_SLAVE_H

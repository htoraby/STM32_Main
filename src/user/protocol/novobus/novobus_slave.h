#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "device.h"
#include "board.h"
#include "host.h"

#define MAX_IDS_BUFFER 40

/*!
 * \brief Класс протокола обмена между stm32 и контроллером визуализации
 *
 */
class NovobusSlave
{

public:

  /*!
   * \brief Комманды протокола "Novobus"
  */
  typedef enum {
    NoneCommand,
    StatusCommand,
    ReadParamsCommand,
    WriteParamsCommand,
    UpdateParamsCommand,
    MessageCommand,
  } NovobusCommand;

  NovobusSlave();
  ~NovobusSlave();

  /*!
   * \brief Инициализация протокола
   */
  void init();

  /*!
   * \brief Основная задача протокола
   */
  void task();

  /*!
   * \brief Функция обработки запросов по Novobus
   */
  void reseivePackage();

  /*!
   * \brief Метод получения события из очереди
   */
  int getMessageEvents();

  /*!
   * \brief Метод получения параметра из очереди
   */
  int getMessageParams();

  /*!
   * \brief Очередь на посылку сообытий
   */
  osMessageQId messageEvents_;

  /*!
   * \brief Очередь на запись данных
   */
  osMessageQId messageParams_;

private:
  /*!
   * \brief Метод проверки очередей и заполнения необходимых полей заголовка
   */
  void checkMessage();

  /*!
   * \brief Идентификатор задачи Novobus
   */
  osThreadId threadId_;

  uint8_t txBuffer_[HOST_BUF_SIZE];
  uint8_t rxBuffer_[HOST_BUF_SIZE];
  uint16_t idsBuffer_[MAX_IDS_BUFFER];
  uint8_t idsCount_;
};

#endif // NOVOBUS_SLAVE_H

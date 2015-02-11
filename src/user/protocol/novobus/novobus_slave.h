#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "define.h"
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
    UpdateEventsCommand,
    ReadEventsCommand,
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
   * \brief Метод добавления ID события в очередь
   */
  void putMessageEvents(uint32_t id);

  /*!
   * \brief Метод добавления ID параметра в очередь
   */
  void putMessageParams(uint32_t id);

private:
  /*!
   * \brief Метод проверки очередей и заполнения необходимых полей заголовка
   */
  void checkMessage();

  /*!
   * \brief Идентификатор задачи Novobus
   */
  osThreadId threadId_;

  /*!
   * \brief Очередь на посылку сообытий
   */
  osMessageQId messageEvents_;

  /*!
   * \brief Очередь на запись данных
   */
  osMessageQId messageParams_;

  uint8_t txBuffer_[HOST_BUF_SIZE];
  uint8_t rxBuffer_[HOST_BUF_SIZE];
  uint16_t idsBuffer_[MAX_IDS_BUFFER];
  uint8_t idsCount_;

  uint16_t rxSize;
};

#endif // NOVOBUS_SLAVE_H

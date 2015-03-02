#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "define.h"
#include "board.h"
#include "host.h"

#define MAX_IDS_BUFFER 40 //!< Максимальное количество ID во временом буффере

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

  /*!
   * \brief Ошибки
  */
  typedef enum {
    NoneError,
    CrcError,
    InvalidCmdError,
  } NovobusError;

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
  void receivePackage();

  /*!
   * \brief Метод получения события из очереди
   * \return ID события
   */
  int getMessageEvents();

  /*!
   * \brief Метод получения параметра из очереди
   * \return ID параметра
   */
  int getMessageParams();

  /*!
   * \brief Метод добавления ID события в очередь
   * \param id - ID события
   */
  void putMessageEvents(uint32_t id);

  /*!
   * \brief Метод добавления ID параметра в очередь
   * \param id - ID параметра
   */
  void putMessageParams(uint32_t id);

private:
  /*!
   * \brief Метод проверки очередей и заполнения необходимых полей заголовка
   */
  void checkMessage();

  //! Идентификатор задачи Novobus
  osThreadId threadId_;
  //! Очередь на посылку сообытий
  osMessageQId messageEvents_;
  //! Очередь на запись данных
  osMessageQId messageParams_;

  uint8_t txBuffer_[HOST_BUF_SIZE];
  uint8_t rxBuffer_[HOST_BUF_SIZE];
  uint16_t idsBuffer_[MAX_IDS_BUFFER];
  uint8_t idsCount_;

  uint16_t rxSize;
};

#endif // NOVOBUS_SLAVE_H

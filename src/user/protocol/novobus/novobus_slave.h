#ifndef NOVOBUS_SLAVE_H
#define NOVOBUS_SLAVE_H

#include "define.h"
#include "board.h"
#include "host.h"

#define MAX_IDS_BUFFER 35 //!< Максимальное количество ID во временом буффере
#define MAX_ADDRS_BUFFER 10 //!< Максимальное количество адресов во временом буффере

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
    NewEventsCommand,
    ReadEventsCommand,
    ReadMinCommand,
    ReadMaxCommand,
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
  void receivePackage(uint8_t sizePkt);

  /*!
   * \brief Метод получения адреса события из очереди
   * \return адрес события
   */
  int getMessageEvents();

  /*!
   * \brief Метод получения ID параметра из очереди
   * \return ID параметра
   */
  uint32_t getMessageParams();

  /*!
   * \brief Метод добавления адреса события в очередь
   * \param addr - адрес события
   */
  void putMessageEvents(uint32_t addr);

  /*!
   * \brief Метод добавления ID параметра в очередь
   * \param id - ID параметра
   */
  void putMessageParams(uint16_t id, uint16_t type = ReadParamsCommand);

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

  uint8_t oldCommand_;
  uint8_t txBuffer_[HOST_BUF_SIZE];
  uint8_t rxBuffer_[HOST_BUF_SIZE];
  uint32_t idsBuffer_[MAX_IDS_BUFFER];
  uint32_t addrsBuffer_[MAX_ADDRS_BUFFER];
  uint8_t idsCount_;
  uint8_t addrsCount_;

  uint16_t rxSize;
};

#endif // NOVOBUS_SLAVE_H

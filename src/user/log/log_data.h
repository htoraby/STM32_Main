#ifndef LOGDATA_H
#define LOGDATA_H

#include "log.h"

/*!
 * \brief Класс архива данных
 *
 */
class LogData : public Log
{
public:
  LogData();
  ~LogData();

  void init();
  void deInit();
  void task();

  void add();

private:
  /*!
   * \brief Метод добавления записи в архив
   *
   * \param
   */
  void add(uint8_t code);

  //! Идентификатор задачи
  osThreadId threadId_;
  osSemaphoreId addSemaphoreId_;

};

#endif // LOGDATA_H

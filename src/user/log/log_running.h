#ifndef LOGRUNNING_H
#define LOGRUNNING_H

#include "log.h"
#include "adc.h"

/*!
 * \brief Класс архива пусковых характеристик
 *
 */
class LogRunning : public Log
{
public:
  LogRunning(TypeLog type = RunTypeLog);
  ~LogRunning();

  /*!
   * \brief Инициализация архива
   * \param
   */
  void init(const char *threadName = "LogRunning");

  void deInit();

  /*!
   * \brief Запуск формирования и записи архива
   */
  void start();
  virtual void task();

protected:
  /*!
   * \brief Метод добавления записи в архив
   */
  void add();

  int16_t *uValue_;

  //! Идентификатор семафора начала записи архива
  osSemaphoreId semaphoreId_;
  //! Id события предшествующего записи аварийного архива
  uint32_t eventId_;

private:
  //! Идентификатор задачи
  osThreadId threadId_;

};

#endif // LOGRUNNING_H

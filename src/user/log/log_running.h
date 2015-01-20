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
   *
   */
  void init(const char *threadName = "LogRunning");
  /*!
   * \brief Запуск формирования и записи архива
   *
   */
  void start(EventType type);
  void task();

protected:
  void add();

  //! Идентификатор семафора начала записи архива
  osSemaphoreId semaphoreId_;
  //! Id события предшествующего записи аварийного архива
  uint32_t eventId_;

  float *uValue_;

private:
  //! Идентификатор задачи
  osThreadId threadId_;
  EventType eventType_;

};

#endif // LOGRUNNING_H

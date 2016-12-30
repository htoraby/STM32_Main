#ifndef LOGEVENT_H
#define LOGEVENT_H

#include "log.h"

#define LOG_EVENT_SIZE 21

/*!
 * \brief Класс архива событий
 */
class LogEvent : public Log
{
public:
  LogEvent();
  ~LogEvent();

  /*!
   * \brief Метод добавления нового события в архив
   *
   * \param
   * \param
   * \param
   * \param
   * \param
   * \return
   */
  uint32_t add(EventCode code, EventType type, EventId id,
               float oldValue = 0, float newValue = 0, uint8_t units = 0);

private:
  //! Идентификатор семафора начала записи архива
  osSemaphoreId semaphoreId_;

};

#endif // LOGEVENT_H

#ifndef LOGEVENT_H
#define LOGEVENT_H

#include "log.h"

/*!
 * \brief Класс архива событий
 *
 */
class LogEvent : public Log
{
public:
  LogEvent();
  ~LogEvent();

  /*!
   * \brief Метод добавления нового события в архив
   *
   */
  void add(uint8_t code, uint8_t type, uint16_t id, float oldValue, float newValue);
};

#endif // LOGEVENT_H

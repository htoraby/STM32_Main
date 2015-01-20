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
  uint32_t add(uint8_t code, uint8_t type, uint16_t id,
               float oldValue = 0, float newValue = 0);
};

#endif // LOGEVENT_H

#ifndef LOGALARM_H
#define LOGALARM_H

#include "log_running.h"

/*!
 * \brief Класс архива аварийных ситуаций
 */
class LogAlarm : public LogRunning
{
public:
  LogAlarm();
  ~LogAlarm();

  void init();

  /*!
   * \brief Запуск формирования и записи архива
   */
  void start(EventType type, uint32_t eventId);
  void task();

};

#endif // LOGALARM_H

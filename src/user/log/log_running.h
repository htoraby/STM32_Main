#ifndef LOGRUNNING_H
#define LOGRUNNING_H

#include "log.h"

/*!
 * \brief Класс архива пусковых характеристик
 *
 */
class LogRunning : public Log
{
public:
  LogRunning();
  ~LogRunning();
};

#endif // LOGRUNNING_H
#ifndef LOGTMS_H
#define LOGTMS_H

#include "log.h"

/*!
 * \brief Класс архива ТМС
 */
class LogTms : public Log
{
public:
  LogTms();
  ~LogTms();

  void init();
  void task();

private:
  void add();

  //! Идентификатор задачи
  osThreadId threadId_;

};

#endif // LOGTMS_H

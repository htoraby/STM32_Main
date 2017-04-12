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
  void deInit();
  void task();

private:
  void add();
  uint32_t calcRecordLogTms();
  void calcAddrLastRecordRosneft(uint32_t countRecord);

  //! Идентификатор задачи
  osThreadId threadId_;


};

#endif // LOGTMS_H

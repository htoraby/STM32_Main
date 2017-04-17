#ifndef LOGTMS_H
#define LOGTMS_H

#include "log.h"

#define LOG_DHS_SIZE 32

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

  /*!
   * \brief readLogRequestedRosneft
   * \return
   */
  StatusType readLogRequestedRosneft(uint32_t shiftFromEnd, uint8_t *buffer, uint32_t quantity);


private:
  void add();

  uint8_t *txBuffer_;



  //! Идентификатор задачи
  osThreadId threadId_;

  /*!
   * \brief Функция подсчёта записей в архивах ГДИ Роснефть
   */
  void calcAddrLastRecordRosneft();


};

#endif // LOGTMS_H

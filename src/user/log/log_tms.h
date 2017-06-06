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
   * \param shiftFromEnd
   * \param buffer
   * \param quantity
   * \return
   */
  StatusType readLogRequestedRosneft(uint32_t shiftFromEnd, uint8_t *buffer, uint32_t quantity);

  /*!
   * \brief calcCodeErrLogRosneft функция формирования кода ошибки для архивов ГДИ Роснефть
   * \return
   */
  uint8_t calcCodeErrLogRosneft();

private:
  void add(); 
  void incCountRecordLogDhs();

  uint8_t *txBuffer_;
  //! Идентификатор задачи
  osThreadId threadId_;



};

#endif // LOGTMS_H

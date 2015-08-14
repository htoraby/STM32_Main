#ifndef LOGDEBUG_H
#define LOGDEBUG_H

#include "log.h"

#define SIZE_MSG_DEBUG SIZE_BUF_LOG - 9

/*!
 * \brief Список типов отладочных сообщений
*/
typedef enum {
  DebugMsg,
  WarningMsg,
  CriticalMsg,
  FatalMsg,
} MsgType;

/*!
 * \brief Класс программистких архивов
 *
 */
class LogDebug : public Log
{
public:
  LogDebug();
  ~LogDebug();

  /*!
   * \brief Метод добавления отладочного сообщения в архив
   *
   * \param type - тип события @ref MsgType
   * \param msg - сообщение, максимальная длина 250 байт
   * \return id записанного события
   */
  uint32_t add(MsgType type, const char *msg, ...);

private:
  char msg_[SIZE_BUF_LOG*2];
#ifdef USE_RTT
  char msg1251_[SIZE_BUF_LOG*2];
#endif

};

#endif // LOGDEBUG_H

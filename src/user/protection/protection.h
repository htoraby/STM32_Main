#ifndef PROTECTION_H
#define PROTECTION_H

#include "user_main.h"

#define PROTECTION_ACTION_OFF         0
#define PROTECTION_ACTION_RESTART     1
#define PROTECTION_ACTION_BLOCK       2
#define PROTECTION_STATE_OFF                0
#define PROTECTION_STATE_ACTIV_BEGIN        10
#define PROTECTION_STATE_ACTIV_WAIT         20
#define PROTECTION_STATE_ACTIV              30
#define PROTECTION_STATE_WORK               40
#define PROTECTION_STATE_WORK_WAIT          50
#define PROTECTION_STATE_REACTION_BEGIN     60
#define PROTECTION_STATE_REACTION_WAIT      70
#define PROTECTION_STATE_REACTION           80
#define PROTECTION_STATE_FAILURE_BEGIN      90
#define PROTECTION_STATE_FAILURE_WAIT       100
#define PROTECTION_STATE_FAILURE            110
#define PROTECTION_STATE_RESTART_BEGIN      120
#define PROTECTION_STATE_RESTART_WAIT       130
#define PROTECTION_STATE_RESTART_BLOCK      140
#define PROTECTION_STATE_RESTART_BLOCK_WAIT 150
#define PROTECTION_STATE_RESTART            160
#define PROTECTION_STATE_STOP               170
#define PROTECTION_STATE_BLOCK              180

/*!
 * \brief The Protection class
 * Класс защит КСУ, реализует общие базовые функции для всех защит
 */
class Protection
{
public:
  Protection();

  ~Protection();

  /// id параметров защиты
  /// id Состояние защиты
  unsigned short idMode_;
  /// id Действие защиты
  unsigned short idReaction_;
  /// id задержка активации защиты
  unsigned short idActivDelay_;
  /// id задержка срабатывания защиты
  unsigned short idTripDelay_;
  /// id задержка АПВ защиты
  unsigned short idRestartDelay_;
  /// id количество АПВ
  unsigned short idRestartLimit_;
  /// id Время сброса количества АПВ
  unsigned short idRestartReset_;
  /// id граница срабатывания защиты
  unsigned short idTripSetpoint_;
  /// id граница АПВ
  unsigned short idRestartSetpoint_;
  /// id Параметр 1
  unsigned short idParam_;
  /// id Параметр 2
  unsigned short idParam2_;
  /// id Состояние автомата
  unsigned short idState_;
  /// id Таймера
  unsigned short idTimer_;
  /// id Счётчика АПВ
  unsigned short idRestartCount_;
  /// id Время от первого АПВ
  unsigned short idRestartResetCount_;
  /// id контролируемого параметра
  unsigned short idValueParam_;

  /// Локальные переменные для обработки
  /// Состояние защиты
  float mode_;
  /// Действие защиты
  float reaction_;
  /// Уставка: задержка активации защиты
  float activDelay_;
  /// Уставка: задержка срабатывания защиты
  float tripDelay_;
  /// Уставка: задержка АПВ защиты
  float restartDelay_;
  /// Уставка: количество АПВ
  float restartLimit_;
  /// Уставка: Время сброса количества АПВ
  float restartReset_;
  /// Уставка: граница срабатывания защиты
  float tripSetpoint_;
  /// Уставка: граница АПВ
  float restartSetpoint_;
  /// Уставка: Параметр 1
  float param_;
  /// Уставка: Параметр 2
  float param2_;

  /// Состояние автомата защиты
  float state_;
  /// Текущее значение контролируемого параметра
  float valueParameter_;
  /// Текущее значение таймера
  float timer_;
  /// Текущее количество АПВ по защите
  float restartCount_;
  /// Время первого срабатывания АПВ
  float restartResetCount_;

  /// Флаг выполнения условия срабатывания защиты
  bool trip_;
  /// Флаг запрещающего параметра
  bool block_;

  /// Идентификатор задачи
  osThreadId threadId_;

  void init(const char *threadName);

//  /*!
//   * \brief getIdProtection
//   * Функция получения id параметров защиты
//   * \param mode
//   * \param reaction
//   * \param activDelay
//   * \param tripDelay
//   * \param restartDelay
//   * \param restartLimit
//   * \param restartReset
//   * \param tripSetpoint
//   * \param restartSetpoint
//   * \param param
//   * \param param2
//   * \param state
//   * \param time
//   * \param restartCount
//   * \param restartResetCount
//   */
  void getIdProtection(unsigned short mode,
                       unsigned short reaction,
                       unsigned short activDelay,
                       unsigned short tripDelay,
                       unsigned short restartDelay,
                       unsigned short restartLimit,
                       unsigned short restartReset,
                       unsigned short tripSetpoint,
                       unsigned short restartSetpoint,
                       unsigned short param,
                       unsigned short param2,
                       unsigned short state,
                       unsigned short time,
                       unsigned short restartCount,
                       unsigned short restartResetCount,
                       unsigned short valueParam);

  /*!
   * \brief getSetpointProt
   * Метод получения уставок защиты, метод должен вызываться с периодом работы
   * работы автомата защиты, таким образом получаем актуальные значения уставок
   */
  void getSetpointProt();


  /*!
   * \brief getCurrentParamProt
   * Метод получения текущих параметров защиты, эти данные меняются в процессе
   * работы автомата защиты
   */
  void getCurrentParamProt();

  /*!
   * \brief checkTripSetPoint
   * Метод текущего значения отсительно уставки
   * \param limit если текущий параметр должен быть ниже уставки 0
   * и 1 если текущий параметр должен быть выше уставки
   * \return 0 параметр в норме, 1 параметр не в норме
   */
  bool checkTripSetPoint(bool limit);

  /*!
   * \brief checkRestartSetPoint
   * Функция проверки выполнено ли условие АПВ по значению параметра
   * \return
   */
  bool checkRestartSetPoint(bool limit);

  /*!
   * \brief checkRestartResetCount
   */
  void checkRestartResetCount();

  /*!
   * \brief protection
   * Автомат работы защиты
   * \return состояние защиты
   */
  void automatProtection();

  /*!
   * \brief taskProtection
   * Задача защиты
   */
  void taskProtection();

  /*!
   * \brief calcTimeFromStart
   * Функция вычисления сколько времени прошло от момента фиксации времени
   * и текущим временем
   * \return
   */
  float calcTimeFromStart();

  /*!
   * \brief calcaTimeUntilend
   * Функция вычисления сколько времени осталось до окончания события
   * \return
   */
  float calcTimeUntilEnd();

  /*!
   * \brief calcRestartResetCount
   * Функция проверки истекло ли время сброса счётчика АПВ
   * \return
   */
  bool calcRestartResetCount();

};

#endif // PROTECTION_H

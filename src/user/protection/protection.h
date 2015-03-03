#ifndef PROTECTION_H
#define PROTECTION_H

#include "user_main.h"

/*!
 * \brief Базовый класс защит КСУ
 */
class Protection
{
public:
  /*!
   * \brief Список действий защиты
  */
  typedef enum {
    ProtectionActionOff,
    ProtectionActionRestart,
    ProtectionActionBlock,
  } ProtectionAction;

  /*!
   * \brief Список состояний защиты
  */
  typedef enum {
    ProtectionStateIdle               = 0,
    ProtectionStateActivBegin        = 10,
    ProtectionStateActivWait         = 20,
    ProtectionStateActiv             = 30,
    ProtectionStateWork              = 40,
    ProtectionStateWorkWait          = 50,
    ProtectionStateReactionBegin     = 60,
    ProtectionStateReactionWait      = 70,
    ProtectionStateReaction          = 80,
    ProtectionStateFailureBegin      = 90,
    ProtectionStateFailureWait       = 100,
    ProtectionStateFailure           = 110,
    ProtectionStateRestartBegin      = 120,
    ProtectionStateRestartWait       = 130,
    ProtectionStateRestartBlock      = 140,
    ProtectionStateRestartBlockWait  = 150,
    ProtectionStateRestart           = 160,
    ProtectionStateStop              = 170,
    ProtectionStateBlock             = 180,
  } ProtectionState;

  Protection();
  ~Protection();

  /*!
   * \brief Инициализация обработки защиты
   * \param threadName - Имя задачи
   */
  void init(const char *threadName);

  /*!
   * \brief Метод получения уставок защиты.
   * Метод должен вызываться с периодом работы автомата защиты,
   * таким образом получаем актуальные значения уставок
   */
  virtual void getSetpointProt();

  /*!
   * \brief Метод получения текущих параметров защиты.
   * Эти данные меняются в процессе работы автомата защиты
   */
  virtual void getCurrentParamProt();

  /*!
   * \brief Метод сохранения текущих параметров защиты
   */
  virtual void setCurrentParamProt();

  /*!
   * \brief Метод проверки условия срабатывания защиты
   * \return false - параметр в норме, true - параметр не в норме
   */
  virtual bool checkAlarm() = 0;

  /*!
   * \brief Метод проверки превышения текущего значения отсительно уставки
   * \return false параметр в норме, true параметр не в норме
   */
  bool isHigherLimit(float setpoint);

  /*!
   * \brief Метод проверки уменьшения текущего значения отсительно уставки
   * \return false параметр в норме, true параметр не в норме
   */
  bool isLowerLimit(float setpoint);

  /*!
   * \brief Метод проверки выполнения условия АПВ по значению параметра
   * \return false параметр в норме, true параметр не в норме
   */
  virtual bool checkBlock();

  /*!
   * \brief Пересчёт текущего значения для сравнения с уставкой
   * \return Пересчитанное значение
   */
  virtual float calcValue();

  /*!
   * \brief Метод проверки таймера сброса счётчиков АПВ защиты
   */
  void checkRestartResetCount();

  /*!
   * \brief Метод добавления события "Сработала защита" в журнал
   */
  virtual void addEventActivationProt();

  /*!
   * \brief Универсальный автомат работы защиты
   */
  virtual void automatProtection();

  /*!
   * \brief Задача защиты
   */
  void task();

  /*!
   * \brief Функция вычисления времени от начала события
   * \return
   */
  float calcTimeFromStart();

  /*!
   * \brief Функция вычисления времени оставшегося до окончания события
   * \return
   */
  float calcTimeUntilEnd();

  /*!
   * \brief Функция проверки времени сброса счётчика АПВ
   * \return
   */
  bool calcRestartResetCount();

protected:
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
  int state_;
  /// Текущее значение контролируемого параметра
  float valueParameter_;
  /// Текущее значение таймера
  float timer_;
  /// Текущее количество АПВ по защите
  float restartCount_;
  /// Время первого срабатывания АПВ
  float restartResetCount_;

  /// Индексы событий для записи в журнал
  uint16_t protActivatedEventId_;
  uint16_t apvEventId_;
  uint16_t apvDisabledEventId_;
  uint16_t protBlockedEventId_;

  /// Флаг выполнения условия срабатывания защиты
  bool alarm_;
  /// Флаг запрещающего параметра
  bool block_;

  /// Идентификатор задачи защиты
  osThreadId threadId_;

};

#endif // PROTECTION_H

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
    ModeOff         = 0,                    //!< Защиты выключена
    ModeBlock,                              //!< Реакция защиты блокировка
    ModeRestart,                            //!< Реакция защиты АПВ
    ModeOn,                                 //!< Защита включена
  } ProtMode;

  /*!
   * \brief Список состояний защиты
  */
  typedef enum {
    StateRunning,
    StateRun,
    StateStopping,
    StateStop,
  } ProtState;

  Protection();
  ~Protection();

  /*!
   * \brief Метод обработки защиты
   */
  void processing();

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
  virtual bool checkAlarm();

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
  virtual bool checkPrevent();

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
  virtual void addEventReactionProt();

  /*!
   * \brief isReactionOff
   * \return
   */
  bool isModeOff();

  /*!
   * \brief isReactionBlock
   * \return
   */
  bool isModeBlock();

  /*!
   * \brief isReactionRestart
   * \return
   */
  bool isModeRestart();

  /*!
   * \brief isReactionOn
   * \return
   */
  bool isModeOn();

  /*!
   * \brief Функция перевода автомата защиты в состояние стоп
   */
  void setStateStop();

  /*!
   * \brief Функция перевода автомата защиты в состояние работа
   */
  void setStateRun();

  /*!
   * \brief Универсальный автомат работы защиты
   */
  virtual void automatProtection();

  /*!
   * \brief Состояние защиты задержка активации
   */
  virtual void processingStateRunning();

  /*!
   * \brief Состояние защита контроля параметра и задержки срабатывания
   */
  virtual void processingStateRun();

  /*!
   * \brief Состояние защиты ожидание останова
   */
  virtual void proccessingStateStopping();

  /*!
   * \brief Состояние защиты останов
   */
  virtual void proccessingStateStop();

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

  /*!
   * \brief isDelay
   * \return
   */
  bool isDelay() {
    return delay_;
  }

  /*!
   * \brief isPrevent
   * \return
   */
  bool isPrevent() {
    return prevent_;
  }

  /*!
   * \brief getApvDisabledEventId
   * \return
   */
  uint16_t getApvDisabledEventId() {
    return apvDisabledEventId_;
  }


protected:

  void incRestartCount();

  /// id параметров защиты
  unsigned short idMode_;                   //!< id Режим защиты
  unsigned short idActivDelay_;             /// id задержка активации защиты
  unsigned short idTripDelay_;              /// id задержка срабатывания защиты
  unsigned short idRestartDelay_;           /// id задержка АПВ защиты
  unsigned short idRestartLimit_;           /// id количество АПВ
  unsigned short idRestartResetTime_;       /// id Время сброса количества АПВ
  unsigned short idTripSetpoint_;           /// id граница срабатывания защиты
  unsigned short idRestartSetpoint_;        /// id граница АПВ
  unsigned short idParam_;                  /// id Параметр 1
  unsigned short idRestartFlag_;            /// id Флаг что защита в состоянии АПВ
  unsigned short idState_;                  /// id Состояние автомата
  unsigned short idTimer_;                  /// id Таймера
  unsigned short idRestartCount_;           /// id Счётчика АПВ
  unsigned short idRestartFirstTime_;       /// id Время первого срабатывания АПВ

  /// Локальные переменные для обработки 
  float mode_;                              //!< Режим защиты
  float activDelay_;                        /// Уставка: задержка активации защиты
  float tripDelay_;                         /// Уставка: задержка срабатывания защиты
  float restartDelay_;                      /// Уставка: задержка АПВ защиты
  float restartLimit_;                      /// Уставка: количество АПВ
  float restartResetTime_;                  /// Уставка: Время сброса количества АПВ
  float tripSetpoint_;                      /// Уставка: граница срабатывания защиты
  float restartSetpoint_;                   /// Уставка: граница АПВ
  float timerDifStart_;                     /// Уставка: Параметр 1
  int state_;                               /// Состояние автомата защиты
  float valueParameter_;                    /// Текущее значение контролируемого параметра
  uint32_t timer_;                          /// Текущее значение таймера
  float restartCount_;                      /// Текущее количество АПВ по защите
  uint32_t restartFirstTime_;               /// Время первого срабатывания АПВ
  bool alarm_;                              /// Флаг выполнения условия срабатывания защиты
  bool prevent_;                            /// Флаг запрещающего параметра
  bool block_;                              /// Флаг что защита в блокировке
  float restart_;                           /// Флаг что защита в состоянии АПВ
  bool attempt_;                            /// Флаг первой попытки запуска
  bool delay_;                              /// Флаг задержки срабатывания

  /// Индексы событий для записи в журнал
  uint16_t protReactEventId_;
  uint16_t apvEventId_;
  uint16_t apvDisabledEventId_;
  uint16_t protBlockedEventId_;

};

#endif // PROTECTION_H

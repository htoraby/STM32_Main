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
    ProtReactionOff         = 0,      /// Защиты выключена
    ProtReactionRestart,               /// Реакция защиты АПВ
    ProtReactionBlock,                /// Реакция защиты блокировка
    ProtReactionOn,                   /// Защита включена
  } ProtectionReaction;

  /*!
   * \brief Список состояний защиты
  */
  typedef enum {
    ProtStateIdle           = 0,
    ProtStateRunning,
    ProtStateRun,
    ProtStateStopping,
    ProtStateStop,
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
  virtual void addEventReactionProt();

  /*!
   * \brief isReactionOff
   * \return
   */
  bool isReactionOff();

  /*!
   * \brief isReactionBlock
   * \return
   */
  bool isReactionBlock();

  /*!
   * \brief isReactionRestart
   * \return
   */
  bool isReactionRestart();

  /*!
   * \brief isReactionOn
   * \return
   */
  bool isReactionOn();

  /*!
   * \brief Универсальный автомат работы защиты
   */
  virtual void automatProtection();

  /*!
   * \brief Пустое состояние защиты
   */
  virtual void processingStateIdle();

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


protected:
  /// id параметров защиты
  unsigned short idReaction_;               /// id Действие защиты
  unsigned short idActivDelay_;             /// id задержка активации защиты
  unsigned short idTripDelay_;              /// id задержка срабатывания защиты
  unsigned short idRestartDelay_;           /// id задержка АПВ защиты
  unsigned short idRestartLimit_;           /// id количество АПВ
  unsigned short idRestartReset_;           /// id Время сброса количества АПВ
  unsigned short idTripSetpoint_;           /// id граница срабатывания защиты
  unsigned short idRestartSetpoint_;        /// id граница АПВ
  unsigned short idParam_;                  /// id Параметр 1
  unsigned short idParam2_;                 /// id Параметр 2
  unsigned short idState_;                  /// id Состояние автомата
  unsigned short idTimer_;                  /// id Таймера
  unsigned short idRestartCount_;           /// id Счётчика АПВ
  unsigned short idRestartResetCount_;      /// id Время от первого АПВ

  /// Локальные переменные для обработки 
  float reaction_;                          /// Действие защиты
  float activDelay_;                        /// Уставка: задержка активации защиты
  float tripDelay_;                         /// Уставка: задержка срабатывания защиты
  float restartDelay_;                      /// Уставка: задержка АПВ защиты
  float restartLimit_;                      /// Уставка: количество АПВ
  float restartReset_;                      /// Уставка: Время сброса количества АПВ
  float tripSetpoint_;                      /// Уставка: граница срабатывания защиты
  float restartSetpoint_;                   /// Уставка: граница АПВ
  float param_;                             /// Уставка: Параметр 1
  float param2_;                            /// Уставка: Параметр 2
  int state_;                               /// Состояние автомата защиты
  float valueParameter_;                    /// Текущее значение контролируемого параметра
  float timer_;                             /// Текущее значение таймера
  float restartCount_;                      /// Текущее количество АПВ по защите
  float restartResetCount_;                 /// Время первого срабатывания АПВ

  /// Индексы событий для записи в журнал
  uint16_t protReactionEventId_;
  uint16_t apvEventId_;
  uint16_t apvDisabledEventId_;
  uint16_t protBlockedEventId_;

  bool alarm_;                              /// Флаг выполнения условия срабатывания защиты
  bool block_;                              /// Флаг запрещающего параметра


};

#endif // PROTECTION_H

#ifndef REGIME_H
#define REGIME_H

#include "user_main.h"

/*!
 * \brief Базовый класс режимов КСУ
 */
class Regime
{
public:
  /*!
   * \brief Список действий режима
  */
  typedef enum {
    OffAction,       //!< Режим отключен
    SingleAction,    //!< Однократный
    EachRunAction,   //!< При каждом запуске
    OnAction,        //!< Режим включен
  } RegimeAction;

  /*!
   * \brief Список состояний режима
  */
  typedef enum {
    IdleState,
    WorkState,
    RunningState,
    WaitPauseState,
    PauseState,
    RestartState,
    StopState,
  } RegimeState;

  Regime();
  ~Regime();

  /*!
   * \brief Метод обработки режима
   */
  virtual void processing() = 0;

protected:
  int action_;    //!< Действие режима
  int state_;     //!< Состояние автомата

};

#endif // REGIME_H

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
   * \brief Список режимов запуска
  */
  typedef enum {
    NoneRegimeRun,                          //!< Нет режима
    SmoothRegimeRun,                        //!< Плавный
    PushRegimeRun,                          //!< Толчковый
    SwingRegimeRun,                         //!< Раскачкой
    SyncRegimeRun,                          //!< С синхронизацией
    PickupRegimeRun,                        //!< Подхват
    SkipFreqRegimeRun,                      //!< Пропуск резонансных частот
    AutoAdaptationRegimeRun,                //!< Автоадаптация
    DirectRegimeRun,                        //!< Прямой пуск
  } TypeRegimeRun;
  /*!
   * \brief Список состояний режима
  */
  typedef enum {
    IdleState = 0,
    RunningState = 100,
    WorkState = 200,
    WaitPauseState = 300,
    PauseState = 400,
    RestartState = 500,
    StopState = 600,
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

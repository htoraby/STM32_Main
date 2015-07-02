#ifndef REGIMERUN_H
#define REGIMERUN_H

#include "regime.h"



class RegimeRun : public Regime
{
public:
  RegimeRun();
  ~RegimeRun();

  /*!
   * \brief Функция получения общих параметров для всех режимов
   */
  void getGeneralSetpoint();

  /*!
   * \brief Функция получения индивидуальный параметров режима
   */
  virtual void getOtherSetpoint();

  /*!
   * \brief Функция сохранения общих параметров для всех режимов
   */
  void setGeneralSetPoint();

  /*!
   * \brief Функция сохранения индивидуальный параметров режима
   */
  virtual void setOtherSetpoint();

  /*!
   * \brief processingStateIdle
   */
  void processingStateIdle();

  /*!
   * \brief processingStateRunning
   */
  virtual void processingStateRunning();

  /*!
   * \brief processingStateWork
   */
  virtual void processingStateWork();

  /*!
   * \brief Автомат работы режима пуска
   */
  void automatRegime();

  /*!
   * \brief Задача работы режима
   */
  void processing();

  LastReasonRun runReason_;                 //!< Причина последней попытки запуска
};

#endif // REGIMERUN_H

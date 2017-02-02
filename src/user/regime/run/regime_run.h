#ifndef REGIMERUN_H
#define REGIMERUN_H

#include "regime.h"

class RegimeRun : public Regime
{
public:
  RegimeRun();
  ~RegimeRun();

  /*!
   * \brief Задача работы режима
   */
  void processing();


protected:
  /*!
   * \brief Функция получения индивидуальный параметров режима
   */
  virtual void getOtherSetpoint();

  /*!
   * \brief Функция сохранения индивидуальный параметров режима
   */
  virtual void setOtherSetpoint();

  /*!
   * \brief processingStateIdle
   */
  virtual void processingStateIdle();

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
  virtual void automatRegime();

  LastReasonRun runReason_;                 //!< Причина последней попытки запуска

private:
  /*!
   * \brief Функция получения общих параметров для всех режимов
   */
  void getGeneralSetpoint();

  /*!
   * \brief Функция сохранения общих параметров для всех режимов
   */
  void setGeneralSetPoint();
};

#endif // REGIMERUN_H

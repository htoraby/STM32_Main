/*
 * classController.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef CCS_H_
#define CCS_H_

#include "device.h"

class Ccs: public Device
{
public:
  Ccs();
  virtual ~Ccs();

  /*!
   * \brief Инициализация массива параметров
   */
  void initParameters();

  /*!
   * \brief Проверка находится ли станция в стопе
   * \return
   */
  bool checkStopCCS();

  /*!
   * \brief Проверка находится ли станция в работе
   * \return
   */
  bool checkWorkCCS();

  /*!
   * \brief Проверка что КСУ в режиме АUTO
   * \return
   */
  bool checkAutoControlMode();

  /*!
   * \brief getTime
   * Функция получения текущего времени в сек от 00:00:00 01.01.1970 года
   * \return Возвращает значение секунд
   */
  float getTime();

  /*!
   * \brief Проверка находится ли станция в блокировке
   * \return
   */
  bool checkBlockCCS();

  /*!
   * \brief Метод обработки изменения состояния
   */
  void conditionChanged();

  /*!
   * \brief Метод обработки изменения режима переключателя
   */
  void controlModeChanged();

private:
  //! Массив параметров устройства
  parameter parametersArray_[CCS_END - CCS_BEGIN];

  //! Предыдущий режим переключателя
  int controlModeOld;
  //! Предыдущие состояние
  int conditionOld;

};

#endif /* CCS_H_ */

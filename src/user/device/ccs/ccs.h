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

  bool checkStopCCS();
  bool checkWorkCCS();

  /*!
   * \brief checkAutoControlMode
   * Проверка что КСУ в режиме АUTO
   * \return
   */
  bool checkAutoControlMode();

  /*!
   * \brief getTime
   * Функция получения текущего времени в сек от
   * 00:00:00 01.01.1970 года
   * \return Возвращает значение секунд
   */
  float getTime();

  bool checkBlockCCS();

};

#endif /* CCS_H_ */

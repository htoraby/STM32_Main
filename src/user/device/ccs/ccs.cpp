/*
 * classController.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "ccs.h"


Ccs::Ccs()
{
// TODO Auto-generated destructor stub
}

Ccs::~Ccs()
{
	// TODO Auto-generated destructor stub
}


// Проверка находится ли станция в стопе
bool Ccs::checkStopCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_STOP)
      ||(state == CCS_CONDITION_WAIT)
      ||(state == CCS_CONDITION_BLOCK)) {
    return 0;
  }
  else
    return 1;
}

// Проверка находится ли станция в работе
bool Ccs::checkWorkCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if ((state == CCS_CONDITION_RUN)||(state == CCS_CONDITION_DELAY))
    return 0;
  else
    return 1;
}

// Проверка находится ли станция в блокировке
bool Ccs::checkBlockCCS()
{
  unsigned int state = (unsigned int)getValue(CCS_CONDITION);
  if (state == CCS_CONDITION_BLOCK)
    return 0;
  else
    return 1;
}

// Проверка что КСУ в режиме АUTO
bool Ccs::checkAutoControlMode()
{
  unsigned int controlMode = (unsigned int)getValue(CCS_CONTROL_MODE);
  if (controlMode == CCS_CONTROL_MODE_AUTO)
    return 0;
  else
    return 1;
}


/*!
 * \brief getTime
 * Функция получения текущего времени в сек от
 * 00:00:00 01.01.1970 года
 * \return Возвращает значение секунд
 */
float Ccs::getTime()
{
  return getValue(CCS_DATE_TIME);
}

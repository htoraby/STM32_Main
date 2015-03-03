/*
 * classTms.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "tms.h"


Tms::Tms() : Device(TMS_BEGIN, parametersArray_, TMS_END - TMS_BEGIN)
{
  initParameters();
}

Tms::~Tms()
{
	// TODO Auto-generated destructor stub
}

void Tms::initParameters()
{
  // Пустой элемент массива
  parameters_[TMS_BEGIN - TMS_BEGIN].id            = TMS_BEGIN;
  parameters_[TMS_BEGIN - TMS_BEGIN].access        = ACCESS_ERROR;
  parameters_[TMS_BEGIN - TMS_BEGIN].operation     = OPERATION_ERROR;
  parameters_[TMS_BEGIN - TMS_BEGIN].physic        = PHYSIC_ERROR;
  parameters_[TMS_BEGIN - TMS_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[TMS_BEGIN - TMS_BEGIN].value         = 0.0;
  parameters_[TMS_BEGIN - TMS_BEGIN].min           = 0.0;
  parameters_[TMS_BEGIN - TMS_BEGIN].max           = 0.0;
  parameters_[TMS_BEGIN - TMS_BEGIN].def           = 0.0;
}


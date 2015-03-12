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

}

void Tms::initParameters()
{ 
  for (int index = 0; index < (TMS_END - TMS_BEGIN); index++) {
    setFieldID(index, index + TMS_BEGIN);
    setFieldAccess(index, ACCESS_ERROR);
    setFieldOperation(index, OPERATION_ERROR);
    setFieldPhysic(index, PHYSIC_ERROR);
    setFieldMin(index, 0.0);
    setFieldMax(index, 0.0);
    setFieldDef(index, 0.0);
    setFieldValidity(index, VALIDITY_ERROR);
    setFieldValue(index, 0.0);
  }
}

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

int Tms::setUnitPressure(float unit)
{
  return setValue(TMS_PRESSURE_UNIT, unit);
}

int Tms::setUnitTemperature(float unit)
{
  return setValue(TMS_TEMPERATURE_UNIT, unit);
}

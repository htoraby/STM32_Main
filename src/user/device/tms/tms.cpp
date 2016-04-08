/*
 * classTms.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "tms.h"
#include "user_main.h"

//! Массив параметров устройства
static parameter parametersArray[TMS_END - TMS_BEGIN] __attribute__((section(".extmem")));

Tms::Tms() : Device(TMS_BEGIN, parametersArray, TMS_END - TMS_BEGIN)
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

void Tms::getConnect()
{

}

void Tms::setConnect(stConnectQuality *counters)
{
  parameters.set(CCS_DHS_CONNECTION_PERCENT, counters->quality);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_TOTAL, counters->transmite);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_SUCCESS, counters->resive);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_TRASH, counters->trash);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_CRC, counters->crc);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_ERR, counters->error);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_LOST, counters->lost);
}

void Tms::resetConnect()
{
  parameters.set(CCS_DHS_CONNECTION_PERCENT, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_TOTAL, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_SUCCESS, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_TRASH, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_CRC, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_ERR, 0);
  parameters.set(CCS_DHS_CONNECTION_PACKAGE_LOST, 0);
}

/*
 * classTmsNovomet.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "tms_novomet.h"

void TmsNovomet::initModbusParameters()
{

}

TmsNovomet::TmsNovomet()
{
  initModbusParameters();
  readParameters();
}

TmsNovomet::~TmsNovomet()
{

}

void TmsNovomet::init()
{
  // Создание задачи обновления параметров
  createThread("UpdateParametersTms");
  // Создание объекта протокола связи с утройством
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  dm_ = new DeviceModbus(modbusParameters_, count,
                        TMS_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1);
  dm_->createThread("ProtocolTms", getValueDeviceQId_);
}

void TmsNovomet::initParameters()
{
  Tms::initParameters();
  // Заполняем карту регистров ТМС
  int count = sizeof(modbusParameters_)/sizeof(ModbusParameter);
  for (int indexModbus = 0; indexModbus <= count; indexModbus++) {
    int indexDevice = getIndexAtId(dm_->getFieldID(indexModbus));
    if (indexDevice) {
      setFieldAccess(indexDevice, ACCESS_OPERATOR);
      setFieldOperation(indexDevice, dm_->getFieldOperation(indexModbus));
      setFieldPhysic(indexDevice, dm_->getFieldPhysic(indexModbus));
      float tempVal = dm_->getFieldMinimum(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldMin(indexDevice, tempVal);
      tempVal = dm_->getFieldMaximum(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldMax(indexDevice, tempVal);
      tempVal = dm_->getFieldDefault(indexModbus);
      tempVal = applyCoef(tempVal, dm_->getFieldCoefficient(indexModbus));
      tempVal = applyUnit(tempVal, dm_->getFieldPhysic(indexModbus), dm_->getFieldUnit(indexModbus));
      setFieldDef(indexDevice, tempVal);
      setFieldValidity(indexDevice, dm_->getFieldValidity(indexModbus));
      setFieldValue(indexDevice, getFieldDefault(indexDevice));
    }
  }
}

void TmsNovomet::getNewValue(uint16_t id)
{

}

uint8_t TmsNovomet::setNewValue(uint16_t id, float value)
{
  return 0;
}

void TmsNovomet::writeToDevice(int id, float value)
{
  dm_->writeModbusParameter(id, value);
}


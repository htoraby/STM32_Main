#include "scada_region_3_0.h"

void ScadaRegion30::initParameters()
{
  countParameters_ = 173;

  scadaParameter_[0].id = CCS_SCADA_ADDRESS;
  scadaParameter_[0].address = 1;
  scadaParameter_[0].operation = OPERATION_WRITE;
  scadaParameter_[0].physic = PHYSIC_NUMERIC;
  scadaParameter_[0].unit = NUMERIC_NUMBER;
  scadaParameter_[0].typeData = TYPE_DATA_CHAR;
  scadaParameter_[0].coefficient = 1;
  scadaParameter_[0].min = 1;
  scadaParameter_[0].max = 247;
  scadaParameter_[0].command = OPERATION_ERROR;
  scadaParameter_[0].value.float_t = 0;

  scadaParameter_[1].id = CCS_WORKING_MODE;
  scadaParameter_[1].address = 3;
  scadaParameter_[1].operation = OPERATION_READ;
  scadaParameter_[1].physic = PHYSIC_NUMERIC;
  scadaParameter_[1].unit = NUMERIC_NUMBER;
  scadaParameter_[1].typeData = TYPE_DATA_UINT16;
  scadaParameter_[1].coefficient = 1;
  scadaParameter_[1].min = 0;
  scadaParameter_[1].max = 65535;
  scadaParameter_[1].command = OPERATION_ERROR;
  scadaParameter_[1].value.float_t = 0;
}

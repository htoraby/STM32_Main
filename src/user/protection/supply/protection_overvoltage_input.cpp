#include "protection_overvoltage_input.h"

ProtectionOvervoltageInput::ProtectionOvervoltageInput()
{

}

ProtectionOvervoltageInput::~ProtectionOvervoltageInput()
{

}

// Метод вычисления контролируемого параметра
float ProtectionOvervoltageInput::calcControlParameter()
{
  return (em.getValue(EM_VOLTAGE_PHASE_1_2)
          + em.getValue(EM_VOLTAGE_PHASE_2_3)
          + em.getValue(EM_VOLTAGE_PHASE_3_1))/3;
}


unsigned char ProtectionOvervoltageInput::protection()
{

}

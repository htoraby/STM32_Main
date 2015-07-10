#include "units.h"

float units[PHYSIC_LAST][6][2] =
{
  {{1.0, 0.0},         //PHYSIC_NUMERIC -> NUMERIC_NUMBER
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_DATE_TIME -> DATE_TIME_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_TIME -> TIME_SS
   {1000000.0, 0.0},   //PHYSIC_TIME -> TIME_US
   {1000.0, 0.0},      //PHYSIC_TIME -> TIME_MS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_PERCENT -> PERCENT_PERCENT
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_LENGHT -> LENGTH_M
   {1609.344, 0.0},    //PHYSIC_LENGHT -> LENGTH_MILE
   {0.3048, 0.0},      //PHYSIC_LENGHT -> LENGTH_FOOT
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_DENSITY -> DENSITY_KG_M3
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_SPACE -> SPACE_MM2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_FREQUENCY -> FREQUENCY_HZ
   {0.001, 0.0},       //PHYSIC_FREQUENCY -> FREQUENCY_KHZ
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_VOLTAGE -> VOLTAGE_V
   {0.001, 0.0},       //PHYSIC_VOLTAGE -> VOLTAGE_KV
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_CURRENT -> CURRENT_A
   {1000.0, 0.0},      //PHYSIC_CURRENT -> CURRENT_MA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_ACTIVE_POWER -> ACTIV_POWER_W
   {0.001, 0.0},       //PHYSIC_ACTIVE_POWER -> ACTIV_POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_FULL_POWER -> FULL_POWER_VA
   {0.001, 0.0},       //PHYSIC_FULL_POWER -> FULL_POWER_KVA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_REACTIVE_POWER -> FULL_POWER_KVA
   {0.001, 0.0},       //PHYSIC_REACTIVE_POWER -> REACTIV_POWER_KVAR
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_ANGLE -> ANGLE_DEGREES
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_RPM -> RPM_RPM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_TEMP -> TEMP_HZ_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_RESISTANCE -> RESISTANCE_OM
   {0.001, 0.0},       //PHYSIC_RESISTANCE -> RESISTANCE_KOM
   {0.000001, 0.0},    //PHYSIC_RESISTANCE -> RESISTANCE_MOM
   {1000.0, 0.0},      //PHYSIC_RESISTANCE -> RESISTANCE_MILIOM
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_RESIST_CABLE -> RESIST_CABLE_KOM_M
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_PRESSURE -> PRESSURE_MPA
   {9.8692, 0.0},      //PHYSIC_PRESSURE -> PRESSURE_ATM
   {10.197, 0.0},      //PHYSIC_PRESSURE -> PRESSURE_AT
   {10.0, 0.0},        //PHYSIC_PRESSURE -> PRESSURE_BAR
   {145.04, 0.0},      //PHYSIC_PRESSURE -> PRESSURE_PSI
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_TEMPERATURE -> TEMPERATURE_C
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_ACCELERATION -> ACCELERATION_G
   {9.80665, 0.0},     //PHYSIC_ACCELERATION -> ACCELERATION_MSS2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_SPEED -> SPEED_M_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_VISCOSITY -> VISCOSITY_MPA_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_FLOW -> FLOW_M3_DD
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_POWER -> POWER_W
   {0.001, 0.0},       //PHYSIC_POWER -> POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_ACTIVE_ENERGY -> ACTIVE_ENERGY_KW_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_REACTIVE_ENERGY -> REACTIVE_ENERGY_KVAR_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_FULL_ENERGY -> FULL_ENERGY_KVAHH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_RATE -> RATE_NM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},         //PHYSIC_INDUNCTANCE -> INDUNCTANCE_MH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
};


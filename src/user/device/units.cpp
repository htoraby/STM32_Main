#include "units.h"

float units[PHYSIC_LAST][6][2] =
{
  {{1.0, 0.0},                            // PHYSIC_NUMERIC
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_DATE_TIME
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TIME TIME_SS
   {0.000001, 0.0},                       // PHYSIC_TIME TIME_US
   {0.001, 0.0},                          // PHYSIC_TIME TIME_MS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_PERCENT PERCENT_PERCENT
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_LENGHT LENGTH_M
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_DENSITY DENSITY_KG_M3
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_SPACE SPACE_M2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FREQUENCY FREQUENCY_HZ
   {1000.0, 0.0},                         // PHYSIC_FREQUENCY FREQUENCY_KHZ
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_VOLTAGE VOLTAGE_V
   {1000.0, 0.0},                         // PHYSIC_VOLTAGE VOLTAGE_KV
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_CURRENT CURRENT_A
   {1000.0, 0.0},                         // PHYSIC_CURRENT CURRENT_MA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACTIVE_POWER ACTIV_POWER_W
   {1000.0, 0.0},                         // PHYSIC_ACTIVE_POWER ACTIV_POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FULL_POWER FULL_POWER_VA
   {1000.0, 0.0},                         // PHYSIC_FULL_POWER FULL_POWER_KVA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_REACTIVE_POWER REACTIV_POWER_VAR
   {1000.0, 0.0},                         // PHYSIC_REACTIVE_POWER REACTIV_POWER_KVAR
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ANGLE ANGLE_DEGREES
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RPM RPM_RPM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TEMP TEMP_HZ_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RESIST_ISOLATION RESISTANCE_OM
   {1000.0, 0.0},                         // PHYSIC_RESIST_ISOLATION RESISTANCE_KOM
   {1000000.0, 0.0},                      // PHYSIC_RESIST_ISOLATION RESISTANCE_MOM
   {0.001, 0.0},                          // PHYSIC_RESIST_ISOLATION RESISTANCE_MILIOM
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RESIST_CABLE RESIST_CABLE_KOM_M
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_PRESSURE PRESSURE_MPA
   {0.1013250, 0.0},                      // PHYSIC_PRESSURE PRESSURE_ATM
   {0.0980665, 0.0},                      // PHYSIC_PRESSURE PRESSURE_AT
   {0.1000000, 0.0},                      // PHYSIC_PRESSURE PRESSURE_BAR
   {0.0068948, 0.0},                      // PHYSIC_PRESSURE PRESSURE_PSI
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TEMPERATURE TEMPERATURE_C
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACCELERATION ACCELERATION_G
   {0.101972, 0.0},                       // PHYSIC_ACCELERATION ACCELERATION_MSS2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_SPEED SPEED_M_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_VISCOSITY VISCOSITY_MPA_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FLOW FLOW_M3_DD
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_POWER POWER_W
   {1000.0, 0.0},                         // POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACTIVE_ENERGY ACTIVE_ENERGY_W_HH
   {1000.0, 0.0},                         // PHYSIC_ACTIVE_ENERGY ACTIVE_ENERGY_KW_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_REACTIVE_ENERGY REACTIVE_ENERGY_VAR_HH
   {1000.0, 0.0},                         // PHYSIC_REACTIVE_ENERGY REACTIVE_ENERGY_KVAR_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FULL_ENERGY FULL_ENERGY_VAHH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RATE RATE_NM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_INDUNCTANCE INDUNCTANCE_MH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}}
};

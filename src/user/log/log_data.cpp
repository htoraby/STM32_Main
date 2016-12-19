#include "log_data.h"
#include "user_main.h"
#include <string.h>

LogData::LogData() : Log(DataTypeLog)
{

}

LogData::~LogData()
{

}

static void logDataTask(void *p)
{
  (static_cast<LogData*>(p))->task();
}

void LogData::init()
{
  Log::init();

  osThreadDef_t t = {"LogData", logDataTask, osPriorityNormal, 0, 3 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void LogData::deInit()
{
  osThreadTerminate(threadId_);
  Log::deInit();
}

void LogData::task()
{
  int normTimeCnt = 0;
  int fastTimeCnt = 0;
  bool startFastMode = false;

  while (1) {
    osDelay(1000);

    if (ksu.isDelay()) {
      if (!startFastMode) {
        startFastMode = true;
        add(FastModeCode);
      } else {
        int period = parameters.get(CCS_LOG_PERIOD_FAST);
        if (++fastTimeCnt >= period) {
          fastTimeCnt = 0;
          add(FastModeCode);
        }
      }
    } else {
      startFastMode = false;
      fastTimeCnt = 0;
    }

    int period = parameters.get(CCS_LOG_PERIOD_NORMAL);
    if (++normTimeCnt >= period) {
      normTimeCnt = 0;
      if (!ksu.isDelay())
        add(NormModeCode);
    }
  }
}

void LogData::add(uint8_t code)
{
  memset(buffer, 0, sizeof(buffer));
  float tempVal = 0.0;
  time_t time = ksu.getTime();

  ++id_;
  *(uint32_t*)(buffer) = id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;

  *(float*)(buffer+9) = parameters.get(VSD_FREQUENCY_NOW);                          // Электон
  *(float*)(buffer+13) = parameters.get(CCS_MOTOR_CURRENT_PHASE_1);                 // Электон
  *(float*)(buffer+17) = parameters.get(CCS_MOTOR_CURRENT_PHASE_2);                 // Электон
  *(float*)(buffer+21) = parameters.get(CCS_MOTOR_CURRENT_PHASE_3);                 // Электон
  *(float*)(buffer+25) = parameters.get(CCS_MOTOR_CURRENT_IMBALANCE);               // Электон
  *(float*)(buffer+29) = parameters.get(CCS_MOTOR_VOLTAGE_NOW);                     // Электон
  *(float*)(buffer+33) = parameters.get(VSD_CURRENT_DC);                            // Электон
  *(float*)(buffer+37) = parameters.get(VSD_VOLTAGE_DC);                            // Электон
  *(float*)(buffer+41) = parameters.get(VSD_OUT_VOLTAGE_MOTOR);                     // Электон
  tempVal = parameters.get(VSD_POWER_ACTIVE) / 1000.0;
  *(float*)(buffer+45) = tempVal;
  //*(float*)(buffer+45) = parameters.get(VSD_POWER_ACTIVE);                          // Электон
  tempVal = parameters.get(VSD_POWER_FULL) / 1000.0;
  *(float*)(buffer+49) = tempVal;
  //*(float*)(buffer+49) = parameters.get(VSD_POWER_FULL);                            // Электон
  *(float*)(buffer+53) = parameters.get(CCS_MOTOR_COS_PHI_NOW);                     // Электон
  *(float*)(buffer+57) = parameters.get(CCS_MOTOR_LOAD_NOW);                        // Электон
  *(float*)(buffer+61) = parameters.get(CCS_VOLTAGE_PHASE_1);                       // У Электона AB
  *(float*)(buffer+65) = parameters.get(CCS_VOLTAGE_PHASE_2);                       // У Электона BC
  *(float*)(buffer+69) = parameters.get(CCS_VOLTAGE_PHASE_3);                       // У Электона CA
  *(float*)(buffer+73) = parameters.get(CCS_RESISTANCE_ISOLATION);                  // Электон
  *(float*)(buffer+77) = parameters.get(TMS_PRESSURE_INTAKE);                       // Электон
  *(float*)(buffer+81) = parameters.get(TMS_FLOW_DISCHARGE);                        // Электон
  *(float*)(buffer+85) = parameters.get(TMS_TEMPERATURE_WINDING);                   // Электон
  *(float*)(buffer+89) = parameters.get(TMS_TEMPERATURE_INTAKE);                    // Электон
  *(float*)(buffer+93) = parameters.get(TMS_PRESSURE_DISCHARGE);                    // Электон
  *(float*)(buffer+97) = parameters.get(TMS_TEMPERATURE_DISCHARGE);                 // Электон
  *(float*)(buffer+101) = parameters.get(TMS_ACCELERATION_XY_INTAKE);               // Электон
  *(float*)(buffer+105) = parameters.get(TMS_ACCELERATION_Z_INTAKE);                // Электон
  *(float*)(buffer+109) = parameters.get(CCS_TEMPERATURE_CCS);                      // Электон
  *(float*)(buffer+113) = parameters.get(VSD_RADIATOR_TEMPERATURE);                 // Электон
  *(float*)(buffer+117) = parameters.get(VSD_CONTROL_TEMPERATURE);                  // Электон
  tempVal = parameters.get(EM_ACTIVE_POWER) / 1000.0;
  *(float*)(buffer+121) = tempVal;
  //*(float*)(buffer+121) = parameters.get(EM_ACTIVE_POWER);                          // Электон
  tempVal = parameters.get(EM_REACTIVE_POWER) / 1000.0;
  *(float*)(buffer+125) = tempVal;
  //*(float*)(buffer+125) = parameters.get(EM_REACTIVE_POWER);                        // Электон
  *(float*)(buffer+129) = parameters.get(EM_COS_PHI);                               // Электон
  *(float*)(buffer+133) = parameters.get(CCS_CURRENT_PHASE_1);
  *(float*)(buffer+137) = parameters.get(CCS_CURRENT_PHASE_2);
  *(float*)(buffer+141) = parameters.get(CCS_CURRENT_PHASE_3);
  tempVal = parameters.get(EM_FULL_POWER) / 1000.0;
  *(float*)(buffer+145) = tempVal;
  //*(float*)(buffer+145) = parameters.get(EM_FULL_POWER);
  *(float*)(buffer+149) = parameters.get(CCS_AI_1_VALUE);
  *(float*)(buffer+153) = parameters.get(CCS_AI_2_VALUE);
  *(float*)(buffer+157) = parameters.get(CCS_AI_3_VALUE);
  *(float*)(buffer+161) = parameters.get(CCS_AI_4_VALUE);
  *(float*)(buffer+165) = parameters.get(CCS_DI_1_VALUE);
  *(float*)(buffer+169) = parameters.get(CCS_DI_2_VALUE);
  *(float*)(buffer+173) = parameters.get(CCS_DI_3_VALUE);
  *(float*)(buffer+177) = parameters.get(CCS_DI_4_VALUE);
  *(float*)(buffer+181) = parameters.get(CCS_CONDITION);
  *(float*)(buffer+185) = parameters.get(CCS_VSD_CONDITION);
  *(float*)(buffer+189) = parameters.get(CCS_WORKING_MODE);
  *(float*)(buffer+193) = parameters.get(CCS_VSD_STATUS_WORD_1);
  *(float*)(buffer+197) = parameters.get(CCS_VSD_STATUS_WORD_2);
  *(float*)(buffer+201) = parameters.get(CCS_VSD_STATUS_WORD_3);
  *(float*)(buffer+205) = parameters.get(CCS_VSD_STATUS_WORD_4);
  *(float*)(buffer+209) = parameters.get(CCS_VSD_STATUS_WORD_5);
  *(float*)(buffer+213) = parameters.get(CCS_VSD_STATUS_WORD_6);
  *(float*)(buffer+217) = parameters.get(CCS_VSD_STATUS_WORD_7);
  *(float*)(buffer+221) = parameters.get(TMS_PSW_TMS);
  *(float*)(buffer+225) = parameters.get(TMS_PSW_TMSN);
  *(float*)(buffer+229) = parameters.get(CCS_VOLTAGE_IMBALANCE_IN);
  *(float*)(buffer+233) = parameters.get(CCS_VOLTAGE_TRANS_OUT);
  *(float*)(buffer+237) = parameters.get(CCS_TURBO_ROTATION_NOW);
  *(float*)(buffer+241) = parameters.get(VSD_ROTATION);
//  *(float*)(buffer+245) =
//  *(float*)(buffer+249) =

#if (HARDWARE_VERSION >= 0x0200)
  write(buffer, 253, false);

  memset(buffer, 0, sizeof(buffer));
  write(buffer, 256);
#else
  write(buffer, 253);
#endif
}


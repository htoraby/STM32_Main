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

  osThreadDef_t t = {"LogData", logDataTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void LogData::task()
{
  int normTimeCnt = 0;
  int fastTimeCnt = 0;
  bool startFastMode = false;

  while (1) {
    osDelay(1000);

    int condition = KSU.getValue(CCS_PERIOD_LOG_NOLMAL_MODE);
#if DEBUG
//    condition = CCS_CONDITION_DELAY;
#endif
    if (condition == CCS_CONDITION_DELAY) {
      if (!startFastMode) {
        startFastMode = true;
        add(FastModeCode);
      } else {
        int period = KSU.getValue(CCS_PERIOD_LOG_FAST_MODE);
#if DEBUG
        period = 5;
#endif
        if (++fastTimeCnt >= period) {
          fastTimeCnt = 0;
          add(FastModeCode);
        }
      }
    } else {
      startFastMode = false;
      fastTimeCnt = 0;
    }

    int period = KSU.getValue(CCS_PERIOD_LOG_NOLMAL_MODE);
#if DEBUG
    period = 60;
#endif
    if (++normTimeCnt >= period) {
      normTimeCnt = 0;
      if (condition != CCS_CONDITION_DELAY)
        add(NormModeCode);
    }
  }
}

void LogData::add(uint8_t code)
{
  memset(buffer, 0, 253);

  time_t time = getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint32_t*)(buffer+8) = code;
  *(uint32_t*)(buffer+9) = KSU.getValue(CCS_NUMBER_WELL);
  *(uint32_t*)(buffer+13) = KSU.getValue(VSD_CURRENT_FREQUENCY);
  *(uint32_t*)(buffer+17) = KSU.getValue(VSD_OUT_CURRENT_PHASE_1);
  *(uint32_t*)(buffer+21) = KSU.getValue(VSD_OUT_CURRENT_PHASE_2);
  *(uint32_t*)(buffer+25) = KSU.getValue(VSD_OUT_CURRENT_PHASE_3);
  *(uint32_t*)(buffer+29) = KSU.getValue(VSD_VOLTAGE_MOTOR);
  *(uint32_t*)(buffer+33) = KSU.getValue(CCS_PROT_DISBALANCE_CURRENT_VALUE);
  *(uint32_t*)(buffer+37) = KSU.getValue(VSD_COS_PHI_MOTOR);
  *(uint32_t*)(buffer+41) = KSU.getValue(CCS_LOAD_MOTOR);
  *(uint32_t*)(buffer+45) = KSU.getValue(VSD_ACTIVE_POWER);

//  *(uint32_t*)(buffer+49) = KSU.getValue();
  *(uint32_t*)(buffer+53) = KSU.getValue(EM_VOLTAGE_PHASE_1_2);
  *(uint32_t*)(buffer+57) = KSU.getValue(EM_VOLTAGE_PHASE_2_3);
  *(uint32_t*)(buffer+61) = KSU.getValue(EM_VOLTAGE_PHASE_3_1);
  *(uint32_t*)(buffer+65) = KSU.getValue(CCS_PROT_DISBALANCE_VOLTAGE_VALUE);
  *(uint32_t*)(buffer+69) = KSU.getValue(EM_CURRENT_PHASE_1);
  *(uint32_t*)(buffer+73) = KSU.getValue(EM_CURRENT_PHASE_2);
  *(uint32_t*)(buffer+77) = KSU.getValue(EM_CURRENT_PHASE_3);
//  *(uint32_t*)(buffer+81) = KSU.getValue();
  *(uint32_t*)(buffer+85) = KSU.getValue(CCS_RESISTANCE_ISOLATION);

//  *(uint32_t*)(buffer+89) = KSU.getValue();
  *(uint32_t*)(buffer+93) = KSU.getValue(TMS_TEMPERATURE_WINDING);
  *(uint32_t*)(buffer+97) = KSU.getValue(TMS_PRESSURE_INTAKE);
  *(uint32_t*)(buffer+101) = KSU.getValue(TMS_TEMPERATURE_INTAKE);
  *(uint32_t*)(buffer+105) = KSU.getValue(EM_ACTIVE_POWER);
  *(uint32_t*)(buffer+109) = KSU.getValue(EM_FULL_POWER);
  *(uint32_t*)(buffer+113) = KSU.getValue(VSD_OUT_CURRENT_MOTOR);
  *(uint32_t*)(buffer+117) = KSU.getValue(VSD_OUT_VOLTAGE_MOTOR);
//  *(uint32_t*)(buffer+121) = KSU.getValue();
//  *(uint32_t*)(buffer+125) = KSU.getValue();

//  *(uint32_t*)(buffer+129) = KSU.getValue();
  *(uint32_t*)(buffer+133) = KSU.getValue(TMS_TEMPERATURE_DISCHARGE);
  *(uint32_t*)(buffer+137) = KSU.getValue(TMS_PRESSURE_DISCHARGE);
//  *(uint32_t*)(buffer+141) = KSU.getValue();
//  *(uint32_t*)(buffer+145) = KSU.getValue();
  *(uint32_t*)(buffer+149) = KSU.getValue(TMS_FLOW_DISCHARGE);
  *(uint32_t*)(buffer+153) = KSU.getValue(VSD_RADIATOR_TEMPERATURE);
  *(uint32_t*)(buffer+157) = KSU.getValue(VSD_CONTROL_TEMPERATURE);
//  *(uint32_t*)(buffer+161) = KSU.getValue();
  *(uint32_t*)(buffer+165) = KSU.getValue(CCS_CONDITION);

  *(uint32_t*)(buffer+169) = KSU.getValue(CCS_WORKING_MODE);
//  *(uint32_t*)(buffer+173) = KSU.getValue();
//  *(uint32_t*)(buffer+177) = KSU.getValue();
  *(uint32_t*)(buffer+181) = KSU.getValue(TMS_PSW_TMS);
  *(uint32_t*)(buffer+185) = KSU.getValue(TMS_PSW_TMSN);
  *(uint32_t*)(buffer+189) = KSU.getValue(VSD_DC_VOLTAGE);
  *(uint32_t*)(buffer+193) = KSU.getValue(VSD_STATUS_WORD);
  *(uint32_t*)(buffer+197) = KSU.getValue(VSD_ALARM_WORD_1);
  *(uint32_t*)(buffer+201) = KSU.getValue(VSD_ALARM_WORD_2);
  *(uint32_t*)(buffer+205) = KSU.getValue(VSD_WARNING_WORD_1);

  *(uint32_t*)(buffer+209) = KSU.getValue(VSD_WARNING_WORD_2);
//  *(uint32_t*)(buffer+213) = KSU.getValue();
  *(uint32_t*)(buffer+217) = KSU.getValue(VSD_LAST_ALARM);
//  *(uint32_t*)(buffer+221) = KSU.getValue();
  *(uint32_t*)(buffer+225) = KSU.getValue(CCS_RGM_PERIODIC_MODE);
  *(uint32_t*)(buffer+229) = KSU.getValue(CCS_RGM_PERIODIC_TIME);
  *(uint32_t*)(buffer+233) = KSU.getValue(CCS_RGM_PERIODIC_TIME);
  *(uint32_t*)(buffer+237) = KSU.getValue(CCS_RGM_PERIODIC_TIME_END);
  *(uint32_t*)(buffer+241) = KSU.getValue(CCS_RGM_PERIODIC_TIME_END);

//  *(uint32_t*)(buffer+245) = KSU.getValue();

//  *(uint32_t*)(buffer+249) = KSU.getValue();

  write(buffer, 253);
}


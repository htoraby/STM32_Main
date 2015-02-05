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

    int condition = ksu.getValue(CCS_CONDITION);
#if DEBUG
//    condition = CCS_CONDITION_DELAY;
#endif
    if (condition == CCS_CONDITION_DELAY) {
      if (!startFastMode) {
        startFastMode = true;
        add(FastModeCode);
      } else {
        int period = ksu.getValue(CCS_PERIOD_LOG_FAST_MODE);
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

    int period = ksu.getValue(CCS_PERIOD_LOG_NOLMAL_MODE);
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
  memset(buffer, 0, sizeof(buffer));

  time_t time = getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;
  *(float*)(buffer+9) = ksu.getValue(CCS_NUMBER_WELL);
  *(float*)(buffer+13) = vsd->getValue(VSD_CURRENT_FREQUENCY);
  *(float*)(buffer+17) = vsd->getValue(VSD_OUT_CURRENT_PHASE_1);
  *(float*)(buffer+21) = vsd->getValue(VSD_OUT_CURRENT_PHASE_2);
  *(float*)(buffer+25) = vsd->getValue(VSD_OUT_CURRENT_PHASE_3);
  *(float*)(buffer+29) = vsd->getValue(VSD_VOLTAGE_MOTOR);
  *(float*)(buffer+33) = ksu.getValue(CCS_MOTOR_IMBALANCE_CURRENT);
  *(float*)(buffer+37) = vsd->getValue(VSD_COS_PHI_MOTOR);
  *(float*)(buffer+41) = ksu.getValue(CCS_LOAD_MOTOR);
  *(float*)(buffer+45) = vsd->getValue(VSD_ACTIVE_POWER);

//  *(float*)(buffer+49) = KSU.getValue();
  *(float*)(buffer+53) = em->getValue(EM_VOLTAGE_PHASE_1_2);
  *(float*)(buffer+57) = em->getValue(EM_VOLTAGE_PHASE_2_3);
  *(float*)(buffer+61) = em->getValue(EM_VOLTAGE_PHASE_3_1);
  *(float*)(buffer+65) = em->getValue(CCS_SYPPLY_IMBALANCE_VOLTAGE);
  *(float*)(buffer+69) = em->getValue(EM_CURRENT_PHASE_1);
  *(float*)(buffer+73) = em->getValue(EM_CURRENT_PHASE_2);
  *(float*)(buffer+77) = em->getValue(EM_CURRENT_PHASE_3);
//  *(float*)(buffer+81) = KSU.getValue();
  *(float*)(buffer+85) = ksu.getValue(CCS_RESISTANCE_ISOLATION);

//  *(float*)(buffer+89) = KSU.getValue();
  *(float*)(buffer+93) = tms->getValue(TMS_TEMPERATURE_WINDING);
  *(float*)(buffer+97) = tms->getValue(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+101) = tms->getValue(TMS_TEMPERATURE_INTAKE);
  *(float*)(buffer+105) = em->getValue(EM_ACTIVE_POWER);
  *(float*)(buffer+109) = em->getValue(EM_FULL_POWER);
  *(float*)(buffer+113) = vsd->getValue(VSD_OUT_CURRENT_MOTOR);
  *(float*)(buffer+117) = vsd->getValue(VSD_OUT_VOLTAGE_MOTOR);
//  *(float*)(buffer+121) = KSU.getValue();
//  *(float*)(buffer+125) = KSU.getValue();

//  *(float*)(buffer+129) = KSU.getValue();
  *(float*)(buffer+133) = tms->getValue(TMS_TEMPERATURE_DISCHARGE);
  *(float*)(buffer+137) = tms->getValue(TMS_PRESSURE_DISCHARGE);
//  *(float*)(buffer+141) = KSU.getValue();
//  *(float*)(buffer+145) = KSU.getValue();
  *(float*)(buffer+149) = tms->getValue(TMS_FLOW_DISCHARGE);
  *(float*)(buffer+153) = vsd->getValue(VSD_RADIATOR_TEMPERATURE);
  *(float*)(buffer+157) = vsd->getValue(VSD_CONTROL_TEMPERATURE);
//  *(float*)(buffer+161) = KSU.getValue();
  *(float*)(buffer+165) = ksu.getValue(CCS_CONDITION);

  *(float*)(buffer+169) = ksu.getValue(CCS_WORKING_MODE);
//  *(float*)(buffer+173) = KSU.getValue();
//  *(float*)(buffer+177) = KSU.getValue();
  *(float*)(buffer+181) = tms->getValue(TMS_PSW_TMS);
  *(float*)(buffer+185) = tms->getValue(TMS_PSW_TMSN);
  *(float*)(buffer+189) = vsd->getValue(VSD_DC_VOLTAGE);
  *(float*)(buffer+193) = vsd->getValue(VSD_STATUS_WORD);
  *(float*)(buffer+197) = vsd->getValue(VSD_ALARM_WORD_1);
  *(float*)(buffer+201) = vsd->getValue(VSD_ALARM_WORD_2);
  *(float*)(buffer+205) = vsd->getValue(VSD_WARNING_WORD_1);

  *(float*)(buffer+209) = vsd->getValue(VSD_WARNING_WORD_2);
//  *(float*)(buffer+213) = KSU.getValue();
  *(float*)(buffer+217) = vsd->getValue(VSD_LAST_ALARM);
//  *(float*)(buffer+221) = KSU.getValue();
  *(float*)(buffer+225) = ksu.getValue(CCS_RGM_PERIODIC_MODE);
  *(float*)(buffer+229) = ksu.getValue(CCS_RGM_PERIODIC_TIME);
  *(float*)(buffer+233) = ksu.getValue(CCS_RGM_PERIODIC_TIME);
  *(float*)(buffer+237) = ksu.getValue(CCS_RGM_PERIODIC_TIME_END);
  *(float*)(buffer+241) = ksu.getValue(CCS_RGM_PERIODIC_TIME_END);

//  *(float*)(buffer+245) = KSU.getValue();

//  *(float*)(buffer+249) = KSU.getValue();

  write(buffer, 253);
}


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

    int condition = parameters.getValue(CCS_CONDITION);
#if DEBUG
//    condition = CCS_CONDITION_DELAY;
#endif
    if (condition == CCS_CONDITION_DELAY) {
      if (!startFastMode) {
        startFastMode = true;
        add(FastModeCode);
      } else {
        int period = parameters.getValue(CCS_LOG_PERIOD_FAST);
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

    int period = parameters.getValue(CCS_LOG_PERIOD_NORMAL);
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

  time_t time = ksu.getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;

  *(float*)(buffer+9) = parameters.getValue(CCS_NUMBER_WELL);
  *(float*)(buffer+13) = parameters.getValue(VSD_FREQUENCY_NOW);
  *(float*)(buffer+17) = parameters.getValue(VSD_CURRENT_OUT_PHASE_1);
  *(float*)(buffer+21) = parameters.getValue(VSD_CURRENT_OUT_PHASE_2);
  *(float*)(buffer+25) = parameters.getValue(VSD_CURRENT_OUT_PHASE_3);
  *(float*)(buffer+29) = parameters.getValue(VSD_MOTOR_VOLTAGE);
  *(float*)(buffer+33) = parameters.getValue(CCS_MOTOR_CURRENT_IMBALANCE);
  *(float*)(buffer+37) = parameters.getValue(VSD_MOTOR_COS_PHI);
  *(float*)(buffer+41) = parameters.getValue(CCS_MOTOR_LOAD_NOW);
  *(float*)(buffer+45) = parameters.getValue(VSD_POWER_ACTIVE);

  *(float*)(buffer+49) = parameters.getValue(VSD_POWER_FULL);
  *(float*)(buffer+53) = parameters.getValue(EM_VOLTAGE_PHASE_1);
  *(float*)(buffer+57) = parameters.getValue(EM_VOLTAGE_PHASE_2);
  *(float*)(buffer+61) = parameters.getValue(EM_VOLTAGE_PHASE_3);
  *(float*)(buffer+65) = parameters.getValue(CCS_VOLTAGE_IMBALANCE_IN);
  *(float*)(buffer+69) = parameters.getValue(EM_CURRENT_PHASE_1);
  *(float*)(buffer+73) = parameters.getValue(EM_CURRENT_PHASE_2);
  *(float*)(buffer+77) = parameters.getValue(EM_CURRENT_PHASE_3);
  *(float*)(buffer+81) = parameters.getValue(CCS_CURRENT_IMBALANCE_IN);
  *(float*)(buffer+85) = parameters.getValue(CCS_RESISTANCE_ISOLATION);

//  *(float*)(buffer+89) = parameters.getValue(TMS_); // TMS_ACTIVE_CURRENT_LEAKAGE|Iутеч(мА) Активные токи утечки
  *(float*)(buffer+93) = parameters.getValue(TMS_TEMPERATURE_WINDING);
  *(float*)(buffer+97) = parameters.getValue(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+101) = parameters.getValue(TMS_TEMPERATURE_INTAKE);
  *(float*)(buffer+105) = parameters.getValue(EM_ACTIVE_POWER);
  *(float*)(buffer+109) = parameters.getValue(EM_FULL_POWER);
  *(float*)(buffer+113) = parameters.getValue(VSD_OUT_CURRENT_MOTOR);
  *(float*)(buffer+117) = parameters.getValue(VSD_OUT_VOLTAGE_MOTOR);
  *(float*)(buffer+121) = parameters.getValue(EM_ACTIVE_ENERGY);
  *(float*)(buffer+125) = parameters.getValue(TMS_ACCELERATION_XY_INTAKE);

  *(float*)(buffer+129) = parameters.getValue(TMS_ACCELERATION_Z_INTAKE);
  *(float*)(buffer+133) = parameters.getValue(TMS_TEMPERATURE_DISCHARGE);
  *(float*)(buffer+137) = parameters.getValue(TMS_PRESSURE_DISCHARGE);
//  *(float*)(buffer+141) = parameters.getValue(RESERVE);
//  *(float*)(buffer+145) = parameters.getValue(RESERVE);
  *(float*)(buffer+149) = parameters.getValue(TMS_FLOW_DISCHARGE);
  *(float*)(buffer+153) = parameters.getValue(VSD_RADIATOR_TEMPERATURE);
  *(float*)(buffer+157) = parameters.getValue(VSD_CONTROL_TEMPERATURE);
//  *(float*)(buffer+161) = parameters.getValue(); // TEMPERATURE_CY_PK|Тбу(°С) Температура платы контроллера
  *(float*)(buffer+165) = parameters.getValue(CCS_CONDITION);

  *(float*)(buffer+169) = parameters.getValue(CCS_WORKING_MODE);
//  *(float*)(buffer+173) = parameters.getValue(); // TIME_PROCESS|Траб/стоп Время работы/стопа
//  *(float*)(buffer+177) = parameters.getValue(); // PSW_CY_PK|PSWПк Слово состяние платы контроллера
  *(float*)(buffer+181) = parameters.getValue(TMS_PSW_TMS);
  *(float*)(buffer+185) = parameters.getValue(TMS_PSW_TMSN);
  *(float*)(buffer+189) = parameters.getValue(VSD_VOLTAGE_DC);
  *(float*)(buffer+193) = parameters.getValue(VSD_STATUS_WORD);
  *(float*)(buffer+197) = parameters.getValue(VSD_ALARM_WORD_1);
  *(float*)(buffer+201) = parameters.getValue(VSD_ALARM_WORD_2);
  *(float*)(buffer+205) = parameters.getValue(VSD_WARNING_WORD_1);

  *(float*)(buffer+209) = parameters.getValue(VSD_WARNING_WORD_2);
//  *(float*)(buffer+213) = parameters.getValue(); // WARNING_WORD_EXT|WarnEXT Слово состояния ЧРП расширенное
  *(float*)(buffer+217) = parameters.getValue(VSD_LAST_ALARM);
  *(float*)(buffer+221) = parameters.getValue(CCS_TURBO_ROTATION_NOW);
  *(float*)(buffer+225) = parameters.getValue(CCS_RGM_PERIODIC_MODE);
  *(float*)(buffer+229) = parameters.getValue(CCS_RGM_PERIODIC_RUN_PERIOD);
  *(float*)(buffer+233) = parameters.getValue(CCS_RGM_PERIODIC_RUN_PERIOD);
  *(float*)(buffer+237) = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
  *(float*)(buffer+241) = parameters.getValue(CCS_RGM_PERIODIC_RUN_TIME_TO_END);

//  *(float*)(buffer+245) = parameters.getValue(RESERVE);
//  *(float*)(buffer+249) = parameters.getValue(RESERVE);

  write(buffer, 253);
}


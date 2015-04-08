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

  time_t time = ksu.getTime();

  *(uint32_t*)(buffer) = ++id_;
  *(uint32_t*)(buffer+4) = time;
  *(uint8_t*)(buffer+8) = code;

  *(float*)(buffer+9) = parameters.get(CCS_NUMBER_WELL);
  *(float*)(buffer+13) = parameters.get(VSD_FREQUENCY_NOW);
  *(float*)(buffer+17) = parameters.get(VSD_CURRENT_OUT_PHASE_1);
  *(float*)(buffer+21) = parameters.get(VSD_CURRENT_OUT_PHASE_2);
  *(float*)(buffer+25) = parameters.get(VSD_CURRENT_OUT_PHASE_3);
  *(float*)(buffer+29) = parameters.get(VSD_MOTOR_VOLTAGE);
  *(float*)(buffer+33) = parameters.get(CCS_MOTOR_CURRENT_IMBALANCE);
  *(float*)(buffer+37) = parameters.get(VSD_MOTOR_COS_PHI);
  *(float*)(buffer+41) = parameters.get(CCS_MOTOR_LOAD_NOW);
  *(float*)(buffer+45) = parameters.get(VSD_POWER_ACTIVE);

  *(float*)(buffer+49) = parameters.get(VSD_POWER_FULL);
  *(float*)(buffer+53) = parameters.get(EM_VOLTAGE_PHASE_1);
  *(float*)(buffer+57) = parameters.get(EM_VOLTAGE_PHASE_2);
  *(float*)(buffer+61) = parameters.get(EM_VOLTAGE_PHASE_3);
  *(float*)(buffer+65) = parameters.get(CCS_VOLTAGE_IMBALANCE_IN);
  *(float*)(buffer+69) = parameters.get(EM_CURRENT_PHASE_1);
  *(float*)(buffer+73) = parameters.get(EM_CURRENT_PHASE_2);
  *(float*)(buffer+77) = parameters.get(EM_CURRENT_PHASE_3);
  *(float*)(buffer+81) = parameters.get(CCS_CURRENT_IMBALANCE_IN);
  *(float*)(buffer+85) = parameters.get(CCS_RESISTANCE_ISOLATION);

//  *(float*)(buffer+89) = parameters.getValue(TMS_); // TMS_ACTIVE_CURRENT_LEAKAGE|Iутеч(мА) Активные токи утечки
  *(float*)(buffer+93) = parameters.get(TMS_TEMPERATURE_WINDING);
  *(float*)(buffer+97) = parameters.get(TMS_PRESSURE_INTAKE);
  *(float*)(buffer+101) = parameters.get(TMS_TEMPERATURE_INTAKE);
  *(float*)(buffer+105) = parameters.get(EM_ACTIVE_POWER);
  *(float*)(buffer+109) = parameters.get(EM_FULL_POWER);
  *(float*)(buffer+113) = parameters.get(VSD_OUT_CURRENT_MOTOR);
  *(float*)(buffer+117) = parameters.get(VSD_OUT_VOLTAGE_MOTOR);
  *(float*)(buffer+121) = parameters.get(EM_ACTIVE_ENERGY);
  *(float*)(buffer+125) = parameters.get(TMS_ACCELERATION_XY_INTAKE);

  *(float*)(buffer+129) = parameters.get(TMS_ACCELERATION_Z_INTAKE);
  *(float*)(buffer+133) = parameters.get(TMS_TEMPERATURE_DISCHARGE);
  *(float*)(buffer+137) = parameters.get(TMS_PRESSURE_DISCHARGE);
//  *(float*)(buffer+141) = parameters.getValue(RESERVE);
//  *(float*)(buffer+145) = parameters.getValue(RESERVE);
  *(float*)(buffer+149) = parameters.get(TMS_FLOW_DISCHARGE);
  *(float*)(buffer+153) = parameters.get(VSD_RADIATOR_TEMPERATURE);
  *(float*)(buffer+157) = parameters.get(VSD_CONTROL_TEMPERATURE);
//  *(float*)(buffer+161) = parameters.getValue(); // TEMPERATURE_CY_PK|Тбу(°С) Температура платы контроллера
  *(float*)(buffer+165) = parameters.get(CCS_CONDITION);

  *(float*)(buffer+169) = parameters.get(CCS_WORKING_MODE);
//  *(float*)(buffer+173) = parameters.getValue(); // TIME_PROCESS|Траб/стоп Время работы/стопа
//  *(float*)(buffer+177) = parameters.getValue(); // PSW_CY_PK|PSWПк Слово состяние платы контроллера
  *(float*)(buffer+181) = parameters.get(TMS_PSW_TMS);
  *(float*)(buffer+185) = parameters.get(TMS_PSW_TMSN);
  *(float*)(buffer+189) = parameters.get(VSD_VOLTAGE_DC);
  *(float*)(buffer+193) = parameters.get(VSD_STATUS_WORD);
  *(float*)(buffer+197) = parameters.get(VSD_ALARM_WORD_1);
  *(float*)(buffer+201) = parameters.get(VSD_ALARM_WORD_2);
  *(float*)(buffer+205) = parameters.get(VSD_WARNING_WORD_1);

  *(float*)(buffer+209) = parameters.get(VSD_WARNING_WORD_2);
//  *(float*)(buffer+213) = parameters.getValue(); // WARNING_WORD_EXT|WarnEXT Слово состояния ЧРП расширенное
  *(float*)(buffer+217) = parameters.get(VSD_LAST_ALARM);
  *(float*)(buffer+221) = parameters.get(CCS_TURBO_ROTATION_NOW);
  *(float*)(buffer+225) = parameters.get(CCS_RGM_PERIODIC_MODE);
  *(float*)(buffer+229) = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
  *(float*)(buffer+233) = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
  *(float*)(buffer+237) = parameters.get(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
  *(float*)(buffer+241) = parameters.get(CCS_RGM_PERIODIC_RUN_TIME_TO_END);

//  *(float*)(buffer+245) = parameters.getValue(RESERVE);
//  *(float*)(buffer+249) = parameters.getValue(RESERVE);

  write(buffer, 253);
}


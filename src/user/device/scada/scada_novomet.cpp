#include "scada_novomet.h"
#include "user_main.h"
#include "regime.h"

ScadaNovomet::ScadaNovomet()
{
  countParameters_ = 927;
  initParameters();
}

ScadaNovomet::~ScadaNovomet()
{

}

void ScadaNovomet::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 3
    uint16_t value = 0;
    if (ksu.isWorkMotor())
      value = 1;
    if (ksu.isDelay())
      value = 2;
    if (!ksu.isWorkMotor())
      value = 3;
    if (ksu.isRestart())
      value = 8;
    if (ksu.isBlock())
      value = 16;
    scadaParameters_[3].value.float_t = value;

    // 4
    value = 0;
    int reason = parameters.get(CCS_LAST_STOP_REASON);
    switch (reason) {
    case LastReasonStopOperator:
      value = 0; break;
    case LastReasonStopProgram:
      value = 1; break;
    case LastReasonStopRemote:
      value = 2; break;
    case LastReasonStopOverVoltIn:
      value = 3; break;
    case LastReasonStopUnderVoltIn:
      value = 4; break;
    case LastReasonStopImbalanceVoltIn:
      value = 5; break;
    case LastReasonStopOverloadMotor:
      value = 7; break;
    case LastReasonStopUnderloadMotor:
      value = 8; break;
    case LastReasonStopImbalanceCurMotor:
      value = 9; break;
      //      TODO:
      //      case LastReasonStopHackSu:
      //        value = 10; break;
    case LastReasonStopResistIsolation:
      value = 11; break;
    case LastReasonStopPressureIntake:
      value = 12; break;
    case LastReasonStopFlowDischarge:
      value = 13; break;
    case LastReasonStopPressureDischarge:
      value = 14; break;
    case LastReasonStopTemperatureMotor:
      value = 15; break;
    case LastReasonStopVibrationMotor:
      value = 16; break;
    case LastReasonStopOvernumberOfStart:
      value = 17; break;
    case LastReasonStopHackSu:
      value = 18; break;
    case LastReasonStopProtDigital1:
      value = 20; break;
    case LastReasonStopProtDigital2:
      value = 21; break;
    case LastReasonStopProtDigital3:
      value = 22; break;
    case LastReasonStopProtDigital4:
      value = 23; break;
    case LastReasonStopImbalanceCurIn:
      value = 24; break;
    case LastReasonStopOutOfSyncMotor:
      value = 25; break;
    case LastReasonStopMaxAnalog1: case LastReasonStopMinAnalog1:
      value = 26; break;
    case LastReasonStopMaxAnalog2: case LastReasonStopMinAnalog2:
      value = 27; break;
    case LastReasonStopMaxAnalog3: case LastReasonStopMinAnalog3:
      value = 28; break;
    case LastReasonStopMaxAnalog4: case LastReasonStopMinAnalog4:
      value = 29; break;
    case LastReasonStopCurrentMotor:
      value = 30; break;
      //    TODO:
      //    case LastReasonStopPressureIntake:
      //      value = 31; break;
      //    TODO:
      //    case LastReasonStopPressureIntake:
      //      value = 32; break;
    case LastReasonStopVsdNoConnect:
      value = 33; break;
    default:
      if (reason)
        value = 46;
      break;
    }
    scadaParameters_[4].value.float_t = value;

    //    10
    //    Невыход на заданную частоту
    //    31
    //    Снижение уровня на приёме насоса
    //    32
    //    Перегрев входного фильтра

    // 5
    value = 0;
    int condition = parameters.get(CCS_CONDITION);
    if (condition == CCS_CONDITION_STOP) {
      value = 2;
    } else {
      int workingMode = parameters.get(CCS_WORKING_MODE);
      switch (workingMode) {
      case CCS_WORKING_MODE_MANUAL:
        value = 0; break;
      case CCS_WORKING_MODE_AUTO: case CCS_WORKING_MODE_PROGRAM:
        value = 1; break;
      default:
        if (workingMode)
          value = 0;
        break;
      }
    }
    scadaParameters_[5].value.float_t = value;

    //93
    value = 0;
    value |= ((parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE) > 0) << 0);
    value |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) > 0) << 1);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE) > 0) << 2);
//    TODO:
//    Защита от максимального тока
//    value |= (parameters.get() << 4);
    value |= ((parameters.get(CCS_PROT_MOTOR_OVERLOAD_MODE) > 0) << 5);
    value |= ((parameters.get(CCS_PROT_MOTOR_UNDERLOAD_MODE) > 0) << 6);
    value |= ((parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE) > 0) << 7);
    value |= ((parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) > 0) << 8);
//    TODO:
//    Защита от невыхода на заданную частоту
//    value |= (parameters.get() << 9);
    value |= ((parameters.get(CCS_PROT_DHS_RESISTANCE_MODE) > 0) << 10);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) > 0) << 11);
//    TODO:
//    Защита от снижения расхода
//    value |= (parameters.get() << 12);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE) > 0) << 13);
    value |= ((parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE) > 0) << 14);
    value |= ((parameters.get(CCS_PROT_DHS_VIBRATION_MODE) > 0) << 15);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_MODE) > 0) << 16);
    value |= ((parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) > 0) << 17);
    value |= ((parameters.get(CCS_PROT_OTHER_VSD_MODE) > 0) << 18);
    value |= ((parameters.get(CCS_PROT_DI_1_MODE) > 0) << 19);
    value |= ((parameters.get(CCS_PROT_DI_2_MODE) > 0) << 20);
    value |= ((parameters.get(CCS_PROT_DI_3_MODE) > 0) << 21);
    value |= ((parameters.get(CCS_PROT_DI_4_MODE) > 0) << 22);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE) > 0) << 23);
    value |= ((parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE) > 0) << 24);
    value |= ((parameters.get(CCS_PROT_AI_1_MODE) > 0) << 27);
    value |= ((parameters.get(CCS_PROT_AI_2_MODE) > 0) << 28);
    value |= ((parameters.get(CCS_PROT_AI_3_MODE) > 0) << 29);
    value |= ((parameters.get(CCS_PROT_AI_4_MODE) > 0) << 30);
    value |= ((parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) > 0) << 31);

    scadaParameters_[88].value.float_t = value;

    //95
    value = 0;
    value |= ((parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE) > 0) << 0);
    value |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) > 0) << 1);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE) > 0) << 2);
    value |= ((parameters.get(CCS_PROT_SUPPLY_PHASE_ROTATION_MODE) > 0) << 3);
//    TODO:
//    Превышение максимального тока ПЧ
//    value |= (parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) << 4);
    value |= ((parameters.get(CCS_PROT_MOTOR_OVERLOAD_MODE) > 0) << 5);
    value |= ((parameters.get(CCS_PROT_MOTOR_UNDERLOAD_MODE) > 0) << 6);
    value |= ((parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE) > 0) << 7);
    value |= ((parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) > 0) << 8);
//    TODO:
//    Невыход на заданную частоту
//    value |= (parameters.get() << 9);
    value |= ((parameters.get(CCS_PROT_DHS_RESISTANCE_MODE) > 0) << 10);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) > 0) << 11);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) > 0) << 12);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE) > 0) << 13);
    value |= ((parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE) > 0) << 14);
    value |= ((parameters.get(CCS_PROT_DHS_VIBRATION_MODE) > 0) << 15);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_MODE) > 0) << 16);
    value |= ((parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) > 0) << 17);
    value |= ((parameters.get(CCS_PROT_OTHER_VSD_MODE) > 0) << 18);
    value |= ((parameters.get(CCS_PROT_DI_1_MODE) > 0) << 19);
    value |= ((parameters.get(CCS_PROT_DI_2_MODE) > 0) << 20);
    value |= ((parameters.get(CCS_PROT_DI_3_MODE) > 0) << 21);
    value |= ((parameters.get(CCS_PROT_DI_4_MODE) > 0) << 22);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE) > 0) << 23);
    value |= ((parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE) > 0) << 24);
    value |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) > 0) << 25);
    value |= ((parameters.get(CCS_PROT_AI_1_MODE) > 0) << 27);
    value |= ((parameters.get(CCS_PROT_AI_2_MODE) > 0) << 28);
    value |= ((parameters.get(CCS_PROT_AI_3_MODE) > 0) << 29);
    value |= ((parameters.get(CCS_PROT_AI_4_MODE) > 0) << 30);
    value |= ((parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) > 0) << 31);

    scadaParameters_[89].value.float_t = value;

    //97
    value = 0;
    reason = parameters.get(CCS_LAST_STOP_REASON);
    switch (reason) {
    case LastReasonStopOperator:
      value = 0; break;
    case LastReasonStopProgram:
      value = 2; break;
    case LastReasonStopRemote:
      value = 3; break;
    default:
      value = 1; break;
    }
    scadaParameters_[90].value.float_t = value;

    //129
    value = 0;
    int language = parameters.get(CCS_LANGUAGE);
    switch (language) {
    case RuLanguage:
      value = 1; break;
    case EnLanguage:
      value = 2; break;
    case EsLanguage:
      value = 3; break;
    }
    scadaParameters_[119].value.float_t = value;

    //133
    value = 0;
    int pressure = parameters.get(CCS_UNIT_PRESSURE);
    switch (pressure) {
    case PRESSURE_MPA:
      value = 0; break;
    case PRESSURE_ATM:
      value = 4; break;
    case PRESSURE_AT:
      value = 1; break;
    case PRESSURE_BAR:
      value = 2; break;
    case PRESSURE_PSI:
      value = 3; break;
    scadaParameters_[123].value.float_t = value;
    }

    //135
    value = 0;
    int temperature = parameters.get(CCS_UNIT_TEMPERATURE);
    switch (temperature) {
    case TEMPERATURE_C:
      value = 0; break;
    case TEMPERATURE_F:
      value = 1; break;
    }
    scadaParameters_[125].value.float_t = value;

    //137
    value = 0;
    int vibration = parameters.get(CCS_UNIT_VIBRATION);
    switch (vibration) {
    case ACCELERATION_G:
      value = 1; break;
    case ACCELERATION_MSS2:
      value = 2; break;
    }
    scadaParameters_[127].value.float_t = value;

    //179
    value = 0;
    int rotation = parameters.get(VSD_ROTATION);
    switch (rotation) {
    case VSD_ROTATION_DIRECT:
      value = 1; break;
    case VSD_ROTATION_REVERSE:
      value = 2; break;
    }
    scadaParameters_[168].value.float_t = value;

    //215
    value = 0;
    uint8_t type = parameters.get(CCS_DHS_TYPE);
    switch (type) {
    case TYPE_DHS_NOVOMET:
      value = 1; break;
    case TYPE_DHS_ALMAZ:
      value = 12; break;
    case TYPE_DHS_BORETS:
      value = 7; break;
    case TYPE_DHS_ELEKTON_2:
    case TYPE_DHS_ELEKTON_3:
      value = 2; break;
    case TYPE_DHS_ETALON:
      value = 6; break;
    case TYPE_DHS_IRZ:
      value = 8; break;
    case TYPE_DHS_ORION:
      value = 9; break;
    case TYPE_DHS_PIC_V2:
      value = 11; break;
    case TYPE_DHS_SCAD:
      value = 10; break;
    case TYPE_DHS_SCAN:
      value = 3; break;
    case TYPE_DHS_TRIOL:
      value = 5; break;
    case TYPE_DHS_ZENIT:
      value = 4; break;
    }
    scadaParameters_[204].value.float_t = value;

    //237
    value = 0;
    value |= ((parameters.get(CCS_RGM_MAINTENANCE_PARAM_MODE) > 0) << 20);
    value |= ((parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE) > 0) << 26);
    value |= ((parameters.get(CCS_RGM_JARRING_MODE) > 0) << 27);
    value |= ((parameters.get(CCS_RGM_CHANGE_FREQ_MODE) > 0) << 29);
    value |= ((parameters.get(CCS_RGM_PERIODIC_MODE) > 0) << 30);
    scadaParameters_[223].value.float_t = value;

    //271
    value = 0;
    int runningType = parameters.get(CCS_RUNNING_TYPE);
    switch (runningType) {
    case Regime::SmoothRegimeRun:
      value = 0; break;
    case Regime::PushRegimeRun:
      value = 2; break;
    case Regime::SwingRegimeRun:
      value = 1; break;
    case Regime::SyncRegimeRun:
      value = 3; break;
    }
    scadaParameters_[251].value.float_t = value;

    //415
    value = 0;
    value |= ((parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE) > 0) << 0);
    value |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) > 0) << 1);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE) > 0) << 2);
//    TODO:
//    Защита от максимального тока
//    value |= (parameters.get() << 4);
    value |= ((parameters.get(CCS_PROT_MOTOR_OVERLOAD_MODE) > 0) << 5);
    value |= ((parameters.get(CCS_PROT_MOTOR_UNDERLOAD_MODE) > 0) << 6);
    value |= ((parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE) > 0) << 7);
    value |= ((parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) > 0) << 8);
//    TODO:
//    Защита от невыхода на заданную частоту
//    value |= (parameters.get() << 9);
    value |= ((parameters.get(CCS_PROT_DHS_RESISTANCE_MODE) > 0) << 10);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) > 0) << 11);
//    TODO:
//    Защита от снижения расхода
//    value |= (parameters.get() << 12);
    value |= ((parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE) > 0) << 13);
    value |= ((parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE) > 0) << 14);
    value |= ((parameters.get(CCS_PROT_DHS_VIBRATION_MODE) > 0) << 15);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_MODE) > 0) << 16);
    value |= ((parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) > 0) << 17);
    value |= ((parameters.get(CCS_PROT_OTHER_VSD_MODE) > 0) << 18);
    value |= ((parameters.get(CCS_PROT_DI_1_MODE) > 0) << 19);
    value |= ((parameters.get(CCS_PROT_DI_2_MODE) > 0) << 20);
    value |= ((parameters.get(CCS_PROT_DI_3_MODE) > 0) << 21);
    value |= ((parameters.get(CCS_PROT_DI_4_MODE) > 0) << 22);
    value |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE) > 0) << 23);
    value |= ((parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE) > 0) << 24);
    value |= ((parameters.get(CCS_PROT_AI_1_MODE) > 0) << 27);
    value |= ((parameters.get(CCS_PROT_AI_2_MODE) > 0) << 28);
    value |= ((parameters.get(CCS_PROT_AI_3_MODE) > 0) << 29);
    value |= ((parameters.get(CCS_PROT_AI_4_MODE) > 0) << 30);
    value |= ((parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) > 0) << 31);

    scadaParameters_[384].value.float_t = value;
  }
}

int ScadaNovomet::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  //346-348, 634-636
  if (((param->address >= 346) && (param->address <= 348)) ||
      ((param->address >= 634) && (param->address <= 636))) {
    unTypeData value;
    value.uint32_t = param->value.float_t;
    time_t time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 634: case 346:
      dateTime.tm_year = value.char_t[1] + 100;
      if (value.uint16_t[0] > 0)
        dateTime.tm_mon = value.char_t[0] - 1;
      break;
    case 635: case 347:
      dateTime.tm_mday = value.char_t[1];
      if(dateTime.tm_mday == 0)
        dateTime.tm_mday = 1;
      dateTime.tm_hour = value.char_t[0];
      break;
    case 636: case 348:
      dateTime.tm_min = value.char_t[1];
      dateTime.tm_sec = value.char_t[0];
      break;
    }

    time = mktime(&dateTime);
    parameters.set(CCS_DATE_TIME, (uint32_t)time, RemoteType);
    return ok_r;
  }

  return err_r;
}

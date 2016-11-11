#include "scada_novomet.h"
#include "user_main.h"
#include "regime.h"
#include "protection.h"

ScadaNovomet::ScadaNovomet()
{
  countParameters_ = 928;
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

    uint16_t value = 0;
    unTypeData data;

    // 3
    if (ksu.isWorkMotor())
      value = 1;
    if (ksu.isDelay())
      value = 2;
    if (!ksu.isWorkMotor())
      value = 4;
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
    data.uint32_t = 0;
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE) == Protection::ProtModeRestart) << 0);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) == Protection::ProtModeRestart) << 1);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE) == Protection::ProtModeRestart) << 2);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) == Protection::ProtModeRestart) << 4);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_OVERLOAD_MODE) == Protection::ProtModeRestart) << 5);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_UNDERLOAD_MODE) == Protection::ProtModeRestart) << 6);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE) == Protection::ProtModeRestart) << 7);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) == Protection::ProtModeOn) << 8);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_RESISTANCE_MODE) == Protection::ProtModeBlock) << 10);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) == Protection::ProtModeRestart) << 11);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_FLOW_DISCHARGE_MODE) == Protection::ProtModeRestart) << 12);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE) == Protection::ProtModeRestart) << 13);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE) == Protection::ProtModeRestart) << 14);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_VIBRATION_MODE) == Protection::ProtModeRestart) << 15);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_MODE) == Protection::ProtModeBlock) << 16);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) == Protection::ProtModeOn) << 17);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_VSD_MODE) == Protection::ProtModeRestart) << 18);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_1_MODE) == Protection::ProtModeRestart) << 19);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_2_MODE) == Protection::ProtModeRestart) << 20);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_3_MODE) == Protection::ProtModeRestart) << 21);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_4_MODE) == Protection::ProtModeRestart) << 22);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE) == Protection::ProtModeRestart) << 23);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE) == Protection::ProtModeRestart) << 24);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_1_MODE) == Protection::ProtModeRestart) << 27);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_2_MODE) == Protection::ProtModeRestart) << 28);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_3_MODE) == Protection::ProtModeRestart) << 29);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_4_MODE) == Protection::ProtModeRestart) << 30);
    scadaParameters_[88].value.float_t = data.uint32_t;

    //95
    data.uint32_t = 0;
    int reasonVsd = parameters.get(CCS_VSD_ALARM_CODE);
    switch (reason) {
    case LastReasonStopOverVoltIn:
      data.uint32_t = (1 << 0); break;
    case LastReasonStopUnderVoltIn:
      data.uint32_t = (1 << 1); break;
    case LastReasonStopImbalanceVoltIn:
      data.uint32_t = (1 << 2); break;
    case LastReasonStopCurrentMotor:
      data.uint32_t = (1 << 4); break;
    case LastReasonStopOverloadMotor:
      data.uint32_t = (1 << 5); break;
    case LastReasonStopUnderloadMotor:
      data.uint32_t = (1 << 6); break;
    case LastReasonStopImbalanceCurMotor:
      data.uint32_t = (1 << 7); break;
    case LastReasonStopResistIsolation:
      data.uint32_t = (1 << 10); break;
    case LastReasonStopPressureIntake:
      data.uint32_t = (1 << 11); break;
    case LastReasonStopFlowDischarge:
      data.uint32_t = (1 << 12); break;
    case LastReasonStopPressureDischarge:
      data.uint32_t = (1 << 13); break;
    case LastReasonStopTemperatureMotor:
      data.uint32_t = (1 << 14); break;
    case LastReasonStopVibrationMotor:
      data.uint32_t = (1 << 15); break;
    case LastReasonStopOvernumberOfStart:
      data.uint32_t = (1 << 16); break;
    case LastReasonStopHackSu:
      data.uint32_t = (1 << 17); break;
    case LastReasonStopHardwareVsd:
      switch (reasonVsd) {
      case VSD_ETALON_ALARM_UNDERVOLTAGE:
      case VSD_DANFOSS_ALARM_A_11:
      case VSD_DANFOSS_ALARM_A_36:
        data.uint32_t = (1 << 25); break;
      case VSD_DANFOSS_ALARM_A_59:
        data.uint32_t = (1 << 31); break;
      default:
        data.uint32_t = (1 << 18); break;
      }
      break;
    case LastReasonStopProtDigital1:
      data.uint32_t = (1 << 19); break;
    case LastReasonStopProtDigital2:
      data.uint32_t = (1 << 20); break;
    case LastReasonStopProtDigital3:
      data.uint32_t = (1 << 21); break;
    case LastReasonStopProtDigital4:
      data.uint32_t = (1 << 22); break;
    case LastReasonStopImbalanceCurIn:
      data.uint32_t = (1 << 23); break;
    case LastReasonStopOutOfSyncMotor:
      data.uint32_t = (1 << 24); break;
    case LastReasonStopMinAnalog1:
    case LastReasonStopMaxAnalog1:
      data.uint32_t = (1 << 27); break;
    case LastReasonStopMinAnalog2:
    case LastReasonStopMaxAnalog2:
      data.uint32_t = (1 << 28); break;
    case LastReasonStopMinAnalog3:
    case LastReasonStopMaxAnalog3:
      data.uint32_t = (1 << 29); break;
    case LastReasonStopMinAnalog4:
    case LastReasonStopMaxAnalog4:
      data.uint32_t = (1 << 30); break;
    default:
      data.uint32_t = 0; break;
    }
    scadaParameters_[89].value.float_t = data.uint32_t;

    //97
    value = 0;
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
    scadaParameters_[119].value.float_t = parameters.get(CCS_LANGUAGE) + 1;

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
    }
    scadaParameters_[123].value.float_t = value;

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

    //139
    scadaParameters_[129].value.float_t = 0;

    //147
    scadaParameters_[136].value.float_t = 0;

    //151
    scadaParameters_[140].value.float_t = 0;

    //189
    value = 0;
    int freq = lround(parameters.get(VSD_SWITCHING_FREQUENCY));
    switch (freq) {
    case 1000: value = 0; break;
    case 1500: value = 1; break;
    case 2000: value = 2; break;
    case 2500: value = 3; break;
    case 3000: value = 4; break;
    case 3500: value = 5; break;
    case 4000: value = 6; break;
    case 5000: value = 7; break;
    case 6000: value = 8; break;
    case 7000: value = 9; break;
    case 8000: value = 10; break;
    case 10000: value = 11; break;
    case 12000: value = 12; break;
    case 14000: value = 13; break;
    case 16000: value = 14; break;
    }
    scadaParameters_[178].value.float_t = value;

    //215
    value = 0;
    uint8_t type = parameters.get(CCS_DHS_TYPE);
    switch (type) {
    case TYPE_DHS_NOVOMET:
      value = 1; break;
    case TYPE_DHS_ALMAZ:
      value = 12; break;
    case TYPE_DHS_BORETS_1:
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
    case Regime::SwingRegimeRun:
      value = 1; break;
    case Regime::PushRegimeRun:
      value = 2; break;
    case Regime::SyncRegimeRun:
      value = 3; break;
    }
    scadaParameters_[251].value.float_t = value;

    //415
    data.uint32_t = 0;
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_MODE) == Protection::ProtModeBlock) << 0);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE) == Protection::ProtModeBlock) << 1);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE) == Protection::ProtModeBlock) << 2);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_CURRENT_MODE) == Protection::ProtModeBlock) << 5);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_OVERLOAD_MODE) == Protection::ProtModeBlock) << 5);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_UNDERLOAD_MODE) == Protection::ProtModeBlock) << 6);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE) == Protection::ProtModeBlock) << 7);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) == Protection::ProtModeBlock) << 8);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_RESISTANCE_MODE) == Protection::ProtModeBlock) << 10);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_MODE) == Protection::ProtModeBlock) << 11);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_FLOW_DISCHARGE_MODE) == Protection::ProtModeBlock) << 12);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_MODE) == Protection::ProtModeBlock) << 13);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE) == Protection::ProtModeBlock) << 14);
    data.uint32_t |= ((parameters.get(CCS_PROT_DHS_VIBRATION_MODE) == Protection::ProtModeBlock) << 15);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_MODE) == Protection::ProtModeBlock) << 16);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) == Protection::ProtModeBlock) << 17);
    data.uint32_t |= ((parameters.get(CCS_PROT_OTHER_VSD_MODE) == Protection::ProtModeBlock) << 18);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_1_MODE) == Protection::ProtModeBlock) << 19);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_2_MODE) == Protection::ProtModeBlock) << 20);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_3_MODE) == Protection::ProtModeBlock) << 21);
    data.uint32_t |= ((parameters.get(CCS_PROT_DI_4_MODE) == Protection::ProtModeBlock) << 22);
    data.uint32_t |= ((parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE) == Protection::ProtModeBlock) << 23);
    data.uint32_t |= ((parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_MODE) == Protection::ProtModeBlock) << 24);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_1_MODE) == Protection::ProtModeBlock) << 27);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_2_MODE) == Protection::ProtModeBlock) << 28);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_3_MODE) == Protection::ProtModeBlock) << 29);
    data.uint32_t |= ((parameters.get(CCS_PROT_AI_4_MODE) == Protection::ProtModeBlock) << 30);
    scadaParameters_[384].value.float_t = data.uint32_t;

    // 425
    value = parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) ? 1 : 0;
    scadaParameters_[393].value.float_t = value;

    //449
    scadaParameters_[417].value.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_SU);
    //451
    scadaParameters_[418].value.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_SU_2);

    //453
    data.uint32_t = 0;
    if (parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT))
      data.uint32_t = 1;
    else if (parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT))
      data.uint32_t = 2;
    else if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT))
      data.uint32_t = 3;
    else if (parameters.get(CCS_PROT_DHS_RESISTANCE_PREVENT))
      data.uint32_t = 4;
    else if (parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT))
      data.uint32_t = 5;
    else if (parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT))
      data.uint32_t = 6;
    else if (parameters.get(CCS_PROT_MOTOR_ASYNC_PREVENT))
      data.uint32_t = 7;
    else if (parameters.get(CCS_PROT_OTHER_LOCK_DOOR_PREVENT))
      data.uint32_t = 8;
    else if (parameters.get(CCS_PROT_OTHER_VSD_PREVENT))
      data.uint32_t = 9;
    else if (parameters.get(CCS_PROT_DI_1_PREVENT))
      data.uint32_t = 10;
    else if (parameters.get(CCS_PROT_DI_2_PREVENT))
      data.uint32_t = 11;
    else if (parameters.get(CCS_PROT_DI_3_PREVENT))
      data.uint32_t = 12;
    else if (parameters.get(CCS_PROT_DI_4_PREVENT))
      data.uint32_t = 13;
    else if (parameters.get(CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_PREVENT))
      data.uint32_t = 18;
    else if (parameters.get(CCS_PROT_AI_1_PREVENT))
      data.uint32_t = 19;
    else if (parameters.get(CCS_PROT_AI_2_PREVENT))
      data.uint32_t = 20;
    else if (parameters.get(CCS_PROT_AI_3_PREVENT))
      data.uint32_t = 21;
    else if (parameters.get(CCS_PROT_AI_4_PREVENT))
      data.uint32_t = 22;
    else if (parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_PREVENT))
      data.uint32_t = 23;
    scadaParameters_[419].value.float_t = data.uint32_t;

    // 475
    value = parameters.get(CCS_RGM_MAINTENANCE_PARAM_MODE) ? 1 : 0;
    scadaParameters_[441].value.float_t = value;

    // 481
    value = parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE) ? 1 : 0;
    scadaParameters_[447].value.float_t = value;

    // 482
    value = parameters.get(CCS_RGM_JARRING_MODE) ? 1 : 0;
    scadaParameters_[448].value.float_t = value;

    // 485
    value = parameters.get(CCS_RGM_PERIODIC_MODE) ? 1 : 0;
    scadaParameters_[451].value.float_t = value;

    //560
    value = (parameters.get(CCS_RUNNING_TYPE) == Regime::SmoothRegimeRun) ? 1 : 0;
    scadaParameters_[512].value.float_t = value;
    //561
    value = (parameters.get(CCS_RUNNING_TYPE) == Regime::SwingRegimeRun) ? 1 : 0;
    scadaParameters_[513].value.float_t = value;
    //562
    value = (parameters.get(CCS_RUNNING_TYPE) == Regime::PushRegimeRun) ? 1 : 0;
    scadaParameters_[514].value.float_t = value;
    //563
    value = (parameters.get(CCS_RUNNING_TYPE) == Regime::SyncRegimeRun) ? 1 : 0;
    scadaParameters_[515].value.float_t = value;

    // 601-602
    value = 0;
    data.uint32_t = 0;
    switch ((int)parameters.get(CCS_AI_1_PARAMETER)) {
    case 1: value = 1; data.uint32_t = 1; break;
    case 2: value = 1; data.uint32_t = 0; break;
    case 3: value = 1; data.uint32_t = 2; break;
    case 4: value = 2; data.uint32_t = 0; break;
    case 5: value = 2; data.uint32_t = 1; break;
    case 6: value = 3; data.uint32_t = 0; break;
    case 7: value = 3; data.uint32_t = 2; break;
    case 8: value = 3; data.uint32_t = 1; break;
    case 11: value = 5; data.uint32_t = 0; break;
    case 12: value = 5; data.uint32_t = 0; break;
    default: value = 0; data.uint32_t = 0; break;
    }
    scadaParameters_[553].value.float_t = value;
    scadaParameters_[554].value.float_t = data.uint32_t;

    // 622-623
    value = 0;
    data.uint32_t = 0;
    switch ((int)parameters.get(CCS_AI_2_PARAMETER)) {
    case 1: value = 1; data.uint32_t = 1; break;
    case 2: value = 1; data.uint32_t = 0; break;
    case 3: value = 1; data.uint32_t = 2; break;
    case 4: value = 2; data.uint32_t = 0; break;
    case 5: value = 2; data.uint32_t = 1; break;
    case 6: value = 3; data.uint32_t = 0; break;
    case 7: value = 3; data.uint32_t = 2; break;
    case 8: value = 3; data.uint32_t = 1; break;
    case 11: value = 5; data.uint32_t = 0; break;
    case 12: value = 5; data.uint32_t = 0; break;
    default: value = 0; data.uint32_t = 0; break;
    }
    scadaParameters_[567].value.float_t = value;
    scadaParameters_[568].value.float_t = data.uint32_t;

    // 643-644
    value = 0;
    data.uint32_t = 0;
    switch ((int)parameters.get(CCS_AI_3_PARAMETER)) {
    case 1: value = 1; data.uint32_t = 1; break;
    case 2: value = 1; data.uint32_t = 0; break;
    case 3: value = 1; data.uint32_t = 2; break;
    case 4: value = 2; data.uint32_t = 0; break;
    case 5: value = 2; data.uint32_t = 1; break;
    case 6: value = 3; data.uint32_t = 0; break;
    case 7: value = 3; data.uint32_t = 2; break;
    case 8: value = 3; data.uint32_t = 1; break;
    case 11: value = 5; data.uint32_t = 0; break;
    case 12: value = 5; data.uint32_t = 0; break;
    default: value = 0; data.uint32_t = 0; break;
    }
    scadaParameters_[581].value.float_t = value;
    scadaParameters_[582].value.float_t = data.uint32_t;

    // 664-665
    value = 0;
    data.uint32_t = 0;
    switch ((int)parameters.get(CCS_AI_4_PARAMETER)) {
    case 1: value = 1; data.uint32_t = 1; break;
    case 2: value = 1; data.uint32_t = 0; break;
    case 3: value = 1; data.uint32_t = 2; break;
    case 4: value = 2; data.uint32_t = 0; break;
    case 5: value = 2; data.uint32_t = 1; break;
    case 6: value = 3; data.uint32_t = 0; break;
    case 7: value = 3; data.uint32_t = 2; break;
    case 8: value = 3; data.uint32_t = 1; break;
    case 11: value = 5; data.uint32_t = 0; break;
    case 12: value = 5; data.uint32_t = 0; break;
    default: value = 0; data.uint32_t = 0; break;
    }
    scadaParameters_[595].value.float_t = value;
    scadaParameters_[596].value.float_t = data.uint32_t;
  }
}

int ScadaNovomet::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  unTypeData value;

  //119
  if (param->address == 119) {
    if (param->value.float_t)
      ksu.start(LastReasonRunRemote);
    return ok_r;
  }

  //121
  if (param->address == 121) {
    if (param->value.float_t)
      ksu.stop(LastReasonStopRemote);
    return ok_r;
  }

  //189
  if (param->address == 189) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 1000; break;
    case 1: value.float_t = 1500; break;
    case 2: value.float_t = 2000; break;
    case 3: value.float_t = 2500; break;
    case 4: value.float_t = 3000; break;
    case 5: value.float_t = 3500; break;
    case 6: value.float_t = 4000; break;
    case 7: value.float_t = 5000; break;
    case 8: value.float_t = 6000; break;
    case 9: value.float_t = 7000; break;
    case 10: value.float_t = 8000; break;
    case 11: value.float_t = 10000; break;
    case 12: value.float_t = 12000; break;
    case 13: value.float_t = 14000; break;
    case 14: value.float_t = 16000; break;
    }
    if (value.float_t != -1) {
      parameters.set(VSD_SWITCHING_FREQUENCY, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  //215
  if (param->address == 215) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0:  value.float_t = TYPE_DHS_NONE; break;
    case 1:  value.float_t = TYPE_DHS_NOVOMET; break;
    case 12: value.float_t = TYPE_DHS_ALMAZ; break;
    case 7:  value.float_t = TYPE_DHS_BORETS_1; break;
    case 2:  value.float_t = TYPE_DHS_ELEKTON_3; break;
    case 6:  value.float_t = TYPE_DHS_ETALON; break;
    case 8:  value.float_t = TYPE_DHS_IRZ; break;
    case 9:  value.float_t = TYPE_DHS_ORION; break;
    case 11: value.float_t = TYPE_DHS_PIC_V2; break;
    case 10: value.float_t = TYPE_DHS_SCAD; break;
    case 3:  value.float_t = TYPE_DHS_SCAN; break;
    case 5:  value.float_t = TYPE_DHS_TRIOL; break;
    case 4:  value.float_t = TYPE_DHS_ZENIT; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_DHS_TYPE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 425
  if (param->address == 425) {
    if (param->value.float_t)
      parameters.set(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ProtModeOn, RemoteType);
    else
      parameters.set(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ProtModeOff, RemoteType);
    return ok_r;
  }

  //449
  if (param->address == 449) {
    return parameters.set(CCS_NUM_PRODUCTION_SU, param->value.uint32_t, RemoteType);
  }
  //451
  if (param->address == 451) {
    return parameters.set(CCS_NUM_PRODUCTION_SU_2, param->value.uint32_t, RemoteType);
  }

  // 475
  if (param->address == 475) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_MAINTENANCE_PARAM_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_MAINTENANCE_PARAM_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 481
  if (param->address == 481) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_ALTERNATION_FREQ_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_ALTERNATION_FREQ_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 482
  if (param->address == 482) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_JARRING_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_JARRING_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 485
  if (param->address == 485) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 560
  if (param->address == 560) {
    parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::OffAction, RemoteType);
    parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::OffAction, RemoteType);
    parameters.set(CCS_RGM_RUN_PICKUP_MODE, Regime::OffAction, RemoteType);
    parameters.set(CCS_RGM_RUN_SYNCHRON_MODE, Regime::OffAction, RemoteType);
    parameters.set(CCS_RGM_RUN_SKIP_RESONANT_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 561
  if (param->address == 561) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::EachRunAction, RemoteType);
    else
      parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 562
  if (param->address == 562) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::EachRunAction, RemoteType);
    else
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 563
  if (param->address == 563) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_RUN_SYNCHRON_MODE, Regime::EachRunAction, RemoteType);
    else
      parameters.set(CCS_RGM_RUN_SYNCHRON_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 601
  if (param->address == 601) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 1: value.float_t = 1; break;
    case 2: value.float_t = 4; break;
    case 3: value.float_t = 6; break;
    case 5: value.float_t = 11; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_1_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }
  // 622
  if (param->address == 622) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 1: value.float_t = 1; break;
    case 2: value.float_t = 4; break;
    case 3: value.float_t = 6; break;
    case 5: value.float_t = 11; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_2_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }
  // 643
  if (param->address == 643) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 1: value.float_t = 1; break;
    case 2: value.float_t = 4; break;
    case 3: value.float_t = 6; break;
    case 5: value.float_t = 11; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_3_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }
  // 664
  if (param->address == 664) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 1: value.float_t = 1; break;
    case 2: value.float_t = 4; break;
    case 3: value.float_t = 6; break;
    case 5: value.float_t = 11; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_4_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  return err_r;
}

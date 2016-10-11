#include "scada_lukoil.h"
#include "user_main.h"
#include "regime.h"
#include "protection.h"

ScadaLukoil::ScadaLukoil()
{
  countParameters_ = 437;
  initParameters();
}

ScadaLukoil::~ScadaLukoil()
{

}

void ScadaLukoil::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 249
    unTypeData data;
    uint16_t value = 0;
    uint32_t state = parameters.get(CCS_CONDITION);
    int reason = parameters.get(CCS_LAST_STOP_REASON);
    bool isWorkMotor = false;
    if ((state == CCS_CONDITION_RUNNING) || (state == CCS_CONDITION_RUN))
      isWorkMotor = true;
    bool isStopMotor = false;
    if (state == CCS_CONDITION_STOP)
      isStopMotor = true;
    uint32_t flag = parameters.get(CCS_CONDITION_FLAG);
    bool isBlock = false;
    if (flag == CCS_CONDITION_FLAG_BLOCK)
      isBlock = true;
    bool isRestart = false;
    if (flag == CCS_CONDITION_FLAG_RESTART)
      isRestart = true;

    if (isWorkMotor) {
      if (parameters.get(CCS_RGM_RUN_PICKUP_STATE) != Regime::IdleState)
        value = 0x01;
      if (parameters.get(CCS_RGM_RUN_PUSH_STATE) != Regime::IdleState)
        value = 0x02;
      if (parameters.get(CCS_RGM_RUN_SWING_STATE) != Regime::IdleState)
        value = 0x03;
//      if (parameters.get() != Regime::IdleState)
//        value = 0x04;
      if (parameters.get(CCS_RGM_MAINTENANCE_PARAM_STATE) != Regime::IdleState)
        value = 0x05;
      if (parameters.get(CCS_RGM_JARRING_STATE) != Regime::IdleState)
        value = 0x06;
      if (parameters.get(CCS_RGM_CHANGE_FREQ_STATE) != Regime::IdleState)
        value = 0x07;
      if (parameters.get(CCS_RGM_OPTIM_VOLTAGE_STATE) != Regime::IdleState)
        value = 0x08;
//      if (parameters.get(CCS_RGM_CURRENT_LIMIT_STATE) != Regime::IdleState)
//        value = 0x09;
//      if (parameters.get() != Regime::IdleState)
//        value = 0x0A;
      if (parameters.get(CCS_RGM_ALTERNATION_FREQ_STATE) != Regime::IdleState)
        value = 0x0B;
//      if (parameters.get(CCS_RGM_PUMP_GAS_STATE) != Regime::IdleState)
//        value = 0x0C;
      if ((parameters.get(CCS_DI_1_VALUE) == parameters.get(CCS_DI_1_VALUE)) &&
          parameters.get(CCS_DI_1_ACTION) == DI_ACTION_REVERSE)
        value = 0x0D;
      if ((parameters.get(CCS_DI_2_VALUE) == parameters.get(CCS_DI_2_VALUE)) &&
          parameters.get(CCS_DI_2_ACTION) == DI_ACTION_REVERSE)
        value = 0x0E;
      if ((parameters.get(CCS_DI_3_VALUE) == parameters.get(CCS_DI_3_VALUE)) &&
          parameters.get(CCS_DI_3_ACTION) == DI_ACTION_REVERSE)
        value = 0x0F;
      if ((parameters.get(CCS_DI_4_VALUE) == parameters.get(CCS_DI_4_VALUE)) &&
          parameters.get(CCS_DI_4_ACTION) == DI_ACTION_REVERSE)
        value = 0x10;

      if ((parameters.get(CCS_DI_1_VALUE) == parameters.get(CCS_DI_1_VALUE)) &&
          parameters.get(CCS_DI_1_ACTION) == DI_ACTION_STOP)
        value = 0x13;
      if ((parameters.get(CCS_DI_2_VALUE) == parameters.get(CCS_DI_2_VALUE)) &&
          parameters.get(CCS_DI_2_ACTION) == DI_ACTION_STOP)
        value = 0x14;
      if ((parameters.get(CCS_DI_3_VALUE) == parameters.get(CCS_DI_3_VALUE)) &&
          parameters.get(CCS_DI_3_ACTION) == DI_ACTION_STOP)
        value = 0x15;
      if ((parameters.get(CCS_DI_4_VALUE) == parameters.get(CCS_DI_4_VALUE)) &&
          parameters.get(CCS_DI_4_ACTION) == DI_ACTION_STOP)
        value = 0x16;

      if ((parameters.get(CCS_DI_1_VALUE) == parameters.get(CCS_DI_1_VALUE)) &&
          parameters.get(CCS_DI_1_ACTION) == DI_ACTION_START)
        value = 0x19;
      if ((parameters.get(CCS_DI_2_VALUE) == parameters.get(CCS_DI_2_VALUE)) &&
          parameters.get(CCS_DI_2_ACTION) == DI_ACTION_START)
        value = 0x1A;
      if ((parameters.get(CCS_DI_3_VALUE) == parameters.get(CCS_DI_3_VALUE)) &&
          parameters.get(CCS_DI_3_ACTION) == DI_ACTION_START)
        value = 0x1B;
      if ((parameters.get(CCS_DI_4_VALUE) == parameters.get(CCS_DI_4_VALUE)) &&
          parameters.get(CCS_DI_4_ACTION) == DI_ACTION_START)
        value = 0x1C;
    } else {
      if (parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::PauseState)
        value = 0x21;
      else if (isRestart) {
        if (reason == LastReasonStopNoVoltage)
          value = 0x23;
        else
          value = 0x22;
        if (parameters.get(CCS_PROT_PREVENT))
          value = 0x27;
      } else if (isBlock) {
        value = 0x24;
        if (reason == LastReasonStopRemote)
          value = 0x29;
      } else {
        value = 0x20;
      }
    }
    scadaParameters_[0].value.uint32_t = value;

    // 250-252
    calcDateTime(parameters.getU32(CCS_DATE_TIME), &scadaParameters_[1]);

    // 253-254
    time_t time = 0;
    if (isStopMotor) {
      if (isRestart)
        time = parameters.get(CCS_RESTART_TIMER);
      else
        time = parameters.get(CCS_STOP_TIME);
    } else {
      time = parameters.get(CCS_RUN_TIME);
    }
    value = time/3600;
    scadaParameters_[4].value.uint32_t = value;
    value = (time/60%60)*256 + time%60;
    scadaParameters_[5].value.uint32_t = value;

    // 255
    data.uint32_t = 0;
    value = 0;
    int reasonVsd = parameters.get(CCS_VSD_ALARM_CODE);
    switch (reason) {
    case LastReasonStopResistIsolation:
      value = 1; break;
    case LastReasonStopUnderVoltIn:
      value = 2; break;
    case LastReasonStopOverVoltIn:
      value = 5; break;
    case LastReasonStopImbalanceVoltIn:
      value = 8; break;
    case LastReasonStopUnderloadMotor:
      value = 11; break;
    case LastReasonStopOverloadMotor:
      value = 12; break;
    case LastReasonStopImbalanceCurMotor:
      value = 13; break;
    case LastReasonStopHackSu:
      value = 18; break;
    case LastReasonStopProtDigital2:
      value = 23; break;
    case LastReasonStopProtDigital1:
      value = 24; break;
    case LastReasonStopTemperatureMotor:
      value = 26; break;
    case LastReasonStopPressureIntake:
      value = 29; break;
    case LastReasonStopOvernumberOfStart:
      value = 31; break;
    case LastReasonStopVibrationMotor:
      value = 35; break;
    case LastReasonStopHardwareVsd:
      switch (reasonVsd) {
      case VSD_NOVOMET_ALARM_TEMP_LINK:
      case VSD_NOVOMET_ALARM_TEMP:
      case VSD_NOVOMET_ALARM_AIR_TEMP:
      case VSD_ETALON_ALARM_OVERHEAT_IGBT:
      case VSD_ETALON_ALARM_OVERHEAT_FILTER:
        value = 40; break;
      default:
        value = 62; break;
      }
      break;
    case LastReasonStopVsdNoConnect:
      value = 63; break;
    case LastReasonStopDigital1:
      value = 94; break;
    case LastReasonStopDigital2:
      value = 95; break;
    case LastReasonStopDigital3:
      value = 96; break;
    case LastReasonStopDigital4:
      value = 97; break;
    case LastReasonStopOperator:
      value = 103; break;
    case LastReasonStopProgram:
      value = 106; break;
    case LastReasonStopNoVoltage:
      value = 107; break;
    case LastReasonStopRemote:
      value = 112; break;
    case LastReasonStopMaxAnalog1:
      value = 128; break;
    case LastReasonStopMinAnalog1:
      value = 129; break;
    case LastReasonStopMaxAnalog2:
      value = 130; break;
    case LastReasonStopMinAnalog2:
      value = 131; break;
    default:
      if (reason)
        value = 115;
      break;
    }
    data.uint16_t[0] = value;

    value = 0;
    int reasonRun = parameters.get(CCS_LAST_RUN_REASON);
    switch (reasonRun) {
    case LastReasonRunOperator:
      value = 0x00; break;
    case LastReasonRunProgram:
      value = 0x01; break;
    case LastReasonRunApvPower:
      value = 0x02; break;
    case LastReasonRunRemote:
      value = 0x04; break;
    case LastReasonRunDigital1:
      value = 0x05; break;
    case LastReasonRunDigital2:
      value = 0x06; break;
    case LastReasonRunDigital3:
      value = 0x07; break;
    case LastReasonRunDigital4:
      value = 0x08; break;
    default:
      value = 0x03; break;
    }
    data.uint16_t[0] |= (value << 8);

    value = 0;
    int workingMode = parameters.get(CCS_WORKING_MODE);
    switch (workingMode) {
    case CCS_WORKING_MODE_MANUAL:
      value = 1; break;
    case CCS_WORKING_MODE_AUTO: case CCS_WORKING_MODE_PROGRAM:
      value = 2; break;
    }
    data.uint32_t |= (value << 12);

    data.uint32_t |= (isBlock << 14);
    data.uint32_t |= (isWorkMotor << 15);

    scadaParameters_[6].value.uint32_t = data.uint32_t;

    // 277
    value = !parameters.get(CCS_DOOR_VALUE);
    scadaParameters_[28].value.float_t = value;

    // 300
    value = 0;
    if (!parameters.get(CCS_DHS_CONNECTION) && parameters.get(CCS_DHS_TYPE))
      value = 0x01;
    else if (parameters.get(TMS_FAIL_LINK_TMSP))
      value = 0x02;
    scadaParameters_[51].value.uint32_t = value;

    // 2312-2313
    time = parameters.get(CCS_RUN_TIME);
    value = time/3600;
    scadaParameters_[66].value.uint32_t = value;
    value = time/60%60;
    scadaParameters_[67].value.uint32_t = value;

    // 2314-2316
    calcDateTime(parameters.getU32(CCS_LAST_RUN_DATE_TIME), &scadaParameters_[68]);

    // 764-766
    calcDateTime(parameters.getU32(CCS_DATE_TIME), &scadaParameters_[127]);

    // 812
    value = parameters.get(CCS_PROT_MOTOR_ASYNC_MODE) ? 1 : 0;
    scadaParameters_[175].value.float_t = value;

    // 816
    value = parameters.get(CCS_PROT_OTHER_LOCK_DOOR_MODE) ? 1 : 0;
    scadaParameters_[179].value.float_t = value;

    // 824
    value = parameters.get(CCS_RGM_PERIODIC_MODE) ? 1 : 0;
    scadaParameters_[187].value.float_t = value;

    // 825-826
    time = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
    value = time/3600;
    scadaParameters_[188].value.float_t = value;
    value = time/60%60;
    scadaParameters_[189].value.float_t = value;

    // 827-828
    time = parameters.get(CCS_RGM_PERIODIC_STOP_PERIOD);
    value = time/3600;
    scadaParameters_[190].value.float_t = value;
    value = time/60%60;
    scadaParameters_[191].value.float_t = value;

    // 849
    value = 0;
    uint8_t type = parameters.get(CCS_DHS_TYPE);
    switch (type) {
    case TYPE_DHS_ELEKTON_2:
    case TYPE_DHS_ELEKTON_3:
      value = 2; break;
    case TYPE_DHS_BORETS:
      value = 3; break;
    case TYPE_DHS_SCAD:
      value = 6; break;
    case TYPE_DHS_IRZ:
      value = 8; break;
    case TYPE_DHS_NOVOMET:
      value = 9; break;
    case TYPE_DHS_SNG:
      value = 10; break;
    case TYPE_DHS_PIC_V2:
      value = 11; break;
    }
    scadaParameters_[212].value.float_t = value;

    // 866
    if (parameters.get(CCS_UNIT_TEMPERATURE) == TEMPERATURE_F)
      value = 8;
    else
      value = 7;
    scadaParameters_[229].value.float_t = value;

    // 871
    if (parameters.get(CCS_UNIT_VIBRATION) == ACCELERATION_MSS2)
      value = 10;
    else
      value = 9;
    scadaParameters_[234].value.float_t = value;

    // 879
    value = 4;
    if (parameters.get(CCS_UNIT_PRESSURE) == PRESSURE_AT)
      value = 3;
    else if (parameters.get(CCS_UNIT_PRESSURE) == PRESSURE_ATM)
      value = 2;
    else if (parameters.get(CCS_UNIT_PRESSURE) == PRESSURE_MPA)
      value = 4;
    else if (parameters.get(CCS_UNIT_PRESSURE) == PRESSURE_PSI)
      value = 5;
    else if (parameters.get(CCS_UNIT_PRESSURE) == PRESSURE_BAR)
      value = 6;
    scadaParameters_[242].value.float_t = value;

    // 890
    switch ((int)parameters.get(CCS_AI_1_PARAMETER)) {
    case 1: value = 2; break;
    case 2: value = 4; break;
    case 3: value = 5; break;
    case 4: value = 7; break;
    case 5: value = 8; break;
    case 6: value = 9; break;
    case 7: value = 10; break;
    case 9: value = 11; break;
    case 10: value = 12; break;
    case 11: value = 13; break;
    case 12: value = 14; break;
    default: value = 0; break;
    }
    scadaParameters_[253].value.float_t = value;

    // 895
    value = parameters.get(CCS_PROT_AI_1_RESTART_SETPOINT) * parameters.get(CCS_PROT_AI_1_PARAMETER) / 100;
    scadaParameters_[258].value.float_t = value;
    // 896
    value = parameters.get(CCS_PROT_AI_1_TRIP_SETPOINT) * parameters.get(CCS_PROT_AI_1_PARAMETER) / 100;
    scadaParameters_[259].value.float_t = value;

    // 909
    value = parameters.get(CCS_PROT_AI_2_RESTART_SETPOINT) * parameters.get(CCS_PROT_AI_2_PARAMETER) / 100;
    scadaParameters_[272].value.float_t = value;
    // 910
    value = parameters.get(CCS_PROT_AI_2_TRIP_SETPOINT) * parameters.get(CCS_PROT_AI_2_PARAMETER) / 100;
    scadaParameters_[273].value.float_t = value;

    // 904
    switch ((int)parameters.get(CCS_AI_2_PARAMETER)) {
    case 1: value = 2; break;
    case 2: value = 4; break;
    case 3: value = 5; break;
    case 4: value = 7; break;
    case 5: value = 8; break;
    case 6: value = 9; break;
    case 7: value = 10; break;
    case 9: value = 11; break;
    case 10: value = 12; break;
    case 11: value = 13; break;
    case 12: value = 14; break;
    default: value = 0; break;
    }
    scadaParameters_[267].value.float_t = value;

    // 931
    value = 0;
    if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction)
      value = 1;
    scadaParameters_[294].value.float_t = value;

    // 937
    value = 0;
    if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction)
      value = 0x01;
    else if (parameters.get(CCS_RGM_RUN_SWING_MODE) != Regime::OffAction)
      value = 0x02;
    scadaParameters_[300].value.float_t = value;

    // 951
    value = 0;
    if (parameters.get(CCS_RGM_JARRING_MODE) != Regime::OffAction)
      value = 2;
    scadaParameters_[314].value.float_t = value;

    // 959
    value = 0;
    if (parameters.get(CCS_RGM_MAINTENANCE_PARAM_MODE) != Regime::OffAction)
      value = 1;
    scadaParameters_[322].value.float_t = value;

    // 960
    switch ((int)parameters.get(CCS_RGM_MAINTENANCE_PARAM_TYPE)) {
    case 0: value = 4; break;
    case 1: value = 2; break;
    case 3: value = 5; break;
    case 4: value = 6; break;
    default: value = 8; break;
    }
    scadaParameters_[323].value.float_t = value;

    // 961
    scadaParameters_[324].value.float_t = parameters.get(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_CURRENT);

    // 969
    value = 0;
    if (parameters.get(CCS_RGM_PUMP_GAS_MODE) != Regime::OffAction)
      value = 1;
    scadaParameters_[332].value.float_t = value;

    // 987
    value = 0;
    if (parameters.get(CCS_RGM_CURRENT_LIMIT_MODE) != Regime::OffAction)
      value = 1;
    scadaParameters_[350].value.float_t = value;

    // 1007
    scadaParameters_[370].value.float_t  = parameters.get(VSD_UF_CHARACTERISTIC_U_1) * 100 /
        parameters.get(CCS_BASE_VOLTAGE);
    // 1009
    scadaParameters_[372].value.float_t  = parameters.get(VSD_UF_CHARACTERISTIC_U_2) * 100 /
        parameters.get(CCS_BASE_VOLTAGE);
    // 1011
    scadaParameters_[374].value.float_t  = parameters.get(VSD_UF_CHARACTERISTIC_U_3) * 100 /
        parameters.get(CCS_BASE_VOLTAGE);
    // 1013
    scadaParameters_[376].value.float_t  = parameters.get(VSD_UF_CHARACTERISTIC_U_4) * 100 /
        parameters.get(CCS_BASE_VOLTAGE);
    // 1015
    scadaParameters_[378].value.float_t  = parameters.get(VSD_UF_CHARACTERISTIC_U_5) * 100 /
        parameters.get(CCS_BASE_VOLTAGE);

    // 1025
    data.uint32_t = parameters.get(CCS_SCADA_BYTERATE);
    switch (data.uint32_t) {
    case 2400: value = 1; break;
    case 4800: value = 2; break;
    case 9600: value = 3; break;
    case 14400: value = 4; break;
    case 19200: value = 5; break;
    case 28800: value = 6; break;
    case 38400: value = 7; break;
    case 57600: value = 8; break;
    case 76800: value = 9; break;
    case 115200: value = 10; break;
    default: value = 11; break;
    }
    scadaParameters_[388].value.float_t = value;

    // 1026
    scadaParameters_[389].value.float_t = 4;

    // 1051-1052
    data.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_CCS);
    scadaParameters_[414].value.uint32_t = data.uint16_t[1];
    scadaParameters_[415].value.uint32_t = data.uint16_t[0];

    // 1054-1056
    time = parameters.getU32(CCS_DATE_PRODUCTION_CCS);
    tm dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[417].value.float_t = dateTime.tm_mday;
    scadaParameters_[418].value.float_t = dateTime.tm_mon + 1;
    scadaParameters_[419].value.float_t = dateTime.tm_year;

    // 1058-1060
    time = parameters.getU32(CCS_DATE_INSTALL_SW_CCS);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[421].value.float_t = dateTime.tm_mday;
    scadaParameters_[422].value.float_t = dateTime.tm_mon + 1;
    scadaParameters_[423].value.float_t = dateTime.tm_year;

    // 1062-1064
    time = parameters.getU32(CCS_DATE_PRODUCTION_SU);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[425].value.float_t = dateTime.tm_mday;
    scadaParameters_[426].value.float_t = dateTime.tm_mon + 1;
    scadaParameters_[427].value.float_t = dateTime.tm_year;

    // 1065-1066
    data.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_SU);
    scadaParameters_[428].value.uint32_t = data.uint16_t[1];
    scadaParameters_[429].value.uint32_t = data.uint16_t[0];

    // 1069
    switch ((int)parameters.get(CCS_EM_TYPE)) {
    case EM_TYPE_NONE: value = 0; break;
    case EM_TYPE_SET4TM: value = 2; break;
    default: value = 3; break;
    }
    scadaParameters_[432].value.float_t = value;

    // 1072-1073
    data.uint32_t = parameters.get(CCS_VERSION_SW_CCS);
    scadaParameters_[435].value.uint32_t = data.uint16_t[1];
    scadaParameters_[436].value.uint32_t = data.uint16_t[0];
  }
}

int ScadaLukoil::setNewValue(ScadaParameter *param)
{
  unTypeData value;
  time_t time;

  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  // 764-766
  if ((param->address >= 764) && (param->address <= 766)) {
    time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 764:
      dateTime.tm_mon = param->value.uint32_t/256 - 1;
      dateTime.tm_mday = param->value.uint32_t%256;
      break;
    case 765:
      dateTime.tm_hour = param->value.uint32_t/256;
      dateTime.tm_year = param->value.uint32_t%256 + 100;
      break;
    case 766:
      dateTime.tm_min = param->value.uint32_t%256;
      dateTime.tm_sec = param->value.uint32_t/256;
      break;
    }

    time = mktime(&dateTime);
    parameters.set(CCS_DATE_TIME, (uint32_t)time, RemoteType);
    return ok_r;
  }

  // 812
  if (param->address == 812) {
    if (param->value.float_t)
      parameters.set(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ProtModeOn, RemoteType);
    else
      parameters.set(CCS_PROT_MOTOR_ASYNC_MODE, Protection::ProtModeOff, RemoteType);
    return ok_r;
  }

  // 816
  if (param->address == 816) {
    if (param->value.float_t)
      parameters.set(CCS_PROT_OTHER_LOCK_DOOR_MODE, Protection::ProtModeOn, RemoteType);
    else
      parameters.set(CCS_PROT_OTHER_LOCK_DOOR_MODE, Protection::ProtModeOff, RemoteType);
    return ok_r;
  }

  // 824
  if (param->address == 824) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 825
  if (param->address == 825) {
    time = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
    value.float_t = param->value.float_t*3600 + time/60%60*60;
    parameters.set(CCS_RGM_PERIODIC_RUN_PERIOD, value.float_t, RemoteType);
    return ok_r;
  }
  // 826
  if (param->address == 826) {
    time = parameters.get(CCS_RGM_PERIODIC_RUN_PERIOD);
    value.float_t = time/3600*3600 + param->value.float_t*60 + time%60;
    parameters.set(CCS_RGM_PERIODIC_RUN_PERIOD, value.float_t, RemoteType);
    return ok_r;
  }

  // 827
  if (param->address == 827) {
    time = parameters.get(CCS_RGM_PERIODIC_STOP_PERIOD);
    value.float_t = param->value.float_t*3600 + time/60%60*60;
    parameters.set(CCS_RGM_PERIODIC_STOP_PERIOD, value.float_t, RemoteType);
    return ok_r;
  }
  // 828
  if (param->address == 828) {
    time = parameters.get(CCS_RGM_PERIODIC_STOP_PERIOD);
    value.float_t = time/3600*3600 + param->value.float_t*60 + time%60;
    parameters.set(CCS_RGM_PERIODIC_STOP_PERIOD, value.float_t, RemoteType);
    return ok_r;
  }

  // 849
  if (param->address == 849) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0:
      value.float_t = TYPE_DHS_NONE; break;
    case 2:
      value.float_t = TYPE_DHS_ELEKTON_3; break;
    case 3:
      value.float_t = TYPE_DHS_BORETS; break;
    case 6:
      value.float_t = TYPE_DHS_SCAD; break;
    case 8:
      value.float_t = TYPE_DHS_IRZ; break;
    case 9:
      value.float_t = TYPE_DHS_NOVOMET; break;
    case 10:
      value.float_t = TYPE_DHS_SNG; break;
    case 11:
      value.float_t = TYPE_DHS_PIC_V2; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_DHS_TYPE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 866
  if (param->address == 866) {
    if (param->value.float_t == 8) {
      parameters.set(CCS_UNIT_TEMPERATURE, TEMPERATURE_F, RemoteType);
      return ok_r;
    }
    if (param->value.float_t == 7) {
      parameters.set(CCS_UNIT_TEMPERATURE, TEMPERATURE_C, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 871
  if (param->address == 871) {
    if (param->value.float_t == 10) {
      parameters.set(CCS_UNIT_VIBRATION, ACCELERATION_MSS2, RemoteType);
      return ok_r;
    }
    if (param->value.float_t == 9) {
      parameters.set(CCS_UNIT_VIBRATION, ACCELERATION_G, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 879
  if (param->address == 879) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 4:
      value.float_t = PRESSURE_MPA; break;
    case 3:
      value.float_t = PRESSURE_AT; break;
    case 2:
      value.float_t = PRESSURE_ATM; break;
    case 5:
      value.float_t = PRESSURE_PSI; break;
    case 6:
      value.float_t = PRESSURE_BAR; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_UNIT_PRESSURE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 890
  if (param->address == 890) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 2: value.float_t = 1; break;
    case 4: value.float_t = 2; break;
    case 5: value.float_t = 3; break;
    case 7: value.float_t = 4; break;
    case 8: value.float_t = 5; break;
    case 9: value.float_t = 6; break;
    case 10: value.float_t = 7; break;
    case 11: value.float_t = 9; break;
    case 12: value.float_t = 10; break;
    case 13: value.float_t = 11; break;
    case 14: value.float_t = 12; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_1_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }
  // 895
  if (param->address == 895) {
    value.float_t  = param->value.float_t * 100 / parameters.get(CCS_PROT_AI_1_PARAMETER);
    parameters.set(CCS_PROT_AI_1_RESTART_SETPOINT, value.float_t, RemoteType);
    return ok_r;
  }
  // 896
  if (param->address == 896) {
    value.float_t  = param->value.float_t * 100 / parameters.get(CCS_PROT_AI_1_PARAMETER);
    parameters.set(CCS_PROT_AI_1_TRIP_SETPOINT, value.float_t, RemoteType);
    return ok_r;
  }

  // 904
  if (param->address == 904) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = 0; break;
    case 2: value.float_t = 1; break;
    case 4: value.float_t = 2; break;
    case 5: value.float_t = 3; break;
    case 7: value.float_t = 4; break;
    case 8: value.float_t = 5; break;
    case 9: value.float_t = 6; break;
    case 10: value.float_t = 7; break;
    case 11: value.float_t = 9; break;
    case 12: value.float_t = 10; break;
    case 13: value.float_t = 11; break;
    case 14: value.float_t = 12; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_AI_2_PARAMETER, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }
  // 909
  if (param->address == 909) {
    value.float_t  = param->value.float_t * 100 / parameters.get(CCS_PROT_AI_2_PARAMETER);
    parameters.set(CCS_PROT_AI_2_RESTART_SETPOINT, value.float_t, RemoteType);
    return ok_r;
  }
  // 910
  if (param->address == 910) {
    value.float_t  = param->value.float_t * 100 / parameters.get(CCS_PROT_AI_2_PARAMETER);
    parameters.set(CCS_PROT_AI_2_TRIP_SETPOINT, value.float_t, RemoteType);
    return ok_r;
  }

  // 931
  if (param->address == 931) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_RUN_PICKUP_MODE, Regime::EachRunAction, RemoteType);
    else
      parameters.set(CCS_RGM_RUN_PICKUP_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 937
  if (param->address == 937) {
    if (param->value.float_t == 0x01)
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::EachRunAction, RemoteType);
    else if (param->value.float_t == 0x02)
      parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::EachRunAction, RemoteType);
    else if (param->value.float_t == 0x00) {
      parameters.set(CCS_RGM_RUN_PUSH_MODE, Regime::OffAction, RemoteType);
      parameters.set(CCS_RGM_RUN_SWING_MODE, Regime::OffAction, RemoteType);
      parameters.set(CCS_RGM_RUN_PICKUP_MODE, Regime::OffAction, RemoteType);
    } else {
      return err_r;
    }
    return ok_r;
  }

  // 951
  if (param->address == 951) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_JARRING_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_JARRING_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 959
  if (param->address == 959) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_MAINTENANCE_PARAM_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_MAINTENANCE_PARAM_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 960
  if (param->address == 960) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 4: value.float_t = 0; break;
    case 2: value.float_t = 1; break;
    case 5: value.float_t = 3; break;
    case 6: value.float_t = 4; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_RGM_MAINTENANCE_PARAM_TYPE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 961
  if (param->address == 961) {
    parameters.set(CCS_RGM_MAINTENANCE_PARAM_SETPOINT_CURRENT, param->value.float_t, RemoteType);
    return ok_r;
  }

  // 969
  if (param->address == 969) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_PUMP_GAS_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_PUMP_GAS_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 987
  if (param->address == 987) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_CURRENT_LIMIT_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_CURRENT_LIMIT_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 1007
  if (param->address == 1007) {
    value.float_t  = param->value.float_t * parameters.get(CCS_BASE_VOLTAGE) / 100;
    parameters.set(VSD_UF_CHARACTERISTIC_U_1, value.float_t, RemoteType);
    return ok_r;
  }
  // 1009
  if (param->address == 1009) {
    value.float_t  = param->value.float_t * parameters.get(CCS_BASE_VOLTAGE) / 100;
    parameters.set(VSD_UF_CHARACTERISTIC_U_2, value.float_t, RemoteType);
    return ok_r;
  }
  // 1011
  if (param->address == 1011) {
    value.float_t  = param->value.float_t * parameters.get(CCS_BASE_VOLTAGE) / 100;
    parameters.set(VSD_UF_CHARACTERISTIC_U_3, value.float_t, RemoteType);
    return ok_r;
  }
  // 1013
  if (param->address == 1013) {
    value.float_t  = param->value.float_t * parameters.get(CCS_BASE_VOLTAGE) / 100;
    parameters.set(VSD_UF_CHARACTERISTIC_U_4, value.float_t, RemoteType);
    return ok_r;
  }
  // 1015
  if (param->address == 1015) {
    value.float_t  = param->value.float_t * parameters.get(CCS_BASE_VOLTAGE) / 100;
    parameters.set(VSD_UF_CHARACTERISTIC_U_5, value.float_t, RemoteType);
    return ok_r;
  }

  // 1025
  if (param->address == 1025) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 1: value.float_t = 2400; break;
    case 2: value.float_t = 4800; break;
    case 3: value.float_t = 9600; break;
    case 4: value.float_t = 14400; break;
    case 5: value.float_t = 19200; break;
    case 6: value.float_t = 28800; break;
    case 7: value.float_t = 38400; break;
    case 8: value.float_t = 57600; break;
    case 9: value.float_t = 76800; break;
    case 10: value.float_t = 115200; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_SCADA_BYTERATE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  // 1026
  if (param->address == 1026) {
    if (param->value.float_t == 4)
      return ok_r;
    return err_r;
  }

  // 1069
  if (param->address == 1069) {
    value.float_t = -1;
    switch (int(param->value.float_t)) {
    case 0: value.float_t = EM_TYPE_NONE; break;
    case 2: value.float_t = EM_TYPE_SET4TM; break;
    case 3: value.float_t = EM_TYPE_ABB_A44; break;
    }
    if (value.float_t != -1) {
      parameters.set(CCS_EM_TYPE, value.float_t, RemoteType);
      return ok_r;
    }
    return err_r;
  }

  return err_r;
}

void ScadaLukoil::calcDateTime(time_t time, ScadaParameter *params)
{
  uint16_t value = 0;
  tm dateTime = *localtime(&time);
  if (dateTime.tm_year > 100)
    dateTime.tm_year = dateTime.tm_year - 100;
  else
    dateTime.tm_year = 0;
  value = (dateTime.tm_mon + 1)*256 + dateTime.tm_mday;
  params[0].value.uint32_t = value;
  value = dateTime.tm_hour*256 + dateTime.tm_year;
  params[1].value.uint32_t = value;
  value = dateTime.tm_sec*256 + dateTime.tm_min;
  params[2].value.uint32_t = value;
}

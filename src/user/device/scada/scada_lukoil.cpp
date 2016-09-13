#include "scada_lukoil.h"
#include "user_main.h"
#include "regime.h"

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
    uint16_t value = 0;
    uint32_t state = parameters.get(CCS_CONDITION);
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
        if (parameters.get(CCS_LAST_STOP_REASON) == LastReasonStopNoVoltage)
          value = 0x23;
        else
          value = 0x22;
        if (parameters.get(CCS_PROT_PREVENT))
          value = 0x27;
      } else if (isBlock) {
        value = 0x24;
        if (parameters.get(CCS_LAST_STOP_REASON) == LastReasonStopRemote)
          value = 0x29;
      }
    }
    scadaParameters_[0].value.float_t = value;

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
    scadaParameters_[4].value.float_t = value;
    value = (time/60%60)*100 + time%60;
    scadaParameters_[5].value.float_t = value;

    // 255
    scadaParameters_[6].value.float_t = value;

    // 277
    value = !parameters.get(CCS_DOOR_VALUE);
    scadaParameters_[28].value.float_t = value;

    // 300
    value = 0;
    if (!parameters.get(CCS_DHS_CONNECTION) && parameters.get(CCS_DHS_TYPE))
      value = 0x01;
    else if (parameters.get(TMS_FAIL_LINK_TMSP))
      value = 0x02;
    scadaParameters_[51].value.float_t = value;

    // 2312-2313
    time = parameters.get(CCS_RUN_TIME);
    value = time/3600;
    scadaParameters_[66].value.float_t = value;
    value = time/60%60;
    scadaParameters_[67].value.float_t = value;

    // 2314-2316
    calcDateTime(parameters.getU32(CCS_LAST_RUN_DATE_TIME), &scadaParameters_[68]);

    // 764-766
    calcDateTime(parameters.getU32(CCS_DATE_TIME), &scadaParameters_[127]);

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
    value = parameters.get(CCS_PROT_AI_1_PARAMETER) * parameters.get(CCS_PROT_AI_1_RESTART_SETPOINT) / 100;
    scadaParameters_[258].value.float_t = value;
    // 896
    value = parameters.get(CCS_PROT_AI_1_PARAMETER) * parameters.get(CCS_PROT_AI_1_TRIP_SETPOINT) / 100;
    scadaParameters_[259].value.float_t = value;

    // 909
    value = parameters.get(CCS_PROT_AI_2_PARAMETER) * parameters.get(CCS_PROT_AI_2_RESTART_SETPOINT) / 100;
    scadaParameters_[272].value.float_t = value;
    // 910
    value = parameters.get(CCS_PROT_AI_2_PARAMETER) * parameters.get(CCS_PROT_AI_2_TRIP_SETPOINT) / 100;
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
    if (parameters.get(CCS_RGM_RUN_PICKUP_STATE) != Regime::IdleState)
      value = 0x01;
    else if (parameters.get(CCS_RGM_RUN_PUSH_STATE) != Regime::IdleState)
      value = 0x02;
    scadaParameters_[300].value.float_t = value;

    // 931
    value = 0;
    if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction)
      value = 1;
    scadaParameters_[294].value.float_t = value;

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

    // 1025
    unTypeData data;
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
    scadaParameters_[417].value.uint32_t = dateTime.tm_mday;
    scadaParameters_[418].value.uint32_t = dateTime.tm_mon + 1;
    scadaParameters_[419].value.uint32_t = dateTime.tm_year;

    // 1058-1060
    time = parameters.getU32(CCS_DATE_INSTALL_SW_CCS);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[421].value.uint32_t = dateTime.tm_mday;
    scadaParameters_[422].value.uint32_t = dateTime.tm_mon + 1;
    scadaParameters_[423].value.uint32_t = dateTime.tm_year;

    // 1062-1064
    time = parameters.getU32(CCS_DATE_PRODUCTION_SU);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[425].value.uint32_t = dateTime.tm_mday;
    scadaParameters_[426].value.uint32_t = dateTime.tm_mon + 1;
    scadaParameters_[427].value.uint32_t = dateTime.tm_year;

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
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
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
  value = (dateTime.tm_mon + 1)*100 + dateTime.tm_mday;
  params[0].value.float_t = value;
  value = dateTime.tm_hour*100 + dateTime.tm_year;
  params[1].value.float_t = value;
  value = dateTime.tm_sec*100 + dateTime.tm_min;
  params[2].value.float_t = value;
}

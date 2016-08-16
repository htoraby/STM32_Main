#include "scada_telescop.h"
#include "user_main.h"

ScadaTelescop::ScadaTelescop()
{
  countParameters_ = 183;
  initParameters();
}

ScadaTelescop::~ScadaTelescop()
{

}

void ScadaTelescop::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 40000
    uint16_t value = 0;
    value |= (ksu.isWorkMotor() << 0);
    value |= (ksu.isBlock() << 1);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT) > 3) << 2);
    value |= (ksu.isAutoMode() << 4);
    value |= (ksu.isManualMode() << 5);
    scadaParameters_[0].value.float_t = value;

    // 40001-40002
    value = 0;
    if (parameters.get(CCS_PROT_PREVENT)) {
      if (parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT))
        value |= (1 << 0);
      if (parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT))
        value |= (1 << 1);
      if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT))
        value |= (1 << 2);
      if (parameters.get(CCS_PROT_DHS_RESISTANCE_PREVENT))
        value |= (1 << 3);
      if (parameters.get(CCS_PROT_OTHER_LOCK_DOOR_PREVENT))
        value |= (1 << 5);
      if (parameters.get(CCS_PROT_MOTOR_ASYNC_PREVENT))
        value |= (1 << 6);
      if (parameters.get(CCS_PROT_SUPPLY_PHASE_ROTATION_PREVENT))
        value |= (1 << 7);
      if (parameters.get(CCS_PROT_AI_1_PREVENT)) {
        if (parameters.get(CCS_PROT_AI_1_ALARM_TYPE) == 0)
          value |= (1 << 8);
      }
      if (parameters.get(CCS_PROT_AI_2_PREVENT)) {
        if (parameters.get(CCS_PROT_AI_2_ALARM_TYPE) == 0)
          value |= (1 << 9);
      }
      if (parameters.get(CCS_PROT_AI_3_PREVENT)) {
        if (parameters.get(CCS_PROT_AI_3_ALARM_TYPE) == 0)
          value |= (1 << 10);
      }
      if (parameters.get(CCS_PROT_AI_4_PREVENT)) {
        if (parameters.get(CCS_PROT_AI_4_ALARM_TYPE) == 0)
          value |= (1 << 11);
      }
      scadaParameters_[1].value.float_t = value;

      value = 0;
      if (parameters.get(CCS_PROT_AI_1_PREVENT) && parameters.get(CCS_PROT_AI_1_ALARM_TYPE))
        value |= (1 << 3);
      if (parameters.get(CCS_PROT_AI_2_PREVENT) && parameters.get(CCS_PROT_AI_2_ALARM_TYPE))
        value |= (1 << 4);
      if (parameters.get(CCS_PROT_AI_3_PREVENT) && parameters.get(CCS_PROT_AI_3_ALARM_TYPE))
        value |= (1 << 5);
      if (parameters.get(CCS_PROT_AI_4_PREVENT) && parameters.get(CCS_PROT_AI_4_ALARM_TYPE))
        value |= (1 << 6);
      if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PREVENT) ||
          parameters.get(CCS_PROT_MOTOR_OVERLOAD_PREVENT) ||
          parameters.get(CCS_PROT_MOTOR_UNDERLOAD_PREVENT) ||
          parameters.get(CCS_PROT_MOTOR_CURRENT_PREVENT) ||
          parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PREVENT) ||
          parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT) ||
          parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT) ||
          parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT) ||
          parameters.get(CCS_PROT_DHS_VIBRATION_PREVENT) ||
          parameters.get(CCS_PROT_DI_1_PREVENT) ||
          parameters.get(CCS_PROT_DI_2_PREVENT) ||
          parameters.get(CCS_PROT_DI_3_PREVENT) ||
          parameters.get(CCS_PROT_DI_4_PREVENT) ||
          parameters.get(CCS_PROT_OTHER_VSD_PREVENT) ||
          parameters.get(CCS_PROT_OTHER_IMB_PREVENT))
        value |= (1 << 15);
      scadaParameters_[2].value.float_t = value;
    }
    else {
      scadaParameters_[1].value.float_t = value;
      scadaParameters_[2].value.float_t = value;
    }

    // 40003
    value = 0;
    int reason = parameters.get(CCS_LAST_STOP_REASON);
    int reasonVsd = parameters.get(CCS_VSD_ALARM_CODE);
    switch (reason) {
    case LastReasonStopOperator:
      value = 12; break;
    case LastReasonStopResistIsolation:
      value = 7; break;
    case LastReasonStopUnderloadMotor:
      value = 5; break;
    case LastReasonStopOverloadMotor:
      value = 4; break;
    case LastReasonStopUnderVoltIn:
      value = 2; break;
    case LastReasonStopOverVoltIn:
      value = 1; break;
    case LastReasonStopHackSu:
      value = 8; break;
    case LastReasonStopManometr:
      value = 9; break;
    case LastReasonStopImbalanceCurMotor:
      value = 6; break;
    case LastReasonStopImbalanceVoltIn:
      value = 3; break;
    case LastReasonStopProgram:
      value = 13; break;
    case LastReasonStopMinAnalog1:
      value = 23; break;
    case LastReasonStopMinAnalog2:
      value = 24; break;
    case LastReasonStopMinAnalog3:
      value = 25; break;
    case LastReasonStopMinAnalog4:
      value = 26; break;
    case LastReasonStopMaxAnalog1:
      value = 34; break;
    case LastReasonStopMaxAnalog2:
      value = 35; break;
    case LastReasonStopMaxAnalog3:
      value = 36; break;
    case LastReasonStopMaxAnalog4:
      value = 37; break;
    case LastReasonStopRemote:
      value = 14; break;
    case LastReasonStopCurrentMotor:
      value = 10; break;
    case LastReasonStopHardwareVsd:
      switch (reasonVsd) {
      case VSD_NOVOMET_ALARM_I_LIMIT_FAST:
      case VSD_ETALON_ALARM_CURRENT_LIMIT:
        value = 10; break;
      case VSD_NOVOMET_ALARM_TEMP_LINK:
      case VSD_NOVOMET_ALARM_TEMP:
      case VSD_NOVOMET_ALARM_AIR_TEMP:
      case VSD_ETALON_ALARM_OVERHEAT_IGBT:
      case VSD_ETALON_ALARM_OVERHEAT_FILTER:
        value = 17; break;
      default:
        value = 45; break;
      }
      break;
    default:
      if (reason)
        value = 46;
      break;
    }
    scadaParameters_[3].value.float_t = value;

    // 40004-40006
    value = 0;
    time_t time = parameters.getU32(CCS_LAST_STOP_DATE_TIME);
    tm dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    value = (dateTime.tm_year << 8) + (dateTime.tm_mon + 1);
    scadaParameters_[4].value.float_t = value;
    value = (dateTime.tm_mday << 8) + dateTime.tm_hour;
    scadaParameters_[5].value.float_t = value;
    value = (dateTime.tm_min << 8) + dateTime.tm_sec;
    scadaParameters_[6].value.float_t = value;

    // 30075-30077
    value = 0;
    time = parameters.getU32(CCS_DATE_TIME);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    value = (dateTime.tm_year << 8) + (dateTime.tm_mon + 1);
    scadaParameters_[173].value.float_t = value;
    value = (dateTime.tm_mday << 8) + dateTime.tm_hour;
    scadaParameters_[174].value.float_t = value;
    value = (dateTime.tm_min << 8) + dateTime.tm_sec;
    scadaParameters_[175].value.float_t = value;
  }
}

int ScadaTelescop::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  // 30075-30077
  if ((param->address >= 30075) && (param->address <= 30077)) {
    unTypeData value;
    value.uint32_t = param->value.float_t;
    time_t time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 30075:
      dateTime.tm_year = value.char_t[1] + 100;
      if (value.uint16_t[0] > 0)
        dateTime.tm_mon = value.char_t[0] - 1;
      break;
    case 30076:
      dateTime.tm_mday = value.char_t[1];
      if(dateTime.tm_mday == 0)
        dateTime.tm_mday = 1;
      dateTime.tm_hour = value.char_t[0];
      break;
    case 30077:
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

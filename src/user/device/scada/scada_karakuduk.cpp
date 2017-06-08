#include "scada_karakuduk.h"
#include "user_main.h"
#include "regime.h"

ScadaKarakuduk::ScadaKarakuduk()
{
  initParameters();
}

ScadaKarakuduk::~ScadaKarakuduk()
{

}

void ScadaKarakuduk::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 277 Режим работы
    uint16_t value = 0;
    int reasonRun = parameters.get(CCS_LAST_RUN_REASON);
    if ((reasonRun == LastReasonRunOperator) || (reasonRun == LastReasonRunRemote))
      value = 1;
    else if (reasonRun == LastReasonRunProgram)
      value = 2;
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP)
      value |= 1 << 8;
    else if ((parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction) &&
             (parameters.get(CCS_WORKING_MODE) == CCS_WORKING_MODE_PROGRAM))
      value |= 3 << 8;
    else
      value |= 2 << 8;
    scadaParameters_[21].value.float_t = value;

    // 256, 289 Состояние СУ
    value = 0;
    if (parameters.get(CCS_CONDITION) == CCS_CONDITION_STOP)
      value = 0xB0;
    if (parameters.get(CCS_PROT_MOTOR_ASYNC_PREVENT))
      value = 0xC7;
    else if (parameters.get(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_BLOCK)
      value = 0xC0;
    else if (parameters.get(CCS_CONDITION_FLAG) == CCS_CONDITION_FLAG_DELAY)
      value = 0xC4;
    else if (parameters.get(CCS_CONDITION) != CCS_CONDITION_STOP) {
      if (reasonRun == LastReasonRunRemote)
        value = 0xCC;
      else
        value = 0xC5;
    }
    scadaParameters_[0].value.float_t = value;
    scadaParameters_[33].value.float_t = value;

    // 290 Причина останова
    value = 0;
    int reasonStop = parameters.get(CCS_LAST_STOP_REASON);
    int reasonVsd = parameters.get(CCS_VSD_ALARM_CODE);
    switch (reasonStop) {
    case LastReasonStopOverVoltIn:
      value = 0x01; break;
    case LastReasonStopUnderVoltIn:
      value = 0x02; break;
    case LastReasonStopImbalanceVoltIn:
      value = 0x03; break;
    case LastReasonStopOverloadMotor:
      value = 0x04; break;
    case LastReasonStopUnderloadMotor:
      value = 0x05; break;
    case LastReasonStopImbalanceCurMotor:
      value = 0x06; break;
    case LastReasonStopCurrentMotor:
      value = 0x07; break;
    case LastReasonStopVsdNoConnect:
      value = 0x08; break;
    case LastReasonStopVibrationMotor:
      value = 0x0A; break;
    case LastReasonStopTemperatureMotor:
      value = 0x0B; break;
    case LastReasonStopPressureIntake:
      value = 0x0C; break;
    case LastReasonStopResistIsolation:
      value = 0x11; break;
    case LastReasonStopHardwareVsd:
      switch (reasonVsd) {
      case VSD_NOVOMET_ALARM_TEMP_LINK:
      case VSD_NOVOMET_ALARM_TEMP:
      case VSD_NOVOMET_ALARM_AIR_TEMP:
      case VSD_ETALON_ALARM_OVERHEAT_IGBT:
      case VSD_ETALON_ALARM_OVERHEAT_FILTER:
        value = 17; break;
      default:
        value = 18; break;
      }
      break;
    case LastReasonStopMaxAnalog1: case LastReasonStopMaxAnalog2:
    case LastReasonStopMinAnalog1: case LastReasonStopMinAnalog2:
    case LastReasonStopMinAnalog3: case LastReasonStopMinAnalog4:
    case LastReasonStopMaxAnalog3: case LastReasonStopMaxAnalog4:
      value = 0x1B; break;
    case LastReasonStopHackSu:
      value = 0x1D; break;
    case LastReasonStopManometr:
      value = 0x1E; break;
    case LastReasonStopNoVoltage:
      value = 0x22; break;
    case LastReasonStopProgram:
      value = 0x23; break;
    case LastReasonStopOperator:
      value = 0x24; break;
    case LastReasonStopOvernumberOfStart:
      value = 0x25; break;
    case LastReasonStopRemote:
      value = 0x28; break;
    }

    if ((reasonRun == LastReasonRunOperator) || (reasonRun == LastReasonRunRemote))
      value |= 1 << 8;
    if (reasonRun == LastReasonRunProgram)
      value |= 2 << 8;

    scadaParameters_[34].value.float_t = value;

    // 291 Значение аварийного параметра
    switch (reasonStop) {
    case LastReasonStopOverVoltIn:
    case LastReasonStopUnderVoltIn:
    case LastReasonStopImbalanceVoltIn:
    case LastReasonStopOverloadMotor:
    case LastReasonStopUnderloadMotor:
    case LastReasonStopImbalanceCurMotor:
    case LastReasonStopCurrentMotor:
    case LastReasonStopVibrationMotor:
    case LastReasonStopTemperatureMotor:
    case LastReasonStopPressureIntake:
      scadaParameters_[35].value.float_t = parameters.get(CCS_PROT_VALUE_PARAMETER);
      break;
    }

    // 295-296
    unTypeData data;
    data.uint32_t = parameters.get(CCS_GENERAL_RUN_DATE_TIME);
    scadaParameters_[39].value.float_t = data.uint16_t[0];
    scadaParameters_[40].value.float_t = data.uint16_t[1];

    // 298-299
    data.uint32_t = parameters.get(EM_ACTIVE_ENERGY)*3600;
    scadaParameters_[42].value.float_t = data.uint16_t[0];
    scadaParameters_[43].value.float_t = data.uint16_t[1];
  }
}

int ScadaKarakuduk::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }
  return err_r;
}

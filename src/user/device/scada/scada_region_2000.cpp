#include "scada_region_2000.h"
#include "user_main.h"

ScadaRegion2000::ScadaRegion2000()
{
  countParameters_ = 75;
  initParameters();
}

ScadaRegion2000::~ScadaRegion2000()
{

}

void ScadaRegion2000::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 256
    uint16_t value = 0;
    int reason = parameters.get(CCS_LAST_STOP_REASON);
    switch (reason) {
    case LastReasonStopOverVoltIn:
      value = 1; break;
    case LastReasonStopUnderVoltIn:
      value = 2; break;
    case LastReasonStopImbalanceVoltIn:
      value = 3; break;
    case LastReasonStopOverloadMotor:
      value = 4; break;
    case LastReasonStopUnderloadMotor:
      value = 5; break;
    case LastReasonStopImbalanceCurMotor:
      value = 6; break;
    case LastReasonStopPressureIntake:
      value = 7; break;
    case LastReasonStopResistIsolation:
      value = 8; break;
    case LastReasonStopManometr:
      value = 9; break;
    case LastReasonStopNoVoltage:
      value = 12; break;
    case LastReasonStopHackSu:
      value = 13; break;
    case LastReasonStopOperator:
      value = 14; break;
    case LastReasonStopProgram:
      value = 15; break;
    case LastReasonStopRemote:
      value = 21; break;
    case LastReasonStopHardwareVsd:
      value = 23; break;
    default:
      if (reason)
        value = 255;
      break;
    }
    scadaParameters_[0].value.float_t = value;

    // 257
    value = 0;
    value |= (ksu.isRunOrWorkMotor() << 0);
    value |= (ksu.isBlock() << 1);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT) > 3) << 2);
    value |= (ksu.isAutoMode() << 3);
    value |= (ksu.isManualMode() << 4);

    if (parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT))
      value |= (1 << 5);
    if (parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT))
      value |= (1 << 6);
    if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT))
      value |= (1 << 7);
    if (parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT))
      value |= (1 << 8);
    if (parameters.get(CCS_PROT_DHS_RESISTANCE_PREVENT))
      value |= (1 << 9);
    if (parameters.get(CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT))
      value |= (1 << 10);
    if (parameters.get(CCS_PROT_DI_1_PREVENT))
      value |= (1 << 11);
    if (parameters.get(CCS_PROT_OTHER_LOCK_DOOR_PREVENT))
      value |= (1 << 12);
    if (parameters.get(CCS_PROT_MOTOR_ASYNC_PREVENT))
      value |= (1 << 13);
    if (parameters.get(CCS_PROT_SUPPLY_PHASE_ROTATION_PREVENT))
      value |= (1 << 14);
    if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PREVENT) ||
        parameters.get(CCS_PROT_MOTOR_OVERLOAD_PREVENT) ||
        parameters.get(CCS_PROT_MOTOR_UNDERLOAD_PREVENT) ||
        parameters.get(CCS_PROT_MOTOR_CURRENT_PREVENT) ||
        parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_PREVENT) ||
        parameters.get(CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT) ||
        parameters.get(CCS_PROT_DHS_VIBRATION_PREVENT) ||
        parameters.get(CCS_PROT_DI_2_PREVENT) ||
        parameters.get(CCS_PROT_DI_3_PREVENT) ||
        parameters.get(CCS_PROT_DI_4_PREVENT) ||
        parameters.get(CCS_PROT_OTHER_VSD_PREVENT) ||
        parameters.get(CCS_PROT_OTHER_IMB_PREVENT) ||
        parameters.get(CCS_PROT_AI_1_PREVENT) ||
        parameters.get(CCS_PROT_AI_2_PREVENT) ||
        parameters.get(CCS_PROT_AI_3_PREVENT) ||
        parameters.get(CCS_PROT_AI_4_PREVENT) ||
        parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_PREVENT) ||
        parameters.get(CCS_PROT_DHS_FLOW_DISCHARGE_PREVENT) ||
        parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_PREVENT) ||
        parameters.get(CCS_PROT_SUPPLY_POWEROFF_PREVENT))
      value |= (1 << 15);

    scadaParameters_[1].value.float_t = value;
  }
}

int ScadaRegion2000::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }
  return err_r;
}

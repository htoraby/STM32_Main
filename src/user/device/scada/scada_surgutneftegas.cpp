#include "scada_surgutneftegas.h"
#include "user_main.h"
#include "regime.h"
#include "string.h"

#define PROTOCOL_CODE 2

ScadaSurgutneftegas::ScadaSurgutneftegas()
{
  countParameters_ = 256;
  initParameters();
}

ScadaSurgutneftegas::~ScadaSurgutneftegas()
{

}

void ScadaSurgutneftegas::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    uint16_t value = 0;

    // 25
    value = !parameters.get(CCS_PHASE_ROTATION);
    scadaParameters_[25].value.float_t = value;

    // 46
    value = parameters.get(CCS_RGM_PERIODIC_MODE) ? 1 : 0;
    scadaParameters_[46].value.float_t = value;

    // 77-78
    unTypeData data;
    data.uint32_t = parameters.get(EM_ACTIVE_ENERGY);
    scadaParameters_[77].value.uint32_t = data.uint16_t[1];
    scadaParameters_[78].value.uint32_t = data.uint16_t[0];

    // 91-92
    data.uint32_t = parameters.get(EM_REACTIVE_ENERGY);
    scadaParameters_[91].value.uint32_t = data.uint16_t[1];
    scadaParameters_[92].value.uint32_t = data.uint16_t[0];

    // 143-144
    time_t time = parameters.getU32(CCS_DATE_CREATE_SW_CCS);
    tm dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[143].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[144].value.uint32_t = dateTime.tm_mday*100;

    // 145-148
    char numProd[20];
    data.uint32_t = parameters.get(CCS_NUM_PRODUCTION_CCS);
    sprintf(numProd, "%09d", (int)data.uint32_t);
    memcpy(&numProd[0], &numProd[1], 8);
    scadaParameters_[145].value.char_t[0] = numProd[7];
    scadaParameters_[145].value.char_t[1] = numProd[6];
    scadaParameters_[146].value.char_t[0] = numProd[5];
    scadaParameters_[146].value.char_t[1] = numProd[4];
    scadaParameters_[147].value.char_t[0] = numProd[3];
    scadaParameters_[147].value.char_t[1] = numProd[2];
    scadaParameters_[148].value.char_t[0] = numProd[1];
    scadaParameters_[148].value.char_t[1] = numProd[0];

    // 149-150
    time = parameters.getU32(CCS_DATE_PRODUCTION_CCS);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[149].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[150].value.uint32_t = dateTime.tm_mday*100;

    // 152-153
    time = parameters.getU32(CCS_DATE_INSTALL_SW_CCS);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[152].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[153].value.uint32_t = dateTime.tm_mday*100;

    // 158-161
    data.uint32_t = parameters.get(CCS_NUM_PRODUCTION_SU);
    sprintf(numProd, "%09d", (int)data.uint32_t);
    memcpy(&numProd[0], &numProd[1], 8);
    scadaParameters_[158].value.char_t[0] = numProd[7];
    scadaParameters_[158].value.char_t[1] = numProd[6];
    scadaParameters_[159].value.char_t[0] = numProd[5];
    scadaParameters_[159].value.char_t[1] = numProd[4];
    scadaParameters_[160].value.char_t[0] = numProd[3];
    scadaParameters_[160].value.char_t[1] = numProd[2];
    scadaParameters_[161].value.char_t[0] = numProd[1];
    scadaParameters_[161].value.char_t[1] = numProd[0];

    // 162-163
    time = parameters.getU32(CCS_DATE_PRODUCTION_SU);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[162].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[163].value.uint32_t = dateTime.tm_mday*100;

    // 168
    data.uint32_t = parameters.get(CCS_SCADA_BYTERATE);
    switch (data.uint32_t) {
    case 1200: value = 0; break;
    case 2400: value = 1; break;
    case 4800: value = 2; break;
    case 9600: value = 3; break;
    case 19200: value = 4; break;
    case 38400: value = 5; break;
    case 57600: value = 6; break;
    case 115200: value = 7; break;
    default: value = 8; break;
    }
    scadaParameters_[168].value.float_t = value;

    // 169
    scadaParameters_[169].value.float_t = PROTOCOL_CODE;

    // 191-193
    time = parameters.getU32(CCS_LAST_RUN_DATE_TIME);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[191].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[192].value.uint32_t = dateTime.tm_mday*100 + dateTime.tm_hour;
    scadaParameters_[193].value.uint32_t = dateTime.tm_min*100 + dateTime.tm_sec;

    // 194-196
    time = parameters.getU32(CCS_LAST_STOP_DATE_TIME);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[194].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[195].value.uint32_t = dateTime.tm_mday*100 + dateTime.tm_hour;
    scadaParameters_[196].value.uint32_t = dateTime.tm_min*100 + dateTime.tm_sec;

    // 211
    value = parameters.get(CCS_RGM_PERIODIC_RUN_TIME_TO_END);
    if (value == 0)
      value = parameters.get(CCS_RGM_PERIODIC_STOP_TIME_TO_END);
    scadaParameters_[211].value.float_t = value;

    // 214-216
    time = parameters.getU32(CCS_DATE_TIME);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    scadaParameters_[214].value.uint32_t = dateTime.tm_year*100 + dateTime.tm_mon + 1;
    scadaParameters_[215].value.uint32_t = dateTime.tm_mday*100 + dateTime.tm_hour;
    scadaParameters_[216].value.uint32_t = dateTime.tm_min*100 + dateTime.tm_sec;

    // 250
    value = 0;
    if (ksu.isBlock() && (parameters.get(CCS_LAST_STOP_REASON) == LastReasonStopRemote))
      value |= (1 << 3);
    if (ksu.isRestart())
      value |= (1 << 4);
    if (!parameters.get(CCS_DOOR_VALUE))
      value |= (1 << 6);
    scadaParameters_[250].value.uint32_t = value;

    // 254
    value = 0;
    if (parameters.get(CCS_PROT_MOTOR_OVERLOAD_DELAY_FLAG))
      value |= (1 << 0);
    if (parameters.get(CCS_PROT_MOTOR_UNDERLOAD_DELAY_FLAG))
      value |= (1 << 1);
    if (parameters.get(CCS_PROT_DHS_RESISTANCE_PREVENT))
      value |= (1 << 2);
    if (parameters.get(CCS_PROT_MOTOR_ASYNC_PREVENT))
      value |= (1 << 3);
    if (parameters.get(CCS_PROT_SUPPLY_PHASE_ROTATION_PREVENT))
      value |= (1 << 4);
    if (parameters.get(CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT) ||
        parameters.get(CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT))
      value |= (1 << 5);
    // TODO. Контактный манометр
    // TODO. Поломка контактора
    if (parameters.get(CCS_PROT_OTHER_LOCK_DOOR_PREVENT))
      value |= (1 << 8);
    if (ksu.isBlock())
      value |= (1 << 9);
    // TODO. Работает оператор
    if (parameters.get(CCS_PROT_MOTOR_IMBALANCE_CURRENT_DELAY_FLAG))
      value |= (1 << 13);
    if (parameters.get(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT))
      value |= (1 << 14);
    if (ksu.isWorkMotor())
      value |= (1 << 15);
    scadaParameters_[254].value.uint32_t = value;
  }
}

int ScadaSurgutneftegas::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  // 46
  if (param->address == 46) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OffAction, RemoteType);
  }

  return err_r;
}

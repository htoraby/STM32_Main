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

    // 20
    value = ((uint8_t)(parameters.get(CCS_WORKING_MODE)+1) << 8) + ((uint8_t)parameters.get(CCS_LAST_RUN_REASON) & 0x1F);
    scadaParameters_[20].value.float_t = value;

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

    // 85
    value = parameters.get(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY);
    scadaParameters_[85].value.float_t = value;

    // 91-92
    data.uint32_t = parameters.get(EM_REACTIVE_ENERGY);
    scadaParameters_[91].value.uint32_t = data.uint16_t[1];
    scadaParameters_[92].value.uint32_t = data.uint16_t[0];

    // 125
    value = parameters.get(CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_RESET);
    scadaParameters_[85].value.float_t = value;

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
    data.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_CCS);
    sprintf(numProd, "%09d", (int)data.uint32_t);
    scadaParameters_[145].value.char_t[0] = numProd[8];
    scadaParameters_[145].value.char_t[1] = numProd[7];
    scadaParameters_[146].value.char_t[0] = numProd[6];
    scadaParameters_[146].value.char_t[1] = numProd[5];
    scadaParameters_[147].value.char_t[0] = numProd[4];
    scadaParameters_[147].value.char_t[1] = numProd[3];
    scadaParameters_[148].value.char_t[0] = numProd[2];
    scadaParameters_[148].value.char_t[1] = numProd[1];

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
    data.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_SU);
    sprintf(numProd, "%09d", (int)data.uint32_t);
    scadaParameters_[158].value.char_t[0] = numProd[8];
    scadaParameters_[158].value.char_t[1] = numProd[7];
    scadaParameters_[159].value.char_t[0] = numProd[6];
    scadaParameters_[159].value.char_t[1] = numProd[5];
    scadaParameters_[160].value.char_t[0] = numProd[4];
    scadaParameters_[160].value.char_t[1] = numProd[3];
    scadaParameters_[161].value.char_t[0] = numProd[2];
    scadaParameters_[161].value.char_t[1] = numProd[1];

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
  unTypeData data;
  time_t time;

  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  // 46
  if (param->address == 46) {
    if (param->value.float_t)
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OnAction, RemoteType);
    else
      parameters.set(CCS_RGM_PERIODIC_MODE, Regime::OffAction, RemoteType);
    return ok_r;
  }

  // 85
  if (param->address == 85) {
    parameters.set(CCS_PROT_DHS_FLOW_DISCHARGE_ACTIV_DELAY, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_PRESSURE_DISCHARGE_ACTIV_DELAY, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_VIBRATION_ACTIV_DELAY, param->value.float_t, RemoteType);
    return ok_r;
  }

  // 125
  if (param->address == 125) {
    parameters.set(CCS_PROT_DI_1_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DI_2_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DI_3_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DI_4_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_AI_1_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_AI_2_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_AI_3_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_AI_4_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_FLOW_DISCHARGE_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_PRESSURE_DISCHARGE_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_DHS_VIBRATION_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_OTHER_VSD_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET, param->value.float_t, RemoteType);
    parameters.set(CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET, param->value.float_t, RemoteType);
    return ok_r;
  }

  // 158-161
  if ((param->address >= 158) && (param->address <= 161)) {
    char numProd[20];
    data.uint32_t = parameters.getU32(CCS_NUM_PRODUCTION_SU);
    sprintf(numProd, "%09d", (int)data.uint32_t);
    switch (param->address) {
    case 158:
      numProd[8] = param->value.char_t[0];
      numProd[7] = param->value.char_t[1];
      break;
    case 159:
      numProd[6] = param->value.char_t[0];
      numProd[5] = param->value.char_t[1];
      break;
    case 160:
      numProd[4] = param->value.char_t[0];
      numProd[3] = param->value.char_t[1];
      break;
    case 161:
      numProd[2] = param->value.char_t[0];
      numProd[1] = param->value.char_t[1];
      break;
    }
    data.uint32_t = atoi(numProd);
    parameters.set(CCS_NUM_PRODUCTION_SU, (uint32_t)data.uint32_t, RemoteType);
    return ok_r;
  }

  // 162-163
  if ((param->address >= 162) && (param->address <= 163)) {
    time = parameters.getU32(CCS_DATE_PRODUCTION_SU);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 162:
      dateTime.tm_year = param->value.uint32_t/100 + 100;
      dateTime.tm_mon = param->value.uint32_t%100 - 1;
      break;
    case 163:
      dateTime.tm_mday = param->value.uint32_t/100;
      break;
    }

    if ((dateTime.tm_mon < 0) || (dateTime.tm_mon > 11) || (dateTime.tm_mday > 31))
      return err_r;

    time = mktime(&dateTime);
    parameters.set(CCS_DATE_PRODUCTION_SU, (uint32_t)time, RemoteType);
    return ok_r;
  }

  // 168
  if (param->address == 168) {
    switch (param->value.uint32_t) {
    case 0: data.float_t = 1200; break;
    case 1: data.float_t = 2400; break;
    case 2: data.float_t = 4800; break;
    case 3: data.float_t = 9600; break;
    case 4: data.float_t = 19200; break;
    case 5: data.float_t = 38400; break;
    case 6: data.float_t = 57600; break;
    case 7: data.float_t = 115200; break;
    default:
      return err_r;
    }
    parameters.set(CCS_SCADA_BYTERATE, data.float_t, RemoteType);
    return ok_r;
  }

  // 214-216
  if ((param->address >= 214) && (param->address <= 216)) {
    time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 214:
      dateTime.tm_year = param->value.uint32_t/100 + 100;
      dateTime.tm_mon = param->value.uint32_t%100 - 1;
      break;
    case 215:
      dateTime.tm_mday = param->value.uint32_t/100;
      dateTime.tm_hour = param->value.uint32_t%100;
      break;
    case 216:
      dateTime.tm_min = param->value.uint32_t/100;
      dateTime.tm_sec = param->value.uint32_t%100;
      break;
    }

    if ((dateTime.tm_mon < 0) || (dateTime.tm_mon > 11) || (dateTime.tm_mday > 31) ||
        (dateTime.tm_hour > 24) || (dateTime.tm_min > 60) || (dateTime.tm_sec > 60))
      return err_r;

    time = mktime(&dateTime);
    parameters.set(CCS_DATE_TIME, (uint32_t)time, RemoteType);
    return ok_r;
  }

  return err_r;
}

static void getParam(uint16_t address, uint8_t * pucFrame, uint16_t * usLen)
{
  uint16_t usLength = *usLen;
  unTypeData data;
  data.uint32_t = 0;
  ScadaParameter *param = scada->parameter(address);

  if (param != NULL) {
    float value = param->value.float_t;
    value = convertFrom(value, param->physic, param->unit);
    value = value / param->coefficient;
    data.uint16_t[0] = decToBCD(lround(value));
  }

  pucFrame[usLength++] = data.char_t[0];
  pucFrame[usLength++] = data.char_t[1];
  *usLen = usLength;
}

eMbSngException eMbSngFuncReadCurrentState(uint8_t * pucFrame, uint16_t *usLen)
{
  eMbSngException eStatus = MB_SNG_EX_NONE;
  uint16_t usLength = 0;

  pucFrame[usLength++] = 0x10;
  pucFrame[usLength++] = 0;

  ScadaParameter *param = scada->parameter(254);
  pucFrame[usLength++] = param->value.char_t[0];
  pucFrame[usLength++] = param->value.char_t[1];

  pucFrame[usLength++] = ((uint8_t)parameters.get(CCS_LAST_STOP_REASON) & 0x3F);

  pucFrame[usLength++] = 0xFF;
  pucFrame[usLength++] = 0xFF;
  pucFrame[usLength++] = 0xFF;

  for (int i = 0; i < 24; ++i) {
    getParam(i, pucFrame, &usLength);
  }

  pucFrame[MB_SNG_DATA_FUNC_OFF+1] = (uint8_t)(usLength - 2);
  *usLen = usLength;

  return eStatus;
}

eMbSngException eMbSngFuncRandomSample(uint8_t * pucFrame, uint16_t * usLen)
{
  eMbSngException eStatus = MB_SNG_EX_NONE;
  uint16_t usLength = 0;

  uint8_t paramCount = pucFrame[MB_SNG_DATA_FUNC_OFF+1];
  if (paramCount > 10)
    return MB_SNG_EX_ILLEGAL_LEN;

  uint8_t paramIndex[10];
  for (int i = 0; i < paramCount; ++i) {
    paramIndex[i] = pucFrame[MB_SNG_DATA_FUNC_OFF+2 + i];
  }

  pucFrame[usLength++] = 0x20;
  pucFrame[usLength++] = 0;

  for (int i = 0; i < paramCount; ++i) {
    pucFrame[usLength++] = paramIndex[i];
    getParam(paramIndex[i], pucFrame, &usLength);
  }

  pucFrame[MB_SNG_DATA_FUNC_OFF+1] = (uint8_t)(usLength - 2);
  *usLen = usLength;

  return eStatus;
}

eMbSngException eMbSngFuncSampleArchive(uint8_t * pucFrame, uint16_t * usLen)
{
  eMbSngException eStatus = MB_SNG_EX_ILLEGAL_DATA;

  return eStatus;
}

eMbSngException eMbSngFuncWriteRegister(uint8_t * pucFrame, uint16_t * usLen)
{
  eMbSngException eStatus = MB_SNG_EX_NONE;
  uint16_t usLength = 0;
  unTypeData data;

  uint8_t paramCount = (pucFrame[MB_SNG_DATA_FUNC_OFF+1]-2)/3;
  for (int i = 0; i < paramCount; ++i) {
    uint8_t address = pucFrame[MB_SNG_DATA_FUNC_OFF+2 + i*3];
    ScadaParameter *param = scada->parameter(address);
    if (param == NULL)
      continue;
    if (param->operation == OPERATION_READ)
      return MB_SNG_EX_ERROR_WRITE;
    if ((param->operation == OPERATION_LIMITED) && ksu.isWorkMotor())
      return MB_SNG_EX_CMD_BLOCK;

    data.uint32_t = 0;
    data.char_t[0] = pucFrame[MB_SNG_DATA_FUNC_OFF+2 + i*3 + 1];
    data.char_t[1] = pucFrame[MB_SNG_DATA_FUNC_OFF+2 + i*3 + 2];
    data.uint32_t = bcdToDec(data.uint16_t[0]);
    if (checkRange(data.uint32_t, param->min, param->max, true) != ok_r)
      return MB_SNG_EX_ILLEGAL_DATA;

    float value = (float)data.uint32_t;
    value = value * param->coefficient;
    value = convertTo(value, param->physic, param->unit);
    param->value.float_t = value;
    if (scada->setNewValue(param) != ok_r)
      return MB_SNG_EX_ILLEGAL_DATA;
  }

  pucFrame[usLength++] = 0x40;
  pucFrame[usLength++] = 0;
  *usLen = usLength;

  return eStatus;
}

eMbSngException eMbSngFuncRestartInterfaceUnit(uint8_t * pucFrame, uint16_t * usLen)
{
  eMbSngException eStatus = MB_SNG_EX_ILLEGAL_DATA;

  return eStatus;
}

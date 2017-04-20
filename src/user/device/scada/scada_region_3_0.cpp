#include "scada_region_3_0.h"
#include "user_main.h"
#include "log_tms.h"

#if USE_EXT_MEM
static uint8_t logDhs[1312] __attribute__((section(".extmem")));
#else
static uint8_t logDhs[1312];
#endif

ScadaRegion30::ScadaRegion30()
{
  countParameters_ = 173;
  initParameters();
  firstAddrDhsLog_ = parameters.get(CCS_DHS_LOG_ROSNEFT_FIRST_REGISTER);
  lastAddrDhsLog_ = parameters.get(CCS_DHS_LOG_ROSNEFT_LAST_REGISTER);
  logDhs_ = logDhs;
}

ScadaRegion30::~ScadaRegion30()
{

}

eMBErrorCode ScadaRegion30::readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  // Если первый из запрашиваемых регистров лежит в адресном поле архивов ТМС(ГДИ)
  if ((address >= firstAddrDhsLog_) && (address <= lastAddrDhsLog_)) {
    // Вызываем функцию обработки запроса чтения архива ТМС(ГДИ)
    return readRegDhsLog(buffer, address, numRegs);
  }
  else {
    // Вызываем функцию чтения параметров
    return Scada::readReg(buffer, address, numRegs);
  }
}

void ScadaRegion30::calcParamsTask()
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
    value |= (ksu.isRunOrWorkMotor() << 0);
    value |= (ksu.isBlock() << 1);
    value |= ((parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT) > 3) << 2);
    value |= (ksu.isAutoMode() << 4);
    value |= (ksu.isManualMode() << 5);
    value |= ((parameters.get(CCS_DHS_CONNECTION) && parameters.get(CCS_DHS_TYPE)) << 6);
    value |= ((parameters.get(CCS_EM_CONNECTION) && parameters.get(CCS_EM_TYPE)) << 7);
    scadaParameters_[0].value.float_t = value;

    // 257-258
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
          parameters.get(CCS_PROT_OTHER_IMB_PREVENT) ||
          parameters.get(CCS_PROT_OTHER_LIMIT_RESTART_PREVENT) ||
          parameters.get(CCS_PROT_DHS_FLOW_DISCHARGE_PREVENT) ||
          parameters.get(CCS_PROT_DHS_PRESSURE_DISCHARGE_PREVENT) ||
          parameters.get(CCS_PROT_SUPPLY_POWEROFF_PREVENT))
        value |= (1 << 15);
      scadaParameters_[2].value.float_t = value;
    }
    else {
      scadaParameters_[1].value.float_t = value;
      scadaParameters_[2].value.float_t = value;
    }

    // 259
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

    //260-262
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

    //301
    value = 0;
    uint8_t type = parameters.get(CCS_DHS_TYPE);
    switch (type) {
    case TYPE_DHS_NOVOMET:
      value = 9; break;
    case TYPE_DHS_ALMAZ:
      value = 2; break;
    case TYPE_DHS_BORETS_1:
    case TYPE_DHS_BORETS_2:
      value = 3; break;
    case TYPE_DHS_ELEKTON_2:
    case TYPE_DHS_ELEKTON_3:
      value = 1; break;
    case TYPE_DHS_ETALON:
      value = 12; break;
    case TYPE_DHS_IRZ:
      value = 6; break;
    case TYPE_DHS_ORION:
      value = 11; break;
    case TYPE_DHS_PIC_V2:
      value = 9; break;
    case TYPE_DHS_SCAD:
      value = 13; break;
    case TYPE_DHS_SCAN:
      value = 13; break;
    case TYPE_DHS_TRIOL:
      value = 10; break;
    case TYPE_DHS_ZENITH:
      value = 4; break;
    }
    scadaParameters_[45].value.float_t = value;

    float cntRecord = parameters.get(CCS_DHS_LOG_COUNT_RECORD);
    if (cntRecord < ((lastAddrDhsLog_ - firstAddrDhsLog_) / SIZE_RECORD_DHS_LOG)) {
      cntRecord = firstAddrDhsLog_ + (cntRecord - 1) * SIZE_RECORD_DHS_LOG;
    }
    parameters.set(CCS_DHS_LOG_ROSNEFT_LAST_RECORD, cntRecord);
    scadaParameters_[74].value.float_t = cntRecord;

    //346-348, 634-636
    value = 0;
    time = parameters.getU32(CCS_DATE_TIME);
    dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    value = (dateTime.tm_year << 8) + (dateTime.tm_mon + 1);
    scadaParameters_[70].value.float_t = value;
    scadaParameters_[167].value.float_t = value;
    value = (dateTime.tm_mday << 8) + dateTime.tm_hour;
    scadaParameters_[71].value.float_t = value;
    scadaParameters_[168].value.float_t = value;
    value = (dateTime.tm_min << 8) + dateTime.tm_sec;
    scadaParameters_[72].value.float_t = value;
    scadaParameters_[169].value.float_t = value;
  }
}

int ScadaRegion30::setNewValue(ScadaParameter *param)
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

eMBErrorCode ScadaRegion30::readRegDhsLog(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  // Если адресация выходит за пределы области архивов ГДИ
  if ((address + numRegs) > lastAddrDhsLog_) {
    return MB_ENOREG;                       // Возвращаем ошибку регистра
  }
  // Определяем по адресу в modbus запросе номер записи в архиве ГДИ
  div_t x;
  x = div(address - firstAddrDhsLog_, SIZE_RECORD_DHS_LOG);
  uint32_t numRecord = x.quot;
  // Определяем по количеству читаемых регистров в modbus запросе,
  // количество читаемых записей из архива ГДИ
  x = div(numRegs, SIZE_RECORD_DHS_LOG);
  uint32_t quantityRecord = x.quot;
  // Вычисляем сколько записей от первой запрашиваемой до конца архива ГДИ, для
  // нахождения смещения от последней записи в архивах на flash
  uint32_t recordToLast = (uint32_t)parameters.get(CCS_DHS_LOG_COUNT_RECORD) - numRecord;
  // Вызываем функцию читающую в буфер logDhs_, quantityRecord записей начиная recordToLast от конца архива
  logTms.readLogRequestedRosneft(recordToLast, logDhs_, quantityRecord);
  // Разбор записей из ТМС ловов и приведение их к формату Роснефть
  for (uint32_t i = 0; i < quantityRecord; i++) {
    // Код ошибки по ЕТТ 6.0 Рснефть
    *buffer++ = logDhs_[17 + LOG_DHS_SIZE * i];
    // Дата и время
    unTypeData value;
    value.char_t[0] = logDhs_[4 + LOG_DHS_SIZE * i];
    value.char_t[1] = logDhs_[5 + LOG_DHS_SIZE * i];
    value.char_t[2] = logDhs_[6 + LOG_DHS_SIZE * i];
    value.char_t[3] = logDhs_[7 + LOG_DHS_SIZE * i];
    time_t time = value.uint32_t;
    tm dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    *buffer++ = dateTime.tm_year;
    *buffer++ = dateTime.tm_mon + 1;
    *buffer++ = dateTime.tm_mday;
    *buffer++ = dateTime.tm_hour;
    *buffer++ = dateTime.tm_min;
    *buffer++ = dateTime.tm_sec;
    // Температура на приёме насоса
    value.char_t[0] = logDhs_[18 + LOG_DHS_SIZE * i];
    value.char_t[1] = logDhs_[19 + LOG_DHS_SIZE * i];
    value.char_t[2] = logDhs_[20 + LOG_DHS_SIZE * i];
    value.char_t[3] = logDhs_[21 + LOG_DHS_SIZE * i];
    value.uint16_t[0] = value.float_t * 100;
    *buffer++ = value.char_t[0];
    *buffer++ = value.char_t[1];
    // Давление на приёме насоса
    value.char_t[0] = logDhs_[9 + LOG_DHS_SIZE * i];
    value.char_t[1] = logDhs_[10 + LOG_DHS_SIZE * i];
    value.char_t[2] = logDhs_[11 + LOG_DHS_SIZE * i];
    value.char_t[3] = logDhs_[12 + LOG_DHS_SIZE * i];
    value.uint16_t[0] = value.float_t * 10.19716213 * 100;
  }
  return MB_ENOERR;
}

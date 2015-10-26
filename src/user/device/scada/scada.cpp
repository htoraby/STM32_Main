#include "scada.h"
#include "user_main.h"
#include "string.h"

#if USE_EXT_MEM
static ScadaParameter scadaParameters[1000] __attribute__((section(".extmem")));
#else
static ScadaParameter scadaParameters[1000];
#endif

static void scadaTask(void *p)
{
  (static_cast<Scada*>(p))->task();
}

static void scadaCalcParamsTask(void *p)
{
  (static_cast<Scada*>(p))->calcParamsTask();
}

Scada::Scada()
{
  scadaParameters_ = scadaParameters;
  osThreadDef(ScadaTask, scadaTask, osPriorityNormal, 0, 4*configMINIMAL_STACK_SIZE);
  mbThreadId_ = osThreadCreate(osThread(ScadaTask), this);

  osThreadDef(ScadaCalcParams, scadaCalcParamsTask, osPriorityNormal, 0, 4*configMINIMAL_STACK_SIZE);
  calcParamsThreadId_ = osThreadCreate(osThread(ScadaCalcParams), this);
}

Scada::~Scada()
{
  eMBDisable();
  eMBClose();

  osThreadTerminate(mbThreadId_);
  osThreadTerminate(calcParamsThreadId_);
}

void Scada::task()
{
  uint8_t address = parameters.get(CCS_SCADA_ADDRESS);
  uint32_t baudRate = parameters.get(CCS_SCADA_BYTERATE);
  eMBParity parity = (eMBParity)parameters.get(CCS_SCADA_PARITY);
  delay_ = parameters.get(CCS_SCADA_DELAY);
  delay_ = parameters.convertFrom(delay_, PHYSIC_TIME, TIME_MS);

  eMBInit(MB_RTU, address, SCADA_UART, baudRate,  parity);
  eMBEnable();
  while (1) {
    eMBPoll();
    osDelay(1);
  }
}

void Scada::calcParamsTask()
{

}

int Scada::setNewValue(ScadaParameter */*param*/)
{
  return err_r;
}

inline int Scada::sizeDataFromTypeData(uint8_t typeData)
{
  switch (typeData) {
  case TYPE_DATA_CHAR: case TYPE_DATA_INT16: case TYPE_DATA_UINT16:
    return 2;
  case TYPE_DATA_INT32: case TYPE_DATA_UINT32: case TYPE_DATA_FLOAT:
    return 4;
  default:
    return -1;
  }
}

inline int Scada::getIndexAtAddress(int address)
{
  address = address - 1;
  if (address >= 0) {
    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].address == address)
        return i;
    }
  }

  logDebug.add(WarningMsg, "Скада. Регистр %d не найден по указаннному адресу", address);

  return -1;
}

eMBErrorCode Scada::readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  for (int i = 0; i < numRegs; ++i) {
    int index = getIndexAtAddress(address + i);
    if (index == -1)
      return MB_ENOREG;

    ScadaParameter *param = &scadaParameters_[index];
    int sizeData = sizeDataFromTypeData(param->typeData);
    if (sizeData == -1)
      return MB_EINVAL;

    float value = param->value.float_t;
    value = parameters.convertFrom(value, param->physic, param->unit);
    value = value / param->coefficient;
    unTypeData data;
    if (param->typeData == TYPE_DATA_FLOAT)
      data.float_t = value;
    else
      data.uint32_t = lround(value);

    if (sizeData == 2) {
      *buffer++ = data.char_t[1];
      *buffer++ = data.char_t[0];
    }
    else {
      i++;
      if (i >= numRegs)
        return MB_ENOREG;

      *buffer++ = data.char_t[3];
      *buffer++ = data.char_t[2];
      *buffer++ = data.char_t[1];
      *buffer++ = data.char_t[0];
    }
  }

  return MB_ENOERR;
}

eMBErrorCode Scada::writeReg(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  for (int i = 0; i < numRegs; ++i) {
    int index = getIndexAtAddress(address + i);
    if (index == -1)
      return MB_ENOREG;

    ScadaParameter *param = &scadaParameters_[index];
    if (param->operation != OPERATION_WRITE)
      return MB_EINVAL;

    int sizeData = sizeDataFromTypeData(param->typeData);
    if (sizeData == -1)
      return MB_EINVAL;

    unTypeData data;
    if (sizeData == 2) {
      data.char_t[3] = 0;
      data.char_t[2] = 0;
      data.char_t[1] = *buffer++;
      data.char_t[0] = *buffer++;
    }
    else {
      i++;
      if (i >= numRegs)
        return MB_ENOREG;

      data.char_t[3] = *buffer++;
      data.char_t[2] = *buffer++;
      data.char_t[1] = *buffer++;
      data.char_t[0] = *buffer++;
    }

    if (param->typeData == TYPE_DATA_FLOAT) {
      if (checkRange(data.float_t, param->min, param->max, true))
        return MB_EINVAL;
    }
    else {
      if (checkRange(data.uint32_t, param->min, param->max, true))
        return MB_EINVAL;
    }

    float value;
    if (param->typeData == TYPE_DATA_FLOAT)
      value = data.float_t * param->coefficient;
    else
//TODO: проверить корректность при записи типов дата время
      value = /*lround*/(data.uint32_t * param->coefficient);
    value = parameters.convertTo(value, param->physic, param->unit);
    param->value.float_t = value;

    if (setNewValue(param) != ok_r)
      return MB_EINVAL;
  }

  return MB_ENOERR;
}

eMBErrorCode Scada::writeCoils(uint8_t *buffer, uint16_t address, uint16_t numCoils)
{
  if (numCoils == 1) {
    int addr = address - 1;
    if (addr < 0)
      return MB_ENOREG;

    uint8_t value = *buffer++;

    switch (addr) {
    case 0x0000:
    case 0x0040:
    case 0x0201:
      if (value) {
        ksu.start(LastReasonRunRemote);
      }
      else {
        ksu.stop(LastReasonStopRemote);
        ksu.resetBlock();
      }
      return MB_ENOERR;
    case 0x0041:
      if (value) {
        ksu.stop(LastReasonStopRemote);
        ksu.resetBlock();
      }
      return MB_ENOERR;
    default:
      break;
    }

    return MB_ENOREG;
  }

  return MB_ENOREG;
}

eMBErrorCode Scada::readInfo(uint8_t *buffer, uint16_t address, uint16_t *len)
{
  int addr = address - 1;
  if (addr < 0)
    return MB_EINVAL;

  if (addr == 0x0E01) {
    *buffer++ = 0x0E;
    *buffer++ = 0x01;
    *buffer++ = 0x01;
    *buffer++ = 0x00;
    *buffer++ = 0x00;
    *buffer++ = 0x03;
    *len += 6;

    // Компания
    *buffer++ = 0x00;
    const char *company = "NOVOMET";
    int size = strlen(company);
    *buffer++ = size;
    memcpy(buffer, company, size);
    buffer += size;
    *len += 2 + size;

    // Код продукта
    *buffer++ = 0x01;
    const char *code = "-05 ";
    size = strlen(code);
    *buffer++ = size;
    memcpy(buffer, code, size);
    buffer += size;
    *len += 2 + size;

    // Версия
    *buffer++ = 0x02;
    char version[20];
    float ver = parameters.get(CCS_VERSION_SW_CCS);
    sprintf(version, "V%d", (int)ver);
    size = strlen(version);
    *buffer++ = size;
    memcpy(buffer, version, size);
    buffer += size;
    *len += 2 + size;

    return MB_ENOERR;
  }

  return MB_EINVAL;
}


eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
  eMBErrorCode status = MB_ENOERR;

  status = scada->readReg(pucRegBuffer, usAddress, usNRegs);

  osDelay(scada->delay());
  return status;
}

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{
  eMBErrorCode status = MB_ENOERR;

  switch (eMode) {
  case MB_REG_READ:
    status = scada->readReg(pucRegBuffer, usAddress, usNRegs);
    break;
  case MB_REG_WRITE:
    status = scada->writeReg(pucRegBuffer, usAddress, usNRegs);
    break;
  }

  osDelay(scada->delay());
  return status;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
                           USHORT usNCoils, eMBRegisterMode eMode)
{
  eMBErrorCode status = MB_ENOERR;

  switch (eMode) {
  case MB_REG_READ:
    status = MB_ENOREG;
    break;
  case MB_REG_WRITE:
    status = scada->writeCoils(pucRegBuffer, usAddress, usNCoils);
    break;
  }

  osDelay(scada->delay());
  return status;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
                              USHORT usNDiscrete)
{
  eMBErrorCode status = MB_ENOERR;

  status = MB_ENOREG;

  osDelay(scada->delay());
  return status;
}

eMBErrorCode eMBRegReadInfo(UCHAR * pucRegBuffer, USHORT usAddress, USHORT * usLen)
{
  eMBErrorCode status = scada->readInfo(pucRegBuffer, usAddress, usLen);

  osDelay(scada->delay());
  return status;
}

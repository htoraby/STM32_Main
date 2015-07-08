#include "scada.h"
#include "user_main.h"

static uint16_t buf[1000];
static uint16_t usRegHoldingStart = 1;

static void scadaTask(void *p)
{
  (static_cast<Scada*>(p))->task();
}

Scada::Scada()
{
  osThreadDef(ScadaTask, scadaTask, osPriorityLow, 0, 4*configMINIMAL_STACK_SIZE);
  mbThreadId_ = osThreadCreate(osThread(ScadaTask), this);
}

Scada::~Scada()
{
  eMBDisable();
  eMBClose();

  osThreadTerminate(mbThreadId_);
}

void Scada::task()
{
  uint8_t address = parameters.get(CCS_SCADA_ADDRESS);
  uint32_t baudRate = parameters.get(CCS_SCADA_BYTERATE);
  eMBParity parity = (eMBParity)parameters.get(CCS_SCADA_PARITY);
  delay_ = parameters.get(CCS_SCADA_DELAY);

  eMBInit(MB_RTU, address, SCADA_UART, baudRate,  parity);
  eMBEnable();
  while (1) {
    eMBPoll();
  }
}

eMBErrorCode Scada::readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  eMBErrorCode status = MB_ENOERR;
  // TODO: Проверка адреса регистра

  int iRegIndex = address - usRegHoldingStart;
  while (numRegs > 0) {
    *buffer++ = (UCHAR)(buf[iRegIndex] >> 8);
    *buffer++ = (UCHAR)(buf[iRegIndex] & 0xFF);
    iRegIndex++;
    numRegs--;
  }

  return status;
}

eMBErrorCode Scada::writeReg(uint8_t *buffer, uint16_t address, uint16_t numRegs)
{
  eMBErrorCode status = MB_ENOERR;
  // TODO: Проверка адреса регистра

  int iRegIndex = address - usRegHoldingStart;
  while (numRegs > 0) {
    buf[iRegIndex] = *buffer++ << 8;
    buf[iRegIndex] |= *buffer++;
    iRegIndex++;
    numRegs--;
  }
//      unTypeData data;
//      data.char_t[3] = buf[1] >> 8;
//      data.char_t[2] = buf[1] & 0xFF;
//      data.char_t[1] = buf[2] >> 8;
//      data.char_t[0] = buf[2] & 0xFF;
  asm("nop");

  return status;
}

eMBErrorCode Scada::writeCoils(uint8_t *buffer, uint16_t address, uint16_t numCoils)
{
  eMBErrorCode status = MB_ENOERR;

  status = MB_ENOREG;

  return status;
}

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
                           USHORT usNRegs)
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
  osDelay(scada->delay());
  return MB_ENOREG;
}

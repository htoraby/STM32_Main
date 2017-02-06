#include "stdlib.h"
#include "string.h"

#include "mb_slave_sng.h"
#include "common.h"

static UCHAR ucMBAddress;
static volatile eMbSngState eState = STATE_NOT_INITIALIZED;
static volatile eMbSngSndState eSndState;
static volatile eMbSngRcvState eRcvState;

volatile UCHAR ucBuf[MB_SNG_SIZE_MAX];

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;
static volatile USHORT usRcvBufferPos;

static eMBErrorCode eMbSngReceive(UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength);
static eMBErrorCode eMbSngSend(UCHAR slaveAddress, const UCHAR * pucFrame, USHORT usLength);
BOOL xMbSngReceiveFSM();
BOOL xMbSngTransmitFSM();
BOOL xMbSngTimerT35Expired();

BOOL(*pxMBFrameCBByteReceived)(void);
BOOL(*pxMBFrameCBTransmitterEmpty)(void);
BOOL(*pxMBPortCBTimerExpired)(void);

eMBErrorCode eMbSngInit(UCHAR ucSlaveAddress, UCHAR ucPort,
                        ULONG ulBaudRate, eMBParity eParity, UCHAR stopBits)
{
  eMBErrorCode eStatus = MB_ENOERR;
  ULONG usTimerT35_50us;

  if ((ucSlaveAddress < MB_ADDRESS_MIN) || (ucSlaveAddress > MB_ADDRESS_MAX)) {
    eStatus = MB_EINVAL;
  }
  else {
    ucMBAddress = ucSlaveAddress;

    pxMBFrameCBByteReceived = xMbSngReceiveFSM;
    pxMBFrameCBTransmitterEmpty = xMbSngTransmitFSM;
    pxMBPortCBTimerExpired = xMbSngTimerT35Expired;

    ENTER_CRITICAL_SECTION();
    if (xMBPortSerialInit(ucPort, ulBaudRate, 8, eParity, stopBits) != TRUE) {
      eStatus = MB_EPORTERR;
    }
    else {
      if (ulBaudRate > 19200) {
        usTimerT35_50us = 35;
      }
      else {
        usTimerT35_50us = (7UL * 220000UL) / (2UL * ulBaudRate);
      }
      if (xMBPortTimersInit((USHORT) usTimerT35_50us) != TRUE) {
        eStatus = MB_EPORTERR;
      }
    }
    EXIT_CRITICAL_SECTION();

    if (eStatus == MB_ENOERR) {
      if (!xMBPortEventInit()) {
        eStatus = MB_EPORTERR;
      }
      else {
        eState = STATE_DISABLED;
      }
    }
  }
  return eStatus;
}

eMBErrorCode eMbSngClose()
{
  eMBErrorCode eStatus = MB_ENOERR;

  if (eState == STATE_DISABLED) {
    vMBPortClose();
  }
  else {
    eStatus = MB_EILLSTATE;
  }
  return eStatus;
}

eMBErrorCode eMbSngEnable()
{
  eMBErrorCode eStatus = MB_ENOERR;

  if (eState == STATE_DISABLED) {
    ENTER_CRITICAL_SECTION();
    eRcvState = STATE_RX_INIT;
    vMBPortSerialEnable(TRUE, FALSE);
    vMBPortTimersEnable();
    EXIT_CRITICAL_SECTION();

    eState = STATE_ENABLED;
  }
  else {
    eStatus = MB_EILLSTATE;
  }
  return eStatus;
}

eMBErrorCode eMbSngDisable()
{
  eMBErrorCode eStatus;

  if (eState == STATE_ENABLED) {
    ENTER_CRITICAL_SECTION();
    vMBPortSerialEnable(FALSE, FALSE);
    vMBPortTimersDisable();
    EXIT_CRITICAL_SECTION();

    eState = STATE_DISABLED;
    eStatus = MB_ENOERR;
  }
  else if (eState == STATE_DISABLED) {
    eStatus = MB_ENOERR;
  }
  else {
    eStatus = MB_EILLSTATE;
  }
  return eStatus;
}

eMBErrorCode eMbSngPoll()
{
  static UCHAR *ucMBFrame;
  static UCHAR ucRcvAddress;
  static UCHAR ucFunctionCode;
  static USHORT usLength;
  static eMbSngException eException;

  eMBErrorCode eStatus = MB_ENOERR;
  eMBEventType eEvent;

  if (eState != STATE_ENABLED) {
    return MB_EILLSTATE;
  }

  if (xMBPortEventGet(&eEvent) == TRUE) {
    switch (eEvent) {
    case EV_READY:
      break;

    case EV_FRAME_RECEIVED:
      eStatus = eMbSngReceive(&ucRcvAddress, &ucMBFrame, &usLength);
      if (eStatus == MB_ENOERR) {
        // Проверка, если пакет нам
        if (ucRcvAddress == ucMBAddress) {
          (void)xMBPortEventPost(EV_EXECUTE);
        }
      }
      break;

    case EV_EXECUTE:
      eException = MB_SNG_EX_ILLEGAL_FUNCTION;
      ucFunctionCode = ucMBFrame[MB_SNG_DATA_FUNC_OFF];

      switch (ucFunctionCode) {
      case 0x10:
        eException = eMbSngFuncReadCurrentState(ucMBFrame, &usLength);
        break;
      case 0x20:
        eException = eMbSngFuncRandomSample(ucMBFrame, &usLength);
        break;
      case 0x30:
        eException = eMbSngFuncSampleArchive(ucMBFrame, &usLength);
        break;
      case 0x40:
        eException = eMbSngFuncWriteRegister(ucMBFrame, &usLength);
        break;
      case 0x50:
        eException = eMbSngFuncRestartInterfaceUnit(ucMBFrame, &usLength);
        break;
      }

      // Возврат кода ошибки
      if (eException != MB_SNG_EX_NONE) {
        usLength = 0;
        ucMBFrame[usLength++] = (UCHAR)(ucFunctionCode + eException);
        ucMBFrame[usLength++] = 0;
      }
      eStatus = eMbSngSend(ucMBAddress, ucMBFrame, usLength);
      break;

    case EV_FRAME_SENT:
      break;
    }
  }
  return MB_ENOERR;
}

static uint16_t crc16Sng(uint8_t *pucFrame, uint8_t usLen)
{
  uint16_t crc = 0;
  for (int i = 0; i < usLen; i++)
    crc = crc + pucFrame[i];
  return crc;
}

eMBErrorCode eMbSngReceive(UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength)
{
  eMBErrorCode eStatus = MB_ENOERR;

  ENTER_CRITICAL_SECTION();
  assert(usRcvBufferPos < MB_SNG_SIZE_MAX);

  uint16_t rxCrc = (ucBuf[usRcvBufferPos - 1] << 8) + ucBuf[usRcvBufferPos - 2];
  uint16_t crc = crc16Sng((uint8_t*)(ucBuf + MB_SNG_ADDR_OFF), usRcvBufferPos - MB_SNG_ADDR_OFF - MB_SNG_SIZE_CRC);
  // Проверка длины пакета, стартового байта и CRC
  if ((usRcvBufferPos >= MB_SNG_SIZE_MIN) &&
      (ucBuf[MB_SNG_START_OFF] == 0x00) &&
      (crc == rxCrc)) {
    *pucRcvAddress = bcdToDec(ucBuf[MB_SNG_ADDR_OFF]);
    *pusLength = (USHORT)(usRcvBufferPos - MB_SNG_FUNC_OFF - MB_SNG_SIZE_CRC);
    *pucFrame = (UCHAR*)&ucBuf[MB_SNG_FUNC_OFF];
  }
  else {
    eStatus = MB_EIO;
  }

  EXIT_CRITICAL_SECTION();
  return eStatus;
}

eMBErrorCode eMbSngSend(UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength)
{
  eMBErrorCode eStatus = MB_ENOERR;
  USHORT usCRC16;

  ENTER_CRITICAL_SECTION();

  if (eRcvState == STATE_RX_IDLE) {
    pucSndBufferCur = (UCHAR *)pucFrame - MB_SNG_FUNC_OFF;
    usSndBufferCount = MB_SNG_FUNC_OFF;

    pucSndBufferCur[MB_SNG_START_OFF] = 0x00;
    pucSndBufferCur[MB_SNG_ADDR_OFF] = decToBCD(ucSlaveAddress);
    usSndBufferCount += usLength;

    usCRC16 = crc16Sng((UCHAR *)(pucSndBufferCur + MB_SNG_ADDR_OFF), usSndBufferCount - MB_SNG_ADDR_OFF);
    ucBuf[usSndBufferCount++] = (UCHAR)(usCRC16 & 0xFF);
    ucBuf[usSndBufferCount++] = (UCHAR)(usCRC16 >> 8);

    eSndState = STATE_TX_XMIT;
    vMBPortSerialEnable(FALSE, TRUE);
  }
  else {
    eStatus = MB_EIO;
  }
  EXIT_CRITICAL_SECTION();
  return eStatus;
}

BOOL xMbSngReceiveFSM()
{
  BOOL xTaskNeedSwitch = FALSE;
  UCHAR ucByte;

  assert(eSndState == STATE_TX_IDLE);

  (void)xMBPortSerialGetByte((CHAR *)&ucByte);

  switch (eRcvState) {
  case STATE_RX_INIT:
    vMBPortTimersEnable();
    break;
  case STATE_RX_ERROR:
    vMBPortTimersEnable();
    break;
  case STATE_RX_IDLE:
    usRcvBufferPos = 0;
    ucBuf[usRcvBufferPos++] = ucByte;
    eRcvState = STATE_RX_RCV;
    vMBPortTimersEnable();
    break;
  case STATE_RX_RCV:
    if (usRcvBufferPos < MB_SNG_SIZE_MAX) {
      ucBuf[usRcvBufferPos++] = ucByte;
    }
    else {
      eRcvState = STATE_RX_ERROR;
    }
    vMBPortTimersEnable();
    break;
  }
  return xTaskNeedSwitch;
}

BOOL xMbSngTransmitFSM()
{
  BOOL xNeedPoll = FALSE;

  assert(eRcvState == STATE_RX_IDLE);

  switch (eSndState) {
  case STATE_TX_IDLE:
    // Разрешить приём/запретить передачу
    vMBPortSerialEnable(TRUE, FALSE);
    break;

  case STATE_TX_XMIT:
    // Проверка на завершение передачи
    if (usSndBufferCount != 0) {
      xMBPortSerialPutByte((CHAR)*pucSndBufferCur);
      pucSndBufferCur++;
      usSndBufferCount--;
    }
    else {
      xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );
      vMBPortSerialEnable(TRUE, FALSE);
      eSndState = STATE_TX_IDLE;
    }
    break;
  }

  return xNeedPoll;
}

BOOL xMbSngTimerT35Expired()
{
  BOOL xNeedPoll = FALSE;

  switch (eRcvState) {
  case STATE_RX_INIT:
    xNeedPoll = xMBPortEventPost(EV_READY);
    break;
  case STATE_RX_RCV:
    xNeedPoll = xMBPortEventPost(EV_FRAME_RECEIVED);
    break;
  case STATE_RX_ERROR:
    break;
  default:
    assert((eRcvState == STATE_RX_INIT) || (eRcvState == STATE_RX_RCV) ||
           (eRcvState == STATE_RX_ERROR));
  }

  vMBPortTimersDisable();
  eRcvState = STATE_RX_IDLE;

  return xNeedPoll;
}



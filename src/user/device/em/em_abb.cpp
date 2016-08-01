#include "em_abb.h"
#include "user_main.h"

#include <string.h>

#define ABB_SLAVE_ADDR 1
#define ABB_READ_CMD 0x03

#define PI_DIV_180 0.0174532925199

static void emAbbTask(void *p)
{
  (static_cast<EmAbb*>(p))->task();
}

EmAbb::EmAbb()
  : numberRequest_(0)
  , sizePkt_(0)
{

}

EmAbb::~EmAbb()
{
  closePort();
  osThreadTerminate(threadId_);
}

void EmAbb::init()
{
  openPort(19200, UART_PARITY_EVEN);

  osThreadDef_t t = {"EmAbb", emAbbTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);

  readParameters();
}

void EmAbb::task()
{
  while (1) {
    osDelay(10);

    sendRequest();
    receiveAnswer();
  }
}

bool EmAbb::isConnect()
{
  bool curConnect = Em::isConnect();

  if (prevConnect_ && !curConnect) {
    for (int i = 0; i < countParameters_; ++i) {
      uint16_t id = getFieldId(i);
      if (id != EM_COEFFICIENT_TRANS_CURRENT) {
        float value = NAN;
        setValue(id, value);
      }
    }
  }
  prevConnect_ = curConnect;

  return curConnect;
}

void EmAbb::sendRequest()
{
  int sizePktTx = 0;
  uint16_t addr = 0;
  uint16_t count = 0;

  switch (numberRequest_)
  {
  // Запрос чтения коэффициента трансформации
  case AbbRequestCoefTransform:
    if (parameters.get(CCS_EM_TYPE) == EM_TYPE_ABB) {
      addr = 0x8C00;
      count = 2;
    }
    // A44
    else {
      addr = 0x8C04;
      count = 8;
    }
    break;
  // Запрос чтения активной энергии
  case AbbRequestEnergyAct:
    addr = 0x5000;
    count = 8;
    break;
  // Запрос чтения реактивной энергии
  case AbbRequestEnergyReact:
    addr = 0x500C;
    count = 8;
    break;
  // Запрос чтения напряжений, токов, мощностей, частоты, фаз
  case AbbRequestData:
    addr = 0x5B00;
    count = 52;
    break;
  // Запрос чтения коэффициентов мощности
  case AbbRequestPowerFactor:
    addr = 0x5B3A;
    count = 4;
    break;
  }

  txBuffer_[0] = ABB_SLAVE_ADDR;
  txBuffer_[1] = ABB_READ_CMD;
  txBuffer_[2] = addr >> 8;
  txBuffer_[3] = addr & 0x00FF;
  txBuffer_[4] = count >> 8;
  txBuffer_[5] = count & 0x00FF;
  sizePktTx = 8;
  sizePkt_ = count*2 + 5;

  uint16_t calcCrc = crc16_ibm(txBuffer_, sizePktTx-2);
  txBuffer_[sizePktTx - 2] = calcCrc & 0x00FF;
  txBuffer_[sizePktTx - 1] = calcCrc >> 8;

  sendUart(txBuffer_, sizePktTx);
  counters_.transmite++;
}

void EmAbb::receiveAnswer()
{
  memset(rxBuffer_, 0, sizeof(rxBuffer_));
  int size = receiveUart(rxBuffer_);
  if (size < sizePkt_) {
    counters_.lost++;
    counters_.failure++;
    return;
  }
  sizePkt_ = size;

  uint16_t rxCrc = (rxBuffer_[sizePkt_ - 1] << 8) + rxBuffer_[sizePkt_ - 2];
  uint16_t calcCrc = crc16_ibm(rxBuffer_, sizePkt_ - 2);

  if (rxCrc != calcCrc) {
    counters_.crc++;
    counters_.failure++;
    return;
  }
  if ((rxBuffer_[0] != txBuffer_[0]) || (rxBuffer_[1] != txBuffer_[1])) {
    counters_.error++;
    counters_.failure++;
    return;
  }

  unTypeData value;
  value.uint32_t = 0;

  switch (numberRequest_)
  {
  // Ответ на запрос чтения коэффициента трансформации
  case AbbRequestCoefTransform:
    if (parameters.get(CCS_EM_TYPE) == EM_TYPE_ABB) {
      value.uint16_t[0] = (rxBuffer_[3] << 8) + rxBuffer_[4];
      coefTransI_ = value.uint16_t[0];
      value.uint16_t[0] = (rxBuffer_[5] << 8) + rxBuffer_[6];
      coefTransU_ = value.uint16_t[0];
    }
    // A44
    else {
      uint32_t numI, numU, denomI, denomU;
      numI = (rxBuffer_[3] << 24) | (rxBuffer_[4] << 16) | (rxBuffer_[5] << 8) | rxBuffer_[6];
      numU = (rxBuffer_[7] << 24) | (rxBuffer_[8] << 16) | (rxBuffer_[9] << 8) | rxBuffer_[10];
      denomI = (rxBuffer_[11] << 24) | (rxBuffer_[12] << 16) | (rxBuffer_[13] << 8) | rxBuffer_[14];
      denomU = (rxBuffer_[15] << 24) | (rxBuffer_[16] << 16) | (rxBuffer_[17] << 8) | rxBuffer_[18];
      if (denomI > 0) {
        coefTransI_ = numI/denomI;
        if (coefTransI_ < 1)
          coefTransI_ = 1;
      } else {
        coefTransI_ = 1;
      }
      if (denomU > 0) {
        coefTransU_ = numU/denomU;
        if (coefTransU_ < 1)
          coefTransU_ = 1;
      } else {
        coefTransU_ = 1;
      }
    }

    coefTransI_ = 1;
    coefTransU_ = 1;
    break;
  // Ответ на запрос чтения активной энергии
  case AbbRequestEnergyAct:
    value.float_t = getUint64(&rxBuffer_[3], 0.01)/coefTransI_/coefTransU_;
    setNewValue(EM_ACTIVE_PLUS_ALL, value.float_t);
    value.float_t = getUint64(&rxBuffer_[11], 0.01)/coefTransI_/coefTransU_;
    setNewValue(EM_ACTIVE_MINUS_ALL, value.float_t);
    break;
  // Ответ на запрос чтения реактивной энергии
  case AbbRequestEnergyReact:
    value.float_t = getUint64(&rxBuffer_[3], 0.01)/coefTransI_/coefTransU_;
    setNewValue(EM_REACTIVE_PLUS_ALL, value.float_t);
    value.float_t = getUint64(&rxBuffer_[11], 0.01)/coefTransI_/coefTransU_;
    setNewValue(EM_REACTIVE_MINUS_ALL, value.float_t);
    break;
  // Ответ на запрос чтения напряжений, токов, мощностей, частоты, фаз
  case AbbRequestData:
    {
      float uA = getUint32(&rxBuffer_[3], 0.1);
      setNewValue(EM_VOLTAGE_PHASE_1, uA);
      float uB = getUint32(&rxBuffer_[7], 0.1);
      setNewValue(EM_VOLTAGE_PHASE_2, uB);
      float uC = getUint32(&rxBuffer_[11], 0.1);
      setNewValue(EM_VOLTAGE_PHASE_3, uC);

      if (parameters.get(CCS_EM_TYPE) == EM_TYPE_ABB_A44) {
        value.float_t = getUint32(&rxBuffer_[15], 0.1)/coefTransU_;
        setNewValue(EM_VOLTAGE_PHASE_1_2, value.float_t);
        value.float_t = getUint32(&rxBuffer_[19], 0.1)/coefTransU_;
        setNewValue(EM_VOLTAGE_PHASE_2_3, value.float_t);
        value.float_t = getUint32(&rxBuffer_[23], 0.1)/coefTransU_;
        setNewValue(EM_VOLTAGE_PHASE_3_1, value.float_t);
      } else {
        float uA2 = uA*uA;
        float uB2 = uB*uB;
        float uC2 = uC*uC;

        float fiA = getInt16(&rxBuffer_[101], 0.1);
        float fiB = getInt16(&rxBuffer_[103], 0.1);
        float fiC = getInt16(&rxBuffer_[105], 0.1);

        float fiAb = angleDiff(fiA, fiB);
        float fiBc = angleDiff(fiB, fiC);
        float fiCa = angleDiff(fiC, fiA);

        float f = uA2 + uB2 - (2.0 * uA * uB * cos(fiAb * PI_DIV_180));
        if (f < 0.0)
          f = 0.0;
        value.float_t = sqrt(f);
        setNewValue(EM_VOLTAGE_PHASE_1_2, value.float_t);

        f = uB2 + uC2 - (2.0 * uB * uC * cos(fiBc * PI_DIV_180));
        if (f < 0.0)
          f = 0.0;
        value.float_t = sqrt(f);
        setNewValue(EM_VOLTAGE_PHASE_2_3, value.float_t);

        f = uC2 + uA2 - (2.0 * uB * uA * cos(fiCa * PI_DIV_180));
        if (f < 0.0)
          f = 0.0;
        value.float_t = sqrt(f);
        setNewValue(EM_VOLTAGE_PHASE_3_1, value.float_t);
      }

      value.float_t = getUint32(&rxBuffer_[27], 0.01)/coefTransI_;
      setNewValue(EM_CURRENT_PHASE_1, value.float_t);
      value.float_t = getUint32(&rxBuffer_[31], 0.01)/coefTransI_;
      setNewValue(EM_CURRENT_PHASE_2, value.float_t);
      value.float_t = getUint32(&rxBuffer_[35], 0.01)/coefTransI_;
      setNewValue(EM_CURRENT_PHASE_3, value.float_t);

      value.float_t = getInt32(&rxBuffer_[43], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_ACTIVE_POWER, value.float_t);
      value.float_t = getInt32(&rxBuffer_[47], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_ACTIVE_POWER_PHASE_1, value.float_t);
      value.float_t = getInt32(&rxBuffer_[51], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_ACTIVE_POWER_PHASE_2, value.float_t);
      value.float_t = getInt32(&rxBuffer_[55], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_ACTIVE_POWER_PHASE_3, value.float_t);


      value.float_t = getInt32(&rxBuffer_[59], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_REACTIVE_POWER, value.float_t);
      value.float_t = getInt32(&rxBuffer_[63], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_REACTIVE_POWER_PHASE_1, value.float_t);
      value.float_t = getInt32(&rxBuffer_[67], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_REACTIVE_POWER_PHASE_2, value.float_t);
      value.float_t = getInt32(&rxBuffer_[71], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_REACTIVE_POWER_PHASE_3, value.float_t);

      value.float_t = getInt32(&rxBuffer_[75], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_FULL_POWER, value.float_t);
      value.float_t = getInt32(&rxBuffer_[79], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_FULL_POWER_PHASE_1, value.float_t);
      value.float_t = getInt32(&rxBuffer_[83], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_FULL_POWER_PHASE_2, value.float_t);
      value.float_t = getInt32(&rxBuffer_[87], 0.01)/coefTransI_/coefTransU_;
      setNewValue(EM_FULL_POWER_PHASE_3, value.float_t);

      value.float_t = getUint16(&rxBuffer_[91], 0.01);
      setNewValue(EM_FREQUENCY, value.float_t);
    }
    break;
  // Ответ на запрос чтения коэффициентов мощности
  case AbbRequestPowerFactor:
    value.float_t = getInt16(&rxBuffer_[3], 0.001);
    setNewValue(EM_COS_PHI, value.float_t);
    value.float_t = getInt16(&rxBuffer_[5], 0.001);
    setNewValue(EM_COS_PHI_PHASE_1, value.float_t);
    value.float_t = getInt16(&rxBuffer_[7], 0.001);
    setNewValue(EM_COS_PHI_PHASE_2, value.float_t);
    value.float_t = getInt16(&rxBuffer_[9], 0.001);
    setNewValue(EM_COS_PHI_PHASE_3, value.float_t);
    break;
  }
  numberRequest_++;
  if (numberRequest_ > AbbRequestPowerFactor)
    numberRequest_ = AbbRequestCoefTransform;

  counters_.resive++;
  counters_.failure = 0;
}

float EmAbb::getUint64(uint8_t *buf, float coef)
{
  if ((buf[0] == 0xFF) && (buf[1] == 0xFF) && (buf[2] == 0xFF) && (buf[3] == 0xFF) &&
      (buf[4] == 0xFF) && (buf[5] == 0xFF) && (buf[6] == 0xFF) && (buf[7] == 0xFF))
    return 0;
  uint64_t value = ((uint64_t)buf[0] << 56) | ((uint64_t)buf[1] << 48) |
      ((uint64_t)buf[2] << 40) | ((uint64_t)buf[3] << 32) |
      (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
  return ((double)value * (double)coef);
}

float EmAbb::getUint32(uint8_t *buf, float coef)
{
  if ((buf[0] == 0xFF) && (buf[1] == 0xFF) && (buf[2] == 0xFF) && (buf[3] == 0xFF))
    return 0;
  uint32_t value = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  return ((float)value * coef);
}

float EmAbb::getInt32(uint8_t *buf, float coef)
{
  if ((buf[0] == 0xFF) && (buf[1] == 0xFF) && (buf[2] == 0xFF) && (buf[3] == 0xFF))
    return 0;
  uint32_t value = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  int value1 = value;
  return ((float)value1 * coef);
}

float EmAbb::getUint16(uint8_t *buf, float coef)
{
  if ((buf[0] == 0xFF) && (buf[1] == 0xFF))
    return 0;
  uint16_t value = (buf[0] << 8) | buf[1];
  return ((float)value * coef);
}

float EmAbb::getInt16(uint8_t *buf, float coef)
{
  if ((buf[0] == 0xFF) && (buf[1] == 0xFF))
    return 0;
  uint16_t value = (buf[0] << 8) | buf[1];
  short value1 = value;
  return ((float)value1 * coef);
}

float EmAbb::angleDiff(float a, float b)
{
  float value = a - b;
  if (value < 0.0)
    value = -value;
  if (value > 180.0)
    value = 360.0 - value;
  if (value < 0.0)
    value = 0.0;
  else if (value > 359.0)
    value = 359.0;
  return value;
}

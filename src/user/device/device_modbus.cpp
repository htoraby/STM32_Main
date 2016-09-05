/*
 * device_modbus.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device_modbus.h"
#include "user_main.h"
#include "stdio.h"

static void deviceModbusTask(void *p)
{
  (static_cast<DeviceModbus*>(p))->exchangeTask();
}

DeviceModbus::DeviceModbus(ModbusParameter *modbusParameters,
                           int countParameter,
                           int numPort,
                           long baudRate,
                           int dataBits,
                           int stopBits,
                           int parity,
                           int address)
  : mbParams_(modbusParameters)
  , countParameter_(countParameter)
  , numPort_(numPort)
  , devAdrs_(address)
  , indexExchange_(1)
  , prevConnect(true)
{
  // Создаём экземпляр класса ModbusMasterSerial
  mms_ = new ModbusMasterSerial();
  // Открываем порт
  mms_->openProtocol(numPort_, baudRate, dataBits, stopBits, parity);
}

DeviceModbus::~DeviceModbus()
{
  osThreadTerminate(threadId_);
  osMessageDelete(messageOutOfTurn_);
  mms_->closeProtocol(numPort_);
  delete mms_;
}

void DeviceModbus::createThread(const char *threadName, osMessageQId getValueDeviceQId)
{
  getValueDeviceQId_ = getValueDeviceQId;

  // Создаём очередь сообщений
  osMessageQDef(OutOfTurn, 300, uint32_t);
  messageOutOfTurn_ = osMessageCreate (osMessageQ(OutOfTurn), NULL);

  // Создаём задачу цикла опроса
  osThreadDef_t t = {threadName, deviceModbusTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void DeviceModbus::setAddrIndexReg(uint16_t addr)
{
  addrIndexReg_ = addr;
}

int DeviceModbus::getFieldID(int index)
{
  return mbParams_[index].id;
}

int DeviceModbus::getFieldAddress(int index)
{
  return mbParams_[index].address;
}

int DeviceModbus::getFieldOperation(int index)
{
  return mbParams_[index].operation;
}

int DeviceModbus::getFieldPhysic(int index)
{
  return mbParams_[index].physic;
}

int DeviceModbus::getFieldUnit(int index)
{
  return mbParams_[index].unit;
}

int DeviceModbus::getFieldValidity(int index)
{
  return mbParams_[index].validity;
}

int DeviceModbus::setFieldValidity(int index, int validity)
{
  mbParams_[index].validity = validity;
  return ok_r;
}

int DeviceModbus::getFieldTypeData(int index)
{
  return mbParams_[index].typeData;
}

int DeviceModbus::getFieldIndex(int index)
{
  return mbParams_[index].index;
}

int DeviceModbus::getFieldFreqExchange(int index)
{
  return mbParams_[index].freqExchange;
}

int DeviceModbus::getFieldCntExchange(int index)
{
  return mbParams_[index].cntExchange;
}

float DeviceModbus::getFieldCoefficient(int index)
{
  return mbParams_[index].coefficient;
}

float DeviceModbus::getFieldMinimum(int index)
{
  return mbParams_[index].min;
}

float DeviceModbus::getFieldMaximum(int index)
{
  return mbParams_[index].max;
}

float DeviceModbus::getFieldDefault(int index)
{
  return mbParams_[index].def;
}

int DeviceModbus::getFieldCommand(int index)
{
  return mbParams_[index].command;
}

int DeviceModbus::setFieldCommand(int index, int command)
{
  mbParams_[index].command = command;
  return mbParams_[index].command;
}

unTypeData DeviceModbus::getFieldValue(int index)
{
  return mbParams_[index].value;
}

ModbusParameter* DeviceModbus::getFieldAll(int index)
{
  return &mbParams_[index];
}

int DeviceModbus::getIndexAtId(int id)
{
  for (int i = 0; i < countParameter_; i++) {
    if (getFieldID(i) == id) {
      return i;
    }
  }
  return 0;
}

int DeviceModbus::getIndexAtAddress(int address, int typeData, uint8_t index)
{
  for (int i = 0; i < countParameter_; i++) {
    if (getFieldAddress(i) == address)
      if (getFieldTypeData(i) == typeData)
        if (getFieldIndex(i) == index)
          return i;
  }
  return 0;
}

int DeviceModbus::getMessageOutOfTurn()
{
  osEvent event = osMessageGet(messageOutOfTurn_, 0);
  if (event.status == osEventMessage)
    return event.value.v;
  return 0;
}

int DeviceModbus::putMessageOutOfTurn(int element)
{
  osStatus status = osMessagePut(messageOutOfTurn_, element, 0);
  if (status)
    return 1;
  else
    return 0;
}

int DeviceModbus::putMessageUpdateId(int id)
{
  osStatus status = osMessagePut(getValueDeviceQId_, id, 0);
  if (status == osOK)
    return 1;
  else
    return 0;
}

void DeviceModbus::writeModbusParameter(int id, float value)
{
  int index = getIndexAtId(id);
  if (index) {
    // Получаем всю структуру параметра
    ModbusParameter *param = getFieldAll(index);
    // Применяем преобразование единиц измерения
    value = (value * (units[param->physic][param->unit][0])) + (units[param->physic][param->unit][1]);
    // Применяем преобразование коэффициента
    value = value / param->coefficient;
    // Применяем тип данных
    switch (param->typeData) {
    case  TYPE_DATA_CHAR:
      param->value.char_t[0] = (char)(value + 0.5);
      break;
    case TYPE_DATA_INT16:
    case TYPE_DATA_ARRAY_INT16:
      param->value.int16_t[0] = (short int)(value + 0.5);
      break;
    case TYPE_DATA_INT32:
    case TYPE_DATA_ARRAY_INT32:
      param->value.int32_t = (int)(value + 0.5);
      break;
    case TYPE_DATA_UINT16:
      param->value.uint16_t[0] = (unsigned short int)(value + 0.5);
      break;
    case  TYPE_DATA_UINT32:
      param->value.uint32_t = (unsigned int)(value + 0.5);
      break;
    case TYPE_DATA_FLOAT:
      param->value.float_t = value;
      break;
    case TYPE_DATA_COIL:
      if (value != 0) {
         param->value.int16_t[0] = 255;
      }
      else {
        param->value.int16_t[0] = 0;
      }
    default:
      break;
    }
    param->command = OPERATION_WRITE;
    putMessageOutOfTurn(index);
  }
}

void DeviceModbus::readModbusParameter(int id)
{
  int index = getIndexAtId(id);
  if (index) {
    ModbusParameter *param = getFieldAll(index);
    param->command = OPERATION_READ;
    putMessageOutOfTurn(index);
  }
}

int DeviceModbus::searchExchangeParameters()
{
  for (int i = indexExchange_; i < countParameter_; i++) {
    if (mbParams_[i].freqExchange > 0) {
      // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
      if (mbParams_[i].cntExchange < mbParams_[i].freqExchange) {
        mbParams_[i].cntExchange++;
      }
      else {
        mbParams_[i].cntExchange = 0;
        indexExchange_ = i+1;
        return i;
      }
    }
  }
  for (int i = 1; i < indexExchange_; i++) {
    if (mbParams_[i].freqExchange > 0) {
      // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
      if (mbParams_[i].cntExchange < mbParams_[i].freqExchange) {
        mbParams_[i].cntExchange++;
      }
      else {
        mbParams_[i].cntExchange = 0;
        indexExchange_ = i+1;
        return i;
      }
    }
  }
  return 0;
}

bool DeviceModbus::isConnect()
{
  bool curConnect = mms_->isConnect();

  if (prevConnect && !curConnect) {
    logDebug.add(WarningMsg, "DeviceModbus: Lost connect port %d device %d",
                 numPort_, devAdrs_);
  }

  if (!prevConnect && curConnect) {
    for (int indexModbus = 0; indexModbus < countParameter_; indexModbus++) {
      mbParams_[indexModbus].cntExchange = mbParams_[indexModbus].freqExchange;
    }
    logDebug.add(WarningMsg, "DeviceModbus: Restored connect port %d device %d",
                 numPort_, devAdrs_);
  }

  prevConnect = curConnect;
  return curConnect;
}

void DeviceModbus::exchangeTask()
{
  uint16_t count = 1;
  while (1) {
    osDelay(1);
    // Проверяем очередь параметров для обработки вне очереди
    int outOfTurn = getMessageOutOfTurn();  // Проверка если параметр вне очереди
    if (outOfTurn && (mbParams_[outOfTurn].command == OPERATION_WRITE)) {
      mbParams_[outOfTurn].validity = err_r;
      switch (mbParams_[outOfTurn].typeData) {                // Тип данных
      case TYPE_DATA_COIL:
        writeCoil(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].value.int16_t[0]);
        break;
      case TYPE_DATA_INT16:
        writeInt16Register(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].value.int16_t[0]);
        break;
      case TYPE_DATA_UINT16:
        writeUint16Register(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].value.uint16_t[0]);
        break;
      case TYPE_DATA_INT32:
        writeInt32Register(devAdrs_, mbParams_[outOfTurn].address, &mbParams_[outOfTurn].value.int32_t, count);
        break;
      case TYPE_DATA_UINT32:
        writeUint32Register(devAdrs_, mbParams_[outOfTurn].address, &mbParams_[outOfTurn].value.uint32_t, count);
        break;
      case TYPE_DATA_FLOAT:
        writeFloatRegister(devAdrs_ ,mbParams_[outOfTurn].address, &mbParams_[outOfTurn].value.float_t, count);
        break;
      case TYPE_DATA_ARRAY_INT16:
        writeArrayInt16Register(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].index, mbParams_[outOfTurn].value.int16_t[0]);
        break;
      case TYPE_DATA_ARRAY_INT32:
         writeArrayInt32Register(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].index, &mbParams_[outOfTurn].value.int32_t, count);
        break;
      default:
        break;
      }
// NOTE: Т.к. ПЧ Новомет, работает не по modbus, и меняет значение регистров
// через некоторое время после получения команды на запись, а отвечает корректным
// ответом сразу. Для него в случае записи параметра по modbus, проверяем если параметр
// читается достаточно редко, настраиваем счётчик циклов чтения параметра, на частота опроса - 10 циклов.
      if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_NOVOMET) {   // Тип
        if (mbParams_[outOfTurn].freqExchange > 20/*VERY_OFTEN*/) {
          mbParams_[outOfTurn].cntExchange = mbParams_[outOfTurn].freqExchange - 20/*VERY_OFTEN*/;
        }
      }
      else {
        mbParams_[outOfTurn].command = OPERATION_READ;
        putMessageOutOfTurn(outOfTurn);
      }
    }
    else {
      if (!(outOfTurn && (mbParams_[outOfTurn].command == OPERATION_READ))) {
        outOfTurn = searchExchangeParameters();
      }
      if (outOfTurn) {
        mbParams_[outOfTurn].validity = err_r;
        switch (mbParams_[outOfTurn].typeData) {
        case TYPE_DATA_COIL:
          readCoils(devAdrs_, mbParams_[outOfTurn].address, bitArr_, count);
          break;
        case TYPE_DATA_INT16:
          readInt16Registers(devAdrs_, mbParams_[outOfTurn].address, uint16Arr_, count);
          break;
        case TYPE_DATA_INT16_4:
          readInt16InputRegisters(devAdrs_, mbParams_[outOfTurn].address, uint16Arr_, count);
          break;
        case TYPE_DATA_UINT16:
          readUint16Registers(devAdrs_, mbParams_[outOfTurn].address, uint16Arr_, count);
          break;
        case TYPE_DATA_INT32:
          readInt32Registers(devAdrs_, mbParams_[outOfTurn].address, uint32Arr_, count);
          break;
        case TYPE_DATA_UINT32:
          readUint32Registers(devAdrs_, mbParams_[outOfTurn].address, uint32Arr_, count);
          break;
        case TYPE_DATA_FLOAT:
          readFloatsRegisters(devAdrs_, mbParams_[outOfTurn].address, floatArr_, count);
          break;
        case TYPE_DATA_ARRAY_INT16:
          readArrayInt16Registers(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].index, uint16Arr_, count);
          break;
        case TYPE_DATA_ARRAY_INT32:
          readArrayInt32Registers(devAdrs_, mbParams_[outOfTurn].address, mbParams_[outOfTurn].index, uint32Arr_, count);
          break;
        case TYPE_DATA_STR:
          readStrRegisters(devAdrs_, mbParams_[outOfTurn].address, uint16Arr_, mbParams_[outOfTurn].index);
          break;
        default:
          break;
        }
        mbParams_[outOfTurn].command = OPERATION_ERROR;
      }
    }
  }
}

int DeviceModbus::getDeviceAddress()
{
  return devAdrs_;
}

void DeviceModbus::setDeviceAddress(int address)
{
  devAdrs_ = address;
}

void DeviceModbus::readCoils(uint8_t slaveAddr, uint16_t startRef, bool *bitArr, uint16_t refCnt)
{
  uint8_t res = mms_->readCoils(slaveAddr, startRef, bitArr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_COIL);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.int16_t[0] = bitArr_[i];
      uint8_t validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity coils, port:%d slaveAddr:%d command:0x01 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_COIL);
    for (uint8_t i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read coils, port:%d slaveAddr:%d command:0x01 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readInt16Registers(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleRegisters(slaveAddr, startRef, regArr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT16);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.int16_t[0] = regArr[i];
      uint8_t validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {       
        logDebug.add(WarningMsg, "DeviceModbus. Error validity int16 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT16);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read int16 register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readInt16InputRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  uint8_t res = mms_->readInputRegisters(slaveAddr, startRef, regArr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT16_4);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.int16_t[0] = regArr[i];
      uint8_t validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity int16 register, port:%d slaveAddr:%d command:0x04 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT16_4);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read int16 register, port:%d slaveAddr:%d command:0x04 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readUint16Registers(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleRegisters(slaveAddr, startRef, regArr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_UINT16);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.uint16_t[0] = regArr[i];
      uint8_t validity = checkRange(mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity uint16 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;

      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_UINT16);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read uint16 register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readInt32Registers(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleLongInts(slaveAddr, startRef, int32Arr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT32);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.int32_t = int32Arr[i];
      uint8_t validity = checkRange(mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity int32 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;

      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_INT32);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read int32 register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readUint32Registers(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleLongInts(slaveAddr, startRef, int32Arr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_UINT32);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.uint32_t = int32Arr[i];
      uint8_t validity = checkRange(mbParams_[index].value.uint32_t, mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity uint32 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.uint32_t, mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_UINT32);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read uint32 register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readFloatsRegisters(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleFloats(slaveAddr, startRef, float32Arr, refCnt);
  if (res == ok_r) {
    int index = getIndexAtAddress(startRef, TYPE_DATA_FLOAT);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.float_t = float32Arr[i];
      uint8_t validity = checkRange(mbParams_[index].value.float_t, mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity float register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%f min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.float_t, mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;

      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    int index = getIndexAtAddress(startRef, TYPE_DATA_FLOAT);
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read float register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::readArrayInt16Registers(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, uint16_t *regArr, uint16_t refCnt)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, indexArray);
  if (res == ok_r) {
    res = mms_->readMultipleRegisters(slaveAddr, startRef, regArr, refCnt);
    if (res == ok_r) {
      int index = getIndexAtAddress(startRef, TYPE_DATA_ARRAY_INT16, indexArray);
      for (int i = 0; i < refCnt; i++) {
        mbParams_[index].value.int16_t[0] = regArr[i];
        uint8_t validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
        if ((validity != ok_r) && (validity != mbParams_[index].validity)) {       
          logDebug.add(WarningMsg, "DeviceModbus. Error validity array int16 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d index:%d",
                       numPort_, slaveAddr, startRef, mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity, indexArray);
        }
        mbParams_[index].validity = validity;
        putMessageUpdateId(mbParams_[index].id);
        index++;
      }
    }
    else {
      int index = getIndexAtAddress(startRef, TYPE_DATA_ARRAY_INT16, indexArray);
      for (int i = 0; i < refCnt; i++) {
        mbParams_[index].validity = err_r;
        if (isConnect()) {
          logDebug.add(WarningMsg, "DeviceModbus. Error read array int16 register, port:%d slaveAddr:%d command:0x03 startRef:%d index:%d",
                       numPort_, slaveAddr, startRef, indexArray);
        }
        putMessageUpdateId(mbParams_[index].id);
        index++;
      }
    }
    res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, 0);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "DeviceModbus. Error mbCmd0x06 Reset Index Array");
  }
  else {
    if (isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Set Index Array %d", indexArray);
  }
}

void DeviceModbus::readArrayInt32Registers(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, uint32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, indexArray);
  if (res == ok_r) {
    uint8_t res = mms_->readMultipleLongInts(slaveAddr, startRef, int32Arr, refCnt);
    if (res == ok_r) {
      int index = getIndexAtAddress(startRef, TYPE_DATA_ARRAY_INT32, indexArray);
      for (int i = 0; i < refCnt; i++) {
        mbParams_[index].value.int32_t = int32Arr[i];
        uint8_t validity = checkRange(mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, true);
        if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
          logDebug.add(WarningMsg, "DeviceModbus. Error validity array int32 register, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d index:%d",
                       numPort_, slaveAddr, startRef,mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity, indexArray);
        }
        mbParams_[index].validity = validity;
        putMessageUpdateId(mbParams_[index].id);
        index++;
      }
    }
    else {
      int index = getIndexAtAddress(startRef, TYPE_DATA_ARRAY_INT32, indexArray);
      for (int i = 0; i < refCnt; i++) {
        mbParams_[index].validity = err_r;
        if (isConnect()) {
          logDebug.add(WarningMsg, "DeviceModbus. Error read array int32 register, port:%d slaveAddr:%d command:0x03 startRef:%d index:%d",
                       numPort_, slaveAddr, startRef, indexArray);
        }
        putMessageUpdateId(mbParams_[index].id);
        index++;
      }
    }
    res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, 0);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Reset Index Array");
  }
  else {
    if (isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Set Index Array %d", indexArray);
  }
}

void DeviceModbus::readStrRegisters(uint8_t slaveAddr, uint16_t startRef, uint16_t *regArr, uint16_t refCnt)
{
  uint8_t res = mms_->readMultipleRegisters(slaveAddr, startRef, regArr, refCnt);
  int index = getIndexAtAddress(startRef, TYPE_DATA_STR, refCnt);
  if (res == ok_r) {
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].value.uint16_t[0] = regArr[i];
      uint8_t validity = checkRange(mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, true);
      if ((validity != ok_r) && (validity != mbParams_[index].validity)) {
        logDebug.add(WarningMsg, "DeviceModbus. Error validity str, port:%d slaveAddr:%d command:0x03 startRef:%d value:%d min:%f max:%f validity:%d",
                     numPort_, slaveAddr, startRef, mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, mbParams_[index].validity);
      }
      mbParams_[index].validity = validity;

      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
  else {
    for (int i = 0; i < refCnt; i++) {
      mbParams_[index].validity = err_r;
      if (isConnect()) {
        logDebug.add(WarningMsg, "DeviceModbus. Error read str register, port:%d slaveAddr:%d command:0x03 startRef:%d",
                     numPort_, slaveAddr, startRef);
      }
      putMessageUpdateId(mbParams_[index].id);
      index++;
    }
  }
}

void DeviceModbus::writeCoil(uint8_t slaveAddr, int bitAddr, int bitVal)
{
  uint8_t res = mms_->writeCoil(slaveAddr, bitAddr, bitVal);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write coil, port:%d slaveAddr:%d command:0x05 bitAddr:%d bitVal:%d",
                 numPort_, slaveAddr, bitAddr, bitVal);
}

void DeviceModbus::writeInt16Register(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, regAddr, regVal);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write int16 register, port:%d slaveAddr:%d command:0x06 regAddr:%d regVal:%d",
                 numPort_, slaveAddr, regAddr, regVal);
}

void DeviceModbus::writeUint16Register(uint8_t slaveAddr, uint16_t regAddr, uint16_t regVal)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, regAddr, regVal);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write uint16 register, port:%d slaveAddr:%d command:0x06 regAddr:%d regVal:%d",
                 numPort_, slaveAddr, regAddr, regVal);
}

void DeviceModbus::writeInt32Register(uint8_t slaveAddr, uint16_t startRef, int32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->writeMultipleLongInts(slaveAddr, startRef, (uint32_t*)int32Arr, 1);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write int32 register, port:%d slaveAddr:%d command:0x10 startRef:%d int32Arr:%d",
                 numPort_, slaveAddr, startRef, int32Arr[0]);
}

void DeviceModbus::writeUint32Register(uint8_t slaveAddr, uint16_t startRef, uint32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->writeMultipleLongInts(slaveAddr, startRef, int32Arr, 1);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write uint32 register, port:%d slaveAddr:%d command:0x10 startRef:%d int32Arr:%d",
                 numPort_, slaveAddr, startRef, (uint32_t)int32Arr[0]);
}

void DeviceModbus::writeFloatRegister(uint8_t slaveAddr, uint16_t startRef, float *float32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->writeMultipleFloats(slaveAddr, startRef, float32Arr, 1);
  if ((res != ok_r) && isConnect())
    logDebug.add(WarningMsg, "DeviceModbus. Error write float register, port:%d slaveAddr:%d command:0x10 startRef:%d float32Arr:%f",
                 numPort_, slaveAddr, startRef, float32Arr[0]);
}

void DeviceModbus::writeArrayInt16Register(uint8_t slaveAddr, uint16_t regAddr, uint8_t indexArray, uint16_t regVal)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, indexArray);
  if (res == ok_r) {
    res = mms_->writeSingleRegister(slaveAddr, regAddr, regVal);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "DeviceModbus. Error write array int16 register, port:%d slaveAddr:%d command:0x06 regAddr:%d regVal:%d index:%d",
                   numPort_, slaveAddr, regAddr, regVal, indexArray);
    res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, 0);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Reset Index Array");
  }
  else {
    if (isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Set Index Array %d", indexArray);
  }
}

void DeviceModbus::writeArrayInt32Register(uint8_t slaveAddr, uint16_t startRef, uint8_t indexArray, int32_t *int32Arr, uint16_t refCnt)
{
  uint8_t res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, indexArray);
  if (res == ok_r) {
    res = mms_->writeMultipleLongInts(slaveAddr, startRef, (uint32_t*)int32Arr, 1);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "DeviceModbus. Error write array int32 register, port:%d slaveAddr:%d command:0x06 startRef:%d int32Arr:%d index:%d",
                   numPort_, slaveAddr, startRef, int32Arr[0], indexArray);
    res = mms_->writeSingleRegister(slaveAddr, addrIndexReg_, 0);
    if ((res != ok_r) && isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Reset Index Array");
  }
  else {
    if (isConnect())
      logDebug.add(WarningMsg, "mbCmd0x06 Set Index Array %d", indexArray);
  }
}

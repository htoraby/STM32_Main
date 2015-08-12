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
  osMessageQDef(OutOfTurn, 100, uint32_t);
  messageOutOfTurn_ = osMessageCreate (osMessageQ(OutOfTurn), NULL);

  // Создаём задачу цикла опроса
  osThreadDef_t t = {threadName, deviceModbusTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
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

int DeviceModbus::getFieldTypeData(int index)
{
  return mbParams_[index].typeData;
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

int DeviceModbus::getIndexAtAddress(int address)
{
  for (int i = 0; i < countParameter_; i++) {
    if (getFieldAddress(i) == address) {
      return i;
    }
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
  case  TYPE_DATA_INT16:
    param->value.int16_t[0] = (short int)(value + 0.5);
    break;
  case  TYPE_DATA_INT32:
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
  default:
    break;
  }
  param->command = OPERATION_WRITE;
  putMessageOutOfTurn(index);
}

void DeviceModbus::readModbusParameter(int id)
{
  int index = getIndexAtId(id);
  ModbusParameter *param = getFieldAll(index);
  param->command = OPERATION_READ;
  putMessageOutOfTurn(index);
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
    logDebug.add(WarningMsg, "Connect lost port: %d, device: %d", numPort_, devAdrs_);
  }

  if (!prevConnect && curConnect) {
    for (int indexModbus = 0; indexModbus < countParameter_; indexModbus++) {
      mbParams_[indexModbus].cntExchange = mbParams_[indexModbus].freqExchange;
    }
    logDebug.add(WarningMsg, "Connect restored port: %d, device: %d", numPort_, devAdrs_);
  }

  prevConnect = curConnect;
  return curConnect;
}

void DeviceModbus::exchangeTask()
{
  int count = 0;
  uint16_t res = 1;
  while (1) {
    osDelay(1);

    // Проверяем очередь параметров для обработки вне очереди
    int outOfTurn = getMessageOutOfTurn();  // Проверка если параметр вне очереди
    if (outOfTurn) {                        // Если есть параметр вне очереди
      if (mbParams_[outOfTurn].command == OPERATION_WRITE) {    // Команда записи
        int address = mbParams_[outOfTurn].address;             // Получаем адрес параметра в устройстве
        switch (mbParams_[outOfTurn].typeData) {                // Тип данных
        case TYPE_DATA_INT16:
          res = mms_->writeSingleRegister(devAdrs_, address, mbParams_[outOfTurn].value.int16_t[0]);
          if (res != ok_r) {
            if (isConnect()) {
              logDebug.add(WarningMsg, "mbCmd0x06 int16 %d %d %d", devAdrs_, address,
                           mbParams_[outOfTurn].value.int16_t[0]);
            }
          }
          break;
        case TYPE_DATA_UINT16:
          res = mms_->writeSingleRegister(devAdrs_, address, mbParams_[outOfTurn].value.uint16_t[0]);
          if (res != ok_r) {
            if (isConnect()) {
              logDebug.add(WarningMsg, "mbCmd0x06 uint16 %d %d %d", devAdrs_, address,
                           mbParams_[outOfTurn].value.uint16_t[0]);
            }
          }
          break;
        case  TYPE_DATA_INT32:
          int32Arr_[0] = mbParams_[outOfTurn].value.int32_t;
          res = mms_->writeMultipleLongInts(devAdrs_, address, int32Arr_, 1);
          if (res != ok_r) {
            if (isConnect()) {
              logDebug.add(WarningMsg, "mbCmd0x10 int32 %d %d %d", devAdrs_, address, int32Arr_[0]);
            }
          }
          break;
        case  TYPE_DATA_UINT32:
          int32Arr_[0] =  mbParams_[outOfTurn].value.uint32_t;
          res = mms_->writeMultipleLongInts(devAdrs_, address, int32Arr_, 1);
          if (res != ok_r) {
            if (isConnect()) {
              logDebug.add(WarningMsg, "mbCmd0x10 uint32 %d %d %d", devAdrs_, address, int32Arr_[0]);
            }
          }
          break;
        case  TYPE_DATA_FLOAT:
          float32Arr_[0] = mbParams_[outOfTurn].value.float_t;
          res = mms_->writeMultipleFloats(devAdrs_ ,address, float32Arr_, 1);
          if (res != ok_r) {
            if (isConnect()) {
              logDebug.add(WarningMsg, "mbCmd0x10 float %d %d %f", devAdrs_, address, float32Arr_[0]);
            }
          }
          break;
        default:
          break;
        }
      }
      else {                                // Чтение вне очереди
        if (mbParams_[outOfTurn].command == OPERATION_READ) {
          int address = mbParams_[outOfTurn].address;
          switch (mbParams_[outOfTurn].typeData) {
          case TYPE_DATA_INT16:
            count = 1;                      // Пока только одного параметра
            res = mms_->readMultipleRegisters(devAdrs_, address, regArr_, count);
            if (res == ok_r) {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].value.int16_t[0] = regArr_[i];
                mbParams_[index].validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
                if (mbParams_[index].validity != ok_r) {
                  logDebug.add(WarningMsg, "0x03 ok_r no valid devAdr %d, index %d, value %d, valid %d", devAdrs_,
                               index, regArr_[i], mbParams_[index].validity);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            else {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].validity = err_r;
                if (isConnect()) {
                  logDebug.add(WarningMsg, "0x03 no ok_r devAdr %d, index %d, value %d, valid %d", devAdrs_, index, regArr_[i]);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            break;
          case TYPE_DATA_UINT16:
            count = 1;                      // Пока только одного параметра
            res = mms_->readMultipleRegisters(devAdrs_, address, regArr_, count);
            if (res == ok_r) {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].value.uint16_t[0] = regArr_[i];
                mbParams_[index].validity = checkRange(mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, true);
                if (mbParams_[index].validity != ok_r) {
                  logDebug.add(WarningMsg, "mbCmd0x03 uint16 %d %d %d %d", devAdrs_,
                               index, regArr_[i], mbParams_[index].validity);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            else {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].validity = err_r;
                if (isConnect()) {
                  logDebug.add(WarningMsg, "mbCmd0x03 uint16 %d %d %d", devAdrs_, index, regArr_[i]);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            break;
          case TYPE_DATA_INT32:
            count = 1;
            res = mms_->readMultipleLongInts(devAdrs_, address, int32Arr_, count);
            if (res == ok_r) {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].value.int32_t = int32Arr_[i];
                mbParams_[index].validity = checkRange(mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, true);
                if (mbParams_[index].validity != ok_r) {
                  logDebug.add(WarningMsg, "mbCmd0x03 int32 %d %d %d %d", devAdrs_,
                               index, int32Arr_[i], mbParams_[index].validity);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            else {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].validity = err_r;
                if (isConnect()) {
                  logDebug.add(WarningMsg, "mbCmd0x03 int32 %d %d %d", devAdrs_, index, int32Arr_[i]);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            break;
          case TYPE_DATA_UINT32:
            count = 1;
            res = mms_->readMultipleLongInts(devAdrs_, address, int32Arr_, count);
            if (res == ok_r) {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].value.uint32_t = int32Arr_[i];
                mbParams_[index].validity = checkRange(mbParams_[index].value.uint32_t, mbParams_[index].min, mbParams_[index].max, true);
                if (mbParams_[index].validity != ok_r) {
                  logDebug.add(WarningMsg, "mbCmd0x03 uint32 %d %d %d %d", devAdrs_, index, int32Arr_[i], mbParams_[index].validity);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            else {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].validity = err_r;
                if (isConnect()) {
                  logDebug.add(WarningMsg, "mbCmd0x03 uint32 %d %d %d", devAdrs_, index, int32Arr_[i]);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            break;
            break;
          case TYPE_DATA_FLOAT:
            count = 1;
            res = mms_->readMultipleFloats(devAdrs_, address, float32Arr_, count);
            if (res == ok_r) {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].value.float_t = float32Arr_[i];
                mbParams_[index].validity = checkRange(mbParams_[index].value.float_t, mbParams_[index].min, mbParams_[index].max, true);
                if (mbParams_[index].validity != ok_r) {
                  logDebug.add(WarningMsg, "mbCmd0x03 float %d %d %f %d", devAdrs_,
                               index, float32Arr_[i], mbParams_[index].validity);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            else {
              int index = getIndexAtAddress(address);
              for (int i = 0; i < count; i++) {
                mbParams_[index].validity = err_r;
                if (isConnect()) {
                  logDebug.add(WarningMsg, "mbCmd0x03 float %d %d %f", devAdrs_,
                               index, float32Arr_[i]);
                }
                putMessageUpdateId(mbParams_[index].id);
                index++;
              }
            }
            break;
          }
        }
      }
      mbParams_[outOfTurn].command = OPERATION_ERROR;
    }
    else {
      outOfTurn = searchExchangeParameters();
      if (outOfTurn) {
        int address = mbParams_[outOfTurn].address;
        switch (mbParams_[outOfTurn].typeData) {
        case TYPE_DATA_INT16:
          count = 1;                      // Пока только одного параметра
          res = mms_->readMultipleRegisters(devAdrs_, address, regArr_, count);
          if (res == ok_r) {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].value.int16_t[0] = regArr_[i];
              mbParams_[index].validity = checkRange(mbParams_[index].value.int16_t[0], mbParams_[index].min, mbParams_[index].max, true);
              if (mbParams_[index].validity != ok_r) {
                logDebug.add(WarningMsg, "0x03 res %d devAdr %d, index %d, value %d, valid %d", res, devAdrs_,
                             index, regArr_[i], mbParams_[index].validity);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          else {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].validity = err_r;
              if (isConnect()) {
                logDebug.add(WarningMsg, "0x03 res %d, devAdr %d, index %d, value %d, valid %d", res, devAdrs_, index, regArr_[i]);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          break;
        case TYPE_DATA_UINT16:
          count = 1;                      // Пока только одного параметра
          res = mms_->readMultipleRegisters(devAdrs_, address, regArr_, count);
          if (res == ok_r) {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].value.uint16_t[0] = regArr_[i];
              mbParams_[index].validity = checkRange(mbParams_[index].value.uint16_t[0], mbParams_[index].min, mbParams_[index].max, true);
              if (mbParams_[index].validity != ok_r) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_,
                             index, regArr_[i], mbParams_[index].validity);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          else {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].validity = err_r;
              if (isConnect()) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_, index, regArr_[i]);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          break;
        case TYPE_DATA_INT32:
          count = 1;
          res = mms_->readMultipleLongInts(devAdrs_, address, int32Arr_, count);
          if (res == ok_r) {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].value.int32_t = int32Arr_[i];
              mbParams_[index].validity = checkRange(mbParams_[index].value.int32_t, mbParams_[index].min, mbParams_[index].max, true);
              if (mbParams_[index].validity != ok_r) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_,
                             index, int32Arr_[i], mbParams_[index].validity);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          else {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].validity = err_r;
              if (isConnect()) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_, index, int32Arr_[i]);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          break;
        case TYPE_DATA_UINT32:
          count = 1;
          res = mms_->readMultipleLongInts(devAdrs_, address, int32Arr_, count);
          if (res == ok_r) {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].value.uint32_t = int32Arr_[i];
              mbParams_[index].validity = checkRange(mbParams_[index].value.uint32_t, mbParams_[index].min, mbParams_[index].max, true);
              if (mbParams_[index].validity != ok_r) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_,
                             index, int32Arr_[i], mbParams_[index].validity);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          else {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].validity = err_r;
              if (isConnect()) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %d, valid %d", devAdrs_, index, int32Arr_[i]);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          break;
          break;
        case TYPE_DATA_FLOAT:
          count = 1;
          res = mms_->readMultipleFloats(devAdrs_, address, float32Arr_, count);
          if (res == ok_r) {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].value.float_t = float32Arr_[i];
              mbParams_[index].validity = checkRange(mbParams_[index].value.float_t, mbParams_[index].min, mbParams_[index].max, true);
              if (mbParams_[index].validity != ok_r) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %f, valid %d", devAdrs_,
                             index, float32Arr_[i], mbParams_[index].validity);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          else {
            int index = getIndexAtAddress(address);
            for (int i = 0; i < count; i++) {
              mbParams_[index].validity = err_r;
              if (isConnect()) {
                logDebug.add(WarningMsg, "0x03 devAdr %d, index %d, value %f, valid %d", devAdrs_, index, float32Arr_[i]);
              }
              putMessageUpdateId(mbParams_[index].id);
              index++;
            }
          }
          break;
        }
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

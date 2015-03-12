/*
 * device_modbus.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device_modbus.h"



static void deviceModbusTask(void *p)
{
  (static_cast<DeviceModbus*>(p))->exchangeTask();
}

DeviceModbus::DeviceModbus(ModbusParameter *modbusParameters,
                           int countParameter,
                           int portName,
                           long baudRate,
                           int dataBits,
                           int stopBits,
                           int parity,
                           int address)
  : modbusParameters_(modbusParameters)
  , countParameter_(countParameter)
  , deviceAddress_(address)
  , indexExchange_(1)
{
  // Создаём экземпляр класса ModbusMasterSerial
  mms_ = new ModbusMasterSerial(portName);
  // Открываем порт
  mms_->openProtocol(portName, baudRate, dataBits, stopBits, parity);
}

DeviceModbus::~DeviceModbus()
{
  osThreadTerminate(threadId_);
}

void DeviceModbus::createThread(const char *threadName, osMessageQId getValueDeviceQId)
{
  getValueDeviceQId_ = getValueDeviceQId;

  // Создаём очередь сообщений
  osMessageQDef(OutOfTurn, 100, uint32_t);
  messageOutOfTurn_ = osMessageCreate (osMessageQ(OutOfTurn), NULL);

  // Создаём задачу цикла опроса
  osThreadDef_t t = {threadName, deviceModbusTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

int DeviceModbus::getFieldID(int index)
{
  return modbusParameters_[index].id;
}

int DeviceModbus::getFieldAddress(int index)
{
  return modbusParameters_[index].address;
}

int DeviceModbus::getFieldOperation(int index)
{
  return modbusParameters_[index].operation;
}

int DeviceModbus::getFieldPhysic(int index)
{
  return modbusParameters_[index].physic;
}

int DeviceModbus::getFieldUnit(int index)
{
  return modbusParameters_[index].unit;
}

int DeviceModbus::getFieldValidity(int index)
{
  return modbusParameters_[index].validity;
}

int DeviceModbus::getFieldTypeData(int index)
{
  return modbusParameters_[index].typeData;
}

int DeviceModbus::getFieldFreqExchange(int index)
{
  return modbusParameters_[index].freqExchange;
}

int DeviceModbus::getFieldCntExchange(int index)
{
  return modbusParameters_[index].cntExchange;
}

float DeviceModbus::getFieldCoefficient(int index)
{
  return modbusParameters_[index].coefficient;
}

float DeviceModbus::getFieldMinimum(int index)
{
  return modbusParameters_[index].min;
}

float DeviceModbus::getFieldMaximum(int index)
{
  return modbusParameters_[index].max;
}

float DeviceModbus::getFieldDefault(int index)
{
  return modbusParameters_[index].def;
}

int DeviceModbus::getFieldCommand(int index)
{
  return modbusParameters_[index].command;
}

int DeviceModbus::setFieldCommand(int index, int command)
{
  modbusParameters_[index].command = command;
  return modbusParameters_[index].command;
}

unTypeData DeviceModbus::getFieldValue(int index)
{
  return modbusParameters_[index].value;
}

ModbusParameter* DeviceModbus::getFieldAll(int index)
{
  return &modbusParameters_[index];
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
  value = (value * (Units[param->physic][param->unit][0])) + (Units[param->physic][param->unit][1]);
  // Применяем преобразование коэффициента
  value = value / param->coefficient;
  // Применяем тип данных
  switch (param->typeData) {
    case  TYPE_DATA_CHAR:
      param->value.tdChar[0] = (char)(value + 0.5);
      break;
    case  TYPE_DATA_INT16:
      param->value.tdInt16[0] = (short int)(value + 0.5);
      break;
    case  TYPE_DATA_INT32:
      param->value.tdInt32 = (int)(value + 0.5);
      break;
    case TYPE_DATA_UINT16:
      param->value.tdUint16[0] = (unsigned short int)(value + 0.5);
      break;
    case  TYPE_DATA_UINT32:
      param->value.tdUint32 = (unsigned int)(value + 0.5);
      break;
    case TYPE_DATA_FLOAT:
      param->value.tdFloat = value;
      break;
    default:
      break;
  }
  param->command = OPERATION_WRITE;
  putMessageOutOfTurn(index);
}

int DeviceModbus::searchExchangeParameters()
{
  for (int i = indexExchange_; i < countParameter_; i++) {
    if (modbusParameters_[i].freqExchange > 0) {
      // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
      if (modbusParameters_[i].cntExchange < modbusParameters_[i].freqExchange) {
        modbusParameters_[i].cntExchange++;
      }
      else {
        modbusParameters_[i].cntExchange = 0;
        indexExchange_ = i+1;
        return i;
      }
    }
  }
  for (int i = 1; i < indexExchange_; i++) {
    if (modbusParameters_[i].freqExchange > 0) {
      // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
      if (modbusParameters_[i].cntExchange < modbusParameters_[i].freqExchange) {
        modbusParameters_[i].cntExchange++;
      }
      else {
        modbusParameters_[i].cntExchange = 0;
        indexExchange_ = i+1;
        return i;
      }
    }
  }
  return 0;
}

void DeviceModbus::exchangeTask()
{
  int count = 0;

  while (1) {
    osDelay(1);
    // Проверяем очередь параметров для обработки вне очереди
    int outOfTurn = getMessageOutOfTurn();
    // Если есть параметры для обработки вне очереди
    if (outOfTurn) {
      // Если записать
      if (modbusParameters_[outOfTurn].command == OPERATION_WRITE) {
        int address = modbusParameters_[outOfTurn].address;
        switch (modbusParameters_[outOfTurn].typeData) {
          case TYPE_DATA_INT16:
            mms_->writeSingleRegister(deviceAddress_,
                                      address,
                                      modbusParameters_[outOfTurn].value.tdInt16[0]);
            break;
          case TYPE_DATA_UINT16:
            mms_->writeSingleRegister(deviceAddress_,
                                      address,
                                      modbusParameters_[outOfTurn].value.tdUint16[0]);
            break;
          case  TYPE_DATA_INT32:
            int32Arr_[0] = modbusParameters_[outOfTurn].value.tdInt32;
            mms_->writeMultipleLongInts(deviceAddress_,
                                        address,
                                        int32Arr_,
                                        1);
            break;
          case  TYPE_DATA_UINT32:
            int32Arr_[0] =  modbusParameters_[outOfTurn].value.tdUint32;
            mms_->writeMultipleLongInts(deviceAddress_,
                                        modbusParameters_[outOfTurn].address,
                                        int32Arr_,
                                        1);
            break;
          case  TYPE_DATA_FLOAT:
            float32Arr_[0] = modbusParameters_[outOfTurn].value.tdFloat;
            mms_->writeMultipleFloats(deviceAddress_,
                                      modbusParameters_[outOfTurn].address,
                                      float32Arr_,
                                      1);
            break;
          default:
            break;
        }
      }
      else {// TODO: Чтение вне очереди
        if (modbusParameters_[outOfTurn].command == OPERATION_READ) {
          int address = modbusParameters_[outOfTurn].address;
          if(!(mms_->readMultipleRegisters(deviceAddress_,address,regArr_,1))) {
            int Index = getIndexAtAddress(address);
            modbusParameters_[Index].value.tdInt16[0] = regArr_[0];
            modbusParameters_[Index].validity = VALIDITY_GOOD;
            putMessageUpdateId(modbusParameters_[Index].id);
          }
          else {
            int Index = getIndexAtAddress(address);
            modbusParameters_[Index].validity = VALIDITY_ERROR;
          }
        }
      }
      modbusParameters_[outOfTurn].command = OPERATION_ERROR;
    }
    else {
      outOfTurn = searchExchangeParameters();
      if (outOfTurn) {
        int address = modbusParameters_[outOfTurn].address;
        switch (modbusParameters_[outOfTurn].typeData) {
          case TYPE_DATA_INT16:
            count = 1;
            if (!(mms_->readMultipleRegisters(deviceAddress_,address,regArr_,count))) {
              // TODO: Сделать проверки на минимум максиму и т.п
              // Получаем индекс элемента в массиве с которого начинаем сохранение
              int Index = getIndexAtAddress(address);
              // Цикл по количеству полученных регистров
              for (int i = 0; i < count; i++) {
                modbusParameters_[Index].value.tdInt16[0] = regArr_[i];
                modbusParameters_[Index].validity = VALIDITY_GOOD;
                putMessageUpdateId(modbusParameters_[Index].id);
                Index++;
              }
              if (outOfTurn == 15)
                asm("nop");
            }
            else {
              int index = getIndexAtAddress(address);
              // Цикл по количеству полученных регистров
              for (int i = 0; i < count; i++) {
                modbusParameters_[index].validity = VALIDITY_ERROR;
                index++;
              }
            }
            break;
          case TYPE_DATA_UINT16:
            mms_->readMultipleRegisters(deviceAddress_,
                                        address,
                                        regArr_,
                                        1);
            break;
          case  TYPE_DATA_INT32:
            mms_->readMultipleLongInts(deviceAddress_,
                                       address,
                                       int32Arr_,
                                       1);
            break;
          case  TYPE_DATA_UINT32:
            mms_->readMultipleLongInts(deviceAddress_,
                                       address,
                                       int32Arr_,
                                       1);
            break;
          case  TYPE_DATA_FLOAT:
            mms_->readMultipleFloats(deviceAddress_,
                                     address,
                                     float32Arr_,
                                     1);
            break;
          default:
            break;
        }
      }
    }
  }
}

int DeviceModbus::getDeviceAddress()
{
  return deviceAddress_;
}

void DeviceModbus::setDeviceAddress(int address)
{
  deviceAddress_ = address;
}

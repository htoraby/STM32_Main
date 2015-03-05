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

// Конструктор класса DeviceModbus
DeviceModbus::DeviceModbus(ModbusParameter *MapRegisters,
                           int Quantity,
                           int PortName,
                           long BaudRate,
                           int DataBits,
                           int StopBits,
                           int Parity,
                           int Address,
                           const char *threadName,
                           osMessageQId messageUpdateID)
  : quantityParam_(Quantity)
  , deviceAddress_(Address)
  , indexExchange_(1)
  , messageUpdateID_(messageUpdateID)
  , modbusParameters_(MapRegisters)
{
  // Создаём очередь сообщений
  osMessageQDef(OutOfTurn, 100, uint32_t);
  messageOutOfTurn_ = osMessageCreate (osMessageQ(OutOfTurn), NULL);

  // Создаём экземпляр класса ModbusMasterSerial
  mms_ = new ModbusMasterSerial(PortName);
  // Открываем порт
  mms_->openProtocol(PortName, BaudRate, DataBits, StopBits, Parity);

  // Создаём задачу цикла опроса
  // Заполняем структуру с параметрами задачи
  osThreadDef_t t = {threadName, deviceModbusTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  // Создаём задачу
  threadId_ = osThreadCreate(&t, this);
}

DeviceModbus::~DeviceModbus()
{
  osThreadTerminate(threadId_);
}

// ----------------------------------------------------------------------------
// Методы для работы с полями массива структур Modbus параметров (картой)
// через индекс параметра в массиве
// ----------------------------------------------------------------------------

// Получить ID из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldID(int Index)
{
  return modbusParameters_[Index].ID;
}

// Получить Address из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldAddress(int Index)
{
  return modbusParameters_[Index].Address;
}

// Получить Operation из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldOperation(int Index)
{
  return modbusParameters_[Index].Operation;
}

// Получить Physic из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldPhysic(int Index)
{
  return modbusParameters_[Index].Physic;
}

// Получить Unit из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldUnit(int Index)
{
  return modbusParameters_[Index].Unit;
}

// Получить Validity из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldValidity(int Index)
{
  return modbusParameters_[Index].Validity;
}

// Получить TypeData из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldTypeData(int Index)
{
  return modbusParameters_[Index].TypeData;
}

// Получить FreqExchange из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldFreqExchange(int Index)
{
  return modbusParameters_[Index].FreqExchange;
}

// Получить CntExchange из структуры ModbusParameter из массива по индексу
int DeviceModbus::getFieldCntExchange(int Index)
{
  return modbusParameters_[Index].CntExchange;
}

// Получить Coefficient из структуры ModbusParameter из массива по индексу
float DeviceModbus::getFieldCoefficient(int Index)
{
  return modbusParameters_[Index].Coefficient;
}

// Получить Minimum из структуры ModbusParameter из массива по индексу
float DeviceModbus::getFieldMinimum(int Index)
{
  return modbusParameters_[Index].Minimum;
}

// Получить Maximum из структуры ModbusParameter из массива по индексу
float DeviceModbus::getFieldMaximum(int Index)
{
  return modbusParameters_[Index].Maximum;
}

float DeviceModbus::getFieldDefault(int Index)
{
  return modbusParameters_[Index].Default;
}

int DeviceModbus::getFieldCommand(int Index)
{
  return modbusParameters_[Index].Command;
}

int DeviceModbus::setFieldCommand(int Index, int Command)
{
  modbusParameters_[Index].Command = Command;
  return modbusParameters_[Index].Command;
}

// Получить Value из структуры ModbusParameter из массива по индексу
unTypeData DeviceModbus::getFieldValue(int Index)
{
  return modbusParameters_[Index].Value;
}

// Получить всю структуру параметра
ModbusParameter* DeviceModbus::getFieldAll(int Index)
{
  return &modbusParameters_[Index];
}

// Метод поиска и получения индекса параметра с указанным ID
int DeviceModbus::getIndexAtID(int ID)
{
  int Quantity = getQuantityParam();
  for (int I = 0; I < Quantity; I++) {
    if (getFieldID(I) == ID) {
      return I;
    }
  }
  return 0;
}

// Метод поиска и получения индекса параметра с указанным Адресу
int DeviceModbus::getIndexAtAddress(int Address)
{
  int Quantity = getQuantityParam();
  for (int I = 0; I < Quantity; I++) {
    if (getFieldAddress(I) == Address) {
      return I;
    }
  }
  return 0;
}

// Получить элемент очереди внеочередной
int DeviceModbus::getMessageOutOfTurn()
{
  osEvent Event;
  Event = osMessageGet(messageOutOfTurn_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

// Добавить элемент в очередь
int DeviceModbus::putMessageOutOfTurn(int Element)
{
  osStatus Status;
  Status = osMessagePut(messageOutOfTurn_, Element, 0);
  if (Status)
    return 1;
  else
    return 0;
}

/*
// Получить элемент очереди готовых параметров
int DeviceModbus::getMessageReadyParam()
{
  osEvent Event;
  Event = osMessageGet(messageReadyParam_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}
*/

// Добавить параметр в очередь готовых параметров
int DeviceModbus::putMessageUpdateID(int id)
{
  osStatus status = osMessagePut(messageUpdateID_, id, 0);
  if (status == osOK)
    return 1;
  else
    return 0;
}

// Метод записи параметра
void DeviceModbus::writeModbusParameter(int ID, float Value)
{
  int Index = getIndexAtID(ID);
  // Получаем всю структуру параметра
  ModbusParameter *Param = getFieldAll(Index);
  // Применяем преобразование единиц измерения
  Value = (Value * (Units[Param->Physic][Param->Unit][0])) + (Units[Param->Physic][Param->Unit][1]);
  // Применяем преобразование коэффициента
  Value = Value * Param->Coefficient;
  // Применяем тип данных
  switch (Param->TypeData) {
    case  TYPE_DATA_CHAR:
      Param->Value.tdChar[0] = (char)(Value + 0.5);
      break;
    case  TYPE_DATA_INT16:
      Param->Value.tdInt16[0] = (short int)(Value + 0.5);
      break;
    case  TYPE_DATA_INT32:
      Param->Value.tdInt32 = (int)(Value + 0.5);
      break;
    case TYPE_DATA_UINT16:
      Param->Value.tdUint16[0] = (unsigned short int)(Value + 0.5);
      break;
    case  TYPE_DATA_UINT32:
      Param->Value.tdUint32 = (unsigned int)(Value + 0.5);
      break;
    case TYPE_DATA_FLOAT:
      Param->Value.tdFloat = Value;
      break;
    default:
      break;
  }
  Param->Command = OPERATION_WRITE;
  putMessageOutOfTurn(Index);
}

int DeviceModbus::searchExchangeParameters()
{
  for (int i = indexExchange_; i < quantityParam_; i++) {
    // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
    if (modbusParameters_[i].CntExchange < modbusParameters_[i].FreqExchange) {
      modbusParameters_[i].CntExchange++;
    }
    else {
      modbusParameters_[i].CntExchange = 0;
      indexExchange_ = i+1;
      return indexExchange_;
    }
  }
  for (int i = 1; i < indexExchange_; i++) {
    // Если счётчик циклов опроса параметра не достиг уставки частоты опроса
    if (modbusParameters_[i].CntExchange < modbusParameters_[i].FreqExchange) {
      modbusParameters_[i].CntExchange++;
    }
    else {
      modbusParameters_[i].CntExchange = 0;
      indexExchange_ = i+1;
      return i;
    }
  }
  return 0;
}

// Метод цикл по карте регистров для чтения и записи параметров
void DeviceModbus::exchangeTask(void)
{
  int Count = 0;

  while (1) {
    osDelay(1);

    // Проверяем очередь параметров для обработки вне очереди
    int outOfTurn = getMessageOutOfTurn();
    // Если есть параметры для обработки вне очереди
    if (outOfTurn) {
      // Если записать
      if (modbusParameters_[outOfTurn].Command == OPERATION_WRITE) {
        int address = modbusParameters_[outOfTurn].Address;
        switch (modbusParameters_[outOfTurn].TypeData) {
          case TYPE_DATA_INT16:
            mms_->writeSingleRegister(deviceAddress_,
                                      address,
                                      modbusParameters_[outOfTurn].Value.tdInt16[0]);
            break;
          case TYPE_DATA_UINT16:
            mms_->writeSingleRegister(deviceAddress_,
                                      address,
                                      modbusParameters_[outOfTurn].Value.tdUint16[0]);
            break;
          case  TYPE_DATA_INT32:
            int32Arr_[0] = modbusParameters_[outOfTurn].Value.tdInt32;
            mms_->writeMultipleLongInts(deviceAddress_,
                                        address,
                                        int32Arr_,
                                        1);
            break;
          case  TYPE_DATA_UINT32:
            int32Arr_[0] =  modbusParameters_[outOfTurn].Value.tdUint32;
            mms_->writeMultipleLongInts(deviceAddress_,
                                        modbusParameters_[outOfTurn].Address,
                                        int32Arr_,
                                        1);
            break;
          case  TYPE_DATA_FLOAT:
            float32Arr_[0] = modbusParameters_[outOfTurn].Value.tdFloat;
            mms_->writeMultipleFloats(deviceAddress_,
                                      modbusParameters_[outOfTurn].Address,
                                      float32Arr_,
                                      1);
            break;
          default:
            break;
        }
      }
      else {// TODO: Чтение вне очереди
        if (modbusParameters_[outOfTurn].Command == OPERATION_READ) {
          int address = modbusParameters_[outOfTurn].Address;
          if(!(mms_->readMultipleRegisters(deviceAddress_,address,regArr_,1))) {
            int Index = getIndexAtAddress(address);
            modbusParameters_[Index].Value.tdInt16[0] = regArr_[0];
            modbusParameters_[Index].Validity = VALIDITY_GOOD;
            putMessageUpdateID(modbusParameters_[Index].ID);
          }
          else {
            int Index = getIndexAtAddress(address);
            modbusParameters_[Index].Validity = VALIDITY_ERROR;
          }
        }
      }
      modbusParameters_[outOfTurn].Command = OPERATION_ERROR;
    }
    else {
      outOfTurn = searchExchangeParameters();
      if (outOfTurn) {
        int address = modbusParameters_[outOfTurn].Address;
        switch (modbusParameters_[outOfTurn].TypeData) {
          case TYPE_DATA_INT16:
            Count = 1;
            if (!(mms_->readMultipleRegisters(deviceAddress_,address,regArr_,Count))) {
              // TODO: Сделать проверки на минимум максиму и т.п
              // Получаем индекс элемента в массиве с которого начинаем сохранение
              int Index = getIndexAtAddress(address);
              // Цикл по количеству полученных регистров
              for (int i = 0; i < Count; i++) {
                modbusParameters_[Index].Value.tdInt16[0] = regArr_[i];
                modbusParameters_[Index].Validity = VALIDITY_GOOD;
                putMessageUpdateID(modbusParameters_[Index].ID);
                Index++;
              }
            }
            else {
              int Index = getIndexAtAddress(address);
              // Цикл по количеству полученных регистров
              for (int i = 0; i < Count; i++) {
                modbusParameters_[Index].Validity = VALIDITY_ERROR;
                Index++;
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
      //else
      // ВНИМАНИЕ!!! Сломался exchangeCycle
    }
  }
}



// Метод чтения количества регистров в карте регистров
int DeviceModbus::getQuantityParam()
{
  return quantityParam_;
}

// Метод записи количества регистров в карте регистров
void DeviceModbus::setQuantityParam(int Quantity)
{
  quantityParam_ = Quantity;
}

// Метод чтения адреса устройства
int DeviceModbus::getDeviceAddress()
{
  return deviceAddress_;
}

// Метод записи количества регистров в карте регистров
void DeviceModbus::setDeviceAddress(int Address)
{
  deviceAddress_ = Address;
}










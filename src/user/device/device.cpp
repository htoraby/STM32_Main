/*
 * classDevice.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device.h"
#include "service.h"

// Конструктор класса
Device::Device()
{
  // TODO Auto-generated constructor stub
}

// Деструктор класса
Device::~Device()
{
  // TODO Auto-generated destructor stub
}

// PUBLIC МЕТОДЫ КЛАССА

// ЧТЕНИЕ И ЗАПИСЬ ПАРАМЕТРОВ ЦЕЛИКОМ И ИХ ЗНАЧЕНИЙ C ПРОВЕРКАМИ
// Функция получения всей структуры параметра с проверками
int Device::getCheckParameter(int ID, Parameter &Param)
{
  // ВНИМАНИЕ
  // Тут должна быть куча проверок каждого поля струтуры

  // Вызываем функцию чтения полей записи по ID
  Param = getParameter(ID);
  return RETURN_OK;
}

// Функция получения текущего значения параметра с проверками
int Device::getCheckValue(int ID, float &Value)
{
  // ВНИМАНИЕ
  // Сюда надо добавить проверки и условия

  // Вызываем функцию чтения значения по ID
  Value = getValue(ID);
  return RETURN_OK;
}

// Функция получения уровня доступа к параметру с проверкой валидности
// полученного значения
int Device::getCheckAccess(int ID, int &Access)
{
  // Получаем уровень доступа параметра по ID
  Access = getAccess(ID);
  // Проверяем на валидность
  if (checkRange(Access, ACCESS_ERROR, ACCESS_LAST, 0))
    return 0;
  else
    return Access;
}

// Функция получения операций над параметром с проверкой валидности
// полученного значения
int Device::getCheckOperation(int ID)
{
  int Operation = getOperation(ID);
  if (checkRange(Operation, OPERATION_ERROR, OPERATION_LAST, 0))
    return 0;
  else
    return Operation;
}

// Функция получения физической величины с проверкой валидности
// полученного значения
int Device::getCheckPhysic(int ID)
{
  int Physic = getPhysic(ID);
  if (checkRange(Physic, PHYSIC_ERROR, PHYSIC_LAST, 0))
    return 0;
  else
    return Physic;
}

// Функция получения валидности параметра с проверкой валидности
// полученного значения
int Device::getCheckValidity(int ID)
{
  int Validity = getValidity(ID);
  if (checkRange(Validity, VALIDITY_ERROR, VALIDITY_LAST, 0))
    return 0;
  else
    return Validity;
}

// Функция получения нового значения параметра с проверкой валидности
// полученного значения
int Device::getCheckUpdate(int ID)
{
  int Update = getUpdate(ID);
  if (checkRange(Update, UPDATE_ERROR, UPDATE_LAST, 1))
    return 0;
  else
    return Update;
}

// ЗАПИСЬ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ С ПРОВЕРКАМИ
// Функция записи полей параметра с проверками
int Device::setCheckParameter(int ID, Parameter Param)
{
  // ВНИМАНИЕ
  // Сюда надо добавить проверки и условия

  // Вызываем функцию записи полей по ID
  setParameter(ID, Param);
  return RETURN_OK;
}

// Функция записи текущего значения параметра с проверками
int Device::setCheckValue(int ID, float Value)
{
  // ВНИМАНИЕ
  // Сюда надо добавить проверки и условия

  // Вызываем функцию записи значения по ID
  setValue(ID, Value);
  return RETURN_OK;
}

// Функция присвоения уровня доступа с проверкой уровня
int Device::setCheckAccess(int ID, int Access)
{
  if (checkRange(Access, ACCESS_ERROR, ACCESS_LAST, 0)) {
    setAccess(ID, Access);
    return RETURN_OK;
  }
  else
    return RETURN_ERROR_ACCESS_VALUE;
}

// Функция присвоения операции с проверкой
int Device::setCheckOperation(int ID, int Operation)
{
  if (checkRange(Operation, OPERATION_ERROR, OPERATION_LAST, 0)) {
    setOperation(ID, Operation);
    return RETURN_OK;
  }
  else
    return RETURN_ERROR_OPERATION_VALUE;
}

// Функция присвоения физической величиный с проверкой
int Device::setCheckPhysic(int ID, int Physic)
{
  if (checkRange(Physic, PHYSIC_ERROR, PHYSIC_LAST, 0)) {
    setPhysic(ID, Physic);
    return RETURN_OK;
  }
  else
    return RETURN_ERROR_PHYSIC_VALUE;
}

// Функция присвоения валидности с проверкой
int Device::setCheckValidity(int ID, int Validity)
{
  if (checkRange(Validity, VALIDITY_ERROR, VALIDITY_LAST, 0)) {
    setValidity(ID, Validity);
    return RETURN_OK;
  }
  else
    return RETURN_ERROR_VALIDITY_VALUE;
}

// Функция присвоения новизны с проверкой
int Device::setCheckUpdate(int ID, int Update)
{
  if (checkRange(Update, UPDATE_ERROR, UPDATE_LAST, 0)) {
    setUpdate(ID, Update);
    return RETURN_OK;
  }
  else
    return RETURN_ERROR_UPDATE_VALUE;
}

// PROTECTED МЕТОДЫ КЛАССА

Parameter Device::getParameter(int ID)
{
  return getParameterIndex(getIndex(ID));
}

// Функция получения значения параметра из массива параметров
double Device::getValue(int ID)
{
  return getValueIndex(getIndex(ID));
}

// Функция получения уровня доступа к параметру
int Device::getAccess(int ID)
{
  return getAccessIndex(getIndex(ID));
}

// Функция получения операций над параметром
int Device::getOperation(int ID)
{
  return getOperationIndex(getIndex(ID));
}

// Функция получения физической величины параметра
int Device::getPhysic(int ID)
{
  return getPhysicIndex(getIndex(ID));
}

// Функция получения валидности параметра
int Device::getValidity(int ID)
{
  return getValidityIndex(getIndex(ID));
}

// Функция получения обновления параметра
int Device::getUpdate(int ID)
{
  return getUpdateIndex(getIndex(ID));
}

// Функция получения минимального значения для параметра
double Device::getMinimum(int ID)
{
  return getMinimumIndex(getIndex(ID));
}

// Функция получения максимального значения для параметра
double Device::getMaximum(int ID)
{
  return getMaximumIndex(getIndex(ID));
}

// Функция получения значения по умолчанию
double Device::getDefault(int ID)
{
  return getDefaultIndex(getIndex(ID));
}

// ЗАПИСЬ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ БЕЗ ПРОВЕРОК
// Функция записи полей параметра
void Device::setParameter(int ID, Parameter Param)
{
  setParameterIndex(getIndex(ID), Param);
}

// Функция присвоения значению параметра
void Device::setValue(int ID, double Value)
{
  setValueIndex(getIndex(ID), Value);
}

// Функция присвоения уровня доступа к параметру
void Device::setAccess(int ID, int Access)
{
  setAccessIndex(getIndex(ID), Access);
}

// Функция присвоения операций над параметром
void Device::setOperation(int ID, int Operation)
{
  setOperationIndex(getIndex(ID), Operation);
}

// Функция присвоения над параметром
void Device::setPhysic(int ID, int Physic)
{
  setPhysicIndex(getIndex(ID), Physic);
}

//Функция присвоения валидности параметра
void Device::setValidity(int ID, int Validity)
{
  setValidityIndex(getIndex(ID), Validity);
}

//Функция присвоения свежести параметра
void Device::setUpdate(int ID, int Update)
{
  setUpdateIndex(getIndex(ID), Update);
}

// Функция присвоения минимального значения параметра
void Device::setMinimum(int ID, double Minimum)
{
  setMinimumIndex(getIndex(ID), Minimum);
}

// Функция присвоения максимального значения параметра
void Device::setMaximum(int ID, double Maximum)
{
  setMaximumIndex(getIndex(ID), Maximum);
}

// Функция присвоения значения по умолчанию
void Device::setDefault(int ID, double Default)
{
  setDefaultIndex(getIndex(ID), Default);
}

// ЧТЕНИЕ ЭЛЕМЕНТА МАССИВА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ
// Функция получения значений полей из структуры Parameter
Parameter Device::getParameterIndex(int Index)
{
  return Parameters[Index];
}

// Функция получения значения поля Value из структуры Parameter
double Device::getValueIndex(int Index)
{
  return Parameters[Index].Value;
}

// Функция получения значения поля ID из структуры Parameter
int Device::getIDIndex(int Index)
{
  return Parameters[Index].ID;
}

// Функция получения значения поля Access из структуры Parameter
int Device::getAccessIndex(int Index)
{
  return Parameters[Index].Access;
}

// Функция получения значения поля Operation из структуры Parameter
int Device::getOperationIndex(int Index)
{
  return Parameters[Index].Operation;
}

// Функция получения значения поля Physic из структуры Parameter
int Device::getPhysicIndex(int Index)
{
  return Parameters[Index].Physic;
}

// Функция получения значения поля Validity из структуры Parameter
int Device::getValidityIndex(int Index)
{
  return Parameters[Index].Validity;
}

// Функция получения значения поля Update из структуры Parameter
int Device::getUpdateIndex(int Index)
{
  return Parameters[Index].Update;
}

// Функция получения значения поля Minimum из структуры Parameter
double Device::getMinimumIndex(int Index)
{
  return Parameters[Index].Minimum;
}

// Функция получения значения поля Maximum из структуры Parameter
double Device::getMaximumIndex(int Index)
{
  return Parameters[Index].Maximum;
}

// Функция получения значения поля Default из структуры Parameter
double Device::getDefaultIndex(int Index)
{
  return Parameters[Index].Default;
}

// ЗАПИСЬ ЭЛЕМЕНТА МАССИВА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ
// Функция присвоения значений полей
void Device::setParameterIndex(int Index, Parameter Param)
{
  Parameters[Index] = Param;
}

// Функция присвоения значения поля Value значению из параметра
void Device::setValueIndex(int Index, double Value)
{
  Parameters[Index].Value = Value;
}

// Функция присвоения значения поля Access значению из параметра
void Device::setAccessIndex(int Index, int Access)
{
  Parameters[Index].Access = Access;
}

// Функция присвоения значения поля Operation значению из параметра
void Device::setOperationIndex(int Index, int Operation)
{
  Parameters[Index].Operation = Operation;
}

// Функция присвоения значения поля Physic значению из параметра
void Device::setPhysicIndex(int Index, int Physic)
{
  Parameters[Index].Physic = Physic;
}

// Функция присвоения значения поля Validity значению из параметра
void Device::setValidityIndex(int Index, int Validity)
{
  Parameters[Index].Validity = Validity;
}

// Функция присвоения значения поля Update значению из параметра
void Device::setUpdateIndex(int Index, int Update)
{
  Parameters[Index].Update = Update;
}

// Функция присвоения значения поля Mimimum значению из параметра
void Device::setMinimumIndex(int Index, double Minimum)
{
  Parameters[Index].Minimum = Minimum;
}

// Функция присвоения значения поля Maximum значению из параметра
void Device::setMaximumIndex(int Index, double Maximum)
{
  Parameters[Index].Maximum = Maximum;
}

// Функция присвоения значения поля Default значению из параметра
void Device::setDefaultIndex(int Index, double Default)
{
  Parameters[Index].Default = Default;
}

// МЕТОДЫ ДЛЯ ПОИСКА ИНДЕКСА ЭЛЕМЕНТА МАССИВА С НУЖНЫМ ID
// Функция поиска в массиве Parameters[] индекса записи с полем ID
int Device::searchIndex(int ID)
{
  int I = 0;
  int Quantity = getNumberElementArray();
  while (I < Quantity) {
    if (ID == getIDIndex(I)) {
      return I;
    }
    I++;
  }
  return 0;
}

// Функция проверки содержится ли в элементе массива с ID - Shift
int Device::checkIndex(int ID)
{
  int Index = 0;
  int Shift = getShiftID();
  Index = ID - Shift;
  if (Parameters[Index].ID == ID)
    return Index;
  else
    return 0;
}

// Функция получения индекса параметра с указанным ID
int Device::getIndex(int ID)
{
  int Index = 0;
  // Проверяем находится ли параметр с указанным ID в предпологаемом элементе
  // массива
  Index = checkIndex(ID);
  // Если находится
  if (Index)
    // Возвращаем индекс
    return Index;
  // Иначе
  else
    // Ищем по массиву такой ID
    Index = searchIndex(ID);
  // Возвращаем
  return Index;
}

// МЕТОДЫ ДЛЯ РАБОТЫ С ЗАКРЫТЫМИ ЧЛЕНАМИ КЛАССА
// Функция получения значения поля ShiftID класса Device
int Device::getShiftID()
{
  return ShiftID;
}

// Функция присвоения значения поля поля ShiftID класса Device
void Device::setShiftID(int Shift)
{
  ShiftID = Shift;
}

// Функция получения значения поля класса Quantity
int Device::getNumberElementArray()
{
  return NumberElementArray;
}

// Функция присвоения значения поля Device NumberElementArray
void Device::setNumberElementArray(int Number)
{
  NumberElementArray = Number;
}

void Device::initParameters()
{
  return;
}

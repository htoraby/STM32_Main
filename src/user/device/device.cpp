/*
 * classDevice.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "Device.h"

/* Конструктор класса */
Device::Device()
{
  // TODO Auto-generated constructor stub
}

/* Деструктор класса */
Device::~Device()
{
  // TODO Auto-generated destructor stub
}

/* Базовые методы работы со структурой Parameter */
/* Функция получения поля Id из структуры */
enID Device::getIdField()
{
  // Защищенная секция

    // Возвращаем значение поля ID
    return Parameter.ID;

};

/* Функция присвоения поля Id в структуре */
enID Device::setIdField(enID ID)
{
  // Защищенная секция
  try {
    // Присваиваем значение
    Parameter.ID = ID;
    // Возвращаем присвоенное значение
    return Parameter.ID;
  }
  // Любая ошибка
  catch(...) {
    // Возвращаем 0
    return 0;
  }
};

/* Функция получения поля Access из структуры */
enAccess Device::getAccessField()
{
  // Защищенная секция
  try {
    // Возвращаем значение поля Access
    return Parameter.Access;
  // Любая ошибка
  }
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция присвоения поля Access в структуре */
enAccess Device::setAccessField(enAccess Access)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Access = Access;
    // Возвращаем присвоенное значение
    return Parameter.Access;
  // Любая ошибка
  catch(...)
    return 0;
};

/* Функция получения поля Operation из структуры */
enOperations Device::getOperationField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Operation
    return Parameter.Operation;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция присвоения поля Operation в структуре */
enOperations Device::setOperationField(enOperations Operation)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Operation = Operation;
    // Возвращаем присвоенное значение
    return Parameter.Operation;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Physic из структуры */
enPhysic getPhysicField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Physic
    return Parameter.Physic;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция присвоения поля Physic в структуре */
enPhysic setPhysicField(enPhysic Physic)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Physic = Physic;
    // Возвращаем присвоенное значение
    return Parameter.Physic;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Validity из структуры */
enValidity getValidityField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Validity
    return Parameter.Validity;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция присвоения поля Validity в структуре */
enValidity setValidityField(enValidity Validity)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Validity = Validity;
    // Возвращаем присвоенное значение
    return Parameter.Validity;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Value из структуры */
double getValueField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Validity
    return Parameter.Value;
  // Любая ошибка
  catch(...)
    return 0;
};

/* Функция присвоения поля Value в структуре */
double setValueField(double Value)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Value = Value;
    // Возвращаем присвоенное значение
    return Parameter.Value;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Minimum из структуры */
double getMinimumField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Minimum
    return Parameter.Minimum;
  // Любая ошибка
  catch(...)
    return 0;
};

/* Функция присвоения поля Minimum в структуре */
double setMinimumField(double Minimum)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Minimum = Minimum;
    // Возвращаем присвоенное значение
    return Parameter.Minimum;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Maximum из структуры */
double getMaximumField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Maximum
    return Parameter.Maximum;
  // Любая ошибка
  catch(...)
    return 0;
};

/* Функция присвоения поля Maximum в структуре */
double setMaximumField(double Maximum)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Maximum = Maximum;
    // Возвращаем присвоенное значение
    return Parameter.Maximum;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};

/* Функция получения поля Default из структуры */
double getDefaultField()
{
  // Защищенная секция
  try
    // Возвращаем значение поля Default
    return Parameter.Default;
  // Любая ошибка
  catch(...)
    return 0;
};

/* Функция присвоения поля Default в структуре */
double setDefaultField(double Default)
{
  // Защищенная секция
  try
    // Присваиваем значение
    Parameter.Default = Default;
    // Возвращаем присвоенное значение
    return Parameter.Default;
  // Любая ошибка
  catch(...)
    // Возвращаем 0
    return 0;
};



/* Функция получения проверенного значения параметра */
int Device::getCheckedValueParameter(int ID, float &Value)
{
  int Result = 1;
  // Защищённая секция
  return Result;
};




// МЕТОД ЗАПИСИ ЗНАЧЕНИЯ ПАРАМЕТРА
// Метод записи значения параметра без проверки уровня доступа
// Параметры:
// ID - уникальный идентификатор параметра
// Value - записываемое значение
// Возвращает:
// Код выполнения операции
int Device::setValueParameter(int ID, float Value)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем наличие в банке параметров параметра с указанным ID
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр с таким ID существует
      // Проверяем можно ли записать параметр
      Result = checkOperation(ID, OPERATION_WRITE);
      if(Result == RETURN_OK)
      {
        // Параметр можно записать
        // Проверяем удовлетворяет ли значение минимуму
        Result = checkMinimum(ID, Value);
        if(Result == RETURN_OK)
        {
          // Значение больше равно минимума
          // Проверяем удовлетворяет ли значение максимуму
          Result = checkMaximum(ID, Value);
          if(Result == RETURN_OK)
          {
            Result = setValue(ID, Value);
          }
          // Возвращаем результат функции checkMaximum
        }
        // Возвращаем результат функции checkMinimum
      }
      // Возвращаем результат функции checkOperation
    }
    // Возвращаем результат функции checkID
  }
  catch(...)
  {
    /// Формируем сообщение упали в функции setValueParameter
  }
  return Result;
};

// МЕТОД ЗАПИСИ ЗНАЧЕНИЯ ПАРАМЕТРА
// Метод записи значения параметра с проверкой системного уровня доступа
// Параметры:
// ID - уникальный идентификатор параметра
// Value - записываемое значение
// Возвращает:
// Код выполнения операции
int Device::setValueParameterCheckingAccess(int ID, float Value)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем уровень доступа к параметру
    Result = checkAccess(ID);
    if(Result == RETURN_OK)
    {
      /// Вызываем функцию записи параметра
      Result = setValueParameter(ID, Value);
    }
    // Возвращаем результат функции checkAccess;
  }
  catch(...)
  {
    /// Формируем сообщение упали в функции setValueParameterCheckingAccess
  }
  return Result;
};

// МЕТОД ЗАПИСИ ЗНАЧЕНИЯ ПАРАМЕТРА
// Метод записи значения параметра с проверкой уровня доступа в параметрах
// Параметры:
// ID - уникальный идентификатор параметра
// Value - записываемое значение
// Access - уровень доступа
// Возвращает:
// Код выполнения операции
int Device::setValueParameterCheckingAccess(int ID, float Value, int Access)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем уровень доступа к параметру
    Result = checkAccess(ID, Access);
    if(Result == RETURN_OK)
    {
      /// Вызываем функцию записи параметра
      Result = setValueParameter(ID, Value);
    }
    // Возвращаем результат функции checkAccess;
  }
  catch(...)
  {
    /// Формируем сообщение упали в функции setValueParameterCheckingAccess
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ ID ПАРАМЕТРА
// В метод передаётся ID параметра над которым мы ходим провести действия
// метод по ID определяет к какамому элементу в массиве параметров устройства
// мы хотим обратиться, извлекает у этого элемента массива значения поля ID
// Параметры:
// ID - идентификатор параметра
// Value - параметр ссылка на переменную которой присваивается ID
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - элемент массива найден и ID извлечено
// RETURN_ERROR_ID_VALUE
int Device::getID(int ID, int &Value)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    Value = Parameters[ID - Shift].ID;
    Result = RETURN_OK;
  }
  catch(...)
  {
    Result = RETURN_ERROR_ID_VALUE;
    // Message: Упали в функции getID | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ УРОВНЯ ДОСТУПА К ПАРАМЕТРУ
// В метод передаётся ID параметра у которого мы хотим узнать уровень доступа
// метод по ID определяет сначала проверяет ID на корректность, после чего
// извлекает уровень доступа с проверкой на корректность значения уровня
// Параметры:
// ID - идентификатор параметра
// Value - параметр ссылка на переменную которой присваивается ID
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_ACCESS_VALUE - Не корректный уровень доступа у параметра
int Device::getAccess(int ID, int &Access)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Access = Parameters[ID - Shift].Access;
      // Проверяем на корректность усровень доступа
      if((Access < 0)||(Access >= ACCESS_LAST))
      {
        // Если не корректный, возвращаем ошибку
        Result = RETURN_ERROR_ACCESS_VALUE;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции getAccess | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ CИСТЕМНОГО УРОВНЯ ДОСТУПА
// Параметры:
// Access - параметр ссылка на переменную которой присваивается уровень доступа
// Возвращает:
// RETURN_ERROR_ACCESS_VALUE - Не корректный уровень доступа у параметра
int Device::getAccessSystem(int &Access)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    Access = AccessSystem;
    // Проверяем на корректность усровень доступа
    if((Access < 0)||(Access >= ACCESS_LAST))
    {
      // Если не корректный, возвращаем ошибку
      Result = RETURN_ERROR_ACCESS_VALUE;
    }
    else
    {
      Result = RETURN_OK;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getAccessSystem
  }
  return Result;
};



// МЕТОД ПОЛУЧЕНИЯ ОПЕРАЦИЙ НАД ПАРАМЕТРОМ
// В метод передаётся ID параметра у которого мы хотим узнать возможные операции
// метод по ID определяет сначала проверяет ID на корректность, после чего
// извлекает операции с проверкой на корректность операций
// Параметры:
// ID - идентификатор параметра
// Operation - параметр ссылка на переменную которой присваивается операции над параметром
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_OPERATION_VALUE - Не корректня операция у параметра
int Device::getOperation(int ID, int &Operation)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Operation = Parameters[ID - Shift].Operation;
      // Проверяем на корректность усровень доступа
      if((Operation < 0)||(Operation >= OPERATION_LAST))
      {
        // Если не корректный, возвращаем ошибку
        Result = RETURN_ERROR_OPERATION_VALUE;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции getOperation | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ СОСТОЯНИЯ СИСТЕМЫ
// Параметры:
// Operation - параметр ссылка на переменную которой присваивается
// возможность записи в параметры с ограниченным доступом
int Device::getOperationSystem(int &Operation)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    Operation = OperationSystem;
    // Проверяем на корректность усровень доступа
    if((Operation < 0)||(Operation >= OPERATION_LAST))
    {
      // Если не корректный, возвращаем ошибку
      Result = RETURN_ERROR_OPERATION_VALUE;
    }
    else
    {
      Result = RETURN_OK;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getOperationSystem
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ ФИЗИЧЕСКОЙ ВЕЛИЧИНЫ ПАРАМЕТРА
// Параметры:
// ID - идентификатор параметра
// Physic - параметр ссылка на переменную которой присваивается
// код физической величины параметры
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_PHYSIC_VALUE - Не корректная физическая величина у параметра
int Device::getPhysic(int ID, int &Physic)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Physic = Parameters[ID - Shift].Physic;
      // Проверяем на корректность физической величины
      if((Physic < 0)||(Physic >= PHYSIC_LAST))
      {
        // Если не корректный, возвращаем ошибку
        Result = RETURN_ERROR_PHYSIC_VALUE;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции getPhysic | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ ВАЛИДНОСТИ ЗНАЧЕНИЯ ПАРАМЕТРА
// Параметры:
// ID - идентификатор параметра
// Validity - параметр ссылка на переменную которой присваивается
// код физической величины параметры
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_VALIDITY_VALUE - Не корректный флаг валидности у параметра
int Device::getValidity(int ID, int &Validity)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Validity = Parameters[ID - Shift].Validity;
      // Проверяем на корректность усровень доступа
      if((Validity < 0)||(Validity >= VALIDITY_LAST))
      {
        // Если не корректный, возвращаем ошибку
        Result = RETURN_ERROR_VALIDITY_VALUE;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции getValidity | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ПАРАМЕТРА
// Параметры:
// ID - идентификатор параметра
// Value - параметр ссылка на переменную которой присваивается
// код физической величины параметры
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
int Device::getValue(int ID, float &Value)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Value = Parameters[ID - Shift].Value;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getValue | ID
  }
  return Result;
};

// МЕТОД ПОЛУЧЕНИЯ ЗНАЧЕНИЯ ПАРАМЕТРА ПО УМОЛЧАНИЮ
// Параметры:
// ID - идентификатор параметра
// Default - параметр ссылка на переменную которой присваивается
// значение параметра по умолчанию
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
int Device::getDefault(int ID, float &Default)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем значение по умолчанию
      Default = Parameters[ID - Shift].Default;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getDefault | ID
  }
  return Result;
}

// МЕТОД ПОЛУЧЕНИЯ МИНИМАЛЬНОГО ЗНАЧЕНИЯ ПАРАМЕТРА
// Параметры:
// ID - идентификатор параметра
// Minimum - параметр ссылка на переменную которой присваивается
// код физической величины параметры
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
int Device::getMinimum(int ID, float &Minimum)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Minimum = Parameters[ID - Shift].Minimum;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getMinimum | ID
  }
  return Result;
};


// МЕТОД ПОЛУЧЕНИЯ МАКСИМАЛЬНОГО ЗНАЧЕНИЯ ПАРАМЕТРА
// Параметры:
// ID - идентификатор параметра
// Maximum - параметр ссылка на переменную которой присваивается
// максимально допустимое значение параметра
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
int Device::getMaximum(int ID, float &Maximum)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Maximum = Parameters[ID - Shift].Maximum;
    }
  }
  catch(...)
  {
    // Message: Упали в функции getMaximum | ID
  }
  return Result;
};

// МЕТОД ЗАПИСИ ЗНАЧЕНИЯ ПАРАМЕТРА В ПОЛЕ VALUE
// Параметры:
// ID - идентификатор параметра
// Value - значение
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Заись проведена успешно
int Device::setValue(int ID, float Value)
{
  int Result = RETURN_ERROR;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = checkID(ID);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Получаем уровень доступа параметра
      Parameters[ID - Shift].Value = Value;
    }
  }
  catch(...)
  {
    // Message: Упали в функции setValue | ID | Value
  }
  return Result;
};

// МЕТОД СБРОСА ЗНАЧЕНИЯ ПАРАМЕТРА В ЗНАЧЕНИЕ ПО УМОЛЧАНИЮ
int Device::resetToDefault(int ID)
{
  int Result = RETURN_ERROR;
  float Default;
  // Защищённая секция
  try
  {
    // Проверяем если ли такой параметр
    Result = getDefault(ID, Default);
    if(Result == RETURN_OK)
    {
      // Параметр найден
      // Приравниваем значение значению по умолчанию
      Parameters[ID - Shift].Value = Default;
    }
  }
  catch(...)
  {
    // Message: Упали в функции resetToDefault | ID
  }
  return Result;
};

// МЕТОД СБРОСА ВСЕХ ПАРАМЕТРОВ В ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ
int Device::resetToDefaultAll()
{
  int Result = RETURN_ERROR;
  int I;
  // Защищённая секция
  try
  {
    // Цикл по массиву параметров
    for(I = 0; I < Quantity; I++)
    {
      Result = resetToDefault(I + Shift);
      if(Result != RETURN_OK)
      {
        break;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции resetToDefaultAll | ID
  }
  return Result;
}

// МЕТОД ПРОВЕРКИ ID ПАРАМЕТРА
// Метод проверяет совпадает ли ID указанный в функции со значение поля
// ID в элементе массива для этого ID
// Параметры:
// ID - идентификатор параметра
// Возвращет:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - ID совпадают
// RETURN_ERROR_ID - ID не совпадают
int Device::checkID(int ID)
{
  int Result = RETURN_ERROR;
  int IdParameter;
  // Защищённая секция
  try
  {
    // Получаем ID параметра по указанному индексу
    Result = getID(ID, IdParameter);
    // Если нашли указанный элемент массива
    if(Result == RETURN_OK)
    {
      // ID в элементе массива, НЕ совпадает с запрашиваемым
      if(ID != IdParameter)
      {
        // Возвращаем RETURN_ERROR_ID
        Result = RETURN_ERROR_ID;
      }
      // Возвращаем RETURN_OK
    }
  }
  catch(...)
  {
    // Message: Упали в функции checkID | ID
  }
  return Result;
};

// МЕТОД ПРОВЕРКИ УРОВНЯ ДОСТУПА К ПАРАМЕТРУ
// Метод проверяет достаточно ли уровня доступа установленного в системе для
// операций над параметром
// Параметры:
// ID - идентификатор параметра
// Возвращет:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_ACCESS_VALUE - Не корректный уровень доступа у параметра
// RETURN_ERROR_ACCESS - Уровня доступа не достаточно
int Device::checkAccess(int ID)
{
  int Result = RETURN_ERROR;
  int AccessSystem;
  try
  {
    // Получем системный уровень доступа
    Result = getAccessSystem(AccessSystem);
    if(Result == RETURN_OK)
    {
      // Если системный уровень получен
      // Вызываем функцию проверки доступа,
      // используя как параметр системный уровень
      Result = checkAccess(ID, AccessSystem);
    }
  }
  catch(...)
  {
    // Message: Упали в функции checkAccess | ID
  }
  return Result;
};

// МЕТОД ПРОВЕРКИ УРОВНЯ ДОСТУПА К ПАРАМЕТРУ
// Метод проверяет достаточно ли уровня доступа указанного в параметрах
// функции для операций над параметром
// Параметры:
// ID - идентификатор параметра
// Access - уровень доступа
// Возвращет:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_ACCESS_VALUE - Не корректный уровень доступа у параметра
// RETURN_ERROR_ACCESS - Уровня доступа не достаточно
int Device::checkAccess(int ID, int Access)
{
  int Result = RETURN_ERROR;
  int AccessParameter;
  // Защищённая секция
  try
  {
    // Получаем уровень доступа к параметру
    Result = getAccess(ID, AccessParameter);
    // Если уровень доступа параметра получен
    if(Result == RETURN_OK)
    {
      // Если уровень доступа параметра больше указанного в параметре
      if(AccessParameter > Access)
      {
        Result = RETURN_ERROR_ACCESS;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции checkAccess | ID | Access
  }
  return Result;
};

// МЕТОД ПРОВЕРКИ ДОПУСТИМОСТИ ОПЕРАЦИИ НАД ПАРАМЕТРОМ
// Параметры:
// ID - идентификатор параметра
// Operation - операция над значением параметра
// Возвращает:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_OPERATION_VALUE - Не корректня операция у параметра
// RETURN_ERROR_OPERATION - Не корректня операция у параметра
int Device::checkOperation(int ID, int Operation)
{
  int Result = RETURN_ERROR;
  int OperationParameter;
  int OperationSystem;
  // Защищённая секция
  try
  {
    /// Сначала проверяем есть ли вообще такая операция которую хотим совершить
    if((Operation > RETURN_ERROR)&&(Operation < OPERATION_LAST))
    {
      // Хотим выполнить существующую операцию
      // Получаем код операций над параметром
      Result  = getOperation(ID, OperationParameter);
      if(Result == RETURN_OK)
      {
        // Получили операции над параметром
        // Если мы хотим записать параметр
        if(Operation == OPERATION_WRITE)
        {
          // Мы хотим записать параметр
          // Параметр доступен для записи только в останове
          if(OperationParameter == OPERATION_LIMITED)
          {
            // Получаем значение из системы
            Result = getOperationSystem(OperationSystem);
            // Если значение получения параметра записи корректно
            if(Result == RETURN_OK)
            {
              // Получили значение из системы
              if(OperationSystem != OPERATION_LIMITED)
              {
                Result = RETURN_ERROR_OPERATION;
              }

            }
            // сли не RETURN_OK возвращаем код ошибки getOperationSystem
          }
          // Если запись не ограничена Вернём RETURN_OK
        }
        // Если не OPERATION_WRITE Вернём RETURN_OK
      }
      // Если не RETURN_OK возвращаем код ошибки getOperation
    }
    // Запросили несуществующую операцию
    else
    {
      // Возвращаем код неверного значения
      Result = RETURN_ERROR_OPERATION_VALUE;
    }
  }
  catch(...)
  {

  }
  return Result;
};

// МЕТОД ПРОВЕРКИ ЗАПИСЫВАЕМОГО ЗНАЧЕНИЯ НА ПРЕВЫШЕНИЕ МИНИМУМА
// Метод проверяет не меньше ли записываемое значение миниального для параметра
// Параметры:
// ID - идентификатор параметра
// Value - записываемое значение
// Возвращет:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_MINIMUM - записываемое значение больше максимума для параметра
int Device::checkMinimum(int ID, float Value)
{
  int Result = RETURN_ERROR;
  float Minimum;
  // Защищённая секция
  try
  {
    // Получаем максимальное значение параметры
    Result = getMinimum(ID, Minimum);
    // Если уровень доступа параметра получен
    if(Result == RETURN_OK)
    {
      // Если уровень доступа параметра больше указанного в параметре
      if(Value < Minimum)
      {
        Result = RETURN_ERROR_MINIMUM;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции checkMinimum | ID | Value
  }
  return Result;
};

// МЕТОД ПРОВЕРКИ ЗАПИСЫВАЕМОГО ЗНАЧЕНИЯ НА ПРЕВЫШЕНИЕ МАКСИМУМА
// Метод проверяет не привышеет ли записываемое значение максимальное для параметры
// Параметры:
// ID - идентификатор параметра
// Value - записываемое значение
// Возвращет:
// RETURN_ERROR - Неизвестная ошибка
// RETURN_OK - Уровень доступа найден и корректен
// RETURN_ERROR_ID_VALUE - Не корректный ID
// RETURN_ERROR_ID - ID не совпадают
// RETURN_ERROR_MAXIMUM - записываемое значение больше максимума для параметра
int Device::checkMaximum(int ID, float Value)
{
  int Result = RETURN_ERROR;
  float Maximum;
  // Защищённая секция
  try
  {
    // Получаем максимальное значение параметры
    Result = getMaximum(ID, Maximum);
    // Если уровень доступа параметра получен
    if(Result == RETURN_OK)
    {
      // Если уровень доступа параметра больше указанного в параметре
      if(Value > Maximum)
      {
        Result = RETURN_ERROR_MAXIMUM;
      }
    }
  }
  catch(...)
  {
    // Message: Упали в функции checkMaximum | ID | Value
  }
  return Result;
};


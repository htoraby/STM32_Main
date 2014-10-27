/*
 * device_modbus.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device_modbus.h"

float Units[28][6][2] =
{
  /// PHYSIC_ERROR Ошибка типа 0
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_NUMERIC Перечислимый тип 1
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_TIME Время 2
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_PERCENT Проценты 3
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_LENGHT Длина 4
  {
    {LENGTH_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_DENSITY Плотность 5
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_SPACE     Площадь 6
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FREQUENCY Частота 7
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_VOLTAGE /// Напряжение 8
  {
    {VOLTAGE_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_CURRENT Ток 9
  {
    {CURRENT_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACTIVE_POWER Активная мощность 10
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FULL_POWER Полная мощность 11
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_REACTIVE_POWER Реактивная мощность 12
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ANGLE Угол 13
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RPM Скорость вращения 14
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_TEMP Темп (разгона или замедления) 15
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RESIST_ISOLATION Сопротивление изоляции 16
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_RESIST_CABLE_LINE Сопротивление кабельной линии 17
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_PRESSURE Давление 18
  {
    {PRESSURE_LAST, 0.0},
    {1.0, 0.0},                            /// PRESSURE_MPA
    {0.1013250, 0.0},                    /// PRESSURE_ATM
    {0.0980665, 0.0},                    /// PRESSURE_AT
    {0.1000000, 0.0},                    /// PRESSURE_BAR
    {0.0068948, 0.0}                    /// PRESSURE_PSI

  },
  /// PHYSIC_TEMPERATURE Температура 19
  {
    {TEMPERATURE_LAST, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACCELERATION Ускорение 20
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_SPEED Cкорость 21
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_VISCOSITY Вязкость 22
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FLOW /// Расход 23
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_POWER Мощность 24
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_ACTIVE_ENERGY Активная энергия 25
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_REACTIVE_ENERGY Реактивная энергия 26
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  },
  /// PHYSIC_FULL_ENERGY Полная энергия 27
  {
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0},
    {1.0, 0.0}
  }
};

DeviceModbus::DeviceModbus(int Address, int Quantity)
{
  DeviceAddress = Address;
  QuantityParam = Quantity;
}

DeviceModbus::~DeviceModbus()
{
  /// TODO Auto-generated destructor stub
}

/** 
@function getDeviceAddress
Функция возвращает адрес устройства (1..254)
@return Адрес устройства
*/
int DeviceModbus::getDeviceAddress()
{
  return DeviceAddress;
}



/*!
@function checkExchangModbusParameters
Проверяется предназначен ли параметр для работы в цикле и если он читается
записывается периодически истёк ли период
@param out_sParameter2 - параметр, отвечающий за что-то еще
@return Адрес устройства
*/
int DeviceModbus::checkExchangModbusParameters(int Index)
{
  int Result = RETURN_ERROR;
  try
  {
    /// Если параметр вообще читается
    if(ModbusParameters[Index].FreqExchange)
    {
      /// Если счётчик задержки больше или равен частоте опроса
      if(ModbusParameters[Index].CntExchange >= ModbusParameters[Index].FreqExchange)
      {
        /// Сбрасываем счётчик
        ModbusParameters[Index].CntExchange = 0;
        /// Возвращаем что параметр надо считать
        Result = RETURN_OK;
      }
      /// Иначе, счётчик задержки не истёк
      else
      {
        /// Увеличиваем счётчик задержки
        ModbusParameters[Index].CntExchange++;
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

/// МЕТОД ПОИСКА СЛЕДУЮЩЕГО ПАРАМЕТРА ДЛЯ РАБОТЫ
/// В методе перебираются всё параметры пока не будет
/// найден параметр для обработки
/// Функция возвращает индекс параметра
int DeviceModbus::searchNextModbusParameters()
{
  int Result = RETURN_ERROR;
  static int Index = 1;
  try
  {
    /// Пока не найдём параметр который надо читать
    while(!checkExchangModbusParameters(Index))
    {
      Result = Index;
      /// Увеличиваем счётчик параметров
      Index++;
      if(Index >= QuantityParam)
      {
        Index = 1;
      }
    }
  }
  catch(...)
  {

  }
  return Result;
}

/// МЕТОД ПОИСКА ПАРАМЕТРА С ПОВЫШЕННЫМ ПРИОРИТЕТ ОПЕРАЦИЙ
/// Функция один раз пробегает по массиву параметров
/// Ищет параметр с максимальным приоритетом операции
/// Приоритет по умолчанию 0 чем больше цифра тем выше приоритет
/// Функция возвращает 0 если нет параметров с высоким приоритетом
/// или индекс параметра в массиве
int DeviceModbus::searchPriorityModbusParameters()
{
  /// Инициализация результата
  int Result = RETURN_ERROR;
  /// Инициализация максимального приоритета
  int Priority = 0;
  /// Счетчик
  int Index = 1;
  try
  {
    /// Цикл по массиву параметров
    while(Index < QuantityParam)
    {
      /// Если приоритет текущего параметра больше предыдущего
      /// максимального приоритета
      if(ModbusParameters[Index].Priority > Priority)
      {
        /// Запоминаем индекс параметра
        Result = Index;
        /// Запоминаем приоритет
        Priority = ModbusParameters[Index].Priority;
      }
      Index++;
    }
  }
  catch(...)
  {

  }
  return Result;
}

/// МЕТОД ПОИСКА ПАРАМЕТРА ДЛЯ ОБРАБОТКИ
/// В методе вызываются функции поиска параметра с высоким приоритетом
/// и поиска следующего параметра для обработки
/// Функция возвращает индекс параметра с которым нужно работать сейчас
int DeviceModbus::searchExchangModbusParameters(void)
{
  int Result = RETURN_ERROR;
  static int I = 0;
  try
  {
    /// Cначала ищем параметр с максимальным приоритетом
    Result = searchPriorityModbusParameters();
    /// Если такого параметра нет
    if (!Result)
    {
      /// Ищем следующий параметр для цикла
      Result = searchNextModbusParameters();
    }
  }
  catch(...)
  {

  }
  return Result;
}

void DeviceModbus::excangeCycle()
{
  int Index = 0;
  try
  {
    /// Находим ID параметра для цикла опроса
    Index = searchExchangModbusParameters();
  }
  catch(...)
  {

  }
}

int DeviceModbus::getIndexInArrayAtAddress(int Address)
{
  int Result = RETURN_ERROR;
  int Index = 1;
  try {
    //! Цикл по карте регистров
    while (Index < QuantityParam) {
      //! Если нашли совпадение адреса
      if (ModbusParameters[Index].Address == Address) {
        //! Возвращаем индекс параметра в массиве с данным адресом
        Result = Index;
        break;
      }
      Index++;
    }
  }
  catch(...) {
    //! Message: Упали в функции getIndexInArrayAtAddress | Address
  }
  return Result;
};

int DeviceModbus::getModbusIDAtIndex(int Index)
{
  int Result = RETURN_ERROR;
  try {
    Result = ModbusParameters[Index].ID;
  }
  catch(...) {
    //! Message: Упали в функции getModbusIDAtIndex | Index
  }
  return Result;
};

int DeviceModbus::getModbusTypeDataAtIndex(int Index)
{
  int Result = RETURN_ERROR;
  try {
    if((ModbusParameters[Index].TypeData > TYPE_DATA_ERROR)
       && (ModbusParameters[Index].TypeData < TYPE_DATA_LAST)) {
       Result = ModbusParameters[Index].TypeData;
    }
  }
  catch(...) {
    //! Message: Упали в функции getModbusTypeDataAtIndex | Index
  }
  return Result;
}

float DeviceModbus::getModbusScaleAtIndex(int Index)
{
  float Result = RETURN_ERROR;
  try {
    Result = ModbusParameters[Index].Scale;
  }
  catch(...) {
    //! Message: Упали в функции getModbusScaleAtIndex | Index
  }
  return Result;
};

float DeviceModbus::getModbusCoefficientAtIndex(int Index)
{
  float Result = RETURN_ERROR;
  try {
    Result = ModbusParameters[Index].Coefficient;
  }
  catch(...) {
    //! Message: Упали в функции getModbusCoefficientAtIndex | Index
  }
  return Result;
};

int DeviceModbus::getModbusPhysicAtIndex(int Index)
{
  int Result = RETURN_ERROR;
  try {
    if((ModbusParameters[Index].Physic > PHYSIC_ERROR)
       && (ModbusParameters[Index].Physic < PHYSIC_LAST)) {
       Result = ModbusParameters[Index].Physic;
    }
  }
  catch(...) {
    //! Message: Упали в функции getModbusPhysicAtIndex | Index
  }
  return Result;
}

int DeviceModbus::getModbusUnitAtIndex(int Index)
{
  int Result = RETURN_ERROR;
  try {
    Result = ModbusParameters[Index].Unit;
  }
  catch(...) {
    //! Message: Упали в функции getModbusUnitAtIndex | Index
  }
  return Result;
}

float DeviceModbus::convModbusToParameters(int Address, unTypeData ModbusValue)
{
  float Result = RETURN_ERROR;
  int Index = 0;
  try {
    //! Получаем индекс параметра в карте регистров
    Index = getIndexInArrayAtAddress(Address);
    //! Преобразуем полученное число в float
    Result = convModbusTypeToFloatAtIndex(Index, ModbusValue);
    //! Применяем масштабируемость
    Result = applyModbusScaleAtIndex(Index, Result);
    //! Применяем коэффициент
    Result = applyModbusCoefficientAtIndex(Index, Result);
    //! Применяем перевод единиц измерения
    Result = convModbusPhysicUnitAtIndex(Index, Result);
  }
  catch(...) {
    /// Message: Упали в функции conversionToParameters
  }
  return Result;
};

float DeviceModbus::convModbusTypeToFloatAtIndex(int Index, unTypeData ModbusValue)
{
  float Result = RETURN_ERROR;
  try {
    int TypeData = getModbusTypeDataAtIndex(Index, TypeData);
    if (TypeData) {
      switch (TypeData) {
      case TYPE_DATA_CHAR:
           Result = (float)ModbusValue.Chars[0];
           break;
      case TYPE_DATA_INT16:
           Result = (float)ModbusValue.Int16[0];
           break;
      case TYPE_DATA_INT32:
           Result = (float)ModbusValue.Int32;
           break;
      case TYPE_DATA_UINT16:
           Result = (float)ModbusValue.Uint16[0];
           break;
      case TYPE_DATA_UINT32:
           Result = (float)ModbusValue.Uint32;
           break;
      case TYPE_DATA_FLOAT:
           Result = (float)ModbusValue.Float;
           break;
      }
    }
  }
  catch (...) {
    //! Message: Упали в функции convModbusTypeToFloatAtIndex | Index
  }
  return Result;
};

float DeviceModbus::applyModbusScaleAtIndex(int Index, float Value)
{
  float Result = RETURN_ERROR;
  try {
    Result = getModbusScaleAtIndex(Index);
    if (Result)
      Result = Value * Result;
  }
  catch (...) {
    //! Message: Упали в функции convModbusScaleAtIndex | Index
  }
}

float DeviceModbus::applyModbusCoefficientAtIndex(int Index, float Value)
{
  float Result = RETURN_ERROR;
  try {
    float Value = getModbusCoefficientAtIndex(Index);
    if (Value)
      Result = Value / getModbusScaleAtIndex(Index);
  }
  catch (...) {
    //! Message: Упали в функции convModbusScaleAtIndex | Index
  }
}


/// МЕТОДЫ ПРЕОБРАЗОВАНИЯ
/// ЗНАЧЕНИЯ ИЗ БАНКА ПАРАМЕТРОВ УСТРОЙСТВА ДЛЯ MODBUS

/// МЕТОД ПРЕОБРАЗОВАНИЯ ЗНАЧЕНИЯ ИЗ БАНКА ПАРАМЕТРОВ В MODBUS ЗНАЧЕНИЕ
/// Сначала переводим в единицы измерения
int classDeviceModbus::convParamToModbus(int Address, float Value, unTypeData &ModbusValue)
{
  int Result = RETURN_ERROR;
  try
  {
    /// Применяем перевод единиц измерения
    Result = convParamToModbusPhysicUnit(Address, Value);
    if(Result == RETURN_OK)
    {
      Result = convParamToModbusCoefficient(Address, Value);
      if(Result == RETURN_OK)
      {
        Result = convParamToModbusScale(Address, Value);
        if(Result == RETURN_OK)
        {
          Result = convParamToModbusType(Address, Value, ModbusValue);
        }
      }
    }
  }
  catch(...)
  {
    /// Message: Упали в функции conversionToParameters
  }
  return Result;
};


/// МЕТОД ПРИВЕДЕНИЯ ЗНАЧЕНИЯ ИЗ ВНУТРЕННЕМУ ПРЕДСТАВЛЕНИЯ К ЕДИНИЦАМ ПАРАМЕТРА
/// В методе к значению полученному из банка параметров преобразование единиц измерения
/// Параметры:
/// Address - адресс параметра в modbus карте,
/// Value - переменная ссылка на перемнную в которую сохраняем значение
/// Возвращает:
/// RETURN_ERROR - Неизвестная ошибка
int classDeviceModbus::convParamToModbusPhysicUnit(int Address, float &Value)
{
  int Result = RETURN_ERROR;
  int Physic;
  int Unit;
  try
  {
    /// Получаем физическую величину параметра
    Result = getModbusPhysic(Address, Physic);
    if(Result == RETURN_OK)
    {
      /// Получаем единицы измерения параметра
      Result = getModbusUnit(Address, Unit);
      if(Result == RETURN_OK)
      {
        /// Вычисляем значение с пропорциональным и линейным коэффициентом
        Value = (Value - (Units[Physic][Unit][1])) / (Units[Physic][Unit][0]);
      }
    }
  }
  catch(...)
  {
    /// Message: Упали в функции conversionModbusValuePhysicUnit
  }
  return Result;
};

int classDeviceModbus::convParamToModbusType(int Address, float &Value, unTypeData &ModbusValue)
{
  int Result = RETURN_ERROR;
  int TypeData;
  try
  {
    Result = getModbusTypeData(Address, TypeData);
    if(Result == RETURN_OK)
    {
      switch(TypeData)
      {
      case    TYPE_DATA_CHAR:
        ModbusValue.Chars[0] = (char)(Value + 0.5);
        break;
      case    TYPE_DATA_INT16:
        ModbusValue.Int16[0] = (short int)(Value + 0.5);
        break;
      case    TYPE_DATA_INT32:
        ModbusValue.Int32 = (int)(Value + 0.5);
        break;
      case    TYPE_DATA_UINT16:
        ModbusValue.Uint16[0] = (unsigned short int)(Value + 0.5);
        break;
      case    TYPE_DATA_UINT32:
        ModbusValue.Uint32 = (unsigned int)(Value + 0.5);
        break;
      case    TYPE_DATA_FLOAT:
        ModbusValue.Float = Value;
        break;
      }
    }
  }
  catch(...)
  {

  }
  return Result;
};



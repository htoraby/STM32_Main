/*
 * device.h

 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "define.h"

/*!
\struct Parameter
Структура для обработки и хранения параметров в станции управления
*/
struct Parameter
{
  /// Уникальный идентификатор параметра заменяет название параметра равен
  /// одному из кодов из enum enID
  int ID;
  /// Уровень доступа к параметру каждый параметр имеет уровень доступа равен
  /// одному из кодов из enAccess
  int Access;
  /// Операции с параметром: чтение, запись, запись во время останова равен
  /// одному из кодов из enOperations
  int Operation;
  /// Физическая величина параметра: длина, время и т.д. равен
  /// одному из кодов из enPhysic
  int Physic;
  /// Поле состояния параметра: true если значение параметра валидно и false
  /// если значение параметра не валидно, равен одному из кодов enValidity
  int Validity;
  /// Поле обновления параметра: false когда изменили и ещё не получили
  /// подтверждения от устройства
  int Update;
  /// Значение параметра, для единообразия значения всех параметров будем
  /// хранить в типе double
  double Value;
  /// Минимально допустимое значение для этого параметра. Используется для
  /// вывода диапазона значений на экран и проверке вводимых значений
  double Minimum;
  /// Максимально допустимое значение для этого параметра. Используется для
  /// вывода диапазона значений на экран и проверке вводимых значений
  double Maximum;
  /// Значение по умолчанию
  double Default;
};

/*!
\class Device
Базовый класс для всех устройств в системе содержит банк параметров
устройства во внутреннем представлении и набором методов для работы с ними
*/
class Device
{
// ОТКРЫТЫЕ ЧЛЕНЫ КЛАССА
public:
  /// Констуртор класса
  Device();
  /// Деструктор класса
  virtual ~Device();

  // ОСНОВНЫЕ PUBLIC МЕТОДЫ КЛАССА
  // ЧТЕНИЕ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ С ПРОВЕРКАМИ
  /*!
   * \brief getCheckParameter
   * Функция получения всей структуры параметра с проверками
   * \param ID - уникальный идентификатор параметра
   * \param Param - ссылка на структуру которой присваиваются значения полей
   * \return 0 - операция выполнена или # код ошибки
   */
  int getCheckParameter(int ID, Parameter &Param);

  /*!
   * \brief getCheckValue
   * Функция получения текущего значения параметра с проверками
   * \param ID - уникальный идентификатор параметра
   * \param Value - параметр ссылка на переменную в которую возвращается значение
   * \return 0 - операция выполнена или # код ошибки
   */
  int getCheckValue(int ID, double &Value);

  /*!
   * \brief getCheckAccess
   * Функция получения уровня доступа к параметру с проверкой валидности
   * полученного значения
   * \param ID - уникальный идентификатор параметра
   * \return 0 - не валидный уровень доступа или # код уровня доступа
   */
  int getCheckAccess(int ID);

  /*!
   * \brief getCheckOperation
   * Функция получения операций над параметром с проверкой валидности
   * полученного значения
   * \param ID - уникальный идентификатор параметра
   * \return 0 - не валидный уровень доступа или # код операции
   */
  int getCheckOperation(int ID);

  /*!
   * \brief getCheckPhysic
   * Функция получения физической величины с проверкой валидности
   * полученного значения
   * \param ID - уникальный идентификатор параметра
   * \return 0 - не валидное значение, или операции
   */
  int getCheckPhysic(int ID);

  /*!
   * \brief getCheckValidity
   * Функция получения валидности параметра с проверкой валидности
   * полученного значения
   * \param ID - уникальный идентификатор параметра
   * \return 0 не валидно или # код валидности
   */
  int getCheckValidity(int ID);

  /*!
   * \brief getCheckUpdate
   * Функция получения нового значения параметра с проверкой валидности
   * полученного значения
   * \param ID - уникальный идентификатор параметра
   * \return 0 не обновлено или # код обновления
   */
  int getCheckUpdate(int ID);

  // ЗАПИСЬ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ С ПРОВЕРКАМИ
  /*!
   * \brief setCheckParameter
   * Функция записи полей параметра с проверками
   * \param Param Структура параметра
   * \return 0 - операция выполнена или # код ошибки
   */
  int setCheckParameter(int ID, Parameter Param);

  /*!
   * \brief setCheckValue
   * Функция присвоения значения параметру с проверками
   * \param ID - уникальный идентификатор параметра
   * \param Value - присваиваемое значение
   * \return 0 - операция выполнена или # код ошибки
   */
  int setCheckValue(int ID, double Value);

  /*!
   * \brief setCheckAccess
   * \param ID
   * \param Access
   * \return
   */
  int setCheckAccess(int ID, int Access);

  /*!
   * \brief setCheckOperation
   * \param ID
   * \param Operation
   * \return
   */
  int setCheckOperation(int ID, int Operation);

  /*!
   * \brief setCheckPhysic
   * \param ID
   * \param Physic
   * \return
   */
  int setCheckPhysic(int ID, int Physic);

  /*!
   * \brief setCheckValidity
   * \param ID
   * \param Validity
   * \return
   */
  int setCheckValidity(int ID, int Validity);

  /*!
   * \brief setCheckUpdate
   * \param ID
   * \param Update
   * \return
   */
  int setCheckUpdate(int ID, int Update);

// НАСЛЕДУЕМЫЕ ЧЛЕНЫ КЛАССА
protected:
  /// Массив параметров устройства
  Parameter Parameters[];
  /// Смещение между элементами массива и ID для устройства
  int ShiftID;
  /// Количество элементов массива устройства
  int NumberElementArray;

  // ЧТЕНИЕ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ БЕЗ ПРОВЕРОК
  /*!
   * \brief getFieldParameter
   * Функция получения полей параметра
   * \param ID - уникальный идентификатор параметра
   * \return значения полей параметра
   */
  Parameter getParameter(int ID);

  /*!
   * \brief getValue
   * Функция получения значения параметра из массива параметров
   * \param ID - уникальный идентификатор параметра
   * \return значение параметра
   */
  double getValue(int ID);

  /*!
   * \brief getAccess
   * Функция получения уровня доступа к параметру
   * \param ID - уникальный идентификатор параметра
   * \return Уровень доступа
   */
  int getAccess(int ID);

  /*!
   * \brief getOperation
   * Функция получения операций над параметром
   * \param ID - уникальный идентификатор параметра
   * \return Код операции над параметром
   */
  int getOperation(int ID);

  /*!
   * \brief getPhysic
   *  Функция получения физической величины параметра
   * \param ID - уникальный идентификатор параметра
   * \return Код физическая величина параметра
   */
  int getPhysic(int ID);

  /*!
   * \brief getValidity
   * Функция получения валидности параметра
   * \param ID - уникальный идентификатор параметра
   * \return Код валидность параметра
   */
  int getValidity(int ID);

  /*!
   * \brief getUpdate
   * Функция получения обновления параметра
   * \param ID - уникальный идентификатор параметра
   * \return Код обновленность параметра
   */
  int getUpdate(int ID);

  /*!
   * \brief getMinimum
   * Функция получения минимального значения для параметра
   * \param ID - уникальный идентификатор параметра
   * \return мининальное значение параметра
   */
  double getMinimum(int ID);

  /*!
   * \brief getMaximum
   * Функция получения максимального значения для параметра
   * \param ID - уникальный идентификатор параметра
   * \return максимальное значение параметра
   */
  double getMaximum(int ID);

  /*!
   * \brief getDefault
   * Функция получения значения по умолчанию
   * \param ID - уникальный идентификатор параметра
   * \return значение параметра по умолчанию
   */
  double getDefault(int ID);

  // ЗАПИСЬ ПАРАМЕТРА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ БЕЗ ПРОВЕРОК
  /*!
   * \brief setParameter
   * Функция записи полей параметра
   * \param ID - уникальный идентификатор параметра
   * \param Param значения полей параметра
   */
  void setParameter(int ID, Parameter Param);

  /*!
   * \brief setValue
   * Функция присвоения значению параметра
   * \param ID - уникальный идентификатор параметра
   * \param Value - присваимое значение
   */
  void setValue(int ID, double Value);

  /*!
   * \brief setAccess
   * Функция присвоения уровня доступа к параметру
   * \param ID - уникальный идентификатор параметра
   * \param Access
   */
  void setAccess(int ID, int Access);

  /*!
   * \brief setOperation
   * Функция присвоения операций над параметром
   * \param ID - уникальный идентификатор параметра
   * \param Operation
   */
  void setOperation(int ID, int Operation);

  /*!
   * \brief setPhysic
   * Функция присвоения операций над параметром
   * \param ID - уникальный идентификатор параметра
   * \param Physic
   */
  void setPhysic(int ID, int Physic);

  /*!
   * \brief setValidity
   * Функция присвоения валидности параметра
   * \param ID - уникальный идентификатор параметра
   * \param Validity
   */
  void setValidity(int ID, int Validity);

  /*!
   * \brief setUpdate
   * Функция присвоения свежести параметра
   * \param ID - уникальный идентификатор параметра
   * \param Update
   */
  void setUpdate(int ID, int Update);

  /*!
   * \brief setMinimum
   * Функция присвоения минимального значения параметра
   * \param ID - уникальный идентификатор параметра
   * \param Minimum
   */
  void setMinimum(int ID, double Minimum);

  /*!
   * \brief setMaximum
   * Функция присвоения максимального значения параметра
   * \param ID - уникальный идентификатор параметра
   * \param Maximum
   */
  void setMaximum(int ID, double Maximum);

  /*!
   * \brief setDefault
   * Функция присвоения значения по умолчанию
   * \param ID - уникальный идентификатор параметра
   * \param Default
   */
  void setDefault(int ID, double Default);

  // ЧТЕНИЕ ЭЛЕМЕНТА МАССИВА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ
  /*!
   * \brief getParameterIndex
   * Функция получения значений полей из структуры Parameter
   * \param Index индекс элемента массива Parameters[]
   * \return Значения полей Parameter
   */
  Parameter getParameterIndex(int Index);

  /*!
   * \brief getValueIndex
   * Функция получения значения поля Value из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Value
   */
  double getValueIndex(int Index);

  /*!
   * \brief getIDIndex
   * Функция получения значения поля ID из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля ID
   */
  int getIDIndex(int Index);

  /*!
   * \brief getAccessIndex
   * Функция получения значения поля Access из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Access
   */
  int getAccessIndex(int Index);

  /*!
   * \brief getOperationIndex
   * Функция получения значения поля Operation из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Operation
   */
  int getOperationIndex(int Index);

  /*!
   * \brief getPhysicIndex
   * Функция получения значения поля Physic из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Physic
   */
  int getPhysicIndex(int Index);

  /*!
   * \brief getValidityIndex
   * Функция получения значения поля Validity из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Validity
   */
  int getValidityIndex(int Index);

  /*!
   * \brief getUpdateIndex
   * Функция получения значения поля Update из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Update
   */
  int getUpdateIndex(int Index);

  /*!
   * \brief getMinimumIndex
   * Функция получения значения поля Minimum из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Minimum
   */
  double getMinimumIndex(int Index);

  /*!
   * \brief getMaximumIndex
   * Функция получения значения поля Maximum из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Maximum
   */
  double getMaximumIndex(int Index);

  /*!
   * \brief getDefaultIndex
   * Функция получения значения поля Default из структуры Parameter
   * \param Index - индекс элемента массива Parameters[]
   * \return Значение поля Default
   */
  double getDefaultIndex(int Index);

  // ЗАПИСЬ ЭЛЕМЕНТА МАССИВА, ЗНАЧЕНИЯ И ДРУГИХ ПОЛЕЙ
  /*!
   * \brief setParameterIndex
   * Функция присвоения значений полей
   * \param Index - индекс элемента массива Parameters[]
   * \param Param присваиваемое значение
   */
  void setParameterIndex(int Index, Parameter Param);

  /*!
   * \brief setValueIndex
   * Функция присвоения значения поля Value значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Value - присваиваемое значение
   */
  void setValueIndex(int Index, double Value);

  /*!
   * \brief setAccessIndex
   * Функция присвоения значения поля Access значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Access - присваиваемый уровень доступа
   */
  void setAccessIndex(int Index, int Access);

  /*!
   * \brief setOperationIndex
   * Функция присвоения значения поля Operation значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Operation - присваиваемые операции
   */
  void setOperationIndex(int Index, int Operation);

  /*!
   * \brief setPhysicIndex
   * Функция присвоения значения поля Physic значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Physic - присваиваемая физическая величина
   */
  void setPhysicIndex(int Index, int Physic);

  /*!
   * \brief setValidityIndex
   * Функция присвоения значения поля Validity значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Validity - присваиваемая валидность
   */
  void setValidityIndex(int Index, int Validity);

  /*!
   * \brief setUpdateIndex
   * Функция присвоения значения поля Update значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Update - присваиваемое обновление
   */
  void setUpdateIndex(int Index, int Update);

  /*!
   * \brief setMimimumIndex
   * Функция присвоения значения поля Mimimum значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Mimimum - присваиваемое минимальное значение
   */
  void setMinimumIndex(int Index, double Minimum);

  /*!
   * \brief setMaximumIndex
   * Функция присвоения значения поля Maximum значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Maximum - присваиваемое максимальное значение
   */
  void setMaximumIndex(int Index, double Maximum);

  /*!
   * \brief setDefaultIndex
   * Функция присвоения значения поля Default значению из параметра
   * \param Index - индекс элемента массива Parameters[]
   * \param Default - присваиваемое значение по умолчанию
   */
  void setDefaultIndex(int Index, double Default);





  // Базовые методы работы c параметрами использующие ID и внутри вызывающие
  // методы для работы с Index в массиве параметров

  /*!
  \function searchIndex
  Функция поиска в массиве Parameters[] индекса записи с полем ID
  раным указанному в параметре
  \param int ID - уникальный идентификатор параметра
  \return 0 - не найден индекс записи
  */
  int searchIndex(int ID);

  /*!
  \function checkIndex
  Функция проверки содержится ли в элементе массива с ID - Shift
  запись с указанным ID
  \param int ID - уникальный идентификатор параметра
  */
  int checkIndex(int ID);

  /*!
  \function getIndex
  Функция получения индекса параметра с указанным ID
  \param int ID - уникальный идентификатор параметра
  */
  int getIndex(int ID);

  /*!
    \function getShiftID
    Функция получения значения поля ShiftID класса Device
    \return Значение ShiftID класса Device
  */
  int getShiftID();

  /*!
    \function setShiftID
    Функция присвоения значения поля поля ShiftID класса Device
    \param int Shift - присваиваемое значение полю ShiftID класса Device
  */
  void setShiftID(int Shift);

  /*!
  \function getNumberElementArray
  Функция получения значения поля Device NumberElementArray
  \return Значение поля Device NumberElementArray
  */
  int getNumberElementArray();

  /*!
  \function setNumberElementArray
  Функция присвоения значения поля Device NumberElementArray
  \param int Number - присваиваемое значение полю Device NumberElementArray
  */
  void setNumberElementArray(int Number);

  // Базовые методы работы со с массивом параметров Parameters эти методы
  // работают с индексом параметра в массиве параметров





private:
};

#endif /* DEVICE_H_ */

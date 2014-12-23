/*
 * device.h

 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "define.h"
#include "service.h"
#include "board.h"

extern float Units[28][6][2];

/*!
 * \brief The Parameter struct
 * Структура для обработки и хранения параметров в станции управления
 */
struct parameter
{
  // Уникальный идентификатор параметра заменяет название параметра равен
  // одному из кодов из enum enID
  unsigned short id;
  // Уровень доступа к параметру каждый параметр имеет уровень доступа равен
  // одному из кодов из enAccess
  unsigned char access;
  // Операции с параметром: чтение, запись, запись во время останова равен
  // одному из кодов из enOperations
  unsigned char operation;
  // Физическая величина параметра: длина, время и т.д. равен
  // одному из кодов из enPhysic
  unsigned char physic;
  // Поле состояния параметра: true если значение параметра валидно и false
  // если значение параметра не валидно, равен одному из кодов enValidity
  unsigned char validity;
  // Поле обновления параметра: false когда изменили и ещё не получили
  // подтверждения от устройства
  unsigned char update;
  // Значение параметра, для единообразия значения всех параметров будем
  // хранить в типе double
  float value;
  // Минимально допустимое значение для этого параметра. Используется для
  // вывода диапазона значений на экран и проверке вводимых значений
  float min;
  // Максимально допустимое значение для этого параметра. Используется для
  // вывода диапазона значений на экран и проверке вводимых значений
  float max;
  // Значение по умолчанию
  float def;
};

/*!
 * \brief The Device class
 * Базовый класс для всех устройств в системе содержит банк параметров
 * устройства во внутреннем представлении и набором методов для работы с ними
 */
class Device
{
// ОТКРЫТЫЕ ЧЛЕНЫ КЛАССА
public:
  /// Констуртор класса
  Device();
  /// Деструктор класса
  virtual ~Device();

  virtual void initParameters();

  // Функция создания задач для FreeRTOS
  /*!
   * \brief createThread Метод создания задач для FreeRTOS
   * \param threadName Название задачи
   */
  void createThread(const char *threadName);

  /*!
   * \brief threadUpdateParametersId_
   * Идентификатор задачи обновления значений параметра устройства
   */
  osThreadId threadUpdateParametersId_;

  /*!
   * \brief createMessageUpdateParameters
   * Метод создания очереди обновленных параметров
   */
  void createMessageUpdateParameters(void);

  /*!
   * \brief messageUpdateParameters_
   * Идентификатор очереди обновленных параметров
   */
  osMessageQId messageUpdateParameters_;

  int getMessageUpdateParameters(void);

  unsigned short countParameter_;

  void updateParameters(void);

  // Базовые методы работы со структурой Parameter
  // Работают с индексом в массиве параметров
  /*!
   * \brief getFieldID
   * Метод получения ID параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле ID
   */
  unsigned short getFieldId(unsigned short index);

  /*!
   * \brief getFieldAccess
   * Метод получения Access параметра по индексу в массиве параметров
   * \param Index
   * \return поле Access
   */
  unsigned char getFieldAccess(unsigned short index);

  /*!
   * \brief getFieldOperation
   * Метод получения Operation параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Operation
   */
  unsigned char getFieldOperation(unsigned short index);

  /*!
   * \brief getFieldPhysic
   * Метод получения Physic параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Physic
   */
  unsigned char getFieldPhysic(unsigned short index);

  /*!
   * \brief getFieldValidity
   * Метод получения Validity параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Validity
   */
  unsigned char getFieldValidity(unsigned short index);

  /*!
   * \brief getFieldUpdate
   * Метод получения Update параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Update
   */
  unsigned char getFieldUpdate(unsigned short index);

  /*!
   * \brief getFieldValue
   * Метод получения Value параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Value
   */
  float getFieldValue(unsigned short index);

  /*!
   * \brief getFieldMinimum
   * Метод получения Minimum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Minimum
   */
  float getFieldMinimum(unsigned short index);

  /*!
   * \brief getFieldMaximum
   * Метод получения Maximum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Maximum
   */
  float getFieldMaximum(unsigned short index);

  /*!
   * \brief getFieldDefault
   * Метод получения Default параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Default
   */
  float getFieldDefault(unsigned short index);

  /*!
   * \brief getFieldAll
   * Метод получения All полей параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return All поля
   */
  parameter getFieldAll(unsigned short index);

  /*!
   * \brief setFieldID
   * Метод присвоения поля ID в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param ID присваемое значение
   */
  void setFieldID(unsigned short index, unsigned short id);

  /*!
   * \brief setFieldAccess
   * Метод присвоения поля Access в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Access присваемое значение
   */
  void setFieldAccess(unsigned short index, unsigned char access);

  /*!
   * \brief setFieldOperation
   * Метод присвоения поля Operation в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Operation присваемое значение
   */
  void setFieldOperation(unsigned short index, unsigned char operation);

  /*!
   * \brief setFieldPhysic
   * Метод присвоения поля Physic в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Physic присваемое значение
   */
  void setFieldPhysic(unsigned short index, unsigned char physic);

  /*!
   * \brief setFieldValidity
   * Метод присвоения поля Validity в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Validity присваемое значение
   */
  void setFieldValidity(unsigned short index, unsigned char validity);

  /*!
   * \brief setFieldUpdate
   * Метод присвоения поля Update в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Update присваемое значение
   */
  void setFieldUpdate(unsigned short index, unsigned char update);

  /*!
   * \brief setFieldValue
   * Метод присвоения поля value в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param value присваемое значение
   */
  void setFieldValue(unsigned short index, float value);

  /*!
   * \brief setFieldMin
   * Метод присвоения поля Min в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param min присваемое значение
   */
  void setFieldMin(unsigned short index, float min);

  /*!
   * \brief setFieldMax
   * Метод присвоения поля Max в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param max присваемое значение
   */
  void setFieldMax(unsigned short index, float max);

  /*!
   * \brief setFieldDef
   * Метод присвоения поля def в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param def присваемое значение
   */
  void setFieldDef(unsigned short index, float def);

  /*!
   * \brief setFieldAll
   * Метод присвоения поля All в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param param присваемое значение
   */
  void setFieldAll(unsigned short index, parameter param);

  /*!
   * \brief Метод поиска и получения индекса по ID параметра
   * \param ID идентификатор параметра
   * \return Index
  */
  unsigned short getIndexAtID(unsigned short ID);

  /*!
   * \brief getValue
   * Метод получения значения параметра по id
   * \param id уникальный идентификатор параметра
   * \return значение параметра
   */
  float getValue(unsigned short id);

  /*!
   * \brief setValue
   * Метод присвоения значения параметру
   * Когда пытаемся записать значение, нам нужно проверить
   * 1. Уровень доступа к параметру
   * 2. Операции над параметром
   * 3. Минимальное и максимальное значение
   * \param id уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  unsigned char setValue(unsigned short id, float value);

// Наследуемые члены класс
protected:
  /// Массив параметров устройства
  parameter parameters_[];

private:

};

#endif /* DEVICE_H_ */

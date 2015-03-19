/*
 * device.h

 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "define.h"
#include "idslist.h"
#include "board.h"
#include "fram.h"

extern float Units[28][6][2];

/*!
 * \brief Структура для обработки и хранения параметров в СУ
 */
struct parameter
{
  unsigned short id;        /// Уникальный идентификатор параметра заменяет название параметра равен
  unsigned char access;     /// Уровень доступа к параметру
  unsigned char operation;  /// Операции с параметром: чтение, запись, запись во время останова
  unsigned char physic;     /// Физический смысл
  float min;                /// Минимально допустимое значение для этого параметра.
  float max;                /// Максимально допустимое значение для этого параметра.
  float def;                /// Значение по умолчанию
  unsigned char validity;   /// Валидность параметра
  float value;              /// Значение параметра, для единообразия значения всех параметров будем
};

/*!
 * \brief Базовый класс для всех устройств
 * Класс содержит банк параметров устройства во внутреннем представлении и
 * наборы методов для работы с ними
 */
class Device
{

public:
  /*!
   * \brief Конструктор класса
   * \param startAddrParams - базовый адрес параметров (ID_BEGIN)
   * \param parameters - указатель на массив параметров
   * \param countParameter - количество параметров
   */
  Device(uint32_t startAddrParams, parameter *parameters, uint16_t countParameter);
  virtual ~Device();

  /*!
   * \brief Инициализация устройства
   */
  virtual void init() = 0;

  /*!
   * \brief Инициализация массива параметров
   */
  void initParameters();

  /*!
   * \brief Метод создания задач для FreeRTOS
   * \param threadName Название задачи
   */
  void createThread(const char *threadName);

  //! Идентификатор задачи обновления значений параметра устройства
  osThreadId updateValueThreadId_;
  //! Идентификатор очереди параметров с новыми значениями от устройства
  osMessageQId getValueDeviceQId_;

  //!
  void updateValueTask();
  virtual void getNewValue(uint16_t id);
  virtual void calcParameters(uint16_t id);

  /*!
   * \brief writeValue
   * \param id
   * \param value
   */
  virtual uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод поиска и получения индекса по ID параметра
   * \param ID идентификатор параметра
   * \return Index
  */
  unsigned short getIndexAtId(unsigned short id);

  /*!
   * \brief Метод получения значения параметра по id
   * \param id уникальный идентификатор параметра
   * \param ok - действительное значение или нет
   * \return значение параметра
   */
  float getValue(unsigned short id, bool *ok = 0);

  /*!
   * \brief Метод присвоения значения параметру
   * Когда пытаемся записать значение, нам нужно проверить
   * 1. Уровень доступа к параметру
   * 2. Операции над параметром
   * 3. Минимальное и максимальное значение
   * \param id уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  unsigned char setValue(unsigned short id, float value);

  /*!
   * \brief Метод получения типа физической величины по id
   * \param id уникальный идентификатор параметра
   * \return типа физической величины
   */
  uint8_t getPhysic(unsigned short id);



  /*!
   * \brief Сохранение массива параметров на Flash
   */
  StatusType saveParameters();

  /*!
   * \brief Чтение массива параметров с Flash
   */
  StatusType readParameters();


protected:

  /*!
   * \brief Метод получения ID параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле ID
   */
  unsigned short getFieldId(unsigned short index);

  /*!
   * \brief Метод получения Access параметра по индексу в массиве параметров
   * \param Index
   * \return поле Access
   */
  unsigned char getFieldAccess(unsigned short index);

  /*!
   * \brief Метод получения Operation параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Operation
   */
  unsigned char getFieldOperation(unsigned short index);

  /*!
   * \brief Метод получения Physic параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Physic
   */
  unsigned char getFieldPhysic(unsigned short index);

  /*!
   * \brief Метод получения Validity параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Validity
   */
  unsigned char getFieldValidity(unsigned short index);

  /*!
   * \brief Метод получения Update параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Update
   */
  unsigned char getFieldUpdate(unsigned short index);

  /*!
   * \brief Метод получения Value параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Value
   */
  float getFieldValue(unsigned short index);

  /*!
   * \brief Метод получения Minimum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Minimum
   */
  float getFieldMinimum(unsigned short index);

  /*!
   * \brief Метод получения Maximum параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Maximum
   */
  float getFieldMaximum(unsigned short index);

  /*!
   * \brief Метод получения Default параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return поле Default
   */
  float getFieldDefault(unsigned short index);

  /*!
   * \brief Метод получения All полей параметра по индексу в массиве параметров
   * \param Index индекс параметра в массиве
   * \return All поля
   */
  parameter getFieldAll(unsigned short index);

  /*!
   * \brief Метод присвоения поля ID в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param ID присваемое значение
   */
  void setFieldID(unsigned short index, unsigned short id);

  /*!
   * \brief Метод присвоения поля Access в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Access присваемое значение
   */
  void setFieldAccess(unsigned short index, unsigned char access);

  /*!
   * \brief Метод присвоения поля Operation в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Operation присваемое значение
   */
  void setFieldOperation(unsigned short index, unsigned char operation);

  /*!
   * \brief Метод присвоения поля Physic в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Physic присваемое значение
   */
  void setFieldPhysic(unsigned short index, unsigned char physic);

  /*!
   * \brief Метод присвоения поля Validity в массиве параметров по индексу
   * \param Index индекс параметра в массиве
   * \param Validity присваемое значение
   */
  void setFieldValidity(unsigned short index, unsigned char validity);

  /*!
   * \brief Метод присвоения поля value в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param value присваемое значение
   */
  void setFieldValue(unsigned short index, float value);

  /*!
   * \brief Метод присвоения поля Min в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param min присваемое значение
   */
  void setFieldMin(unsigned short index, float min);

  /*!
   * \brief Метод присвоения поля Max в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param max присваемое значение
   */
  void setFieldMax(unsigned short index, float max);

  /*!
   * \brief Метод присвоения поля def в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param def присваемое значение
   */
  void setFieldDef(unsigned short index, float def);

  /*!
   * \brief Метод присвоения поля All в массиве параметров по индексу
   * \param index индекс параметра в массиве
   * \param param присваемое значение
   */
  void setFieldAll(unsigned short index, parameter param);

  float applyCoef(float value, float coef);

  float applyUnit(float value, int physic, int units);

  //! Начальный адрес массива параметров из списка enID
  uint32_t startAddrParams_;
  //! Указатель на массив параметров устройства
  parameter *parameters_;
  //! Количество параметров в массиве
  uint16_t countParameter_;

private:

};

#endif /* DEVICE_H_ */

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
#include "units.h"
#include "board.h"
#include "fram.h"
#include "log.h"

/*!
 * \brief Структура для обработки и хранения параметров в СУ
 */
struct parameter
{
  uint16_t id;        //! Уникальный идентификатор параметра заменяет название параметра равен
  uint8_t access;     //! Уровень доступа к параметру
  uint8_t operation;  //! Операции с параметром: чтение, запись, запись во время останова
  uint8_t physic;     //! Физический смысл
  float min;          //! Минимально допустимое значение для этого параметра.
  float max;          //! Максимально допустимое значение для этого параметра.
  float def;          //! Значение по умолчанию
  uint32_t discret;
  uint8_t validity;   //! Валидность параметра
  unTypeData value;   //! Значение параметра, для единообразия значения всех параметров будем
  uint8_t code;       //! Код события журнала событий, при изменениии параметра.
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
  virtual void init();

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
   * \brief setNewValue
   * \param id
   * \param value
   * \param eventType - тип события для фиксирования изменений в журнале
   */
  virtual uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  /*!
   * \brief setNewValue
   * \param id
   * \param value
   * \param eventType - тип события для фиксирования изменений в журнале
   */
  virtual uint8_t setNewValue(uint16_t id, uint32_t value, EventType eventType = AutoType);

  /*!
   * \brief setNewValue
   * \param id
   * \param value
   * \param eventType - тип события для фиксирования изменений в журнале
   */
  virtual uint8_t setNewValue(uint16_t id, int value, EventType eventType = AutoType);

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
   * \brief Метод получения значения параметра типа uint32_t по id
   * \param id уникальный идентификатор параметра
   * \param ok - действительное значение или нет
   * \return значение параметра
   */
  uint32_t getValueUint32(unsigned short id, bool *ok = 0);

  /*!
   * \brief Метод присвоения значения параметру
   * Когда пытаемся записать значение, нам нужно проверить
   * 1. Уровень доступа к параметру
   * 2. Операции над параметром
   * 3. Минимальное и максимальное значение
   * \param id уникальный идентификатор параметра
   * \param value присваемое значение
   * \param reason источник(первопричина) изменения параметра (по умолчанию авто)
   * \return 0 - значение присвоено или код ошибки
   */
  uint8_t setValue(uint16_t id, float value, EventType eventType = NoneType);
  uint8_t setValueForce(uint16_t id, float value, EventType eventType = NoneType);
  /*!
   * \brief Метод присвоения значения параметра типа uint32_t
   * \param id - уникальный идентификатор параметра
   * \param value - присваемое значение
   * \return 0 - значение присвоено или код ошибки
   */
  uint8_t setValue(uint16_t id, uint32_t value, EventType eventType = AutoType);

  /*!
   * \brief Метод присвоения значения параметра типа float переменным типа int
   * \param id - уникальный идентификатор параметра
   * \param value - присваемое значение
   * \return 0 - значение присвоено или код ошибки
   */
  uint8_t setValue(uint16_t id, int value, EventType eventType = AutoType);



  /*!
   * \brief Функция сброса значения параметра на значение по умолчанию
   * \param id - уникальный идентификатор параметра
   * \return
   */
  void resetValue(uint16_t id);

  /*!
   * \brief getMinimum
   * \param id
   * \return
   */
  float getMin(uint16_t id);

  /*!
   * \brief getMaximum
   * \param id
   * \return
   */
  float getMax(uint16_t id);

  /*!
   * \brief Метод получения значения параметра по умолчанию
   * \param id
   * \return
   */
  float getValueDef(uint16_t id);

  /*!
   * \brief Метод получения типа физической величины по id
   * \param id уникальный идентификатор параметра
   * \return типа физической величины
   */
  uint8_t getPhysic(unsigned short id);

  uint32_t getDiscret(unsigned short id);

  uint8_t getValidity(unsigned short id);

  void setValidity(uint16_t id, uint8_t validity);

  /*!
   * \brief setAccess
   * \param id
   * \param access
   */
  void setAccess(uint16_t id, uint8_t access);

  /*!
   * \brief setOperation
   * \param id
   * \param operation
   */
  void setOperation(uint16_t id, uint8_t operation);

  /*!
   * \brief Сохранение массива параметров на Flash
   */
  StatusType saveParameters();

  /*!
   * \brief Чтение массива параметров с Flash
   */
  StatusType readParameters();

  /*!
   * \brief Сохранить текущую конфигурацию в профиль
   * \param address - адрес на Flash с которого начинается запись конфигурации
   * \return
   */
  StatusType saveConfig(uint32_t address);

  /*!
   * \brief Загрузить конфигурацию из профиля
   * \param address - адрес на Flash с которого начинается чтение конфигурации
   * \return
   */
  StatusType loadConfig(uint32_t address);

  /*!
   * \brief Загрузка значений конфигурации из профиля в массив уставок по умолчанию
   * \param address - адрес на Flash с которого начинается чтение конфигурации
   * \param data - указатель на массив уставок по умолчанию
   * \return
   */
  StatusType loadConfigInProfileDefault(uint32_t address, float *data);

  /*!
   * \brief Функция проверки есть ли связь с утройством
   * \return
   */
  virtual bool isConnect();

  uint8_t setMin(uint16_t id, float min);

  uint8_t setMax(uint16_t id, float max);

  uint16_t getFieldDiscret(uint16_t index);

  void reactionToConnect(bool curConnect);

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
   * \brief  Метод получения value.uint32_t параметра по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return
   */
  uint32_t getFieldValueUint32(uint16_t index);

  /*!
   * \brief Функция получения поля "code" параметра, по индексу в массиве параметров
   * \param index - индекс параметра в массиве
   * \return
   */
  EventCode getFieldCode(uint16_t index);

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
  template<typename T> void setFieldValue(const uint16_t &index, const T &value);

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

  /*!
   * \brief Метод присвоения поля value, значению из поля def
   * \param index - индекс параметра в массиве
   */
  void setFieldValueDef(unsigned short index);

  float applyCoef(float value, float coef);

  float applyUnit(float value, int physic, int unit);

  //! Начальный адрес массива параметров из списка enID
  uint32_t startAddrParams_;
  //! Указатель на массив параметров устройства
  parameter *parameters_;
  //! Количество параметров в массиве
  uint16_t countParameters_;

  //!
  bool prevConnect_;

};

#endif /* DEVICE_H_ */

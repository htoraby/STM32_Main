#ifndef SCADA_H
#define SCADA_H

#include "board.h"
#include "define.h"
#include "idslist.h"
#include "units.h"
#include "log.h"

#include "mb.h"
#include "mbport.h"
#include "mb_slave_sng.h"

//! Структура для хранения полей параметра из карты Скады
struct ScadaParameter
{
  int16_t id;          //!< Уникальный идентификатор параметра
  uint16_t address;    //!< Адрес регистра в Скаде
  uint8_t operation;   //!< Операции с параметром
  uint8_t physic;      //!< Физическая величина параметра
  uint8_t unit;        //!< Единицы измерения параметра
  uint8_t typeData;    //!< Тип данных
  float coefficient;   //!< Коэффициент преобразования параметра
  float min;           //!< Минимальное значение параметра
  float max;           //!< Максимальное значение параметра
  uint8_t command;     //!< Команда читать или писать
  unTypeData value;    //!< Значение параметра
};

class Scada
{
public:
  /*!
   * \brief Список протоколов телемеханики
   */
  enum ScadaType {
    NoneType,           //!< Откл
    NovometType,        //!< Новомет
    Region30Type,       //!< Регион 3.0
    Region2000Type,     //!< Регион 2000
    TelescopType,       //!< Телескоп
    LukoilType,         //!< Лукойл
    KarakudukType,      //!< Каракудукмунай
    SurgutneftegasType, //!< Сургутнефтегаз
  };

  Scada();
  virtual ~Scada();

  /*!
   * \brief Основная задача обработки запросов от скады
   */
  void task();

  /*!
   * \brief Задача вычисления параметров уникальных для конкретной скады
   */
  virtual void calcParamsTask();

  /*!
   * \brief Функция чтения регистра
   * \param buffer - указатель на буффер для записи данных
   * \param address - начальный адрес
   * \param numRegs - количество регистров
   * \return
   */
  virtual eMBErrorCode readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);

  /*!
   * \brief Функция записи регистра
   * \param buffer - указатель на буффер полученных данных
   * \param address - начальный адрес
   * \param numRegs - количество регистров
   * \return
   */
  virtual eMBErrorCode writeReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);

  /*!
   * \brief Функция управления
   * \param buffer - указатель на буффер полученных данных
   * \param address - начальный адрес
   * \param numRegs - количество регистров
   * \return
   */
  virtual eMBErrorCode writeCoils(uint8_t *buffer, uint16_t address, uint16_t numCoils);

  /*!
   * \brief readInfo
   * \param buffer
   * \param address
   * \param len
   * \return
   */
  eMBErrorCode readInfo(uint8_t *buffer, uint16_t address, uint16_t *len);

  float delay() const { return delay_; }

  /*!
   * \brief Получение параметра массива по адресу
   * \param address
   * \return
   */
  ScadaParameter *parameter(uint16_t address);

  /*!
   * \brief Метод обработки значения параметра полученного от Скады
   * \param param
   * \return
   */
  virtual int setNewValue(ScadaParameter *param);

protected:
  /*!
   * \brief Метод получения размера типа переменной
   * \param typeData
   * \return
   */
  int sizeDataFromTypeData(uint8_t typeData);

  /*!
   * \brief Метод получения индекса в массиве параметров
   * \param address - адрес регистра + 1
   * \return
   */
  int getIndexAtAddress(int address);

  //! Указатель на массив параметров Скады
  ScadaParameter *scadaParameters_;
  //! Количество параметров в массиве
  uint16_t countParameters_;

private:
  //! Идентификатор задачи опроса MB
  osThreadId mbThreadId_;
  //! Идентификатор задачи пересчёта параметров Скады
  osThreadId calcParamsThreadId_;
  //! Задержка ответа
  float delay_;

};

#endif // SCADA_H

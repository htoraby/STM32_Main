#ifndef SCADA_H
#define SCADA_H

#include "board.h"
#include "define.h"
#include "idslist.h"
#include "units.h"
#include "log.h"

#include "mb.h"
#include "mbport.h"

//! Структура для хранения полей параметра из карты Скады
struct ScadaParameter
{
  uint16_t id;         //!< Уникальный идентификатор параметра
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
    NoneType,         //!< Откл
    NovometType,      //!< Новомет
    Region30Type,     //!< Регион 3.0
  };

  Scada();
  ~Scada();

  void task();
  virtual void calcParametersTask();

  eMBErrorCode readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);
  eMBErrorCode writeReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);
  eMBErrorCode writeCoils(uint8_t *buffer, uint16_t address, uint16_t numCoils);

  float delay() const { return delay_; }

protected:
  /*!
   * \brief Метод получения индекса в массиве параметров
   * \param address - адрес регистра
   * \return
   */
  int getIndexAtAddress(int address);

  int sizeDataFromTypeData(uint8_t typeData);

  //! Указатель на массив параметров Скады
  ScadaParameter *scadaParameters_;
  //! Количество параметров в массиве
  uint16_t countParameters_;

private:
  //! Идентификатор задачи опроса MB
  osThreadId mbThreadId_;
  //! Задержка ответа
  float delay_;

};

#endif // SCADA_H

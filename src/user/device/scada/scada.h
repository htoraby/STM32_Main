#ifndef SCADA_H
#define SCADA_H

#include "board.h"
#include "define.h"
#include "mb.h"
#include "mbport.h"

//! Структура для хранения полей параметра из карты Скады
struct ScadaParameter
{
  int id;              //!< Уникальный идентификатор параметра
  int address;         //!< Адрес регистра в Скаде
  int operation;       //!< Операции с параметром
  int physic;          //!< Физическая величина параметра
  int unit;            //!< Единицы измерения параметра
  int typeData;        //!< Тип данных
  float coefficient;   //!< Коэффициент преобразования параметра
  float min;           //!< Минимальное значение параметра
  float max;           //!< Максимально значение параметра
  float def;           //!< Значение по умолчанию
  int command;         //!< Команда читать или писать
  int validity;        //!< Флаг получено ли значение параметра
  unTypeData value;    //!< Значение, записываемое или считанное
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

  eMBErrorCode readReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);
  eMBErrorCode writeReg(uint8_t *buffer, uint16_t address, uint16_t numRegs);
  eMBErrorCode writeCoils(uint8_t *buffer, uint16_t address, uint16_t numCoils);

  float delay() const { return delay_; }

private:
  //! Идентификатор задачи опроса MB
  osThreadId mbThreadId_;
  //! Задержка ответа
  float delay_;

};

#endif // SCADA_H

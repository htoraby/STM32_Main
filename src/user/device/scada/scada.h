#ifndef SCADA_H
#define SCADA_H

#include "board.h"
#include "mb.h"
#include "mbport.h"

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

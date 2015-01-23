#ifndef ADC_EXT_H
#define ADC_EXT_H

#include "board.h"

#define ANALOG_IN_NUM 8

/*!
 \brief Список режимов аналогового входа: напряжение (мВ), ток (мкА)

*/
enum AnalogInModeType {
  AnalogInModeVoltage,  //!< В
  AnalogInModeCurrent,  //!< мА
};

typedef struct {
  uint32_t value;    //!< Измеренное значение
  uint32_t valueTmp; //!< Промежуточное накопленное значение измерений
  uint8_t mode;
} AnalogInputDef;

extern AnalogInputDef analogInExt[];

/*!
 \brief Инициализация модуля работы с внешним АЦП

*/
void adcExtInit();

/*!
 \brief Установка режима аналогового входа внешнего АЦП

 \param num - номер аналогового входа
 \param mode - режим @ref AnalogInModeType
*/
void setModeAnalogInExt(uint8_t num, uint8_t mode);

/*!
 \brief Получение значения с аналогового входа внешнего АЦП

 \return - возвращаемое значение (В или мА)
*/
float getValueAnalogInExt(uint8_t num);

#endif // ADC_EXT_H

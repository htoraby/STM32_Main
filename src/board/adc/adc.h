#ifndef ADC_H
#define ADC_H

#include "board.h"

/*!
 \brief Список ADC каналов

*/
typedef enum {
  adc1,
  adc2,
  adc3,
  adcMax,
} adcNum;

extern ADC_HandleTypeDef hadc[];

#ifdef __cplusplus

/*!
 \brief Инициализация ADC:
 Встроенный датчик температуры,
 контроль напряжения ядра CPU,
 контроль напряжения батарейки

 \param num - номер ADC
*/
void adcInit(adcNum num);

void adcStartDma();

StatusType getAnalogIn(uint32_t channel, uint32_t numSamples, uint32_t *value);

/*!
 \brief Получение температуры CPU с внутреннего датчика

 \param value - возвращаемое значение
 \return StatusType - ошибка или ок
*/
StatusType getCoreTemperature(float *value);

/*!
 \brief Получение напряжения батарейки

 \param value - возвращаемое значение
 \return StatusType - ошибка или ок
*/
StatusType getCoreVbattery(float *value);

/*!
 \brief Получение напряжения питания ядра CPU

 \param value - возвращаемое значение
 \return StatusType - ошибка или ок
*/
StatusType getCoreVref(float *value);

#endif

#endif // ADC_H

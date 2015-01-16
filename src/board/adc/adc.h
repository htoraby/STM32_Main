#ifndef ADC_H
#define ADC_H

#include "board.h"

//! Количество каналов: Ua, Ub, Uc
#define ADC_CNANNELS_NUM 3
//! 20*200 точек с каждого канала за 4 сек, 20 точек на полупериод (20мс)
#define ADC_POINTS_NUM 4000
//! Период измерения - получение по одной точки для каждого канала: 100 - 1мс
#define ADC_TIM_PERIOD 100

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
extern uint16_t adcData[];

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

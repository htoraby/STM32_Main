#ifndef ADC_H
#define ADC_H

#include "board.h"

//! Количество каналов: Ua, Ub, Uc
#define ADC_CNANNELS_NUM 3
//! Время записи одного архива (мс)
#define ARCHIVE_TIME 2000
//! Количество точек на период (20мс)
#define HC_POINTS_NUM 40
//! Количество точек с каждого канала за время ARCHIVE_TIME
#define ADC_POINTS_NUM ARCHIVE_TIME/20*HC_POINTS_NUM
//! Период измерения - получение по одной точки для каждого канала: 100 - 1мс
#define ADC_TIM_PERIOD 100*20/HC_POINTS_NUM

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

/*!
 \brief Запуск измерений каналов: Ua, Ub, Uc

*/
void adcStartDma();

StatusType getAnalogIn(uint32_t channel, uint32_t numSamples, uint32_t *value);

/*!
 \brief Копирование значений из временного буффера (DMA) в основной
 \param data - основной буффер
*/
void copyAdcData(uint16_t *data);

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

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "board.h"

/*!
 \brief Инициализация внешнего датчика температуры

 \return StatusType - ошибка или ок
*/
StatusType tempSensorInit();

/*!
 \brief Функция получения значения температуры с внешнего датчика

 \return float - значение температуры (точность 0,5 С)
*/
float tempSensorReadData();

#endif // TEMP_SENSOR_H

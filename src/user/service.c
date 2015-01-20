/*
 * service.c
 *
 *  Created on: 30.10.2014
 *      Author: trofimenko
 */

#include "service.h"

// Функция проверки вхождения значения в диапазон не включая границы
unsigned char checkRangeNoInclude(double value, double min, double max)
{
  // Если значение больше минимума
  if (value > min) {
    // Если значение меньше максимума
    if (value < max) {
      // Возвращаем что прошли проверку
      return 0;
    }
    else
      // Иначе возвращаем что больше максимума
      return 2;
  }
  else
    // Возвращаем что меньше минимума
    return 1;
}

// Функция проверки вхождения значения в диапазон включая границы
unsigned char checkRangeInclude(double value, double min, double max)
{
  // Если значение больше минимума
  if (value >= min) {
    // Если значение меньше максимума
    if (value <= max) {
      // Возвращаем что прошли проверку
      return 0;
    }
    else
      // Иначе возвращаем что больше максимума
      return 2;
  }
  else
    // Возвращаем что меньше минимума
    return 1;
}

// Функция проверки вхождения значения в диапазон
unsigned char checkRange(double value, double min, double max, unsigned char inc)
{
  // Если включая диапазон
  if (inc)
    return checkRangeInclude(value, min, max);
  // Иначе не включая диапазон
  else
    return checkRangeNoInclude(value, min, max);
}




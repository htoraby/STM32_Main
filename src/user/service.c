/*
 * service.c
 *
 *  Created on: 30.10.2014
 *      Author: trofimenko
 */

#include "service.h"

// Функция проверки вхождения значения в диапазон не включая границы
int checkRangeNoInclude(double Value, double Minimum, double Maximum)
{
  if ((Value > Minimum) && (Value < Maximum))
    return 1;
  else
    return 0;
}

// Функция проверки вхождения значения в диапазон включая границы
int checkRangeInclude(double Value, double Minimum, double Maximum)
{
  if ((Value >= Minimum) && (Value <= Maximum))
    return 1;
  else
    return 0;
}

// Функция проверки вхождения значения в диапазон
int checkRange(double Value, double Minimum, double Maximum, int Include)
{
  if (Include)
    return checkRangeInclude(Value, Minimum, Maximum);
  else
    return checkRangeNoInclude(Value, Minimum, Maximum);
}



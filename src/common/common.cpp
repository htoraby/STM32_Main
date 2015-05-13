#include "common.h"
#include "cmsis_os.h"

void *operator new(size_t size)
{
  void *p = pvPortMalloc(size);
  if (p == NULL) {
    // Ошибка: память не выделена
    asm("nop");
  }
  return p;
}

void operator delete(void *p)
{
  vPortFree(p);
}

uint16_t crc16_ibm(uint8_t *buf, uint8_t size, uint16_t crc)
{
  for (uint8_t byte = 0; byte < size; byte++) {
    crc = crc ^ buf[byte];
    for (uint8_t bit = 0; bit < 8; bit++) {
      uint8_t flag = crc & 0x0001;
      crc = crc >> 1;
      if (flag)
        crc = crc ^ 0xA001;
    }
  }
  return crc;
}

int checkRangeNoInclude(double value, double min, double max)
{
  // Если значение больше минимума
  if (value > min) {
    // Если значение меньше максимума
    if (value < max) {
      // Возвращаем что прошли проверку
      return 0;
    }
    else {
      // Иначе возвращаем что больше максимума
      return 3;
    }
  }
  else {
    // Возвращаем что меньше минимума
    return 2;
  }
}

int checkRangeInclude(double value, double min, double max)
{
  // Если значение больше минимума
  if (value >= min) {
    // Если значение меньше максимума
    if (value <= max) {
      // Возвращаем что прошли проверку
      return 0;
    }
    else {
      // Иначе возвращаем что больше максимума
      return 3;
    }
  }
  else {
    // Возвращаем что меньше минимума
    return 2;
  }
}

int checkRange(double value, double min, double max, bool inc)
{
  // Если включая диапазон
  if (inc)
    return checkRangeInclude(value, min, max);
  // Иначе не включая диапазон
  else
    return checkRangeNoInclude(value, min, max);
}


bool checkBit(uint32_t value, uint8_t bit)
{
  if (value & (1 << bit))
    return true;
  else
    return false;
}

float calcImbalance(float value1, float value2, float value3, int digit)
{
  float max_;
  float min_;
  float avarage;
  float imbalance;

  value1 = round(value1 * pow(10,digit));
  value2 = round(value2 * pow(10,digit));
  value3 = round(value3 * pow(10,digit));
  max_ = max(value1, max(value2, value3));
  min_ = min(value1, min(value2, value3));
  avarage = (value1 + value2 + value3)/3;
  if (avarage == 0)
    avarage = 1;
  imbalance = ((max_ - min_)/avarage)*100;
  if (imbalance < 0)
    imbalance = 0;
  return imbalance;
}


uint32_t getTimeToEnd(uint32_t period, uint32_t time)
{
  int32_t timeToEnd = period - time;
  if (timeToEnd >= 0)
    return timeToEnd;
  return 0;
}

float copySign(float value, float sign)
{
  float valueTmp;
  valueTmp = fabs(value);
  if (sign < 0)
    valueTmp = -valueTmp;
  return valueTmp;
}

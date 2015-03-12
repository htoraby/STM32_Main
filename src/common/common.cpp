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
      return 2;
    }
  }
  else {
    // Возвращаем что меньше минимума
    return 1;
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
      return 2;
    }
  }
  else {
    // Возвращаем что меньше минимума
    return 1;
  }
}

int checkRange(double value, double min, double max, uint8_t inc)
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

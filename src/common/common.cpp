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
  uint8_t byte = 0;
  uint8_t bit = 0;
  uint8_t flag;
  for (byte = 0; byte < size; byte++) {
    crc = crc ^ buf[byte];
    for (bit = 0; bit < 8; bit++) {
      flag = crc & 0x0001;
      crc = crc >> 1;
      if (flag)
        crc = crc ^ 0xA001;
    }
  }
  return crc;
}

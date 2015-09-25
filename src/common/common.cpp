#include "common.h"
#include "cmsis_os.h"
#include "user_main.h"

void *operator new(size_t size)
{
  void *p = pvPortMalloc(size);
  if (p == NULL) {
    logDebug.add(CriticalMsg, "Ошибка: память не выделена");
  }
  return p;
}

void operator delete(void *p)
{
  vPortFree(p);
}

uint16_t crc16_ibm(uint8_t *buf, uint32_t size, uint16_t crc)
{
  for (uint32_t byte = 0; byte < size; byte++) {
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

int checkRangeNoInclude(double value, double min, double max, uint16_t discret)
{
  uint32_t valueU32 = value * discret + 0.5;
  uint32_t minU32 = min * discret + 0.5;
  uint32_t maxU32 = max * discret + 0.5;
  // Если значение больше минимума
  if (valueU32 > minU32) {
    // Если значение меньше максимума
    if (valueU32 < maxU32) {
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

int checkRangeInclude(double value, double min, double max, uint16_t discret)
{
  uint32_t valueU32 = value * discret + 0.5;
  uint32_t minU32 = min * discret + 0.5;
  uint32_t maxU32 = max * discret + 0.5;
  // Если значение больше минимума
  if (valueU32 >= minU32) {
    // Если значение меньше максимума
    if (valueU32 <= maxU32) {
      // Возвращаем что прошли проверку
      return ok_r;
    }
    else {
      // Иначе возвращаем что больше максимума
      return err_max_r;
    }
  }
  else {
    // Возвращаем что меньше минимума
    return err_min_r;
  }
}

int checkRange(double value, double min, double max, bool inc, uint32_t discret)
{
  // Если включая диапазон
  if (inc)
    return checkRangeInclude(value, min, max, discret);
  // Иначе не включая диапазон
  else
    return checkRangeNoInclude(value, min, max, discret);
}


bool checkBit(uint32_t value, uint8_t bit)
{
  if (value & (1 << bit))
    return true;
  else
    return false;
}

uint32_t setOneToBit(uint32_t value, uint8_t bit)
{
  return value |= (1 << bit);
}

uint32_t setZeroToBit(uint32_t value, uint8_t bit)
{
  return value &= (~(1 << bit));
}

uint32_t setBit(uint32_t value, uint8_t bit, bool flag)
{
  if (flag)
    return setOneToBit(value, bit);
  else
    return setZeroToBit(value, bit);
}

float calcImbalance(float value1, float value2, float value3, int digit)
{
  float max_;
  float min_;
  float average;
  float imbalance;

  value1 = round(value1 * pow(10,digit));
  value2 = round(value2 * pow(10,digit));
  value3 = round(value3 * pow(10,digit));
  max_ = max(value1, max(value2, value3));
  min_ = min(value1, min(value2, value3));
  average = calcAverage3Values(value1, value2, value3);
  if (average == 0)
    average = 1;
  imbalance = ((max_ - min_)/average)*100;
  if (imbalance < 0)
    imbalance = 0;
  return imbalance;
}

float calcAverage3Values(float value1, float value2, float value3)
{
  return (value1 + value2 + value3)/3;
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

typedef struct ConvLetter {
  char win1251;
  int  unicode;
} Letter;

static Letter g_letters[] = {
  {0x82, 0x201A}, // SINGLE LOW-9 QUOTATION MARK
  {0x83, 0x0453}, // CYRILLIC SMALL LETTER GJE
  {0x84, 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
  {0x85, 0x2026}, // HORIZONTAL ELLIPSIS
  {0x86, 0x2020}, // DAGGER
  {0x87, 0x2021}, // DOUBLE DAGGER
  {0x88, 0x20AC}, // EURO SIGN
  {0x89, 0x2030}, // PER MILLE SIGN
  {0x8A, 0x0409}, // CYRILLIC CAPITAL LETTER LJE
  {0x8B, 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
  {0x8C, 0x040A}, // CYRILLIC CAPITAL LETTER NJE
  {0x8D, 0x040C}, // CYRILLIC CAPITAL LETTER KJE
  {0x8E, 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
  {0x8F, 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
  {0x90, 0x0452}, // CYRILLIC SMALL LETTER DJE
  {0x91, 0x2018}, // LEFT SINGLE QUOTATION MARK
  {0x92, 0x2019}, // RIGHT SINGLE QUOTATION MARK
  {0x93, 0x201C}, // LEFT DOUBLE QUOTATION MARK
  {0x94, 0x201D}, // RIGHT DOUBLE QUOTATION MARK
  {0x95, 0x2022}, // BULLET
  {0x96, 0x2013}, // EN DASH
  {0x97, 0x2014}, // EM DASH
  {0x99, 0x2122}, // TRADE MARK SIGN
  {0x9A, 0x0459}, // CYRILLIC SMALL LETTER LJE
  {0x9B, 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
  {0x9C, 0x045A}, // CYRILLIC SMALL LETTER NJE
  {0x9D, 0x045C}, // CYRILLIC SMALL LETTER KJE
  {0x9E, 0x045B}, // CYRILLIC SMALL LETTER TSHE
  {0x9F, 0x045F}, // CYRILLIC SMALL LETTER DZHE
  {0xA0, 0x00A0}, // NO-BREAK SPACE
  {0xA1, 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
  {0xA2, 0x045E}, // CYRILLIC SMALL LETTER SHORT U
  {0xA3, 0x0408}, // CYRILLIC CAPITAL LETTER JE
  {0xA4, 0x00A4}, // CURRENCY SIGN
  {0xA5, 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
  {0xA6, 0x00A6}, // BROKEN BAR
  {0xA7, 0x00A7}, // SECTION SIGN
  {0xA8, 0x0401}, // CYRILLIC CAPITAL LETTER IO
  {0xA9, 0x00A9}, // COPYRIGHT SIGN
  {0xAA, 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
  {0xAB, 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
  {0xAC, 0x00AC}, // NOT SIGN
  {0xAD, 0x00AD}, // SOFT HYPHEN
  {0xAE, 0x00AE}, // REGISTERED SIGN
  {0xAF, 0x0407}, // CYRILLIC CAPITAL LETTER YI
  {0xB0, 0x00B0}, // DEGREE SIGN
  {0xB1, 0x00B1}, // PLUS-MINUS SIGN
  {0xB2, 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xB3, 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xB4, 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
  {0xB5, 0x00B5}, // MICRO SIGN
  {0xB6, 0x00B6}, // PILCROW SIGN
  {0xB7, 0x00B7}, // MIDDLE DOT
  {0xB8, 0x0451}, // CYRILLIC SMALL LETTER IO
  {0xB9, 0x2116}, // NUMERO SIGN
  {0xBA, 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
  {0xBB, 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
  {0xBC, 0x0458}, // CYRILLIC SMALL LETTER JE
  {0xBD, 0x0405}, // CYRILLIC CAPITAL LETTER DZE
  {0xBE, 0x0455}, // CYRILLIC SMALL LETTER DZE
  {0xBF, 0x0457} // CYRILLIC SMALL LETTER YI
};

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)
{
  int i = 0;
  int j = 0;
  for(; i < (int)n && utf8[i] != 0; ++i) {
    char prefix = utf8[i];
    char suffix = utf8[i+1];
    if ((prefix & 0x80) == 0) {
      windows1251[j] = (char)prefix;
      ++j;
    } else if ((~prefix) & 0x20) {
      int first5bit = prefix & 0x1F;
      first5bit <<= 6;
      int sec6bit = suffix & 0x3F;
      int unicode_char = first5bit + sec6bit;


      if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
        windows1251[j] = (char)(unicode_char - 0x350);
      } else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
        windows1251[j] = (char)(unicode_char);
      } else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
        windows1251[j] = (char)(unicode_char - 0x382);
      } else {
        int count = sizeof(g_letters) / sizeof(Letter);
        for (int k = 0; k < count; ++k) {
          if (unicode_char == g_letters[k].unicode) {
            windows1251[j] = g_letters[k].win1251;
            goto NEXT_LETTER;
          }
        }
        // can't convert this char
        return 0;
      }
NEXT_LETTER:
      ++i;
      ++j;
    } else {
      // can't convert this chars
      return 0;
    }
  }
  windows1251[j] = 0;
  return 1;
}

unsigned char toBcd(unsigned char value)
{
  return ((value/10)<<4) + value%10;
}

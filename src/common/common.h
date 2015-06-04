#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <math.h>

enum StatusType {
  StatusOk,
  StatusError,
};



#ifdef __cplusplus

/*!
 * \brief Переопределение оператора new
 *
 * \param size - размер необходимой памяти
 * \return void * - указатель на выделенную область памяти
 */
void *operator new(size_t size);

/*!
 * \brief Переопределение оператора delete
 *
 * \param p - указатель на объект
 */
void operator delete(void *p);

/*!
 \brief Метод подсчёта контрольной суммы

 \param buf  - указатель на данные
 \param size - размер данных
 \param crc - начальное значение контрольной суммы (по умолчанию: 0xFFFF)
 \return uint16_t - контрольная сумма
*/
uint16_t crc16_ibm(uint8_t *buf, uint8_t size, uint16_t crc = 0xFFFF);

/*!
 * \brief Шаблон определения максимального числа
 *
 * \param a
 * \param b
 * \return T
 */
template<class T> inline const T& max(const T& a, const T& b)
{
  if (a > b) return(a);
  else return(b);
}

/*!
 * \brief Шаблон определения минимального числа
 *
 * \param a
 * \param b
 * \return T
 */
template<class T> inline const T& min(const T& a, const T& b)
{
  if (a < b) return(a);
  else return(b);
}

/*!
 * \brief Функция проверки вхождения значения в диапазон не включая границы
 *
 * \param a
 * \param min
 * \param max
 * \return
 */
int checkRangeNoInclude(double value, double min, double max);

/*!
 * \brief Функция проверки вхождения значения в диапазон включая границы
 *
 * \param value
 * \param min
 * \param max
 * \return
 */
int checkRangeInclude(double value, double min, double max);

/*!
 * \brief Функция проверки вхождения значения в диапазон
 *
 * \param value
 * \param min
 * \param max
 * \return
 */
int checkRange(double value, double min, double max, bool inc);

/*!
 * \brief Функция проверки бита
 * \param value значение в котором проверяется бит
 * \param bit номер бита слева направо от 0
 * \return
 */
bool checkBit(uint32_t value, uint8_t bit);

/*!
 * \brief Функция вычисления дисбаланса между 3 значениями по формуле СНГ
 * \param value1
 * \param value2
 * \param value3
 * \param digit
 * \return
 */
float calcImbalance(float value1, float value2, float value3, int digit);

/*!
 * \brief Функция вычисления среднего между 3 значениями
 * \param value1 - значение 1
 * \param value2 - значение 2
 * \param value3 - значение 3
 * \return среднее значение
 */
float calcAverage3Values(float value1, float value2, float value3);

/*!
 * \brief Функция получения времени до конца периода
 * \param period
 * \param time
 * \return значение в секундах
 */
uint32_t getTimeToEnd(uint32_t period, uint32_t time);

/*!
 * \brief Функция возвращает число value со знаком числа sign
 * \param value
 * \param sign
 * \return
 */
float copySign(float value, float sign);

#endif

#endif // COMMON_H

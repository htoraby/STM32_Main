#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <math.h>

enum StatusType {
  StatusOk,
  StatusError,
};

/*!
 * \brief Список ошибок Slave для отображения в GUI
 */
enum ErrorSlave {
  NoErr,                       //!< Нет ошибки
  NoConnectionUsbErr,          //!< Не подключен USB накопитель
  WriteFileUsbErr,             //!< Ошибка записи файла
  CloseFileUsbErr,             //!< Ошибка закрытия файла
  MkDirUsbErr,                 //!< Ошибка создания каталога
  OpenFileUsbErr,              //!< Ошибка открытия файла
  OpenDirUsbErr,               //!< Ошибка открытия каталога
  NotFoundFwUpdateErr,         //!< Файл прошивки не найден
  CrcFwUpdateErr,              //!< Ошибка CRC файла прошивки
  HeaderFwUpdateErr,           //!< Ошибка в заголовке файла прошивки
  SaveFwMasterUpdateErr,       //!< Ошибка сохранения файла прошивки GUI
  TimeoutMasterUpdateErr,      //!< Тайм-аут сохранения файла прошивки GUI
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
uint16_t crc16_ibm(uint8_t *buf, uint32_t size, uint16_t crc = 0xFFFF);

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
int checkRangeNoInclude(double value, double min, double max, uint16_t discret = 1);

/*!
 * \brief Функция проверки вхождения значения в диапазон включая границы
 *
 * \param value
 * \param min
 * \param max
 * \return
 */
int checkRangeInclude(double value, double min, double max, uint16_t discret = 1);

/*!
 * \brief Функция проверки вхождения значения в диапазон
 *
 * \param value
 * \param min
 * \param max
 * \return
 */
int checkRange(double value, double min, double max, bool inc, uint32_t discret = 1);

/*!
 * \brief Функция проверки бита
 * \param value значение в котором проверяется бит
 * \param bit номер бита слева направо от 0
 * \return
 */
bool checkBit(uint32_t value, uint8_t bit);

/*!
 * \brief setOneToBit
 * \param value
 * \param bit
 * \return
 */
uint32_t setOneToBit(uint32_t value, uint8_t bit);

/*!
 * \brief setZeroToBit
 * \param value
 * \param bit
 * \return
 */
uint32_t setZeroToBit(uint32_t value, uint8_t bit);

/*!
 * \brief setBit
 * \param value
 * \param bit
 * \param flag
 * \return
 */
uint32_t setBit(uint32_t value, uint8_t bit, bool flag);

/*!
 * \brief copyBit Функция копирования бита из одной переменной в другую
 * \param source переменная источник из которой копируется
 * \param sourceBit номер бит в источнике
 * \param target переменная в которую копируется бит
 * \param targetBit номер бита в цели
 */
void copyBit(uint32_t source, uint8_t sourceBit, uint32_t target, uint8_t targetBit);

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

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n);

unsigned char toBcd(unsigned char value);

#endif

#endif // COMMON_H

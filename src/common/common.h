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
  * \brief Адреса на FLASH
 */
typedef enum {
  //! FlashSpi1
  AddrUpdateHeader = 0x00800000,
  AddrUpdateFile1  = 0x00800100,
  AddrUpdateFile2  = 0x00C00000,
  AddrSaveConfig1  = 0x01000000,
  AddrSaveConfig2  = 0x01019000,
  AddrSaveConfig3  = 0x01032000,
  AddrSaveConfig4  = 0x0104B000,
  AddrSaveConfig5  = 0x01064000,
  AddrCompressLog  = 0x0107D000,                      //! Адрес начала сжатого архива станции (10 Мб)
  AddrAddNew       = 0x01A7D000,
} AddrFlashSpi;

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
  MiniLzoInitUsbErr,           //!< Ошибка инициализации архиватора MiniLZO
  MiniLzoUsbErr,               //!< Ошибка архиватора MiniLZO
  SetProfileVsdErr,            //!< Ошибка конфигурирования ЧРП
  SetVoltageTapOfErr,          //!< Неправильно выбрано напряжение отпайки ТМПН
  NotFoundConfigFileErr,       //!< Файл конфигурации не найден
  MultipleConfigFilesErr,      //!< Найдено несколько файлов конфигурации!\n Должен быть только один файл
  HeaderConfigFileErr,         //!< Ошибка в заголовке файла конфигурации
  CrcConfigFileErr,            //!< Ошибка CRC файла конфигурации
};

/*!
 * \brief Список языков интерфейса
 */
enum LanguageList {
  RuLanguage,
  EnLanguage,
  EsLanguage,
};

/*!
 * \brief Список профилей с уставками по умолчанию
 */
enum ProfileDefaultSetpoint {
  NovometDefaultSetpoint,     //!< Новомет
  Profile1DefaultSetpoint,    //!< Профиль 1
  Profile2DefaultSetpoint,    //!< Профиль 2
  Profile3DefaultSetpoint,    //!< Профиль 3
  Profile4DefaultSetpoint,    //!< Профиль 4
  Profile5DefaultSetpoint,    //!< Профиль 5
  RosneftDefaultSetpoint,     //!< Роснефть
};

/*!
 * \brief Список параметров для аналоговых входов
 */
enum AnalogInParamType {
  AnalogInParamUnit,
  AnalogInParamPressure,
  AnalogInParamTemp,
  AnalogInParamVibrAcceler,
  AnalogInParamVibrSpeed,
  AnalogInParamResistIsol,
  AnalogInParamConsumption,
};

/*!
 * \brief Список единиц измерения давления для аналоговых входов
 */
enum AiUnitsPressure {
  AiUnitsPressureMpa,
  AiUnitsPressureAtm,
  AiUnitsPressurePsi,
  AiUnitsPressureKgSm2,
  AiUnitsPressureBar,
};

/*!
 * \brief Список единиц измерения температуры для аналоговых входов
 */
enum AiUnitsTemp {
  AiUnitsTempC,
  AiUnitsTempF,
};

/*!
 * \brief Список единиц измерения виброускорения для аналоговых входов
 */
enum AiUnitsVibrationAcceler {
  AiUnitsVibrationG,
  AiUnitsVibrationMS2,
  AiUnitsVibrationMmS2,
};

/*!
 * \brief Список единиц измерения виброскорости для аналоговых входов
 */
enum AiUnitsVibrationSpeed {
  AiUnitsVibrationMS,
  AiUnitsVibrationMmS,
};

/*!
 * \brief Список единиц измерения сопротивления изоляции для аналоговых входов
 */
enum AiUnitsResistIsol {
  AiUnitsResistIsolKom,
};

/*!
 * \brief Список единиц измерения расхода для аналоговых входов
 */
enum AiUnitsConsumption {
  AiUnitsConsumptionM3Day,
  AiUnitsConsumptionM3Hour,
  AiUnitsConsumptionBblDay,
  AiUnitsConsumptionBblHour,
};

/*!
 * \brief The stConnectQuality struct
 */
struct stConnectQuality {
  float quality;
  float transmite;
  float resive;
  float trash;
  float crc;
  float error;
  float lost;
  float failure;
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
 * \brief Конвертация значения из формата STM
 * \param value
 * \param physic
 * \param unit
 * \return
 */
float convertFrom(float value, int physic, int unit);

/*!
 * \brief Конвертация значения в формат STM
 * \param value
 * \param physic
 * \param unit
 * \return
 */
float convertTo(float value, int physic, int unit);

/*!
 * \brief Функция возвращает число value со знаком числа sign
 * \param value
 * \param sign
 * \return
 */
float copySign(float value, float sign);

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n);

unsigned char toBcd(unsigned char value);

unsigned short bcdToDec(unsigned short value);
unsigned short decToBCD(unsigned short value);

#endif

#endif // COMMON_H

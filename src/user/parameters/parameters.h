#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "device.h"
#include "fram.h"
#include "parameters_default.h"

#define PARAMETERS_SIZE 21504*4
#define PARAMS_SAVE_TIME 60000 //!< Период сохранения параметров в миллисекундах

#pragma pack(1)

typedef struct {
  unsigned int size;
  unsigned short codeProduction;
  unsigned char codeEquip;
  unsigned char subCodeEquip;
  unsigned short version;
  unsigned int date;
} CFG_FILE_HEADER;

#pragma pack()

/*!
 * \brief Класс сохранения/чтения параметров в/из Flash
 *
 */
class Parameters
{
public:
  Parameters();
  ~Parameters();

  /*!
   * \brief Инициализация
   */
  void init();
  void task();
  void setDelayTask();

  /*!
   * \brief Запуск сохранения параметров из ОЗУ в Flash
   */
  void startSave();

  /*!
   * \brief Сохранение параметров из ОЗУ в Flash
   */
  void save();

  /*!
   * \brief Чтение параметров из Flash в ОЗУ
   */
  void read();

  /*!
   * \brief Метод чтения параметра по ID с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \return значение параметра
   */
  float get(unsigned short id);

  /*!
   * \brief Метод чтения параметра типа uint32_t по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \return значение параметра
   */
  uint32_t getU32(unsigned short id);

  /*!
   * \brief Метод записи параметра по ID с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   * \return 0 - значение присвоено или ошибка
   */
  int set(unsigned short id, float value, EventType eventType = AutoType);

  /*!
   * \brief Метод записи параметра типа uint32_t по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   * \return 0 - значение присвоено или ошибка
   */
  int set(unsigned short id, uint32_t value, EventType eventType = AutoType);

  /*!
   * \brief Метод записи параметра типа int по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   * \return 0 - значение присвоено или ошибка
   */
  int set(unsigned short id, int value, EventType eventType = AutoType);

  /*!
   * \brief Метод записи параметра по ID с постановкой в очередь
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   */
  void setDelay(uint16_t id, uint32_t value, EventType eventType);

  /*!
   * \brief Метод получения типа физической величины по ID
   * с определением необходимого массива
   * \param id уникальный идентификатор параметра
   * \return типа физической величины
   */
  uint8_t getPhysic(unsigned short id);

  uint8_t getValidity(unsigned short id);

  bool isValidity(uint16_t id);

  void setValidity(uint16_t id, uint8_t validity);

  float getMin(uint16_t id);

  uint8_t setMin(uint16_t id, float value);

  float getMax(uint16_t id);

  float getValueDef(uint16_t id);

  uint8_t setMax(uint16_t id, float value);

  void setAccess(uint16_t id, uint8_t access);

  void setOperation(uint16_t id, uint8_t operation);

  /*!
   * \brief checkZero Функция проверки на ноль
   * \param id уникальный идентификатор параметра
   * \param reset надо ли сбрасывать параметр
   * \param value значение отличное от значения по умолчанию на которое надо сбросить
   * \return
   */
  float checkZero(unsigned short id, bool reset, float value = 0);

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
   * \brief Сохрание текущих параметров в профиль
   */
  void saveConfig();

  /*!
   * \brief Загрузка параметров из профиля
   */
  void loadConfig();

  /*!
   * \brief Загрузка значений из профиля в массив уставок по умолчанию
   */
  void setProfileDefaultSetpoint();

  /*!
   * \brief Сброс всех уставок по умолчанию
   */
  void setAllDefault();

  /*!
   * \brief Сброс значения параметра по умолчанию
   * в зависимости от выбранного профиля
   * \param id - уникальный идентификатор параметра
   */
  void setDefault(uint16_t id);

private:
  /*!
   * \brief Сохранение конфигурации в профиль
   * \param profile
   */
  void saveConfigProfile(int profile);
  /*!
   * \brief Сохранение конфигурации на USB
   * \param profile
   */
  bool saveConfigUsb();
  /*!
   * \brief Получение пути файла
   */
  void getFilePath(char *path);
  /*!
   * \brief Загрузка конфигурации из профиля
   */
  void loadConfigProfile(int profile);
  /*!
   * \brief Загрузка конфигурации с USB
   */
  bool loadConfigUsb();
  /*!
   * \brief Поиск файла на USB носителе
   * \param fileName
   */
  void getConfigFile(char *fileName);

  //! Идентификатор семафора
  osSemaphoreId semaphoreId_;

  osMessageQId messageIdParams_;
  osMessageQId messageValueParams_;

  float profileDefaultParams_[COUNT_PARAMETERS_DEFAULT];
  bool isBanSaveConfig_;

};

#endif // PARAMETERS_H

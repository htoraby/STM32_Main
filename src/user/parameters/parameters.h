#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "device.h"
#include "fram.h"

#define PARAMS_SAVE_TIME 60000 //!< Период сохранения параметров в миллисекундах

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
   * \brief Метод получения типа физической величины по ID
   * с определением необходимого массива
   * \param id уникальный идентификатор параметра
   * \return типа физической величины
   */
  uint8_t getPhysic(unsigned short id);

  uint8_t getValidity(unsigned short id);

  void setValidity(uint16_t id, uint8_t validity);

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

private:
  //! Идентификатор семафора
  osSemaphoreId semaphoreId_;

};

#endif // PARAMETERS_H

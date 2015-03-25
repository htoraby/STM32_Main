#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "device.h"
#include "fram.h"

#define PARAMS_SAVE_TIME 10000 //!< Период сохранения параметров (60 секунд)

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
   * \brief Чтение параметров из Flash в ОЗУ
   */
  void read();

  /*!
   * \brief Метод чтения параметра по ID с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \return значение параметра
   */
  float getValue(unsigned short id);

  /*!
   * \brief Метод чтения параметра типа uint32_t по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \return значение параметра
   */
  uint32_t getValueUint32(unsigned short id);

  /*!
   * \brief Метод записи параметра по ID с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  int setValue(unsigned short id, float value);

  /*!
   * \brief Метод записи параметра типа uint32_t по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  int setValue(unsigned short id, uint32_t value);

  /*!
   * \brief Метод записи параметра типа int по ID
   * с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  int setValue(unsigned short id, int value);

  /*!
   * \brief Метод получения типа физической величины по ID
   * с определением необходимого массива
   * \param id уникальный идентификатор параметра
   * \return типа физической величины
   */
  uint8_t getPhysic(unsigned short id);

private:
  /*!
   * \brief Сохранение параметров из ОЗУ в Flash
   */
  void save();

  //! Идентификатор семафора
  osSemaphoreId semaphoreId_;

};

#endif // PARAMETERS_H

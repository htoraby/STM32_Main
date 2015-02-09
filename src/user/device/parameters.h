#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "device.h"
#include "fram.h"

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
   * \brief Метод записи параметра по ID с определением необходимого массива
   * \param id - уникальный идентификатор параметра
   * \param value присваемое значение
   * \return 0 - значение присвоено или ошибка
   */
  int setValue(unsigned short id, float value);

private:
  /*!
   * \brief Сохранение параметров из ОЗУ в Flash
   */
  void save();

  //! Идентификатор семафора
  osSemaphoreId semaphoreId_;

};

#endif // PARAMETERS_H

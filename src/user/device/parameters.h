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
   * \brief Запуск сохранения парамметров из ОЗУ в Flash
   */
  void startSave();

  /*!
   * \brief Чтение парамметров из Flash в ОЗУ
   */
  void read();

private:
  /*!
   * \brief Сохранение парамметров из ОЗУ в Flash
   */
  void save();

  //! Идентификатор семафора
  osSemaphoreId semaphoreId_;

};

#endif // PARAMETERS_H

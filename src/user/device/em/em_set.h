#ifndef EM_SET_H_
#define EM_SET_H_

#include "em.h"

/*!
 * \brief Класс счётчика электроэнергии СЕТ
 *
*/
class EmSet: public Em
{
public:
  EmSet();
  virtual ~EmSet();

  /*!
   * \brief Задача отправки/приёма данных в/от СЭ
  */
  void task();

private:
  /*!
   * \brief Метод отправки запросов
  */
  void sendRequest();

  /*!
   * \brief Метод обработки ответов на запросы
  */
  void receiveAnswer();

  /*!
   * \brief Сохранение полученных значений в массив параметров
  */
  void setParameter();

  /*!
   * \brief Сохранение полученных значений энергии в массив параметров
  */
  void setParamEnergy();

  /*!
   * \brief Сохранение полученных значений энергопотребления (за весь период)
   *  в массив параметров
  */
  void setParamEnergyConsumpAll();

  /*!
   * \brief Сохранение полученных значений энергопотребления (текущие сутки)
   * в массив параметров
  */
  void setParamEnergyConsumpCurDay();

  /*!
   * \brief Сохранение полученных значений энергопотребления (предыдущие сутки)
   * в массив параметров
  */
  void setParamEnergyConsumpPrevDay();

  /*!
   * \brief Сохранение полученных значений энергопотребления (текущий месяц)
   * в массив параметров
  */
  void setParamEnergyConsumpCurMonth();

  /*!
   * \brief Сохранение полученных значений энергопотребления (предыдущий месяц)
   * в массив параметров
  */
  void setParamEnergyConsumpPrevMonth();

  /*!
   * \brief Сохранение константы счётчика
  */
  void setConstantEm();

  /*!
   * \brief Подготовка записи коэффициента трансформации в СЭ
   * Если коэффициент СЭ не отличается от значения в массиве,
   * то пропускаем запись
   * return - номер следующего запроса
  */
  int prepareSendConstantCoefTrans();

  //! Идентификатор задачи
  osThreadId threadId_;

  //! Номер запроса
  int numberRequest_;
  //! Константа счётчика
  int constEm_;
  //! Константа коэффициента трансформации тока
  float constCoefTrans_;
  //! Количество принятых байт в пакете
  int sizePkt_;

};

#endif /* EM_SET_H_ */

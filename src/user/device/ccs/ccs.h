/*
 * classController.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef CCS_H_
#define CCS_H_

#include "device.h"

class Ccs: public Device
{
public:
  enum LedCondition {
    /// Горит красный светодиод
    LedConditionStop,
    /// Горит желтый светодиод
    LedConditionWaitApv,
    /// Горит желтый и зеленый светодиоды
    LedConditionDelay,
    /// Горит красный светодиод
    LedConditionBlock,
    /// Горит зеленый светодиод
    LedConditionRun,
    /// Мигает зеленый светодиод
    LedConditionRunning,
    /// Мигает зеленый светодиод
    LedConditionStopping
  };

  Ccs();
  virtual ~Ccs();

  /*!
   * \brief Инициализация
   */
  void init();

  /*!
   * \brief Основная задача обработки СУ
   */
  void mainTask();

  /*!
   * \brief Задача обработки очереди событий включения/мигания LED
   */
  void ledConditionTask();

  /*!
   * \brief Задача обработки переключения состояния ЧРП
   */
  void vsdConditionTask();

  void initParameters();

  /*!
   * \brief Проверка находится ли станция в стопе
   * \return
   */
  bool isStopCCS();

  /*!
   * \brief Проверка находится ли станция в работе
   * \return
   */
  bool isWorkCCS();

  /*!
   * \brief Проверка находится ли станция в ожидании
   * \return
   */
  bool isWaitCCS();

  /*!
   * \brief Проверка находится ли станция в блокировке
   * \return
   */
  bool isBlockCCS();

  /*!
   * \brief Проверка что КСУ в режиме АUTO
   * \return
   */
  bool isAutoControlMode();

  /*!
   * \brief getTime
   * Функция получения текущего времени в сек от 00:00:00 01.01.1970 года
   * \return Возвращает значение секунд
   */
  float getTime();

  /*!
   * \brief Метод обработки изменения состояния
   */
  void conditionChanged();

  /*!
   * \brief Метод проверки команды на запуск/останов
   */
  void cmdCheck();

private:
  /*!
   * \brief Метод добавления в очередь событий включения/мигания LED
   */
  void setLedCondition(LedCondition condition);

  //! Массив параметров устройства
  parameter parametersArray_[CCS_END - CCS_BEGIN];

  //! Очередь событий включения/мигания LED
  osMessageQId ledMessage_;
  //! Предыдущие состояние станции
  int conditionOld;
  //! Предыдущие состояние VSD
  int vsdConditionOld;

};

#endif /* CCS_H_ */

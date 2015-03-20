/*
 * classController.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef CCS_H_
#define CCS_H_

#include "device.h"
#include "log.h"

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
    /// Мигает желтый светодиод
    LedConditionWaitRun,
    /// Мигает зеленый светодиод
    LedConditionRunning,
    /// Мигает зеленый светодиод
    LedConditionStopping
  };

  Ccs();
  virtual ~Ccs();

  void init();
  void initTask();

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
   * \brief Метод запуска двигателя
   * \param type - Вид события
   */
  void start(EventType type = AutoType);

  /*!
   * \brief Метод останова двигателя
   * \param type - Вид события
   */
  void stop(EventType type = AutoType);

  /*!
   * \brief Проверка находится ли станция в стопе
   * \return
   */
  bool isStopMotor();

  /*!
   * \brief Проверка находится ли станция в работе состояние работа, это
   * все состояния кроме состояний "Стоп" и "Ожидания АПВ"
   * \return
   */
  bool isWorkMotor();

  /*!
   * \brief Проверка находится ли станция в ожидании
   * \return
   */
  bool isDelay();

  /*!
   * \brief setDelayCCS
   */
  void setDelay();

  /*!
   * \brief resetDelayCCS
   */
  void resetDelay();

  /*!
   * \brief isRestartCCS
   * \return
   */
  bool isRestart();

  /*!
   * \brief setRestartCCS
   */
  void setRestart();

  /*!
   * \brief resetRestartCCS
   */
  void resetRestart();

  /*!
   * \brief Проверка находится ли станция в блокировке
   * \return 0 - Не находится; 1 - находится
   */
  bool isBlock();

  /*!
   * \brief Установка флага блокировки
   */
  void setBlock();

  /*!
   * \brief Сброс флага блокировки
   */
  void resetBlock();

  /*!
   * \brief isPrevent
   * \return
   */
  bool isPrevent();

  /*!
   * \brief Проверка что КСУ в режиме АUTO
   * \return
   */
  bool isAutoMode();

  /*!
   * \brief isManualMode
   * \return
   */
  bool isManualMode();

  /*!
   * \brief isStopMode
   * \return
   */
  bool isStopMode();

  /*!
   * \brief Проверка что КСУ в режиме "Программа"
   * \return true - да, false - нет
   */
  bool isProgramMode();

  /*!
   * \brief getTime
   * Функция получения текущего времени в сек от 00:00:00 01.01.1970 года
   * \return Возвращает значение секунд
   */
  float getTime();

private:
  /*!
   * \brief Метод добавления в очередь событий включения/мигания LED
   */
  void setLedCondition(LedCondition condition);

  /*!
   * \brief Метод обработки изменения состояния
   */
  void conditionChanged();

  /*!
   * \brief calcCondition
   */
  void calcCondition();

  /*!
   * \brief Метод проверки команды на запуск/останов
   */
  void checkCmd();

  /*!
   * \brief Метод проверки возможности пуска
   * \return
   */
  bool checkCanStart();

  /*!
   * \brief Метод проверки возможности останова
   * \return
   */
  bool checkCanStop();

  //! Массив параметров устройства
  parameter parametersArray_[CCS_END - CCS_BEGIN];

  //! Очередь событий включения/мигания LED
  osMessageQId ledMessage_;
  //! Предыдущие состояние станции
  int conditionOld;
  int flagOld_;

};

#endif /* CCS_H_ */

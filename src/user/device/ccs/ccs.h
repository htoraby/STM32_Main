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
  /*!
   * \brief Список состояний светодиодов
   */
  enum LedCondition {
    OnRedLed,                   //!< Горит красный
    OnRedOnYellowLed,           //!< Горит красный и горит желтый
    OnGreenToogleYellowLed,     //!< Горит зеленый и мигает желтый
    ToogleGreenToogleYellowLed, //!< Мигает зеленый и мигает желтый
    ToogleRedLed,               //!< Мигает красный
    ToogleGreenToogleRedLed,    //!< Мигают зеленый и красный
    OnGreenLed,                 //!< Горит зеленый
    ToogleYellowLed,            //!< Мигает желтый
    ToogleGreenLed,             //!< Мигает зеленый
  };

  /*!
   * \brief Список надписей о состоянии СУ
   */
  enum GeneralCondition {
    GeneralConditionStop,       //!< Стоп
    GeneralConditionStopping,   //!< Торможение
    GeneralConditionRunning,    //!< Запуск
    GeneralConditionRun,        //!< Работа
    GeneralConditionDelay,      //!< Задержка срабатывания
    GeneralConditionRestart,    //!< Ожидание АПВ
    GeneralConditionBlock       //!< Блокировка
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
   * \param reason - Причина запуска
   */
  void start(LastReasonRun reason);

  /*!
   * \brief Метод останова двигателя
   * \param reason - Причина остановова
   */
  void stop(LastReasonStop reason);

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
   * \brief Проверка что КСУ в режиме "Программа"
   * \return true - да, false - нет
   */
  bool isProgramMode();

  /*!
   * \brief Функция получения текущего времени
   * в сек от 00:00:00 01.01.1970 года
   * \return значение в секундах
   */
  uint32_t getTime();

  /*!
   * \brief Функция получения прошедщего времени относительно текущего времени
   * \param time - фиксированное время
   * \return значение в секундах
   */
  uint32_t getSecFromCurTime(uint32_t time);

  /*!
   * \brief Функция получения прошедщего времени относительно текущего времени
   * \param timeId - Id фиксированного времени
   * \return значение в секундах
   */
  uint32_t getSecFromCurTime(enID timeId);

  /*!
   * \brief calcMotorCurrentPhase1
   */
  void calcMotorCurrentPhase1();

  /*!
   * \brief calcMotorCurrentPhase2
   */
  void calcMotorCurrentPhase2();

  /*!
   * \brief calcMotorCurrentPhase3
   */
  void calcMotorCurrentPhase3();

  /*!
   * \brief calcMotorCurrentAvarage
   */
  void calcMotorCurrentAvarage();

  /*!
   * \brief calcMotorCurrentPhase
   * \param vsdOutCurrent
   * \param coefCorrect
   * \param motorCurrent
   */
  void calcMotorCurrentPhase(uint16_t vsdOutCurrent, uint16_t coefCorrect, uint16_t motorCurrent);

  /*!
   * \brief calcMotorCurrentImbalance
   */
  void calcMotorCurrentImbalance();

  /*!
   * \brief calcVoltageImbalanceIn
   */
  void calcVoltageImbalanceIn();

private:
  /*!
   * \brief Метод добавления в очередь событий включения/мигания LED
   */
  void setLedCondition(LedCondition condition);

  /*!
   * \brief Метод обработки изменения состояния
   */
  void changedCondition();

  /*!
   * \brief Метод обработки изменения режима работы
   */
  void changedWorkMode();

  /*!
   * \brief Метод подсчёта времени работы и останова
   */
  void calcTime();

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

  /*!
   * \brief calcParameters
   */
  void calcParameters();

  /*!
   * \brief calcCoefTransformation
   */
  void calcCoefTransformation();

  //! Массив параметров устройства
  parameter parametersArray_[CCS_END - CCS_BEGIN];

  //! Очередь событий включения/мигания LED
  osMessageQId ledMessage_;
  //! Предыдущие состояние станции
  int conditionOld_;
  //! Предыдущие значение флага (задержка, АПВ, блокировка)
  int flagOld_;
  //! Предыдущий режим работы
  int workModeOld_;

};

#endif /* CCS_H_ */

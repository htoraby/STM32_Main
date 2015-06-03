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
    OnRedToogleYellowLed,            //!< Мигает желтый
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

  /*!
   * \brief Список команд пуска от контроллера визуализации
   */
  enum CmdStart {
    CmdStartNone,       //!< Нет запуска
    CmdStartOperator,   //!< Запуск - оператор
    CmdStartRemote,     //!< Запуск - дистанционный
  };

  /*!
   * \brief Список команд останова от контроллера визуализации
   */
  enum CmdStop {
    CmdStopNone,       //!< Нет останова
    CmdStopOperator,   //!< Останов - оператор
    CmdStopRemote,     //!< Останов - дистанционный
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
   * \brief Функция вычисления коэффициента трансформации трансформатора
   * \param voltTapOff - напряжение отпайки
   * \param voltInput - входной напряжение трансформатора
   * \return
   */
  float calcTransCoef(float voltTapOff, float voltInput);

  /*!
   * \brief Функция вычисления тока на фазе двигателя
   * \param vsdCurOut
   * \param coefCorrect
   * \param coefTrans
   * \return
   */
  float calcMotorCurrentPhase(float vsdCurOut,
                              float coefCorrect,
                              float coefTrans);
  /*!
   * \brief Функция вычисления падения напряжения на фильтре
   * \param outFilter - наличие выходного фильтра
   * \param inductFilter - индуктивность фильтра
   * \param current - расчёт для этого значения тока
   * \param freq - расчет для этого значения частоты
   * \param coefTrans - коэффициент трансформатора
   * \return
   */
  float calcDropVoltageFilter(float outFilter,
                              float inductFilter,
                              float current,
                              float freq,
                              float coefTrans);

  /*!
   * \brief Функция вычисления падения напряжения на кабельной линии
   * \param lenght - длина кабельной линни
   * \param cross - сечение кабеля
   * \param current - ток
   * \return
   */
  float calcDropVoltageCable(float lenght, float cross, float current);

  /*!
   * \brief Функция вычисления напряжения на фазе двигателя
   * \param vsdVoltOut - выходное напряжение ПЧ
   * \param dropVoltFilter - падение напряжения на фильтре
   * \param dropVoltCable - падение напряжения на кабеле
   * \param coefTrans - коэффициент транформации ТМПН
   * \return
   */
  float calcMotorVoltagePhase(float vsdVoltOut,
                              float dropVoltFilter,
                              float dropVoltCable,
                              float coefTrans);

  /*!
   * \brief calcMotorCurrentImbalance
   */
  void calcMotorCurrentImbalance();

  float calcMotorSpeed(float mtrType, float freq);

  float calcMotorCos(float actPwr, float fullPwr);



  /*!
   * \brief Функция вычисления индуктивности двигателя по полному сопротивлению фаз двигателя
   * \param resist сопротивление
   * \return
   */
  float calcMotorInductFromResistPhase(float resist);

  /*!
   * \brief Функция вычисления полного сопротивления фаз двигателя по индуктивности
   * \param induct
   * \return
   */
  float calcMotorResistPhaseFromInduct(float induct);

  /*!
   * \brief Функция вычисляет и сохраняет в регистр общую индуктивность системы
   */
  void calcSystemInduct();

  /*!
   * \brief calcVoltageImbalanceIn
   */
  void calcVoltageImbalanceIn();

  /*!
   * \brief Функция вычисления расчётных параметров
   */
  void calcParameters();

  float calcMotorLoad(float mtrCur, float mtrCos, float nomMtrCur, float nomMtrCos);

  uint8_t setNewValue(uint16_t id, float value);

  uint8_t setNewValue(uint16_t id, uint32_t value);

  uint8_t setNewValue(uint16_t id, int value);

  /*!
   * \brief Функция пересчёта периода одного шага
   * для режима "Программного изменения частоты"
   */
  void calcRegimeChangeFreqPeriodOneStep();

  /*!
   * \brief Контроль питания и отключение ИБП по таймауту
   */
  void controlPower();

  /*!
   * \brief Метод проверки отключения питания платы
   * \return
   */
  bool isPowerOff() const {
    return powerOffFlag_;
  }

  /*!
   * \brief Функция проверки связи с устройствами
   */
  void checkConnectDevice();

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
   * \brief Метод проверки команды на запуск/останов
   * от контроллера визуализации
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
   * \brief Метод подсчёта времени работы и останова
   */
  void calcTime();

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

  //! Флаг потери питания
  bool powerOffFlag_;
  //! Тайаут отключения питания ИБП
  int powerOffTimeout_;


};

#endif /* CCS_H_ */

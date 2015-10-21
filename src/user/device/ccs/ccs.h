/*
 * classController.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef CCS_H_
#define CCS_H_

#include <math.h>

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
   * \brief Задача обработки вспомогательных команд СУ
   */
  void toolsTask();

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
   * \brief Функция вычисления расчётных параметров
   */
  void calcParametersTask();

  /*!
   * \brief Функция вычисления коэффициента трансформации трансформатора
   * \return
   */
  float calcTransCoef();

  /*!
   * \brief Функция вычисления тока на фазе двигателя
   * \param vsdCurOut - ток фазы на выходе ЧРП с учётом коэффициента коррекции
   * \return ток на фазе двигателя
   */
  float calcMotorCurrentPhase(float vsdCurOut);
  float calcMotorCurrentPhase1();
  float calcMotorCurrentPhase2();
  float calcMotorCurrentPhase3();
  float calcMotorCurrentAverage();
  float calcMotorCurrentImbalance();

  /*!
   * \brief Функция вычисления падения напряжения на фильтре
   * \param current - расчёт для этого значения тока
   * \return величину падения напряжения на фильтре
   */
  float calcDropVoltageFilter(float current, float freq = -1, float coefTrans = -1);

  /*!
   * \brief Функция вычисления падения напряжения на кабельной линии
   * \param current - ток
   * \return величину падения напряжения в кабельной линии
   */
  float calcDropVoltageCable(float current);

  /*!
   * \brief Функция вычисления напряжения на фазе двигателя
   * \param dropVoltFilter - падение напряжения на фильтре
   * \param dropVoltCable - падение напряжения на кабеле
   * \return напряжение фазы двигателя
   */
  float calcMotorVoltagePhase(float dropVoltFilter, float dropVoltCable);
  float calcMotorVoltagePhase1();
  float calcMotorVoltagePhase2();
  float calcMotorVoltagePhase3();

  /*!
   * \brief Функция вычисления дисбаланса напряжения двигателя
   * \return
   */
  float calcMotorVoltageImbalance();

  /*!
   * \brief Функция вычисления текущей скорости вращения двигателя
   * \return Скорость вращения
   */
  float calcMotorSpeed();

  /*!
   * \brief Функция вычисления текущего коэффициента мощности
   * \return Текущий коэффициент мощности
   */
  float calcMotorCos();

  /*!
   * \brief Функция вычисления загрузки двигателя
   * \return
   */
  float calcMotorLoad();

  /*!
   * \brief Функция вычисления корректированного значения входного напряжения
   * \return
   */
  float calcInputVoltagePhase1();
  float calcInputVoltagePhase2();
  float calcInputVoltagePhase3();
  float calcInputVoltagePhase12();
  float calcInputVoltagePhase23();
  float calcInputVoltagePhase31();
  float calcInputVoltageImbalance();

  /*!
   * \brief Функция вычисления дисбаланса входных токов
   * \return
   */
  float calcInputCurrentImbalance();

  /*!
   * \brief Функция вычисления индуктивности двигателя по полному сопротивлению фаз двигателя
   * \return
   */
  float calcMotorInductFromResistPhase();

  /*!
   * \brief Функция вычисления полного сопротивления фаз двигателя по индуктивности
   * \return
   */
  float calcMotorResistPhaseFromInduct();

  /*!
   * \brief Функция вычисляет и сохраняет в регистр общую индуктивность системы
   */
  float calcSystemInduct();

  /*!
   * \brief Функция вычисления рекомендуемой отпайки ТМПН
   * \return
   */
  float calcTransRecommendedTapOff();

  /*!
   * \brief calcResistanceIsolation
   * \return
   */
  float calcResistanceIsolation();


  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  uint8_t setNewValue(uint16_t id, uint32_t value, EventType eventType = AutoType);

  uint8_t setNewValue(uint16_t id, int value, EventType eventType = AutoType);

  /*!
   * \brief Функция пересчёта периода одного шага
   * для режима "Программного изменения частоты"
   */
  void calcRegimeChangeFreqPeriodOneStep();

  /*!
   * \brief calcRegimeRun
   */
  void calcRegimeRun();

  /*!
   * \brief Функция вычисления входных напряжений с АЦП
   */
  void calcInputVoltageFromAdc();

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

  /*!
   * \brief Функция начала перезагрузки
   */
  void startReboot();

  /*!
   * \brief setFreq
   */
  void setFreq(float value);

  int calcUfCharacteristic();

  /*!
   * \brief Метод задания максимального значения параметра "Максимальная рабочая частота"
   */
  void setMaxBaseFrequency();

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
   * \brief Метод запуска по команде от контроллера визуализации
   */
  void cmdStart(int value);

  /*!
   * \brief Метод останова по команде от контроллера визуализации
   */
  void cmdStop(int value);

  void cmdProtSupplyOvervoltageSetpointReset();
  void cmdProtSupplyUndervoltageSetpointReset();
  void cmdProtSupplyImbalanceVoltageSetpointReset();

  void cmdProtMotorOverloadSetpointReset();
  void cmdProtMotorUnderloadSetpointReset();
  void cmdProtMotorImbalanceCurrentSetpointReset();
  void cmdProtMotorAsyncModeSetpointReset();

  void cmdProtDhsPressureIntakeSetpointReset();
  void cmdProtDhsTemperatureMotorSetpointReset();
  void cmdProtDhsResistanceSetpointReset();

  void cmdProtOtherHardwareVsdSetpointReset();
  void cmdProtOtherVsdNoConnectSetpointReset();

  /*!
   * \brief calcCounterStop
   * \param reason
   */
  void calcCountersStop(float reason);

  /*!
   * \brief calcCountersRun
   * \param reason
   */
  void calcCountersRun(float reason);

  /*!
   * \brief cmdCounterAllReset
   */
  void cmdCountersAllReset();

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

  float checkTypeStop();

  /*!
   * \brief Метод подсчёта времени работы и останова
   */
  void calcTime();

  /*!
   * \brief Перезагрузка контроллера
   */
  void reboot();

  /*!
   * \brief Функция запуска обновления ПО
   */
  void startUpdateSoftware();

  /*!
   * \brief Функция обновления ПО
   */
  void updateSoftware();

  /*!
   * \brief Проверка подключения мастера
   *  Если нет связи - перезагрузка мастера
   */
  void checkConnectMaster();

  void setCmd(uint16_t id);
  void resetCmd(uint16_t id);

  /*!
   * \brief Получение значений с дискретных входов и сохранение в параметрах
   */
  void calcDigitalInput();

  /*!
   * \brief Получение значений с аналоговых входов и сохранение в параметрах
   */
  void calcAnalogInput();

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

  //! Таймер задержки проверки подключения устройств
  int checkConnectDeviceTimer_;

  osThreadId ccsMainTaskId_;

  //! Идентификатор семафора перезапуска платы
  osSemaphoreId rebootSemaphoreId_;
  //! Идентификатор семафора обновления ПО
  osSemaphoreId updateSemaphoreId_;
  //! Идентификатор семафора переинициализации скады
  osSemaphoreId scadaSemaphoreId_;

  bool isConnectMaster_;

  int countPhaseRotation_;

};

#endif /* CCS_H_ */

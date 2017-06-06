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

#define RESTART_TIME_MAX 100

struct stCalcAsync                          // Структура для вычисления параметров по модели
{
  float UVSD;                               // Выходное напряжение ЧРП
  float Un;                                 // Напряжение UVSD * sin(PhiVSD)
  float ULn;                                //
  float dUn;
  float Ut;
  float URn;
  float dUt;
  float Usu[3];                  // íàïðÿæåíèå íà âûõîäå ÑÓ
  float ULf;
  float dFi;                     //
  float FiL;                     // ñäâèã íàïðÿæåíèÿ Usu îòíîñèòåëüíî íàïðÿæåíèÿ UVSD
  float IVSD[3];                 // òîê íà âûõîäå Ï× (ïî-ôàçíî)
  float Cf;
  float Isf;
  float In[3];
  float dIn;
  float It;
  float IH[3];
  float ULped[3];
  float Uped[3];                 // íàïðÿæåíèå íà ÏÝÄ
  float FiH;
  float dFiH;
  float LH;                      // èíäóêòèâíîñòü ñèñòåìû
  float Rn;
  float Psi;                     // óãîë ìåæäó òîêîì íàãðóçêè è ÝÄÑ
  float EMF[3];
  float EMFaver;
  float EMFModel;                // ÝÄÑ ïî ìîäåëè
  bool  flEMF;                   // ôëàã, óêàçûâàþùèé íà êîððåêòíîñòü ðàñ÷¸òà ÝÄÑ
};

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

  bool isParametersControl() { return isParametersControl_; }

  /*!
   * \brief Метод запуска двигателя
   * \param reason - Причина запуска
   */
  void start(LastReasonRun reason, bool force = false);

  /*!
   * \brief Метод останова двигателя
   * \param reason - Причина остановова
   */
  void stop(LastReasonStop reason);

  /*!
   * \brief Метод перехода в состояние "Работа" при запущеном ЧРП
   */
  void syncStart();

  /*!
   * \brief Метод перехода в состояние "Стоп" при остановленом ЧРП
   */
  void syncStop();

  /*!
   * \brief Проверка находится ли станция в стопе
   * \return
   */
  bool isBreakOrStopMotor();

  /*!
   * \brief isStopMotor
   * \return
   */
  bool isStopMotor();

  float isAlarmStop();

  /*!
   * \brief Проверка находится ли станция в работе состояние работа, это
   * все состояния кроме состояний "Стоп" и "Ожидания АПВ"
   * \return
   */
  bool isRunOrWorkMotor();

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
   * \brief resetBlockDevice метод сброса блокировки устройства
   */
  void resetBlockDevice();

  /*!
   * \brief checkStartDevice Метод прослойка проверки запуска от различных устройтв пуска
   * \return
   */
  bool checkStartDevice();

  /*!
   * \brief checkStopDevice метод проверки останова от различных устройств
   * \return
   */
  bool checkStopDevice();

  /*!
   * \brief stopDevice метод вызова функции останова управляющего устройства
   * \return
   */
  int stopDevice();

  /*!
   * \brief startDeviceметод вызова функции запуска управляющего устройства
   * \return
   */
  int startDevice(bool init);

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
  float applyCoefTransForCurrent(float vsdCurOut);
  float calcMotorCurrentPhase1();
  float calcMotorCurrentPhase2();
  float calcMotorCurrentPhase3();

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
   * \brief calcPowerMotor
   */
  void calcPowerMotor();

  void calcAsyncMotor();

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
  float calcMotorActivePower();

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


  /*!
   * \brief Функция вычисления корректированного значения входного тока
   * \return
   */
  float calcInputCurrentPhase1();
  float calcInputCurrentPhase2();
  float calcInputCurrentPhase3();
  float calcInputCurrentImbalance();

  /*!
   * \brief calcCurrentParam
   */
  void calcCurrentParam();

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
  void calcResistanceIsolation();


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
   * \param isSaveParameters - флаг сохранения параметров перед перезагрузкой
   */
  void startReboot(bool isSaveParameters = true);

  /*!
   * \brief setFreq
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   */
  int setFreq(float value, EventType eventType = NoneType, bool setPoint = true);

  int calcUfCharacteristic();

  /*!
   * \brief Метод задания максимального значения параметра "Максимальная рабочая частота"
   * Задаётся в зависимости от типа двигателя и профиля
   */
  void setMaxBaseFrequency();



  /*!
   * \brief Функция записи кода ошибки Slave в параметр для передачи в Master
   * \param error - код ошибки
   */
  void setError(int error);

  void intRestartCount();
  int getRestartCount();
  uint32_t getRestartTime();
  void incRestartCount();
  void decRestartCount();
  void resetRestartCount();

private:

  //! Расчётные параметры
  /*!
   * \brief Получение значений с цифровых входов, сохранение и обработка
   */
  void calcDigitalInputs();

  /*!
   * \brief calcTest
   */
  void calcTest();

  /*!
   * \brief Функция вычисления входных напряжений с АЦП
   */
  void calcInputVoltageFromAdc();

  /*!
   * \brief calcInputVoltageImbalance
   * \return
   */
  float calcInputVoltageImbalance();

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
  void cmdProtSupplyImbalanceCurrentSetpointReset();
  void cmdProtSupplySetpointReset();

  void cmdProtMotorOverloadSetpointReset();
  void cmdProtMotorCurrentSetpointReset();
  void cmdProtMotorUnderloadSetpointReset();
  void cmdProtMotorImbalanceCurrentSetpointReset();
  void cmdProtMotorAsyncModeSetpointReset();
  void cmdProtMotorOutOfSyncSetpointReset();
  void cmdProtMotorSetpointReset();

  void cmdProtDhsPressureIntakeSetpointReset();
  void cmdProtDhsPressureDischargeSetpointReset();
  void cmdProtDhsTemperatureMotorSetpointReset();
  void cmdProtDhsResistanceSetpointReset();
  void cmdProtDhsVibrationSetpointReset();
  void cmdProtDhsFlowDischargeSetpointReset();
  void cmdProtDhsSetpointReset();

  void cmdProtOvernumberOfStartSetpointReset();
  void cmdProtOtherHardwareVsdSetpointReset();
  void cmdProtOtherVsdNoConnectSetpointReset();
  void cmdProtOtherOverheatInputFilterSetpointReset();
  void cmdProtOtherSetpointReset();

  void cmdProtDigitalInput1SetpointReset();
  void cmdProtDigitalInput2SetpointReset();
  void cmdProtDigitalInput3SetpointReset();
  void cmdProtDigitalInput4SetpointReset();
  void cmdProtDigitalSetpointReset();

  void cmdProtAnalogInput1SetpointReset();
  void cmdProtAnalogInput2SetpointReset();
  void cmdProtAnalogInput3SetpointReset();
  void cmdProtAnalogInput4SetpointReset();
  void cmdProtAnalogSetpointReset();

  void cmdProtSetpointReset();

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
  bool checkCanStart(bool isForce = false);

  /*!
   * \brief initStart
   */
  void initStart();

  /*!
   * \brief Метод проверки возможности останова
   * \return
   */
  bool checkCanStop();

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
   * \brief Метод обработки значения сигнала с цифрового входа
   * \param num - номер цифрового входа
   */
  void changedDigitalInput(int num);

  /*!
   * \brief Получение значений с аналоговых входов и сохранение в параметры
   */
  void calcAnalogInputs();

  void calcTemperatureSTM32();

  void calcTemperatureCCS();

  void calcTurboRotation();

  void setRelayOutputs();

  /*!
   * \brief Метод переключения RS-485/RS-232 ТМС/Скады
   */
  void setDhsScadaInterface();

  /*!
   * \brief Индикация турбинного вращения
   */
  void indicationTurbineRotation();

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
  //! Идентификатор семафора Установки профиля уставок по умолчанию
  osSemaphoreId setProfileDefaultSemaphoreId_;
  //! Идентификатор семафора Сохранения конфигурации
  osSemaphoreId saveConfigSemaphoreId_;
  //! Идентификатор семафора Загрузки конфигурации
  osSemaphoreId loadConfigSemaphoreId_;

  bool isConnectMaster_;

  int countPhaseRotation_;

  uint32_t restartTime_[RESTART_TIME_MAX+1];

  bool isParametersControl_;

  stCalcAsync calcAsync_;

};

#endif /* CCS_H_ */

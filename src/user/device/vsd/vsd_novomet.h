/*
 * vsd_novomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_NOVOMET_H_
#define VSD_NOVOMET_H_

#include "vsd.h"
#include "device_modbus.h"

/*!
 * \brief The enInvertorControl enum
 * Флаги регистра управления ПЧ
 */
enum enInvertorControl
{  
  INV_CONTROL_START           = 1,          /// Запустить инвертор ("Старт")
  INV_CONTROL_STOP            = 2,          /// Остановить инвертор ("Стоп")
  INV_CONTROL_LEFT_DIRECTION  = 4,          /// Задать левое вращение
  INV_CONTROL_RIGHT_DIRECTION = 8,          /// Задать правое вращение
  INV_CONTROL_ALARM           = 16,         /// Аварийный останов
  INV_CONTROL_CURRENT_OPT     = 32,         /// Задание оптимизации по току
  INV_CONTROL_POWER_OPT       = 64,         /// задание оптимизации по мощности
  INV_CONTROL_CLEAR_OPT       = 128,        /// Отключить оптимизацию
  INV_CONTROL_ASYN_MOTOR      = 256,        /// Асинхронный
  INV_CONTROL_VENT_MOTOR      = 512,        /// Вентильный
  INV_CONTROL_OVERPWM1_ON     = 1024,       /// Включить режим OverPWM1
  INV_CONTROL_OVERPWM2_ON     = 2048,       /// Включить режим OverPWM2
  INV_CONTROL_OVERPWM_OFF     = 4096,       /// Выключить режим OverPWM
  INV_CONTROL_DISCHARGE_ON    = 8192,       /// Включить предразряд шины инвертора
  INV_CONTROL_DISCHARGE_OFF   = 16384,      /// Выключить предразряд шины инвертора
};

/*!
 * \brief Флаги статусных регистров инвертора
 */
enum enInvertorStatus
{ 
  VSD_STATUS_STARTED          = 0,          /// Запуск ПЧ
  VSD_STATUS_WAIT_RECT_START,               /// Ожидаем запуска выпрямителя
  VSD_STATUS_STOPPED_REGISTER,              /// Инвертор остановлен по изменению важного параметра
  VSD_STATUS_STOPPED_EXTERNAL,              /// Инвертор остановлен по команде извне
  VSD_STATUS_WAIT_RECT_STOP,                /// Ожидание остнова выпрямителя
  VSD_STATUS_FAULT_STOPPED,                 /// Остановлен по причине FAULT
  VSD_STATUS_RIGHT_DIRECTION,               /// Правое направление вращения
  VSD_STATUS_I_LIMIT,                       /// Токоограничение
  VSD_STATUS_ULOW,                          /// Недостаточно напряжения
  VSD_STATUS_STOPPED_ALARM,                 /// Остановлен аварийно
  VSD_STATUS_UD_LOW_FAULT,                  /// Остановлен по снижению напряжения на шин
  VSD_STATUS_UD_HIGH_FAULT,                 /// Остановлен по превышению напряжения на шине
  VSD_STATUS_TO_STOP_MODE,                  /// Режим плавной остановки двигателя
  VSD_STATUS_UIN_ASYM,                      /// Остановлен по несимметрии входного напряжения
  VSD_STATUS_URECT_SHORTCIRCUIT,            /// Остановлен по КЗ от выпрямителя
  VSD_STATUS_RESERVED,                      /// Резерв
  VSD_STATUS_I_RMS       = 16,              /// Сработала тепловая защита
  VSD_STATUS_AST_ERR,                       /// Система автонастройки не смогла определить параметры линии
  VSD_STATUS_I_LIMIT_FAST,                  /// Превышение порога мгновенного токоограничения
  VSD_STATUS_CURRENT_OPT,                   /// Включена оптимизация по току
  VSD_STATUS_POWER_OPT,                     /// Включена оптимизация по выходной мощности
  VSD_STATUS_OPT_DONE,
  VSD_STATUS_M_TYPE0,
  VSD_STATUS_M_TYPE1,
  VSD_STATUS_DISCHARGE_ON,
  VSD_STATUS_DISCAHRGE,
  VSD_STATUS_DISCHARGE_ERR,
  VSD_STATUS_VC_ERR,
  VSD_STATUS_3               = 32
};

enum enRegulatorQueue
{
  VSD_REQULATOR_QUEUE_NULL = -1,            /// Пустой
  VSD_REQULATOR_QUEUE_TEST,                 /// Тестовый
  VSD_REQULATOR_QUEUE_UF,                   /// U/f
  VSD_REQULATOR_QUEUE_TORQUE,               /// Расклинка
  VSD_REQULATOR_QUEUE_VECT,                 /// Векторный
  VSD_REQULATOR_QUEUE_AUTO,                 /// Автонастройка
  VSD_REQULATOR_QUEUE_TURBO                 /// Турбинное вращение
};

enum enPeriodExchange
{
  NOT_READ                        = 0,      /// Не читать
  EVERY_TIME                      = 1,      /// Каждый раз
  VERY_OFTEN                      = 10,     /// Очень часто
  OFTEN                           = 100,    /// Часто
  RARELY                          = 1000,   /// Редко
  HARDLY_EVER                     = 10000   /// Очень редко
};

class VsdNovomet: public Vsd
{
public:
  VsdNovomet();
  virtual ~VsdNovomet();

  void init();
  void initModbusParameters();
  void initParameters();
  void getNewValue(uint16_t id);

  uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод записи параметра в устройство
   * \param id
   * \param value
   * \return
   */
  void writeToDevice(int id, float value);

  /*!
   * \brief Метод проверки флага в регистре статуса инвертора
   * \param bit проверяемый бит
   * \return 0 - бит не установлен 1 - бит установлен
   */
  bool checkVsdStatus(uint8_t bit);

  /*!
   * \brief Метод запуска ЧРП Новомет
   * Для управления запусками и остановами, и для контроля состояния ПЧ
   * используется регистр управления IREG_INVERTOR_CONTROL и два регистра
   * состояния IREG_INVERTOR_STATUS и IREG_INVERTOR_EXT_STATUS.
   * Пуск осуществляется установкой в "1" бита 0 ("старт") в регистре управления
   * ПЧ IREG_INVERTOR_CONTROL через внешний интерфейс MODBUS, либо по
   * внутренней CAN шине с помощью пульта управления ПЧ.
   * После получения команды "старт" в случае, если включен режим остановки
   * турбинного вращения для ВД, перед процедурой запуска двигателя
   * производится разряд шины инвертора.
   * Далее контроллер ПЧ устанавливает биты INV_STATUS_STARTED и
   * INV_STATUS_WAIT_RECT_START регистра управления и ждет подтверждения
   * заряда конденсаторов DC фильтра от платы управления выпрямителем
   * (выделенный сигнал оптоволоконной линии).
   * После получения подтверждения бит INV_STATUS_WAIT_RECT_START
   * снимается, контроллер ПЧ включает инвертор и запускает очередь
   * отработки регуляторов двигателя (торможение турбинного вращения,
   * режим пуска - с раскачкой или толчковый, основной режим управления).
   * \return Код выполнения операции
   */
  int start();

  /*!
   * \brief Метод останова ЧРП Новомет
   * При установке бита 1 ("Стоп") в регистре управления ПЧ контроллер ПЧ
   * устанавливает биты INV_STATUS_STOPPED_EXTERNAL и INV_STATUS_TO_STOP_MODE,
   * и ожидает окончания процедуры плавного останова двигателя.
   * После останова двигателя контроллер выключает инвертор, снимает флаг
   * INV_STATUS_STARTED, устанавливает флаг INV_STATUS_WAIT_RECT_STOP и
   * ждет сигнала подтверждения выключения от платы управления выпрямителем.
   * После получения подтверждения выключения флаг INV_STATUS_WAIT_RECT_STOP
   * снимается, система готова для очередного запуска.
   * \return Код выполнения операции
   */
  int stop();

  bool checkStart();
  bool checkStop();

  /*!
   * \brief Метод записи основного режима работы ЧРП
   * Основных методов 2:
   * U/f регулирование АД и ВД код: 1
   * Векторное управление ВД код: 3
   * Поскольку у ЧРП только один основной алгоритм и он обязан быть,
   * записываем его последним
   * \return
   */
  int setMainRegimeVSD();

  /*!
   * \brief Метод задания уставки частоты двигателя
   * При установке текущей частоты проверяется диапазон уставок
   * минимальной и максимальной частоты
   * \param value - частота
   * \return
   */
  int setFrequency(float value);

  /*!
   * \brief Метод задания минимальной частоты
   * \param value частота
   * \return 0 - задали 1 - не задали
   */
  int setMinFrequency(float value);

  /*!
   * \brief Метод задания максимальной частоты
   * \param value - значение максимальной частоты
   * \return 0 - задали 1 - не задали
   */
  int setMaxFrequency(float value);

  /*!
   * \brief Метод задания типа двигателя
   * \param TypeMotor - Тип двигателя
   * \return Код результата операции
   */
  int setMotorType(float value);

  /*!
   * \brief getMotorType
   */
  void calcMotorType();

  /*!
   * \brief calcSpeedUp
   */
  void calcTempSpeedUp();

  /*!
   * \brief calcTimeSpeedUp
   */
  void calcTimeSpeedUp();

  /*!
   * \brief calcMotorControl
   */
  void calcMotorControl();

  /*!
   * \brief calcRotation
   */
  void calcRotation();

  /*!
   * \brief Функция вычисления тока звена постоянного тока
   */
  void calcCurrentDC();

  /*!
   * \brief Метод задания времени набора частоты
   * \return Код результата операции
   */
  int setTempSpeedUp(float value);

  /*!
   * \brief Метод задания времени снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setTempSpeedDown(float value);

  /*!
   * \brief Метод задания типа управления двигателем, в ЧРП Новомет задание типа
   * управления двигателем это задание основного алгоритма управления
   * Основные алгоритмы управления - те, при которых происходит эксплуатация установки.
   * U/f режим управления для АД или ВД;
   * векторный режим управления для ВД.
   * Основной режим задаем в очереди алгоритмов, последним в рабочей очереди
   * Рабочая очередь регуляторов в общем виде состоит из серии стартовых
   * алгоритмов и одного основного алгоритма работы. В каждый регистр рабочей
   * очереди записывается код соотв. алгоритма. Помимо кода алгоритмов управления,
   * в регистре очереди может быть прописан код пустого алгоритма. Каждый стартовый
   * режим после выполнения по заданным условиям, а также пустой алгоритм,
   * передает управление инвертором следующему стартовому алгоритму из рабочей
   * очереди, пока оно не дойдет до кода основного алгоритма. Основной алгоритм
   * управления отрабатывается неограниченно долго по времени вплоть до момента
   * остановки ПЧ по той или иной причине. Безразлично, какие коды алгоритмов
   * прописаны в последующих регистрах очереди, т.к. управление к ним передано
   * не будет. В частности, очередь алгоритмов управления может состоять только
   * из одного, основного, режима управления, если он встретился первым из не
   * пустых алгоритмов. Код алгоритма, работающего в данный момент, отображается
   * в регистре IREG_CURRENT_REGULATOR, доступном только для чтения.
   * \param value
   * \return Код результата операции
   */
  int setMotorControl(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F1
   * \param value
   * \return
   */
  int setUfF1(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F2
   * \param value
   * \return
   */

  int setUfF2(float value);
  /*!
   * \brief Метод задания точки характеристики U/f F3
   * \param value
   * \return
   */
  int setUfF3(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F4
   * \param value
   * \return
   */
  int setUfF4(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F5
   * \param value
   * \return
   */
  int setUfF5(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F6
   * \param value
   * \return
   */
  int setUfF6(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U1
   * \param value
   * \return
   */
  int setUfU1(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U2
   * \param value
   * \return
   */
  int setUfU2(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U3
   * \param value
   * \return
   */
  int setUfU3(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U4
   * \param value
   * \return
   */
  int setUfU4(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U5
   * \param value
   * \return
   */
  int setUfU5(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U6
   * \param value
   * \return
   */
  int setUfU6(float value);

  /*!
   * \brief Метод задания направления вращения
   * \param value - направление вращения 1 прямое(правое),
   * 2 обратное(левое)
   * \return
   */
  int setRotation(float value);

  /*!
   * \brief Проверка на "необходимость" работы с параметром
   * \param indexParam
   * \return
   */
  int checkExchangModbusParameters(int indexParam);

  /*!
   * \brief Функция вычисления значений,
   */
  void calcParameters(uint16_t id);


private:
  ModbusParameter modbusParameters_[135];
  DeviceModbus *dm_;

};

#endif /* VSD_NOVOMET_H_ */

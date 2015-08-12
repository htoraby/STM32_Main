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

class RegimeRunNovomet;

/*!
 * \brief The enInvertorControl enum
 * Флаги регистра управления ПЧ
 */
enum enInvertorControl
{  
  INV_CONTROL_START           = 1,       //!< Запустить инвертор ("Старт")
  INV_CONTROL_STOP            = 2,       //!< Остановить инвертор ("Стоп")
  INV_CONTROL_LEFT_DIRECTION  = 4,       //!< Задать левое вращение (обратное)
  INV_CONTROL_RIGHT_DIRECTION = 8,       //!< Задать правое вращение (прямое)
  INV_CONTROL_ALARM           = 16,      //!< Аварийный останов
  INV_CONTROL_CURRENT_OPT     = 32,      //!< Задание оптимизации по току
  INV_CONTROL_POWER_OPT       = 64,      //!< задание оптимизации по мощности
  INV_CONTROL_CLEAR_OPT       = 128,     //!< Отключить оптимизацию
  INV_CONTROL_ASYN_MOTOR      = 256,     //!< Асинхронный
  INV_CONTROL_VENT_MOTOR      = 512,     //!< Вентильный
  INV_CONTROL_OVERPWM1_ON     = 1024,    //!< Включить режим OverPWM1
  INV_CONTROL_OVERPWM2_ON     = 2048,    //!< Включить режим OverPWM2
  INV_CONTROL_OVERPWM_OFF     = 4096,    //!< Выключить режим OverPWM
  INV_CONTROL_DISCHARGE_ON    = 8192,    //!< Включить предразряд шины инвертора
  INV_CONTROL_DISCHARGE_OFF   = 16384,   //!< Выключить предразряд шины инвертора
};

enum enRegulatorQueue
{
  VSD_REQULATOR_QUEUE_NULL = -1,         //!< Пустой
  VSD_REQULATOR_QUEUE_TEST,              //!< Тестовый
  VSD_REQULATOR_QUEUE_UF,                //!< U/f
  VSD_REQULATOR_QUEUE_PUSH,              //!< Расклинка
  VSD_REQULATOR_QUEUE_VECT,              //!< Векторный
  VSD_REQULATOR_QUEUE_AUTO,              //!< Автонастройка
  VSD_REQULATOR_QUEUE_PICKUP             //!< Турбинное вращение
};

class VsdNovomet: public Vsd
{
public:
  VsdNovomet();
  virtual ~VsdNovomet();

  void init();
  void initModbusParameters();
  void initParameters();

  bool isConnect();

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
   * \param type - тип останова (плавный, выбегом)
   * \return Код выполнения операции
   */
  int stop(float type);

  /*!
   * \brief Метод аварийного останова (не используется)
   * \return
   */
  int alarmstop();

  /*!
   * \brief Метод подтверждения команды "Пуск" от ЧРП
   * проверяет что стоит бит что ЧРП запустился
   * \return
   */
  bool checkStart();

  /*!
   * \brief Метод подтверждения команды "Стоп" от ЧРП
   * проверяет что стоит бит, останова ЧРП
   * \return
   */
  bool checkStop();

  /*!
   * \brief Автомат режимов пуска ЧРП
   */
  void processingRegimeRun();

  int onRegimePickup();

  /*!
   * \brief Функция включения толчкового режима
   * Функция посылает в ЧРП настройки толчкового режима из КСУ
   * \return
   */
  int onRegimePush();

  /*!
   * \brief Функция(пустая) выключения толчкового режима
   * \return
   */
  int offRegimePush();

  /*!
   * \brief onRegimeSwing
   * \return
   */
  int onRegimeSwing();

  /*!
   * \brief offRegimeSwing
   * \return
   */
  int offRegimeSwing();

  /*!
   * \brief onRegimeJarring
   * \return
   */
  int onRegimeJarring();

  /*!
   * \brief offRegimeJarring
   * \return
   */
  int offRegimeJarring();

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

  int setMotorType(float value);

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
  int setTimeSpeedUp(float value);
  int setTimeSpeedDown(float value);
  int setTempSpeedUp(float value);
  int setTempSpeedDown(float value);


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
   * \param value - направление вращения 0 прямое(правое),
   * 1 обратное(левое)
   * \return
   */
  int setRotation(float value);

  int setSwitchingFrequency(float value);

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

  RegimeRunNovomet *regimeRun_;
};

#endif /* VSD_NOVOMET_H_ */

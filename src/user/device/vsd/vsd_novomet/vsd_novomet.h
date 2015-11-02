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

enum enRegulatorQueue
{
  VSD_REQULATOR_QUEUE_NULL = -1,            //!< Пустой
  VSD_REQULATOR_QUEUE_TEST,                 //!< Тестовый
  VSD_REQULATOR_QUEUE_UF,                   //!< U/f
  VSD_REQULATOR_QUEUE_PUSH,                 //!< Расклинка
  VSD_REQULATOR_QUEUE_VECT,                 //!< Векторный
  VSD_REQULATOR_QUEUE_AUTO,                 //!< Автонастройка
  VSD_REQULATOR_QUEUE_PICKUP                //!< Турбинное вращение
};

enum enResMode
{
  VSD_RES_MODE_ANGLE = 0,                   //!< Расчётный угол отклонения ротора
  VSD_RES_MODE_TORQUE = 1,                  //!< Мгновенная величина расчетного момента
  VSD_RES_MODE_POWER = 2,                   //!< Мгновенная мощность привода
  VSD_RES_MODE_NONE = 3,                    //!< Не использовать
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

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  int setMotorType(float value);
  int setMotorFrequency(float value);
  int setMotorCurrent(float value);
  int setMotorVoltage(float value);
  void setLimitsMotor();

  // РЕЖИМЫ ПУСКА
  int onRegimePush();
  int offRegimePush();
  int onRegimeSwing();
  int offRegimeSwing();
  int onRegimePickup();
  int offRegimePickup();
  int onRegimeAutoAdaptation();
  int offRegimeAutoAdaptation();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
  int setRotation(float value);
  int setMinFrequency(float value);
  int setMaxFrequency(float value);
  int setFrequency(float value);
  int setTimeSpeedUp(float value);
  int setTimeSpeedDown(float value);
  int setSwitchingFrequency(float value);
  int setSwitchingFrequencyMode(float value);
  int setResonanceRemoveSource(float value);
  int setSumInduct(float value);

  // НАСТРОЙКА U/f
  int setUf_f1(float value);
  int setUf_f2(float value);
  int setUf_f3(float value);
  int setUf_f4(float value);
  int setUf_f5(float value);
  int setUf_f6(float value);
  int setUf_U1(float value);
  int setUf_U2(float value);
  int setUf_U3(float value);
  int setUf_U4(float value);
  int setUf_U5(float value);
  int setUf_U6(float value);

  int calcUfCharacteristicU(float value);
  int calcUfCharacteristicF(float value);

  void readUfCharacterictic();
  int setBaseVoltage(float value);
  int setBaseFrequency(float value);

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
   * \brief readInDevice
   * \param id
   */
  void readInDevice(int id);

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

  int resetBlock();

  /*!
   * \brief Автомат режимов пуска ЧРП
   */
  void processingRegimeRun();





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
   * \brief calcSpeedDown
   */
  void calcTempSpeedDown();

  /*!
   * \brief calcTimeSpeedDown
   */
  void calcTimeSpeedDown();

  /*!
   * \brief calcRotation
   */
  void calcRotation();

  /*!
   * \brief Функция вычисления тока звена постоянного тока
   */
  void calcCurrentDC();

  void calcSwitchFreqMode();

  void calcResonanceRemoveSource();

  void calcSystemInduct();

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

  /*!
   * \brief Сброс очереди запуска ЧРП
   * \return
   */
  void resetRunQueue();
private:
  ModbusParameter modbusParameters_[159];
  DeviceModbus *dm_;

  RegimeRunNovomet *regimeRun_;
};

#endif /* VSD_NOVOMET_H_ */

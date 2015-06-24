/*
 * vsd_novomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_ETALON_H_
#define VSD_ETALON_H_

#include "vsd.h"
#include "device_modbus.h"

enum enInfoEtalon
{
  VSD_ETALON_INFO_READY = 0,                //!< Готов
  VSD_ETALON_INFO_UNDERLOAD = 1,            //!< Недогруз
  VSD_ETALON_INFO_OVERLOAD = 2,             //!< Перегруз
  VSD_ETALON_INFO_RESISTANCE = 3,           //!< Низкое Rиз
  VSD_ETALON_INFO_UNDERVOLTAGE = 4,         //!< Низкое напряжение
  VSD_ETALON_INFO_OVERVOLTAGE = 5,          //!< Высокое напряжение
  VSD_ETALON_INFO_OVERVOLTAGE_DC = 6,       //!< Высокое U сил. Цепи
  VSD_ETALON_INFO_UNDERVOLTAGE_DC = 7,      //!< Низкое U сил. цепи
  VSD_ETALON_INFO_RUN_COUNT = 8,            //!< Прев. кол-во пусков
  VSD_ETALON_INFO_OVERHEAT_IGBT = 9,        //!< Перегрев IGBT
  VSD_ETALON_INFO_OVERHEAT_FILTER = 10,     //!< Перегрев фильтра
  VSD_ETALON_INFO_PROT = 11,                //!< Защита ЧРП
  VSD_ETALON_INFO_SUPPLY_DRIVERS = 12,      //!< Питание драйверов
  VSD_ETALON_INFO_MONOMETR = 13,            //!< Конт манометр
  VSD_ETALON_INFO_AI_0 = 14,                //!< Доп. аналог вход 0
  VSD_ETALON_INFO_SEQUENCE_PHASE = 15,      //!< Чередование фаз
  VSD_ETALON_INFO_OVERHEAT_MOTOR = 16,      //!< Выс. Температура
  VSD_ETALON_INFO_OVERVIBRATION = 17,     	//!< Выс. Вибрация
  VSD_ETALON_INFO_PRESSURE = 18,            //!< Низ. Давление
  VSD_ETALON_INFO_19 = 19,                  //!< Ошибка 19
  VSD_ETALON_INFO_PRESSURE_Z = 20,          //!< Низкое Pзатр
  VSD_ETALON_INFO_IMBALANCE_CURRENT = 21, 	//!< Дисбаланс токов
  VSD_ETALON_INFO_IMBALANCE_VOLTAGE = 22,   //!< Дисбаланс напряж.
  VSD_ETALON_INFO_TURBINE = 23,             //!< Турбинное вращение
  VSD_ETALON_INFO_24 = 24,                  //!< Прочие ошибки
  VSD_ETALON_INFO_FAILURE_SUPPLY = 25,      //!< Авария питания
  VSD_ETALON_INFO_DOOR = 26,                //!< Открыта дверь
  VSD_ETALON_INFO_LOST_SUPPLY = 27,         //!< Пропало питание
  VSD_ETALON_INFO_CONDENSATOR = 28,         //!< Нет заряда конденс.
  VSD_ETALON_INFO_TERISTORS = 29,           //!< Авария тиристоров
  VSD_ETALON_INFO_CURRENT_LIMIT = 30,       //!< Токоограничение
  VSD_ETALON_INFO_31 = 31,                  //!< Ошибка номер 31
  VSD_ETALON_INFO_32 = 32,                  //!< По подхвату
  VSD_ETALON_INFO_AUTO_STOP = 33,           //!< Авто останов
  VSD_ETALON_INFO_MANUAL_STOP = 34,         //!< Ручной останов
  VSD_ETALON_INFO_REMOTE_STOP = 35,         //!< Внешний останов
  VSD_ETALON_INFO_AUTO_RUN = 36,            //!< Автоматич. пуск
  VSD_ETALON_INFO_MANUAL_RUN = 37,          //!< Ручной пуск
  VSD_ETALON_INFO_REMOTE_RUN = 38,          //!< Внешний пуск
  VSD_ETALON_INFO_RESTART_COUNT = 39,       //!< По количеству АПВ
  VSD_ETALON_INFO_MEMORY = 40,              //!< Ошибка ОЗУ
  VSD_ETALON_INFO_41 = 41,                  //!< Отключен
  VSD_ETALON_INFO_DI = 42,                  //!< Отказ дискр. вх.
  VSD_ETALON_INFO_ADC = 43,                 //!< Отказ АЦП
  VSD_ETALON_INFO_ANALOG_SUPPLY = 44,       //!< Аналог. Питание
  VSD_ETALON_INFO_SENSOR_SUPPLY = 45,       //!< Питание датчиков
  VSD_ETALON_INFO_EEPROM = 46,              //!< Ошибка EEPROM
  VSD_ETALON_INFO_NOT_READY = 47,           //!< ПЧ не готов
  VSD_ETALON_INFO_SETPOINT = 48,            //!< Сбой уставок
  VSD_ETALON_INFO_BLOCK_RUN = 49            //!< Блокировка пуска
};

class VsdEtalon: public Vsd
{
public:
  VsdEtalon();
  virtual ~VsdEtalon();

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
   * \brief readInDevice
   * \param id
   */
  void readInDevice(int id);

  /*!
   * \brief Метод запуска
   * \return Код выполнения операции
   */
  int start();

  /*!
   * \brief Метод останова
   * \return Код выполнения операции
   */
  int stop();

  /*!
   * \brief alarmstop
   * \return
   */
  int alarmstop();

  bool checkStart();
  bool checkStop();

  /*!
   * \brief Функция включения толчкового режима
   * Записывает в ЧРП настройки режима и
   * Записывает в параметр 265 VSD_ETALON_START_TYPE - 1
   * \return
   */
  int onRegimePush();

  /*!
   * \brief Функция выключения толчкового режима
   * Записывает в параметр 265 VSD_ETALON_START_TYPE - 0
   * \return
   */
  int offRegimePush();

  /*!
   * \brief Функция включения режима раскачки
   * Записывает в ЧРП настройки режима
   * Записывает в параметр 265 VSD_ETALON_START_TYPE - 2
   * \return
   */
  int onRegimeSwing();

  /*!
   * \brief Функция выключения режима раскачки
   * Записывает в параметр 265 VSD_ETALON_START_TYPE - 0
   * \return
   */
  int offRegimeSwing();

  /*!
   * \brief onRegimePickup
   * \return
   */
  int onRegimePickup();

  /*!
   * \brief offRegimePickup
   * \return
   */
  int offRegimePickup();

  /*!
   * \brief onRegimeSkipFreq
   * \return
   */
  int onRegimeSkipFreq();

  /*!
   * \brief offRegimeSkipFreq
   * \return
   */
  int offRegimeSkipFreq();

  /*!
   * \brief Метод записи основного режима работы ЧРП
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
   * \brief Метод задания времени набора частоты
   * \return Код результата операции
   */
  int setTimeSpeedUp(float value);

  /*!
   * \brief Метод задания времени снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setTimeSpeedDown(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F6
   * \param value
   * \return
   */
  int setUfF6(float value);

  /*!
   * \brief setUfU
   * \param id
   * \param value
   * \return
   */
  int setUfU(uint16_t id, float value);

  /*!
   * \brief Метод задания точки характиристики U/f U6
   * \param value
   * \return
   */
  int setUfU6(float value);

  /*!
   * \brief Функция чтения U/f характеристики
   */
  void readUfCharacterictic();

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
  ModbusParameter modbusParameters_[110];
  DeviceModbus *dm_;

  /*!
   * \brief convertBitVsdStatus
   * \param value
   */
  void convertBitVsdStatus(float value);
};

#endif /* VSD_NOVOMET_H_ */

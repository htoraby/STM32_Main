/*
 * vsd_etalon.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_ETALON_H_
#define VSD_ETALON_H_

#include "vsd.h"
#include "device_modbus.h"

class RegimeRunEtalon;

enum enEtalonStatus
{
  VSD_ETALON_STATUS_READY = 0,                //!< Готов
  VSD_ETALON_STATUS_UNDERLOAD = 1,            //!< Недогруз
  VSD_ETALON_STATUS_OVERLOAD = 2,             //!< Перегруз
  VSD_ETALON_STATUS_RESISTANCE = 3,           //!< Низкое Rиз
  VSD_ETALON_STATUS_UNDERVOLTAGE = 4,         //!< Низкое напряжение
  VSD_ETALON_STATUS_OVERVOLTAGE = 5,          //!< Высокое напряжение
  VSD_ETALON_STATUS_OVERVOLTAGE_DC = 6,       //!< Высокое U сил. Цепи
  VSD_ETALON_STATUS_UNDERVOLTAGE_DC = 7,      //!< Низкое U сил. цепи
  VSD_ETALON_STATUS_RUN_COUNT = 8,            //!< Прев. кол-во пусков
  VSD_ETALON_STATUS_OVERHEAT_IGBT = 9,        //!< Перегрев IGBT
  VSD_ETALON_STATUS_OVERHEAT_FILTER = 10,     //!< Перегрев фильтра
  VSD_ETALON_STATUS_PROT = 11,                //!< Защита ЧРП
  VSD_ETALON_STATUS_SUPPLY_DRIVERS = 12,      //!< Питание драйверов
  VSD_ETALON_STATUS_MONOMETR = 13,            //!< Конт манометр
  VSD_ETALON_STATUS_AI_0 = 14,                //!< Доп. аналог вход 0
  VSD_ETALON_STATUS_SEQUENCE_PHASE = 15,      //!< Чередование фаз
  VSD_ETALON_STATUS_OVERHEAT_MOTOR = 16,      //!< Выс. Температура
  VSD_ETALON_STATUS_OVERVIBRATION = 17,     	//!< Выс. Вибрация
  VSD_ETALON_STATUS_PRESSURE = 18,            //!< Низ. Давление
  VSD_ETALON_STATUS_19 = 19,                  //!< Ошибка 19
  VSD_ETALON_STATUS_PRESSURE_Z = 20,          //!< Низкое Pзатр
  VSD_ETALON_STATUS_IMBALANCE_CURRENT = 21, 	//!< Дисбаланс токов
  VSD_ETALON_STATUS_IMBALANCE_VOLTAGE = 22,   //!< Дисбаланс напряж.
  VSD_ETALON_STATUS_TURBINE = 23,             //!< Турбинное вращение
  VSD_ETALON_STATUS_24 = 24,                  //!< Прочие ошибки
  VSD_ETALON_STATUS_FAILURE_SUPPLY = 25,      //!< Авария питания
  VSD_ETALON_STATUS_DOOR = 26,                //!< Открыта дверь
  VSD_ETALON_STATUS_LOST_SUPPLY = 27,         //!< Пропало питание
  VSD_ETALON_STATUS_CONDENSATOR = 28,         //!< Нет заряда конденс.
  VSD_ETALON_STATUS_TERISTORS = 29,           //!< Авария тиристоров
  VSD_ETALON_STATUS_CURRENT_LIMIT = 30,       //!< Токоограничение
  VSD_ETALON_STATUS_31 = 31,                  //!< Ошибка номер 31
  VSD_ETALON_STATUS_32 = 32,                  //!< По подхвату
  VSD_ETALON_STATUS_AUTO_STOP = 33,           //!< Авто останов
  VSD_ETALON_STATUS_MANUAL_STOP = 34,         //!< Ручной останов
  VSD_ETALON_STATUS_REMOTE_STOP = 35,         //!< Внешний останов
  VSD_ETALON_STATUS_AUTO_RUN = 36,            //!< Автоматич. пуск
  VSD_ETALON_STATUS_MANUAL_RUN = 37,          //!< Ручной пуск
  VSD_ETALON_STATUS_REMOTE_RUN = 38,          //!< Внешний пуск
  VSD_ETALON_STATUS_RESTART_COUNT = 39,       //!< По количеству АПВ
  VSD_ETALON_STATUS_MEMORY = 40,              //!< Ошибка ОЗУ
  VSD_ETALON_STATUS_41 = 41,                  //!< Отключен
  VSD_ETALON_STATUS_DI = 42,                  //!< Отказ дискр. вх.
  VSD_ETALON_STATUS_ADC = 43,                 //!< Отказ АЦП
  VSD_ETALON_STATUS_ANALOG_SUPPLY = 44,       //!< Аналог. Питание
  VSD_ETALON_STATUS_SENSOR_SUPPLY = 45,       //!< Питание датчиков
  VSD_ETALON_STATUS_EEPROM = 46,              //!< Ошибка EEPROM
  VSD_ETALON_STATUS_NOT_READY = 47,           //!< ПЧ не готов
  VSD_ETALON_STATUS_SETPOINT = 48,            //!< Сбой уставок
  VSD_ETALON_STATUS_BLOCK_RUN = 49            //!< Блокировка пуска
};

class VsdEtalon: public Vsd
{
public:
  VsdEtalon();
  virtual ~VsdEtalon();

  /*!
   * \brief Метод инициализации карты modbus регистров ЧРП Эталон
   */
  void initModbusParameters();

  /*!
   * \brief Переопределенная функция инициализации внутреннего массива
   * параметров ЧРП. Заполняем поля существующих регистров согласно карты
   * modbus регистров ЧРП Эталон(доступ, операции, границы, по умолчанию и
   * валидность)
   */
  void initParameters();

  /*!
   * \brief Переопределенный метод инициализации устройства.
   * вызывает функции initModbusParameters(), создает объект протокола обмена
   * modbus, доинициализирует функцией initParameters() и присваивает в массив
   * сохранённые значения функцией Device::readParameters()
   */
  void init();

  /*!
   * \brief Переопределенный метод проверки наличия связи с устройством по
   * протоколу обмена данными. Вызывает метод dm_->isConnect()
   * \return true - связь есть, false - связи нет
   */
  bool isConnect();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  int setMotorType(float value);

  // РЕЖИМЫ ПУСКА
  int onRegimePush();
  int offRegimePush();
  int onRegimeSwing();
  int offRegimeSwing();
  int onRegimePickup();
  int offRegimePickup();
  int onRegimeSkipFreq();
  int offRegimeSkipFreq();

  // НАСТРОЙКИ ЧРП
  int setRotation(float value);
  int setMinFrequency(float value);
  int setMaxFrequency(float value);
  int setFrequency(float value);
  int setTimeSpeedUp(float value); 
  int setTimeSpeedDown(float value);   
  int setSwitchingFrequency(float value);

  // НАСТРОЙКИ U/f
  int setUfU1(float value);
  int setUfU2(float value);
  int setUfU3(float value);
  int setUfU4(float value);
  int setUfU5(float value);

  int setUfU6(float value);
  int setUfF6(float value);

  // СЕРВИСНОЕ МЕНЮ ЧРП
  int setCoefVoltageInAB(float value);
  int setCoefVoltageInBC(float value);
  int setCoefVoltageInCA(float value);


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
   * \param type - тип останова
   * \return Код выполнения операции
   */
  int stop(float type);

  /*!
   * \brief alarmstop
   * \return
   */
  int alarmstop();

  bool checkStart();
  bool checkStop();

  int resetBlock();


  void processingRegimeRun();

  /*!
   * \brief resetSetpoints
   * \return
   */
  int resetSetpoints();







  /*!
   * \brief setUfU
   * \param id
   * \param value
   * \return
   */
  int setUfU(uint16_t id, float value);

  /*!
   * \brief Функция чтения U/f характеристики
   */
  void readUfCharacterictic();

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
  ModbusParameter modbusParameters_[121];
  DeviceModbus *dm_;

  RegimeRunEtalon *regimeRun_;

  /*!
   * \brief convertBitVsdStatus
   * \param value
   */
  void convertBitVsdStatus(float value);
};

#endif /* VSD_NOVOMET_H_ */

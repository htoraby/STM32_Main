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

enum enVsdEtalonAlarm {
  VSD_ETALON_ALARM_UNDERLOAD      = 3001,   //!< Недогруз
  VSD_ETALON_ALARM_MTZ_OR_ASYNC   = 3002,   //!< МТЗ или рассинхронизация ВД
  VSD_ETALON_ALARM_RESISTANCE     = 3003,   //!< Низкое Rиз
  VSD_ETALON_ALARM_UNDERVOLTAGE   = 3004,   //!< Низкое напряжение
  VSD_ETALON_ALARM_OVERVOLTAGE    = 3005,   //!< Высокое напряжение
  VSD_ETALON_ALARM_OVERVOLTAGE_DC = 3006,   //!< Высокое U сил. Цепи
  VSD_ETALON_ALARM_UNDERVOLTAGE_DC = 3007,  //!< Низкое U сил. цепи
  VSD_ETALON_ALARM_NOT_CONNECT     = 3008,   //!< Потеря связи с КСУ
  VSD_ETALON_ALARM_OVERHEAT_IGBT  = 3009,   //!< Перегрев IGBT
  VSD_ETALON_ALARM_OVERHEAT_FILTER = 3010,  //!< Перегрев фильтра
  VSD_ETALON_ALARM_PROT           = 3011,   //!< Защита ЧРП
  VSD_ETALON_ALARM_SUPPLY_DRIVERS = 3012,   //!< Питание драйверов
  VSD_ETALON_ALARM_MONOMETR       = 3013,   //!< Конт манометр
  VSD_ETALON_ALARM_AI_0           = 3014,   //!< Доп. аналог вход 0
  VSD_ETALON_ALARM_SEQUENCE_PHASE = 3015,   //!< Чередование фаз
  VSD_ETALON_ALARM_OVERHEAT_MOTOR = 3016,   //!< Выс. Температура
  VSD_ETALON_ALARM_OVERVIBRATION  = 3017,   //!< Выс. Вибрация
  VSD_ETALON_ALARM_PRESSURE       = 3018,   //!< Низ. Давление
  VSD_ETALON_ALARM_CONTACTOR_OUT  = 3019,   //!< Авария контактора вых. ЧР
  VSD_ETALON_ALARM_CONTACTOR_DS   = 3020,   //!< Авария контактора Пр.пуска
  VSD_ETALON_ALARM_IMBALANCE_CURRENT = 3021,//!< Дисбаланс токов
  VSD_ETALON_ALARM_IMBALANCE_VOLTAGE = 3022,//!< Дисбаланс напряж.
  VSD_ETALON_ALARM_TURBINE        = 3023,   //!< Турбинное вращение
  VSD_ETALON_ALARM_24             = 3024,   //!< Прочие ошибки
  VSD_ETALON_ALARM_FAILURE_SUPPLY = 3025,   //!< Авария питания
  VSD_ETALON_ALARM_DOOR           = 3026,   //!< Открыта дверь
  VSD_ETALON_ALARM_LOST_SUPPLY    = 3027,   //!< Пропало питание
  VSD_ETALON_ALARM_CONDENSATOR    = 3028,   //!< Нет заряда конденс.
  VSD_ETALON_ALARM_TERISTORS      = 3029,   //!< Авария тиристоров
  VSD_ETALON_ALARM_CURRENT_LIMIT  = 3030,   //!< Токоограничение
  VSD_ETALON_ALARM_31             = 3031,   //!< Автомат фильтра
  VSD_ETALON_ALARM_32             = 3032,   //!< По подхвату
  VSD_ETALON_ALARM_AUTO_STOP      = 3033,   //!< Авто останов
  VSD_ETALON_ALARM_MANUAL_STOP    = 3034,   //!< Ручной останов
  VSD_ETALON_ALARM_REMOTE_STOP    = 3035,   //!< Внешний останов
  VSD_ETALON_ALARM_AUTO_RUN       = 3036,   //!< Автоматич. пуск
  VSD_ETALON_ALARM_MANUAL_RUN     = 3037,   //!< Ручной пуск
  VSD_ETALON_ALARM_REMOTE_RUN     = 3038,   //!< Внешний пуск
  VSD_ETALON_ALARM_RESTART_COUNT  = 3039,   //!< По количеству АПВ
  VSD_ETALON_ALARM_MEMORY         = 3040,   //!< Ошибка ОЗУ
  VSD_ETALON_ALARM_41             = 3041,   //!< Отключен
  VSD_ETALON_ALARM_DI             = 3042,   //!< Отказ дискр. вх.
  VSD_ETALON_ALARM_ADC            = 3043,   //!< Отказ АЦП
  VSD_ETALON_ALARM_ANALOG_SUPPLY  = 3044,   //!< Аналог. Питание
  VSD_ETALON_ALARM_SENSOR_SUPPLY  = 3045,   //!< Питание датчиков
  VSD_ETALON_ALARM_EEPROM         = 3046,   //!< Ошибка EEPROM
  VSD_ETALON_ALARM_NOT_READY      = 3047,   //!< ПЧ не готов
  VSD_ETALON_ALARM_SETPOINT       = 3048,   //!< Сбой уставок
  VSD_ETALON_ALARM_BLOCK_RUN      = 3049    //!< Блокировка пуска
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

  // ТЕХНОЛОГИЧЕСКИЕ РЕЖИМЫ
  // Режим токоограничения
  int onRegimeCurrentLimitation();
  int offRegimeCurrentLimitation();
  int configRegimeCurrentLimitation();

  // НАСТРОЙКИ ЧРП
  int setMinFrequency(float value);
  int setMaxFrequency(float value);
  int setFrequency(float value, EventType eventType = NoneType);
  int setTimeSpeedUp(float value); 
  int setTimeSpeedDown(float value);   
  int setSwitchingFrequency(float value);

  int calcUfCharacteristicU(float value);
  int calcUfCharacteristicF(float value);

  void readUfCharacterictic();
  int setBaseVoltage(float value);
  int setBaseFrequency(float value);
  void readTransNeedVoltageTapOff();

  // СЕРВИСНОЕ МЕНЮ ЧРП
  int setCoefVoltageInAB(float value);
  int setCoefVoltageInBC(float value);
  int setCoefVoltageInCA(float value);

  float checkAlarmVsd();
  float checkAlarmVsdUnderVoltage();
  float checkAlarmVsdOverVoltage();
  bool checkPreventVsd();

  void getNewValue(uint16_t id);

  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

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
  int start(bool init);

  /*!
   * \brief Метод останова
   * \param isAlarm - тип останова
   * \return Код выполнения операции
   */
  int stop(bool isAlarm);

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
   * \param idU
   * \param idUPercent
   * \param value
   * \return
   */
  int setUfU(uint16_t idU, uint16_t idUPercent, float value);

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
   * \brief getConnect
   */
  void getConnect();
  void resetConnect();
  int onProtConnect();
  int offProtConnect();

private: 
  void setLimitsCcsParameters();

  ModbusParameter modbusParameters_[141];
  DeviceModbus *dm_;

  RegimeRunEtalon *regimeRun_;

};

#endif /* VSD_NOVOMET_H_ */

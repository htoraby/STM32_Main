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

enum enRegulatorQueue {
  VSD_REQULATOR_QUEUE_NULL = -1,            //!< Пустой
  VSD_REQULATOR_QUEUE_TEST,                 //!< Тестовый
  VSD_REQULATOR_QUEUE_UF,                   //!< U/f
  VSD_REQULATOR_QUEUE_PUSH,                 //!< Расклинка
  VSD_REQULATOR_QUEUE_VECT,                 //!< Векторный
  VSD_REQULATOR_QUEUE_AUTO,                 //!< Автонастройка
  VSD_REQULATOR_QUEUE_PICKUP                //!< Турбинное вращение
};

enum enResMode {
  VSD_RES_MODE_ANGLE = 0,                   //!< Расчётный угол отклонения ротора
  VSD_RES_MODE_TORQUE = 1,                  //!< Мгновенная величина расчетного момента
  VSD_RES_MODE_POWER = 2,                   //!< Мгновенная мощность привода
  VSD_RES_MODE_NONE = 3,                    //!< Не использовать
};

enum enVsdNovometStatus1 {
  VSD_NOVOMET_STATUS_STARTED      = 0,      //!< Запуск ПЧ
  VSD_NOVOMET_STATUS_WAIT_START   = 1,      //!< Ожидаем запуска выпрямителя
  VSD_NOVOMET_STATUS_STOP_REGISTER= 2,      //!< Инвертор остановлен по изменению важного параметра
  VSD_NOVOMET_STATUS_STOP_EXTERNAL= 3,      //!< Инвертор остановлен по команде извне
  VSD_NOVOMET_STATUS_WAIT_STOP    = 4,      //!< Ожидание остнова выпрямителя
  VSD_NOVOMET_STATUS_FAULT_STOPPED= 5,      //!< Остановлен по причине FAULT
  VSD_NOVOMET_STATUS_RESERVED_6   = 6,      //!< Резерв
  VSD_NOVOMET_STATUS_I_LIMIT      = 7,      //!< Токоограничение
  VSD_NOVOMET_STATUS_ULOW         = 8,      //!< Недостаточно напряжения
  VSD_NOVOMET_STATUS_STOP_ALARM   = 9,      //!< Остановлен аварийно
  VSD_NOVOMET_STATUS_UD_LOW_FAULT = 10,     //!< Остановлен по снижению напряжения на шин
  VSD_NOVOMET_STATUS_UD_HIGH_FAULT= 11,     //!< Остановлен по превышению напряжения на шине
  VSD_NOVOMET_STATUS_TO_STOP_MODE = 12,     //!< Режим плавной остановки двигателя
  VSD_NOVOMET_STATUS_UIN_ASYM     = 13,     //!< Остановлен по несимметрии входного напряжения
  VSD_NOVOMET_STATUS_URECT_SHORT  = 14,     //!< Остановлен по КЗ от выпрямителя
  VSD_NOVOMET_STATUS_RESERVED     = 15,     //!< Резерв
};

enum enVsdNovometStatus2 {
  VSD_NOVOMET_STATUS_FC_IT_ERR    = 0,      //!< Сработала токовая тепловая защита инвертора
  VSD_NOVOMET_STATUS_AST_ERR      = 1,      //!< Система автонастройки не смогла определить параметры линии
  VSD_NOVOMET_STATUS_I_LIMIT_FAST = 2,      //!< Превышение порога мгновенного токоограничения
  VSD_NOVOMET_STATUS_CURRENT_OPT  = 3,      //!< Включена оптимизация по току
  VSD_NOVOMET_STATUS_POWER_OPT    = 4,      //!< Включена оптимизация по выходной мощности
  VSD_NOVOMET_STATUS_OPT_DONE     = 5,      //!< Оптимизация состоялась
  VSD_NOVOMET_STATUS_DISCHARGE_ON = 8,      //!< Задействован режим разряда шины
  VSD_NOVOMET_STATUS_DISCAHRGE    = 9,      //!< Режим разряда шины работает
  VSD_NOVOMET_STATUS_DISCHARGE_ERR= 10,     //!< Ошибка режима разряда шины
  VSD_NOVOMET_STATUS_VC_ERR       = 11,     //!< Ошибка векторного режима
  VSD_NOVOMET_STATUS_M_I_FAST_ERR = 12,     //!< Быстрая токовая защита двигателя
  VSD_NOVOMET_STATUS_M_I2T_ERR    = 13,     //!< Токовая защита двигателя (перегруз)
};

enum enVsdNovometStatus3 {
  VSD_NOVOMET_STATUS_RIGHT_DIRECT = 0,      //!< Правое направление вращения
  VSD_NOVOMET_STATUS_OWERPWM1     = 1,      //!< OverPWM тип 1
  VSD_NOVOMET_STATUS_OWERPWM2     = 2,      //!< OverPWM тип 2
  VSD_NOVOMET_STATUS_M_TYPE0      = 3,      //!< Бит типа двигателя 0
  VSD_NOVOMET_STATUS_M_TYPE1      = 4,      //!< Бит типа двигателя 1
  VSD_NOVOMET_STATUS_RES_TYPE0    = 5,      //!< Бит типа противорезонанса
  VSD_NOVOMET_STATUS_RES_TYPE1    = 6,      //!< Бит типа противорезонанса
};

enum enVsdNovometStatus5 {
  VSD_NOVOMET_STATUS_ABC_STATE    = 0,      //!< Направление чередования фаз
  VSD_NOVOMET_STATUS_1            = 1,
  VSD_NOVOMET_STATUS_ERR_STATE    = 2,      //!< Ошибка синхронизации с сетью
  VSD_NOVOMET_STATUS_CHARGE_STATE = 3,      //!< Производится заряд ёмкости инвертора
  VSD_NOVOMET_STATUS_STARTED_STATE= 4,      //!< Ёмкость заряжена
  VSD_NOVOMET_STATUS_SYNC_STATE   = 5,      //!< Первая попытка синхронизации
  VSD_NOVOMET_STATUS_BOUNCE_WAIT  = 6,      //!< Ожидание времени антидребезга
  VSD_NOVOMET_STATUS_ERR_SHORT    = 7,      //!< Ошибка по КЗ на инверторе
};

enum enVsdNovometStatus7 {
  VSD_NOVOMET_STATUS_IMAX         = 0,      //!< Превышен максимальный ток инвертора
  VSD_NOVOMET_STATUS_IZ           = 1,      //!< Детектирован разностный ток выходных фаз (утечка)
  VSD_NOVOMET_STATUS_AN_MON       = 2,      //!< Ошибка монитора питания  аналоговой цепи контроллера
  VSD_NOVOMET_STATUS_CTR_MON      = 3,      //!< Ошибка монитора питания цифровой цепи контроллера
  VSD_NOVOMET_STATUS_CLK_MON      = 4,      //!< Ошибка монитора питания часов контроллера
  VSD_NOVOMET_STATUS_MB_MON       = 5,      //!< Ошибка монитора платы измерений
  VSD_NOVOMET_STATUS_DRV0         = 6,      //!< Ошибка драйвера 0
  VSD_NOVOMET_STATUS_DRV1         = 7,      //!< Ошибка драйвера 1
  VSD_NOVOMET_STATUS_DRV2         = 8,      //!< Ошибка драйвера 2
  VSD_NOVOMET_STATUS_TEST         = 9,      //!< Включен тестовый режим контроллера ПЧ
  VSD_NOVOMET_STATUS_TEMP_LINK    = 10,     //!< Ошибка связи с термодатчиками
  VSD_NOVOMET_STATUS_TEMP         = 11,     //!< Перегрев силовых модулей
};


/*!
 * \brief Коды аварий ЧРП
 */
enum enVsdNovometAlarm {
  // STATUS_WORD_1 ЧРП Новомет
  VSD_NOVOMET_ALARM_STOP_REGISTER = 1002,   //!< Инвертор остановлен по изменению важного параметра
  VSD_NOVOMET_ALARM_STOP_EXTERNAL = 1003,   //!< Инвертор остановлен по команде извне
  VSD_NOVOMET_ALARM_FAULT_STOPPED = 1005,   //!< Остановлен по причине FAULT
  VSD_NOVOMET_ALARM_I_LIMIT       = 1007,   //!< Токоограничение
  VSD_NOVOMET_ALARM_ULOW          = 1008,   //!< Недостаточно напряжения
  VSD_NOVOMET_ALARM_STOPPED_ALARM = 1009,   //!< Остановлен аварийно
  VSD_NOVOMET_ALARM_UD_LOW_FAULT  = 1010,   //!< Остановлен по снижению напряжения на шин
  VSD_NOVOMET_ALARM_UD_HIGH_FAULT = 1011,   //!< Остановлен по превышению напряжения на шине
  VSD_NOVOMET_ALARM_UIN_ASYM      = 1013,   //!< Остановлен по несимметрии входного напряжения
  VSD_NOVOMET_ALARM_URECT_SHORT   = 1014,   //!< Остановлен по КЗ от выпрямителя
  // STATUS_WORD_2 ЧРП Новомет
  VSD_NOVOMET_ALARM_FC_IT_ERR     = 1016,   //!< Сработала токовая тепловая защита инвертора
  VSD_NOVOMET_ALARM_AST_ERR       = 1017,   //!< Система автонастройки не смогла определить параметры линии
  VSD_NOVOMET_ALARM_I_LIMIT_FAST  = 1018,   //!< Превышение порога мгновенного токоограничения
  VSD_NOVOMET_ALARM_DISCHARGE_ERR = 1026,   //!< Ошибка режима разряда шины
  VSD_NOVOMET_ALARM_VC_ERR        = 1027,   //!< Ошибка векторного режима
  VSD_NOVOMET_ALARM_M_I_FAST_ERR  = 1028,   //!< Быстрая токовая защита двигателя
  VSD_NOVOMET_ALARM_M_I2T_ERR     = 1029,   //!< Токовая защита двигателя (перегруз)
  // STATUS_WORD_5 ЧРП Новомет
  VSD_NOVOMET_ALARM_ABC_STATE     = 1064,   //!< Направление чередования фаз
  VSD_NOVOMET_ALARM_ERR_STATE     = 1066,   //!< Ошибка синхронизации с сетью
  VSD_NOVOMET_ALARM_ERR_SHORT     = 1071,   //!< Ошибка по КЗ на инверторе
  // STATUS_WORD_7 ЧРП Новомет
  VSD_NOVOMET_ALARM_IMAX          = 1096,   //!< Превышен максимальный ток инвертора
  VSD_NOVOMET_ALARM_IZ            = 1097,   //!< Детектирован разностный ток выходных фаз (утечка)
  VSD_NOVOMET_ALARM_AN_MON        = 1098,   //!< Ошибка монитора питания  аналоговой цепи контроллера
  VSD_NOVOMET_ALARM_CTR_MON       = 1099,   //!< Ошибка монитора питания цифровой цепи контроллера
  VSD_NOVOMET_ALARM_CLK_MON       = 1100,   //!< Ошибка монитора питания часов контроллера
  VSD_NOVOMET_ALARM_MB_MON        = 1101,   //!< Ошибка монитора платы измерений
  VSD_NOVOMET_ALARM_DRV0          = 1102,   //!< Ошибка драйвера 0
  VSD_NOVOMET_ALARM_DRV1          = 1103,   //!< Ошибка драйвера 1
  VSD_NOVOMET_ALARM_DRV2          = 1104,   //!< Ошибка драйвера 2
  VSD_NOVOMET_ALARM_TEST          = 1105,   //!< Включен тестовый режим контроллера ПЧ
  VSD_NOVOMET_ALARM_TEMP_LINK     = 1106,   //!< Ошибка связи с термодатчиками
  VSD_NOVOMET_ALARM_TEMP          = 1107    //!< Перегрев силовых модулей
};

/*!
 * \brief The enControl enum
 * Флаги регистра управления ПЧ
 */
enum enControl
{
  VSD_CONTROL_START           = 1,       //!< Запустить инвертор ("Старт")
  VSD_CONTROL_STOP            = 2,       //!< Остановить инвертор ("Стоп")
  VSD_CONTROL_LEFT_DIRECTION  = 4,       //!< Задать левое вращение (обратное)
  VSD_CONTROL_RIGHT_DIRECTION = 8,       //!< Задать правое вращение (прямое)
  VSD_CONTROL_ALARM           = 16,      //!< Аварийный останов
  VSD_CONTROL_CURRENT_OPT     = 32,      //!< Задание оптимизации по току
  VSD_CONTROL_POWER_OPT       = 64,      //!< задание оптимизации по мощности
  VSD_CONTROL_CLEAR_OPT       = 128,     //!< Отключить оптимизацию
  VSD_CONTROL_ASYN_MOTOR      = 256,     //!< Асинхронный
  VSD_CONTROL_VENT_MOTOR      = 512,     //!< Вентильный
  VSD_CONTROL_OVERPWM1_ON     = 1024,    //!< Включить режим OverPWM1
  VSD_CONTROL_OVERPWM2_ON     = 2048,    //!< Включить режим OverPWM2
  VSD_CONTROL_OVERPWM_OFF     = 4096,    //!< Выключить режим OverPWM
  VSD_CONTROL_DISCHARGE_ON    = 8192,    //!< Включить предразряд шины инвертора
  VSD_CONTROL_DISCHARGE_OFF   = 16384,   //!< Выключить предразряд шины инвертора
  VSD_CONTROL_RESET           = 32768,
};

enum enControl2
{
  VSD_CONTROL_2_RES_ANGLE     = 1,       //!< Установить сигнал о колебания по углу
  VSD_CONTROL_2_RES_TORQUE    = 2,       //!< Установить сигнал о колебания по моменту
  VSD_CONTROL_2_RES_POWER     = 4,       //!< Установить сигнал о колебаниях по мощности
};

enum enSwitchFreqMode
{
  VSD_SWITCHING_FREQUENCY_MODE_SIN = 0,
  VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_1 = 1,
  VSD_SWITCHING_FREQUENCY_MODE_OVERPWM_2 = 2
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

  float checkAlarmVsd();
  bool checkPreventVsd();

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

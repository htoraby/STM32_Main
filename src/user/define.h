/*
 * define.h
 *
 *  Created on: 14.06.2012
 *      Author: trofimenko
 */

#ifndef DEFINE_H_
#define DEFINE_H_

/*!
 * \brief The unDataType union
 * Объединение для преобразования типов данных
 */
union unTypeData
{
  unsigned char      char_t[4];
  signed short int   int16_t[2];
  signed long int    int32_t;
  unsigned short int uint16_t[2];
  unsigned long int  uint32_t;
  float              float_t;
};

/*!
 * \brief The enTypeData enum
 * Перечисление типов данных
 */
enum enTypeData {
  TYPE_DATA_ERROR  = 0,
  TYPE_DATA_CHAR   = 1,
  TYPE_DATA_INT16  = 2,
  TYPE_DATA_INT32  = 3,
  TYPE_DATA_UINT16 = 4,
  TYPE_DATA_UINT32 = 5,
  TYPE_DATA_FLOAT  = 6,
  TYPE_DATA_LAST
};

/*!
 * \brief The enPeriodExchange enum
 */
enum enPeriodExchange
{
  NOT_READ                        = 0,      /// Не читать
  EVERY_TIME                      = 1,      /// Каждый раз
  VERY_OFTEN                      = 10,     /// Очень часто
  OFTEN                           = 100,    /// Часто
  RARELY                          = 1000,   /// Редко
  HARDLY_EVER                     = 10000   /// Очень редко
};

enum enReturns
{
  ok_r                          = 0,
  err_r                         = 1,
  err_min_r                     = 2,
  err_max_r                     = 3,
  RETURN_ERROR_ID_VALUE         = 4,
  RETURN_ERROR_ID               = 5,
  RETURN_ERROR_ACCESS_VALUE     = 6,
  RETURN_ERROR_ACCESS           = 7,
  RETURN_ERROR_OPERATION_VALUE  = 8,
  RETURN_ERROR_OPERATION        = 9,
  RETURN_ERROR_PHYSIC_VALUE     = 10,
  RETURN_ERROR_PHYSIC           = 11,
  RETURN_ERROR_VALIDITY_VALUE   = 12,
  RETURN_ERROR_VALIDITY         = 13,
  RETURN_ERROR_TYPE_DATA_VALUE  = 14,
  RETURN_ERROR_UNIT_VALUE       = 15,
  RETURN_ERROR_UPDATE_VALUE     = 16,
  RETURN_LAST
};

/*!
 * \brief Список уровеней доступа
 */
enum enAccess
{
  ACCESS_ERROR          = 0,
  ACCESS_OPERATOR				= 1,
  ACCESS_TECHNOLOG			= 2,
  ACCESS_SERVICE				= 3,
  ACCESS_GOD            = 4,
	ACCESS_LAST
};

/*!
 * \brief Список команд
 */
enum enOperation
{
  OPERATION_ERROR			  = 0,                //!<
  OPERATION_READ				= 1,                //!< Параметр только для чтения
  OPERATION_WRITE				= 2,                //!< Параметр только для записи
  OPERATION_LIMITED			= 3,                //!< Параметр для записи только во время останова
	OPERATION_LAST
};

/*!
 * \brief Перечисление типов двигателя
 */
enum enMotorType {
  VSD_MOTOR_TYPE_ASYNC = 0,                 //!< Асинхронный двигатель
  VSD_MOTOR_TYPE_VENT  = 1                  //!< Вентильный двигатель
};

/*!
 * \brief Перечисление направлений вращений двигателя
 */
enum enRotation {
  VSD_ROTATION_DIRECT = 0,                   /// Прямое(правое) направление вращения
  VSD_ROTATION_REVERSE                       /// Обратное(левое) направление вращения
};

/*!
 * \brief Перечисление типов управления двигателем
 */
enum enMotorControl {
  VSD_MOTOR_CONTROL_UF = 0,                 /// Метод управления U/f
  VSD_MOTOR_CONTROL_VECT                    /// Векторное управление
};

/*!
 * \brief The enUfType enum
 * Перечисление типов зависимости U/f
 */
enum enUfType {
  /// По точкам
  VSD_UF_TYPE_POINT = 0,
  /// Линейное
  VSD_UF_TYPE_LINE,
  /// Квадратичное
  VSD_UF_TYPE_SQR,
  /// Обратно квадратичное
  VSD_UF_TYPE_SQRT
};

/*!
 * \brief The enNominalVoltage enum
 * Перечисление типов номинального напряжения
 */
enum enNominalVoltage {
  /// 380 вольт
  NOMINAL_VOLTAGE_380 = 0,
  /// 480 вольт
  NOMINAL_VOLTAGE_480,
  /// 500 вольт
  NOVINAL_VOLTAGE_500
};

/*!
 * \brief The enNominalFrequency enum
 * Перечисление номинальная частота
 */
enum enNominalFrequency {
  /// 50Гц
  NOMINAL_FREQUENCY_50 = 0,
  /// 60Гц
  NOMINAL_FREQUENCY_60
};

/*!
 * \brief The enCableCross enum
 * Перечисление сечений кабеля
 */
enum enCableCross {
  /// 16
  CABLE_CROSS_16 = 0,
  /// 21
  CABLE_CROSS_21,
  /// 25
  CABLE_CROSS_25,
  /// 33
  CABLE_CROSS_33,
  /// 42
  CABLE_CROSS_42
};

/*!
 * \brief The enTypeDHS enum
 * Перечисление типов ТМС
 */
enum enTypeDHS {
  TYPE_DHS_NONE,                            //!< Не подключен
  TYPE_DHS_NOVOMET,                         //!< Новомет
  TYPE_DHS_ALMAZ,                           //!< Алмаз
  TYPE_DHS_BORETS,                          //!< Борец
  TYPE_DHS_ELEKTON,                         //!< Электон
  TYPE_DHS_ETALON,                          //!< Эталон
  TYPE_DHS_IRZ,                             //!< Ирз
  TYPE_DHS_ORION,                           //!< Орион
  TYPE_DHS_PIC_V2,                          //!< Шлюмберже
  TYPE_DHS_SCAD,                            //!< Скад
  TYPE_DHS_SCAN,                            //!< Скан
  TYPE_DHS_TRIOL,                           //!< Триол
  TYPE_DHS_ZENIT                            //!< Зенит
};

/*!
 * \brief The enByteRate enum
 * Скорость устройства по RS
 */
enum enByteRate {
  BYTE_RATE_9600,
  BYTE_RATE_14400,
  BYTE_RATE_19200,
  BYTE_RATE_38400,
  BYTE_RATE_57600,
  BYTE_RATE_115200
};

/*!
 * \brief The enSequencePhase enum
 * Порядок чередования фаз
 */
enum enSequencePhase {
  PHASE_ABC = 0,
  PHASE_CBA
};

/*!
 * \brief The enParamProt enum
 * Кретерий защиты
 */
enum enParamProt {
  PARAM_PROT_POWER = 0,
  PARAM_PROT_CURRENT
};

enum enValidity {
  VALIDITY_OK         = 0,
  VALIDITY_ERROR      = 1,
  VALIDITY_MIN        = 2,
  VALIDITY_MAX        = 3,
	VALIDITY_LAST
};

enum enUpdate {
  UPDATE_ERROR				= 0,
  UPDATE_LAST
};

enum enCcsCondition {
  CCS_CONDITION_STOP,                       //!< Останов
  CCS_CONDITION_STOPPING,                   //!< Торможение
  CCS_CONDITION_RUNNING,                    //!< Запуск
  CCS_CONDITION_RUN                         //!< Работа
};

enum enCcsWorkingMode {
  CCS_WORKING_MODE_MANUAL = 0,
  CCS_WORKING_MODE_AUTO,
  CCS_WORKING_MODE_PROGRAM
};

enum enVsdCondition {
  VSD_CONDITION_STOP,                       //!< Состояние останов
  VSD_CONDITION_STOPPING,                   //!< Состояние торможение
  VSD_CONDITION_WAIT_STOP,                  //!< Состояние ожидание начала торможения
  VSD_CONDITION_RUN,                        //!< Состояние работа
  VSD_CONDITION_RUNNING,                    //!< Состояние запуск
  VSD_CONDITION_WAIT_RUN                    //!< Состояние ожидание запуска
};

enum enCcsConditionFlag {
  CCS_CONDITION_FLAG_NULL    = 0,
  CCS_CONDITION_FLAG_DELAY,                 //!< Ожидание срабатывания
  CCS_CONDITION_FLAG_RESTART,               //!< Ожидание АПВ
  CCS_CONDITION_FLAG_BLOCK                  //!< Блокировка
};

/*!
 * \brief Список причин пуска
 */
enum LastReasonRun {
  LastReasonRunNone                     = 0,    //!< Нет пуска
  LastReasonRunOperator                 = 1,    //!< "Оператор"
  LastReasonRunAuto                     = 2,    //!< "Автоматический"
  LastReasonRunRemote                   = 3,    //!< "Телеуправление"
  LastReasonRunProgram                  = 4,    //!< "По программе"
  LastReasonRunApvUnderloadMotor        = 5,    //!< "АПВ по защите: Недогруз двигателя"
  LastReasonRunApvOverloadMotor         = 6,    //!< "АПВ по защите: Перегруз двигателя"
  LastReasonRunApvImbalanceCurMotor     = 7,    //!< "АПВ по защите: Дисбаланс токов ПЭД"
  LastReasonRunApvUnderVoltIn           = 8,    //!< "АПВ по защите: Снижение питания сети"
  LastReasonRunApvOverVoltIn            = 9,    //!< "АПВ по защите: Превышение питания сети"
  LastReasonRunApvImbalanceVoltIn       = 10,   //!< "АПВ по защите: Дисбаланс напряжения"
  LastReasonRunApvTurbineRotation       = 11,   //!< "АПВ по защите: Турбинное вращение"
  LastReasonRunApvPhaseSequence         = 12,   //!< "АПВ по защите: Чередование фаз"
  LastReasonRunApvLockDoor              = 13,   //!< "АПВ по защите: Блокировка двери"
  LastReasonRunApvPressureIntake        = 14,   //!< "АПВ по защите: Pmin на приёме насоса"
  LastReasonRunApvTemperatureMotor      = 15,   //!< "АПВ по защите: Перегрев двигателя"
  LastReasonRunApvVibrationMotor        = 16,   //!< "АПВ по защите: Превышение вибрации"
  LastReasonRunApvUnderPressure1        = 17,   //!< "АПВ по защите: "
  LastReasonRunApvMinAnalog1            = 18,   //!< "АПВ по защите: Ан.вх.№1 min"
  LastReasonRunApvMaxAnalog1            = 19,   //!< "АПВ по защите: Ан.вх.№1 max"
  LastReasonRunApvMinAnalog2            = 20,   //!< "АПВ по защите: Ан.вх.№2 min"
  LastReasonRunApvMaxAnalog2            = 21,   //!< "АПВ по защите: Ан.вх.№2 max"
  LastReasonRunApvFreqPower             = 22,   //!< "АПВ по защите: "
  LastReasonRunApvPower                 = 23,   //!< "АПВ по защите: "
  // Дополнительные причины пуска
  LastReasonRunMinAnalog3               = 100,  //!< "Ан.вх.№3 min"
  LastReasonRunMinAnalog4               = 101,  //!< "Ан.вх.№4 min"
  LastReasonRunMaxAnalog3               = 102,  //!< "Ан.вх.№3 max"
  LastReasonRunMaxAnalog4               = 103,  //!< "Ан.вх.№4 max"
  LastReasonRunApvCurrentMotor          = 104,  //!< "АПВ по защите: Предел тока двигателя"
  LastReasonRunApvOutOfSyncMotor        = 105,  //!< "АПВ по защите: Рассинхронизация ПВЭД"
  LastReasonRunApvResistIsolation       = 106,  //!< "АПВ по защите: Сопротивление изоляции"
  LastReasonRunApvHardwareVsd           = 107,  //!< "АПВ по защите: Аппаратные защиты ЧРП"
};

/*!
 * \brief Список причин останова
 */
enum LastReasonStop {
  LastReasonStopNone                    = 0,    //!< Нет останова
  LastReasonStopOperator                = 1,    //!< "Оператор"
  LastReasonStopResistIsolation         = 2,    //!< "Сопротивление изоляции"
  LastReasonStopUnderloadMotor          = 3,    //!< "Недогруз двигателя"
  LastReasonStopOverloadMotor           = 4,    //!< "Перегруз двигателя"
  LastReasonStopUnderVoltIn             = 5,    //!< "Снижение питания сети"
  LastReasonStopOverVoltIn              = 6,    //!< "Превышение питания сети"
  LastReasonStopHackSu                  = 9,    //!< "Взлом СУ"
  LastReasonStopManometr                = 12,   //!< "Контактный манометр"
  LastReasonStopPressureIntake          = 13,   //!< "Pmin на приёме насоса"
  LastReasonStopTemperatureMotor        = 14,   //!< "Перегрев двигателя"
  LastReasonStopVibrationMotor          = 16,   //!< "Превышение вибрации"
  LastReasonStopImbalanceCurMotor       = 20,   //!< "Дисбаланс токов ПЭД"
  LastReasonStopImbalanceVoltIn         = 21,   //!< "Дисбаланс напряжения"
  LastReasonStopProgram                 = 24,   //!< "По программе"
  LastReasonStopMaxAnalog1              = 26,   //!< "Ан.вх.№1 max"
  LastReasonStopMaxAnalog2              = 27,   //!< "Ан.вх.№2 max"
  LastReasonStopRemote                  = 28,   //!< "Телеуправление"
  LastReasonStopNoVoltage               = 30,   //!<
  LastReasonStopOverloadNoMinVolt       = 35,   //!<
  LastReasonStopUnderloadNoMinVolt      = 36,   //!<
  LastReasonStopOverloadNoMaxVolt       = 37,   //!<
  LastReasonStopUnderloadNoMaxVolt      = 38,   //!<
  LastReasonStopImbalanceVolt           = 39,   //!<
  LastReasonStopMinAnalog1              = 41,   //!<
  LastReasonStopMinAnalog2              = 42,   //!<
  LastReasonStopOverloadBadVolt         = 50,   //!<
  LastReasonStopUnderloadBadVolt        = 51,   //!<
  LastReasonStopImbalanceCurrentBadVolt = 52,   //!<
  LastReasonStopImbalanceVoltageBadVolt = 53,   //!<
  LastReasonStopDigital1                = 59,   //!<
  LastReasonStopDigital2                = 60,   //!<
  LastReasonStopDigital3                = 61,   //!<
  LastReasonStopDigital4                = 62,   //!<
  // Дополнительные причины останова
  LastReasonStopMinAnalog3              = 100,  //!<
  LastReasonStopMinAnalog4              = 101,  //!<
  LastReasonStopMaxAnalog3              = 102,  //!<
  LastReasonStopMaxAnalog4              = 103,  //!<
  LastReasonStopCurrentMotor            = 104,  //!< "Предел тока двигателя"
  LastReasonStopOutOfSyncMotor          = 105,  //!< "Рассинхронизация ПВЭД"
  LastReasonStopHardwareVsd             = 106,  //!< "Аппаратные защиты ЧРП"
};

/*!
 * \brief Флаги статусных регистров инвертора
 */
enum enVsdFlags {
  VSD_STATUS_STARTED              = 0,      //!< Запуск ПЧ
  VSD_STATUS_WAIT_RECT_START      = 1,      //!< Ожидаем запуска выпрямителя
  VSD_STATUS_STOPPED_REGISTER     = 2,      //!< Инвертор остановлен по изменению важного параметра
  VSD_STATUS_STOPPED_EXTERNAL     = 3,      //!< Инвертор остановлен по команде извне
  VSD_STATUS_WAIT_RECT_STOP       = 4,      //!< Ожидание остнова выпрямителя
  VSD_STATUS_FAULT_STOPPED        = 5,      //!< Остановлен по причине FAULT
  VSD_STATUS_RIGHT_DIRECTION      = 6,      //!< Правое направление вращения
  VSD_STATUS_I_LIMIT              = 7,      //!< Токоограничение
  VSD_STATUS_ULOW                 = 8,      //!< Недостаточно напряжения
  VSD_STATUS_STOPPED_ALARM        = 9,      //!< Остановлен аварийно
  VSD_STATUS_UD_LOW_FAULT         = 10,     //!< Остановлен по снижению напряжения на шин
  VSD_STATUS_UD_HIGH_FAULT        = 11,     //!< Остановлен по превышению напряжения на шине
  VSD_STATUS_TO_STOP_MODE         = 12,     //!< Режим плавной остановки двигателя
  VSD_STATUS_UIN_ASYM             = 13,     //!< Остановлен по несимметрии входного напряжения
  VSD_STATUS_URECT_SHORTCIRCUIT   = 14,     //!< Остановлен по КЗ от выпрямителя
  VSD_STATUS_RESERVED             = 15,     //!< Резерв

  VSD_STATUS_FC_I2T_ERR           = 16,     //!< Сработала токовая тепловая защита инвертора
  VSD_STATUS_AST_ERR              = 17,     //!< Система автонастройки не смогла определить параметры линии
  VSD_STATUS_I_LIMIT_FAST         = 18,     //!< Превышение порога мгновенного токоограничения
  VSD_STATUS_CURRENT_OPT          = 19,     //!< Включена оптимизация по току
  VSD_STATUS_POWER_OPT            = 20,     //!< Включена оптимизация по выходной мощности
  VSD_STATUS_OPT_DONE             = 21,     //!< Оптимизация состоялась
  VSD_STATUS_M_TYPE0              = 22,     //!< Бит типа двигателя 0
  VSD_STATUS_M_TYPE1              = 23,     //!< Бит типа двигателя 1
  VSD_STATUS_DISCHARGE_ON         = 24,     //!< Задействован режим разряда шины
  VSD_STATUS_DISCAHRGE            = 25,     //!< Режим разряда шины работает
  VSD_STATUS_DISCHARGE_ERR        = 26,     //!< Ошибка режима разряда шины
  VSD_STATUS_VC_ERR               = 27,     //!< Ошибка векторного режима
  VSD_STATUS_I_FAST_ERR           = 28,     //!< Быстрая токовая защита двигателя
  VSD_STATUS_M_I2T_ERR            = 29,     //!< Токовая защита двигателя (перегруз)
  VSD_STATUS_OWERPWM1             = 30,     //!< OverPWM тип 1
  VSD_STATUS_OWERPWM2             = 31,     //!< OverPWM тип 2

  VSD_STATUS_3                    = 32,

  VSD_STATUS_IMAX                 = 48,     //!< Превышен максимальный ток инвертора
  VSD_STATUS_IZ                   = 49,     //!< Детектирован разностный ток выходных фаз (утечка)
  VSD_STATUS_AN_MON               = 50,     //!< Ошибка монитора питания  аналоговой цепи контроллера
  VSD_STATUS_CTR_MON              = 51,     //!< Ошибка монитора питания цифровой цепи контроллера
  VSD_STATUS_CLK_MON              = 52,     //!< Ошибка монитора питания часов контроллера
  VSD_STATUS_MB_MON               = 53,     //!< Ошибка монитора платы измерений
  VSD_STATUS_DRV0                 = 54,     //!< Ошибка драйвера 0
  VSD_STATUS_DRV1                 = 55,     //!< Ошибка драйвера 1
  VSD_STATUS_DRV2                 = 56,     //!< Ошибка драйвера 2
  VSD_STATUS_TEST                 = 57,     //!< Включен тестовый режим контроллера ПЧ

  VSD_THYR_ABC_STATE              = 64,     //!< Направление чередования фаз
  VSD_THYR_1                      = 65,
  VSD_THYR_ERR_STATE              = 66,     //!< Ошибка синхронизации с сетью
  VSD_THYR_CHARGE_STATE           = 67,     //!< Производится заряд ёмкости инвертора
  VSD_THYR_STARTED_STATE          = 68,     //!< Ёмкость заряжена
  VSD_THYR_SYNC_STATE             = 69,     //!< Первая попытка синхронизации
  VSD_THYR_BOUNCE_WAIT            = 70,     //!< Ожидание времени антидребезга
  VSD_THYR_ERR_SHORTCIRQUIT       = 71,     //!< Ошибка по КЗ на инверторе

  VSD_STATUS_READY                = 80,     //!< Готов
  VSD_STATUS_UNDERLOAD            = 81,     //!< Недогруз
  VSD_STATUS_RESISTANCE           = 82,     //!< Низкое Rиз
  VSD_STATUS_UNDERVOLTAGE         = 83,     //!< Низкое напряжение
  VSD_STATUS_OVERVOLTAGE          = 84,     //!< Высокое напряжение
  VSD_STATUS_RUN_COUNT            = 85,     //!< Прев. кол-во пусков
  VSD_STATUS_OVERHEAT_FILTER      = 86,     //!< Перегрев фильтра
  VSD_STATUS_MONOMETR             = 87,     //!< Конт манометр
  VSD_STATUS_OVERHEAT_MOTOR       = 88,     //!< Выс. Температура
  VSD_STATUS_OVERVIBRATION        = 89,     //!< Выс. Вибрация
  VSD_STATUS_PRESSURE             = 90,     //!< Низ. Давление
  VSD_STATUS_PRESSURE_Z           = 91,     //!< Низкое Pзатр
  VSD_STATUS_AI_0                 = 92,     //!< Доп. аналог вход 0
  VSD_STATUS_TURBINE              = 93,     //!< Турбинное вращение
  VSD_STATUS_OVERHEAT_IGBT        = 94,     //!< Перегрев IGBT
};

/*!
 * \brief Список частотных приводов поддерживаемых в ПО
 */
enum enVsdType {
  VSD_TYPE_NULL = 0,                        //!< Неизвестный или отсутствует
  VSD_TYPE_NOVOMET,                         //!< ЧРП Новомет
  VSD_TYPE_DANFOSS,                         //!< ЧРП Danfoss
  VSD_TYPE_ETALON                           //!< ЧРП Эталон
};

/*!
 * \brief Список счётчиков электроэнергии поддерживаемых в ПО
 */
enum enEmType {
  EM_TYPE_NONE = 0,                         //!< Неизвестный или отсутствует
  EM_TYPE_SET4TM,                           //!< СЭТ-4ТМ03
};

#define NUM_PI 3.14159265358979323846
#define SQRT_3 1.732050807568877
#define NOM_VOLTAGE 220

#endif /* DEFINE_H_ */

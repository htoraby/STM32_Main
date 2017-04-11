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
  TYPE_DATA_COIL   = 7,
  TYPE_DATA_ARRAY_INT16 = 8,
  TYPE_DATA_ARRAY_INT32 = 9,
  TYPE_DATA_INT16_4 = 10,
  TYPE_DATA_STR,
  TYPE_DATA_UINT16_B,
  TYPE_DATA_UINT32_B,
  TYPE_DATA_UINT16_4,
  TYPE_DATA_UINT32_4,
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
  OPERATION_WRITE_DELAY = 4,                //!< Параметр с задержкой после записи
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
  TYPE_DHS_BORETS_1,                        //!< Борец
  TYPE_DHS_ELEKTON_2,                       //!< Электон
  TYPE_DHS_ELEKTON_3,                       //!< Эталон ТМСН-2
  TYPE_DHS_ETALON,                          //!< Эталон ТМСН-3
  TYPE_DHS_IRZ,                             //!< Ирз
  TYPE_DHS_ORION,                           //!< Орион
  TYPE_DHS_PIC_V2,                          //!< Шлюмберже
  TYPE_DHS_SCAD,                            //!< Скад
  TYPE_DHS_SCAN,                            //!< Скан
  TYPE_DHS_TRIOL,                           //!< Триол
  TYPE_DHS_ZENITH,                           //!< Зенит
  TYPE_DHS_SNG,                             //!< СНГ
  TYPE_DHS_BORETS_2,                        //!< Борец СПТ-2
  TYPE_DHS_TRANSFER,                        //!< РН-Transfer
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
  CCS_CONDITION_BREAK,                      //!< Торможение
  CCS_CONDITION_RUN,                    //!< Запуск
  CCS_CONDITION_WORK                         //!< Работа
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
  LastReasonRunApvPower                 = 23,   //!< "АПВ по защите: Отключение питания сети"
  // Дополнительные причины пуска
  LastReasonRunApvMinAnalog3            = 100,  //!< "АПВ по защите: Ан.вх.№3 min"
  LastReasonRunApvMinAnalog4            = 101,  //!< "АПВ по защите: Ан.вх.№4 min"
  LastReasonRunApvMaxAnalog3            = 102,  //!< "АПВ по защите: Ан.вх.№3 max"
  LastReasonRunApvMaxAnalog4            = 103,  //!< "АПВ по защите: Ан.вх.№4 max"
  LastReasonRunApvCurrentMotor          = 104,  //!< "АПВ по защите: Предел тока двигателя"
  LastReasonRunApvOutOfSyncMotor        = 105,  //!< "АПВ по защите: Рассинхронизация ПВЭД"
  LastReasonRunApvResistIsolation       = 106,  //!< "АПВ по защите: Сопротивление изоляции"
  LastReasonRunApvHardwareVsd           = 107,  //!< "АПВ по защите: Аппаратные защиты ЧРП"
  LastReasonRunDigital1                 = 108,  //!< "Сигнал цифр.вх.1"
  LastReasonRunDigital2                 = 109,  //!< "Сигнал цифр.вх.2"
  LastReasonRunDigital3                 = 110,  //!< "Сигнал цифр.вх.3"
  LastReasonRunDigital4                 = 111,  //!< "Сигнал цифр.вх.4"
  LastReasonRunApvDigital1              = 112,  //!< "АПВ по защите: Цифр.вх.1"
  LastReasonRunApvDigital2              = 113,  //!< "АПВ по защите: Цифр.вх.2"
  LastReasonRunApvDigital3              = 114,  //!< "АПВ по защите: Цифр.вх.3"
  LastReasonRunApvDigital4              = 115,  //!< "АПВ по защите: Цифр.вх.4"
  LastReasonRunApvImbalanceCurIn        = 116,  //!< "АПВ по защите: Дисбаланс входных токов"
  LastReasonRunApvFlowDischarge         = 117,  //!< "АПВ по защите: Минимальный расход"
  LastReasonRunApvPressureDischarge     = 118,  //!< "АПВ по защите: Pmax на выкиде насоса"
  LastReasonRunApvOverheatInputFilter   = 119,  //!< "АПВ по защите: Перегрев фильтра"
  LastReasonRunAdaptationVector         = 120,  //!<

  LastReasonRunLast
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
  LastReasonStopNoVoltage               = 30,   //!< "Отключение питания сети"
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
  LastReasonStopProtDigital1,                   //!<
  LastReasonStopProtDigital2,                   //!<
  LastReasonStopProtDigital3,                   //!<
  LastReasonStopProtDigital4,                   //!<
  LastReasonStopVsdNoConnect,                   //!< "ЧРП. Нет связи"
  LastReasonStopImbalanceCurIn,                 //!< "Дисбаланс входных токов"
  LastReasonStopVsdErrControl,                  //!< "ЧРП. Ошибка управления"
  LastReasonStopOvernumberOfStart,              //!< "Превышение числа пусков"
  LastReasonStopFlowDischarge,                  //!< "Минимальный расход"
  LastReasonStopPressureDischarge,              //!< "Pmax на выкиде насоса"
  LastReasonStopOverheatInputFilter,            //!< "Перегрев фильтра"
  LastReasonStopAdaptationVector,               //!< "Режим автоадаптации"
  LastReasonStopLast                    = 65535,
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
  EM_TYPE_ABB,                              //!< ABB
  EM_TYPE_ABB_A44,                          //!< ABB A44
};

/*!
 * \brief Список типов остановов
 */
enum enTypeStop {
  TYPE_STOP_SMOTH = 0,
  TYPE_STOP_ALARM
};

/*!
 * \brief Список действий по сигналу с цифрового входа
 */
enum enDiAction
{
  DI_ACTION_NONE,       //!< Нет
  DI_ACTION_STOP,       //!< Стоп
  DI_ACTION_START,      //!< Пуск
  DI_ACTION_REVERSE,    //!< Реверс
  DI_ACTION_PROTECTION, //!< Защита
  DI_ACTION_BUTTON,     //!< Кнопка оператора
  DI_ACTION_WARNING,    //!< Предупреждение
  DI_ACTION_LAST
};

/*!
 * \brief Список состояний СУ для включения релейного выхода
 */
enum enDoAction
{
  DO_ACTION_NONE,       //!< Нет
  DO_ACTION_STOP,       //!< Стоп
  DO_ACTION_RUN,        //!< Работа
  DO_ACTION_RESTART,    //!< АПВ
  DO_ACTION_BLOCK,      //!< Блок
  DO_ACTION_LAST
};

#define NUM_PI 3.14159265358979323846
#define SQRT_3 1.732050807568877
#define NOM_VOLTAGE 220

#endif /* DEFINE_H_ */

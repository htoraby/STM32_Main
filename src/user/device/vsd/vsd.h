/*
 * vsd.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_H_
#define VSD_H_

#include "device.h"
#include "vsd_log.h"

#define VSD_CMD_NUMBER_REPEATS 10 //!< Количество повторов
#define VSD_CMD_TIMEOUT 1000      //!< Время ожидания подтверждения команды

/*!
 * \brief Флаги статусных регистров инвертора
 */
enum enStatus {
  // STATUS_WORD_1
  VSD_STATUS_STARTED              = 0,      //!< Запуск ПЧ
  VSD_STATUS_WAIT_RECT_START      = 1,      //!< Ожидаем запуска выпрямителя
  VSD_STATUS_STOPPED_REGISTER     = 2,      //!< Инвертор остановлен по изменению важного параметра
  VSD_STATUS_STOPPED_EXTERNAL     = 3,      //!< Инвертор остановлен по команде извне
  VSD_STATUS_WAIT_RECT_STOP       = 4,      //!< Ожидание остнова выпрямителя
  VSD_STATUS_FAULT_STOPPED        = 5,      //!< Остановлен по причине FAULT
  VSD_STATUS_1_RESERVED_6         = 6,      //!< Резерв
  VSD_STATUS_I_LIMIT              = 7,      //!< Токоограничение
  VSD_STATUS_ULOW                 = 8,      //!< Недостаточно напряжения
  VSD_STATUS_STOPPED_ALARM        = 9,      //!< Остановлен аварийно
  VSD_STATUS_UD_LOW_FAULT         = 10,     //!< Остановлен по снижению напряжения на шин
  VSD_STATUS_UD_HIGH_FAULT        = 11,     //!< Остановлен по превышению напряжения на шине
  VSD_STATUS_TO_STOP_MODE         = 12,     //!< Режим плавной остановки двигателя
  VSD_STATUS_UIN_ASYM             = 13,     //!< Остановлен по несимметрии входного напряжения
  VSD_STATUS_URECT_SHORTCIRCUIT   = 14,     //!< Остановлен по КЗ от выпрямителя
  VSD_STATUS_RESERVED             = 15,     //!< Резерв
  // STATUS_WORD_2
  VSD_STATUS_FC_IT_ERR            = 16,     //!< Сработала токовая тепловая защита инвертора
  VSD_STATUS_AST_ERR              = 17,     //!< Система автонастройки не смогла определить параметры линии
  VSD_STATUS_I_LIMIT_FAST         = 18,     //!< Превышение порога мгновенного токоограничения
  VSD_STATUS_CURRENT_OPT          = 19,     //!< Включена оптимизация по току
  VSD_STATUS_POWER_OPT            = 20,     //!< Включена оптимизация по выходной мощности
  VSD_STATUS_OPT_DONE             = 21,     //!< Оптимизация состоялась
  VSD_STATUS_2_RESERVED_22        = 22,     //!< Резерв
  VSD_STATUS_2_RESERVED_23        = 23,     //!< Резерв
  VSD_STATUS_DISCHARGE_ON         = 24,     //!< Задействован режим разряда шины
  VSD_STATUS_DISCAHRGE            = 25,     //!< Режим разряда шины работает
  VSD_STATUS_DISCHARGE_ERR        = 26,     //!< Ошибка режима разряда шины
  VSD_STATUS_VC_ERR               = 27,     //!< Ошибка векторного режима
  VSD_STATUS_M_I_FAST_ERR         = 28,     //!< Быстрая токовая защита двигателя
  VSD_STATUS_M_I2T_ERR            = 29,     //!< Токовая защита двигателя (перегруз)
  VSD_STATUS_2_RESERVED_30        = 30,     //!< Резерв
  VSD_STATUS_2_RESERVED_31        = 31,     //!< Резерв
  // STATUS_WORD_3
  VSD_STATUS_RIGHT_DIRECTION      = 32,     //!< Правое направление вращения
  VSD_STATUS_OWERPWM1             = 33,     //!< OverPWM тип 1
  VSD_STATUS_OWERPWM2             = 34,     //!< OverPWM тип 2
  VSD_STATUS_M_TYPE0              = 35,     //!< Бит типа двигателя 0
  VSD_STATUS_M_TYPE1              = 36,     //!< Бит типа двигателя 1
  VSD_STATUS_RES_TYPE0            = 37,     //!< Бит типа противорезонанса
  VSD_STATUS_RES_TYPE1            = 38,     //!< Бит типа противорезонанса
  // STATUS_WORD_7
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
  VSD_STATUS_INV_FLT_TEMP_LINK    = 58,     //!< Ошибка связи с термодатчиками
  VSD_STATUS_INV_FLT_TEMP         = 59,     //!< Перегрев силовых модулей
  // STATUS_WORD_5
  VSD_STATUS_ABC_STATE            = 64,     //!< Направление чередования фаз
  VSD_STATUS_1                    = 65,
  VSD_STATUS_ERR_STATE            = 66,     //!< Ошибка синхронизации с сетью
  VSD_STATUS_CHARGE_STATE         = 67,     //!< Производится заряд ёмкости инвертора
  VSD_STATUS_STARTED_STATE        = 68,     //!< Ёмкость заряжена
  VSD_STATUS_SYNC_STATE           = 69,     //!< Первая попытка синхронизации
  VSD_STATUS_BOUNCE_WAIT          = 70,     //!< Ожидание времени антидребезга
  VSD_STATUS_ERR_SHORTCIRQUIT     = 71,     //!< Ошибка по КЗ на инверторе
  // STATUS_WORD_4
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
  VSD_STATUS_ERR_19               = 94,     //!< Ошибка 19
  VSD_STATUS_ERR_31               = 95,     //!< Ошибка 31
  // STATUS_WORD_6
  VSD_STATUS_NO_CONNECT           = 250,     //!< Нет связи с ЧРП
};

/*!
 * \brief Коды аварий ЧРП
 */
enum enAlarmVsd {
  VSD_ALARM_NONE                  = 0,      //!< Нет аварии
  VSD_ALARM_NO_CONNECT            = 1,      //!< Нет связи с ЧРП

  VSD_ALARM_STOPPED_REGISTER      = 1002,   //!< Инвертор остановлен по изменению важного параметра
  VSD_ALARM_STOPPED_EXTERNAL      = 1003,   //!< Инвертор остановлен по команде извне
  VSD_ALARM_FAULT_STOPPED         = 1005,   //!< Остановлен по причине FAULT
  VSD_ALARM_I_LIMIT               = 1007,   //!< Токоограничение
  VSD_ALARM_ULOW                  = 1008,   //!< Недостаточно напряжения
  VSD_ALARM_STOPPED_ALARM         = 1009,   //!< Остановлен аварийно
  VSD_ALARM_UD_LOW_FAULT          = 1010,   //!< Остановлен по снижению напряжения на шин
  VSD_ALARM_UD_HIGH_FAULT         = 1011,   //!< Остановлен по превышению напряжения на шине
  VSD_ALARM_UIN_ASYM              = 1013,   //!< Остановлен по несимметрии входного напряжения
  VSD_ALARM_URECT_SHORTCIRCUIT    = 1014,   //!< Остановлен по КЗ от выпрямителя

  VSD_ALARM_FC_IT_ERR             = 1016,   //!< Сработала токовая тепловая защита инвертора
  VSD_ALARM_AST_ERR               = 1017,   //!< Система автонастройки не смогла определить параметры линии
  VSD_ALARM_I_LIMIT_FAST          = 1018,   //!< Превышение порога мгновенного токоограничения
  VSD_ALARM_DISCHARGE_ERR         = 1026,   //!< Ошибка режима разряда шины
  VSD_ALARM_VC_ERR                = 1027,   //!< Ошибка векторного режима
  VSD_ALARM_M_I_FAST_ERR          = 1028,   //!< Быстрая токовая защита двигателя
  VSD_ALARM_M_I2T_ERR             = 1029,   //!< Токовая защита двигателя (перегруз)

  VSD_ALARM_IMAX                  = 1032,   //!< Превышен максимальный ток инвертора
  VSD_ALARM_IZ                    = 1033,   //!< Детектирован разностный ток выходных фаз (утечка)
  VSD_ALARM_AN_MON                = 1034,   //!< Ошибка монитора питания  аналоговой цепи контроллера
  VSD_ALARM_CTR_MON               = 1035,   //!< Ошибка монитора питания цифровой цепи контроллера
  VSD_ALARM_CLK_MON               = 1036,   //!< Ошибка монитора питания часов контроллера
  VSD_ALARM_MB_MON                = 1037,   //!< Ошибка монитора платы измерений
  VSD_ALARM_DRV0                  = 1038,   //!< Ошибка драйвера 0
  VSD_ALARM_DRV1                  = 1039,   //!< Ошибка драйвера 1
  VSD_ALARM_DRV2                  = 1040,   //!< Ошибка драйвера 2
  VSD_ALARM_TEST                  = 1041,   //!< Включен тестовый режим контроллера ПЧ
  VSD_ALARM_INV_FLT_TEMP_LINK     = 1042,   //!< Ошибка связи с термодатчиками
  VSD_ALARM_INV_FLT_TEMP          = 1043,   //!< Перегрев силовых модулей

  VSD_ALARM_ABC_STATE             = 1048,   //!< Направление чередования фаз
  VSD_ALARM_ERR_STATE             = 1049,   //!< Ошибка синхронизации с сетью
  VSD_ALARM_ERR_SHORTCIRQUIT      = 1050,   //!< Ошибка по КЗ на инверторе

  VSD_ALARM_A_28                  = 2000,   //!< Проверка тормоза A28
  VSD_ALARM_A_69                  = 2001,   //!< Темп. силовой платы A69
  VSD_ALARM_A_14                  = 2002,   //!< Пробой на землю A14
  VSD_ALARM_A_65                  = 2003,   //!< Темп. платы управления А65
  VSD_ALARM_A_17                  = 2004,   //!< Упр. слово ТО А17
  VSD_ALARM_A_13                  = 2005,   //!< Превыш тока А13
  VSD_ALARM_A_12                  = 2006,   //!< Предел момента A12
  VSD_ALARM_A_11                  = 2007,   //!< Перегрев термист. двигателя А11
  VSD_ALARM_A_10                  = 2008,   //!< Превышение ETR двигателя А10
  VSD_ALARM_A_9                   = 2009,   //!< Перегрузка инвертера А9
  VSD_ALARM_A_8                   = 2010,   //!< Пониж. напряжение пост. тока А8
  VSD_ALARM_A_7                   = 2011,   //!< Повыш. напряжение пост. тока А7
  VSD_ALARM_A_16                  = 2012,   //!< Короткое замыкание А16
  VSD_ALARM_A_33                  = 2013,   //!< Отказ из-за броска тока А33
  VSD_ALARM_A_4                   = 2014,   //!< Обрыв фазы питания А4
  VSD_ALARM_AAD                   = 2015,   //!< Сбой ААД
  VSD_ALARM_A_2                   = 2016,   //!< Ошибка действующего нуля А2
  VSD_ALARM_A_38                  = 2017,   //!< Внутренний отказ А38
  VSD_ALARM_A_26                  = 2018,   //!< Перегрузка тормоза А26
  VSD_ALARM_A_30                  = 2019,   //!< Обрыв фазы U A30
  VSD_ALARM_A_31                  = 2020,   //!< Обрыв фазы V A31
  VSD_ALARM_A_32                  = 2021,   //!< Обрыв фазы W A32
  VSD_ALARM_A_34                  = 2022,   //!< Отказ Fieldbus А34
  VSD_ALARM_A_47                  = 2023,   //!< Низкое напряжение питания 24В А47
  VSD_ALARM_A_36                  = 2024,   //!< Отказ питания А36
  VSD_ALARM_A_48                  = 2025,   //!< Низкое напряжение питания 1,8В А48
  VSD_ALARM_A_25                  = 2026,   //!< Тормозной резистор А25
  VSD_ALARM_A_27                  = 2027,   //!< Тормозной IGBT А27
  VSD_ALARM_A_67                  = 2028,   //!< Смена доп.устройства А67
  VSD_ALARM_A_80                  = 2029,   //!< Провод иниц. А80
  VSD_ALARM_A_68                  = 2030,   //!< Безоп. останов А68
  VSD_ALARM_A_63                  = 2031,   //!< Мала эффективность механич. тормоза А63

  VSD_ALARM_SERVICE_TRIP          = 2032,     //!< Сервисная защита
  VSD_ALARM_HI_TEMP_DISCHARGE     = 2041,     //!< Высокая температура разряда
  VSD_ALARM_START_ERROR           = 2042,     //!< Ошибка пуска
  VSD_ALARM_SPEED_LIMIT           = 2043,     //!< Предел скорости
  VSD_ALARM_EXTERNAL_BLOCK        = 2044,     //!< Внешняя блокировка
  VSD_ALARM_EXTERNAL_DEVICE       = 2045,     //!< Недопустимое сочетание дополнительных устройств
  VSD_ALARM_PROT_DEVICE           = 2046,     //!< Дополнительное защитное устройство отсутствует
  VSD_ALARM_KTY                   = 2049,     //!< Ошибка KTY
  VSD_ALARM_FAN                   = 2050,     //!< Ошибка вентиляторов
  VSD_ALARM_ECB                   = 2051,     //!< Ошибка ECB
  VSD_ALARM_A_59                  = 2057,     //!< Предел по току А59
  VSD_ALARM_A_90                  = 2061,     //!< Отказ энкодера A90
  VSD_ALARM_A_74                  = 2062,     //!< Термистор PTC А74
  VSD_ALARM_A_72                  = 2063,     //!< Опасный отказ

  VSD_ALARM_UNDERLOAD             = 3001,     //!< Недогруз
  VSD_ALARM_OVERLOAD              = 3002,     //!< Перегруз
  VSD_ALARM_RESISTANCE            = 3003,     //!< Низкое Rиз
  VSD_ALARM_UNDERVOLTAGE          = 3004,     //!< Низкое напряжение
  VSD_ALARM_OVERVOLTAGE           = 3005,     //!< Высокое напряжение
  VSD_ALARM_OVERVOLTAGE_DC        = 3006,     //!< Высокое U сил. Цепи
  VSD_ALARM_UNDERVOLTAGE_DC       = 3007,     //!< Низкое U сил. цепи
  VSD_ALARM_RUN_COUNT             = 3008,     //!< Прев. кол-во пусков
  VSD_ALARM_OVERHEAT_IGBT         = 3009,     //!< Перегрев IGBT
  VSD_ALARM_OVERHEAT_FILTER       = 3010,     //!< Перегрев фильтра
  VSD_ALARM_PROT                  = 3011,     //!< Защита ЧРП
  VSD_ALARM_SUPPLY_DRIVERS        = 3012,     //!< Питание драйверов
  VSD_ALARM_MONOMETR              = 3013,     //!< Конт манометр
  VSD_ALARM_AI_0                  = 3014,     //!< Доп. аналог вход 0
  VSD_ALARM_SEQUENCE_PHASE        = 3015,     //!< Чередование фаз
  VSD_ALARM_OVERHEAT_MOTOR        = 3016,     //!< Выс. Температура
  VSD_ALARM_OVERVIBRATION         = 3017,     //!< Выс. Вибрация
  VSD_ALARM_PRESSURE              = 3018,     //!< Низ. Давление
  VSD_ALARM_19                    = 3019,     //!< Ошибка 19
  VSD_ALARM_PRESSURE_Z            = 3020,     //!< Низкое Pзатр
  VSD_ALARM_IMBALANCE_CURRENT     = 3021,     //!< Дисбаланс токов
  VSD_ALARM_IMBALANCE_VOLTAGE     = 3022,     //!< Дисбаланс напряж.
  VSD_ALARM_TURBINE               = 3023,     //!< Турбинное вращение
  VSD_ALARM_24                    = 3024,     //!< Прочие ошибки
  VSD_ALARM_FAILURE_SUPPLY        = 3025,     //!< Авария питания
  VSD_ALARM_DOOR                  = 3026,     //!< Открыта дверь
  VSD_ALARM_LOST_SUPPLY           = 3027,     //!< Пропало питание
  VSD_ALARM_CONDENSATOR           = 3028,     //!< Нет заряда конденс.
  VSD_ALARM_TERISTORS             = 3029,     //!< Авария тиристоров
  VSD_ALARM_CURRENT_LIMIT         = 3030,     //!< Токоограничение
  VSD_ALARM_31                    = 3031,     //!< Ошибка номер 31
  VSD_ALARM_32                    = 3032,     //!< По подхвату
  VSD_ALARM_AUTO_STOP             = 3033,     //!< Авто останов
  VSD_ALARM_MANUAL_STOP           = 3034,     //!< Ручной останов
  VSD_ALARM_REMOTE_STOP           = 3035,     //!< Внешний останов
  VSD_ALARM_AUTO_RUN              = 3036,     //!< Автоматич. пуск
  VSD_ALARM_MANUAL_RUN            = 3037,     //!< Ручной пуск
  VSD_ALARM_REMOTE_RUN            = 3038,     //!< Внешний пуск
  VSD_ALARM_RESTART_COUNT         = 3039,     //!< По количеству АПВ
  VSD_ALARM_MEMORY                = 3040,     //!< Ошибка ОЗУ
  VSD_ALARM_41                    = 3041,     //!< Отключен
  VSD_ALARM_DI                    = 3042,     //!< Отказ дискр. вх.
  VSD_ALARM_ADC                   = 3043,     //!< Отказ АЦП
  VSD_ALARM_ANALOG_SUPPLY         = 3044,     //!< Аналог. Питание
  VSD_ALARM_SENSOR_SUPPLY         = 3045,     //!< Питание датчиков
  VSD_ALARM_EEPROM                = 3046,     //!< Ошибка EEPROM
  VSD_ALARM_NOT_READY             = 3047,     //!< ПЧ не готов
  VSD_ALARM_SETPOINT              = 3048,     //!< Сбой уставок
  VSD_ALARM_BLOCK_RUN             = 3049      //!< Блокировка пуска
};

/*!
 * \brief Коды предупреждений ЧРП ЧРП
 */
enum enWarningVsd {
  VSD_WARNING_NONE                = 0,      //!< Нет предупреждения
  VSD_WARNING_NO_CONNECT          = 1,      //!< Потеря связи с ЧРП

  VSD_WARNING_W_28                = 2000,   //!< Проверка тормоза W28
  VSD_WARNING_W_69                = 2001,   //!< Темп. силовой платы W69
  VSD_WARNING_W_14                = 2002,   //!< Пробой на землю W14
  VSD_WARNING_W_65                = 2003,   //!< Темп. платы управления W65
  VSD_WARNING_W_17                = 2004,   //!< Упр. слово ТО W17
  VSD_WARNING_W_13                = 2005,   //!< Превыш тока W13
  VSD_WARNING_W_12                = 2006,   //!< Предел момента W12
  VSD_WARNING_W_11                = 2007,   //!< Перегрев термист. двигателя W11
  VSD_WARNING_W_10                = 2008,   //!< Превышение ETR двигателя W10
  VSD_WARNING_W_9                 = 2009,   //!< Перегрузка инвертера W9
  VSD_WARNING_W_8                 = 2010,   //!< Пониж. напряжение пост. тока W8
  VSD_WARNING_W_7                 = 2011,   //!< Повыш. напряжение пост. тока W7
  VSD_WARNING_W_6                 = 2012,   //!< Пониж. напряжение пост. тока W6
  VSD_WARNING_W_5                 = 2013,   //!< Повыш. напряжение пост. тока W5
  VSD_WARNING_W_4                 = 2014,   //!< Обрыв фазы питания W4
  VSD_WARNING_W_3                 = 2015,   //!< Нет двигателя W3
  VSD_WARNING_W_2                 = 2016,   //!< Ошибка действующего нуля W2
  VSD_WARNING_W_1                 = 2017,   //!< Низкое напряжение источника 10В W1
  VSD_WARNING_W_26                = 2018,   //!< Перегрузка тормоза W26
  VSD_WARNING_W_25                = 2019,   //!< Тормозной резистор W25
  VSD_WARNING_W_27                = 2020,   //!< Тормозной IGBT W27
  VSD_WARNING_W_49                = 2021,   //!< Предел скорости W49
  VSD_WARNING_W_34                = 2022,   //!< Отказ Fieldbus W34
  VSD_WARNING_W_47                = 2023,   //!< Низкое напряжение питания 24В W47
  VSD_WARNING_W_36                = 2024,   //!< Отказ питания W36
  VSD_WARNING_W_59                = 2025,   //!< Предел по току W59
  VSD_WARNING_W_66                = 2026,   //!< Низкая температура W66
  VSD_WARNING_W_64                = 2027,   //!< Предел по напряжению W64
  VSD_WARNING_W_90                = 2028,   //!< Отказ энкодера W90
  VSD_WARNING_W_62                = 2029,   //!< Пр вых. част W62
  VSD_WARNING_W_68                = 2030,   //!< Безоп. останов W68
  VSD_WARNING_EXT                 = 2031,   //!< Расширенное слово состояния
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

/*!
 * \brief Базовый класс ЧРП
 * Наследник класса Device использующий его структуру и методы хранения банка
 * параметров имеющий свою карту параметров класс, использующийся как базовый
 * для классов конкретных ЧРП
 */
class Vsd: public Device
{

public:
  Vsd();
  virtual ~Vsd();

  /*!
   * \brief Метод инициализации внутреннего массива параметров ЧРП
   */
  void initParameters();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  /*!
   * \brief Метод задания типа двигателя в массив параметров
   * \param value - тип двигателя: VSD_MOTOR_TYPE_ASYNC = 0, VSD_MOTOR_TYPE_VENT = 1
   * \return 0 - задание успешно, !0 - ошибки
   */
  int setMotorType(float value);

  /*!
   * \brief setMotorFrequency
   * \param value
   * \return
   */
  virtual int setMotorFrequency(float value);

  /*!
   * \brief setMotorCurrent
   * \param value
   * \return
   */
  virtual int setMotorCurrent(float value);

  /*!
   * \brief setMotorVoltage
   * \param value
   * \return
   */
  virtual int setMotorVoltage(float value);

  /*!
   * \brief setMotorSpeed
   * \param value
   * \return
   */
  virtual int setMotorSpeed(float value);

  /*!
   * \brief setLimitsMotor
   * \return
   */
  virtual void setLimitsMotor();

  // РЕЖИМЫ ПУСКА
  /*!
   * \brief Метод настройки толчкового режима
   * \return
   */
  virtual int onRegimePush();

  /*!
   * \brief Метод выключения толчкогового режима
   * \return
   */
  virtual int offRegimePush();

  /*!
   * \brief onRegimeSwing
   * \return
   */
  virtual int onRegimeSwing();

  /*!
   * \brief offRegimeSwing
   * \return
   */
  virtual int offRegimeSwing();

  /*!
   * \brief Метод настройки режима подхвата
   * \return
   */
  virtual int onRegimePickup();

  /*!
   * \brief offRegimePickup
   * \return
   */
  virtual int offRegimePickup();

  /*!
   * \brief onRegimeSkipFreq
   * \return
   */
  virtual int onRegimeSkipFreq();

  /*!
   * \brief offRegimeSkipFreq
   * \return
   */
  virtual int offRegimeSkipFreq();

  /*!
   * \brief onRegimeAutoAdaptation
   * \return
   */
  virtual int onRegimeAutoAdaptation();

  /*!
   * \brief offRegimeAutoAdaptation
   * \return
   */
  virtual int offRegimeAutoAdaptation();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
  /*!
   * \brief Функция записи в регистр управления двигателем
   * \param value
   * \return 0 - задание успешно, № - ошибки
   */
  int setVsdControl(float value);
  
  /*!
   * \brief Функция записи в регистр направления вращения
   * \param value - направление вращения
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setRotation(float value);  
  
  /*!
   * \brief Функция записи в регистр минимальной частоты
   * \param value - минимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  int setMinFrequency(float value);

  /*!
   * \brief setLimitsMinFrequence функция присвоения минимальной границы
   * Максимальной частоты и уставки частоты минимальной частоте
   * \param value
   * \return
   */
  int setLimitsMinFrequence(float value);
  
  /*!
   * \brief Функция записи в регистр максимальной частоты
   * \param value - максимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  int setMaxFrequency(float value);

  /*!
   * \brief setLimitsMaxFrequence функция присвоения максимальной границы
   * минимальной частоты и уставки частоты максимальной частоте частоте
   * \param value
   * \return
   */
  int setLimitsMaxFrequence(float value);

  /*!
   * \brief Функция записи в регистр уставки частоты
   * \param value - задаваемая частота
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setFrequency(float value);

  /*!
   * \brief setTimeSpeedUp
   * \param value
   * \return 
   */
  virtual int setTimeSpeedUp(float value);
  
  /*!
   * \brief setTimeSpeedDown
   * \param value
   * \return 
   */
  virtual int setTimeSpeedDown(float value);
  
  /*!
   * \brief Функция записи в регистр частоты коммутации (Частота ШИМ)
   * \param value задаваемая частота коммутации (Частота ШИМ)
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSwitchingFrequency(float value);

  /*!
   * \brief setSwitchingFrequencyCode
   * \param value
   * \return
   */
  virtual int setSwitchingFrequencyCode(float value);

  /*!
   * \brief setSwitchingFrequencyMode
   * \param value
   * \return
   */
  virtual int setSwitchingFrequencyMode(float value);

  /*!
   * \brief setResonanceRemoveSource
   * \param value
   * \return
   */
  virtual int setResonanceRemoveSource(float value);

  /*!
   * \brief setOutFilter
   * \param value
   * \return
   */
  virtual int setOutFilter(float value);

  // НАСТРОЙКА U/f
  /*!
   * \brief setUf_f1
   * \param value
   * \return
   */
  int setUf_f1(float value);

  /*!
   * \brief setUf_f2
   * \param value
   * \return
   */
  int setUf_f2(float value);

  /*!
   * \brief setUf_f3
   * \param value
   * \return
   */
  int setUf_f3(float value);

  /*!
   * \brief setUf_f4
   * \param value
   * \return
   */
  int setUf_f4(float value);

  /*!
   * \brief setUf_f5
   * \param value
   * \return
   */
  int setUf_f5(float value);

  /*!
   * \brief setUf_f6
   * \param value
   * \return
   */
  int setUf_f6(float value);

  /*!
   * \brief setUf_U1
   * \param value
   * \return
   */
  int setUf_U1(float value);

  /*!
   * \brief setUf_U2
   * \param value
   * \return
   */
  int setUf_U2(float value);

  /*!
   * \brief setUf_U3
   * \param value
   * \return
   */
  int setUf_U3(float value);

  /*!
   * \brief setUf_U4
   * \param value
   * \return
   */
  int setUf_U4(float value);

  /*!
   * \brief setUf_U5
   * \param value
   * \return
   */
  int setUf_U5(float value);

  /*!
   * \brief setUf_U6
   * \param value
   * \return
   */
  int setUf_U6(float value);

  /*!
   * \brief calcUfCharacteristic
   * \return
   */
  virtual int calcUfCharacteristicU(float value);

  /*!
   * \brief calcUfCharacteristicF
   * \param value
   * \return
   */
  virtual int calcUfCharacteristicF(float value);

  /*!
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  int setCurrentLim(float value);

  /*!
   * \brief Функция записи в регистр суммарной индуктивности
   * \param induct - задаваемая суммарная индуктивности
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSumInduct(float value);


  int setUfU(uint16_t id, float value);



  /*! Читаемые параметры ЧРП */
  /*!
   * \brief Метод получения текущего значения частоты из массива параметров
   * \return Текущее значение частоты
   */
  float getCurrentFreq();

  /*!
   * \brief Метод получения уставки частоты из массива параметров
   * \return Текущее значение частоты
   */
  float getSetpointFreq();

  /*!
   * \brief Метод получения номинальной частоты двигателя из массива параметров
   * \return Метод получения номинальной частоты двигателя
   */
  float getNominalFreq();

  /*!
   * \brief getTypeStop
   * \return
   */
  float getTypeStop();

  /*! Команды и операции */

  /*!
   * \brief Метод запуска ЧРП
   * \return Код результата операции
   */
  virtual int start();

  /*!
   * \brief Метод останова ЧРП
   * \param type - тип останова
   * \return
   */
  virtual int stop(float type);

  /*!
   * \brief Метод проверки запуска ЧРП
   * \return true - запуск, false - нет
   */
  virtual bool checkStart();

  /*!
   * \brief Метод проверки останова ЧРП
   * \return true - останов, false - нет
   */
  virtual bool checkStop();

  virtual int resetBlock();

  /*!
   * \brief Метод проверки флага в регистре статуса инвертора
   * \param bit - проверяемый бит
   * \return 0 - бит не установлен 1 - бит установлен
   */
  virtual bool checkStatusVsd(uint8_t bit);

  virtual void processingRegimeRun();



  /*!
   * \brief onRegimeJarring
   * \return
   */
  virtual int onRegimeJarring();

  /*!
   * \brief offRegimeJarring
   * \return
   */
  virtual int offRegimeJarring();

  /*!
   * \brief resetSetpoints
   * \return
   */
  virtual int resetSetpoints();

  /*!
   * \brief Функция вычисления текущего cos ЧРП по активной и полной мощности
   * \return
   */
  int calcVsdCos();

  /*!
   * \brief writeToDevice
   */
  virtual void writeToDevice(int id, float value);

  VsdLog *log() const { return log_; }

protected:
  VsdLog *log_;

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */

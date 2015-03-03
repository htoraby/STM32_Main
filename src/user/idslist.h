#ifndef IDSLIST
#define IDSLIST

enum enID
{
  CCS_BEGIN,                                /// 0.
  CCS_MOTOR_SPEED_NOW,                      /// 1.Текущие обороты двигателя
  CCS_MOTOR_CURRENT_PHASE_1_NOW,            /// 2.Ток двигателя фаза 1
  CCS_MOTOR_CURRENT_PHASE_2_NOW,            /// 3.Ток двигателя фаза 2
  CCS_MOTOR_CURRENT_PHASE_3_NOW,            /// 4.Ток двигателя фаза 3
  CCS_RESERVE_1,
  CCS_MOTOR_CURRENT_AVARAGE_NOW,            /// 6.Ток двигателя средний
  CCS_MOTOR_LOAD_NOW,                       /// 7.Текущая загрузка двигателя
  CCS_TURBO_ROTATION,                       /// 8.Текущее турбинное вращение
  CCS_RESISTANCE_ISOLATION,                 /// 9.Сопротивление изоляции в системе
  CCS_PHASE_ROTATION,                       /// 10.Порядок чередования фаз
  CCS_CONDITION,                            /// 11.Обобщенное состояние защит КСУ
  CCS_RUNNING_TYPE,                         /// 12.Режим пуска двигателя
  CCS_WORKING_MODE,                         /// 13.Режим работы
  CCS_LOG_PERIOD_NORMAL,                    /// Период записи
  CCS_LOG_PERIOD_FAST,                      /// Период экстренной записи
  CCS_LOG_PERIOD_DHS,                       /// Период записи ТМС
  CCS_MOTOR_CURRENT_HH,                     /// Ток холостого хода
  CCS_TRANS_NOMINAL_POWER,                  /// Мощность ТМПН
  CCS_TRANS_NOMINAL_CURRENT,                /// Номинальный ток ТМПН
  CCS_TRANS_NOMINAL_VOLTAGE,                /// Номинальное напряжение ТМПН
  CCS_TRANS_CABLE_LENGHT,                   /// Длина кабеля
  CCS_TRANS_CABLE_CROSS,                    /// Сечение кабеля
  CCS_TRANS_NEED_VOLTAGE_TAP_OFF,           /// Необходимое напряжение отпайки
  CCS_TRANS_VOLTAGE_TAP_OFF,                /// Напряжение отпайки
  CCS_DHS_TYPE,                             /// Тип ТМС
  CCS_ACCESS_LEVEL,                         /// Уровень доступа к контроллеру
  CCS_CMD_STOP,                                 /// Команда остановки
  CCS_CMD_START,                                /// Команда запуска
  /// Состояние ЧРП
  CCS_VSD_CONDITION,
  /// Производительность насоса
  CCS_PUMP_CAPACITY,
  /// Напор ЭЦН
  CCS_PUMP_HEAD,
  /// Глубина спуска
  CCS_DEPTH,
  /// Дисбаланс токов двигателя
  CCS_MOTOR_IMBALANCE_CURRENT,
  /// Среднее входное напряжение
  CCS_SUPPLY_INPUT_VOLTAGE_AVERAGE,
  /// Дисбаланс входного напряжения
  CCS_SYPPLY_IMBALANCE_VOLTAGE,

  /// Время работы в периодическом режиме,
  CCS_RGM_PERIODIC_TIME,
  /// Время до окончания работы в периодическом режиме
  CCS_RGM_PERIODIC_TIME_END,
  /// Защиты
  /// Защиты по сети
  /// Защита "Превышение питания сети"
  /// Защита "Превышение питания сети", Включение/выключение уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_MODE,
  /// Защита "Превышение питания сети", Реакция защиты уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_REACTION,
  /// Защита "Превышение питания сети", Задержка активации уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY,
  /// Защита "Превышение питания сети", Задержка срабатывания уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY,
  /// Защита "Превышение питания сети", Задержка АПВ уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY,
  /// Защита "Превышение питания сети", Количество АПВ уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT,
  /// Защита "Превышение питания сети", Автосброс счетчика АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET,
  /// Защита "Превышение питания сети", Уставка срабатывания защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT,
  /// Защита "Превышение питания сети" Уставка срабатывания АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT,
  /// Защита "Превышение питания сети" Уставка параметр защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER,
  /// Защита "Превышение питания сети" Уставка параметр защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER_2,
  /// Защита "Превышение питания сети" Текущий параметр состояние автомата
  CCS_PROT_SUPPLY_OVERVOLTAGE_STATE,
  /// Защита "Превышение питания сети" Текущий параметр Фиксированное время
  CCS_PROT_SUPPLY_OVERVOLTAGE_TIME,
  /// Защита "Превышение питания сети" Текущий параметр Количество АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT,
  /// Защита "Превышение питания сети" Текущий параметр Время первого срабатывания АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET_COUNT,
  /// Защиты "Снижение питания сети"
  CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_REACTION,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER_2,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT,
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET_COUNT,
  /// Защита "Дисбаланс входных токов"
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_REACTION,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER_2,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT,
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET_COUNT,
  /// Защиты "Дисбаланс входных токов"
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_REACTION,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_ACTIV_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_DELAY,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_LIMIT,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_RESET,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_SETPOINT,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_SETPOINT,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PARAMETER,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PARAMETER_2,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_STATE,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TIME,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_COUNT,
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_RESET_COUNT,
  /// Защиты двигателя
  /// Защита "Перегруз" (ЗП)
  CCS_PROT_MOTOR_OVERLOAD_MODE,
  CCS_PROT_MOTOR_OVERLOAD_REACTION,
  CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY,
  CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET,
  CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT,
  CCS_PROT_MOTOR_OVERLOAD_PARAMETER,
  CCS_PROT_MOTOR_OVERLOAD_PARAMETER_2,
  CCS_PROT_MOTOR_OVERLOAD_STATE,
  CCS_PROT_MOTOR_OVERLOAD_TIME,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT,
  CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET_COUNT,
  /// Защита "Недогруз" (ЗП)
  CCS_PROT_MOTOR_UNDERLOAD_MODE,
  CCS_PROT_MOTOR_UNDERLOAD_REACTION,
  CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY,
  CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET,
  CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT,
  CCS_PROT_MOTOR_UNDERLOAD_PARAMETER,
  CCS_PROT_MOTOR_UNDERLOAD_PARAMETER_2,
  CCS_PROT_MOTOR_UNDERLOAD_STATE,
  CCS_PROT_MOTOR_UNDERLOAD_TIME,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT,
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET_COUNT,
  /// Защита "Дисбаланс токов двигателя"
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_REACTION,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER_2,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT,
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET_COUNT,
  /// Защита "Турбинное вращение"
  CCS_PROT_MOTOR_ASYNC_MODE,
  CCS_PROT_MOTOR_ASYNC_REACTION,
  CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY,
  CCS_PROT_MOTOR_ASYNC_TRIP_DELAY,
  CCS_PROT_MOTOR_ASYNC_RESTART_DELAY,
  CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT,
  CCS_PROT_MOTOR_ASYNC_RESTART_RESET,
  CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT,
  CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT,
  CCS_PROT_MOTOR_ASYNC_PARAMETER,
  CCS_PROT_MOTOR_ASYNC_PARAMETER_2,
  CCS_PROT_MOTOR_ASYNC_STATE,
  CCS_PROT_MOTOR_ASYNC_TIME,
  CCS_PROT_MOTOR_ASYNC_RESTART_COUNT,
  CCS_PROT_MOTOR_ASYNC_RESTART_RESET_COUNT,
  /// Защиты ТМС
  /// Защита "Давление на приёме насоса"
  CCS_PROT_DHS_PRESSURE_INTAKE_MODE,
  CCS_PROT_DHS_PRESSURE_INTAKE_REACTION,
  CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY,
  CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET,
  CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT,
  CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER,
  CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER_2,
  CCS_PROT_DHS_PRESSURE_INTAKE_STATE,
  CCS_PROT_DHS_PRESSURE_INTAKE_TIME,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT,
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET_COUNT,
  /// Защита "Температура двигателя"
  CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_REACTION,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER_2,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT,
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET_COUNT,
  /// Защита "Сопротивление изоляции"
  CCS_PROT_DHS_RESISTANCE_MODE,
  CCS_PROT_DHS_RESISTANCE_REACTION,
  CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY,
  CCS_PROT_DHS_RESISTANCE_TRIP_DELAY,
  CCS_PROT_DHS_RESISTANCE_RESTART_DELAY,
  CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT,
  CCS_PROT_DHS_RESISTANCE_RESTART_RESET,
  CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT,
  CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT,
  CCS_PROT_DHS_RESISTANCE_PARAMETER,
  CCS_PROT_DHS_RESISTANCE_PARAMETER_2,
  CCS_PROT_DHS_RESISTANCE_STATE,
  CCS_PROT_DHS_RESISTANCE_TIME,
  CCS_PROT_DHS_RESISTANCE_RESTART_COUNT,
  CCS_PROT_DHS_RESISTANCE_RESTART_RESET_COUNT,
  /// Защита "Вибрация двигателя"
  CCS_PROT_DHS_VIBRATION_MODE,
  CCS_PROT_DHS_VIBRATION_REACTION,
  CCS_PROT_DHS_VIBRATION_ACTIV_DELAY,
  CCS_PROT_DHS_VIBRATION_TRIP_DELAY,
  CCS_PROT_DHS_VIBRATION_RESTART_DELAY,
  CCS_PROT_DHS_VIBRATION_RESTART_LIMIT,
  CCS_PROT_DHS_VIBRATION_RESTART_RESET,
  CCS_PROT_DHS_VIBRATION_TRIP_SETPOINT,
  CCS_PROT_DHS_VIBRATION_RESTART_SETPOINT,
  CCS_PROT_DHS_VIBRATION_PARAMETER,
  CCS_PROT_DHS_VIBRATION_PARAMETER_2,
  CCS_PROT_DHS_VIBRATION_STATE,
  CCS_PROT_DHS_VIBRATION_TIME,
  CCS_PROT_DHS_VIBRATION_RESTART_COUNT,
  CCS_PROT_DHS_VIBRATION_RESTART_RESET_COUNT,
  /// Защиты по аналоговым входам
  /// Защиты Аналоговый вход 1
  CCS_PROT_AI_1_MODE,
  CCS_PROT_AI_1_REACTION,
  CCS_PROT_AI_1_ACTIV_DELAY,
  CCS_PROT_AI_1_TRIP_DELAY,
  CCS_PROT_AI_1_RESTART_DELAY,
  CCS_PROT_AI_1_RESTART_LIMIT,
  CCS_PROT_AI_1_RESTART_RESET,
  CCS_PROT_AI_1_TRIP_SETPOINT,
  CCS_PROT_AI_1_RESTART_SETPOINT,
  CCS_PROT_AI_1_PARAMETER,
  CCS_PROT_AI_1_PARAMETER_2,
  CCS_PROT_AI_1_STATE,
  CCS_PROT_AI_1_TIME,
  CCS_PROT_AI_1_RESTART_COUNT,
  CCS_PROT_AI_1_RESTART_RESET_COUNT,
  /// Защиты Аналоговый вход 2
  CCS_PROT_AI_2_MODE,
  CCS_PROT_AI_2_REACTION,
  CCS_PROT_AI_2_ACTIV_DELAY,
  CCS_PROT_AI_2_TRIP_DELAY,
  CCS_PROT_AI_2_RESTART_DELAY,
  CCS_PROT_AI_2_RESTART_LIMIT,
  CCS_PROT_AI_2_RESTART_RESET,
  CCS_PROT_AI_2_TRIP_SETPOINT,
  CCS_PROT_AI_2_RESTART_SETPOINT,
  CCS_PROT_AI_2_PARAMETER,
  CCS_PROT_AI_2_PARAMETER_2,
  CCS_PROT_AI_2_STATE,
  CCS_PROT_AI_2_TIME,
  CCS_PROT_AI_2_RESTART_COUNT,
  CCS_PROT_AI_2_RESTART_RESET_COUNT,
  /// Защиты Аналоговый вход 3
  CCS_PROT_AI_3_MODE,
  CCS_PROT_AI_3_REACTION,
  CCS_PROT_AI_3_ACTIV_DELAY,
  CCS_PROT_AI_3_TRIP_DELAY,
  CCS_PROT_AI_3_RESTART_DELAY,
  CCS_PROT_AI_3_RESTART_LIMIT,
  CCS_PROT_AI_3_RESTART_RESET,
  CCS_PROT_AI_3_TRIP_SETPOINT,
  CCS_PROT_AI_3_RESTART_SETPOINT,
  CCS_PROT_AI_3_PARAMETER,
  CCS_PROT_AI_3_PARAMETER_2,
  CCS_PROT_AI_3_STATE,
  CCS_PROT_AI_3_TIME,
  CCS_PROT_AI_3_RESTART_COUNT,
  CCS_PROT_AI_3_RESTART_RESET_COUNT,
  /// Защиты Аналоговый вход 4
  CCS_PROT_AI_4_MODE,
  CCS_PROT_AI_4_REACTION,
  CCS_PROT_AI_4_ACTIV_DELAY,
  CCS_PROT_AI_4_TRIP_DELAY,
  CCS_PROT_AI_4_RESTART_DELAY,
  CCS_PROT_AI_4_RESTART_LIMIT,
  CCS_PROT_AI_4_RESTART_RESET,
  CCS_PROT_AI_4_TRIP_SETPOINT,
  CCS_PROT_AI_4_RESTART_SETPOINT,
  CCS_PROT_AI_4_PARAMETER,
  CCS_PROT_AI_4_PARAMETER_2,
  CCS_PROT_AI_4_STATE,
  CCS_PROT_AI_4_TIME,
  CCS_PROT_AI_4_RESTART_COUNT,
  CCS_PROT_AI_4_RESTART_RESET_COUNT,
  /// Защиты по дискретным входам
  /// Защита дискретный вход 1
  CCS_PROT_DI_1_MODE,
  CCS_PROT_DI_1_REACTION,
  CCS_PROT_DI_1_ACTIV_DELAY,
  CCS_PROT_DI_1_TRIP_DELAY,
  CCS_PROT_DI_1_RESTART_DELAY,
  CCS_PROT_DI_1_RESTART_LIMIT,
  CCS_PROT_DI_1_RESTART_RESET,
  CCS_PROT_DI_1_TRIP_SETPOINT,
  CCS_PROT_DI_1_RESTART_SETPOINT,
  CCS_PROT_DI_1_PARAMETER,
  CCS_PROT_DI_1_PARAMETER_2,
  CCS_PROT_DI_1_STATE,
  CCS_PROT_DI_1_TIME,
  CCS_PROT_DI_1_RESTART_COUNT,
  CCS_PROT_DI_1_RESTART_RESET_COUNT,
  /// Защита дискретный вход 2
  CCS_PROT_DI_2_MODE,
  CCS_PROT_DI_2_REACTION,
  CCS_PROT_DI_2_ACTIV_DELAY,
  CCS_PROT_DI_2_TRIP_DELAY,
  CCS_PROT_DI_2_RESTART_DELAY,
  CCS_PROT_DI_2_RESTART_LIMIT,
  CCS_PROT_DI_2_RESTART_RESET,
  CCS_PROT_DI_2_TRIP_SETPOINT,
  CCS_PROT_DI_2_RESTART_SETPOINT,
  CCS_PROT_DI_2_PARAMETER,
  CCS_PROT_DI_2_PARAMETER_2,
  CCS_PROT_DI_2_STATE,
  CCS_PROT_DI_2_TIME,
  CCS_PROT_DI_2_RESTART_COUNT,
  CCS_PROT_DI_2_RESTART_RESET_COUNT,
  /// Защита дискретный вход 3
  CCS_PROT_DI_3_MODE,
  CCS_PROT_DI_3_REACTION,
  CCS_PROT_DI_3_ACTIV_DELAY,
  CCS_PROT_DI_3_TRIP_DELAY,
  CCS_PROT_DI_3_RESTART_DELAY,
  CCS_PROT_DI_3_RESTART_LIMIT,
  CCS_PROT_DI_3_RESTART_RESET,
  CCS_PROT_DI_3_TRIP_SETPOINT,
  CCS_PROT_DI_3_RESTART_SETPOINT,
  CCS_PROT_DI_3_PARAMETER,
  CCS_PROT_DI_3_PARAMETER_2,
  CCS_PROT_DI_3_STATE,
  CCS_PROT_DI_3_TIME,
  CCS_PROT_DI_3_RESTART_COUNT,
  CCS_PROT_DI_3_RESTART_RESET_COUNT,
  /// Защита дискретный вход 4
  CCS_PROT_DI_4_MODE,
  CCS_PROT_DI_4_REACTION,
  CCS_PROT_DI_4_ACTIV_DELAY,
  CCS_PROT_DI_4_TRIP_DELAY,
  CCS_PROT_DI_4_RESTART_DELAY,
  CCS_PROT_DI_4_RESTART_LIMIT,
  CCS_PROT_DI_4_RESTART_RESET,
  CCS_PROT_DI_4_TRIP_SETPOINT,
  CCS_PROT_DI_4_RESTART_SETPOINT,
  CCS_PROT_DI_4_PARAMETER,
  CCS_PROT_DI_4_PARAMETER_2,
  CCS_PROT_DI_4_STATE,
  CCS_PROT_DI_4_TIME,
  CCS_PROT_DI_4_RESTART_COUNT,
  CCS_PROT_DI_4_RESTART_RESET_COUNT,
  /// Защиты прочие
  /// Защита "Превышения числа пусков"
  CCS_PROT_OTHER_LIMIT_RESTART_MODE,
  CCS_PROT_OTHER_LIMIT_RESTART_REACTION,
  CCS_PROT_OTHER_LIMIT_RESTART_ACTIV_DELAY,
  CCS_PROT_OTHER_LIMIT_RESTART_TRIP_DELAY,
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_DELAY,
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_LIMIT,
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_RESET,
  CCS_PROT_OTHER_LIMIT_RESTART_TRIP_SETPOINT,
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_SETPOINT,
  CCS_PROT_OTHER_LIMIT_RESTART_PARAMETER,
  CCS_PROT_OTHER_LIMIT_RESTART_PARAMETER_2,
  CCS_PROT_OTHER_LIMIT_RESTART_STATE,
  CCS_PROT_OTHER_LIMIT_RESTART_TIME,
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT,
  /// Защита "Блокировка силового отсека"
  CCS_PROT_OTHER_LOCK_DOOR_MODE,
  CCS_PROT_OTHER_LOCK_DOOR_REACTION,
  CCS_PROT_OTHER_LOCK_DOOR_ACTIV_DELAY,
  CCS_PROT_OTHER_LOCK_DOOR_TRIP_DELAY,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_DELAY,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_LIMIT,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_RESET,
  CCS_PROT_OTHER_LOCK_DOOR_TRIP_SETPOINT,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_SETPOINT,
  CCS_PROT_OTHER_LOCK_DOOR_PARAMETER,
  CCS_PROT_OTHER_LOCK_DOOR_PARAMETER_2,
  CCS_PROT_OTHER_LOCK_DOOR_STATE,
  CCS_PROT_OTHER_LOCK_DOOR_TIME,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_COUNT,
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_RESET_COUNT,
  /// Защита "Аппаратные защиты ЧРП"
  CCS_PROT_OTHER_VSD_MODE,
  CCS_PROT_OTHER_VSD_REACTION,
  CCS_PROT_OTHER_VSD_ACTIV_DELAY,
  CCS_PROT_OTHER_VSD_TRIP_DELAY,
  CCS_PROT_OTHER_VSD_RESTART_DELAY,
  CCS_PROT_OTHER_VSD_RESTART_LIMIT,
  CCS_PROT_OTHER_VSD_RESTART_RESET,
  CCS_PROT_OTHER_VSD_TRIP_SETPOINT,
  CCS_PROT_OTHER_VSD_RESTART_SETPOINT,
  CCS_PROT_OTHER_VSD_PARAMETER,
  CCS_PROT_OTHER_VSD_PARAMETER_2,
  CCS_PROT_OTHER_VSD_STATE,
  CCS_PROT_OTHER_VSD_TIME,
  CCS_PROT_OTHER_VSD_RESTART_COUNT,
  CCS_PROT_OTHER_VSD_RESTART_RESET_COUNT,
  /// Защита "Контроль УКИ"
  CCS_PROT_OTHER_IMB_MODE,
  CCS_PROT_OTHER_IMB_REACTION,
  CCS_PROT_OTHER_IMB_ACTIV_DELAY,
  CCS_PROT_OTHER_IMB_TRIP_DELAY,
  CCS_PROT_OTHER_IMB_RESTART_DELAY,
  CCS_PROT_OTHER_IMB_RESTART_LIMIT,
  CCS_PROT_OTHER_IMB_RESTART_RESET,
  CCS_PROT_OTHER_IMB_TRIP_SETPOINT,
  CCS_PROT_OTHER_IMB_RESTART_SETPOINT,
  CCS_PROT_OTHER_IMB_PARAMETER,
  CCS_PROT_OTHER_IMB_PARAMETER_2,
  CCS_PROT_OTHER_IMB_STATE,
  CCS_PROT_OTHER_IMB_TIME,
  CCS_PROT_OTHER_IMB_RESTART_COUNT,
  CCS_PROT_OTHER_IMB_RESTART_RESET_COUNT,
  /// Режим периодический
  CCS_RGM_PERIODIC_MODE,
  /// Режим периодический
  CCS_RGM_PERIODIC_RUN_TIMER,
  /// Режим периодический
  CCS_RGM_PERIODIC_STOP_TIMER,
  /// Режим чередования частот
  CCS_RGM_ALTERNATION_FREQ_MODE,
  /// Режим чередования частот частота 1
  CCS_RGM_ALTERNATION_FREQ_FREQ_1,
  /// Режим чередования частот время работы на частоте 1
  CCS_RGM_ALTERNATION_FREQ_TIMER_1,
  /// Режим чередования частот уставка ЗСП
  CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_1,
  /// Режим чередования частот частота 2
  CCS_RGM_ALTERNATION_FREQ_FREQ_2,
  /// Режим чередования частот время работы на частоте 2
  CCS_RGM_ALTERNATION_FREQ_TIMER_2,
  /// Режим чередования частот уставка ЗСП 2
  CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_2,
  /// Режим программного изменения частоты
  CCS_RGM_CHANGE_FREQ_MODE,
  /// Режим программного изменения частоты
  CCS_RGM_CHANGE_FREQ_BEGIN_FREQ,
  /// Режим программного изменения частоты
  CCS_RGM_CHANGE_FREQ_END_FREQ,
  /// Режим программного изменения частоты
  CCS_RGM_CHANGE_FREQ_TIMER_FREQ,
  /// Параметры ВНР
  CCS_RGM_MODE_DATE_TIME,
  /// Параметры ВНР
  CCS_RGM_MODE_CURRENT,
  /// Параметры ВНР
  CCS_RGM_MODE_VOLTAGE,
  /// Параметры ВНР
  CCS_RGM_MODE_LOAD,
  /// Режим поддержания параметра включение
  CCS_RGM_MAINTENANCE_PARAM_MODE,
  /// Режим поддержания параметра параметра
  CCS_RGM_MAINTENANCE_PARAM,
  /// Режим поддержания параметра зависимость
  CCS_RGM_MAINTENANCE_PARAM_DEPENDENCE,
  /// Режим поддержания параметра поддержка значения
  CCS_RGM_MAINTENANCE_PARAM_SETPOINT,
  /// Режим поддержания параметра период регулирования
  CCS_RGM_MAINTENANCE_PARAM_PERIOD,
  /// Режим поддержания параметра пропорциональный коэффициент
  CCS_RGM_MAINTENANCE_PARAM_PROP,
  /// Режим поддержания параметра интегральный коэффициент
  CCS_RGM_MAINTENANCE_PARAM_INT,
  /// Режим поддержания параметра мин рабочая частота
  CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ,
  /// Режим поддержания параметра макс рабочая частота
  CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ,
  /// Режим Учёт нагрева кабеля вкл/откл
  CCS_RGM_HEAT_CABLE_MODE,
  /// Режим Учёт нагрева кабеля сопротивление при 20с
  CCS_RGM_HEAT_CABLE_RESISTANCE_20,
  /// Режим Учёт нагрева кабеля сопротивление при 80с
  CCS_RGM_HEAT_CABLE_RESISTANCE_80,
  /// Режим Учёт нагрева кабеля номинальный ток кабеля
  CCS_RGM_HEAT_CABLE_MODE_CURRENT,
  /// Режим Учёт нагрева кабеля время нагрева
  CCS_RGM_HEAT_CABLE_MODE_DELAY_REATION,
  /// Режим оптимизации напряжения вкл/откл
  CCS_RGM_OPTIM_VOLTAGE_MODE,
  /// Режим оптимизации напряжения диапазон
  CCS_RGM_OPTIM_VOLTAGE_SCALE,
  /// Режим оптимизации напряжения шаг
  CCS_RGM_OPTIM_VOLTAGE_STEP,
  /// Режим оптимизации напряжения первый цикл после запуска
  CCS_RGM_OPTIM_VOLTAGE_DELAY_REACTION,
  /// Режим оптимизации напряжения периодичность оптимизации
  CCS_RGM_OPTIM_VOLTAGE_DELAY_RESTART,
  /// Режим прокачки газа вкл/откл
  CCS_RGM_PUMP_GAS_MODE,
  /// Режим прокачки газа
  CCS_RGM_PUMP_GAS_SETPOINT,
  /// Режим прокачки газа
  CCS_RGM_PUMP_GAS_DELAY_REACTION,
  /// Режим прокачки газа
  CCS_RGM_PUMP_GAS_LIMIT_RESTART,
  /// Режим ограничения тока вкл/откл
  CCS_RGM_CURRENT_LIMIT_MODE,
  /// Режим ограничения тока уставка
  CCS_RGM_CURRENT_LIMIT_SETPOINT,
  /// Режим ограничения тока шаг снижения частоты
  CCS_RGM_CURRENT_LIMIT_STEPDOWN,
  /// Режим ограничения тока задержка срабатывания
  CCS_RGM_CURRENT_LIMIT_DELAY_REACTION,
  /// Режим ограничения тока задержка АПВ
  CCS_RGM_CURRENT_LIMIT_DELAY_RESTART,
  /// Коэффициент корректировки выходного тока фаза U
  CCS_COEF_OUT_CURRENT_U,
  /// Коэффициент корректировки выходного тока фаза V
  CCS_COEF_OUT_CURRENT_V,
  /// Коэффициент корректировки выходного тока фаза W
  CCS_COEF_OUT_CURRENT_W,
  /// Коэффициент корректировки сопротивления изоляции
  CCS_COEF_RESISTANCE_ISOLATION,
  /// Номер ЦДНГ
  CCS_NUMBER_CDNG,
  /// Номер куста
  CCS_NUMBER_BUSH,
  /// Номер скважины
  CCS_NUMBER_WELL,
  /// Количество пусков
  CCS_COUNT_START,
  /// Количество отключений по перезрузу
  CCS_PROT_OVERLOAD_COUNT_STOP,
  /// Количество отключений по недогрузу
  CCS_PROT_UNDERLOAD_COUNT_STOP,
  /// Количество отключений по другим защитам
  CCS_PROT_OTHER_COUNT_STOP,
  /// Количество АПВ после остановов по напряжению
  CCS_PROT_VOLTAGE_COUNT_RESTART,
  /// Количество АПВ после остановов по перегрузу
  CCS_PROT_OVERLOAD_COUNT_RESTART,
  /// Количество АПВ после остановов по недогрузу
  CCS_PROT_UNDERLOAD_COUNT_RESTART,
  /// Количествог АПВ после остановов по дисбалансу токов
  CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART,
  CCS_GENERAL_RUN_DATE_TIME,
  /// Общее время простоя СУ
  CCS_GENERAL_STOP_DATE_TIME,
  /// Текущая наработка
  CCS_RUN_DATE_TIME,
  /// Текущий простой
  CCS_STOP_DATE_TIME,
  /// Время до изменения режима
  CCS_RGM_TIMER,
  /// Время до запуска
  CCS_RESTART_TIMER,
  /// Дата время последнего включения
  CCS_LAST_RUN_DATE_TIME,
  /// Причина последнего пуска
  CCS_LAST_RUN_REASON,
  /// Дата время последнего отключения
  CCS_LAST_STOP_DATE_TIME,
  /// Причина носледнего отключения
  CCS_LAST_STOP_REASON,
  /// Дата время изменения частоты
  CCS_PREVIEW_FREQUENCY_DATE_TIME,
  /// Предпоследняя частота
  CCS_PREVIEW_FREQUENCY,
  /// Тип КСУ
  CCS_TYPE_CCS,
  /// Код производителя КСУ
  CCS_CODE_PRODUCTION_CCS,
  /// Заводской номер КСУ
  CCS_NUM_PRODUCTION_CCS,
  /// Версия ПО КСУ
  CCS_VERSION_SW_CCS,
  /// Версия ПО МС
  CCS_VERSION_SW_MS,
  /// Версия ПО ПЧ
  CCS_VERSION_SW_VSD,
  /// Дата изготовления КСУ
  CCS_DATE_PRODUCTION_CCS,
  /// Дата установки ПО КСУ
  CCS_DATE_INSTALL_SW_CCS,
  /// ДАта генерации ПО КСУ
  CCS_DATE_CREATE_SW_CCS,
  /// Дата установки ПО ПЧ
  CCS_DATE_INSTALL_SW_VSD,
  /// Тип СУ
  CCS_TYPE_SU,
  /// Код завода изготовителя СУ
  CCS_CODE_PRODUCTION_SU,
  /// Заводской номер СУ
  CCS_NUM_PRODUCTION_SU,
  /// Дата изготовления СУ
  CCS_DATE_PRODUCTION_SU,
  /// Примечание 1
  CCS_NOTE_1,
  /// Примечание 2
  CCS_NOTE_2,
  /// Примечание 3
  CCS_NOTE_3,
  /// Примечание 4
  CCS_NOTE_4,
  /// Примечание 5
  CCS_NOTE_5,
  /// Примечание 6
  CCS_NOTE_6,
  /// Примечание 7
  CCS_NOTE_7,
  /// Примечание 8
  CCS_NOTE_8,
  /// Примечание 9
  CCS_NOTE_9,
  /// Примечание 10
  CCS_NOTE_10,
  /// Дата время в формате POSIX
  CCS_DATE_TIME,
  /// Дата время год
  CCS_DATE_TIME_YEAR,
  /// Дата время месяц
  CCS_DATE_TIME_MONTH,
  /// Дата время дни
  CCS_DATE_TIME_DAY,
  /// Дата время часы
  CCS_DATE_TIME_HOUR,
  /// Дата время минуты
  CCS_DATE_TIME_MIN,
  /// Дата время секунды
  CCS_DATE_TIME_SEC,
  /// Дата время мс
  CCS_DATE_TIME_MS,
  /// Язык интерфейса
  CCS_LANGUAGE,
  /// Включение системы паролей
  CCS_PASSWORD_ENEBLE,
  /// Текущий уровень доступа
  CCS_PASSWORD_LEVEL,
  /// Пароль 1 уровня
  CCS_PASSWORD_LEVEL_1,
  /// Пароль 2 уровня
  CCS_PASSWORD_LEVEL_2,
  /// Пароль 3 уровня
  CCS_PASSWORD_LEVEL_3,
  /// Пароль 4 уровня
  CCS_PASSWORD_LEVEL_4,
  /// Пароль 5 уровня
  CCS_PASSWORD_LEVEL_5,
  /// Протокол телемеханики
  CCS_SCADA_TYPE,
  /// Версия протокола телемеханики
  CCS_SCADA_CODE,
  /// Адресу СУ в телемеханике
  CCS_SCADA_ADDRESS,
  /// Скорость опроса в телемеханике
  CCS_SCADA_BYTERATE,
  /// Чётность в телемеханике
  CCS_SCADA_PARITY,
  /// Количество стоп-бит в телемеханике
  CCS_SCADA_STOPBIT,
  /// Задержка ответа в телемеханкие
  CCS_SCADA_DELAY,
  /// Единицы измерения давления
  CCS_UNIT_PRESSURE,
  /// Единицы измерения давления
  CCS_UNIT_TEMPERATURE,
  /// Единицы измерения вибрации
  CCS_UNIT_VIBRATION,
  /// Единицы измерения длины
  CCS_UNIT_LENGHT,
  /// Единицы измерения расхода
  CCS_UNIT_FLOW,
  /// Единицы измерения площади
  CCS_UNIT_PLACE,
  CCS_PHYSIC_MIN_ADD_AI_1,									/// Физический минимнимум дополнительного аналогового входа 1
  CCS_PHYSIC_MAX_ADD_AI_1,									/// Физический максимум дополнительного аналогового входа 1
  CCS_PHYSIC_MIN_ADD_AI_2,									/// Физический минимнимум дополнительного аналогового входа 2
  CCS_PHYSIC_MAX_ADD_AI_2,									/// Физический максимум дополнительного аналогового входа 2

  /// Последний параметр
  CCS_END,

  VSD_BEGIN        = 10000,
  //// Первый параметр ЧРП
  VSD_MOTOR_TYPE,                           /// Конструкция двигателя Danfoss 1-10 Используется в интерфейсе
  VSD_MOTOR_SPEED,                          /// Скорость двигателя Danfoss 1-25 Используется в проекте, интерфейс
  VSD_MOTOR_POWER,                          /// Мощность двигателя Danfoss 1-20 Используется в проекте, интерфейс
  VSD_MOTOR_VOLTAGE,                        /// Напряжение двигателя Danfoss 1-22 Используется в проекте, интерфейс Novomet 0х0045 IREG_MOTOR_U_MAX
  VSD_MOTOR_CURRENT,                        /// Ток двигателя Danfoss 1-24 Используется в проекте, интерфейс Novomet 0х0044 IREG_MOTOR_I_MAX
  VSD_MOTOR_FREQUENCY,                      /// Частота двигателя Danfoss 1-23 Используется в проекте, интерфейс Novomet 0х0043 IREG_MOTOR_F_MAX
  VSD_MOTOR_COS_PHI,                        /// Косинус Фи Danfoss 14-43 Используется в проекте
  VSD_D_AXIS_INDUNSTANCE,                   /// Индуктивности по оси D Danfoss 1-37 Используется в проекте
  VSD_BACK_EMF,                             /// Противо эдс Danfoss 1-40 Используется в проекте
  VSD_MOTOR_POLES,                          /// Число полюсов двигателя Danfoss 1-39 Используется в проекте
  VSD_CONTROL_MOTOR,                        /// Принцип управления двигателем Danfoss 1-01 Не используется в проекте, только при конфигурировании
  VSD_CONFIG_MODE,                          /// Режим конфигурирования Danfoss 1-00 Не используется в проекте, только при конфигурировании
  VSD_ROTATION,                             /// Направления вращения Danfoss 4-10 Не используется в проекте, только при конфигурировании
  VSD_LOW_LIM_SPEED_MOTOR,                  /// Нижний предел скорости Danfoss 4-12 Гц Используется в проекте
  VSD_HIGH_LIM_SPEED_MOTOR,                 /// Верхний передел скорости Danfoss 4-14 Используется в проекте Novomet 0х0037 IREG_FREQ_REF_MAX
  VSD_FREQUENCY,                            /// Фиксированная скорость Danfoss 3-11 Уставка частоты, основной задаваемый параметр Novomet 0х0034 IREG_FREQ_REF
  VSD_TIMER_DISPERSAL,                      /// Время разгона Danfoss 3-41 % Активно используется в проекте
  VSD_TIMER_DELAY,                          /// Время замедления Danfoss 3-42 % Активно используется в проекте
  VSD_FREQUENCY_NOW,                        /// Выходная частота ПЧ
  VSD_SPEED_RPM_NOW,                        /// Обороты двигателя
  VSD_OUT_VOLTAGE_MOTOR,                    /// Напряжение на выходе ПЧ
  VSD_OUT_CURRENT_PHASE_1,                  /// Выходной ток ПЧ фаза U
  VSD_OUT_CURRENT_PHASE_2,                  /// Выходной ток ПЧ фаза V
  VSD_OUT_CURRENT_PHASE_3,                  /// Выходной ток ПЧ фаза W

  VSD_INDEX,
  //// Указатель индекса для массивных параметров
  //// Danfoss 0x0008 используется для массивов параметров

  VSD_UNIT_SPEED,
  //// Единицы измерения скорости
  //// Danfoss 0-02 Не используется в проекте, только при конфигурировании

  VSD_TORQUE_CHARACTERISTIC,
  //// Характеристика момента нагрузки
  //// Danfoss 1-03 Не используется в проекте, только при конфигурировании

  VSD_OVERLOAD_MODE,
  //// Режим перегрузки
  //// Danfoss 1-04 Не используется в проекте, только при конфигурировании

  VSD_DAMPING_GANE,
  //// Усиление демпфирования
  //// Danfoss 1-14 Не используется в проекте, только при конфигурировании

  VSD_LOW_SPEED_FILTER_TIME,
  //// Время фильтрации при низкой скорости
  //// Danfoss 1-15 Используется в режиме автоадаптации

  VSD_HIGH_SPEED_FILTER_TIME,
  //// Время фильтрации при высокой скорости
  //// Danfoss 1-16 Используется в режиме автоадаптации

  VSD_RATE_TORQUE_MOTOR,
  //// Длительный номинальный момент двигателя
  //// Danfoss 1-26 Записывается в проекте один раз

  VSD_RESISTANCE_STATOR,
  //// Сопротивление статора
  //// Danfoss 1-30 Используется в режиме автоадаптации

  VSD_UF_CHARACTERISTIC_U,
  //// Характеристика Uf - U
  //// Danfoss 1-55

  VSD_UF_CHARACTERISTIC_F,
  //// Характеристика Uf - f
  //// Danfoss 1-56

  VSD_RESONANCE_REMOVE,
  //// Подавление резонанса %
  //// Danfoss 1-64 Не используется в проекте, только при конфигурировании

  VSD_RESONANCE_TIME,
  //// Постаянная времени подавления
  //// Danfoss 1-65 Не используется в проекте, только при конфигурировании
  //// Novomet 0x0067

  VSD_MIN_CURRENT_LOW_SPEED,
  //// Минимальный ток при низкой скорости
  //// Danfoss 1-66 Не используется в проекте, только при конфигурировании

  VSD_PM_START_MODE,
  //// Тип запуска вентиля
  //// Danfoss 1-70 Не используется в проекте, только при конфигурировании

  VSD_START_DELAY,
  //// Задержка запуска
  //// Danfoss 1-71 Используется в защите от невыхода на заданную частоту

  VSD_START_FUNCTION,
  //// Функция запуска
  //// Danfoss 1-72 Не используется в проекте, только при конфигурировании

  VSD_STOP_FUNCTION,
  //// Функция при остановке
  //// Danfoss 1-80 Не используется в проекте, только при конфигурировании

  VSD_STOP_SPEED,
  //// Минимальная скорость для функции при останове
  //// Danfoss 1-82 Не используется в проекте, только при конфигурировании

  VSD_CONTROL_TERMISTOR_MTR,
  //// Контроль термистора двигателя
  //// Danfoss 1-90 Не используется в проекте, только при конфигурировании

  VSD_THERMISTOR_RESOURCE,
  //// Вход термистора
  //// Danfoss 1-93 Не используется в проекте, только при конфигурировании

  VSD_PARKING_CURRENT,
  //// Начальный ток
  //// Danfoss 2-06 Не используется в проекте, только при конфигурировании

  VSD_PARKING_TIME,
  //// Длительность действия начального тока
  //// Danfoss 2-07 В проекте есть, но отключено, используется при конфигурировании

  VSD_OVERVOLTAGE_CONTROL,
  //// Контроль перенапряжения
  //// Danfoss 2-17 Не используется в проекте, только при конфигурировании

  VSD_MIN_REFERENCE,
  //// Минимальное задание
  //// Danfoss 3-02 Не используется в проекте, только при конфигурировании

  VSD_MAX_REFERENCE,
  //// Максимальное задание
  //// Danfoss 3-03 Не используется в проекте, только при конфигурировании

  VSD_RESOURCE_TASK_1,
  //// Ресурс задания 1
  //// Danfoss 3-15 Не используется в проекте, только при конфигурировании

  VSD_RESOURCE_TASK_2,
  //// Ресурс задания 2
  //// Danfoss 3-16 Не используется в проекте, только при конфигурировании

  VSD_RESOURCE_TASK_3,
  //// Ресурс задания 3
  //// Danfoss 3-17 Не используется в проекте, только при конфигурировании

  VSD_TYPE_SPEED_CHANGE,
  //// Тип изменения скорости
  //// Danfoss 3-40 Не используется в проекте, только при конфигурировании

  VSD_TIMER_DISP_FIX_SPEED,
  //// Время разгона фиксированной скорости
  //// Danfoss 3-80 Используется в проекте один раз

  VSD_TIMER_DELAY_FIX_SPEED,
  //// Время замедления фиксированной скорости
  //// Danfoss 3-81 Используется в проекте один раз


  VSD_TORQUE_LIMIT,
  //// Ограничение момента
  //// Danfoss 4-16 % Используется в защитах ЧРП. Интерфейс

  VSD_TORQUE_LIMIT_GEN,
  //// Ограничение момента в режиме генератора
  //// Danfoss 4-17 % Не используется в проекте, только при конфигурировании

  VSD_CURRENT_LIMIT,
  //// Предел тока
  //// Danfoss 4-18 %

  VSD_MAX_OUTPUT_FREQUENCY,
  //// Максимальная выходная частота
  //// Danfoss 4-19 Используется в проекте

  VSD_MTR_FEEDBACK_LOSS_FUNC,
  //// Реакция на ошибку обратной связи
  //// Danfoss 4-30 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR_FUNCTION,
  //// Реакция на ошибку задания частоты
  //// Danfoss 4-34 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR,
  //// Величина несоответствия частота
  //// Danfoss 4-35 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR_TIMEOUT,
  //// Величина несоответствия частоты
  //// Danfoss 4-36 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR_RAMPING,
  //// Величина несоответствия частоты при разгоне
  //// Danfoss 4-37 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR_RAMP_TIME,
  //// Задержка срабатывания при несоответствии частоты при разгоне
  //// Danfoss 4-38 Не используется в проекте, только при конфигурировании

  VSD_TRACK_ERROR_AFTER_RAMP,
  //// Tracking Error After Ramping Timeout
  //// Danfoss 4-39 Не используется в проекте, только при конфигурировании

  VSD_WARNING_CURRENT_LOW,
  //// Danfoss 4-50 Не используется в проекте, только при конфигурировании

  VSD_WARNING_CURRENT_HIGH,
  //// Danfoss 4-51 Не используется в проекте, только при конфигурировании

  VSD_WARNING_SPEED_LOW,
  //// Минимальная скорость
  //// Danfoss 4-52 об/мин Не используется в проекте, только при конфигурировании

  VSD_TERMINAL_27_MODE,
  //// Режим клеммы 27
  //// Danfoss 5-01 Не используется в проекте, только при конфигурировании

  VSD_TERMINAL_29_MODE,
  //// Режим клеммы 29
  //// Danfoss 5-02 Не используется в проекте, только при конфигурировании

  VSD_DI_18,
  //// Цифровой вход 18
  //// Danfoss 5-10 Не используется в проекте, только при конфигурировании

  VSD_DI_19,
  //// Цифровой вход 19
  //// Danfoss 5-11 Не используется в проекте, только при конфигурировании

  VSD_DI_27,
  //// Цифровой вход 27
  //// Danfoss 5-12 Не используется в проекте, только при конфигурировании

  VSD_DI_32,
  //// Цифровой вход 32
  //// Danfoss 5-14 Не используется в проекте, только при конфигурировании

  VSD_DI_33,
  //// Цифровой вход 33
  //// Danfoss 5-15 Не используется в проекте, только при конфигурировании

  VSD_TERMINAL_27_DI,
  //// Клемма 27 Цифровой вход
  //// Danfoss 5-30 Не используется в проекте, только при конфигурировании

  VSD_TERMINAL_29_DI,
  //// Клемма 29 Цифровой вход
  //// Danfoss 5-31 Не используется в проекте, только при конфигурировании

  VSD_FUNCTION_RELE,
  //// Реле функций
  //// Danfoss 5-40 Не используется в проекте, только при конфигурировании

  VSD_ON_DELAY_RELAY,
  //// Нипонятно
  //// Danfoss 5-41 Не используется в проекте, только при конфигурировании

  VSD_42_AO,
  //// Клемма 42 аналоговый вход
  //// Danfoss 6-50 Не используется в проекте, только при конфигурировании

  VSD_SL_CONTROLLER_MODE,
  //// Режим контроллера SL
  //// Danfoss 13-00 Не используется в проекте, только при конфигурировании

  VSD_SL_START_EVENT,
  //// Событие запуска
  //// Danfoss 13-01 Не используется в проекте, только при конфигурировании

  VSD_SL_STOP_EVENT,
  //// Событие останова
  //// Danfoss 13-02 Не используется в проекте, только при конфигурировании

  VSD_SL_RESET,
  //// Сброс настроек компараторов
  //// Danfoss 13-03 Не используется в проекте, только при конфигурировании

  VSD_SL_10,
  //// Операнд сравнения компаратора
  //// Danfoss 13-10 Не используется в проекте, только при конфигурировании

  VSD_SL_11,
  //// Оператор сравнения компаратора
  //// Danfoss 13-11 Не используется в проекте, только при конфигурировании

  VSD_SL_12,
  //// Значение компаратора
  //// Danfoss 13-12 Не используется в проекте, только при конфигурировании

  VSD_SL_15,
  //// RS-FF Operand S
  //// Danfoss 13-15 Не используется в проекте, только при конфигурировании

  VSD_SL_16,
  //// RS-FF Operand R
  //// Danfoss 13-16 Не используется в проекте, только при конфигурировании

  VSD_SL_20,
  //// Временная задержка
  //// Danfoss 13-20 Не используется в проекте, только при конфигурировании

  VSD_SL_40,
  //// Булева переменная логического соотношения 1
  //// Danfoss 13-40 Не используется в проекте, только при конфигурировании

  VSD_SL_41,
  //// Оператор логического соотношения 1
  //// Danfoss 13-41 Не используется в проекте, только при конфигурировании

  VSD_SL_42,
  //// Булева переменная логического соотношения 2
  //// Danfoss  13-42 Не используется в проекте, только при конфигурировании

  VSD_SL_43,
  //// Logic Rule Operator 2
  //// Danfoss 13-43 Не используется в проекте, только при конфигурировании

  VSD_SL_44,
  //// Logic Rule Boolean 3
  //// Danfoss 13-44 Не используется в проекте, только при конфигурировании

  VSD_SL_51,
  //// Событие контроллера SL
  //// Danfoss 13-51 Не используется в проекте, только при конфигурировании

  VSD_SL_52,
  //// Действие контроллера SL
  //// Danfoss 13-52 Не используется в проекте, только при конфигурировании

  VSD_SWITCHING_FREQUENCY,
  //// Частота коммутации
  //// Danfoss 14-01 Используется в проекте
  //// Novomet 0х003C IREG_FREQ_PWM

  VSD_OVERMODULATION,
  //// Сверхмодуляция
  //// Danfoss 14-03 Не используется в проекте, только при конфигурировании

  VSD_DEAD_TIME_COMPENSATION,
  //// Компенсация задержки
  //// Danfoss 14-06 Не используется в проекте, только при конфигурировании

  VSD_MAINS_FAILURE,
  //// Отказ питания
  //// Danfoss 14-10 Не используется в проекте, только при конфигурировании

  VSD_MAINS_VOLTAGE_FAILURE,
  //// Напряжение сети при отказе питания
  //// Danfoss 14-11 Не используется в проекте, только при конфигурировании

  VSD_RESET_MODE,
  //// Режим сброса
  //// Danfoss 14-20 Не используется в проекте, только при конфигурировании

  VSD_AUTOSTART_TIME,
  //// Время автоматического перезапуск
  //// Danfoss 14-21 Не используется в проекте, только при конфигурировании

  VSD_DELAY_CURRENT_LIMIT,
  //// Задержка перед выключение при пределе тока
  //// Danfoss 14-24 Не используется в проекте, только при конфигурировании

  VSD_DELAY_TORQUE_LIMIT,
  //// Задержка перед выключение при пределе моменте
  //// Danfoss 14-25 Не используется в проекте, только при конфигурировании

  VSD_FIL_TIME_CURRENT_LIMIT,
  //// Время фильтра регулятора тока
  //// Danfoss 14-32 Используется в проекте, CalcParam()

  VSD_DC_COMPENSATION,
  //// Компенсация напряжения постоянного тока
  //// Danfoss 14-51 Не используется в проекте, только при конфигурировании

  VSD_FAN_CONTROL,
  //// Управление вентилятором
  //// Danfoss 14-52 Не используется в проекте, только при конфигурировании

  VSD_OUT_FILTER,
  //// Выходной фильтр
  //// Danfoss 14-55 Используется в проекте, интерфейс

  VSD_FAIL_RESET,
  //// Сброс аварий
  //// Danfoss 14-90 Не используется в проекте, только при конфигурировании

  VSD_SOFT_VERSION,
  //// Версия ПО ЧРП
  //// Danfoss 15-43 Используется в проекте

  VSD_COMMAND_WORD,
  //// Командное слово
  //// Danfoss 16-00 Используется в проекте

  VSD_STATUS_WORD,
  //// Слово состояния
  //// Danfoss 16-03 Активно используется в проекте

  VSD_ACTIVE_POWER,
  //// Активная мощность
  //// Danfoss 16-10 Используется в проекте
  //// Novomet 0х0071 IREG_POUT

  VSD_OUT_CURRENT_MOTOR,
  //// Выходной ток средний
  //// Danfoss 16-14 Используется в проекте
  //// Novomet 0х0072 IREG_IOUT

  VSD_DC_VOLTAGE,
  //// Напряжение цепи пост.тока
  //// Danfoss 16-30 Используется в проекте
  //// Novomet 0х008B IREG_UD

  VSD_RADIATOR_TEMPERATURE,
  //// Температура радиатора
  //// Danfoss 16-34 Используется в проекте

  VSD_CONTROL_TEMPERATURE,
  //// Температура платы управления
  //// Danfoss 16-39 Используется в проекте

  VSD_DI_VSD,
  //// Цифровоый вход ПЧ
  //// Danfoss 16-60 Используется в проекте

  VSD_LAST_ALARM,
  //// Номер последней аварии и предупреждения
  //// Danfoss 16-87 Используется в проекте

  VSD_ALARM_WORD_1,
  //// Слово Аварии 1
  //// Danfoss 16-90 Используется в проекте

  VSD_ALARM_WORD_2,
  //// Слово Аварии 2
  //// Danfoss 16-91 Используется в проекте

  VSD_WARNING_WORD_1,
  //// Слово предупреждения 1
  //// Danfoss 16-92 Используется в проекте

  VSD_WARNING_WORD_2,
  //// Слово предупреждения 2
  //// Danfoss 16-93 Используется в проекте

  VSD_HIGH_START_TORQUE_TIME,
  //// Время применения высокого пускового момента
  //// Danfoss 30-20 Не используется в проекте, только при конфигурировании

  VSD_HIGH_START_TORQUE_CURRENT,
  //// Пусковой ток в % от номинального
  //// Danfoss 30-21 Не используется в проекте, только при конфигурировании

  VSD_LOCK_ROTOR_PROTECTION,
  //// Защита блокировки ротора
  //// Danfoss 30-22 Не используется в проекте, только при конфигурировании

  VSD_LOCK_ROTOR_TIME,
  //// Время задержки аварии блокировки ротора
  //// Danfoss 30-23 Не используется в проекте, только при конфигурировании

  VSD_THYR_ANGLE_REFERENCE,
  //// Угол управления тиристорами; служебный регистр для отладки работы выпрямителя
  //// Novomet 0x000A IREG_THYR_ANGLEREF

  VSD_THYR_STATUS,
  //// Статус платы выпрямителя. Регистр для внутреннего использования в ПЧ
  //// Novomet 0x000B IREG_THYR_STATUS

  VSD_THYR_CONTROL,
  //// Регистр управления тиристорами; используется только контроллером ПЧ
  //// Novomet 0x000С IREG_THYR_CONTROL

  VSD_THYR_VOLT_SHORT_CUILT,
  //// Напряжение защиты от КЗ инвертора.
  //// Novomet 0x000D IREG_THYR_USHORTCIRCUIT

  /// Статус индикатора
  /// Novomet 0x0014
  VSD_INDICATOR_STATUS,

  VSD_DOUTPUTS,
  //// Маска состояния цифровых выходов.
  //// Novomet 0х001E IREG_MEASURE_OUTPUTS

  VSD_DINPUTS,
  //// Маска состояния цифровых входов.
  //// Novomet 0х001F IREG_MEASURE_INPUTS

  VSD_DOUTPUTS_ENABLE,
  //// Маска включения цифровых выходов.
  //// Novomet 0х0020 IREG_MEASURE_OUTPUT_ENABLE

  VSD_DOUTPUTS_DISABLE,
  //// Маска выключения цифровых выходов
  //// Novomet 0х0021 IREG_MEASURE_OUTPUT_DISABLE

  VSD_VENT_ON_TEMPERATURE,
  //// Температура включения главного вентилятора.
  //// Novomet 0х0022 IREG_MEASURE_VENT1_ON

  VSD_VENT_OFF_TEMPERATURE,
  //// Температура выключения главного вентилятора.
  //// Novomet 0х0023 IREG_MEASURE_VENT1_OFF

  VSD_VENT_PERIOD,
  //// Период проверки температуры радиаторов
  //// Novomet 0х0024 IREG_MEASURE_VENT1_SCAN_PERIOD

  VSD_VENT_TEMPERATURE_FAULT,
  //// Температура срабатывания тепловой защиты радиаторов
  //// Novomet 0х0025 IREG_MEASURE_TFAULT

  VSD_INVERTOR_CONTROL,
  //// Флаговый регистр управления ПЧ
  //// Novomet 0х0032 IREG_INVERTOR_CONTROL

  VSD_INVERTOR_STATUS,
  //// Регистр состояния инвертора
  //// Novomet 0х0033 IREG_INVERTOR_STATUS

  VSD_FREQUENCY_ERROR,
  //// Уставка частоты "тревожного" режима
  //// Novomet 0х0036 IREG_FREQ_ERROR

  VSD_T_SPEEDUP,
  //// Период нарастания частоты.
  //// Novomet 0х0038 IREG_T_SPEEDUP

  VSD_T_SPEEDDOWN,
  //// Период Период снижения частоты
  //// Novomet 0х0039 IREG_T_SPEEDDOWN

  VSD_T_ILIMIT_SPEEDDOWN,
  //// Период снижения частоты при токоограничении.
  //// Novomet 0х003A IREG_T_ILIMIT_SPEEDDOWN

  VSD_T_ULOW_SPEEDDOWN,
  //// Период снижения частоты при нехватке напряжения на инверторе
  //// Novomet 0х003B IREG_T_ULOW_SPEEDDOWN

  VSD_CURRENT_REGULATOR,
  //// Тип текущего алгоритма управления (регулятора)
  //// Novomet 0х003D IREG_CURRENT_REGULATOR

  VSD_UF_U_FORCE,
  //// Напряжение форсировки
  //// Novomet 0х003E IREG_UF_U_FORCE

  VSD_UF_F_FORCE,
  //// Частота форсировки
  //// Novomet 0х003F IREG_UF_F_FORCE

  VSD_UF_UHH,
  //// Расчетное напряжение холостого хода.
  //// Novomet 0х0040 IREG_UF_UHH

  VSD_IFB_COMP,
  //// Постоянная времени компенсации индуктивности
  //// Novomet 0х0041 IREG_IFB_COMP

  VSD_VUOUT_PROP,
  //// Пропорциональный коэффициент ОС частотного контура регулирования
  //// Novomet 0х0042 IREG_VUOUT_PROP

  VSD_ILIMIT,
  //// Максимальный порог ограничения тока на выходе ПЧ, абс. вел
  //// Novomet 0х004E IREG_ILIMIT

  VSD_VUOUT_INTEG,
  //// Интегральный коэффициент ОС частотного контура регулирования
  //// Novomet 0х004F IREG_VUOUT_INTEG

  VSD_VTOUF_INTEG,
  //// Коэффициент постоянной времени обратного перехода с вектора на U/f
  //// Novomet 0х0050 IREG_VTOUF_INTEG

  VSD_UOUT_RATED,
  //// Текущее выходное напряжение ПЧ
  //// Novomet 0х0051 IREG_UOUT_RATED

  VSD_LOUT,
  //// Суммарная индуктивность на выходе ПЧ, фазное значение
  //// Novomet 0х0052 IREG_LOUT

  VSD_SW_STARTUP_FREQUENCY,
  //// Режим раскачки: частота
  //// Novomet 0х0053 IREG_STARTUP_FREQ

  VSD_SW_STARTUP_ANGLE_OSC,
  //// Режим раскачки: угол качания
  //// Novomet 0х0054 IREG_STARTUP_ANGLE_OSC

  VSD_SW_STARTUP_ANGLE_SHIFT,
  //// Режим раскачки: угол сдвига
  //// Novomet 0х0055 IREG_STARTUP_ANGLE_SHIFT

  VSD_SW_STARTUP_OSC_COUNT,
  //// Режим раскачки: количество качаний на угол
  //// Novomet 0х0056 IREG_STARTUP_OSC_COUNT

  VSD_SW_STARTUP_ROTATIONS,
  //// Режим раскачки: количество оборотов двигателя
  //// Novomet 0х0057    IREG_STARTUP_ROTATIONS

  VSD_SW_STARTUP_U_PULSE,
  //// Режим раскачки: кратность напряжения импульса расклинки
  //// Novomet 0х0058 IREG_STARTUP_U_PULSE

  VSD_SW_STARTUP_I_LIM,
  //// Режим раскачки: токоограничение
  //// Novomet 0х0059 IREG_STARTUP_I_LIM

  VSD_VFREQ_INTEG,
  //// Интегральный коэффициент ОС фазового контура регулирования
  //// Novomet 0х005A IREG_VFREQ_INTEG

  VSD_REGULATOR_QUEUE_1,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005B IREG_REGULATOR_QUEUE1

  VSD_REGULATOR_QUEUE_2,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005C IREG_REGULATOR_QUEUE2

  VSD_REGULATOR_QUEUE_3,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005D IREG_REGULATOR_QUEUE3

  VSD_REGULATOR_QUEUE_4,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005E IREG_REGULATOR_QUEUE4

  VSD_REGULATOR_QUEUE_5,
  //// Регистры рабочей очереди алгоритмов управления
  //// Novomet 0х005F IREG_REGULATOR_QUEUE5

  VSD_UD_LOW_FAULT,
  //// Порог защиты ПЧ по снижению напряжения на инверторе
  //// Novomet 0х0060 IREG_UD_LOW_FAULT

  VSD_UD_HIGH_FAULT,
  //// Порог защиты ПЧ по превыщению напряжения на инверторе
  //// Novomet 0х0061 IREG_UD_HIGH_FAULT

  VSD_UIN_ASYM_LEVEL,
  //// Порог защиты ПЧ по несимметрии входного напряжения на инверторе
  //// Novomet 0х0062 IREG_UIN_ASYM_LEVEL

  VSD_UIN_ASYM_DELAY,
  //// Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
  //// Novomet 0х0063 IREG_UIN_ASYM_DELAY

  VSD_F_VECT,
  //// Частота перехода на векторный алгоритм
  //// Novomet 0х0064 IREG_F_VECT

  VSD_VFREQ_GAIN,
  //// Пропорциональный коэффициент ОС фазового контура регулирования
  //// Novomet 0х0065 IREG_VFREQ_GAIN

  VSD_VANGLE_GAIN,
  //// Фазовый коэффициент усиления системы подавления резонанса
  //// Novomet 0х0066 IREG_VANGLE_GAIN

  VSD_RES_TIMECONST,
  //// Постоянная времени системы подавления резонанса
  //// Novomet 0х0067 IREG_RES_TIMECONST

  VSD_RES_IAMP_VALID,
  //// Служебный регистр для отладки алгоритмов управления
  //// Novomet 0х0068 IREG_RES_IAMP_VALID

  VSD_IB,
  //// Выходной ток ПЧ по фазе В. (амплитудное значение)
  //// Novomet 0х0069    IREG_IB

  VSD_PHB,
  //// Фаза выходного тока В
  //// Novomet 0х006A IREG_PHB

  VSD_IC,
  //// Выходной ток ПЧ по фазе В. (амплитудное значение)
  //// Novomet 0х006B    IREG_IC

  VSD_PHC,
  //// Фаза выходного тока В
  //// Novomet 0х006C IREG_PHC

  VSD_SW_STARTUP_I_LIM_PULSE,
  //// Порог токоограничения импульса расклинки в режиме с раскачкой
  //// Novomet 0х006D IREG_STARTUP_I_LIM_PULSE

  VSD_DECEL_SPEEDDOWN,
  //// Время снижения уставки в режиме остановки турбинного вращения
  //// Novomet 0х006E IREG_DECEL_SPEEDDOWN

  VSD_DECEL_VOLTAGE,
  //// Напряжение остаточного напряжения на шине (окончания разряда) в режиме остановки турбинного вращения (для ВД)
  //// Novomet 0х006F IREG_DECEL_VOLTAGE

  VSD_DECEL_F_MAX,
  //// Начальная частота режима остановки турбинного вращения (для АД)
  //// Novomet 0х0070 IREG_DECEL_F_MAX

  VSD_TIME_SECOND,
  //// Время секунды
  //// Novomet 0х0073 IREG_TIME_SECOND

  VSD_TIME_MINUTE,
  //// Время минуты
  //// Novomet 0х0074 IREG_TIME_MINUTE

  VSD_TIME_HOUR,
  //// Время часы
  //// Novomet 0х0075 IREG_TIME_HOUR

  VSD_TIME_DAY,
  //// Время дни
  //// Novomet 0х0076 IREG_TIME_DAY

  VSD_TIME_MONTH,
  //// Время месяцы
  //// Novomet 0х0077 IREG_TIME_MONTH

  VSD_TIME_YEAR,
  //// Время годы
  //// Novomet 0х0078 IREG_TIME_YEAR

  VSD_AST_IMEAS,
  //// Величина измерительного тока автонастройки
  //// Novomet 0х0079 IREG_AST_IMEAS

  VSD_AST_UMEAS,
  //// Величина измерительного напряжения автонастройки
  //// Novomet 0х007A IREG_AST_UMEAS

  VSD_AST_LOUT_1_0,
  //// Измеренная величина выходной индуктивности автонастройки
  //// Novomet 0х007B IREG_AST_LOUT_1_0

  VSD_AST_STANDING,
  //// Дополнительный регистр автонастройки, зарезервирован
  //// Novomet 0х007С IREG_AST_STANDING

  VSD_INVERTOR_EXT_STATUS,
  //// Расширенный регистр состояния инвертора
  //// Novomet 0х007D IREG_INVERTOR_EXT_STATUS

  VSD_JARRING_PERIOD,
  //// Период экскурсий частоты режима встряхивания.
  //// Novomet 0х007E IREG_JARRING_PERIOD

  VSD_JARRING_UPDFREQ,
  //// Величина прибавки частоты вращения выше уставки в режиме встряхивания
  //// Novomet 0х007F IREG_JARRING_UPDFREQ

  VSD_JARRING_UPTIME,
  //// Время прибавки частоты вращения выше уставки в режиме встряхивания
  //// Novomet 0х0080 IREG_JARRING_UPTIME

  VSD_JARRING_DOWNDFREQ,
  //// Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  //// Novomet 0х0081 IREG_JARRING_DOWNDFREQ

  VSD_JARRING_DOWNTIME,
  //// Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  //// Novomet 0х0082 IREG_JARRING_DOWNTIME

  VSD_JARRING_MODE,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0083 IREG_JARRING_MODE

  VSD_JARRING_TICK_CNT,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0084 IREG_JARRING_TICK_CNT

  VSD_JARRING_SECOND_CNT,
  //// Регистры обслуживания внутренних нужд режима встряхивания
  //// Novomet 0х0085 IREG_JARRING_SECOND_CNT

  VSD_START_FREQ,
  //// Стартовая частота вращения двигателя в основном режиме регулирования
  //// Novomet 0х0086 IREG_START_FREQ

  VSD_IA,
  //// Выходной ток ПЧ по фазе A. (амплитудное значение)
  //// Novomet 0х0087 IREG_IA

  VSD_PHA,
  //// Фаза выходного тока A
  //// Novomet 0х0088 IREG_PHA

  VSD_PHOUT,
  //// Усредненная фаза выходного тока ПЧ
  //// Novomet 0х0089 IREG_PHOUT

  // Первая точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_1,

  // Первая точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_1,

  // Вторая точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_2,

  // Вторая точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_2,

  // Третья точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_3,

  // Третья точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_3,

  // Четвертая точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_4,

  // Четвертая точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_4,

  // Пятая точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_5,

  // Пятая точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_5,

  // Шестая точка напряжения U/f характеристики
  VSD_UF_CHARACTERISTIC_U_6,

  // Шестая точка частоты U/f характеристики
  VSD_UF_CHARACTERISTIC_F_6,
  VSD_TEST0,                                  // Тестовый регистр 0
  VSD_TEST1,                                  // Тестовый регистр 1
  VSD_TEST2,                                  // Тестовый регистр 2
  VSD_TEST3,                                  // Тестовый регистр 3
  VSD_TEST4,                                  // Тестовый регистр 4
  VSD_TEST5,                                  // Тестовый регистр 5
  VSD_TEST6,                                  // Тестовый регистр 6
  VSD_TEST7,                                  // Тестовый регистр 7
  VSD_IOUT,                                   // Выходной ток ПЧ
  VSD_INV_FAULT,                              // Регистр ошибок инвертора
  VSD_SOUT,                                   // Текущая выходная полная мощность ПЧ
  VSD_M_IRMS,                                 // Уровень защиты i2t двигателя
  VSD_M_TRMS,                                 // Постоянная времени защиты i2t двигателя
  VSD_OVERPWM2_ANGLE,                         // Максимальный угол остановки вектора в режиме
  VSD_OVERPWM_GAIN,                           // Степень постоянной времени отработки
  VSD_DECEL_U_MAX,                            // Стартовое напряжение торможения авторотации
  VSD_FC_IRMS,                                // Ток тепловой защиты ПЧ длительный
  VSD_FC_ILIM_LONG,                           // Ограничение тока для импульсов
  VSD_FC_TLIM,                                // Время импульса с общим (максимальным) токоограничением
  VSD_U_SCALE,                                // Масштаб измеряемого напряжения ПЧ отн. базового 250А
  VSD_I_SCALE,                                // Масштаб измеряемого тока ПЧ отн. базового 250А
  VSD_M_I_FAST,                               // Уровень быстрой токовой защиты двигателя
  VSD_T_BLANK,                                // Время отключения защиты двигателя после старта
  VSD_PID_I_REF,                              // Уровень уставки тока для ПИД-регулятора
  VSD_PID_T_REG,                              // Постоянная времени для ПИД-регулятора
  VSD_PID_T_REG_1,                            // Постоянная времени для ПИД-регулятора
  VSD_INV_STATUS3,                            // Расширенные слово состояния 3
  VSD_END,

  TMS_BEGIN       = 15000,

  TMS_RESISTANCE_ISOLATION,
  //// Сопротивление изоляции
  //// Novomet BN-03

  TMS_PRESSURE_INTAKE,
  //// Давление на приёме

  TMS_TEMPERATURE_WINDING,
  //// Температура обмоток двигателя

  TMS_TEMPERATURE_INTAKE,
  //// Температура на приёме

  TMS_ACCELERATION_X_INTAKE,
  //// Ускорение по оси Х на приёме

  TMS_ACCELERATION_Y_INTAKE,
  //// Ускорение по оси Y на приёме

  TMS_ACCELERATION_XY_INTAKE,
  //// Ускорение по оси Y на приёме

  TMS_ACCELERATION_Z_INTAKE,
  //// Ускорение по оси Z на приёме

  TMS_ACCELERATIN_XYZ_INTAKE,
  //// Ускорение по оси по трём осям

  TMS_FLOW_DISCHARGE,
  //// Расход на выкиде

  TMS_PRESSURE_DISCHARGE,
  //// Давление на выкиде

  TMS_TEMPERATURE_DISCHARGE,
  //// Температура на выкиде

  TMS_MAX_PRESSURE_INTAKE,
  //// Максимальное давление на приёме

  TMS_MAX_TEMPERATUR_WINDING,
  //// Максимальная температура обмоток

  TMS_MAX_TEMPERATURE_INTAKE,
  //// Максимальная температура на приёме

  TMS_MAX_ACCELERATION_INTAKE,
  //// Максимальное ускорение

  TMS_FAIL_LINK_TMSP,
  //// Обрыв связи с ТМСП

  TMS_MANUFACTURE_TMSP,
  //// Код изготовителя ТМСП

  TMS_NUMBER_TMSP,
  //// Серийный номер ТМСП

  TMS_DATE_TMSP,
  //// Дата изготовления ТМСП

  TMS_CONSTRUCTION_TMSP,
  //// Код конструкции ТМСП

  TMS_PROTOCOL_TMSP,
  //// Код протокола ТМСП

  TMS_SENSOR_TMSP,
  //// Состав датчиков ТМСП

  TMS_SOFT_TMSP,
  //// Версия По ТМСП

  TMS_TIME_BIT,
  //// Скорости передачи бита

  TMS_MANUFACTURE_TMSN,
  //// Код завода изготовителя ТМСН

  TMS_NUMBER_TMSN,
  //// Заводской номер ТМСН

  TMS_DATE_TMSN,
  //// Дата изготовления ТМСН

  TMS_CONSTRUCTION_TMSN,
  //// Код конструкции ТМСН

  TMS_SOFT_TMSN,
  //// Версия ПО ТМСН

  TMS_PROTOCOL_TMSN,
  //// Код протокола ТМСН

  TMS_MIN_FLOW_DISCHARGE,
  //// Минимальный расход на выкиде

  TMS_MAX_FLOW_DISCHARGE,
  //// Максимальный расход на выкиде

  TMS_MAX_PRESSURE_DISHARGE,
  //// Максимальное давление на выкиде

  TMS_MAX_TEMPERATURE_DISCHARGE,
  //// Максимальная температура на выкиде

  TMS_RESISTANCE_ISOLATION_TIME,
  //// Время измерения сопротивления изоляции

  TMS_MEASURE_TIME,
  //// Время измерения данных и передачи

  TMS_RESISTANCE_TRANS_TIME,
  //// Время перехода в режим измерения сопротивления изоляции

  TMS_NUMBER_WELL,
  //// Номер скважины

  TMS_NUMBER_BUSH,
  //// Номер куста

  TMS_NUMBER_CDNG,
  //// Номер ЦДНГ

  TMS_MODBUS_ADDRESS,
  //// Адрес Modbus

  TMS_MODBUS_SPEED,
  //// Скорость Modbus

  TMS_MODBUS_STOP_BIT,
  //// Количество Стоп-бит

  TMS_DATA_TIME,
  //// Текущее дата время

  TMS_READ_STATIC_PARAM,
  //// Команда чтения статических параметров

  TMS_PACKAGE_ERROR_TMSP,
  //// Количество ошибок от ТМСП

  TMS_TEMPERATURE_MOTOR,
  //// Температура масла двигателя

  TMS_TEMPERATURE_UNIT,
  //// Единицы измерения температуры

  TMS_PRESSURE_UNIT,
  //// Единицы измерения давления

  TMS_PSW_TMS,
  //// Слово состояние системы

  TMS_PSW_TMSN,
  //// Слово состояние наземного блока

  TMS_PSW_TMSP_1,
  //// Cлово состояние погружного блока

  TMS_PSW_TMSP_2,
  //// Слово состояние погружного блока 2

  TMS_ANGLE_INTAKE,
  //// Зенитный угол

  TMS_END,
  //// Последний параметр ТМС, должен быть всегда последним параметром ТМС
  //// и быть без номера, всё новое добавлять до него

  EM_BEGIN         = 20000,

  //// Первый параметр группы счётчиков электроэнергии
  EM_FREQUENCY,
  //// Частота питающей сети

  EM_CURRENT_PHASE_1,
  //// Ток первой фазы

  EM_CURRENT_PHASE_2,
  //// Ток второй фазы

  EM_CURRENT_PHASE_3,
  //// Ток третей фазы

  EM_VOLTAGE_PHASE_1,
  //// Напряжение первой фазы

  EM_VOLTAGE_PHASE_2,
  //// Напряжение второй фазы

  EM_VOLTAGE_PHASE_3,
  //// Напряжение третей фазы

  EM_VOLTAGE_PHASE_1_2,
  //// Напряжение межфазное первой и второй фазы

  EM_VOLTAGE_PHASE_2_3,
  //// Напряжение межфазное второй и третьей фазы

  EM_VOLTAGE_PHASE_3_1,
  //// Напряжение межфазное третьей и первой фазы

  EM_COS_PHI,
  //// Общий коэффициент мощности

  EM_COS_PHI_PHASE_1,
  //// Коэффициент мощности первой фазы

  EM_COS_PHI_PHASE_2,
  //// Коэффициент мощности второй фазы

  EM_COS_PHI_PHASE_3,
  //// Коэффициент мощности третей фазы

  EM_FULL_POWER,
  //// Полная мощность

  EM_FULL_POWER_PHASE_1,
  //// Полная мощность первой фазы

  EM_FULL_POWER_PHASE_2,
  //// Полная мощность второй фазы

  EM_FULL_POWER_PHASE_3,
  //// Полная мощность третей фазы

  EM_ACTIVE_POWER,
  //// Общая активная мощность

  EM_ACTIVE_POWER_PHASE_1,
  //// Активная мощность первой фазы

  EM_ACTIVE_POWER_PHASE_2,
  //// Активная мощность второй фазы

  EM_ACTIVE_POWER_PHASE_3,
  //// Активная мощность третей фазы

  EM_REACTIVE_POWER,
  //// Общая реактивная мощность

  EM_REACTIVE_POWER_PHASE_1,
  //// Реактивная мощность первой фазы

  EM_REACTIVE_POWER_PHASE_2,
  //// Реактивная мощность второй фазы

  EM_REACTIVE_POWER_PHASE_3,
  //// Реактивная мощность третей фазы

  EM_ACTIVE_ENERGY,
  //// Общая активная энергия

  EM_ACTIVE_PLUS_ALL,
  //// Обшая активная плюс энергия за весь период

  EM_ACTIVE_MINUS_ALL,
  //// Обшая активная минус энергия за весь период

  EM_ACTIVE_PLUS_CUR_DAY,
  //// Активная плюс энергия за текущий день

  EM_ACTIVE_MINUS_CUR_DAY,
  //// Активная минус энергия за текущий день

  EM_ACTIVE_PLUS_PREV_DAY,
  //// Активная плюс энергия за предыдущий день

  EM_ACTIVE_MINUS_PREV_DAY,
  //// Активная минус энергия за текущий день

  EM_ACTIVE_PLUS_CUR_MONTH,
  //// Активная плюс энергия за текущий месяц

  EM_ACTIVE_MINUS_CUR_MONTH,
  //// Активная минус энергия за текущий месяц

  EM_ACTIVE_PLUS_PREV_MONTH,
  //// Активная плюс энергия на предыдущий месяц

  EM_ACTIVE_MINUS_PREV_MONTH,
  //// Активная минус энергия на предыдущий месяц

  EM_REACTIVE_ENERGY,
  //// Общая реактивня энергия

  EM_REACTIVE_PLUS_ALL,
  //// Общая плюс реактивная энергия за весь период

  EM_REACTIVE_MINUS_ALL,
  //// Общая минус реактивная энергия за весь период

  EM_REACTIVE_PLUS_CUR_DAY,
  //// Плюс реактивная энергия за весь день

  EM_REACTIVE_MINUS_CUR_DAY,
  //// Минус реактивная энергия за весь день

  EM_REACTIVE_PLUS_PREV_DAY,
  //// Плюс реактивная энергия за весь день

  EM_REACTIVE_MINUS_PREV_DAY,
  //// Минус реактивная энергия за весь день

  EM_REACTIVE_PLUS_CUR_MONTH,
  //// Плюс реактивная энергия за месяц

  EM_REACTIVE_MINUS_CUR_MONTH,
  //// Минус реактивная энергия за месяц

  EM_REACTIVE_PLUS_PREV_MONTH,
  //// Плюс реактивная энергия за предыдущий месяц

  EM_REACTIVE_MINUS_PREV_MONTH,
  //// Минус реактивная энергия за предыдущий месяц

  EM_COEFFICIENT_TRANS_CURRENT,
  //// Коэффицинт трансформации тока

  EM_COEFFICIENT_TRANS_VOLTAGE,
  //// Коэффициент трансформации напряжения

  EM_END
};

#endif // IDSLIST


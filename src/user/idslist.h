#ifndef IDSLIST
#define IDSLIST

enum enID
{
  CCS_BEGIN,                                                //!< Пустой регистр
  CCS_MOTOR_SPEED_NOW,                                      //!< Текущие обороты двигателя
  CCS_MOTOR_CURRENT_PHASE_1,                                //!< Ток двигателя фаза 1
  CCS_MOTOR_CURRENT_PHASE_2,                                //!< Ток двигателя фаза 2
  CCS_MOTOR_CURRENT_PHASE_3,                                //!< Ток двигателя фаза 3
  CCS_MOTOR_CURRENT_IMBALANCE,                              //!< Дисбаланс токов двигателя
  CCS_MOTOR_CURRENT_AVARAGE,                                //!< Ток двигателя средний
  CCS_MOTOR_LOAD_NOW,                                       //!< Текущая загрузка двигателя
  CCS_MOTOR_VOLTAGE_NOW,                                    //!< Текущее напряжение двигателя
  CCS_MOTOR_COS_PHI_NOW,                                    //!< Текущий Косинус Фи
  CCS_TURBO_ROTATION_NOW,                                   //!< Текущее турбинное вращение
  CCS_RESISTANCE_ISOLATION,                                 //!< Сопротивление изоляции в системе
  CCS_PHASE_ROTATION,                                       //!< Порядок чередования фаз
  CCS_CONDITION,                                            //!< 
  CCS_RUNNING_TYPE,                                         //!< Режим пуска двигателя
  CCS_WORKING_MODE,                                         //!< Режим работы
  CCS_VOLTAGE_IMBALANCE_IN,                                 //!< Дисбаланс входного напряжения
  CCS_CURRENT_IMBALANCE_IN,                                 //!< Дисбаланс входного тока
  CCS_AI_1_VALUE,                                           //!< 
  CCS_AI_2_VALUE,                                           //!< 
  CCS_AI_3_VALUE,                                           //!< 
  CCS_AI_4_VALUE,                                           //!< 
  CCS_DOOR_VALUE,                                           //!< 
  CCS_MONOMETR_CONTACT_VALUE,                               //!< 
  CCS_DI_1_VALUE,                                           //!< 
  CCS_DI_2_VALUE,                                           //!< 
  CCS_DI_3_VALUE,                                           //!< 
  CCS_DI_4_VALUE,                                           //!< 
  CCS_LOG_PERIOD_NORMAL,                                    //!< Период записи
  CCS_LOG_PERIOD_FAST,                                      //!< Период экстренной записи
  CCS_LOG_PERIOD_DHS,                                       //!< Период записи ТМС
  CCS_MOTOR_CURRENT_HH,                                     //!< Ток холостого хода
  CCS_TRANS_NOMINAL_POWER,                                  //!< Мощность ТМПН
  CCS_TRANS_NOMINAL_CURRENT,                                //!< Номинальный ток ТМПН
  CCS_TRANS_NOMINAL_VOLTAGE,                                //!< Номинальное напряжение ТМПН
  CCS_TRANS_CABLE_LENGHT,                                   //!< Длина кабеля
  CCS_TRANS_CABLE_CROSS,                                    //!< Сечение кабеля
  CCS_TRANS_NEED_VOLTAGE_TAP_OFF,                           //!< Необходимое напряжение отпайки
  CCS_TRANS_VOLTAGE_TAP_OFF,                                //!< Напряжение отпайки
  CCS_DHS_TYPE,                                             //!< Тип ТМС
  CCS_PUMP_CAPACITY,                                        //!< Производительность насоса
  CCS_PUMP_HEAD,                                            //!< Напор ЭЦН
  CCS_DEPTH,                                                //!< Глубина спуска
  CCS_PROT_SUPPLY_OVERVOLTAGE_MODE,                         //!< Режим работа защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_PREVENT,                      //!< Параметр запрещающий запуск
  CCS_PROT_SUPPLY_OVERVOLTAGE_ACTIV_DELAY,                  //!< Задержка активации уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_DELAY,                   //!< Задержка срабатывания уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_DELAY,                //!< Задержка АПВ уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_LIMIT,                //!< Количество АПВ уставка
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_RESET,                //!< Автосброс счетчика АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_TRIP_SETPOINT,                //!< Уставка срабатывания защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_SETPOINT,             //!< 
  CCS_PROT_SUPPLY_OVERVOLTAGE_PARAMETER,                    //!< Уставка параметр защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FLAG,                 //!< Уставка параметр защиты
  CCS_PROT_SUPPLY_OVERVOLTAGE_STATE,                        //!< Текущий параметр состояние автомата
  CCS_PROT_SUPPLY_OVERVOLTAGE_TIME,                         //!< Текущий параметр Фиксированное время
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_COUNT,                //!< Текущий параметр Количество АПВ
  CCS_PROT_SUPPLY_OVERVOLTAGE_RESTART_FIRST_TIME,           //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_MODE,                        //!< Защита "Превышение питания сети"
  CCS_PROT_SUPPLY_UNDERVOLTAGE_PREVENT,                     //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_ACTIV_DELAY,                 //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_DELAY,                  //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_DELAY,               //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_LIMIT,               //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_RESET,               //!< Автосброс счетчика АПВ
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TRIP_SETPOINT,               //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_SETPOINT,            //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_PARAMETER,                   //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FLAG,                //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_STATE,                       //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_TIME,                        //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_COUNT,               //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_RESTART_FIRST_TIME,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_MODE,                   //!< Защита "Дисбаланс напряжения"
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PREVENT,                //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_ACTIV_DELAY,            //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_DELAY,             //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_DELAY,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_LIMIT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_RESET,          //!< Автосброс счетчика АПВ
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TRIP_SETPOINT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_SETPOINT,       //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_PARAMETER,              //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FLAG,           //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_STATE,                  //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_TIME,                   //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_COUNT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_RESTART_FIRST_TIME,     //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_MODE,                   //!< Защиты "Дисбаланс входных токов"
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PREVENT,                //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_ACTIV_DELAY,            //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_DELAY,             //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_DELAY,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_LIMIT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_RESET,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TRIP_SETPOINT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_SETPOINT,       //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_PARAMETER,              //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_FLAG,           //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_STATE,                  //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_TIME,                   //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_COUNT,          //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_RESTART_FIRST_TIME,     //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_MODE,                      //!< Защиты "Чередование фаз"
  CCS_PROT_SUPPLY_PHASE_ROTATION_PREVENT,                   //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_ACTIV_DELAY,               //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_TRIP_DELAY,                //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_DELAY,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_LIMIT,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_RESET,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_TRIP_SETPOINT,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_SETPOINT,          //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_PARAMETER,                 //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_FLAG,              //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_STATE,                     //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_TIME,                      //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_COUNT,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_RESTART_FIRST_TIME,        //!< 
  CCS_PROT_MOTOR_OVERLOAD_MODE,                             //!< Защита "Перегруз" (ЗП)
  CCS_PROT_MOTOR_OVERLOAD_PREVENT,                          //!< 
  CCS_PROT_MOTOR_OVERLOAD_ACTIV_DELAY,                      //!< 
  CCS_PROT_MOTOR_OVERLOAD_TRIP_DELAY,                       //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_DELAY,                    //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_LIMIT,                    //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_RESET,                    //!< 
  CCS_PROT_MOTOR_OVERLOAD_TRIP_SETPOINT,                    //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_SETPOINT,                 //!< 
  CCS_PROT_MOTOR_OVERLOAD_PARAMETER,                        //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_FLAG,                     //!< 
  CCS_PROT_MOTOR_OVERLOAD_STATE,                            //!< 
  CCS_PROT_MOTOR_OVERLOAD_TIME,                             //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_COUNT,                    //!< 
  CCS_PROT_MOTOR_OVERLOAD_RESTART_FIRST_TIME,               //!< 
  CCS_PROT_MOTOR_UNDERLOAD_MODE,                            //!< Защита "Недогруз" (ЗП)
  CCS_PROT_MOTOR_UNDERLOAD_PREVENT,                         //!< 
  CCS_PROT_MOTOR_UNDERLOAD_ACTIV_DELAY,                     //!< 
  CCS_PROT_MOTOR_UNDERLOAD_TRIP_DELAY,                      //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_DELAY,                   //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_LIMIT,                   //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_RESET,                   //!< 
  CCS_PROT_MOTOR_UNDERLOAD_TRIP_SETPOINT,                   //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_SETPOINT,                //!< 
  CCS_PROT_MOTOR_UNDERLOAD_PARAMETER,                       //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_FLAG,                    //!< 
  CCS_PROT_MOTOR_UNDERLOAD_STATE,                           //!< 
  CCS_PROT_MOTOR_UNDERLOAD_TIME,                            //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_COUNT,                   //!< 
  CCS_PROT_MOTOR_UNDERLOAD_RESTART_FIRST_TIME,              //!< 
  CCS_PROT_MOTOR_CURRENT_MODE,                              //!< 
  CCS_PROT_MOTOR_CURRENT_PREVENT,                           //!< Защита ЧРП "Предел тока двигателя"
  CCS_PROT_MOTOR_CURRENT_ACTIV_DELAY,                       //!< 
  CCS_PROT_MOTOR_CURRENT_TRIP_DELAY,                        //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_DELAY,                     //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_LIMIT,                     //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_RESET,                     //!< 
  CCS_PROT_MOTOR_CURRENT_TRIP_SETPOINT,                     //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_SETPOINT,                  //!< 
  CCS_PROT_MOTOR_CURRENT_PARAMETER,                         //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_FLAG,                      //!< 
  CCS_PROT_MOTOR_CURRENT_STATE,                             //!< 
  CCS_PROT_MOTOR_CURRENT_TIME,                              //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_COUNT,                     //!< 
  CCS_PROT_MOTOR_CURRENT_RESTART_FIRST_TIME,                //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_MODE,                    //!< Защита "Дисбаланс токов двигателя"
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_PREVENT,                 //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_ACTIV_DELAY,             //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_DELAY,              //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_DELAY,           //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_LIMIT,           //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_RESET,           //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TRIP_SETPOINT,           //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_SETPOINT,        //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_PARAMETER,               //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FLAG,            //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_STATE,                   //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_TIME,                    //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_COUNT,           //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_RESTART_FIRST_TIME,      //!< 
  CCS_PROT_MOTOR_ASYNC_MODE,                                //!< Защита "Турбинное вращение"
  CCS_PROT_MOTOR_ASYNC_PREVENT,                             //!< 
  CCS_PROT_MOTOR_ASYNC_ACTIV_DELAY,                         //!< 
  CCS_PROT_MOTOR_ASYNC_TRIP_DELAY,                          //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_DELAY,                       //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_LIMIT,                       //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_RESET,                       //!< 
  CCS_PROT_MOTOR_ASYNC_TRIP_SETPOINT,                       //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_SETPOINT,                    //!< 
  CCS_PROT_MOTOR_ASYNC_PARAMETER,                           //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_FLAG,                        //!< 
  CCS_PROT_MOTOR_ASYNC_STATE,                               //!< 
  CCS_PROT_MOTOR_ASYNC_TIME,                                //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_COUNT,                       //!< 
  CCS_PROT_MOTOR_ASYNC_RESTART_FIRST_TIME,                  //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_MODE,                          //!< Защита "Рассинхронизация ПВЭД"
  CCS_PROT_MOTOR_OUT_OF_SYNC_PREVENT,                       //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY,                   //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY,                    //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY,                 //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT,                 //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET,                 //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT,                 //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT,              //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER,                     //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FLAG,                  //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_STATE,                         //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_TIME,                          //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT,                 //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_FIRST_TIME,            //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_MODE,                        //!< Защита "Давление на приёме насоса"
  CCS_PROT_DHS_PRESSURE_INTAKE_PREVENT,                     //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_ACTIV_DELAY,                 //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_DELAY,                  //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_DELAY,               //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_LIMIT,               //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_RESET,               //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_TRIP_SETPOINT,               //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_SETPOINT,            //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_PARAMETER,                   //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FLAG,                //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_STATE,                       //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_TIME,                        //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_COUNT,               //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_RESTART_FIRST_TIME,          //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_MODE,                      //!< Защита "Температура двигателя"
  CCS_PROT_DHS_TEMPERATURE_MOTOR_PREVENT,                   //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_ACTIV_DELAY,               //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_DELAY,                //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_DELAY,             //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_LIMIT,             //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_RESET,             //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TRIP_SETPOINT,             //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_SETPOINT,          //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_PARAMETER,                 //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FLAG,              //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_STATE,                     //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_TIME,                      //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_COUNT,             //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_RESTART_FIRST_TIME,        //!< 
  CCS_PROT_DHS_RESISTANCE_MODE,                             //!< Защита "Сопротивление изоляции"
  CCS_PROT_DHS_RESISTANCE_PREVENT,                          //!< 
  CCS_PROT_DHS_RESISTANCE_ACTIV_DELAY,                      //!< 
  CCS_PROT_DHS_RESISTANCE_TRIP_DELAY,                       //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_DELAY,                    //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_LIMIT,                    //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_RESET,                    //!< 
  CCS_PROT_DHS_RESISTANCE_TRIP_SETPOINT,                    //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_SETPOINT,                 //!< 
  CCS_PROT_DHS_RESISTANCE_PARAMETER,                        //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_FLAG,                     //!< 
  CCS_PROT_DHS_RESISTANCE_STATE,                            //!< 
  CCS_PROT_DHS_RESISTANCE_TIME,                             //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_COUNT,                    //!< 
  CCS_PROT_DHS_RESISTANCE_RESTART_FIRST_TIME,               //!< 
  CCS_PROT_DHS_VIBRATION_MODE,                              //!< Защита "Вибрация двигателя"
  CCS_PROT_DHS_VIBRATION_PREVENT,                           //!< 
  CCS_PROT_DHS_VIBRATION_ACTIV_DELAY,                       //!< 
  CCS_PROT_DHS_VIBRATION_TRIP_DELAY,                        //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_DELAY,                     //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_LIMIT,                     //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_RESET,                     //!< 
  CCS_PROT_DHS_VIBRATION_TRIP_SETPOINT,                     //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_SETPOINT,                  //!< 
  CCS_PROT_DHS_VIBRATION_PARAMETER,                         //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_FLAG,                      //!< 
  CCS_PROT_DHS_VIBRATION_STATE,                             //!< 
  CCS_PROT_DHS_VIBRATION_TIME,                              //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_COUNT,                     //!< 
  CCS_PROT_DHS_VIBRATION_RESTART_FIRST_TIME,                //!< 
  CCS_PROT_AI_1_MODE,                                       //!< Защиты Аналоговый вход 1
  CCS_PROT_AI_1_PREVENT,                                    //!< 
  CCS_PROT_AI_1_ACTIV_DELAY,                                //!< 
  CCS_PROT_AI_1_TRIP_DELAY,                                 //!< 
  CCS_PROT_AI_1_RESTART_DELAY,                              //!< 
  CCS_PROT_AI_1_RESTART_LIMIT,                              //!< 
  CCS_PROT_AI_1_RESTART_RESET,                              //!< 
  CCS_PROT_AI_1_TRIP_SETPOINT,                              //!< 
  CCS_PROT_AI_1_RESTART_SETPOINT,                           //!< 
  CCS_PROT_AI_1_PARAMETER,                                  //!< 
  CCS_PROT_AI_1_RESTART_FLAG,                               //!< 
  CCS_PROT_AI_1_STATE,                                      //!< 
  CCS_PROT_AI_1_TIME,                                       //!< 
  CCS_PROT_AI_1_RESTART_COUNT,                              //!< 
  CCS_PROT_AI_1_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_AI_2_MODE,                                       //!< Защиты Аналоговый вход 2
  CCS_PROT_AI_2_PREVENT,                                    //!< 
  CCS_PROT_AI_2_ACTIV_DELAY,                                //!< 
  CCS_PROT_AI_2_TRIP_DELAY,                                 //!< 
  CCS_PROT_AI_2_RESTART_DELAY,                              //!< 
  CCS_PROT_AI_2_RESTART_LIMIT,                              //!< 
  CCS_PROT_AI_2_RESTART_RESET,                              //!< 
  CCS_PROT_AI_2_TRIP_SETPOINT,                              //!< 
  CCS_PROT_AI_2_RESTART_SETPOINT,                           //!< 
  CCS_PROT_AI_2_PARAMETER,                                  //!< 
  CCS_PROT_AI_2_RESTART_FLAG,                               //!< 
  CCS_PROT_AI_2_STATE,                                      //!< 
  CCS_PROT_AI_2_TIME,                                       //!< 
  CCS_PROT_AI_2_RESTART_COUNT,                              //!< 
  CCS_PROT_AI_2_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_AI_3_MODE,                                       //!< Защиты Аналоговый вход 3
  CCS_PROT_AI_3_PREVENT,                                    //!< 
  CCS_PROT_AI_3_ACTIV_DELAY,                                //!< 
  CCS_PROT_AI_3_TRIP_DELAY,                                 //!< 
  CCS_PROT_AI_3_RESTART_DELAY,                              //!< 
  CCS_PROT_AI_3_RESTART_LIMIT,                              //!< 
  CCS_PROT_AI_3_RESTART_RESET,                              //!< 
  CCS_PROT_AI_3_TRIP_SETPOINT,                              //!< 
  CCS_PROT_AI_3_RESTART_SETPOINT,                           //!< 
  CCS_PROT_AI_3_PARAMETER,                                  //!< 
  CCS_PROT_AI_3_RESTART_FLAG,                               //!< 
  CCS_PROT_AI_3_STATE,                                      //!< 
  CCS_PROT_AI_3_TIME,                                       //!< 
  CCS_PROT_AI_3_RESTART_COUNT,                              //!< 
  CCS_PROT_AI_3_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_AI_4_MODE,                                       //!< Защиты Аналоговый вход 4
  CCS_PROT_AI_4_PREVENT,                                    //!< 
  CCS_PROT_AI_4_ACTIV_DELAY,                                //!< 
  CCS_PROT_AI_4_TRIP_DELAY,                                 //!< 
  CCS_PROT_AI_4_RESTART_DELAY,                              //!< 
  CCS_PROT_AI_4_RESTART_LIMIT,                              //!< 
  CCS_PROT_AI_4_RESTART_RESET,                              //!< 
  CCS_PROT_AI_4_TRIP_SETPOINT,                              //!< 
  CCS_PROT_AI_4_RESTART_SETPOINT,                           //!< 
  CCS_PROT_AI_4_PARAMETER,                                  //!< 
  CCS_PROT_AI_4_RESTART_FLAG,                               //!< 
  CCS_PROT_AI_4_STATE,                                      //!< 
  CCS_PROT_AI_4_TIME,                                       //!< 
  CCS_PROT_AI_4_RESTART_COUNT,                              //!< 
  CCS_PROT_AI_4_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_DI_1_MODE,                                       //!< Защиты по дискретным входам
  CCS_PROT_DI_1_PREVENT,                                    //!< 
  CCS_PROT_DI_1_ACTIV_DELAY,                                //!< 
  CCS_PROT_DI_1_TRIP_DELAY,                                 //!< 
  CCS_PROT_DI_1_RESTART_DELAY,                              //!< 
  CCS_PROT_DI_1_RESTART_LIMIT,                              //!< 
  CCS_PROT_DI_1_RESTART_RESET,                              //!< 
  CCS_PROT_DI_1_TRIP_SETPOINT,                              //!< 
  CCS_PROT_DI_1_RESTART_SETPOINT,                           //!< 
  CCS_PROT_DI_1_PARAMETER,                                  //!< 
  CCS_PROT_DI_1_RESTART_FLAG,                               //!< 
  CCS_PROT_DI_1_STATE,                                      //!< 
  CCS_PROT_DI_1_TIME,                                       //!< 
  CCS_PROT_DI_1_RESTART_COUNT,                              //!< 
  CCS_PROT_DI_1_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_DI_2_MODE,                                       //!< Защита дискретный вход 2
  CCS_PROT_DI_2_PREVENT,                                    //!< 
  CCS_PROT_DI_2_ACTIV_DELAY,                                //!< 
  CCS_PROT_DI_2_TRIP_DELAY,                                 //!< 
  CCS_PROT_DI_2_RESTART_DELAY,                              //!< 
  CCS_PROT_DI_2_RESTART_LIMIT,                              //!< 
  CCS_PROT_DI_2_RESTART_RESET,                              //!< 
  CCS_PROT_DI_2_TRIP_SETPOINT,                              //!< 
  CCS_PROT_DI_2_RESTART_SETPOINT,                           //!< 
  CCS_PROT_DI_2_PARAMETER,                                  //!< 
  CCS_PROT_DI_2_RESTART_FLAG,                               //!< 
  CCS_PROT_DI_2_STATE,                                      //!< 
  CCS_PROT_DI_2_TIME,                                       //!< 
  CCS_PROT_DI_2_RESTART_COUNT,                              //!< 
  CCS_PROT_DI_2_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_DI_3_MODE,                                       //!< Защита дискретный вход 3
  CCS_PROT_DI_3_PREVENT,                                    //!< 
  CCS_PROT_DI_3_ACTIV_DELAY,                                //!< 
  CCS_PROT_DI_3_TRIP_DELAY,                                 //!< 
  CCS_PROT_DI_3_RESTART_DELAY,                              //!< 
  CCS_PROT_DI_3_RESTART_LIMIT,                              //!< 
  CCS_PROT_DI_3_RESTART_RESET,                              //!< 
  CCS_PROT_DI_3_TRIP_SETPOINT,                              //!< 
  CCS_PROT_DI_3_RESTART_SETPOINT,                           //!< 
  CCS_PROT_DI_3_PARAMETER,                                  //!< 
  CCS_PROT_DI_3_RESTART_FLAG,                               //!< 
  CCS_PROT_DI_3_STATE,                                      //!< 
  CCS_PROT_DI_3_TIME,                                       //!< 
  CCS_PROT_DI_3_RESTART_COUNT,                              //!< 
  CCS_PROT_DI_3_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_DI_4_MODE,                                       //!< Защита дискретный вход 4
  CCS_PROT_DI_4_PREVENT,                                    //!< 
  CCS_PROT_DI_4_ACTIV_DELAY,                                //!< 
  CCS_PROT_DI_4_TRIP_DELAY,                                 //!< 
  CCS_PROT_DI_4_RESTART_DELAY,                              //!< 
  CCS_PROT_DI_4_RESTART_LIMIT,                              //!< 
  CCS_PROT_DI_4_RESTART_RESET,                              //!< 
  CCS_PROT_DI_4_TRIP_SETPOINT,                              //!< 
  CCS_PROT_DI_4_RESTART_SETPOINT,                           //!< 
  CCS_PROT_DI_4_PARAMETER,                                  //!< 
  CCS_PROT_DI_4_RESTART_FLAG,                               //!< 
  CCS_PROT_DI_4_STATE,                                      //!< 
  CCS_PROT_DI_4_TIME,                                       //!< 
  CCS_PROT_DI_4_RESTART_COUNT,                              //!< 
  CCS_PROT_DI_4_RESTART_FIRST_TIME,                         //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_MODE,                        //!< Защита "Превышения числа пусков"
  CCS_PROT_OTHER_LIMIT_RESTART_PREVENT,                     //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_ACTIV_DELAY,                 //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_TRIP_DELAY,                  //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_DELAY,               //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_LIMIT,               //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_RESET,               //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_TRIP_SETPOINT,               //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_SETPOINT,            //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_PARAMETER,                   //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_FLAG,                //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_STATE,                       //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_TIME,                        //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_RESTART_COUNT,               //!< 
  CCS_PROT_OTHER_LOCK_DOOR_MODE,                            //!< Защита "Блокировка силового отсека"
  CCS_PROT_OTHER_LOCK_DOOR_PREVENT,                         //!< 
  CCS_PROT_OTHER_LOCK_DOOR_ACTIV_DELAY,                     //!< 
  CCS_PROT_OTHER_LOCK_DOOR_TRIP_DELAY,                      //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_DELAY,                   //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_LIMIT,                   //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_RESET,                   //!< 
  CCS_PROT_OTHER_LOCK_DOOR_TRIP_SETPOINT,                   //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_SETPOINT,                //!< 
  CCS_PROT_OTHER_LOCK_DOOR_PARAMETER,                       //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_FLAG,                    //!< 
  CCS_PROT_OTHER_LOCK_DOOR_STATE,                           //!< 
  CCS_PROT_OTHER_LOCK_DOOR_TIME,                            //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_COUNT,                   //!< 
  CCS_PROT_OTHER_LOCK_DOOR_RESTART_FIRST_TIME,              //!< 
  CCS_PROT_OTHER_VSD_MODE,                                  //!< Защита "Аппаратные защиты ЧРП"
  CCS_PROT_OTHER_VSD_PREVENT,                               //!< 
  CCS_PROT_OTHER_VSD_ACTIV_DELAY,                           //!< 
  CCS_PROT_OTHER_VSD_TRIP_DELAY,                            //!< 
  CCS_PROT_OTHER_VSD_RESTART_DELAY,                         //!< 
  CCS_PROT_OTHER_VSD_RESTART_LIMIT,                         //!< 
  CCS_PROT_OTHER_VSD_RESTART_RESET,                         //!< 
  CCS_PROT_OTHER_VSD_TRIP_SETPOINT,                         //!< 
  CCS_PROT_OTHER_VSD_RESTART_SETPOINT,                      //!< 
  CCS_PROT_OTHER_VSD_PARAMETER,                             //!< 
  CCS_PROT_OTHER_VSD_RESTART_FLAG,                          //!< 
  CCS_PROT_OTHER_VSD_STATE,                                 //!< 
  CCS_PROT_OTHER_VSD_TIME,                                  //!< 
  CCS_PROT_OTHER_VSD_RESTART_COUNT,                         //!< 
  CCS_PROT_OTHER_VSD_RESTART_FIRST_TIME,                    //!< 
  CCS_PROT_OTHER_IMB_MODE,                                  //!< Защита "Контроль УКИ"
  CCS_PROT_OTHER_IMB_PREVENT,                               //!< 
  CCS_PROT_OTHER_IMB_ACTIV_DELAY,                           //!< 
  CCS_PROT_OTHER_IMB_TRIP_DELAY,                            //!< 
  CCS_PROT_OTHER_IMB_RESTART_DELAY,                         //!< 
  CCS_PROT_OTHER_IMB_RESTART_LIMIT,                         //!< 
  CCS_PROT_OTHER_IMB_RESTART_RESET,                         //!< 
  CCS_PROT_OTHER_IMB_TRIP_SETPOINT,                         //!< 
  CCS_PROT_OTHER_IMB_RESTART_SETPOINT,                      //!< 
  CCS_PROT_OTHER_IMB_PARAMETER,                             //!< 
  CCS_PROT_OTHER_IMB_RESTART_FLAG,                          //!< 
  CCS_PROT_OTHER_IMB_STATE,                                 //!< 
  CCS_PROT_OTHER_IMB_TIME,                                  //!< 
  CCS_PROT_OTHER_IMB_RESTART_COUNT,                         //!< 
  CCS_PROT_OTHER_IMB_RESTART_FIRST_TIME,                    //!< 
  CCS_RGM_PERIODIC_MODE,                                    //!< Режим периодический
  CCS_RGM_PERIODIC_STATE,                                   //!< 
  CCS_RGM_PERIODIC_RUN_PERIOD,                              //!< 
  CCS_RGM_PERIODIC_STOP_PERIOD,                             //!< 
  CCS_RGM_PERIODIC_RUN_BEGIN_TIME,                          //!< 
  CCS_RGM_PERIODIC_RUN_TIME_TO_END,                         //!< 
  CCS_RGM_PERIODIC_STOP_BEGIN_TIME,                         //!< 
  CCS_RGM_PERIODIC_STOP_TIME_TO_END,                        //!< 
  CCS_RGM_ALTERNATION_FREQ_MODE,                            //!< Режим чередования частот
  CCS_RGM_ALTERNATION_FREQ_FREQ_1,                          //!< Режим чередования частот частота 1
  CCS_RGM_ALTERNATION_FREQ_TIMER_1,                         //!< Режим чередования частот время работы на частоте 1
  CCS_RGM_ALTERNATION_FREQ_TIMER_1_END,                     //!< 
  CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_1,                     //!< Режим чередования частот уставка ЗСП
  CCS_RGM_ALTERNATION_FREQ_FREQ_2,                          //!< Режим чередования частот частота 2
  CCS_RGM_ALTERNATION_FREQ_TIMER_2,                         //!< Режим чередования частот время работы на частоте 2
  CCS_RGM_ALTERNATION_FREQ_TIMER_2_END,                     //!< 
  CCS_RGM_ALTERNATION_FREQ_UNDERLOAD_2,                     //!< Режим чередования частот уставка ЗСП 2
  CCS_RGM_CHANGE_FREQ_MODE,                                 //!< Режим программного изменения частоты
  CCS_RGM_CHANGE_FREQ_BEGIN_FREQ,                           //!< 
  CCS_RGM_CHANGE_FREQ_END_FREQ,                             //!< 
  CCS_RGM_CHANGE_FREQ_TIMER_FREQ,                           //!< 
  CCS_RGM_CHANGE_FREQ_RUN_TIMER_FREQ,                       //!< 
  CCS_RGM_CHANGE_FREQ_END_TIMER_FREQ,                       //!< 
  CCS_RGM_MODE_DATE_TIME,                                   //!< Параметры ВНР
  CCS_RGM_MODE_CURRENT,                                     //!< 
  CCS_RGM_MODE_VOLTAGE,                                     //!< 
  CCS_RGM_MODE_LOAD,                                        //!< 
  CCS_RGM_MAINTENANCE_PARAM_MODE,                           //!< Режим поддержания параметра включение
  CCS_RGM_MAINTENANCE_PARAM,                                //!< Режим поддержания параметра параметра
  CCS_RGM_MAINTENANCE_PARAM_DEPENDENCE,                     //!< Режим поддержания параметра зависимость
  CCS_RGM_MAINTENANCE_PARAM_SETPOINT,                       //!< Режим поддержания параметра поддержка значения
  CCS_RGM_MAINTENANCE_PARAM_PERIOD,                         //!< Режим поддержания параметра период регулирования
  CCS_RGM_MAINTENANCE_PARAM_PROP,                           //!< Режим поддержания параметра пропорциональный коэффициент
  CCS_RGM_MAINTENANCE_PARAM_INT,                            //!< Режим поддержания параметра интегральный коэффициент
  CCS_RGM_MAINTENANCE_PARAM_MIN_FREQ,                       //!< Режим поддержания параметра мин рабочая частота
  CCS_RGM_MAINTENANCE_PARAM_MAX_FREQ,                       //!< Режим поддержания параметра макс рабочая частота
  CCS_RGM_JARRING_MODE,                                     //!< Режим встряхивания
  CCS_RGM_JARRING_PERIOD,                                   //!< Период экскурсий частоты режима встряхивания.
  CCS_RGM_JARRING_UPDFREQ,                                  //!< Величина прибавки частоты вращения выше уставки в режиме встряхивания
  CCS_RGM_JARRING_UPTIME,                                   //!< Время прибавки частоты вращения выше уставки в режиме встряхивания
  CCS_RGM_JARRING_DOWNDFREQ,                                //!< Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  CCS_RGM_JARRING_DOWNTIME,                                 //!< Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  CCS_RGM_HEAT_CABLE_MODE,                                  //!< Режим Учёт нагрева кабеля
  CCS_RGM_HEAT_CABLE_RESISTANCE_20,                         //!< Режим Учёт нагрева кабеля сопротивление при 20с
  CCS_RGM_HEAT_CABLE_RESISTANCE_80,                         //!< Режим Учёт нагрева кабеля сопротивление при 80с
  CCS_RGM_HEAT_CABLE_MODE_CURRENT,                          //!< Режим Учёт нагрева кабеля номинальный ток кабеля
  CCS_RGM_HEAT_CABLE_MODE_DELAY_REACTION,                   //!< Режим Учёт нагрева кабеля время нагрева
  CCS_RGM_OPTIM_VOLTAGE_MODE,                               //!< Режим оптимизации напряжения вкл
  CCS_RGM_OPTIM_VOLTAGE_SCALE,                              //!< Режим оптимизации напряжения диапазон
  CCS_RGM_OPTIM_VOLTAGE_STEP,                               //!< Режим оптимизации напряжения шаг
  CCS_RGM_OPTIM_VOLTAGE_DELAY_REACTION,                     //!< Режим оптимизации напряжения первый цикл после запуска
  CCS_RGM_OPTIM_VOLTAGE_DELAY_RESTART,                      //!< Режим оптимизации напряжения периодичность оптимизации
  CCS_RGM_PUMP_GAS_MODE,                                    //!< Режим прокачки газа
  CCS_RGM_PUMP_GAS_SETPOINT,                                //!< Режим прокачки газа
  CCS_RGM_PUMP_GAS_DELAY_REACTION,                          //!< Режим прокачки газа
  CCS_RGM_PUMP_GAS_LIMIT_RESTART,                           //!< Режим прокачки газа
  CCS_RGM_CURRENT_LIMIT_MODE,                               //!< Режим ограничения тока
  CCS_RGM_CURRENT_LIMIT_SETPOINT,                           //!< Режим ограничения тока уставка
  CCS_RGM_CURRENT_LIMIT_STEPDOWN,                           //!< Режим ограничения тока шаг снижения частоты
  CCS_RGM_CURRENT_LIMIT_DELAY_REACTION,                     //!< Режим ограничения тока задержка срабатывания
  CCS_RGM_CURRENT_LIMIT_DELAY_RESTART,                      //!< Режим ограничения тока задержка АПВ
  CCS_COEF_VOLTAGE_IN_A,                                    //!< Коэффициент корректировки входного напряжения
  CCS_COEF_VOLTAGE_IN_B,                                    //!< 
  CCS_COEF_VOLTAGE_IN_C,                                    //!< 
  CCS_COEF_OUT_CURRENT_1,                                   //!< Коэффициент корректировки выходного тока фаза U
  CCS_COEF_OUT_CURRENT_2,                                   //!< Коэффициент корректировки выходного тока фаза V
  CCS_COEF_OUT_CURRENT_3,                                   //!< Коэффициент корректировки выходного тока фаза W
  CCS_COEF_RESISTANCE_ISOLATION,                            //!< Коэффициент корректировки сопротивления изоляции
  CCS_NUMBER_CDNG,                                          //!< Номер ЦДНГ
  CCS_NUMBER_BUSH,                                          //!< Номер куста
  CCS_NUMBER_WELL,                                          //!< Номер скважины
  CCS_COUNT_START,                                          //!< Количество пусков
  CCS_PROT_OVERLOAD_COUNT_STOP,                             //!< Количество отключений по перезрузу
  CCS_PROT_UNDERLOAD_COUNT_STOP,                            //!< Количество отключений по недогрузу
  CCS_PROT_OTHER_COUNT_STOP,                                //!< Количество отключений по другим защитам
  CCS_PROT_VOLTAGE_COUNT_RESTART,                           //!< Количество АПВ после остановов по напряжению
  CCS_PROT_OVERLOAD_COUNT_RESTART,                          //!< Количество АПВ после остановов по перегрузу
  CCS_PROT_UNDERLOAD_COUNT_RESTART,                         //!< Количество АПВ после остановов по недогрузу
  CCS_PROT_IMBALANCE_CURRENT_MOTOR_COUNT_RESTART,           //!< Количество АПВ после остановов по дисбалансу токов
  CCS_GENERAL_RUN_DATE_TIME,                                //!< Общее время работы СУ
  CCS_GENERAL_STOP_DATE_TIME,                               //!< Общее время простоя СУ
  CCS_RUN_TIME,                                             //!< Текущая наработка
  CCS_STOP_TIME,                                            //!< Текущий простой
  CCS_RGM_TIMER,                                            //!< Время до изменения режима
  CCS_RESTART_TIMER,                                        //!< Время до запуска
  CCS_LAST_RUN_DATE_TIME,                                   //!< Дата время последнего включения
  CCS_LAST_RUN_REASON,                                      //!< Причина последнего пуска
  CCS_LAST_STOP_DATE_TIME,                                  //!< Дата время последнего отключения
  CCS_LAST_STOP_REASON,                                     //!< Причина носледнего отключения
  CCS_PREVIEW_FREQUENCY_DATE_TIME,                          //!< Дата время изменения частоты
  CCS_PREVIEW_FREQUENCY,                                    //!< Предпоследняя частота
  CCS_TYPE_CCS,                                             //!< Тип КСУ
  CCS_CODE_PRODUCTION_CCS,                                  //!< Код производителя КСУ
  CCS_NUM_PRODUCTION_CCS,                                   //!< Заводской номер КСУ
  CCS_VERSION_SW_CCS,                                       //!< Версия ПО КСУ
  CCS_VERSION_SW_MS,                                        //!< Версия ПО МС
  CCS_VERSION_SW_VSD,                                       //!< Версия ПО ПЧ
  CCS_DATE_PRODUCTION_CCS,                                  //!< Дата изготовления КСУ
  CCS_DATE_INSTALL_SW_CCS,                                  //!< Дата установки ПО КСУ
  CCS_DATE_CREATE_SW_CCS,                                   //!< ДАта генерации ПО КСУ
  CCS_DATE_INSTALL_SW_VSD,                                  //!< Дата установки ПО ПЧ
  CCS_TYPE_SU,                                              //!< Тип СУ
  CCS_CODE_PRODUCTION_SU,                                   //!< Код завода изготовителя СУ
  CCS_NUM_PRODUCTION_SU,                                    //!< Заводской номер СУ
  CCS_DATE_PRODUCTION_SU,                                   //!< Дата изготовления СУ
  CCS_NOTE_1,                                               //!< Примечание 1
  CCS_NOTE_2,                                               //!< Примечание 2
  CCS_NOTE_3,                                               //!< Примечание 3
  CCS_NOTE_4,                                               //!< Примечание 4
  CCS_NOTE_5,                                               //!< Примечание 5
  CCS_NOTE_6,                                               //!< Примечание 6
  CCS_NOTE_7,                                               //!< Примечание 7
  CCS_NOTE_8,                                               //!< Примечание 8
  CCS_NOTE_9,                                               //!< Примечание 9
  CCS_NOTE_10,                                              //!< Примечание 10
  CCS_DATE_TIME,                                            //!< Дата время в формате POSIX
  CCS_DATE_TIME_YEAR,                                       //!< Дата время год
  CCS_DATE_TIME_MONTH,                                      //!< Дата время месяц
  CCS_DATE_TIME_DAY,                                        //!< Дата время дни
  CCS_DATE_TIME_HOUR,                                       //!< Дата время часы
  CCS_DATE_TIME_MIN,                                        //!< Дата время минуты
  CCS_DATE_TIME_SEC,                                        //!< Дата время секунды
  CCS_DATE_TIME_MS,                                         //!< Дата время мс
  CCS_LANGUAGE,                                             //!< Язык интерфейса
  CCS_PASSWORD_ENEBLE,                                      //!< Включение системы паролей
  CCS_PASSWORD_LEVEL,                                       //!< Текущий уровень доступа
  CCS_PASSWORD_LEVEL_1,                                     //!< Пароль 1 уровня
  CCS_PASSWORD_LEVEL_2,                                     //!< Пароль 2 уровня
  CCS_PASSWORD_LEVEL_3,                                     //!< Пароль 3 уровня
  CCS_PASSWORD_LEVEL_4,                                     //!< Пароль 4 уровня
  CCS_PASSWORD_LEVEL_5,                                     //!< Пароль 5 уровня
  CCS_SCADA_TYPE,                                           //!< Протокол телемеханики
  CCS_SCADA_CODE,                                           //!< Версия протокола телемеханики
  CCS_SCADA_ADDRESS,                                        //!< Адресу СУ в телемеханике
  CCS_SCADA_BYTERATE,                                       //!< Скорость опроса в телемеханике
  CCS_SCADA_PARITY,                                         //!< Чётность в телемеханике
  CCS_SCADA_STOPBIT,                                        //!< Количество стоп-бит в телемеханике
  CCS_SCADA_DELAY,                                          //!< Задержка ответа в телемеханкие
  CCS_UNIT_PRESSURE,                                        //!< Единицы измерения давления
  CCS_UNIT_TEMPERATURE,                                     //!< Единицы измерения давления
  CCS_UNIT_VIBRATION,                                       //!< Единицы измерения вибрации
  CCS_UNIT_LENGHT,                                          //!< Единицы измерения длины
  CCS_UNIT_FLOW,                                            //!< Единицы измерения расхода
  CCS_UNIT_PLACE,                                           //!< Единицы измерения площади
  CCS_VSD_CONDITION,                                        //!< Состояние ЧРП
  CCS_CMD_STOP,                                             //!< Команда остановки
  CCS_CMD_START,                                            //!< Команда запуска
  CCS_CMD_LOG_COPY,                                         //!< 
  CCS_CMD_LOG_DELETE,                                       //!< 
  CCS_CMD_SERVICE_LOG_DELETE,                               //!< 
  CCS_CMD_DHS_TYPE_AUTO,                                    //!< 
  CCS_CMD_PROT_ALL_UNBLOCKING,                              //!< 
  CCS_CMD_PROT_ALL_SETPOINT_RESET,                          //!< 
  CCS_CMD_PROT_SUPPLY_UNBLOCKING,                           //!< 
  CCS_CMD_PROT_SUPPLY_SETPOINT_RESET,                       //!< 
  CCS_CMD_PROT_SUPPLY_OVERVOLTAGE_SETPOINT_RESET,           //!< 
  CCS_CMD_PROT_SUPPLY_UNDERVOLTAGE_SETPOINT_RESET,          //!< 
  CCS_CMD_PROT_SUPPLY_IMBALANCE_VOLTAGE_SETPOINT_RESET,     //!< 
  CCS_CMD_PROT_SUPPLY_IMBALANCE_CURRENT_SETPOINT_RESET,     //!< 
  CCS_CMD_PROT_SUPPLY_PHASE_ROTATION_SETPOINT_RESET,        //!< 
  CCS_CMD_PROT_MOTOR_UNBLOCKING,                            //!< 
  CCS_CMD_PROT_MOTOR_SETPOINT_RESET,                        //!< 
  CCS_CMD_PROT_MOTOR_OVERLOAD_SETPOINT_RESET,               //!< 
  CCS_CMD_PROT_MOTOR_UNDERLOAD_SETPOINT_RESET,              //!< 
  CCS_CMD_PROT_MOTOR_IMBALANCE_CURRENT_SETPOINT_RESET,      //!< 
  CCS_CMD_PROT_MOTOR_ASYNC_SETPOINT_RESET,                  //!< 
  CCS_CMD_PROT_MOTOR_OUT_OF_SYNC_SETPOINT_RESET,            //!< 
  CCS_CMD_PROT_DHS_UNBLOCKING,                              //!< 
  CCS_CMD_PROT_DHS_SETPOINT_RESET,                          //!< 
  CCS_CMD_PROT_DHS_PRESSURE_INTAKE_SETPOINT_RESET,          //!< 
  CCS_CMD_PROT_DHS_TEMPERATURE_MOTOR_SETPOINT_RESET,        //!< 
  CCS_CMD_PROT_DHS_RESISTANCE_SETPOINT_RESET,               //!< 
  CCS_CMD_PROT_DHS_VIBRATION_SETPOINT_RESET,                //!< 
  CCS_CMD_PROT_AI_SETPOINT_RESET,                           //!< 
  CCS_CMD_PROT_AI_1_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_AI_2_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_AI_3_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_AI_4_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_DI_SETPOINT_RESET,                           //!< 
  CCS_CMD_PROT_DI_1_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_DI_2_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_DI_3_SETPOINT_RESET,                         //!< 
  CCS_CMD_PROT_DI_4_SETPOINT_RESET,                         //!< 
  CCS_CMD_RGM_MODE_SAVE,                                    //!< 
  CCS_CMD_COUNTER_ALL_RESET,                                //!< 
  CCS_VOLTAGE_TRANS_OUT,                                    //!< 
  CCS_CONDITION_FLAG,                                       //!< 
  CCS_LAST_RUN_REASON_TMP,                                  //!< Причина последнего пуска - регистр временного хранения значения
  CCS_PROT_PREVENT,                                         //!< Наличие хотя бы одного запрещающего параметра
  CCS_TIMER_DIFFERENT_START,                                //!< Уставка ВРП (время разновременного пуска)
  CCS_RUN_BEGIN_TIME,                                       //!< Время начало текущей наработки
  CCS_STOP_BEGIN_TIME,                                      //!< Время начало текущего простоя
  CCS_PROT_SUPPLY_OVERVOLTAGE_BLOCK_FLAG,                   //!< 
  CCS_PROT_SUPPLY_UNDERVOLTAGE_BLOCK_FLAG,                  //!< 
  CCS_PROT_SUPPLY_IMBALANCE_VOLTAGE_BLOCK_FLAG,             //!< 
  CCS_PROT_SUPPLY_IMBALANCE_CURRENT_BLOCK_FLAG,             //!< 
  CCS_PROT_SUPPLY_PHASE_ROTATION_BLOCK_FLAG,                //!< 
  CCS_PROT_MOTOR_OVERLOAD_BLOCK_FLAG,                       //!< 
  CCS_PROT_MOTOR_UNDERLOAD_BLOCK_FLAG,                      //!< 
  CCS_PROT_MOTOR_CURRENT_BLOCK_FLAG,                        //!< 
  CCS_PROT_MOTOR_IMBALANCE_CURRENT_BLOCK_FLAG,              //!< 
  CCS_PROT_MOTOR_ASYNC_BLOCK_FLAG,                          //!< 
  CCS_PROT_MOTOR_OUT_OF_SYNC_BLOCK_FLAG,                    //!< 
  CCS_PROT_DHS_PRESSURE_INTAKE_BLOCK_FLAG,                  //!< 
  CCS_PROT_DHS_TEMPERATURE_MOTOR_BLOCK_FLAG,                //!< 
  CCS_PROT_DHS_RESISTANCE_BLOCK_FLAG,                       //!< 
  CCS_PROT_DHS_VIBRATION_BLOCK_FLAG,                        //!< 
  CCS_PROT_AI_1_BLOCK_FLAG,                                 //!< 
  CCS_PROT_AI_2_BLOCK_FLAG,                                 //!< 
  CCS_PROT_AI_3_BLOCK_FLAG,                                 //!< 
  CCS_PROT_AI_4_BLOCK_FLAG,                                 //!< 
  CCS_PROT_DI_1_BLOCK_FLAG,                                 //!< 
  CCS_PROT_DI_2_BLOCK_FLAG,                                 //!< 
  CCS_PROT_DI_3_BLOCK_FLAG,                                 //!< 
  CCS_PROT_DI_4_BLOCK_FLAG,                                 //!< 
  CCS_PROT_OTHER_LIMIT_RESTART_BLOCK_FLAG,                  //!< 
  CCS_PROT_OTHER_LOCK_DOOR_BLOCK_FLAG,                      //!< 
  CCS_PROT_OTHER_VSD_BLOCK_FLAG,                            //!< 
  CCS_PROT_OTHER_IMB_BLOCK_FLAG,                            //!< 
  CCS_GENERAL_CONDITION,                                    //!< Общее состояние
  CCS_COEF_TRANSFORMATION,                                  //!< Коэффициент трансформации
  CCS_LAST_STOP_REASON_TMP,                                 //!< Причина последнего останова - регистр временного хранения значения
  CCS_END,                                                  //!< Последний параметр
  VSD_BEGIN=10000,                                          //!< Первый параметр ЧРП
  VSD_FREQUENCY_NOW,                                        //!< Выходная частота ПЧ
  VSD_FREQUENCY,                                            //!< Фиксированная скорость Danfoss 3-11 Уставка частоты, основной задаваемый параметр Novomet 0х0034 IREG_FREQ_REF
  VSD_SPEED_RPM_NOW,                                        //!< Обороты двигателя
  VSD_OUT_VOLTAGE_MOTOR,                                    //!< Напряжение на выходе ПЧ
  VSD_MOTOR_TYPE,                                           //!< Конструкция двигателя Danfoss 1-10 Используется в интерфейсе
  VSD_MOTOR_CONTROL,                                        //!< Принцип управления двигателем Danfoss 1-01 Не используется в проекте, только при конфигурировании
  VSD_ROTATION,                                             //!< Направления вращения Danfoss 4-10 Не используется в проекте, только при конфигурировании
  VSD_CURRENT_OUT_PHASE_1,                                  //!< Выходной ток ПЧ фаза U
  VSD_CURRENT_OUT_PHASE_2,                                  //!< Выходной ток ПЧ фаза V
  VSD_CURRENT_OUT_PHASE_3,                                  //!< Выходной ток ПЧ фаза W
  VSD_CURRENT_DC,                                           //!< Ток звена постоянного тока
  VSD_VOLTAGE_DC,                                           //!< Напряжение цепи пост.тока Danfoss 16-30 Используется в проекте Novomet 0х008B IREG_UD
  VSD_POWER_ACTIVE,                                         //!< Активная мощность Danfoss 16-10 Используется в проекте Novomet 0х0071 IREG_POUT
  VSD_POWER_FULL,                                           //!< Полная мощность
  VSD_MOTOR_POWER,                                          //!< Мощность двигателя Danfoss 1-20 Используется в проекте, интерфейс
  VSD_MOTOR_VOLTAGE,                                        //!< Напряжение двигателя Danfoss 1-22 Используется в проекте, интерфейс Novomet 0х0045 IREG_MOTOR_U_MAх
  VSD_MOTOR_CURRENT,                                        //!< Номинальный ток двигателя Danfoss 1-24 Используется в проекте, интерфейс Novomet 0х0044 IREG_MOTOR_I_MAх
  VSD_MOTOR_SPEED,                                          //!< Скорость двигателя Danfoss 1-25 Используется в проекте, интерфейс
  VSD_MOTOR_FREQUENCY,                                      //!< Частота двигателя Danfoss 1-23 Используется в проекте, интерфейс Novomet 0х0043 IREG_MOTOR_F_MAх
  VSD_MOTOR_COS_PHI,                                        //!< Номинальный косинус Фи Danfoss 14-43 Используется в проекте
  VSD_D_AXIS_INDUNSTANCE,                                   //!< Индуктивности по оси D Danfoss 1-37 Используется в проекте
  VSD_BACK_EMF,                                             //!< Противо эдс Danfoss 1-40 Используется в проекте
  VSD_UF_UHH,                                               //!< Ток холостого хода
  VSD_MOTOR_POLES,                                          //!< Число полюсов двигателя Danfoss 1-39 Используется в проекте
  VSD_LOW_LIM_SPEED_MOTOR,                                  //!< Нижний предел скорости Danfoss 4-12 Гц Используется в проекте
  VSD_HIGH_LIM_SPEED_MOTOR,                                 //!< Верхний передел скорости Danfoss 4-14 Используется в проекте Novomet 0х0037 IREG_FREQ_REF_MAх
  VSD_T_SPEEDUP,                                            //!< Период нарастания частоты Novomet 0х0038 IREG_T_SPEEDUP
  VSD_T_SPEEDDOWN,                                          //!< Период Период снижения частоты Novomet 0х0039 IREG_T_SPEEDDOWN
  VSD_UF_TYPE,                                              //!< U/f характеристика
  VSD_UF_CHARACTERISTIC_U_1,                                //!< Первая точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_1,                                //!< Первая точка частоты U/f характеристика
  VSD_UF_CHARACTERISTIC_U_2,                                //!< Вторая точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_2,                                //!< Вторая точка частоты U/f характеристика
  VSD_UF_CHARACTERISTIC_U_3,                                //!< Третья точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_3,                                //!< Третья точка частоты U/f характеристика
  VSD_UF_CHARACTERISTIC_U_4,                                //!< Четвертая точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_4,                                //!< Четвертая точка частоты U/f характеристика
  VSD_UF_CHARACTERISTIC_U_5,                                //!< Пятая точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_5,                                //!< Пятая точка частоты U/f характеристика
  VSD_UF_CHARACTERISTIC_U_6,                                //!< Шестая точка напряжения U/f характеристика
  VSD_UF_CHARACTERISTIC_F_6,                                //!< Шестая точка частоты U/f характеристика
  VSD_THYR_ANGLE_REFERENCE,                                 //!< Угол управления тиристорами;
  VSD_THYR_STATUS,                                          //!< Статус платы выпрямителя.
  VSD_THYR_CONTROL,                                         //!< Регистр управления тиристорами;
  VSD_THYR_VOLT_SHORT_CUILT,                                //!< Напряжение защиты от КЗ инвертора.
  VSD_INDICATOR_STATUS,                                     //!< Статус индикатора
  VSD_DOUTPUTS,                                             //!< Маска состояния цифровых выходов.
  VSD_DINPUTS,                                              //!< Маска состояния цифровых входов.
  VSD_DOUTPUTS_ENABLE,                                      //!< Маска включения цифровых выходов.
  VSD_DOUTPUTS_DISABLE,                                     //!< Маска выключения цифровых выходов.
  VSD_VENT_ON_TEMPERATURE,                                  //!< Температура включения главного вентилятора.
  VSD_VENT_OFF_TEMPERATURE,                                 //!< Температура выключения главного вентилятора.
  VSD_VENT_PERIOD,                                          //!< Период проверки температуры радиаторов
  VSD_VENT_TEMPERATURE_FAULT,                               //!< Температура срабатывания тепловой защиты радиаторов
  VSD_INVERTOR_CONTROL,                                     //!< Флаговый регистр управления
  VSD_INVERTOR_STATUS,                                      //!< Регистр состояния инвертора
  VSD_FREQUENCY_ERROR,                                      //!< Уставка частоты "тревожного" режима
  VSD_T_ILIMIT_SPEEDDOWN,                                   //!< Период снижения частоты при токоограничении.
  VSD_T_ULOW_SPEEDDOWN,                                     //!< Период снижения частоты при нехватке напряжения на инверторе
  VSD_CURRENT_REGULATOR,                                    //!< Тип текущего алгоритма управления (регулятора)
  VSD_UF_U_FORCE,                                           //!< Напряжение форсировки
  VSD_UF_F_FORCE,                                           //!< Частота форсировки
  VSD_IFB_COMP,                                             //!< Постоянная времени компенсации индуктивности
  VSD_VUOUT_PROP,                                           //!< Пропорциональный коэффициент ОС частотного контура регулирования
  VSD_ILIMIT,                                               //!< Максимальный порог ограничения тока на выходе ПЧ, абс. вел
  VSD_VUOUT_INTEG,                                          //!< Интегральный коэффициент ОС частотного контура регулирования
  VSD_VTOUF_INTEG,                                          //!< Коэффициент постоянной времени обратного перехода с вектора на U/f
  VSD_LOUT,                                                 //!< Суммарная индуктивность на выходе ПЧ, фазное значение
  VSD_SW_STARTUP_FREQUENCY,                                 //!< Режим раскачки: частота
  VSD_SW_STARTUP_ANGLE_OSC,                                 //!< Режим раскачки: угол качания
  VSD_SW_STARTUP_ANGLE_SHIFT,                               //!< Режим раскачки: угол сдвига
  VSD_SW_STARTUP_OSC_COUNT,                                 //!< Режим раскачки: количество качаний на угол
  VSD_SW_STARTUP_ROTATIONS,                                 //!< Режим раскачки: количество оборотов двигателя
  VSD_SW_STARTUP_U_PULSE,                                   //!< Режим раскачки: кратность напряжения импульса расклинки
  VSD_SW_STARTUP_I_LIM,                                     //!< Режим раскачки: токоограничение
  VSD_SW_STARTUP_I_LIM_PULSE,                               //!< Порог токоограничения импульса расклинки в режиме с раскачкой
  VSD_VFREQ_INTEG,                                          //!< Интегральный коэффициент ОС фазового контура регулирования
  VSD_REGULATOR_QUEUE_1,                                    //!< Регистры рабочей очереди алгоритмов управления
  VSD_REGULATOR_QUEUE_2,                                    //!< Регистры рабочей очереди алгоритмов управления
  VSD_REGULATOR_QUEUE_3,                                    //!< Регистры рабочей очереди алгоритмов управления
  VSD_REGULATOR_QUEUE_4,                                    //!< Регистры рабочей очереди алгоритмов управления
  VSD_REGULATOR_QUEUE_5,                                    //!< Регистры рабочей очереди алгоритмов управления
  VSD_UD_LOW_FAULT,                                         //!< Порог защиты ПЧ по снижению напряжения на инверторе
  VSD_UD_HIGH_FAULT,                                        //!< Порог защиты ПЧ по превыщению напряжения на инверторе
  VSD_UIN_ASYM_LEVEL,                                       //!< Порог защиты ПЧ по несимметрии входного напряжения на инверторе
  VSD_UIN_ASYM_DELAY,                                       //!< Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
  VSD_F_VECT,                                               //!< Частота перехода на векторный алгоритм
  VSD_VFREQ_GAIN,                                           //!< Пропорциональный коэффициент ОС фазового контура регулирования
  VSD_VANGLE_GAIN,                                          //!< Фазовый коэффициент усиления системы подавления резонанса
  VSD_RES_TIMECONST,                                        //!< Постоянная времени системы подавления резонанса
  VSD_RES_IAMP_VALID,                                       //!< Служебный регистр для отладки алгоритмов управления
  VSD_IB,                                                   //!< Выходной ток ПЧ по фазе В. (амплитудное значение)
  VSD_PHB,                                                  //!< Фаза выходного тока В
  VSD_IC,                                                   //!< Выходной ток ПЧ по фазе C. (амплитудное значение)
  VSD_PHC,                                                  //!< Фаза выходного тока C
  VSD_DECEL_SPEEDDOWN,                                      //!< Время снижения уставки в режиме остановки турбинного вращения
  VSD_DECEL_VOLTAGE,                                        //!< Напряжение остаточного напряжения на шине (окончания разряда) в режиме остановки турбинного вращения (для ВД)
  VSD_DECEL_F_MAX,                                          //!< Начальная частота режима остановки турбинного вращения (для АД)
  VSD_TIME_SECOND,                                          //!< Время секунды
  VSD_TIME_MINUTE,                                          //!< Время минуты
  VSD_TIME_HOUR,                                            //!< Время часы
  VSD_TIME_DAY,                                             //!< Время дни
  VSD_TIME_MONTH,                                           //!< Время месяцы
  VSD_TIME_YEAR,                                            //!< Время годы
  VSD_AST_IMEAS,                                            //!< Величина измерительного тока автонастройки
  VSD_AST_UMEAS,                                            //!< Величина измерительного напряжения автонастройки
  VSD_AST_LOUT_1_0,                                         //!< Измеренная величина выходной индуктивности автонастройки
  VSD_AST_STANDING,                                         //!< Дополнительный регистр автонастройки, зарезервирован
  VSD_INVERTOR_STATUS2,                                     //!< Расширенный регистр состояния инвертора
  VSD_JARRING_PERIOD,                                       //!< Период экскурсий частоты режима встряхивания.
  VSD_JARRING_UPDFREQ,                                      //!< Величина прибавки частоты вращения выше уставки в режиме встряхивания
  VSD_JARRING_UPTIME,                                       //!< Время прибавки частоты вращения выше уставки в режиме встряхивания
  VSD_JARRING_DOWNDFREQ,                                    //!< Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
  VSD_JARRING_DOWNTIME,                                     //!< Время уменьшения частоты вращения ниже уставки в режиме встряхивания
  VSD_JARRING_MODE,                                         //!< Регистры обслуживания внутренних нужд режима встряхивания
  VSD_JARRING_TICK_CNT,                                     //!< Регистры обслуживания внутренних нужд режима встряхивания
  VSD_JARRING_SECOND_CNT,                                   //!< Регистры обслуживания внутренних нужд режима встряхивания
  VSD_START_FREQ,                                           //!< Стартовая частота вращения двигателя в основном режиме регулирования
  VSD_IA,                                                   //!< Выходной ток ПЧ по фазе A. (амплитудное значение)
  VSD_PHA,                                                  //!< Фаза выходного тока A
  VSD_PHOUT,                                                //!< Усредненная фаза выходного тока ПЧ
  VSD_TEST0,                                                //!< Тестовый регистр 0
  VSD_TEST1,                                                //!< Тестовый регистр 1
  VSD_TEST2,                                                //!< Тестовый регистр 2
  VSD_TEST3,                                                //!< Тестовый регистр 3
  VSD_TEST4,                                                //!< Тестовый регистр 4
  VSD_TEST5,                                                //!< Тестовый регистр 5
  VSD_TEST6,                                                //!< Тестовый регистр 6
  VSD_TEST7,                                                //!< Тестовый регистр 7
  VSD_IOUT,                                                 //!< Выходной ток ПЧ
  VSD_INV_FAULT,                                            //!< Регистр ошибок инвертора
  VSD_SOUT,                                                 //!< Текущая выходная полная мощность ПЧ
  VSD_M_IRMS,                                               //!< Уровень защиты i2t двигателя
  VSD_M_TRMS,                                               //!< Постоянная времени защиты i2t двигателя
  VSD_OVERPWM2_ANGLE,                                       //!< Максимальный угол остановки вектора в режиме
  VSD_OVERPWM_GAIN,                                         //!< Степень постоянной времени отработки
  VSD_DECEL_U_MAX,                                          //!< Стартовое напряжение торможения авторотации
  VSD_FC_IRMS,                                              //!< Ток тепловой защиты ПЧ длительный
  VSD_FC_ILIM_LONG,                                         //!< Ограничение тока для импульсов
  VSD_FC_TLIM,                                              //!< Время импульса с общим (максимальным) токоограничением
  VSD_U_SCALE,                                              //!< Масштаб измеряемого напряжения ПЧ отн. базового 250А
  VSD_I_SCALE,                                              //!< Масштаб измеряемого тока ПЧ отн. базового 250А
  VSD_M_I_FAST,                                             //!< Уровень быстрой токовой защиты двигателя
  VSD_T_BLANK,                                              //!< Время отключения защиты двигателя после старта
  VSD_PID_I_REF,                                            //!< Уровень уставки тока для ПИД-регулятора
  VSD_PID_T_REG,                                            //!< Постоянная времени для ПИД-регулятора
  VSD_PID_T_REG_1,                                          //!< Постоянная времени для ПИД-регулятора
  VSD_INVERTOR_STATUS3,                                     //!< Расширенный регистр состояния инвертора 3
  VSD_CONFIG_MODE,                                          //!< Режим конфигурирования Danfoss 1-00 Не используется в проекте, только при конфигурировании
  VSD_TIMER_DISPERSAL,                                      //!< Время разгона Danfoss 3-41 % Активно используется в проекте
  VSD_TIMER_DELAY,                                          //!< Время замедления Danfoss 3-42 % Активно используется в проекте
  VSD_INDEX,                                                //!< Указатель индекса для массивных параметров Danfoss 0х0008 используется для массивов параметров
  VSD_UNIT_SPEED,                                           //!< Единицы измерения скорости Danfoss 0-02 Не используется в проекте, только при конфигурировании
  VSD_TORQUE_CHARACTERISTIC,                                //!< Характеристика момента нагрузки Danfoss 1-03 Не используется в проекте, только при конфигурировании
  VSD_OVERLOAD_MODE,                                        //!< Режим перегрузки Danfoss 1-04 Не используется в проекте, только при конфигурировании
  VSD_DAMPING_GANE,                                         //!< Усиление демпфирования Danfoss 1-14 Не используется в проекте, только при конфигурировании
  VSD_LOW_SPEED_FILTER_TIME,                                //!< Время фильтрации при низкой скорости Danfoss 1-15 Используется в режиме автоадаптации
  VSD_HIGH_SPEED_FILTER_TIME,                               //!< Время фильтрации при высокой скорости Danfoss 1-16 Используется в режиме автоадаптации
  VSD_RATE_TORQUE_MOTOR,                                    //!< Длительный номинальный момент двигателе Danfoss 1-26 Записывается в проекте один раз
  VSD_RESISTANCE_STATOR,                                    //!< Сопротивление статора Danfoss 1-30 Используется в режиме автоадаптации
  VSD_UF_CHARACTERISTIC_U,                                  //!< Характеристика Uf - U Danfoss 1-55
  VSD_UF_CHARACTERISTIC_F,                                  //!< Характеристика Uf - f Danfoss 1-56
  VSD_RESONANCE_REMOVE,                                     //!< Подавление резонанса % Danfoss 1-64 Не используется в проекте, только при конфигурировании
  VSD_RESONANCE_TIME,                                       //!< Постаянная времени подавления Danfoss 1-65 Не используется в проекте, только при конфигурировании Novomet 0х0067
  VSD_MIN_CURRENT_LOW_SPEED,                                //!< Минимальный ток при низкой скорости Danfoss 1-66 Не используется в проекте, только при конфигурировании
  VSD_PM_START_MODE,                                        //!< Тип запуска вентиля Danfoss 1-70 Не используется в проекте, только при конфигурировании
  VSD_START_DELAY,                                          //!< Задержка запуска Danfoss 1-71 Используется в защите от невыхода на заданную частоту
  VSD_START_FUNCTION,                                       //!< Функция запуска Danfoss 1-72 Не используется в проекте, только при конфигурировании
  VSD_STOP_FUNCTION,                                        //!< Функция при остановке Danfoss 1-80 Не используется в проекте, только при конфигурировании
  VSD_STOP_SPEED,                                           //!< Минимальная скорость для функции при останове Danfoss 1-82 Не используется в проекте, только при конфигурировании
  VSD_CONTROL_TERMISTOR_MTR,                                //!< Контроль термистора двигателя Danfoss 1-90 Не используется в проекте, только при конфигурировании
  VSD_THERMISTOR_RESOURCE,                                  //!< Вход термистора Danfoss 1-93 Не используется в проекте, только при конфигурировании
  VSD_PARKING_CURRENT,                                      //!< Начальный ток Danfoss 2-06 Не используется в проекте, только при конфигурировании
  VSD_PARKING_TIME,                                         //!< Длительность действия начального ток Danfoss 2-07 В проекте есть, но отключено, используется при конфигурировании
  VSD_OVERVOLTAGE_CONTROL,                                  //!< Контроль перенапряжения Danfoss 2-17 Не используется в проекте, только при конфигурировании
  VSD_MIN_REFERENCE,                                        //!< Минимальное задание Danfoss 3-02 Не используется в проекте, только при конфигурировании
  VSD_MAX_REFERENCE,                                        //!< Максимальное задани Danfoss 3-03 Не используется в проекте, только при конфигурировании
  VSD_RESOURCE_TASK_1,                                      //!< Ресурс задания 1 Danfoss 3-15 Не используется в проекте, только при конфигурировании
  VSD_RESOURCE_TASK_2,                                      //!< Ресурс задания 2 Danfoss 3-16 Не используется в проекте, только при конфигурировании
  VSD_RESOURCE_TASK_3,                                      //!< Ресурс задания 3 Danfoss 3-17 Не используется в проекте, только при конфигурировании
  VSD_TYPE_SPEED_CHANGE,                                    //!< Тип изменения скорости Danfoss 3-40 Не используется в проекте, только при конфигурировании
  VSD_TIMER_DISP_FIX_SPEED,                                 //!< Время разгона фиксированной скорости Danfoss 3-80 Используется в проекте один раз
  VSD_TIMER_DELAY_FIX_SPEED,                                //!< Время замедления фиксированной скорости Danfoss 3-81 Используется в проекте один раз
  VSD_TORQUE_LIMIT,                                         //!< Ограничение момента Danfoss 4-16 % Используется в защитах ЧРП. Интерфейс
  VSD_TORQUE_LIMIT_GEN,                                     //!< Ограничение момента в режиме генератора Danfoss 4-17 % Не используется в проекте, только при конфигурировании
  VSD_CURRENT_LIMIT,                                        //!< Предел тока Danfoss 4-18 %
  VSD_MAX_OUTPUT_FREQUENCY,                                 //!< Максимальная выходная частота Danfoss 4-19 Используется в проекте
  VSD_MTR_FEEDBACK_LOSS_FUNC,                               //!< Реакция на ошибку обратной связи Danfoss 4-30 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR_FUNCTION,                                 //!< Реакция на ошибку задания частоты Danfoss 4-34 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR,                                          //!< Величина несоответствия частота Danfoss 4-35 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR_TIMEOUT,                                  //!< Величина несоответствия частоты Danfoss 4-36 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR_RAMPING,                                  //!< Величина несоответствия частоты при разгоне Danfoss 4-37 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR_RAMP_TIME,                                //!< Задержка срабатывания при несоответствии частоты при разгоне Danfoss 4-38 Не используется в проекте, только при конфигурировании
  VSD_TRACK_ERROR_AFTER_RAMP,                               //!< Tracking Error After Ramping Timeout Danfoss 4-39 Не используется в проекте, только при конфигурировании
  VSD_WARNING_CURRENT_LOW,                                  //!< Danfoss 4-50 Не используется в проекте, только при конфигурировании
  VSD_WARNING_CURRENT_HIGH,                                 //!< Danfoss 4-51 Не используется в проекте, только при конфигурировании
  VSD_WARNING_SPEED_LOW,                                    //!< Минимальная скорость Danfoss 4-52 об/мин Не используется в проекте, только при конфигурировании
  VSD_TERMINAL_27_MODE,                                     //!< Режим клеммы 27 Danfoss 5-01 Не используется в проекте, только при конфигурировании
  VSD_TERMINAL_29_MODE,                                     //!< Режим клеммы 29 Danfoss 5-02 Не используется в проекте, только при конфигурировании
  VSD_DI_18,                                                //!< Цифровой вход 18 Danfoss 5-10 Не используется в проекте, только при конфигурировании
  VSD_DI_19,                                                //!< Цифровой вход 19 Danfoss 5-11 Не используется в проекте, только при конфигурировании
  VSD_DI_27,                                                //!< Цифровой вход 27 Danfoss 5-12 Не используется в проекте, только при конфигурировании
  VSD_DI_32,                                                //!< Цифровой вход 32 Danfoss 5-14 Не используется в проекте, только при конфигурировании
  VSD_DI_33,                                                //!< Цифровой вход 33 Danfoss 5-15 Не используется в проекте, только при конфигурировании
  VSD_TERMINAL_27_DI,                                       //!< Клемма 27 Цифровой вход Danfoss 5-30 Не используется в проекте, только при конфигурировании
  VSD_TERMINAL_29_DI,                                       //!< Клемма 29 Цифровой вход Danfoss 5-31 Не используется в проекте, только при конфигурировании
  VSD_FUNCTION_RELE,                                        //!< Реле функций Danfoss 5-40 Не используется в проекте, только при конфигурировании
  VSD_ON_DELAY_RELAY,                                       //!< Нипонятно Danfoss 5-41 Не используется в проекте, только при конфигурировании
  VSD_42_AO,                                                //!< Клемма 42 аналоговый вход Danfoss 6-50 Не используется в проекте, только при конфигурировании
  VSD_SL_CONTROLLER_MODE,                                   //!< Режим контроллера SL Danfoss 13-00 Не используется в проекте, только при конфигурировании
  VSD_SL_START_EVENT,                                       //!< Событие запуска Danfoss 13-01 Не используется в проекте, только при конфигурировании
  VSD_SL_STOP_EVENT,                                        //!< Событие останова Danfoss 13-02 Не используется в проекте, только при конфигурировании
  VSD_SL_RESET,                                             //!< Сброс настроек компараторов Danfoss 13-03 Не используется в проекте, только при конфигурировании
  VSD_SL_10,                                                //!< Операнд сравнения компаратора Danfoss 13-10 Не используется в проекте, только при конфигурировании
  VSD_SL_11,                                                //!< Оператор сравнения компаратора Danfoss 13-11 Не используется в проекте, только при конфигурировании
  VSD_SL_12,                                                //!< Значение компаратора Danfoss 13-12 Не используется в проекте, только при конфигурировании
  VSD_SL_15,                                                //!< RS-FF Operand S Danfoss 13-15 Не используется в проекте, только при конфигурировании
  VSD_SL_16,                                                //!< RS-FF Operand R Danfoss 13-16 Не используется в проекте, только при конфигурировании
  VSD_SL_20,                                                //!< Временная задержка Danfoss 13-20 Не используется в проекте, только при конфигурировании
  VSD_SL_40,                                                //!< Булева переменная логического соотношения 1 Danfoss 13-40 Не используется в проекте, только при конфигурировании
  VSD_SL_41,                                                //!< Оператор логического соотношения 1 Danfoss 13-41 Не используется в проекте, только при конфигурировании
  VSD_SL_42,                                                //!< Булева переменная логического соотношения 2 Danfoss 13-42 Не используется в проекте, только при конфигурировании
  VSD_SL_43,                                                //!< Logic Rule Operator 2 Danfoss 13-43 Не используется в проекте, только при конфигурировании
  VSD_SL_44,                                                //!< Logic Rule Boolean 3 Danfoss 13-44 Не используется в проекте, только при конфигурировании
  VSD_SL_51,                                                //!< Событие контроллера SL Danfoss 13-51 Не используется в проекте, только при конфигурировании
  VSD_SL_52,                                                //!< Действие контроллера SL Danfoss 13-52 Не используется в проекте, только при конфигурировании
  VSD_SWITCHING_FREQUENCY,                                  //!< Частота коммутации Danfoss 14-01 Используется в проекте Novomet 0х003C IREG_FREQ_PWM
  VSD_OVERMODULATION,                                       //!< Сверхмодуляция Danfoss 14-03 Не используется в проекте, только при конфигурировании
  VSD_DEAD_TIME_COMPENSATION,                               //!< Компенсация задержки Danfoss 14-06 Не используется в проекте, только при конфигурировании
  VSD_MAINS_FAILURE,                                        //!< Отказ питания Danfoss 14-10 Не используется в проекте, только при конфигурировании
  VSD_MAINS_VOLTAGE_FAILURE,                                //!< Напряжение сети при отказе питания Danfoss 14-11 Не используется в проекте, только при конфигурировании
  VSD_RESET_MODE,                                           //!< Режим сброса Danfoss 14-20 Не используется в проекте, только при конфигурировании
  VSD_AUTOSTART_TIME,                                       //!< Время автоматического перезапуск Danfoss 14-21 Не используется в проекте, только при конфигурировании
  VSD_DELAY_CURRENT_LIMIT,                                  //!< Задержка перед выключение при пределе тока Danfoss 14-24 Не используется в проекте, только при конфигурировании
  VSD_DELAY_TORQUE_LIMIT,                                   //!< Задержка перед выключение при пределе моменте Danfoss 14-25 Не используется в проекте, только при конфигурировании
  VSD_FIL_TIME_CURRENT_LIMIT,                               //!< Время фильтра регулятора тока Danfoss 14-32 Используется в проекте, CalcParam()
  VSD_DC_COMPENSATION,                                      //!< Компенсация напряжения постоянного тока Danfoss 14-51 Не используется в проекте, только при конфигурировании
  VSD_FAN_CONTROL,                                          //!< Управление вентилятором Danfoss 14-52 Не используется в проекте, только при конфигурировании
  VSD_OUT_FILTER,                                           //!< Выходной фильтр Danfoss 14-55 Используется в проекте, интерфейс
  VSD_FAIL_RESET,                                           //!< Сброс аварий Danfoss 14-90 Не используется в проекте, только при конфигурировании
  VSD_SOFT_VERSION,                                         //!< Версия ПО ЧРП Danfoss 15-43 Используется в проекте
  VSD_COMMAND_WORD,                                         //!< Командное слово Danfoss 16-00 Используется в проекте
  VSD_STATUS_WORD,                                          //!< Слово состояния Danfoss 16-03 Активно используется в проекте
  VSD_OUT_CURRENT_MOTOR,                                    //!< Выходной ток средний
  VSD_RADIATOR_TEMPERATURE,                                 //!< Температура радиатора Danfoss 16-34 Используется в проекте
  VSD_CONTROL_TEMPERATURE,                                  //!< Температура платы управления Danfoss 16-39 Используется в проекте
  VSD_DI_VSD,                                               //!< Цифровоый вход ПЧ Danfoss 16-60 Используется в проекте
  VSD_LAST_ALARM,                                           //!< Номер последней аварии и предупреждения Danfoss 16-87 Используется в проекте
  VSD_ALARM_WORD_1,                                         //!< Слово Аварии 1 Danfoss 16-90 Используется в проекте
  VSD_ALARM_WORD_2,                                         //!< Слово Аварии 2 Danfoss 16-91 Используется в проекте
  VSD_WARNING_WORD_1,                                       //!< Слово предупреждения 1 Danfoss 16-92 Используется в проекте
  VSD_WARNING_WORD_2,                                       //!< Слово предупреждения 2 Danfoss 16-93 Используется в проекте
  VSD_HIGH_START_TORQUE_TIME,                               //!< Время применения высокого пускового момента Danfoss 30-20 Не используется в проекте, только при конфигурировании
  VSD_HIGH_START_TORQUE_CURRENT,                            //!< Пусковой ток в % от номинального Danfoss 30-21 Не используется в проекте, только при конфигурировании
  VSD_LOCK_ROTOR_PROTECTION,                                //!< Защита блокировки ротора Danfoss 30-22 Не используется в проекте, только при конфигурировании
  VSD_LOCK_ROTOR_TIME,                                      //!< Время задержки аварии блокировки ротора Danfoss 30-23 Не используется в проекте, только при конфигурировании
  VSD_TEMP_SPEEDUP,                                         //!< 
  VSD_TEMP_SPEEDDOWN,                                       //!< 
  VSD_END,                                                  //!< 
  TMS_BEGIN=15000,                                          //!< 
  TMS_RESISTANCE_ISOLATION,                                 //!< Сопротивление изоляции
  TMS_PRESSURE_INTAKE,                                      //!< Давление на приёме
  TMS_TEMPERATURE_WINDING,                                  //!< Температура обмоток двигателя
  TMS_TEMPERATURE_INTAKE,                                   //!< Температура на приёме
  TMS_ACCELERATION_X_INTAKE,                                //!< Ускорение по оси X на приёме
  TMS_ACCELERATION_Y_INTAKE,                                //!< Ускорение по оси Y на приёме
  TMS_ACCELERATION_Z_INTAKE,                                //!< Ускорение по оси Z на приёме
  TMS_PRESSURE_DISCHARGE,                                   //!< Давление на выкиде
  TMS_TEMPERATURE_DISCHARGE,                                //!< Температура на выкиде
  TMS_FLOW_DISCHARGE,                                       //!< Расход на выкиде
  TMS_ACCELERATION_XY_INTAKE,                               //!< Ускорение по оси Y на приёме
  TMS_ACCELERATIN_XYZ_INTAKE,                               //!< Ускорение по оси по трём осям
  TMS_MAX_PRESSURE_INTAKE,                                  //!< Максимальное давление на приёме
  TMS_MAX_TEMPERATUR_WINDING,                               //!< Максимальная температура обмоток
  TMS_MAX_TEMPERATURE_INTAKE,                               //!< Максимальная температура на приёме
  TMS_MAX_ACCELERATION_INTAKE,                              //!< Максимальное ускорение
  TMS_FAIL_LINK_TMSP,                                       //!< Обрыв связи с ТМСП
  TMS_MANUFACTURE_TMSP,                                     //!< Код изготовителя ТМСП
  TMS_NUMBER_TMSP,                                          //!< Серийный номер ТМСП
  TMS_DATE_TMSP,                                            //!< Дата изготовления ТМСП
  TMS_CONSTRUCTION_TMSP,                                    //!< Код конструкции ТМСП
  TMS_PROTOCOL_TMSP,                                        //!< Код протокола ТМСП
  TMS_SENSOR_TMSP,                                          //!< Состав датчиков ТМСП
  TMS_SOFT_TMSP,                                            //!< Версия По ТМСП
  TMS_TIME_BIT,                                             //!< Скорости передачи бита
  TMS_MANUFACTURE_TMSN,                                     //!< Код завода изготовителя ТМСН
  TMS_NUMBER_TMSN,                                          //!< Заводской номер ТМСН
  TMS_DATE_TMSN,                                            //!< Дата изготовления ТМСН
  TMS_CONSTRUCTION_TMSN,                                    //!< Код конструкции ТМСН
  TMS_SOFT_TMSN,                                            //!< Версия ПО ТМСН
  TMS_PROTOCOL_TMSN,                                        //!< Код протокола ТМСН
  TMS_MIN_FLOW_DISCHARGE,                                   //!< Минимальный расход на выкиде
  TMS_MAX_FLOW_DISCHARGE,                                   //!< Максимальный расход на выкиде
  TMS_MAX_PRESSURE_DISHARGE,                                //!< Максимальное давление на выкиде
  TMS_MAX_TEMPERATURE_DISCHARGE,                            //!< Максимальная температура на выкиде
  TMS_RESISTANCE_ISOLATION_TIME,                            //!< Время измерения сопротивления изоляции
  TMS_MEASURE_TIME,                                         //!< Время измерения данных и передачи
  TMS_RESISTANCE_TRANS_TIME,                                //!< Время перехода в режим измерения сопротивления изоляции
  TMS_NUMBER_WELL,                                          //!< Номер скважины
  TMS_NUMBER_BUSH,                                          //!< Номер куста
  TMS_NUMBER_CDNG,                                          //!< Номер ЦДНГ
  TMS_MODBUS_ADDRESS,                                       //!< Адрес Modbus
  TMS_MODBUS_SPEED,                                         //!< Скорость Modbus
  TMS_MODBUS_STOP_BIT,                                      //!< Количество Стоп-бит
  TMS_DATA_TIME,                                            //!< Текущее дата время
  TMS_READ_STATIC_PARAM,                                    //!< Команда чтения статических параметров
  TMS_PACKAGE_ERROR_TMSP,                                   //!< Количество ошибок от ТМСП
  TMS_TEMPERATURE_MOTOR,                                    //!< Температура масла двигателя
  TMS_TEMPERATURE_UNIT,                                     //!< Единицы измерения температуры
  TMS_PRESSURE_UNIT,                                        //!< Единицы измерения давления
  TMS_PSW_TMS,                                              //!< Слово состояние системы
  TMS_PSW_TMSN,                                             //!< Слово состояние наземного блока
  TMS_PSW_TMSP_1,                                           //!< Cлово состояние погружного блока
  TMS_PSW_TMSP_2,                                           //!< Слово состояние погружного блока 2
  TMS_ANGLE_INTAKE,                                         //!< Зенитный угол
  TMS_HOWMIDITY,                                            //!< Обводненность
  TMS_END,                                                  //!< 
  EM_BEGIN=20000,                                           //!< Первый параметр группы счётчиков электроэнергии
  EM_VOLTAGE_PHASE_1,                                       //!< Напряжение первой фазы
  EM_VOLTAGE_PHASE_2,                                       //!< Напряжение второй фазы
  EM_VOLTAGE_PHASE_3,                                       //!< Напряжение третей фазы
  EM_CURRENT_PHASE_1,                                       //!< Ток первой фазы
  EM_CURRENT_PHASE_2,                                       //!< Ток второй фазы
  EM_CURRENT_PHASE_3,                                       //!< Ток третей фазы
  EM_ACTIVE_POWER,                                          //!< Общая активная мощность
  EM_FREQUENCY,                                             //!< Частота питающей сети
  EM_FULL_POWER,                                            //!< Полная мощность
  EM_ACTIVE_ENERGY,                                         //!< Общая активная энергия
  EM_VOLTAGE_PHASE_1_2,                                     //!< Напряжение межфазное первой и второй фазы
  EM_VOLTAGE_PHASE_2_3,                                     //!< Напряжение межфазное второй и третьей фазы
  EM_VOLTAGE_PHASE_3_1,                                     //!< Напряжение межфазное третьей и первой фазы
  EM_COS_PHI,                                               //!< Общий коэффициент мощности
  EM_COS_PHI_PHASE_1,                                       //!< Коэффициент мощности первой фазы
  EM_COS_PHI_PHASE_2,                                       //!< Коэффициент мощности второй фазы
  EM_COS_PHI_PHASE_3,                                       //!< Коэффициент мощности третей фазы
  EM_FULL_POWER_PHASE_1,                                    //!< Полная мощность первой фазы
  EM_FULL_POWER_PHASE_2,                                    //!< Полная мощность второй фазы
  EM_FULL_POWER_PHASE_3,                                    //!< Полная мощность третей фазы
  EM_ACTIVE_POWER_PHASE_1,                                  //!< Активная мощность первой фазы
  EM_ACTIVE_POWER_PHASE_2,                                  //!< Активная мощность второй фазы
  EM_ACTIVE_POWER_PHASE_3,                                  //!< Активная мощность третей фазы
  EM_REACTIVE_POWER,                                        //!< Общая реактивная мощность
  EM_REACTIVE_POWER_PHASE_1,                                //!< Реактивная мощность первой фазы
  EM_REACTIVE_POWER_PHASE_2,                                //!< Реактивная мощность второй фазы
  EM_REACTIVE_POWER_PHASE_3,                                //!< Реактивная мощность третей фазы
  EM_ACTIVE_PLUS_ALL,                                       //!< Обшая активная плюс энергия за весь период
  EM_ACTIVE_MINUS_ALL,                                      //!< Обшая активная минус энергия за весь период
  EM_ACTIVE_PLUS_CUR_DAY,                                   //!< Активная плюс энергия за текущий день
  EM_ACTIVE_MINUS_CUR_DAY,                                  //!< Активная минус энергия за текущий день
  EM_ACTIVE_PLUS_PREV_DAY,                                  //!< Активная плюс энергия за предыдущий день
  EM_ACTIVE_MINUS_PREV_DAY,                                 //!< Активная минус энергия за текущий день
  EM_ACTIVE_PLUS_CUR_MONTH,                                 //!< Активная плюс энергия за текущий месяц
  EM_ACTIVE_MINUS_CUR_MONTH,                                //!< Активная минус энергия за текущий месяц
  EM_ACTIVE_PLUS_PREV_MONTH,                                //!< Активная плюс энергия на предыдущий месяц
  EM_ACTIVE_MINUS_PREV_MONTH,                               //!< Активная минус энергия на предыдущий месяц
  EM_REACTIVE_ENERGY,                                       //!< Общая реактивня энергия
  EM_REACTIVE_PLUS_ALL,                                     //!< Общая плюс реактивная энергия за весь период
  EM_REACTIVE_MINUS_ALL,                                    //!< Общая минус реактивная энергия за весь период
  EM_REACTIVE_PLUS_CUR_DAY,                                 //!< Плюс реактивная энергия за весь день
  EM_REACTIVE_MINUS_CUR_DAY,                                //!< Минус реактивная энергия за весь день
  EM_REACTIVE_PLUS_PREV_DAY,                                //!< Плюс реактивная энергия за весь день
  EM_REACTIVE_MINUS_PREV_DAY,                               //!< Минус реактивная энергия за весь день
  EM_REACTIVE_PLUS_CUR_MONTH,                               //!< Плюс реактивная энергия за месяц
  EM_REACTIVE_MINUS_CUR_MONTH,                              //!< Минус реактивная энергия за месяц
  EM_REACTIVE_PLUS_PREV_MONTH,                              //!< Плюс реактивная энергия за предыдущий месяц
  EM_REACTIVE_MINUS_PREV_MONTH,                             //!< Минус реактивная энергия за предыдущий месяц
  EM_COEFFICIENT_TRANS_CURRENT,                             //!< Коэффицинт трансформации тока
  EM_COEFFICIENT_TRANS_VOLTAGE,                             //!< Коэффициент трансформации напряжения
  EM_END,                                                   //!< 
};

#endif // IDSLIST

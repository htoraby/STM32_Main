#ifndef LOG_IDLIST
#define LOG_IDLIST

/*!
 * \brief Id события
*/
typedef enum {
  PowerOnId = 25002,              //!< "Включение питания"
  PowerOffId,                     //!< "Отключение питания"
  RebootSoftwareId,               //!< "Перезагрузка ПО"
  UpdateSoftwareId,               //!< "Обновление ПО"

  OverVoltInProtReactId,          //!< "Сработала защита: Превышение питания сети"
  OverVoltInApvId,                //!< "АПВ по защите: Превышение питания сети"
  OverVoltInApvDisabledId,        //!< "АПВ запрещено: Превышение питания сети"
  OverVoltInProtBlockedId,        //!< "Блокировка по защите: Превышение питания сети"

  UnderVoltInProtReactId,         //!< "Сработала защита: Снижение питания сети"
  UnderVoltInApvId,               //!< "АПВ по защите: Снижение питания сети"
  UnderVoltInApvDisabledId,       //!< "АПВ запрещено: Снижение питания сети"
  UnderVoltInProtBlockedId,       //!< "Блокировка по защите: Снижение питания сети"

  ImbalanceVoltInProtReactId,     //!< "Сработала защита: Дисбаланс напряжения"
  ImbalanceVoltInApvId,           //!< "АПВ по защите: Дисбаланс напряжения"
  ImbalanceVoltInApvDisabledId,   //!< "АПВ запрещено: Дисбаланс напряжения"
  ImbalanceVoltInProtBlockedId,   //!< "Блокировка по защите: Дисбаланс напряжения"

  OverloadMotorProtReactId,       //!< "Сработала защита: Перегруз двигателя"
  OverloadMotorApvId,             //!< "АПВ по защите: Перегруз двигателя"
  OverloadMotorApvDisabledId,     //!< "АПВ запрещено: Перегруз двигателя"
  OverloadMotorProtBlockedId,     //!< "Блокировка по защите: Перегруз двигателя"

  UnderloadMotorProtReactId,      //!< "Сработала защита: Недогруз двигателя"
  UnderloadMotorApvId,            //!< "АПВ по защите: Недогруз двигателя"
  UnderloadMotorApvDisabledId,    //!< "АПВ запрещено: Недогруз двигателя"
  UnderloadMotorProtBlockedId,    //!< "Блокировка по защите: Недогруз двигателя"

  CurrentMotorProtReactId,        //!< "Сработала защита: Предел тока двигателя"
  CurrentMotorApvId,              //!< "АПВ по защите: Предел тока двигателя"
  CurrentMotorApvDisabledId,      //!< "АПВ запрещено: Предел тока двигателя"
  CurrentMotorProtBlockedId,      //!< "Блокировка по защите: Предел тока двигателя"

  ImbalanceCurMotorProtReactId,   //!< "Сработала защита: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvId,         //!< "АПВ по защите: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvDisabledId, //!< "АПВ запрещено: Дисбаланс токов ПЭД"
  ImbalanceCurMotorProtBlockedId, //!< "Блокировка по защите: Дисбаланс токов ПЭД"

  OutOfSyncMotorProtReactId,      //!< "Сработала защита: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvId,            //!< "АПВ по защите: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvDisabledId,    //!< "АПВ запрещено: Рассинхронизация ПВЭД"
  OutOfSyncMotorProtBlockedId,    //!< "Блокировка по защите: Рассинхронизация ПВЭД"

  TurbineRotationBeginId,         //!< "Турбинное вращение. Начало"
  TurbineRotationEndId,           //!< "Турбинное вращение. Окончание"

  TemperatureMotorProtReactId,    //!< "Сработала защита: Перегрев двигателя"
  TemperatureMotorApvId,          //!< "АПВ по защите: Перегрев двигателя"
  TemperatureMotorApvDisabledId,  //!< "АПВ запрещено: Перегрев двигателя"
  TemperatureMotorProtBlockedId,  //!< "Блокировка по защите: Перегрев двигателя"

  PressureIntakeProtReactId,      //!< "Сработала защита: Давление на приёме насоса"
  PressureIntakeApvId,            //!< "АПВ по защите: Давление на приёме насоса"
  PressureIntakeApvDisabledId,    //!< "АПВ запрещено: Давление на приёме насоса"
  PressureIntakeProtBlockedId,    //!< "Блокировка по защите: Давление на приёме насоса"

  ResistIsolationProtReactId,     //!< "Сработала защита: Сопротивление изоляции"
  ResistIsolationApvId,           //!< "АПВ по защите: Сопротивление изоляции"
  ResistIsolationApvDisabledId,   //!< "АПВ запрещено: Сопротивление изоляции"
  ResistIsolationProtBlockedId,   //!< "Блокировка по защите: Сопротивление изоляции"

  LookDoorProtReactId,            //!< "Сработала защита: Блокировка двери силового отсека"
  LookDoorProtBlockedId,          //!< "Блокировка по защите: Блокировка двери"

  HardwareVsdProtReactId,         //!< "Сработала защита: Аппаратная защита ЧРП"
  HardwareVsdApvId,               //!< "АПВ по защите: Аппаратная защита ЧРП"
  HardwareVsdApvDisabledId,       //!< "АПВ запрещено: Аппаратная защита ЧРП"
  HardwareVsdProtBlockedId,       //!< "Блокировка по защите: Аппаратная защита ЧРП"

  RegimeSoftChangeFreqOffId,                //!< "Режим программного изменения частоты отключен"
  RegimeAlternationFreqOffId,               //!< "Режим чередования фаз отключен"
  RegimeRunPushOffId,                       //!< "Режим пуска толчками отключен"
  RegimeRunSwingOffId,                      //!< "Режим пуска раскачкой отключен"
  RegimeRunPickupOffId,                     //!< "Режим подхвата турбинного вращения отключен"
  RegimeRunAutoAdaptationOffId,             //!< "Режим автоадаптации отключен"

  AddDeviceDhsId,                           //!< "Подключение ТМС"
  RemoveDeviceDhsId,                        //!< "Отключение ТМС"
  AddDeviceVsdId,                           //!< "Подключение ЧРП"
  RemoveDeviceVsdId,                        //!< "Отключение ЧРП"
  AddDeviceEmId,                            //!< "Подключение СЭ"
  RemoveDeviceEmId,                         //!< "Отключение СЭ"
  AddDeviceFiltOutId,                       //!< "Подключение Выходного фильтра"
  RemoveDeviceFiltOutId,                    //!< "Отключение Выходного фильтра"

  CopyLogId,                                //!< "Копирование архивов"
  DelLogId,                                 //!< "Удаление архивов"
  DelDebugLogId,                            //!< "Удаление служебных архивов"

  WriteAlarmLogId,                          //!< "Запись аварийного архива"

  DigitalInput1LogId,                       //!< "Сигнал на цифровом входе 1"
  DigitalInput2LogId,                       //!< "Сигнал на цифровом входе 2"
  DigitalInput3LogId,                       //!< "Сигнал на цифровом входе 3"
  DigitalInput4LogId,                       //!< "Сигнал на цифровом входе 4"

  DigitalInput1ProtReactId,        //!< "Сработала защита: Цифровой вход 1"
  DigitalInput1ApvId,              //!< "АПВ по защите: Цифровой вход 1"
  DigitalInput1ApvDisabledId,      //!< "АПВ запрещено: Цифровой вход 1"
  DigitalInput1ProtBlockedId,      //!< "Блокировка по защите: Цифровой вход 1"

  DigitalInput2ProtReactId,        //!< "Сработала защита: Цифровой вход 2"
  DigitalInput2ApvId,              //!< "АПВ по защите: Цифровой вход 2"
  DigitalInput2ApvDisabledId,      //!< "АПВ запрещено: Цифровой вход 2"
  DigitalInput2ProtBlockedId,      //!< "Блокировка по защите: Цифровой вход 2"

  DigitalInput3ProtReactId,        //!< "Сработала защита: Цифровой вход 3"
  DigitalInput3ApvId,              //!< "АПВ по защите: Цифровой вход 3"
  DigitalInput3ApvDisabledId,      //!< "АПВ запрещено: Цифровой вход 3"
  DigitalInput3ProtBlockedId,      //!< "Блокировка по защите: Цифровой вход 3"

  DigitalInput4ProtReactId,        //!< "Сработала защита: Цифровой вход 4"
  DigitalInput4ApvId,              //!< "АПВ по защите: Цифровой вход 4"
  DigitalInput4ApvDisabledId,      //!< "АПВ запрещено: Цифровой вход 4"
  DigitalInput4ProtBlockedId,      //!< "Блокировка по защите: Цифровой вход 4"

  AnalogInput1ProtReactId,        //!< "Сработала защита: Аналоговый вход 1"
  AnalogInput1ApvId,              //!< "АПВ по защите: Аналоговый вход 1"
  AnalogInput1ApvDisabledId,      //!< "АПВ запрещено: Аналоговый вход 1"
  AnalogInput1ProtBlockedId,      //!< "Блокировка по защите: Аналоговый вход 1"

  AnalogInput2ProtReactId,        //!< "Сработала защита: Аналоговый вход 2"
  AnalogInput2ApvId,              //!< "АПВ по защите: Аналоговый вход 2"
  AnalogInput2ApvDisabledId,      //!< "АПВ запрещено: Аналоговый вход 2"
  AnalogInput2ProtBlockedId,      //!< "Блокировка по защите: Аналоговый вход 2"

  AnalogInput3ProtReactId,        //!< "Сработала защита: Аналоговый вход 3"
  AnalogInput3ApvId,              //!< "АПВ по защите: Аналоговый вход 3"
  AnalogInput3ApvDisabledId,      //!< "АПВ запрещено: Аналоговый вход 3"
  AnalogInput3ProtBlockedId,      //!< "Блокировка по защите: Аналоговый вход 3"

  AnalogInput4ProtReactId,        //!< "Сработала защита: Аналоговый вход 4"
  AnalogInput4ApvId,              //!< "АПВ по защите: Аналоговый вход 4"
  AnalogInput4ApvDisabledId,      //!< "АПВ запрещено: Аналоговый вход 4"
  AnalogInput4ProtBlockedId,      //!< "Блокировка по защите: Аналоговый вход 4"

  ImbalanceCurInProtReactId,      //!< "Сработала защита: Дисбаланс входных токов"
  ImbalanceCurInApvId,            //!< "АПВ по защите: Дисбаланс входных токов"
  ImbalanceCurInApvDisabledId,    //!< "АПВ запрещено: Дисбаланс входных токов"
  ImbalanceCurInProtBlockedId,    //!< "Блокировка по защите: Дисбаланс входных токов"

  PowerOffProtReactId,            //!< "Сработала защита: Нет питания сети"
  PowerOffApvId,                  //!< "АПВ по защите: Нет питания сети"
  PowerOffApvDisabledId,          //!< "АПВ запрещено: Нет питания сети"

  OvernumberOfStartProtReactId,   //!< "Сработала защита: Превышение числа пусков"
  OvernumberOfStartProtBlockedId, //!< "Блокировка по защите: Превышение числа пусков"

  ModeCodeManualId,               //!< "Режим работы: Ручной"
  ModeCodeAutoId,                 //!< "Режим работы: Автоматический"
  ModeCodeProgramId,              //!< "Режим работы: По программе"

  VibrationMotorProtReactId,      //!< "Сработала защита: Вибрация двигателя"
  VibrationMotorApvId,            //!< "АПВ по защите: Вибрация двигателя"
  VibrationMotorApvDisabledId,    //!< "АПВ запрещено: Вибрация двигателя"
  VibrationMotorProtBlockedId,    //!< "Блокировка по защите: Вибрация двигателя"

  FlowDischargeProtReactId,       //!< "Сработала защита: Минимальный расход"
  FlowDischargeApvId,             //!< "АПВ по защите: Минимальный расход"
  FlowDischargeApvDisabledId,     //!< "АПВ запрещено: Минимальный расход"
  FlowDischargeProtBlockedId,     //!< "Блокировка по защите: Минимальный расход"

  PressureDischargeProtReactId,   //!< "Сработала защита: Давление на выкиде насоса"
  PressureDischargeApvId,         //!< "АПВ по защите: Давление на выкиде насоса"
  PressureDischargeApvDisabledId, //!< "АПВ запрещено: Давление на выкиде насоса"
  PressureDischargeProtBlockedId, //!< "Блокировка по защите: Давление на выкиде насоса"

  RgmJarringF1Id,                 //!< Режим встряхивания частота F1
  RgmJarringF2Id,                 //!< Режим встряхивания частота F2

  OverheatInputFilterProtReactId, //!< "Сработала защита: Перегрев фильтра"
  OverheatInputFilterProtBlockedId,//!< "Блокировка по защите: Перегрев фильтра"

  RegimeRunAutoAdaptationStartId,     //!< Запущена автоадаптация векторного режима
  RegimeRunAutoAdaptationCompleteId,  //!< Автоадаптация к ТМПН векторного управления успешно завершена. Подобранное сопротивление - %1 Ом
  RegimeRunAutoAdaptationIncompleteId,//!< Автоадаптация к ТМПН векторного управления не завершена

  RegimeRunSwingStartId,              //!< Пуск с раскачкой
  RegimeRunSwingFinishId,             //!< Пуск с раскачкой завершен

  RegimeRunPushStartId,               //!< Пуск толчками
  RegimeRunPushFinishId,              //!< Пуск толчками завершен

  RegimeRunPickupStartId,             //!< Подхват
  RegimeRunPickupFinishId,            //!< Подхват завершен

  RegimeRunSkipResonantStartId,       //!< Пропуск F
  RegimeRunSkipResonantFinishId,      //!< Пропуск F завершен
  RegimeRunSkipResonantFailId,        //!< Пропуск F ошибка
  RegimeRunSkipResonantAbortId,       //!< Пропуск F прерван
  RegimeRunSkipResonantOffId,         //!< "Режим Пропуск F отключен"

  RegimeRunSynchronStartId,           //!< Пуск с синхронизацией
  RegimeRunSynchronFinishId,          //!< Пуск с синхронизацией завершен
  RegimeRunSynchronFailId,            //!< Пуск с синхронизацией ошибка
  RegimeRunSynchronOffId,             //!< Пуск с синхронизацией отключен

  RegimeRunPushFailId,                //!< Пуск толчками ошибка
  RegimeRunPushAbortId,               //!< Пуск толчками прерван

  RegimeRunSwingFailId,               //!< Пуск с раскачкой ошибка
  RegimeRunSwingAbortId,              //!< Пуск с раскачкой прерван

  AddDeviceFiltInputId,               //!< "Выбрано наличие входного фильтра"
  RemoveDeviceFiltInputId,            //!< "Выбрано отсутствие входного фильтра"

  RegimeRunPickupFailId,              //!< Пуск толчками ошибка
  RegimeRunPickupAbortId,             //!< Пуск толчками прерван

  DhsLinkTMSPOkId,                    //!< Восстановление связи с ТМСП
  DhsLinkTMSPFailId,                  //!< Отказ связи с ТМСП

  DhsPressureIntakeSensorOkId,        //!< Датчик давления на приёме насоса в норме
  DhsPressureIntakeSensorFailId,      //!< Отказ датчика давления на приёме насоса
  DhsTemperatureIntakeSensorOkId,     //!< Датчик температуры на приеме насоса в норме
  DhsTemperatureIntakeSensorFailId,   //!< Отказ датчика температуры на приеме насоса
  DhsTemperatureWindingSensorOkId,    //!< Датчик температуры обмотки ПЭД в норме
  DhsTemperatureWindingSensorFailId,  //!< Отказ датчика температуры обмотки ПЭД
  DhsTemperatureMotorSensorOkId,      //!< Датчик температуры масла ПЭД в норме
  DhsTemperatureMotorSensorFailId,    //!< Отказ датчика температуры масла ПЭД
  DhsAccelerationXIntakeSensorOkId,   //!< Датчик вибрации по оси X в норме
  DhsAccelerationXIntakeSensorFailId, //!< Отказ датчика вибрации по оси X
  DhsAccelerationYIntakeSensorOkId,   //!< Датчик вибрации по оси Y в норме
  DhsAccelerationYIntakeSensorFailId, //!< Отказ датчика вибрации по оси Y
  DhsAccelerationZIntakeSensorOkId,   //!< Датчик вибрации по оси Z в норме
  DhsAccelerationZIntakeSensorFailId, //!< Отказ датчика вибрации по оси Z
  DhsPressureDischargeSensorOkId,     //!< Датчик давления на выкиде насоса в НКТ в норме
  DhsPressureDischargeSensorFailId,   //!< Отказ датчика давления на выкиде насоса в НКТ
  DhsPressureDischargeZTSensorOkId,   //!< Датчик давления на выкиде насоса, затрубное в норме
  DhsPressureDischargeZTSensorFailId, //!< Отказ датчика давления на выкиде насоса, затрубное
  DhsTemperatureDischargeSensorOkId,  //!< Датчик температуры на выкиде насоса в норме
  DhsTemperatureDischargeSensorFailId,//!< Отказ датчика температуры на выкиде насоса
  DhsFlowDischargeSensorOkId,         //!< Датчик расхода жидкости на выкиде насоса в норме
  DhsFlowDischargeSensorFailId,       //!< Отказ датчика расхода жидкости на выкиде насоса
  DhsHowmidityDischargeSensorOkId,    //!< Датчик обводненности в норме
  DhsHowmidityDischargeSensorFailId,  //!< Отказ датчика обводненности

  RegimeRunDirectStartId,             //!< Прямой пуск
  RegimeRunDirectFinishId,            //!< Прямой пуск завершен
  RegimeRunDirectFailId,              //!< Прямой пуск ошибка
  RegimeRunDirectOffId,               //!< Прямой пуск отключен

  AddDeviceBypassContactorsInputId,   //!< "Выбрано наличие байпасных контакторов"
  RemoveDeviceBypassContactorsInputId,//!< "Выбрано отсутствие байпасных контакторов"
  
  SaveConfigId,                       //!< Сохранение уставок в профиль
  LoadConfigId,                       //!< Загрузка уставок из профиля
  SaveConfigUsbId,                    //!< Сохранение конфигурации на USB
  LoadConfigUsbId,                    //!< Загрузка конфигурации с USB

  CounterAllResetId,                  //!< Сброс счётчиков

} EventId;

#endif // LOG_IDLIST


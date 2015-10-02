#ifndef LOG_IDLIST
#define LOG_IDLIST

/*!
 * \brief Id события
*/
typedef enum {
  RunId = 25000,                  //!< "Пуск"...
  StopId,                         //!< "Стоп"...
  PowerOnId,                      //!< "Включение питания"
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

  HardwareVsdUdLowProtReactId,    //!< "Сработала защита: Аппаратная защита ЧРП, напряжение на шине инвертора Ud ниже минимально допустимого"
  HardwareVsdUdHighProtReactId,   //!< "Сработала защита: Аппаратная защита ЧРП, напряжение на шине инвертора Ud выше максимально допустимого"
  HardwareVsdUinAsymProtReactId,  //!< "Сработала защита: Аппаратная защита ЧРП, несимметрия входного переменного напряжения по фазам выше максимально допустимой"
  HardwareVsdShortCirProtReactId, //!< "Сработала защита: Аппаратная защита ЧРП, короткое замыкание шины инвертора"
  HardwareVsdI2tProtReactId,      //!< "Сработала защита: Аппаратная защита ЧРП, ампер-секундная защита инвертора"
  HardwareVsdAstProtReactId,      //!< "Сработала защита: Аппаратная защита ЧРП, ошибка автонастройки установки"
  HardwareVsdVcProtReactId,       //!< "Сработала защита: Аппаратная защита ЧРП, ошибка рассогласования частоты векторного режима управления"
  HardwareVsdUnderloadProtReactId,//!< "Сработала защита: Аппаратная защита ЧРП, недогруз"
  HardwareVsdResistanceProtReactId,         //!< "Сработала защита: Аппаратная защита ЧРП, низкое Rиз"
  HardwareVsdUndervoltageProtReactId,       //!< "Сработала защита: Аппаратная защита ЧРП, низкое напряжение"
  HardwareVsdOvervoltageProtReactId,        //!< "Сработала защита: Аппаратная защита ЧРП, высокое напряжение"
  HardwareVsdRunCountProtReactId,           //!< "Сработала защита: Аппаратная защита ЧРП, прев. кол-во пусков"
  HardwareVsdOverheatFilterProtReactId,     //!< "Сработала защита: Аппаратная защита ЧРП, перегрев фильтра"
  HardwareVsdInfoProtReactId,               //!< "Сработала защита: Аппаратная защита ЧРП, защита ЧРП"
  HardwareVsdSupplyDriversProtReactId,      //!< "Сработала защита: Аппаратная защита ЧРП, питание драйверов"
  HardwareVsdMonometrProtReactId,           //!< "Сработала защита: Аппаратная защита ЧРП, конт манометр"
  HardwareVsdAI0ProtReactId,                //!< "Сработала защита: Аппаратная защита ЧРП, доп. аналог вход 0"
  HardwareVsdSequencePhaseReactId,          //!< "Сработала защита: Аппаратная защита ЧРП, чередование фаз"
  HardwareVsdOverheatMotorReactId,          //!< "Сработала защита: Аппаратная защита ЧРП, выс. температура"
  HardwareVsdOvervibrationReactId,          //!< "Сработала защита: Аппаратная защита ЧРП, выс. вибрация"
  HardwareVsdPressureReactId,               //!< "Сработала защита: Аппаратная защита ЧРП, низ. давление"
  HardwareVsd19ReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, Ошибка 19"
  HardwareVsdPressureZReactId,              //!< "Сработала защита: Аппаратная защита ЧРП, низкое Pзатр"
  HardwareVsdImbalanceCurrentReactId,       //!< "Сработала защита: Аппаратная защита ЧРП, дисбаланс токов"
  HardwareVsdImbalanceVoltageReactId,       //!< "Сработала защита: Аппаратная защита ЧРП, дисбаланс напряж"
  HardwareVsdTurbineReactId,                //!< "Сработала защита: Аппаратная защита ЧРП, турбинное вращение"
  HardwareVsd24ReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, прочие ошибки"
  HardwareVsdFailureSupplyReactId,          //!< "Сработала защита: Аппаратная защита ЧРП, авария питания"
  HardwareVsdDoorReactId,                   //!< "Сработала защита: Аппаратная защита ЧРП, открыта дверь"
  HardwareVsdLostSupplyReactId,             //!< "Сработала защита: Аппаратная защита ЧРП, пропало питание"
  HardwareVsdCondensatorReactId,            //!< "Сработала защита: Аппаратная защита ЧРП, нет заряда конденс"
  HardwareVsdTeristorsReactId,              //!< "Сработала защита: Аппаратная защита ЧРП, авария тиристоров"
  HardwareVsdCurrentLimitReactId,           //!< "Сработала защита: Аппаратная защита ЧРП, токоограничение"
  HardwareVsd31ReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, ошибка номер 31"
  HardwareVsd32ReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, по подхвату"
  HardwareVsdMemoryReactId,                 //!< "Сработала защита: Аппаратная защита ЧРП, ошибка ОЗУ"
  HardwareVsd41ReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, отключен"
  HardwareVsdDIReactId,                     //!< "Сработала защита: Аппаратная защита ЧРП, отказ дискр. вх."
  HardwareVsdADCReactId,                    //!< "Сработала защита: Аппаратная защита ЧРП, отказ АЦП"
  HardwareVsdAnalogSupplyReactId,           //!< "Сработала защита: Аппаратная защита ЧРП, аналог. питание"
  HardwareVsdSensorSupplyReactId,           //!< "Сработала защита: Аппаратная защита ЧРП, питание датчиков"
  HardwareVsdEepromReactId,                 //!< "Сработала защита: Аппаратная защита ЧРП, ошибка EEPROM"
  HardwareVsdNotReadyReactId,               //!< "Сработала защита: Аппаратная защита ЧРП, ПЧ не готов"
  HardwareVsdSetPointReactId,               //!< "Сработала защита: Аппаратная защита ЧРП, сбой уставок"
  HardwareVsdBlockRunReactId,               //!< "Сработала защита: Аппаратная защита ЧРП, блокировка пуска"
  HardwareVsdIfastReactId,                  //!< "Сработала защита: Аппаратная защита ЧРП, быстрая токовая защита"
  HardwareVsdMI2TReactId,                   //!< "Сработала защита: Аппаратная защита ЧРП, перегруз двигателя"

  RegimeSoftChangeFreqOffId,                //!< "Режим программного изменения частоты отключен"
  RegimeAlternationFreqOffId,               //!< "Режим чередования фаз отключен"
  RegimeRunPushOffId,                       //!< "Режим пуска толчками отключен"
  RegimeRunSwingOffId,                      //!< "Режим пуска раскачкой отключен"
  RegimeRunPickupOffId,                     //!< "Режим подхвата турбинного вращения отключен"

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

} EventId;

#endif // LOG_IDLIST


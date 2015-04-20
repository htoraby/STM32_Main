#ifndef LOG_IDLIST
#define LOG_IDLIST

/*!
 * \brief Id события
*/
typedef enum {
  RunId = 10000,                  //!< "Пуск"...
  StopId,                         //!< "Стоп"...
  PowerOnId,                      //!< "Включение питания"
  PowerOffId,                     //!< "Отключение питания"

  OverVoltInProtReactId,          //!< "Сработала защита: Превышение питания сети"
  OverVoltInApvId,                //!< "АПВ по защите: Превышение питания сети"
  OverVoltInApvDisabledId,        //!< "АПВ запрещен: Превышение питания сети"
  OverVoltInProtBlockedId,        //!< "Блокировка по защите: Превышение питания сети"

  UnderVoltInProtReactId,         //!< "Сработала защита: Снижение питания сети"
  UnderVoltInApvId,               //!< "АПВ по защите: Снижение питания сети"
  UnderVoltInApvDisabledId,       //!< "АПВ запрещен: Снижение питания сети"
  UnderVoltInProtBlockedId,       //!< "Блокировка по защите: Снижение питания сети"

  ImbalanceVoltInProtReactId,     //!< "Сработала защита: Дисбаланс напряжения"
  ImbalanceVoltInApvId,           //!< "АПВ по защите: Дисбаланс напряжения"
  ImbalanceVoltInApvDisabledId,   //!< "АПВ запрещен: Дисбаланс напряжения"
  ImbalanceVoltInProtBlockedId,   //!< "Блокировка по защите: Дисбаланс напряжения"

  OverloadMotorProtReactId,       //!< "Сработала защита: Перегруз двигателя"
  OverloadMotorApvId,             //!< "АПВ по защите: Перегруз двигателя"
  OverloadMotorApvDisabledId,     //!< "АПВ запрещен: Перегруз двигателя"
  OverloadMotorProtBlockedId,     //!< "Блокировка по защите: Перегруз двигателя"

  UnderloadMotorProtReactId,      //!< "Сработала защита: Недогруз двигателя"
  UnderloadMotorApvId,            //!< "АПВ по защите: Недогруз двигателя"
  UnderloadMotorApvDisabledId,    //!< "АПВ запрещен: Недогруз двигателя"
  UnderloadMotorProtBlockedId,    //!< "Блокировка по защите: Недогруз двигателя"

  CurrentMotorProtReactId,        //!< "Сработала защита: Предел тока двигателя"
  CurrentMotorApvId,              //!< "АПВ по защите: Предел тока двигателя"
  CurrentMotorApvDisabledId,      //!< "АПВ запрещен: Предел тока двигателя"
  CurrentMotorProtBlockedId,      //!< "Блокировка по защите: Предел тока двигателя"

  ImbalanceCurMotorProtReactId,   //!< "Сработала защита: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvId,         //!< "АПВ по защите: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvDisabledId, //!< "АПВ запрещен: Дисбаланс токов ПЭД"
  ImbalanceCurMotorProtBlockedId, //!< "Блокировка по защите: Дисбаланс токов ПЭД"

  OutOfSyncMotorProtReactId,      //!< "Сработала защита: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvId,            //!< "АПВ по защите: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvDisabledId,    //!< "АПВ запрещен: Рассинхронизация ПВЭД"
  OutOfSyncMotorProtBlockedId,    //!< "Блокировка по защите: Рассинхронизация ПВЭД"

  TurbineRotationBeginId,         //!< "Турбинное вращение. Начало"
  TurbineRotationEndId,           //!< "Турбинное вращение. Окончание"

  TemperatureMotorProtReactId,    //!< "Сработала защита: Перегрев двигателя"
  TemperatureMotorApvId,          //!< "АПВ по защите: Перегрев двигателя"
  TemperatureMotorApvDisabledId,  //!< "АПВ запрещен: Перегрев двигателя"
  TemperatureMotorProtBlockedId,  //!< "Блокировка по защите: Перегрев двигателя"

  PressureIntakeProtReactId,      //!< "Сработала защита: Давление на приёме насоса"
  PressureIntakeApvId,            //!< "АПВ по защите: Давление на приёме насоса"
  PressureIntakeApvDisabledId,    //!< "АПВ запрещен: Давление на приёме насоса"
  PressureIntakeProtBlockedId,    //!< "Блокировка по защите: Давление на приёме насоса"

  ResistIsolationProtReactId,     //!< "Сработала защита: Сопротивление изоляции"
  ResistIsolationApvId,           //!< "АПВ по защите: Сопротивление изоляции"
  ResistIsolationApvDisabledId,   //!< "АПВ запрещен: Сопротивление изоляции"
  ResistIsolationProtBlockedId,   //!< "Блокировка по защите: Сопротивление изоляции"

  LookDoorProtReactId,            //!< "Сработала защита: Блокировка двери силового отсека"
  LookDoorProtBlockedId,          //!< "Блокировка по защите: Блокировка двери"

  HardwareVsdProtReactId,         //!< "Сработала защита: Аппаратная защита ЧРП"
  HardwareVsdApvId,               //!< "АПВ по защите: Аппаратная защита ЧРП"
  HardwareVsdApvDisabledId,       //!< "АПВ запрещен: Аппаратная защита ЧРП"
  HardwareVsdProtBlockedId,       //!< "Блокировка по защите: Аппаратная защита ЧРП"

  HardwareVsdUdLowProtReactId,    //!< "Сработала защита: Аппаратная защита ЧРП, напряжение на шине инвертора Ud ниже минимально допустимого"
  HardwareVsdUdHighProtReactId,   //!< "Сработала защита: Аппаратная защита ЧРП, напряжение на шине инвертора Ud выше максимально допустимого"
  HardwareVsdUinAsymProtReactId,  //!< "Сработала защита: Аппаратная защита ЧРП, несимметрия входного переменного напряжения по фазам выше максимально допустимой"
  HardwareVsdShortCirProtReactId, //!< "Сработала защита: Аппаратная защита ЧРП, короткое замыкание шины инвертора"
  HardwareVsdI2tProtReactId,      //!< "Сработала защита: Аппаратная защита ЧРП, ампер-секундная защита инвертора"
  HardwareVsdAstProtReactId,      //!< "Сработала защита: Аппаратная защита ЧРП, ошибка автонастройки установки"
  HardwareVsdVcProtReactId,       //!< "Сработала защита: Аппаратная защита ЧРП, ошибка рассогласования частоты векторного режима управления"

  RegimeSoftChangeFreqOffId,      //!< "Режим программного изменения частоты отключен"
  RegimeAlternationFreqOffId,     //!< "Режим чередования фаз отключен"
  RegimeRunPushOffId,             //!< "Режим пуска толчками отключен"
  RegimeRunSwingOffId,            //!< "Режим пуска раскачкой отключен"

} EventId;

#endif // LOG_IDLIST


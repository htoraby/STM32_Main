#ifndef LOG_IDLIST
#define LOG_IDLIST

/*!
 * \brief Id события
*/
typedef enum {
  RunId = 10000,                  //!< Пуск

  OverVoltInProtReactId,          //!< "Сработала защита: Превышение питания сети"
  OverVoltInApvDisabledId,        //!< "АПВ запрещен: Превышение питания сети"
  OverVoltInProtBlockedId,        //!< "Блокировка по защите: Превышение питания сети"

  UnderVoltInProtReactId,         //!< "Сработала защита: Снижение питания сети"
  UnderVoltInApvDisabledId,       //!< "АПВ запрещен: Снижение питания сети"
  UnderVoltInProtBlockedId,       //!< "Блокировка по защите: Снижение питания сети"

  ImbalanceVoltInProtReactId,     //!< "Сработала защита: Дисбаланс напряжения"
  ImbalanceVoltInApvDisabledId,   //!< "АПВ запрещен: Дисбаланс напряжения"
  ImbalanceVoltInProtBlockedId,   //!< "Блокировка по защите: Дисбаланс напряжения"

  OverloadMotorProtReactId,       //!< "Сработала защита: Перегруз двигателя"
  OverloadMotorApvDisabledId,     //!< "АПВ запрещен: Перегруз двигателя"
  OverloadMotorProtBlockedId,     //!< "Блокировка по защите: Перегруз двигателя"

  UnderloadMotorProtReactId,      //!< "Сработала защита: Недогруз двигателя"
  UnderloadMotorApvDisabledId,    //!< "АПВ запрещен: Недогруз двигателя"
  UnderloadMotorProtBlockedId,    //!< "Блокировка по защите: Недогруз двигателя"

  CurrentMotorProtReactId,        //!< "Сработала защита: Предел тока двигателя"
  CurrentMotorApvDisabledId,      //!< "АПВ запрещен: Предел тока двигателя"
  CurrentMotorProtBlockedId,      //!< "Блокировка по защите: Предел тока двигателя"

  ImbalanceCurMotorProtReactId,   //!< "Сработала защита: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvDisabledId, //!< "АПВ запрещен: Дисбаланс токов ПЭД"
  ImbalanceCurMotorProtBlockedId, //!< "Блокировка по защите: Дисбаланс токов ПЭД"

  OutOfSyncMotorProtReactId,      //!< "Сработала защита: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvDisabledId,    //!< "АПВ запрещен: Рассинхронизация ПВЭД"
  OutOfSyncMotorProtBlockedId,    //!< "Блокировка по защите: Рассинхронизация ПВЭД"

  TurbineRotationBeginId,         //!< "Турбинное вращение. Начало"
  TurbineRotationEndId,           //!< "Турбинное вращение. Окончание"

  TemperatureMotorProtReactId,    //!< "Сработала защита: Перегрев двигателя"
  TemperatureMotorApvDisabledId,  //!< "АПВ запрещен: Перегрев двигателя"
  TemperatureMotorProtBlockedId,  //!< "Блокировка по защите: Перегрев двигателя"

  PressureIntakeProtReactId,      //!< "Сработала защита: Давление на приёме насоса"
  PressureIntakeApvDisabledId,    //!< "АПВ запрещен: Давление на приёме насоса"
  PressureIntakeProtBlockedId,    //!< "Блокировка по защите: Давление на приёме насоса"

  ResistIsolationProtReactId,     //!< "Сработала защита: Сопротивление изоляции"
  ResistIsolationApvDisabledId,   //!< "АПВ запрещен: Сопротивление изоляции"
  ResistIsolationProtBlockedId,   //!< "Блокировка по защите: Сопротивление изоляции"

  LookDoorProtReactId,            //!< "Сработала защита: Блокировка двери силового отсека"
  LookDoorProtBlockedId,          //!< "Блокировка по защите: Блокировка двери"

} EventId;

#endif // LOG_IDLIST


#ifndef LOG_IDLIST
#define LOG_IDLIST

/*!
 * \brief Id события
*/
typedef enum {
  RunId = 10000,

  OverVoltInProtActivId,          //!< "Сработала защита: Превышение питания сети"
  OverVoltInApvId,                //!< "АПВ по защите: Превышение питания сети"
  OverVoltInApvDisabledId,        //!< "АПВ запрещен: Превышение питания сети"
  OverVoltInProtBlockedId,        //!< "Блокировка по защите: Превышение питания сети"

  UnderVoltInProtActivId,         //!< "Сработала защита: Снижение питания сети"
  UnderVoltInApvId,               //!< "АПВ по защите: Снижение питания сети"
  UnderVoltInApvDisabledId,       //!< "АПВ запрещен: Снижение питания сети"
  UnderVoltInProtBlockedId,       //!< "Блокировка по защите: Снижение питания сети"

  OverloadMotorProtActivId,       //!< "Сработала защита: Перегруз двигателя"
  OverloadMotorApvId,             //!< "АПВ по защите: Перегруз двигателя"
  OverloadMotorApvDisabledId,     //!< "АПВ запрещен: Перегруз двигателя"
  OverloadMotorProtBlockedId,     //!< "Блокировка по защите: Перегруз двигателя"

  UnderloadMotorProtActivId,      //!< "Сработала защита: Недогруз двигателя"
  UnderloadMotorApvId,            //!< "АПВ по защите: Недогруз двигателя"
  UnderloadMotorApvDisabledId,    //!< "АПВ запрещен: Недогруз двигателя"
  UnderloadMotorProtBlockedId,    //!< "Блокировка по защите: Недогруз двигателя"

  ImbalanceCurMotorProtActivId,   //!< "Сработала защита: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvId,         //!< "АПВ по защите: Дисбаланс токов ПЭД"
  ImbalanceCurMotorApvDisabledId, //!< "АПВ запрещен: Дисбаланс токов ПЭД"
  ImbalanceCurMotorProtBlockedId, //!< "Блокировка по защите: Дисбаланс токов ПЭД"

  OutOfSyncMotorProtActivId,      //!< "Сработала защита: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvId,            //!< "АПВ по защите: Рассинхронизация ПВЭД"
  OutOfSyncMotorApvDisabledId,    //!< "АПВ запрещен: Рассинхронизация ПВЭД"
  OutOfSyncMotorProtBlockedId,    //!< "Блокировка по защите: Рассинхронизация ПВЭД"

  TurbineRotationBeginId,         //!< "Турбинное вращение. Начало"
  TurbineRotationEndId,           //!< "Турбинное вращение. Окончание"
} EventId;

#endif // LOG_IDLIST

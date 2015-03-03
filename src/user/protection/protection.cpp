#include "protection.h"

static void protectionTask(void *p)
{
  (static_cast<Protection*>(p))->task();
}

Protection::Protection()
{

}

Protection::~Protection()
{
  osThreadTerminate(threadId_);
}

void Protection::init(const char *threadName)
{
  // Создаём задачу обработки защиты
  osThreadDef_t t = {threadName, protectionTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void Protection::getSetpointProt()
{
  mode_           = ksu.getValue(idMode_);
  reaction_       = ksu.getValue(idReaction_);
  activDelay_     = ksu.getValue(idActivDelay_);
  tripDelay_      = ksu.getValue(idTripDelay_);
  restartDelay_   = ksu.getValue(idRestartDelay_);
  restartLimit_   = ksu.getValue(idRestartLimit_);
  restartReset_   = ksu.getValue(idRestartReset_);
  tripSetpoint_   = ksu.getValue(idTripSetpoint_);
  restartSetpoint_= ksu.getValue(idRestartSetpoint_);
  param_          = ksu.getValue(idParam_);
  param2_         = ksu.getValue(idParam2_);
}

void Protection::getCurrentParamProt()
{
  state_                = ksu.getValue(idState_);
  timer_                = ksu.getValue(idTimer_);
  restartCount_         = ksu.getValue(idRestartCount_);
  restartResetCount_    = ksu.getValue(idRestartResetCount_);
}

void Protection::setCurrentParamProt()
{
  ksu.setValue(idState_, state_);
  ksu.setValue(idTimer_, timer_);
  ksu.setValue(idRestartCount_, restartCount_);
  ksu.setValue(idRestartResetCount_, restartResetCount_);
}

bool Protection::isHigherLimit(float setpoint)
{
  valueParameter_ = calcValue();
  // Если текущий параметр больше уставки
  if (valueParameter_ > setpoint)
    return true;
  else
    return false;
}

bool Protection::isLowerLimit(float setpoint)
{
  valueParameter_ = calcValue();
  // Если текущий параметр меньше уставки
  if (valueParameter_ < setpoint)
    return true;
  else
    return false;
}

void Protection::checkRestartResetCount()
{
  // Проверяем если уставка времени сброса больше чем разница между текущим
  // временем и временем первого АПВ
  if (restartReset_ >= (ksu.getTime() - restartResetCount_)) {
    // Сбрасываем количество АПВ
    restartCount_ = 0;
    // Фиксируем время сброса
    restartResetCount_ = ksu.getTime();
  }
}

void Protection::addEventActivationProt()
{
  logEvent.add(ProtectCode, AutoType, protActivatedEventId_, valueParameter_, tripSetpoint_);
}

void Protection::task()
{
  while(1) {
    osDelay(100);

    // Получаем уставки защиты
    getSetpointProt();
    // Получаем текущие параметры защиты
    getCurrentParamProt();
    // Определяем выполняется ли условие срабатывания защиты
    alarm_ = checkAlarm();
    // Определяем есть ли блокирующий параметр
    block_ = checkBlock();
    // Выполняем шаг автомата защиты
    automatProtection();
    // Сохраняем текущие параметры защиты
    setCurrentParamProt();
  }
}

void Protection::automatProtection()
{
  switch (state_) {

    // Состояние "ничегонеделанья"
    case ProtectionStateIdle:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Остаёмся в этом же состоянии
        state_ = ProtectionStateIdle;
      }
      // Станция в работе
      else {
        // Если станция в работе
        if (!ksu.isWorkCCS()) {
          // Переход на активацию
          state_ = ProtectionStateActivBegin;
        }
      }
      break;

    // Состояние начала ожидания активации
    case ProtectionStateActivBegin:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      // Станция в работе
      else {
        // Фиксируем текущее время
        timer_ = ksu.getTime();
        // Переходим в состояние ожидания активации защиты
        state_ = ProtectionStateActivWait;
      }
      break;

    // Состояние ожидания активации
    case ProtectionStateActivWait:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      // Станция в работе
      else {
        // Если задано время активации и время активации истекло
        if ((activDelay_) && (activDelay_ >= (ksu.getTime() - timer_))) {
          // Переходим в состояние активированно
          state_ =  ProtectionStateActiv;
        }
        else {
          // Остаёмся в этом состоянии
          state_ = ProtectionStateActivWait;
        }
      }
      break;

    // Состояние активации
    case ProtectionStateActiv:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      else {
        // TODO: Message Активирована защита
        state_ = ProtectionStateWork;
      }
      break;

    // Состояние работы защиты
    case ProtectionStateWork:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      else {
        // Если контролируемый параметр не в норме
        if (alarm_) {
          // Переходим в задержку срабатывания
          state_ = ProtectionStateReactionBegin;
        }
        else {
          // Продолжаем работать
          state_ = ProtectionStateWork;
        }
      }
      break;

    // Состояние начала задержки срабатывания
    case  ProtectionStateReactionBegin:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      else {
        // Если контролируемый параметр не в норме
        if (alarm_) {
          // Фиксируем текущее время
          timer_ = ksu.getTime();
          // Переходим в состояние ожидания срабатывания
          state_ = ProtectionStateReactionWait;
        }
        else {
          // Продолжаем работать
          state_ = ProtectionStateWork;
        }
      }

    // Состояние ожидания задержки срабатывания
    case  ProtectionStateReactionWait:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      // Станция в работе
      else {
        // Если контролируемый параметр не в норме
        if (alarm_) {
          // Если задано время срабатывания и время срабатывания истекло
          if ((tripDelay_) && (tripDelay_ >= (ksu.getTime() - timer_))) {
            // Переходим в состояние сработало
            state_ =  ProtectionStateReaction;
          }
          else {
            // Остаёмся в этом состоянии
            state_ = ProtectionStateReactionWait;
          }
        }
        else {
          // Продолжаем работать
          state_ = ProtectionStateWork;
        }
      }
      break;

    // Состояние срабатывания
    case  ProtectionStateReaction:
      // Если станция остановлена
      if (!ksu.isStopCCS()) {
        // Переходим в состояние "ничегонеделанья"
        state_ = ProtectionStateIdle;
      }
      else {
        // Если контролируемый параметр не в норме
        if (alarm_) {
          // TODO: Message Срабатывания защиты
          addEventActivationProt();
          state_ = ProtectionStateFailure;
        }
        else {
          state_ = ProtectionStateWork;
        }
      }
      break;

    // Состояние формирования команды останова
    case ProtectionStateFailureBegin:
      // TODO: Message Команда останов
      state_ = ProtectionStateFailureWait;
      break;

      // Состояние ожидания останова
    case  ProtectionStateFailureWait:
      // Переходим в состояние остановились, принимаем дальнейшие решения
      state_ = ProtectionStateFailure;
      break;


    // Состояние принятия решения дальнейших действий
    case  ProtectionStateFailure:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else {
        // Если действие защиты блокировка или превышено количество АПВ
        if ((reaction_ == ProtectionActionBlock) || (restartCount_ >= restartLimit_)) {
          state_ =  ProtectionStateBlock;
        }
        else
          state_ = ProtectionStateRestartBegin;
      }
      break;


    // Состояние начала отсчёта АПВ
    // В это состояние попадаем если разрешены АПВ по защите
    case  ProtectionStateRestartBegin:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else {
        // Если СУ в работе и автоматическом режиме
        if ((!ksu.isWorkCCS()) && (!ksu.isAutoControlMode())) {
          // TODO: Message Пуск
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
        else {
          // Фиксируем текущее время
          timer_ = ksu.getTime();
          state_ = ProtectionStateRestartWait;
        }
      }
      break;

    // Состояние ожидания АПВ
    // Здесь происходит отсчёт времени таймера АПВ
    case  ProtectionStateRestartWait:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else {
        // Если СУ в работе и автоматическом режиме
        if ((!ksu.isWorkCCS()) && (!ksu.isAutoControlMode())) {
          // TODO: Message Пуск
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
        else {
          // Если задано время АПВ и время АПВ истекло
          if ((restartDelay_) && (restartReset_ >= (ksu.getTime() - timer_))) {
            // Переходим в состояние проверки блокирующего параметра
            state_ =  ProtectionStateRestartBlock;
          }
          // Время АПВ не истекло, остаёмся в этом состоянии
          else {
            state_ = ProtectionStateRestartWait;
          }
        }
      }
      break;

    // Состояние проверки блокирующего параметра
    case ProtectionStateRestartBlock:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else {
        // Если СУ в работе и автоматическом режиме
        if ((!ksu.isWorkCCS()) && (!ksu.isAutoControlMode())) {
          // TODO: Message Пуск
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
        else {
          // Если есть блокирующий параметр
          if (block_) {
            // TODO: Message АПВ запрещен
            logEvent.add(ProtectCode, AutoType, apvDisabledEventId_);
            state_ = ProtectionStateRestartBlockWait;
          }
          else {
            state_ = ProtectionStateRestart;
          }
        }
      }
      break;

    // Состояние ожидания пока исчезнет блокирующий параметр
    case  ProtectionStateRestartBlockWait:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else {
        // Если СУ в работе и автоматическом режиме
        if ((!ksu.isWorkCCS()) && (!ksu.isAutoControlMode())) {
          // TODO: Message Пуск
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
        else {
          // Если есть блокирующий параметр
          if (block_) {
            // Остаёмся в этом состоянии
            state_ = ProtectionStateRestartBlockWait;
          }
          else {
            state_ = ProtectionStateRestart;
          }
        }
      }
      break;

    // Состояние АПВ
    case  ProtectionStateRestart:
      // Если СУ в блокировке или не автоматическом режиме
      if (!ksu.isBlockCCS() || ksu.isAutoControlMode()) {
        // Переходим в состояние блокировка
        state_ = ProtectionStateBlock;
      }
      else
      {
        // Если СУ в работе и автоматическом режиме
        if ((!ksu.isWorkCCS()) && (!ksu.isAutoControlMode())) {
          // TODO: Message Пуск
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
        else {
          // TODO: Message Пуск по АПВ
          logEvent.add(ProtectCode, AutoType, apvEventId_);
          // Переходим в состояние активации защиты
          state_ = ProtectionStateActivBegin;
        }
      }
      break;

    // Состояние стоп
    case  ProtectionStateStop:

      break;

    // Состояние блокировка
    case  ProtectionStateBlock:
      // TODO: Message Блокировка по защите
      logEvent.add(ProtectCode, AutoType, protBlockedEventId_);
      // Переходим в состояние "ничегонеделанья"
      state_ = ProtectionStateIdle;
      break;

    // Неизвестное состояние
    default:

      break;
  }
}


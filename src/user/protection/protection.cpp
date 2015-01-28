#include "protection.h"

static void protectionTask(void *p)
{
  (static_cast<Protection*>(p))->taskProtection();
}

// Конструктор класса защит
Protection::Protection()
{

}

Protection::~Protection()
{
  osThreadTerminate(threadId_);
}

void Protection::init(const char *threadName)
{
  // Создаём задачу цикла опроса
  // Заполняем структуру с параметрами задачи
  osThreadDef_t t = {threadName, protectionTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  // Создаём задачу
  threadId_ = osThreadCreate(&t, this);
}

// Функция получения Id параметров защиты
void Protection::getIdProtection(unsigned short mode,
                            unsigned short reaction,
                            unsigned short activDelay,
                            unsigned short tripDelay,
                            unsigned short restartDelay,
                            unsigned short restartLimit,
                            unsigned short restartReset,
                            unsigned short tripSetpoint,
                            unsigned short restartSetpoint,
                            unsigned short param,
                            unsigned short param2,
                            unsigned short state,
                            unsigned short time,
                            unsigned short restartCount,
                            unsigned short restartResetCount,
                            unsigned short valueParam)
{
  idMode_ = mode;
  idReaction_= reaction;
  idTripDelay_ = tripDelay;
  idRestartDelay_ = restartDelay;
  idRestartLimit_ = restartLimit;
  idRestartReset_ = restartReset;
  idTripSetpoint_ = tripSetpoint;
  idRestartSetpoint_ = restartSetpoint;
  idState_ = state;
  idTimer_ = time;
  idRestartCount_ = restartCount;
  idRestartResetCount_ = restartResetCount;
}

// Метод получения уставок защиты
// Вызывается в бесконечном цикле для получения
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

// Метод получения текущих параметров защиты
void Protection::getCurrentParamProt()
{
  state_                = ksu.getValue(idState_);
  timer_                = ksu.getValue(idTimer_);
  restartCount_         = ksu.getValue(idRestartCount_);
  restartResetCount_    = ksu.getValue(idRestartResetCount_);
  valueParameter_       = ksu.getValue(idValueParam_);
}

// Метод текущего значения отсительно уставки
bool Protection::checkTripSetPoint(bool limit)
{
  // Если текущий параметр должен быть выше уставки
  if (limit) {
    // Если уставка больше текущего параметра
    if (ksu.getValue(idTripSetpoint_) >= valueParameter_)
      return 1;
    else
      return 0;
  }
  // Если текущий параметр должен быть ниже уставки
  else
  {
    // Если уставка больше текущего параметра
    if (ksu.getValue(idTripSetpoint_) >= valueParameter_)
      return 0;
    else
      return 1;
  }
}

// Метод проверки текущего значения отсительно уставки
bool Protection::checkRestartSetPoint(bool limit)
{
  // Если текущий параметр должен быть выше уставки
  if (limit) {
    // Если уставка больше текущего параметра
    if (restartSetpoint_ >= valueParameter_)
      return 1;
    else
      return 0;
  }
  // Если текущий параметр должен быть ниже уставки
  else
  {
    // Если уставка больше текущего параметра
    if (restartSetpoint_ >= valueParameter_)
      return 0;
    else
      return 1;
  }
}

// Метод проверки истёк ли таймер сброса счётчиков АПВ защиты
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

// Метод задача защиты
void Protection::taskProtection()
{
  while(1) {
    // Получаем уставки защиты
    getSetpointProt();
    // Получаем текущие параметры защиты
    getCurrentParamProt();
    // Определяем выполняется ли условие срабатывания защиты
    //trip_ = checkTripSetPoint(limit);
    // Определяем есть ли блокирующий параметр
    //block_ = checkRestartSetPoint(limit);
    // Выполняем шаг автомата защиты
    automatProtection();
    // Сохраняем текущие параметры защиты
    //setCurrentParamProt();
  }
}

// Универсальный автомат работы защиты
void Protection::automatProtection()
{
  // Автомат работы защиты
  switch((unsigned char)state_){

  // Состояние "ничегонеделанья"
  case PROTECTION_STATE_OFF:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Остаёмся в этом же состоянии
      state_ = PROTECTION_STATE_OFF;
    }
    // Станция в работе
    else {
      // Если станция в работе
      if (!ksu.checkWorkCCS()) {
        // Переход на активацию
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
    }
    break;

  // Состояние начала ожидания активации
  case PROTECTION_STATE_ACTIV_BEGIN:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    // Станция в работе
    else {
      // Фиксируем текущее время
      timer_ = ksu.getTime();
      // Переходим на состояние ожидания активации защиты
      state_ = PROTECTION_STATE_ACTIV_WAIT;
    }
    break;

  // Состояние ожидания активации
  case PROTECTION_STATE_ACTIV_WAIT:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    // Станция в работе
    else {
      // Если задано время активации и время активации истекло
      if ((activDelay_) && (activDelay_ >= (ksu.getTime() - timer_))) {
        // Переходим на состояние активированно
        state_ =  PROTECTION_STATE_ACTIV;
      }
      else {
        // Остаёмся в этом состоянии
        state_ = PROTECTION_STATE_ACTIV_WAIT;
      }
    }
    break;

  // Состояние активации
  case PROTECTION_STATE_ACTIV:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    else {
      // TODO: Message Активирована защита
      state_ = PROTECTION_STATE_WORK;
    }
    break;

  // Состояние работы защиты
  case PROTECTION_STATE_WORK:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    else {
      // Если контролируемый параметр не в норме
      if (trip_) {
        // Переходим в задержку срабатывания
        state_ = PROTECTION_STATE_REACTION_BEGIN;
      }
      else {
        // Продолжаем работать
        state_ = PROTECTION_STATE_WORK;
      }
    }
    break;

  // Состояние начала задержки срабатывания
  case  PROTECTION_STATE_REACTION_BEGIN:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    else {
      // Если контролируемый параметр не в норме
      if (trip_) {
        // Фиксируем текущее время
        timer_ = ksu.getTime();
        // Переходим на состояние ожидания срабатывания
        state_ = PROTECTION_STATE_REACTION_WAIT;
      }
      else {
        // Продолжаем работать
        state_ = PROTECTION_STATE_WORK;
      }
    }

  // Состояние ожидания задержки срабатывания
  case  PROTECTION_STATE_REACTION_WAIT:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    // Станция в работе
    else {
      // Если контролируемый параметр не в норме
      if (trip_) {
        // Если задано время срабатывания и время срабатывания истекло
        if ((tripDelay_) && (tripDelay_ >= (ksu.getTime() - timer_))) {
          // Переходим на состояние сработало
          state_ =  PROTECTION_STATE_REACTION;
        }
        else {
          // Остаёмся в этом состоянии
          state_ = PROTECTION_STATE_REACTION_WAIT;
        }
      }
      else {
        // Продолжаем работать
        state_ = PROTECTION_STATE_WORK;
      }
    }
    break;

  // Состояние срабатывания
  case  PROTECTION_STATE_REACTION:
    // Если станция остановлена
    if (!ksu.checkStopCCS()) {
      // Переходим на состояние "ничегонеделанья"
      state_ = PROTECTION_STATE_OFF;
    }
    else {
      // Если контролируемый параметр не в норме
      if (trip_) {
        // TODO: Message Срабатывания защиты
        state_ = PROTECTION_STATE_FAILURE;
      }
      else {
        state_ = PROTECTION_STATE_WORK;
      }
    }
    break;

  // Состояние формирования команды останова
  case PROTECTION_STATE_FAILURE_BEGIN:
    // TODO: Message Команда останов
    state_ = PROTECTION_STATE_FAILURE_WAIT;
    break;

  // Состояние ожидания останова
  case  PROTECTION_STATE_FAILURE_WAIT:
    // Переходим на состояние остановились, принимаем дальнейшие решения
    state_ = PROTECTION_STATE_FAILURE;
    break;


  // Состояние принятия решения дальнейших действий
  case  PROTECTION_STATE_FAILURE:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
      // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else {
      // Если действие защиты блокировка или превышено количество АПВ
      if ((reaction_ == PROTECTION_ACTION_BLOCK) || (restartCount_ >= restartLimit_)) {
        state_ =  PROTECTION_STATE_BLOCK;
      }
      else
        state_ = PROTECTION_STATE_RESTART_BEGIN;
    }
    break;


  // Состояние начала отсчёта АПВ
  // В это состояние попадаем если разрешены АПВ по защите
  case  PROTECTION_STATE_RESTART_BEGIN:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
      // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.checkWorkCCS()) && (!ksu.checkAutoControlMode())) {
        // TODO: Message Пуск
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
      else {
        // Фиксируем текущее время
        timer_ = ksu.getTime();
        state_ = PROTECTION_STATE_RESTART_WAIT;
      }
    }
    break;

  // Состояние ожидания АПВ
  // Здесь происходит отсчёт времени таймера АПВ
  case  PROTECTION_STATE_RESTART_WAIT:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
      // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.checkWorkCCS()) && (!ksu.checkAutoControlMode())) {
        // TODO: Message Пуск
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
      else {
        // Если задано время АПВ и время АПВ истекло
        if ((restartDelay_) && (restartReset_ >= (ksu.getTime() - timer_))) {
          // Переходим на состояние проверки блокирующего параметра
          state_ =  PROTECTION_STATE_RESTART_BLOCK;
        }
        // Время АПВ не истекло, остаёмся в этом состоянии
        else {
          state_ = PROTECTION_STATE_RESTART_WAIT;
        }
      }
    }
    break;

  // Состояние проверки блокирующего параметра
  case PROTECTION_STATE_RESTART_BLOCK:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
      // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.checkWorkCCS()) && (!ksu.checkAutoControlMode())) {
        // TODO: Message Пуск
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
      else {
        // Если есть блокирующий параметр
        if (block_) {
          // TODO: Message АПВ запрещен
          state_ = PROTECTION_STATE_RESTART_BLOCK_WAIT;
        }
        else {
          state_ = PROTECTION_STATE_RESTART;
        }
      }
    }
    break;

  // Состояние ожидания пока исчезнет блокирующий параметр
  case  PROTECTION_STATE_RESTART_BLOCK_WAIT:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
    // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.checkWorkCCS()) && (!ksu.checkAutoControlMode())) {
        // TODO: Message Пуск
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
      else {
        // Если есть блокирующий параметр
        if (block_) {
          // Остаёмся в этом состоянии
          state_ = PROTECTION_STATE_RESTART_BLOCK_WAIT;
        }
        else {
          state_ = PROTECTION_STATE_RESTART;
        }
      }
    }
    break;

  // Состояние АПВ
  case  PROTECTION_STATE_RESTART:
    // Если СУ в блокировке или не автоматическом режиме
    if (!ksu.checkBlockCCS() || ksu.checkAutoControlMode()) {
      // Переходим в состояние блокировка
      state_ = PROTECTION_STATE_BLOCK;
    }
    else
    {
      // Если СУ в работе и автоматическом режиме
      if ((!ksu.checkWorkCCS()) && (!ksu.checkAutoControlMode())) {
        // TODO: Message Пуск
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
      else {
        // TODO: Message Пуск по АПВ
        // Переходим на состояние активации защиты
        state_ = PROTECTION_STATE_ACTIV_BEGIN;
      }
    }
    break;

  // Состояние стоп
  case  PROTECTION_STATE_STOP:

    break;

  // Состояние блокировка
  case  PROTECTION_STATE_BLOCK:
    // TODO: Message Блокировка защита
    // Переходим на состояние "ничегонеделанья"
    state_ = PROTECTION_STATE_OFF;
    break;

  // Неизвестное состояние
  default:

    break;
  }
}


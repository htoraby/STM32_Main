#include "novobus_slave.h"
#include "user_main.h"
#include "string.h"
#include "update.h"

#define MAX_QUEUE_SIZE 500
#define ANSWER_TIMEOUT 300000

static void novobusSlaveTask(void *p)
{
  (static_cast<NovobusSlave*>(p))->task();
}

NovobusSlave::NovobusSlave()
  : oldCommand_(NoneCommand)
  , idsCount_(0)
  , addrsCount_(0)
  , isConnect_(true)
{

}

NovobusSlave::~NovobusSlave()
{

}

void NovobusSlave::init()
{
  hostInit();

  // Создаём очередь сообщений событий
  osMessageQDef(MessageEventsNovobus, MAX_QUEUE_SIZE, uint32_t);
  messageEvents_ = osMessageCreate(osMessageQ(MessageEventsNovobus), NULL);

  // Создаём очередь сообщений параметров для записи
  osMessageQDef(MessageParamsNovobus, MAX_QUEUE_SIZE, uint32_t);
  messageParams_ = osMessageCreate(osMessageQ(MessageParamsNovobus), NULL);

  // Создаём задачу обработки принятых пакетов
  osThreadDef_t t = {"NovobusSlave", novobusSlaveTask, osPriorityNormal, 0, 3 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

bool NovobusSlave::isConnect()
{
  return isConnect_;
}

void NovobusSlave::task()
{
  osSemaphoreId semaphoreId = getHostSemaphore();
  hostEnable();

  while(1) {
    // Проверка семафора - если он свободен, то получен покет от хоста
    if (osSemaphoreWait(semaphoreId, ANSWER_TIMEOUT) != osEventTimeout) {
      rxSize = hostReadData(rxBuffer_);
      receivePackage(rxSize);

      osDelay(1);
      hostWriteData(txBuffer_, txBuffer_[1]);
      isConnect_ = true;
    }
    // Нет связи
    else {
      isConnect_ = false;
    }
  }
}

int NovobusSlave::getMessageEvents()
{
  osEvent Event;
  Event = osMessageGet(messageEvents_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return -1;
}

uint32_t NovobusSlave::getMessageParams()
{
  osEvent Event;
  Event = osMessageGet(messageParams_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

void NovobusSlave::putMessageEvents(uint32_t addr)
{
  osMessagePut(messageEvents_, addr, 0);
}

void NovobusSlave::putMessageParams(uint16_t id, uint16_t type)
{
  uint32_t message = (type << 16) + id;
  osMessagePut(messageParams_, message, 0);
}

void NovobusSlave::receivePackage(uint8_t sizePkt)
{
  // Заполняем заголовок пакета
  txBuffer_[0] = rxBuffer_[0];
  txBuffer_[2] = NoneError;

  // Команда
  uint8_t command = rxBuffer_[0];
  uint8_t sizePktT = rxBuffer_[1];
  uint16_t calcCrc = 0;
  // Количество данных
  uint8_t dataNumber;
  // Временная переменная преобразования типов
  unTypeData id;
  unTypeData value;
  EventType eventType;

  // Проверка контрольной суммы
  if (sizePkt && (sizePkt == sizePktT)) {
    // Получаем контрольную сумму
    uint16_t rxCrc = (rxBuffer_[sizePktT - 2] << 8) + rxBuffer_[sizePktT - 1];
    // Вычисляем контрольную сумму
    calcCrc = crc16_ibm(rxBuffer_, sizePktT - 2);

    if (rxCrc == calcCrc) {
      statHost.rxGood++;
      // Анализ команды
      switch (command) {
      // Команда запроса данных от Master к Slave есть ли у него готовые данные
      case StatusCommand:
        checkMessage();

        sizePkt = 7;
        break;

        // Команда чтения ID новых событий
      case NewEventsCommand:
        if (!addrsCount_) {
          if (osMessageNumber(messageEvents_)) {
            while (1) {
              int addr = getMessageEvents();
              if (addr >= 0) {
                addrsBuffer_[addrsCount_++] = addr;
                if (addrsCount_ >= MAX_ADDRS_BUFFER)
                  break;
              }
              else {
                break;
              }
            }
          }
        }

        if (addrsCount_) {
          for (int i = 0; i < addrsCount_; ++i) {
            id.uint32_t = addrsBuffer_[i];
            txBuffer_[5 + i*4] = id.char_t[3];
            txBuffer_[6 + i*4] = id.char_t[2];
            txBuffer_[7 + i*4] = id.char_t[1];
            txBuffer_[8 + i*4] = id.char_t[0];
          }
        }

        checkMessage();

        sizePkt = 7 + addrsCount_*4;
        break;

        // Команда чтения событий
      case ReadEventsCommand:
        // После получения мастером списка адресов событий - обнуляем счётчик
        addrsCount_ = 0;

        // Количество событий = (размер пакета - заголовок - crc) / (размер адреса)
        dataNumber = (sizePkt - 4)/4;
        // Цикл по количеству считываемых событий
        for (int i = 0; i < dataNumber; i++) {
          // Получаем адрес события
          id.char_t[3] = rxBuffer_[2 + i*4];
          id.char_t[2] = rxBuffer_[3 + i*4];
          id.char_t[1] = rxBuffer_[4 + i*4];
          id.char_t[0] = rxBuffer_[5 + i*4];
          int addr = id.uint32_t;
          // Считываем событие
          logRead(addr, &txBuffer_[5 + i*LOG_EVENT_SIZE], LOG_EVENT_SIZE);
        }

        checkMessage();

        sizePkt = 7 + dataNumber*LOG_EVENT_SIZE;
        break;

        // Команда чтения ID обновлённых параметров
      case UpdateParamsCommand:
        if (!idsCount_) {
          if (osMessageNumber(messageParams_)) {
            while (1) {
              id.uint32_t = getMessageParams();
              if (id.uint32_t) {
                idsBuffer_[idsCount_++] = id.uint32_t;
                if (idsCount_ >= MAX_IDS_BUFFER)
                  break;
              }
              else {
                break;
              }
            }
          }
        }

        if (idsCount_) {
          for (int i = 0; i < idsCount_; ++i) {
            txBuffer_[5 + i*3] = idsBuffer_[i] >> 16;
            txBuffer_[6 + i*3] = idsBuffer_[i] >> 8;
            txBuffer_[7 + i*3] = idsBuffer_[i];
          }
        }

        checkMessage();

        sizePkt = 7 + idsCount_*3;
        break;

        // Команда чтения параметров
      case ReadParamsCommand:
        // После получения мастером списка ID параметров - обнуляем счётчик
        idsCount_ = 0;

        // Количество параметров = (размер пакета - заголовок - crc) /
        // (размер id)
        dataNumber = (sizePkt - 4)/2;
        // Цикл по количеству считываемых параметров
        for (int i = 0; i < dataNumber; i++) {
          // Получаем ID параметра
          id.uint16_t[0] = (rxBuffer_[2 + i*2] << 8) + rxBuffer_[3 + i*2];
          // Получить значение параметра
          if (parameters.getPhysic(id.uint16_t[0]) == PHYSIC_DATE_TIME)
            value.uint32_t = parameters.getU32(id.uint16_t[0]);
          else
            value.float_t = parameters.get(id.uint16_t[0]);
          txBuffer_[5 + i*6]  = id.char_t[1];
          txBuffer_[6 + i*6]  = id.char_t[0];
          txBuffer_[7 + i*6]  = value.char_t[3];
          txBuffer_[8 + i*6]  = value.char_t[2];
          txBuffer_[9 + i*6]  = value.char_t[1];
          txBuffer_[10 + i*6] = value.char_t[0];
        }

        checkMessage();

        sizePkt = 7 + dataNumber*6;
        break;

        // Команда записи параметров
      case WriteParamsCommand:
        // Количество параметров = (размер пакета - заголовок - crc) /
        // (размер id и значения параметра)
        dataNumber = (sizePkt - 4)/7;
        for (int i = 0; i < dataNumber; i++) {
          // Получаем ID параметра
          id.uint16_t[0] = (rxBuffer_[2 + i*7] << 8) + rxBuffer_[3 + i*7];
          // Получаем значение параметра
          value.char_t[3] = rxBuffer_[4 + i*7];
          value.char_t[2] = rxBuffer_[5 + i*7];
          value.char_t[1] = rxBuffer_[6 + i*7];
          value.char_t[0] = rxBuffer_[7 + i*7];
          eventType = (EventType)rxBuffer_[8 + i*7];
          // Вызываем функцию записи значения параметра
          if (parameters.getPhysic(id.uint16_t[0]) == PHYSIC_DATE_TIME)
            parameters.set(id.uint16_t[0], (uint32_t)value.uint32_t, eventType);
          else
            parameters.set(id.uint16_t[0], value.float_t, eventType);
        }

        checkMessage();

        sizePkt = 7;
        break;

        // Команда чтения минимума параметра
      case ReadMinCommand:
        // После получения мастером списка ID параметров - обнуляем счётчик
        idsCount_ = 0;

        // Количество параметров = (размер пакета - заголовок - crc) /
        // (размер id)
        dataNumber = (sizePkt - 4)/2;
        // Цикл по количеству считываемых параметров
        for (int i = 0; i < dataNumber; i++) {
          // Получаем ID параметра
          id.uint16_t[0] = (rxBuffer_[2 + i*2] << 8) + rxBuffer_[3 + i*2];
          // Получить значение параметра
          value.float_t = parameters.getMin(id.uint16_t[0]);
          txBuffer_[5 + i*6]  = id.char_t[1];
          txBuffer_[6 + i*6]  = id.char_t[0];
          txBuffer_[7 + i*6]  = value.char_t[3];
          txBuffer_[8 + i*6]  = value.char_t[2];
          txBuffer_[9 + i*6]  = value.char_t[1];
          txBuffer_[10 + i*6] = value.char_t[0];
        }

        checkMessage();

        sizePkt = 7 + dataNumber*6;
        break;

        // Команда чтения маума параметра
      case ReadMaxCommand:
        // После получения мастером списка ID параметров - обнуляем счётчик
        idsCount_ = 0;

        // Количество параметров = (размер пакета - заголовок - crc) /
        // (размер id)
        dataNumber = (sizePkt - 4)/2;
        // Цикл по количеству считываемых параметров
        for (int i = 0; i < dataNumber; i++) {
          // Получаем ID параметра
          id.uint16_t[0] = (rxBuffer_[2 + i*2] << 8) + rxBuffer_[3 + i*2];
          value.float_t = parameters.getMax(id.uint16_t[0]);
          txBuffer_[5 + i*6]  = id.char_t[1];
          txBuffer_[6 + i*6]  = id.char_t[0];
          txBuffer_[7 + i*6]  = value.char_t[3];
          txBuffer_[8 + i*6]  = value.char_t[2];
          txBuffer_[9 + i*6]  = value.char_t[1];
          txBuffer_[10 + i*6] = value.char_t[0];
        }

        checkMessage();

        sizePkt = 7 + dataNumber*6;
        break;

        // Команда чтения прошивки обновления
      case ReadSoftwareCommand:
        id.char_t[3] = rxBuffer_[2];
        id.char_t[2] = rxBuffer_[3];
        id.char_t[1] = rxBuffer_[4];
        id.char_t[0] = rxBuffer_[5];
        value.char_t[1] = rxBuffer_[6];
        value.char_t[0] = rxBuffer_[7];
        readSwFromFlashExt(id.uint32_t, &txBuffer_[11], value.uint32_t);

        txBuffer_[5] = rxBuffer_[2];
        txBuffer_[6] = rxBuffer_[3];
        txBuffer_[7] = rxBuffer_[4];
        txBuffer_[8] = rxBuffer_[5];
        txBuffer_[9] = rxBuffer_[6];
        txBuffer_[10] = rxBuffer_[7];

        checkMessage();

        sizePkt = 7 + 6 + value.uint32_t;
        break;
        // Команда записи прошивки обновления
      case WriteSoftwareCommand:
        id.char_t[3] = rxBuffer_[2];
        id.char_t[2] = rxBuffer_[3];
        id.char_t[1] = rxBuffer_[4];
        id.char_t[0] = rxBuffer_[5];
        value.char_t[1] = rxBuffer_[6];
        value.char_t[0] = rxBuffer_[7];
        writeSwInFlashExt(id.uint32_t, &rxBuffer_[8], value.uint32_t);

        checkMessage();

        sizePkt = 7;
        break;

      default:
        txBuffer_[2] = InvalidCmdError;
        txBuffer_[3] = NoneCommand;
        txBuffer_[4] = 0;
        sizePkt = 7;
        break;
      }
    }
    // Ошибка CRC
    else {
      statHost.crcError++;
      txBuffer_[2] = CrcError;
      txBuffer_[3] = NoneCommand;
      txBuffer_[4] = 0;
      sizePkt = 7;
    }
  }
  // Неверный размер пакета
  else {
    statHost.sizeError++;
    txBuffer_[2] = CrcError;
    txBuffer_[3] = NoneCommand;
    txBuffer_[4] = 0;
    sizePkt = 7;
  }

  txBuffer_[1] = sizePkt;
  // Вычисляем и добавляем контрольную сумму
  calcCrc = crc16_ibm(txBuffer_, txBuffer_[1] - 2);
  txBuffer_[sizePkt-2] = calcCrc >> 8;
  txBuffer_[sizePkt-1] = calcCrc;
}

void NovobusSlave::checkMessage()
{
  txBuffer_[3] = NoneCommand;
  txBuffer_[4] = 0;

  uint8_t numberEvents = osMessageNumber(messageEvents_);
  uint8_t numberParams = osMessageNumber(messageParams_);

  if (numberEvents && !((oldCommand_ == NewEventsCommand) && numberParams)) {
    txBuffer_[3] = NewEventsCommand;
    txBuffer_[4] = numberEvents;
  } else if (numberParams) {
    txBuffer_[3] = UpdateParamsCommand;
    txBuffer_[4] = numberParams;
  }

  oldCommand_ = txBuffer_[3];
}

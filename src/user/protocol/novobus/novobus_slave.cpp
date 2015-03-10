#include "novobus_slave.h"
#include "user_main.h"
#include "string.h"

#define MAX_QUEUE_SIZE 500

static void novobusSlaveTask(void *p)
{
  (static_cast<NovobusSlave*>(p))->task();
}

NovobusSlave::NovobusSlave()
  : idsCount_(0)
  , addrsCount_(0)
{

}

NovobusSlave::~NovobusSlave()
{

}

void NovobusSlave::init()
{
  hostInit();

  // Создаём очередь сообщений событий (надписей на дисплее)
  osMessageQDef(MessageEventsNovobus, MAX_QUEUE_SIZE, uint32_t);
  messageEvents_ = osMessageCreate(osMessageQ(MessageEventsNovobus), NULL);

  // Создаём очередь сообщений параметров для записи
  osMessageQDef(MessageParamsNovobus, MAX_QUEUE_SIZE, uint32_t);
  messageParams_ = osMessageCreate(osMessageQ(MessageParamsNovobus), NULL);

  // Создаём задачу Novobus slave
  osThreadDef_t t = {"NovobusSlave", novobusSlaveTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void NovobusSlave::task()
{
  osSemaphoreId semaphoreId = getHostSemaphore();

  while(1) {
    // Проверка семафора - если он свободен, то получен покет от хоста
    if (osSemaphoreWait(semaphoreId, osWaitForever) != osEventTimeout) {
      rxSize = hostReadData(rxBuffer_);
      if (rxSize) {
        receivePackage();

        osDelay(1);
        hostWriteData(txBuffer_, txBuffer_[1]);
      }
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

int NovobusSlave::getMessageParams()
{
  osEvent Event;
  Event = osMessageGet(messageParams_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

void NovobusSlave::putMessageEvents(uint32_t id)
{
//  TODO: Необходима реализация чтения событий
//  osMessagePut(messageEvents_, id, 0);
}

void NovobusSlave::putMessageParams(uint32_t id)
{
  osMessagePut(messageParams_, id, 0);
}

// Команда анализа полученного запроса
void NovobusSlave::receivePackage()
{
  // Заполняем заголовок пакета
  txBuffer_[0] = rxBuffer_[0];
  txBuffer_[2] = NoneError;

  // Команда
  uint8_t command = rxBuffer_[0];
  // Количество байт в запросе
  uint8_t sizePkt = rxBuffer_[1];
  // Количество данных
  uint8_t dataNumber;
  // Временная переменная преобразования типов
  unTypeData id;
  unTypeData value;

  // Получаем контрольную сумму
  uint16_t rxCrc = (rxBuffer_[sizePkt - 2] << 8) + rxBuffer_[sizePkt - 1];
  // Вычисляем контрольную сумму
  uint16_t calcCrc = crc16_ibm(rxBuffer_, sizePkt - 2);

  // Проверка контрольной суммы
  if (rxCrc == calcCrc) {
    // Анализ команды
    switch (command) {
      // Команда запроса данных от Master к Slave есть ли у него готовые данные
      case StatusCommand:
        checkMessage();

        sizePkt = 7;
        break;

        // Команда чтения ID новых событий
      case UpdateEventsCommand:
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
            id.tdUint32 = addrsBuffer_[i];
            txBuffer_[5 + i*4] = id.tdChar[3];
            txBuffer_[6 + i*4] = id.tdChar[2];
            txBuffer_[7 + i*4] = id.tdChar[1];
            txBuffer_[8 + i*4] = id.tdChar[0];
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
          id.tdChar[3] = rxBuffer_[2 + i*4];
          id.tdChar[2] = rxBuffer_[3 + i*4];
          id.tdChar[1] = rxBuffer_[4 + i*4];
          id.tdChar[0] = rxBuffer_[5 + i*4];
          int addr = id.tdUint32;
          // Считываем событие
          logRead(FlashSpi5, addr, &txBuffer_[5 + i*21], 21);
        }

        checkMessage();

        sizePkt = 7 + dataNumber*21;
        break;

        // Команда чтения ID обновлённых параметров
      case UpdateParamsCommand:
        if (!idsCount_) {
          if (osMessageNumber(messageParams_)) {
            while (1) {
              id.tdUint16[0] = getMessageParams();
              if (id.tdUint16[0]) {
                idsBuffer_[idsCount_++] = id.tdUint16[0];
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
            txBuffer_[5 + i*2] = idsBuffer_[i] >> 8;
            txBuffer_[6 + i*2] = idsBuffer_[i];
          }
        }

        checkMessage();

        sizePkt = 7 + idsCount_*2;
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
          id.tdUint16[0] = (rxBuffer_[2 + i*2] << 8) + rxBuffer_[3 + i*2];
          // Получить значение параметра
          value.tdFloat = parameters.getValue(id.tdUint16[0]);
          txBuffer_[5 + i*6]  = id.tdChar[1];
          txBuffer_[6 + i*6]  = id.tdChar[0];
          txBuffer_[7 + i*6]  = value.tdChar[3];
          txBuffer_[8 + i*6]  = value.tdChar[2];
          txBuffer_[9 + i*6]  = value.tdChar[1];
          txBuffer_[10 + i*6] = value.tdChar[0];
        }

        checkMessage();

        sizePkt = 7 + dataNumber*6;
        break;

        // Команда записи параметров
      case WriteParamsCommand:
        // Количество параметров = (размер пакета - заголовок - crc) /
        // (размер id и значения параметра)
        dataNumber = (sizePkt - 4)/6;
        for (int i = 0; i < dataNumber; i++) {
          // Получаем ID параметра
          id.tdUint16[0] = (rxBuffer_[2 + i*6] << 8) + rxBuffer_[3 + i*6];
          // Получаем значение параметра
          value.tdChar[3] = rxBuffer_[4 + i*6];
          value.tdChar[2] = rxBuffer_[5 + i*6];
          value.tdChar[1] = rxBuffer_[6 + i*6];
          value.tdChar[0] = rxBuffer_[7 + i*6];
          // Вызываем функцию записи значения параметра
          parameters.setValue(id.tdUint16[0], value.tdFloat);
        }

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
  else {
    // Ошибка CRC
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

  // Проверка очереди событий
  uint8_t dataNumber = osMessageNumber(messageEvents_);
  if (dataNumber) {
    txBuffer_[3] = UpdateEventsCommand;
  }
  // Проверка очереди параметров
  else {
    dataNumber = osMessageNumber(messageParams_);
    if (dataNumber)
      txBuffer_[3] = UpdateParamsCommand;
  }
  txBuffer_[4] = dataNumber;
}

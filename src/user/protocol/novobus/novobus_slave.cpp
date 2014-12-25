#include "novobus_slave.h"
#include "host.h"

static void novobusSlaveTask(void *p)
{
  (static_cast<NovobusSlave*>(p))->exchangeCycle();
}

NovobusSlave::NovobusSlave()
{
  // Создаём задачу Novobus slave
  // Заполняем структуру с параметрами задачи
  osThreadDef_t t = {"novobusSlave", novobusSlaveTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  // Создаём задачу
  threadId_ = osThreadCreate(&t, this);

  // Создаём очередь сообщений событий (надписей на дисплее)
  osMessageQDef(MessageEventSPI, 100, uint32_t);
  messageEventSPI_ = osMessageCreate (osMessageQ(MessageEventSPI), NULL);

  // Создаём очередь сообщений параметров для записи
  osMessageQDef(MessageParamSPI, 100, uint32_t);
  messageParamSPI_ = osMessageCreate (osMessageQ(MessageParamSPI), NULL);
}

NovobusSlave::~NovobusSlave()
{

}

void NovobusSlave::exchangeCycle(void)
{
  osSemaphoreId semaphoreId = hostSemaphoreCreate();
  // Бесконечный цикл
  while(1) {
    // Проверить семафор, если он свободен
    if (osSemaphoreWait(semaphoreId, osWaitForever) != osEventTimeout) {
      if (hostReadData(rxBuffer_)) {
        reseivePackage();
        hostWriteData(txBuffer_, txBuffer_[1]);
      }
    }
  }

}

// Получить элемент очереди внеочередной
int NovobusSlave::getMessageEventSPI()
{
  osEvent Event;
  Event = osMessageGet(messageEventSPI_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

// Получить элемент очереди внеочередной
int NovobusSlave::getMessageParamSPI()
{
  osEvent Event;
  Event = osMessageGet(messageParamSPI_, 0);
  if (Event.status == osEventMessage)
    return Event.value.v;
  return 0;
}

// Команда анализа полученного запроса
void NovobusSlave::reseivePackage()
{
  // Команда
  char command = rxBuffer_[0];
  // Количество байт в запросе 2 байт и 1 элемент массива
  uint8_t count = rxBuffer_[1];
  // Количество параметров считываемых в запросе
  char countParam;
  // Временная переменная преобразования типов
  DataType ID;
  DataType Value;
  int i = 0;
  // Получаем контрольную сумму
  uint16_t rxCrc = (rxBuffer_[count - 2] << 8) + rxBuffer_[count - 1];
  // Вычисляем контрольную сумму
  uint16_t calcCrc = crc16_ibm(rxBuffer_, rxBuffer_[1] - 2);
  // Если контрольная сумма сошлась
  if (rxCrc == calcCrc) {
    // Анализ команды
    switch (rxBuffer_[0]) {
    // Команда запроса данных от Master к Slave есть ли у него готовые данные
    case NOVOBUS_COMMAND_DIAGNOSTIC:
      // Сначала проверяем очередь событий, если есть события сначала отправляем
      // максимум событий которые можем отправить
      while (1) {
        ID.DtInt16[0] = getMessageEventSPI();
        if (ID.DtInt16[0]) {
          txBuffer_[2*i + 2] = ID.DtChar[0];
          txBuffer_[2*i + 3] = ID.DtChar[1];
          i++;
        }
        else
          break;
      }
      // Если положили хоть одно событие
      if (i) {
        // Добавить команду
        txBuffer_[0] = command;
        count = 2*i + 4;
        //  Количество байт в пакете
        txBuffer_[1] = count;
        // Вычисляем и добавляем контрольную сумму
        calcCrc = crc16_ibm(txBuffer_, (txBuffer_[count] - 2));
//        calcCrc = crc16_ibm(0, (txBuffer_[count] - 2), txBuffer_);
        txBuffer_[count-1] = calcCrc >> 8;
        txBuffer_[count] = calcCrc;
      }
      // Если не было событий, проверим очередь данных и положим из неё
      else {
        while (1) {
          ID.DtInt16[0] = getMessageParamSPI();
          if (ID.DtInt16[0]) {
            // TODO: Value.DtFloat = Device.getValue(ID.DtUint16);
            txBuffer_[6*i + 2] = ID.DtChar[0];
            txBuffer_[6*i + 3] = ID.DtChar[1];
            txBuffer_[6*i + 4] = Value.DtChar[0];
            txBuffer_[6*i + 5] = Value.DtChar[1];
            txBuffer_[6*i + 6] = Value.DtChar[2];
            txBuffer_[6*i + 7] = Value.DtChar[3];
            i++;
          }
          else
            break;
        }
        // Если есть данные для записи
        if (i) {
          // Добавить команду
          txBuffer_[0] = NOVOBUS_COMMAND_WRITE;
          count = 6*i + 4;
          //  Количество байт в пакете
          txBuffer_[1] = count;
          // Вычисляем и добавляем контрольную сумму
          calcCrc = crc16_ibm(txBuffer_, txBuffer_[count] - 2);
          txBuffer_[count-1] = calcCrc >> 8;
          txBuffer_[count] = calcCrc;
        }
        // Нет новых параметов, возвращаем эхо
        else {
          txBuffer_[0] = rxBuffer_[0];
          txBuffer_[1] = rxBuffer_[1];
          txBuffer_[1] = rxBuffer_[2];
          txBuffer_[1] = rxBuffer_[3];
          txBuffer_[1] = rxBuffer_[4];
        }
      }
      break;

    // Команда чтения данных
    case  NOVOBUS_COMMAND_READ:
      // Количество байт - команда, счётчик, контрольная сумма / 2 байта на параметр
      countParam = (count - 4)/2;
      // Цикл по количеству считываемых параметров
      for (int i = 0; i < countParam; i++) {
        // Получаем ID параметра
        ID.DtChar[0] = rxBuffer_[2*i + 2];
        ID.DtChar[1] = rxBuffer_[2*i + 3];
        // Получить значение параметра
        // TODO: Value.DtFloat = Device.getValue(ID.DtUint16[0]);
        // Переписать в выходной буфер ID
        txBuffer_[6*i + 2] = ID.DtChar[0];
        txBuffer_[6*i + 3] = ID.DtChar[1];
        // Переписать в выходной буфер Value
        txBuffer_[6*i + 4] = Value.DtChar[0];
        txBuffer_[6*i + 5] = Value.DtChar[1];
        txBuffer_[6*i + 6] = Value.DtChar[1];
        txBuffer_[6*i + 7] = Value.DtChar[1];
      }
      // Добавить команду
      txBuffer_[0] = command;
      //  Количество байт в пакете
      txBuffer_[1] = count;
      // Вычисляем и добавляем контрольную сумму
      calcCrc = crc16_ibm(txBuffer_, txBuffer_[count] - 2);
      txBuffer_[count-1] = (char)(calcCrc >> 8);
      txBuffer_[count] = (char)calcCrc;
      break;

    // Команда записи данных
    case  NOVOBUS_COMMAND_WRITE:
      // Количество байт - команда, счётчик, контрольная сумма / 8 байт
      // на параметр 4 на ID и 4 на значение
      countParam = (count - 4)/6;
      for (int i = 0; i < countParam; i++) {
        // Получаем ID параметра
        ID.DtChar[0] = rxBuffer_[6*i + 2];
        ID.DtChar[1] = rxBuffer_[6*i + 3];
        // Получаем значение параметра
        Value.DtChar[0] = rxBuffer_[6*i + 4];
        Value.DtChar[1] = rxBuffer_[6*i + 5];
        Value.DtChar[2] = rxBuffer_[6*i + 6];
        Value.DtChar[3] = rxBuffer_[6*i + 7];
        // Вызываем функцию записи значения параметра
        // TODO: Device.setValue(ID.DtUint16, Value.DtFloat);
        // Указываем ID в ответном пакете
        txBuffer_[6*i + 2] = ID.DtChar[0];
        txBuffer_[6*i + 3] = ID.DtChar[1];
        // Код пока всегда указываем что всё хорошо
        // TODO: Добавить обработку если не записали параметр
        txBuffer_[6*i + 4] = 0;
        txBuffer_[6*i + 5] = 0;
        txBuffer_[6*i + 6] = 0;
        txBuffer_[6*i + 7] = 0;
      }
      // Добавить команду
      txBuffer_[0] = command;
      //  Количество байт в пакете
      txBuffer_[1] = count;
      // Вычисляем и добавляем контрольную сумму
      calcCrc = crc16_ibm(txBuffer_, txBuffer_[count] - 2);
      txBuffer_[count-1] = calcCrc >> 8;
      txBuffer_[count] = calcCrc;
      break;
    // Команда посылки сообщения
    case  NOVOBUS_COMMAND_MESSAGE:

      break;
    default:

      break;
    }
  }
}

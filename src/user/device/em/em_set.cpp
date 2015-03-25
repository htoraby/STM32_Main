#include "em_set.h"
#include "user_main.h"
#include <string.h>

static void emSetTask(void *p)
{
  (static_cast<EmSet*>(p))->task();
}

EmSet::EmSet()
  : numberRequest_(0)
  , constEm_(1250)
  , constCoefTrans_(1.0)
{
  readParameters();
}

EmSet::~EmSet()
{
  closePort();
  osThreadTerminate(threadId_);
}

void EmSet::init()
{
  openPort(9600);

  osThreadDef_t t = {"EmSet", emSetTask, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE};
  threadId_ = osThreadCreate(&t, this);
}

void EmSet::task()
{
  while (1) {
    osDelay(10);

    sendRequest();
    receiveAnswer();
  }
}

void EmSet::sendRequest()
{
  int sizePktTx = 0;

  switch (numberRequest_)
  {
    // Запрос чтения данных 1
    case 1:
      {
        uint8_t buf[] = {0x84,0x08,0x23,0x01,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных 2
    case 2:
      {
        uint8_t buf[] = {0x84,0x08,0x23,0x02,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных 3
    case 3:
      {
        uint8_t buf[] = {0x84,0x08,0x23,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных константы счётчика
    case 4:
      {
        uint8_t buf[] = {0x84,0x08,0x12,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных энергии
    case 5:
      {
        uint8_t buf[] = {0x84,0x0A,0x00,0x00,0x00,0x05,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения коэффициента трансформации тока
    case 6:
      {
        uint8_t buf[] = {0x84,0x08,0x02,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запись коэффициента трансформации тока
    case 7:
      {
        uint8_t buf[] = {0x84,0x03,0x1C,0x00,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
        int coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
        txBuffer_[3] = coefTrans >> 8;
        txBuffer_[4] = coefTrans & 0x00FF;
      }
      break;
    // Запрос чтения данных энергии за весь период
    case 8:
      {
        uint8_t buf[] = {0x84,0x0A,0x00,0x00,0x00,0x0F,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных энергии за текущие сутки
    case 9:
      {
        uint8_t buf[] = {0x84,0x0A,0x04,0x00,0x00,0x0F,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных энергии за предыдущие сутки
    case 10:
      {
        uint8_t buf[] = {0x84,0x0A,0x05,0x00,0x00,0x0F,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
    // Запрос чтения данных энергии за текущий месяц
    case 11:
      {
        uint8_t buf[] = {0x84,0x0A,0x03,0x00,0x00,0x0F,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
        time_t time = ksu.getTime();
        tm dateTime = *localtime(&time);
        txBuffer_[3] = dateTime.tm_mon;
      }
      break;
    // Запрос чтения данных энергии за предыдущий месяц
    case 12:
      {
        uint8_t buf[] = {0x84,0x0A,0x03,0x00,0x00,0x0F,0x00,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
        time_t time = ksu.getTime();
        tm dateTime = *localtime(&time);
        txBuffer_[3] = (dateTime.tm_mon == 1) ? 12 : (dateTime.tm_mon - 1);
      }
      break;
    // Запрос открытия канала данных
    default:
      {
        uint8_t buf[] = {0x84,0x01,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00};
        sizePktTx = sizeof(buf);
        memcpy(txBuffer_, buf, sizePktTx);
      }
      break;
  }

  uint16_t calcCrc = crc16_ibm(txBuffer_, sizePktTx);
  sizePktTx = sizePktTx + 2;
  txBuffer_[sizePktTx - 2] = calcCrc & 0x00FF;
  txBuffer_[sizePktTx - 1] = calcCrc >> 8;

  sendUart(txBuffer_, sizePktTx);
}

void EmSet::receiveAnswer()
{
  sizePkt_ = receiveUart(rxBuffer_);
  if (!sizePkt_) {
    // TODO: Предупреждение: пустой пакет
    return;
  }

  uint16_t rxCrc = (rxBuffer_[sizePkt_ - 1] << 8) + rxBuffer_[sizePkt_ - 2];
  uint16_t calcCrc = crc16_ibm(rxBuffer_, sizePkt_ - 2);

  if (rxCrc != calcCrc) {
    // TODO: Предупреждение: Ошибка CRC
    return;
  }

  switch (numberRequest_)
  {
    // Ответ на запрос открытия канала данных
    case 0:
      // Проверка на открытие канала
      if (rxBuffer_[1] == 0) {
        numberRequest_++;
      }
      // Канал не открыт - повторяем запрос
      else {
        numberRequest_ = 0;
      }
      break;
    // Ответ на запрос чтения данных 1
    case 1:
      setParameter();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных 2
    case 2:
      setParameter();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных 3
    case 3:
      setParameter();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных константы счётчика
    case 4:
      setConstantEm();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии
    case 5:
      setParamEnergy();
      numberRequest_++;
      break;
    // Ответ на запрос чтения коэффициента трансформации тока
    case 6:
      prepareSendConstantCoefTrans();
      numberRequest_++;
      break;
    // Запись коэффициента трансформации тока
    case 7:
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии за весь период
    case 8:
      setParamEnergyConsumpAll();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии за текущие сутки
    case 9:
      setParamEnergyConsumpCurDay();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии за предыдущие сутки
    case 10:
      setParamEnergyConsumpPrevDay();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии за текущий месяц
    case 11:
      setParamEnergyConsumpCurMonth();
      numberRequest_++;
      break;
    // Ответ на запрос чтения данных энергии за предыдущий месяц
    case 12:
      setParamEnergyConsumpPrevMonth();
      numberRequest_ = 1;
      break;
  }

  asm("nop");
}

void EmSet::setParameter()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  switch(rxBuffer_[1])
  {
    case  1:
      // 16. Резерв
      value.char_t[0] = rxBuffer_[sizePkt_ - 6];
      value.char_t[1] = rxBuffer_[sizePkt_ - 5];
      value.char_t[2] = rxBuffer_[sizePkt_ - 4];
      value.char_t[3] = rxBuffer_[sizePkt_ - 3];
//      setValue(EM_, value.tdFloat);

      // 15. Резерв
      value.char_t[0] = rxBuffer_[sizePkt_ - 10];
      value.char_t[1] = rxBuffer_[sizePkt_ - 9];
      value.char_t[2] = rxBuffer_[sizePkt_ - 8];
      value.char_t[3] = rxBuffer_[sizePkt_ - 7];
//      setValue(EM_, value.tdFloat);

      // 14. Температура внутри счётчика
      value.char_t[0] = rxBuffer_[sizePkt_ - 14];
      value.char_t[1] = rxBuffer_[sizePkt_ - 13];
      value.char_t[2] = rxBuffer_[sizePkt_ - 12];
      value.char_t[3] = rxBuffer_[sizePkt_ - 11];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 13. Частота
      value.char_t[0] = rxBuffer_[sizePkt_ - 18];
      value.char_t[1] = rxBuffer_[sizePkt_ - 17];
      value.char_t[2] = rxBuffer_[sizePkt_ - 16];
      value.char_t[3] = rxBuffer_[sizePkt_ - 15];
      value.float_t = value.float_t*1000.0;
      setValue(EM_FREQUENCY, value.float_t);

      // 12. Активная мощность потерь Фаза 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 22];
      value.char_t[1] = rxBuffer_[sizePkt_ - 21];
      value.char_t[2] = rxBuffer_[sizePkt_ - 20];
      value.char_t[3] = rxBuffer_[sizePkt_ - 19];
//      setValue(EM_, value.tdFloat);

      // 11. Активная мощность потерь Фаза 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 26];
      value.char_t[1] = rxBuffer_[sizePkt_ - 25];
      value.char_t[2] = rxBuffer_[sizePkt_ - 24];
      value.char_t[3] = rxBuffer_[sizePkt_ - 23];
//      setValue(EM_, value.tdFloat);

      // 10. Активная мощность потерь Фаза 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 30];
      value.char_t[1] = rxBuffer_[sizePkt_ - 29];
      value.char_t[2] = rxBuffer_[sizePkt_ - 28];
      value.char_t[3] = rxBuffer_[sizePkt_ - 27];
//      setValue(EM_, value.tdFloat);

      // 9. Активная мошность потерь по трём фазам
      value.char_t[0] = rxBuffer_[sizePkt_ - 34];
      value.char_t[1] = rxBuffer_[sizePkt_ - 33];
      value.char_t[2] = rxBuffer_[sizePkt_ - 32];
      value.char_t[3] = rxBuffer_[sizePkt_ - 31];
//      setValue(EM_, value.tdFloat);

      // 8. THD между фазами 3 и 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 38];
      value.char_t[1] = rxBuffer_[sizePkt_ - 37];
      value.char_t[2] = rxBuffer_[sizePkt_ - 36];
      value.char_t[3] = rxBuffer_[sizePkt_ - 35];
//      setValue(EM_, value.tdFloat);

      // 7. THD между фазами 2 и 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 42];
      value.char_t[1] = rxBuffer_[sizePkt_ - 41];
      value.char_t[2] = rxBuffer_[sizePkt_ - 40];
      value.char_t[3] = rxBuffer_[sizePkt_ - 39];
//      setValue(EM_, value.tdFloat);

      // 6. THD между фазами 1 и 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 46];
      value.char_t[1] = rxBuffer_[sizePkt_ - 45];
      value.char_t[2] = rxBuffer_[sizePkt_ - 44];
      value.char_t[3] = rxBuffer_[sizePkt_ - 43];
//      setValue(EM_, value.tdFloat);

      // 5. Коэффициент несимметрии напряжённости
      value.char_t[0] = rxBuffer_[sizePkt_ - 50];
      value.char_t[1] = rxBuffer_[sizePkt_ - 49];
      value.char_t[2] = rxBuffer_[sizePkt_ - 48];
      value.char_t[3] = rxBuffer_[sizePkt_ - 47];
//      setValue(EM_, value.tdFloat);

      // 4. THD Напряжения на фазе 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 54];
      value.char_t[1] = rxBuffer_[sizePkt_ - 53];
      value.char_t[2] = rxBuffer_[sizePkt_ - 52];
      value.char_t[3] = rxBuffer_[sizePkt_ - 51];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 3. THD Напряжения на фазе 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 58];
      value.char_t[1] = rxBuffer_[sizePkt_ - 57];
      value.char_t[2] = rxBuffer_[sizePkt_ - 56];
      value.char_t[3] = rxBuffer_[sizePkt_ - 55];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 2. THD Напряжения на фазе 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 62];
      value.char_t[1] = rxBuffer_[sizePkt_ - 61];
      value.char_t[2] = rxBuffer_[sizePkt_ - 60];
      value.char_t[3] = rxBuffer_[sizePkt_ - 59];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 1. Коэффициент несимметрии напряжённости
      value.char_t[0] = rxBuffer_[sizePkt_ - 66];
      value.char_t[1] = rxBuffer_[sizePkt_ - 65];
      value.char_t[2] = rxBuffer_[sizePkt_ - 64];
      value.char_t[3] = rxBuffer_[sizePkt_ - 63];
//      setValue(EM_, value.tdFloat);
      break;
    case  2:
      // 16. THD тока по фазе 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 6];
      value.char_t[1] = rxBuffer_[sizePkt_ - 5];
      value.char_t[2] = rxBuffer_[sizePkt_ - 4];
      value.char_t[3] = rxBuffer_[sizePkt_ - 3];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 15. THD тока по фазе 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 10];
      value.char_t[1] = rxBuffer_[sizePkt_ - 9];
      value.char_t[2] = rxBuffer_[sizePkt_ - 8];
      value.char_t[3] = rxBuffer_[sizePkt_ - 7];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 14. THD тока по фазе 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 14];
      value.char_t[1] = rxBuffer_[sizePkt_ - 13];
      value.char_t[2] = rxBuffer_[sizePkt_ - 12];
      value.char_t[3] = rxBuffer_[sizePkt_ - 11];
      value.float_t = value.float_t*1000.0;
//      setValue(EM_, value.tdFloat);

      // 13. Коэффициент несиметрии тока в обратной последовательности
      value.char_t[0] = rxBuffer_[sizePkt_ - 18];
      value.char_t[1] = rxBuffer_[sizePkt_ - 17];
      value.char_t[2] = rxBuffer_[sizePkt_ - 16];
      value.char_t[3] = rxBuffer_[sizePkt_ - 15];
//      setValue(EM_, value.tdFloat);

      // 12. Ток фазы 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 22];
      value.char_t[1] = rxBuffer_[sizePkt_ - 21];
      value.char_t[2] = rxBuffer_[sizePkt_ - 20];
      value.char_t[3] = rxBuffer_[sizePkt_ - 19];
      value.float_t = value.float_t*(1000/coefTrans);
      setValue(EM_CURRENT_PHASE_3, value.float_t);

      // 11. Ток фазы 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 26];
      value.char_t[1] = rxBuffer_[sizePkt_ - 25];
      value.char_t[2] = rxBuffer_[sizePkt_ - 24];
      value.char_t[3] = rxBuffer_[sizePkt_ - 23];
      value.float_t = value.float_t*(1000/coefTrans);
      setValue(EM_CURRENT_PHASE_2, value.float_t);

      // 10. Ток фазы 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 30];
      value.char_t[1] = rxBuffer_[sizePkt_ - 29];
      value.char_t[2] = rxBuffer_[sizePkt_ - 28];
      value.char_t[3] = rxBuffer_[sizePkt_ - 27];
      value.float_t = value.float_t*(1000/coefTrans);
      setValue(EM_CURRENT_PHASE_1, value.float_t);

      // 9. Коэффициент несиметрии тока в прямой последовательности
      value.char_t[0] = rxBuffer_[sizePkt_ - 34];
      value.char_t[1] = rxBuffer_[sizePkt_ - 33];
      value.char_t[2] = rxBuffer_[sizePkt_ - 32];
      value.char_t[3] = rxBuffer_[sizePkt_ - 31];
//      setValue(EM_, value.tdFloat);

      // 8. Межфазное напряжение 3 и 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 38];
      value.char_t[1] = rxBuffer_[sizePkt_ - 37];
      value.char_t[2] = rxBuffer_[sizePkt_ - 36];
      value.char_t[3] = rxBuffer_[sizePkt_ - 35];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_3_1, value.float_t);

      // 7. Межфазное напряжение 2 и 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 42];
      value.char_t[1] = rxBuffer_[sizePkt_ - 41];
      value.char_t[2] = rxBuffer_[sizePkt_ - 40];
      value.char_t[3] = rxBuffer_[sizePkt_ - 39];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_2_3, value.float_t);

      // 6. Межфазное напряжение 1 и 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 46];
      value.char_t[1] = rxBuffer_[sizePkt_ - 45];
      value.char_t[2] = rxBuffer_[sizePkt_ - 44];
      value.char_t[3] = rxBuffer_[sizePkt_ - 43];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_1_2, value.float_t);

      // 5. Напряжение прямой последовательности
      value.char_t[0] = rxBuffer_[sizePkt_ - 50];
      value.char_t[1] = rxBuffer_[sizePkt_ - 49];
      value.char_t[2] = rxBuffer_[sizePkt_ - 48];
      value.char_t[3] = rxBuffer_[sizePkt_ - 47];
//      setValue(EM_, value.tdFloat);

      // 4. Напряжение на 3 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 54];
      value.char_t[1] = rxBuffer_[sizePkt_ - 53];
      value.char_t[2] = rxBuffer_[sizePkt_ - 52];
      value.char_t[3] = rxBuffer_[sizePkt_ - 51];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_3, value.float_t);

      // 3. Напряжение на 2 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 58];
      value.char_t[1] = rxBuffer_[sizePkt_ - 57];
      value.char_t[2] = rxBuffer_[sizePkt_ - 56];
      value.char_t[3] = rxBuffer_[sizePkt_ - 55];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_2, value.float_t);

      // 2. Напряжение на 1 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 62];
      value.char_t[1] = rxBuffer_[sizePkt_ - 61];
      value.char_t[2] = rxBuffer_[sizePkt_ - 60];
      value.char_t[3] = rxBuffer_[sizePkt_ - 59];
      value.float_t = value.float_t*1000.0;
      setValue(EM_VOLTAGE_PHASE_1, value.float_t);

      // 1. Батарея питания
      value.char_t[0] = rxBuffer_[sizePkt_ - 66];
      value.char_t[1] = rxBuffer_[sizePkt_ - 65];
      value.char_t[2] = rxBuffer_[sizePkt_ - 64];
      value.char_t[3] = rxBuffer_[sizePkt_ - 63];
//      setValue(EM_, value.tdFloat);
      break;
    case  3:
      // 16. Коэффициент мощности по 3 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 6];
      value.char_t[1] = rxBuffer_[sizePkt_ - 5];
      value.char_t[2] = rxBuffer_[sizePkt_ - 4];
      value.char_t[3] = rxBuffer_[sizePkt_ - 3];
      value.float_t = value.float_t*1000.0;
      setValue(EM_COS_PHI_PHASE_3, value.float_t);

      // 15. Коэффициент мощности по 2 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 10];
      value.char_t[1] = rxBuffer_[sizePkt_ - 9];
      value.char_t[2] = rxBuffer_[sizePkt_ - 8];
      value.char_t[3] = rxBuffer_[sizePkt_ - 7];
      value.float_t = value.float_t*1000.0;
      setValue(EM_COS_PHI_PHASE_2, value.float_t);

      // 14. Коэффициент мощности по 1 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 14];
      value.char_t[1] = rxBuffer_[sizePkt_ - 13];
      value.char_t[2] = rxBuffer_[sizePkt_ - 12];
      value.char_t[3] = rxBuffer_[sizePkt_ - 11];
      value.float_t = value.float_t*1000.0;
      setValue(EM_COS_PHI_PHASE_1, value.float_t);

      // 13. Коэффициент мощности по всем фазам
      value.char_t[0] = rxBuffer_[sizePkt_ - 18];
      value.char_t[1] = rxBuffer_[sizePkt_ - 17];
      value.char_t[2] = rxBuffer_[sizePkt_ - 16];
      value.char_t[3] = rxBuffer_[sizePkt_ - 15];
      value.float_t = value.float_t*1000.0;
      setValue(EM_COS_PHI, value.float_t);

      // 12. Полная мсщность по 3 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 22];
      value.char_t[1] = rxBuffer_[sizePkt_ - 21];
      value.char_t[2] = rxBuffer_[sizePkt_ - 20];
      value.char_t[3] = rxBuffer_[sizePkt_ - 19];
      value.float_t = value.float_t*1000.0;
      setValue(EM_FULL_POWER_PHASE_3, value.float_t);

      // 11. Полная мощность по 2 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 26];
      value.char_t[1] = rxBuffer_[sizePkt_ - 25];
      value.char_t[2] = rxBuffer_[sizePkt_ - 24];
      value.char_t[3] = rxBuffer_[sizePkt_ - 23];
      value.float_t = value.float_t*1000.0;
      setValue(EM_FULL_POWER_PHASE_2, value.float_t);

      // 10. Полная мощность по 1 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 30];
      value.char_t[1] = rxBuffer_[sizePkt_ - 29];
      value.char_t[2] = rxBuffer_[sizePkt_ - 28];
      value.char_t[3] = rxBuffer_[sizePkt_ - 27];
      value.float_t = value.float_t*1000.0;
      setValue(EM_FULL_POWER_PHASE_1, value.float_t);

      // 9. Полная мощность по 3-м фазам
      value.char_t[0] = rxBuffer_[sizePkt_ - 34];
      value.char_t[1] = rxBuffer_[sizePkt_ - 33];
      value.char_t[2] = rxBuffer_[sizePkt_ - 32];
      value.char_t[3] = rxBuffer_[sizePkt_ - 31];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_FULL_POWER, value.float_t);

      // 8. Реактивная мощность по 3 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 38];
      value.char_t[1] = rxBuffer_[sizePkt_ - 37];
      value.char_t[2] = rxBuffer_[sizePkt_ - 36];
      value.char_t[3] = rxBuffer_[sizePkt_ - 35];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_REACTIVE_POWER_PHASE_3, value.float_t);

      // 7. Реактивная мощность по 2 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 42];
      value.char_t[1] = rxBuffer_[sizePkt_ - 41];
      value.char_t[2] = rxBuffer_[sizePkt_ - 40];
      value.char_t[3] = rxBuffer_[sizePkt_ - 39];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_REACTIVE_POWER_PHASE_2, value.float_t);

      // 6.  Реактивная мощность по 1 фазе
      value.char_t[0] = rxBuffer_[sizePkt_ - 46];
      value.char_t[1] = rxBuffer_[sizePkt_ - 45];
      value.char_t[2] = rxBuffer_[sizePkt_ - 44];
      value.char_t[3] = rxBuffer_[sizePkt_ - 43];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_REACTIVE_POWER_PHASE_1, value.float_t);

      // 5. Реактивная мощность по 3-м фазам
      value.char_t[0] = rxBuffer_[sizePkt_ - 50];
      value.char_t[1] = rxBuffer_[sizePkt_ - 49];
      value.char_t[2] = rxBuffer_[sizePkt_ - 48];
      value.char_t[3] = rxBuffer_[sizePkt_ - 47];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_REACTIVE_POWER, value.float_t);

      // 4. Активная мсщность на фазе 3
      value.char_t[0] = rxBuffer_[sizePkt_ - 54];
      value.char_t[1] = rxBuffer_[sizePkt_ - 53];
      value.char_t[2] = rxBuffer_[sizePkt_ - 52];
      value.char_t[3] = rxBuffer_[sizePkt_ - 51];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_ACTIVE_POWER_PHASE_3, value.float_t);

      // 3. Активная мсщность на фазе 2
      value.char_t[0] = rxBuffer_[sizePkt_ - 58];
      value.char_t[1] = rxBuffer_[sizePkt_ - 57];
      value.char_t[2] = rxBuffer_[sizePkt_ - 56];
      value.char_t[3] = rxBuffer_[sizePkt_ - 55];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_ACTIVE_POWER_PHASE_2, value.float_t);

      // 2. Активная мсщность на фазе 1
      value.char_t[0] = rxBuffer_[sizePkt_ - 62];
      value.char_t[1] = rxBuffer_[sizePkt_ - 61];
      value.char_t[2] = rxBuffer_[sizePkt_ - 60];
      value.char_t[3] = rxBuffer_[sizePkt_ - 59];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_ACTIVE_POWER_PHASE_1, value.float_t);

      // 1. Суммарная активная мощность по 3 фазам
      value.char_t[0] = rxBuffer_[sizePkt_ - 66];
      value.char_t[1] = rxBuffer_[sizePkt_ - 65];
      value.char_t[2] = rxBuffer_[sizePkt_ - 64];
      value.char_t[3] = rxBuffer_[sizePkt_ - 63];
      value.float_t = value.float_t*1000/coefTrans;
      setValue(EM_ACTIVE_POWER, value.float_t);
      break;
  }
}

void EmSet::setParamEnergy()
{
  unTypeData value;
  value.uint32_t = 0;

  // 2. R
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*1000/(2*constEm_);
  setValue(EM_REACTIVE_ENERGY, value.float_t);

  // 1. A
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*1000000/(2*constEm_);
  setValue(EM_ACTIVE_ENERGY, value.float_t);
}

void EmSet::setParamEnergyConsumpAll()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  // 1. R-
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_MINUS_ALL, value.float_t);

  // 1. R+
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_PLUS_ALL, value.float_t);

  // 2. A-
  value.char_t[0] = rxBuffer_[sizePkt_ - 11];
  value.char_t[1] = rxBuffer_[sizePkt_ - 12];
  value.char_t[2] = rxBuffer_[sizePkt_ - 13];
  value.char_t[3] = rxBuffer_[sizePkt_ - 14];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_MINUS_ALL, value.float_t);

  // 2. A+
  value.char_t[0] = rxBuffer_[sizePkt_ - 15];
  value.char_t[1] = rxBuffer_[sizePkt_ - 16];
  value.char_t[2] = rxBuffer_[sizePkt_ - 17];
  value.char_t[3] = rxBuffer_[sizePkt_ - 18];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_PLUS_ALL, value.float_t);
}

void EmSet::setParamEnergyConsumpCurDay()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  // 1. R-
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_MINUS_CUR_DAY, value.float_t);

  // 1. R+
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_PLUS_CUR_DAY, value.float_t);

  // 2. A-
  value.char_t[0] = rxBuffer_[sizePkt_ - 11];
  value.char_t[1] = rxBuffer_[sizePkt_ - 12];
  value.char_t[2] = rxBuffer_[sizePkt_ - 13];
  value.char_t[3] = rxBuffer_[sizePkt_ - 14];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_MINUS_CUR_DAY, value.float_t);

  // 2. A+
  value.char_t[0] = rxBuffer_[sizePkt_ - 15];
  value.char_t[1] = rxBuffer_[sizePkt_ - 16];
  value.char_t[2] = rxBuffer_[sizePkt_ - 17];
  value.char_t[3] = rxBuffer_[sizePkt_ - 18];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_PLUS_CUR_DAY, value.float_t);
}

void EmSet::setParamEnergyConsumpPrevDay()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  // 1. R-
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_MINUS_PREV_DAY, value.float_t);

  // 1. R+
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_PLUS_PREV_DAY, value.float_t);

  // 2. A-
  value.char_t[0] = rxBuffer_[sizePkt_ - 11];
  value.char_t[1] = rxBuffer_[sizePkt_ - 12];
  value.char_t[2] = rxBuffer_[sizePkt_ - 13];
  value.char_t[3] = rxBuffer_[sizePkt_ - 14];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_MINUS_PREV_DAY, value.float_t);

  // 2. A+
  value.char_t[0] = rxBuffer_[sizePkt_ - 15];
  value.char_t[1] = rxBuffer_[sizePkt_ - 16];
  value.char_t[2] = rxBuffer_[sizePkt_ - 17];
  value.char_t[3] = rxBuffer_[sizePkt_ - 18];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_PLUS_PREV_DAY, value.float_t);
}

void EmSet::setParamEnergyConsumpCurMonth()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  // 1. R-
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_MINUS_CUR_MONTH, value.float_t);

  // 1. R+
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_PLUS_CUR_MONTH, value.float_t);

  // 2. A-
  value.char_t[0] = rxBuffer_[sizePkt_ - 11];
  value.char_t[1] = rxBuffer_[sizePkt_ - 12];
  value.char_t[2] = rxBuffer_[sizePkt_ - 13];
  value.char_t[3] = rxBuffer_[sizePkt_ - 14];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_MINUS_CUR_MONTH, value.float_t);

  // 2. A+
  value.char_t[0] = rxBuffer_[sizePkt_ - 15];
  value.char_t[1] = rxBuffer_[sizePkt_ - 16];
  value.char_t[2] = rxBuffer_[sizePkt_ - 17];
  value.char_t[3] = rxBuffer_[sizePkt_ - 18];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_PLUS_CUR_MONTH, value.float_t);
}

void EmSet::setParamEnergyConsumpPrevMonth()
{
  float coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);
  unTypeData value;
  value.uint32_t = 0;

  // 1. R-
  value.char_t[0] = rxBuffer_[sizePkt_ - 3];
  value.char_t[1] = rxBuffer_[sizePkt_ - 4];
  value.char_t[2] = rxBuffer_[sizePkt_ - 5];
  value.char_t[3] = rxBuffer_[sizePkt_ - 6];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_MINUS_PREV_MONTH, value.float_t);

  // 1. R+
  value.char_t[0] = rxBuffer_[sizePkt_ - 7];
  value.char_t[1] = rxBuffer_[sizePkt_ - 8];
  value.char_t[2] = rxBuffer_[sizePkt_ - 9];
  value.char_t[3] = rxBuffer_[sizePkt_ - 10];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_REACTIVE_PLUS_PREV_MONTH, value.float_t);

  // 2. A-
  value.char_t[0] = rxBuffer_[sizePkt_ - 11];
  value.char_t[1] = rxBuffer_[sizePkt_ - 12];
  value.char_t[2] = rxBuffer_[sizePkt_ - 13];
  value.char_t[3] = rxBuffer_[sizePkt_ - 14];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_MINUS_PREV_MONTH, value.float_t);

  // 2. A+
  value.char_t[0] = rxBuffer_[sizePkt_ - 15];
  value.char_t[1] = rxBuffer_[sizePkt_ - 16];
  value.char_t[2] = rxBuffer_[sizePkt_ - 17];
  value.char_t[3] = rxBuffer_[sizePkt_ - 18];
  value.float_t = (float)value.uint32_t*coefTrans*constCoefTrans_/(2*constEm_);
  setValue(EM_ACTIVE_PLUS_PREV_MONTH, value.float_t);
}

void EmSet::setConstantEm()
{
  switch(rxBuffer_[sizePkt_ - 4] & 0x0F) {
    case 0:
      constEm_ = 5000;
      break;
    case 1:
      constEm_ = 25000;
      break;
    case 2:
      constEm_ = 1250;
      break;
    case 3:
      constEm_ = 6250;
      break;
    case 4:
      constEm_ = 500;
      break;
    case 5:
      constEm_ = 250;
      break;
    case 6:
      constEm_ = 6400;
      break;
  }
}

int EmSet::prepareSendConstantCoefTrans()
{
  constCoefTrans_ = rxBuffer_[sizePkt_ - 12] * 256 + rxBuffer_[sizePkt_ - 11];
  int coefTrans = getValue(EM_COEFFICIENT_TRANS_CURRENT);

  if((rxBuffer_[sizePkt_ - 9] == (coefTrans & 0x00FF))
     && (rxBuffer_[sizePkt_ - 10] == (coefTrans >> 8)))
    return 8;
  else
    return 7;
}

#include "vsd_log.h"
#include "user_main.h"

#define MAX_COUNT 120

VsdLog::VsdLog(int numPort,
               long baudRate,
               int dataBits,
               int stopBits,
               int parity,
               int address)
  : numPort_(numPort)
  , devAdrs_(address)
  , prevConnect_(true)
{
  mms_ = new ModbusMasterSerial();
  // Открываем порт
  mms_->openProtocol(numPort_, baudRate, dataBits, stopBits, parity);

  semaphoreId_ = osSemaphoreCreate(NULL, 1);
}

VsdLog::~VsdLog()
{
  mms_->closeProtocol(numPort_);
  delete mms_;

  osSemaphoreDelete(semaphoreId_);
}

bool VsdLog::isConnect()
{
  bool curConnect = mms_->isConnect();

  if (prevConnect_ && !curConnect) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Архивы ЧРП: Связь потеряна (numPort = %d, devAdrs = %d)", numPort_, devAdrs_);
#endif
  }

  if (!prevConnect_ && curConnect) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Архивы ЧРП: Связь восстановлена (numPort = %d, devAdrs = %d)", numPort_, devAdrs_);
#endif
  }

  prevConnect_ = curConnect;
  return curConnect;
}

bool VsdLog::checkAlarm()
{
  return false;
}

bool VsdLog::checkReady()
{
  return false;
}

void VsdLog::resetReady()
{

}

void VsdLog::setAlarm()
{

}

void VsdLog::resetAlarm()
{

}

void VsdLog::readAlarmLog(int16_t */*ia*/, int16_t */*ib*/, int16_t */*ic*/,
                          int16_t */*ud*/)
{

}

void VsdLog::readRunningLog(int16_t */*ia*/, int16_t */*ib*/, int16_t */*ic*/,
                            int16_t */*ud*/, int16_t */*cos*/)
{

}

void VsdLog::writeReg(uint32_t addr, int16_t value)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  uint16_t res = 1;
  res = mms_->writeSingleRegister(devAdrs_, addr, value);
  if (res != MODBUS_OK) {
    asm("nop");
  }

  osSemaphoreRelease(semaphoreId_);
}

void VsdLog::readLog(uint32_t addr, uint16_t *buf, uint32_t size)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  uint16_t res = 1;
  uint16_t address = addr;
  uint32_t count = MAX_COUNT;
  uint32_t readAll = 0;
  while (1) {
    if ((readAll + MAX_COUNT) > size)
      count = size - readAll;
    res = mms_->readMultipleRegisters(devAdrs_, address, &buf[readAll], count);
    if (res != MODBUS_OK) {
      break;
    }
    readAll += count;
    address += count;
    if (readAll >= size)
      break;
  }

  osSemaphoreRelease(semaphoreId_);
}

void VsdLog::readNovometLog(int16_t *ia, int16_t *ib, int16_t *ic,
                                 int16_t *ud)
{
  osSemaphoreWait(semaphoreId_, osWaitForever);

  uint16_t buffer[210] = {0};                             // Буфер с запросом данных
  int16_t field = 1999;                                   // Количество не готовых записей
  uint16_t fieldShift = 25;                               // Смещение от конца архива
  uint16_t fieldCnt = 25;                                 // Количество читаемых записей
  int16_t i = 0;
  int16_t res = 0;
  int16_t val = 0;

  float difCoefCur = parameters.get(VSD_MAXVAL_CAN_INV_IA);     // Смещение нуля тока
  float propCoefCur = parameters.get(VSD_MAX_CAN_INV_IA);       // Максимум тока
  if (propCoefCur == 0)
    propCoefCur = 1;
  propCoefCur = (parameters.get(VSD_SCALE_CAN_INV_IA) / propCoefCur) * (parameters.get(VSD_I_SCALE) / 100.0);

  float difCoefVolt = parameters.get(VSD_MAXVAL_CAN_INV_UD);     // Смещение нуля напряжения
  float propCoefVolt = parameters.get(VSD_MAX_CAN_INV_UD);
  if (propCoefVolt == 0)
    propCoefVolt = 1;
  propCoefVolt = (parameters.get(VSD_SCALE_CAN_INV_UD) / propCoefVolt) * (parameters.get(VSD_U_SCALE) / 100.0);

  while (field >= 0) {                            // Пока не все записи
    res = mms_->readLogNovomet(devAdrs_, fieldShift, buffer, fieldCnt);
    if (res == 0) {                                       // Нет ответа зануляем то что опрашивали
      while (field >= 0) {                    // Все последующие данные зануляем
        ic[field] = 0;
        ib[field] = 0;
        ia[field] = 0;
        ud[field] = 0;
        field--;
      }
//      while (fieldCnt > 0) {                              // От 10 до 1
//        ic[field] = 0;
//        ib[field] = 0;
//        ia[field] = 0;
//        ud[field] = 0;
//        fieldCnt--;
//        field--;
//      }
//      fieldCnt = 25;
//      fieldShift = fieldShift + fieldCnt;

    }
    else {
      if (res == 1) {                                     // Получили сообщение что больше нет данных
        while (field >= 0) {                    // Все последующие данные зануляем
          ic[field] = 0;
          ib[field] = 0;
          ia[field] = 0;
          ud[field] = 0;
          field--;
        }
      }
      else {
//        i = res - 1;
//        while (i >= 3) {
//          ic[field] = (int)(((float)((int)buffer[i]) - difCoefCur) * propCoefCur);
//          ib[field] = (int)(((float)((int)buffer[i - 1])- difCoefCur) * propCoefCur);
//          ia[field] = (int)(((float)((int)buffer[i - 2]) - difCoefCur) * propCoefCur);
//          ud[field] = (int)(((float)((int)buffer[i - 3]) - difCoefVolt) * propCoefVolt);
//          i = i - 4;
//          field--;
//        }
        while (i <= res - 1) {
          val = (int)buffer[i];
          ud[field] = (int)(((float)val - difCoefVolt) * propCoefVolt);
          i++;
          val = (int)buffer[i];
          ia[field] = (int)(((float)val - difCoefCur) * propCoefCur);
          i++;
          val = (int)buffer[i];
          ib[field] = (int)(((float)val - difCoefCur) * propCoefCur);
          i++;
          val = (int)buffer[i];
          ic[field] = (int)(((float)val - difCoefCur) * propCoefCur);
          i++;
          field--;
        }
        if ((res / 4) == fieldCnt) {
          fieldCnt = 25;
          fieldShift = fieldShift + fieldCnt;
        }
        else {
          fieldShift = fieldShift + res / 4;
          fieldCnt = fieldCnt - res / 4;
        }
      }
    }
    i = 0;
  }

  osSemaphoreRelease(semaphoreId_);
}

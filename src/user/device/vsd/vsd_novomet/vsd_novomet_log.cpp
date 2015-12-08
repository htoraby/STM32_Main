#include "vsd_novomet_log.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"
#include "user_main.h"

VsdNovometLog::VsdNovometLog()
  : VsdLog(VSD_LOG_UART, 57600, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{

}

VsdNovometLog::~VsdNovometLog()
{

}

bool VsdNovometLog::checkAlarm()
{
  return (!parameters.get(CCS_DI_11_VALUE) && parameters.isValidity(CCS_DI_11_VALUE));
}

bool VsdNovometLog::checkReady()
{
  return true;
}

void VsdNovometLog::resetReady()
{
  setDigitalOutput(DO1, PinReset);
}

void VsdNovometLog::resetAlarm()
{
  setDigitalOutput(DO1, PinReset);
}

void VsdNovometLog::setAlarm()
{
  setDigitalOutput(DO1, PinSet);
}

void VsdNovometLog::readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                                 uint16_t *ud)
{
  uint8_t txBuffer[8] = {0};                              // Буфер с запросом данных
  uint8_t rxBuffer[253] = {0};                            // Входной буфер
  int16_t fieldInComplete = 2000;                         // Количество не готовых записей
  uint16_t retryCnt = 0;                                  // Количество повторов
  uint16_t fieldShift = 30;                               // Смещение от конца архива
  uint16_t fieldCnt = 30;                                 // Количество читаемых записей
  uint16_t crc = 0xffff;                                  // Контрольная сумма
  uint16_t rxByteCnt = 0;                                 // Количество принятых байт
  int16_t i = 0;
  div_t fieldRead;                                        // Количество целых записей
  while (fieldInComplete >= 0) {                            // Пока не все записи
    while (retryCnt < 5) {
      txBuffer[0] = 1;                                    // Адрес устройства
      txBuffer[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;   // Команды
      txBuffer[2] = ((fieldShift >> 8) & 0x00ff);         // Старший байт адреса первого регистра
      txBuffer[3] = (fieldShift) & 0x00ff;                // Младший байт адреса первого регистра
      txBuffer[4] = ((fieldCnt >> 8) & 0x00ff);           // Старший байт количества регистров
      txBuffer[5] = (fieldCnt) & 0x00ff;                  // Младший байт количества регистров
      crc = crc16_ibm(txBuffer, 6);                       // Вычисляем контрольную сумму
      txBuffer[6] = crc & 0x00ff;                         // Младший байт контрольной суммы
      txBuffer[7] = ((crc >> 8) & 0x00ff);                // Старший байт контрольной суммы
      uartWriteData(VSD_LOG_UART, txBuffer, 8);           // Посылка запроса

      rxByteCnt = uartReadData(VSD_LOG_UART, rxBuffer);   // Приём данных
                                                          // Записей больше нет
      if ((rxByteCnt == 2) && (rxBuffer[rxByteCnt-1] == 0xff) && (rxBuffer[rxByteCnt-2] == 0xff)) {
        while (fieldInComplete >= 0) {                    // Все последующие данные зануляем
          ic[fieldInComplete] = 0;
          ib[fieldInComplete] = 0;
          ia[fieldInComplete] = 0;
          ud[fieldInComplete] = 0;
          fieldInComplete--;
        }
      }
      else {
        fieldRead = div(rxByteCnt - 2,8);                 // Проверяем сколько у нас записей
        if ((fieldRead.rem == 0) && (((rxBuffer[rxByteCnt - 1] << 8) + rxBuffer[rxByteCnt - 2]) == crc16_ibm(rxBuffer, (rxByteCnt - 2)))) {
          i = rxByteCnt - 3;
          while (i >= 7) {
            ic[fieldInComplete] = (rxBuffer[i - 1] << 8) + rxBuffer[i];
            ib[fieldInComplete] = (rxBuffer[i - 3] << 8) + rxBuffer[i - 2];
            ia[fieldInComplete] = (rxBuffer[i - 5] << 8) + rxBuffer[i - 4];
            ud[fieldInComplete] = (rxBuffer[i - 7] << 8) + rxBuffer[i - 6];
            i = i - 8;
            fieldInComplete--;
          }
          if (fieldRead.quot == fieldCnt) {
            fieldCnt = 30;
            fieldShift = fieldShift + fieldCnt;
          }
          else {
            fieldShift = fieldShift + fieldRead.quot;
            fieldCnt = fieldCnt - fieldRead.quot;
          }
          i = 0;
          retryCnt = 5;
        }
        else {
          retryCnt++;
          if (retryCnt >= 5) {
            i = fieldCnt;
            while (i >= 0) {
              ud[fieldInComplete] = 0;
              ia[fieldInComplete] = 0;
              ib[fieldInComplete] = 0;
              ic[fieldInComplete] = 0;
              i--;
              fieldInComplete--;
            }
            fieldCnt = 30;
            fieldShift = fieldShift + fieldCnt;
          }
        }
      }
    }
    retryCnt = 0;
  }
}

void VsdNovometLog::readRunningLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                                  uint16_t *ud, uint16_t *cos)
{
  setAlarm();
  uint8_t txBuffer[8] = {0};                              // Буфер с запросом данных
  uint8_t rxBuffer[253] = {0};                            // Входной буфер
  int16_t fieldInComplete = 2000;                         // Количество не готовых записей
  uint16_t retryCnt = 0;                                  // Количество повторов
  uint16_t fieldShift = 30;                               // Смещение от конца архива
  uint16_t fieldCnt = 30;                                 // Количество читаемых записей
  uint16_t crc = 0xffff;                                  // Контрольная сумма
  uint16_t rxByteCnt = 0;                                 // Количество принятых байт
  int16_t i = 0;
  div_t fieldRead;                                        // Количество целых записей
  while (fieldInComplete >= 0) {                            // Пока не все записи
    while (retryCnt < 5) {
      txBuffer[0] = 1;                                    // Адрес устройства
      txBuffer[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;   // Команды
      txBuffer[2] = ((fieldShift >> 8) & 0x00ff);         // Старший байт адреса первого регистра
      txBuffer[3] = (fieldShift) & 0x00ff;                // Младший байт адреса первого регистра
      txBuffer[4] = ((fieldCnt >> 8) & 0x00ff);           // Старший байт количества регистров
      txBuffer[5] = (fieldCnt) & 0x00ff;                  // Младший байт количества регистров
      crc = crc16_ibm(txBuffer, 6);                       // Вычисляем контрольную сумму
      txBuffer[6] = crc & 0x00ff;                         // Младший байт контрольной суммы
      txBuffer[7] = ((crc >> 8) & 0x00ff);                // Старший байт контрольной суммы
      uartWriteData(VSD_LOG_UART, txBuffer, 8);           // Посылка запроса

      rxByteCnt = uartReadData(VSD_LOG_UART, rxBuffer);   // Приём данных
                                                          // Записей больше нет
      if ((rxByteCnt == 2) && (rxBuffer[rxByteCnt-1] == 0xff) && (rxBuffer[rxByteCnt-2] == 0xff)) {
        while (fieldInComplete >= 0) {                    // Все последующие данные зануляем
          ic[fieldInComplete] = 0;
          ib[fieldInComplete] = 0;
          ia[fieldInComplete] = 0;
          ud[fieldInComplete] = 0;
          cos[fieldInComplete] = 0;
          fieldInComplete--;
        }
      }
      else {
        fieldRead = div(rxByteCnt - 2,8);                 // Проверяем сколько у нас записей
        if ((fieldRead.rem == 0) && (((rxBuffer[rxByteCnt - 1] << 8) + rxBuffer[rxByteCnt - 2]) == crc16_ibm(rxBuffer, (rxByteCnt - 2)))) {
          i = rxByteCnt - 3;
          while (i >= 7) {
            ic[fieldInComplete] = (rxBuffer[i - 1] << 8) + rxBuffer[i];
            ib[fieldInComplete] = (rxBuffer[i - 3] << 8) + rxBuffer[i - 2];
            ia[fieldInComplete] = (rxBuffer[i - 5] << 8) + rxBuffer[i - 4];
            ud[fieldInComplete] = (rxBuffer[i - 7] << 8) + rxBuffer[i - 6];
            cos[fieldInComplete] = 0;
            i = i - 8;
            fieldInComplete--;
          }
          if (fieldRead.quot == fieldCnt) {
            fieldCnt = 30;
            fieldShift = fieldShift + fieldCnt;
          }
          else {
            fieldShift = fieldShift + fieldRead.quot;
            fieldCnt = fieldCnt - fieldRead.quot;
          }
          i = 0;
          retryCnt = 5;
        }
        else {
          retryCnt++;
          if (retryCnt >= 5) {
            i = fieldCnt;
            while (i >= 0) {
              ic[fieldInComplete] = 0;
              ib[fieldInComplete] = 0;
              ia[fieldInComplete] = 0;
              ud[fieldInComplete] = 0;
              cos[fieldInComplete] = 0;
              i--;
              fieldInComplete--;
            }
            fieldCnt = 30;
            fieldShift = fieldShift + fieldCnt;
          }
        }
      }
    }
    retryCnt = 0;
  }
}

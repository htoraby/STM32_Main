#include "vsd_novomet_log.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"

VsdNovometLog::VsdNovometLog()
  : VsdLog(VSD_LOG_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{

}

VsdNovometLog::~VsdNovometLog()
{

}

void VsdNovometLog::readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                                 uint16_t *ud)
{
  /*
  uint8_t txBuffer[8];                                    // Выходной буфер
  uint8_t rxBuffer[1024];                                 // Входной буфер
  int cntRxByte = 0;                                      // Количество принятых байт в ответе
  int crc = 0;                                            // Контрольная сумма
  int i = 0;
  int j = 0;
  int k = 0;
  int retryCnt = 0;                                       // Счётчиков повторов запросов данных
  for (cntBlock = 20; cntBlock--; cntBlock >= 1) {
    while (retryCnt < 5) {
      txBuffer_[0] = 1;                                   // Адрес устройства
      txBuffer_[1] = MODBUS_READ_HOLDING_REGISTERS_0x03;  // Команды
      txBuffer_[2] = (((cntBlock * 100) >> 8) & 0x00ff);  // Старший байт адреса первого регистра
      txBuffer_[3] = (cntBlock * 100) & 0x00ff;           // Младший байт адреса первого регистра
      txBuffer_[4] = 0;                                   // Старший байт количества регистров
      txBuffer_[5] = 100 & 0x00ff;                        // Младший байт количества регистров
      crc = crc16_ibm(txBuffer, 6);                       // Вычисляем контрольную сумму
      txBuffer_[6] = crc & 0x00ff;                        // Младший байт контрольной суммы
      txBuffer_[7] = ((crc >> 8) & 0x00ff);               // Старший байт контрольной суммы

      uartWriteData(VSD_LOG_UART, txBuffer, 8);
      cntRxByte = uartReadData(VSD_LOG_UART, rxBuffer);

      if (((rxBuffer[cntRxByte - 1] << 8) + rxBuffer[cntRxByte - 2]) == crc16_ibm(rxBuffer, (cntRxByte - 2))) {
        while (i < cntRxByte - 2) {
          ud[j] = rxBuffer[i] << 8 + rxBuffer[i + 1];
          ia[j] = rxBuffer[i + 2] << 8 + rxBuffer[i + 3];
          ib[j] = rxBuffer[i + 4] << 8 + rxBuffer[i + 5];
          ic[j] = rxBuffer[i + 6] << 8 + rxBuffer[i + 7];
          i = i + 8;
          j++;
        }
        retryCnt = 5;
      }
      else {
        retryCnt++;
        if (retryCnt >= 5) {
          k = j + 100;
          while (j <= k) {
            ud[j] = -1; ia[j] = -1; ib[j] = -1; ic[j] = -1;
            j++;
          }
        }
      }
    }
    retryCnt = 0;
  }
  */
}


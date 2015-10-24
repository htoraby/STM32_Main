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
  uint8_t txBuffer[5];
  uint8_t rxBuffer[4096];
  uint8_t rxStr[255];
  uint8_t param[5];
  int i,j,k,l,m,n;
  char* end_ptr;
  int16_t val;
  txBuffer[0] = 'l';
  txBuffer[1] = '0';
  txBuffer[2] = 'a';
  txBuffer[3] = 'd';
  txBuffer[4] = '\n';
  uartWriteData(VSD_LOG_UART, txBuffer, 5);
  // Получить входной буфер
  int cntRxByte = uartReadData(VSD_LOG_UART, rxBuffer);
  // Организовать цикл по буферу выделяя строки
  for (i = 0; i++; i <= cntRxByte) {
    if (rxBuffer[i] != '\n') {
      rxStr[j] = rxBuffer[i];   // Получили одну строку
      j++;
    }
    else {
      // В каждой строке, выделить подстроки со значениями Ud, Ia, Ib, Ic, Ud
      for (k = 0; k++; k <= 5) {
        while ((rxStr[l] != ' ') && (l <= j)) {
          param[m] = rxStr[l];
          l++;
          m++;
        }
        switch (k) {
        case 2:
          val = strtol((char*)param, &end_ptr, 10);
          if ((*end_ptr) || ((val >= 10000) || (val < 0))) {
            val = -1;
          }
          ud[n] = val;
          break;
          break;
        case 3:
          val = strtol((char*)param, &end_ptr, 10);
          if ((*end_ptr) || ((val >= 10000) || (val < 0))) {
            val = -1;
          }
          ia[n] = val;
          break;
        case 4:
          val = strtol((char*)param, &end_ptr, 10);
          if ((*end_ptr) || ((val >= 10000) || (val < 0))) {
            val = -1;
          }
          ib[n] = val;
          break;
        case 5:
          val = strtol((char*)param, &end_ptr, 10);
          if ((*end_ptr) || ((val >= 10000) || (val < 0))) {
            val = -1;
          }
          ic[n] = val;
          break;
        default:
          break;
        }
        param[0] = '\0';
        l++;
        m = 0;
      }
      rxStr[0] = '\0';
      j = 0;
      n++;
    }
  }
  */
}


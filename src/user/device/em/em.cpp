#include "em.h"
#include "uart.h"
#include "user_main.h"

//! Массив параметров устройства
static parameter parametersArray[EM_END - EM_BEGIN] __attribute__((section(".extmem")));

Em::Em()
  : Device(EM_BEGIN, parametersArray, EM_END - EM_BEGIN)
{
  initParameters();
}

Em::~Em()
{

}

void Em::openPort(uint32_t baudRate, uint32_t parity, uint32_t stopBits)
{
  // Инициализация UART
  if (uartInit(EM_UART, baudRate, parity, stopBits) == StatusError) {
    logDebug.add(CriticalMsg, "Em. Error init, UART:%d baudRate:%d parity:%d stopBits:%d",
                 EM_UART, baudRate, parity, stopBits);
  } else {
    semaphoreAnswer_ = uartGetSemaphoreId(EM_UART);
  }
}

void Em::closePort()
{
  uartClose(EM_UART);
}

StatusType Em::sendUart(uint8_t *data, int count)
{
  StatusType status = StatusOk;
  if (uartWriteData(EM_UART, data, count) == StatusError) {
#if (USE_LOG_WARNING == 1)
    logDebug.add(WarningMsg, "Счётчик: ошибка отправки данных (EM_UART = %d)",
                 EM_UART);
#endif
    status = StatusError;
  }
  return status;
}

int Em::receiveUart(uint8_t *data)
{
  // Начало приёма пакета с таймаутом ожидания ответа
  if (osSemaphoreWait(semaphoreAnswer_, EM_ANSWER_TIMEOUT) == osEventTimeout) {
    // Возвращаем ошибку что нет ответа от устройства
    return 0;
  }
  // Получили первый байт
  else {
    // Получаем пакет данных пока время между байтами не превысит EM_TIME_END_PACKAGE
    while (1) {
      if (osSemaphoreWait(semaphoreAnswer_, EM_TIME_END_PACKAGE) == osEventTimeout) {
        // Получаем количество принятых байт и данные
        return uartReadData(EM_UART, data);
      }
    }
  }
}

int Em::setCoefficientTransformation()
{
  return err_r;
}

int Em::setCoefficientTransforamationCurrent()
{
  return err_r;
}

int Em::setCoefficientTransforamationVoltage()
{
  return err_r;
}

bool Em::isConnect()
{
  if (counters_.failure > EM_COUNTER_LOST_CONNECT)
    return false;
  else
    return true;
}

void Em::getConnect()
{
  parameters.set(CCS_EM_CONNECTION_PERCENT, counters_.quality);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_TOTAL, counters_.transmite);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_SUCCESS, counters_.resive);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_TRASH, counters_.trash);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_CRC, counters_.crc);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_ERR, counters_.error);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_LOST, counters_.lost);
}

void Em::resetConnect()
{
  counters_ = {0,0,0,0,0,0,0,0};
  parameters.set(CCS_EM_CONNECTION_PERCENT, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_TOTAL, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_SUCCESS, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_TRASH, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_CRC, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_ERR, 0);
  parameters.set(CCS_EM_CONNECTION_PACKAGE_LOST, 0);
}

void Em::calcConnect()
{
  counters_.quality = counters_.resive / counters_.transmite * 100;
  if (counters_.transmite > 999999.0) {
    counters_ = {counters_.quality,0,0,0,0,0,0,0};
  }
}

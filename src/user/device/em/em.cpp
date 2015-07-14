#include "em.h"
#include "uart.h"

Em::Em() : Device(EM_BEGIN, parametersArray_, EM_END - EM_BEGIN)
{

}

Em::~Em()
{

}

void Em::openPort(uint32_t baudRate, uint32_t parity, uint32_t stopBits)
{
  // Инициализация UART
  if (uartInit(EM_UART, baudRate, parity, stopBits) == StatusError) {
    // TODO: Предупреждение об ошибке инициализации UART
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
    // TODO: Предупреждение об ошибке отправки данных

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
  if (failCounter_ >= 5)
    return false;
  else
    return true;
}

void Em::calcConnect()
{
  if (totalCounter_ > 1000000) {
    totalCounter_ = 0;
    successCounter_ = 0;
    lostCounter_ = 0;
    crcCounter_ = 0;
    errCounter_ = 0;
    trashCounter_ = 0;
  }
  if (totalCounter_) {
    calcConnect_ = (successCounter_ * 100) / totalCounter_;
  }
}

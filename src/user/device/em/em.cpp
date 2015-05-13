#include "em.h"
#include "uart.h"

Em::Em() : Device(EM_BEGIN, parametersArray_, EM_END - EM_BEGIN)
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

void Em::initParameters()
{
  // Пустой элемент массива
  parameters_[EM_BEGIN - EM_BEGIN].id            = EM_BEGIN;
  parameters_[EM_BEGIN - EM_BEGIN].access        = ACCESS_ERROR;
  parameters_[EM_BEGIN - EM_BEGIN].operation     = OPERATION_ERROR;
  parameters_[EM_BEGIN - EM_BEGIN].physic        = PHYSIC_NUMERIC;
  parameters_[EM_BEGIN - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_BEGIN - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_BEGIN - EM_BEGIN].min           = 0.0;
  parameters_[EM_BEGIN - EM_BEGIN].max           = 0.0;
  parameters_[EM_BEGIN - EM_BEGIN].def           = 0.0;

  parameters_[EM_FREQUENCY - EM_BEGIN].id            = EM_FREQUENCY;
  parameters_[EM_FREQUENCY - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_FREQUENCY - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_FREQUENCY - EM_BEGIN].physic        = PHYSIC_FREQUENCY;
  parameters_[EM_FREQUENCY - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_FREQUENCY - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_FREQUENCY - EM_BEGIN].min           = 0.0;
  parameters_[EM_FREQUENCY - EM_BEGIN].max           = 100000.0;
  parameters_[EM_FREQUENCY - EM_BEGIN].def           = 0.0;

  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].id            = EM_CURRENT_PHASE_1;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].physic        = PHYSIC_CURRENT;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].min           = 0.0;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_CURRENT_PHASE_1 - EM_BEGIN].def           = 0.0;

  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].id            = EM_CURRENT_PHASE_2;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].physic        = PHYSIC_CURRENT;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].min           = 0.0;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_CURRENT_PHASE_2 - EM_BEGIN].def           = 0.0;

  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].id            = EM_CURRENT_PHASE_3;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].physic        = PHYSIC_CURRENT;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].min           = 0.0;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_CURRENT_PHASE_3 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_1;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].physic        = PHYSIC_VOLTAGE;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_1 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_2;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].physic        = PHYSIC_VOLTAGE;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_2 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_3;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].physic        = PHYSIC_VOLTAGE;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_3 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_1_2;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].physic        = PHYSIC_VOLTAGE;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_1_2 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_2_3;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].physic        = PHYSIC_VOLTAGE;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_2_3 - EM_BEGIN].def           = 0.0;

  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].id            = EM_VOLTAGE_PHASE_3_1;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].min           = 0.0;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_VOLTAGE_PHASE_3_1 - EM_BEGIN].def           = 0.0;

  parameters_[EM_COS_PHI - EM_BEGIN].id            = EM_COS_PHI;
  parameters_[EM_COS_PHI - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_COS_PHI - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_COS_PHI - EM_BEGIN].physic        = PHYSIC_NUMERIC;
  parameters_[EM_COS_PHI - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_COS_PHI - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_COS_PHI - EM_BEGIN].min           = 0.0;
  parameters_[EM_COS_PHI - EM_BEGIN].max           = 100000.0;
  parameters_[EM_COS_PHI - EM_BEGIN].def           = 0.0;

  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].id            = EM_COS_PHI_PHASE_1;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].physic        = PHYSIC_NUMERIC;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].min           = 0.0;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_COS_PHI_PHASE_1 - EM_BEGIN].def           = 0.0;

  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].id            = EM_COS_PHI_PHASE_2;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].physic        = PHYSIC_NUMERIC;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].min           = 0.0;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_COS_PHI_PHASE_2 - EM_BEGIN].def           = 0.0;

  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].id            = EM_COS_PHI_PHASE_3;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].physic        = PHYSIC_NUMERIC;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].min           = 0.0;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].max           = 100000.0;
  parameters_[EM_COS_PHI_PHASE_3 - EM_BEGIN].def           = 0.0;

  parameters_[EM_FULL_POWER - EM_BEGIN].id            = EM_FULL_POWER;
  parameters_[EM_FULL_POWER - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_FULL_POWER - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_FULL_POWER - EM_BEGIN].physic        = PHYSIC_FULL_POWER;
  parameters_[EM_FULL_POWER - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_FULL_POWER - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_FULL_POWER - EM_BEGIN].min           = 0.0;
  parameters_[EM_FULL_POWER - EM_BEGIN].max           = 100000.0;
  parameters_[EM_FULL_POWER - EM_BEGIN].def           = 0.0;

  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].id        = EM_FULL_POWER_PHASE_1;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].physic    = PHYSIC_FULL_POWER;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].min       = 0.0;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_FULL_POWER_PHASE_1 - EM_BEGIN].def       = 0.0;

  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].id        = EM_FULL_POWER_PHASE_2;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].physic    = PHYSIC_FULL_POWER;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].min       = 0.0;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_FULL_POWER_PHASE_2 - EM_BEGIN].def       = 0.0;

  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].id        = EM_FULL_POWER_PHASE_3;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].physic    = PHYSIC_FULL_POWER;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].min       = 0.0;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_FULL_POWER_PHASE_3 - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_POWER - EM_BEGIN].id            = EM_ACTIVE_POWER;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].physic        = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].min           = 0.0;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].max           = 100000.0;
  parameters_[EM_ACTIVE_POWER - EM_BEGIN].def           = 0.0;

  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].id        = EM_ACTIVE_POWER_PHASE_1;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_POWER_PHASE_1 - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].id        = EM_ACTIVE_POWER_PHASE_2;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_POWER_PHASE_2 - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].id        = EM_ACTIVE_POWER_PHASE_3;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_POWER_PHASE_3 - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_POWER - EM_BEGIN].id            = EM_REACTIVE_POWER;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].access        = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].operation     = OPERATION_READ;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].physic        = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].validity      = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].value.float_t         = 0.0;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].min           = 0.0;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].max           = 100000.0;
  parameters_[EM_REACTIVE_POWER - EM_BEGIN].def           = 0.0;

  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].id        = EM_REACTIVE_POWER_PHASE_1;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_POWER_PHASE_1 - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].id        = EM_REACTIVE_POWER_PHASE_2;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_POWER_PHASE_2 - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].id        = EM_REACTIVE_POWER_PHASE_3;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_POWER_PHASE_3 - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].id        = EM_ACTIVE_ENERGY;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].physic    = PHYSIC_ACTIVE_ENERGY;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_ENERGY - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].id        = EM_ACTIVE_PLUS_ALL;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_PLUS_ALL - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].id        = EM_ACTIVE_PLUS_CUR_DAY;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_PLUS_CUR_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].id        = EM_ACTIVE_MINUS_CUR_DAY;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_MINUS_CUR_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].id        = EM_ACTIVE_PLUS_PREV_DAY;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_PLUS_PREV_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].id        = EM_ACTIVE_MINUS_PREV_DAY;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_MINUS_PREV_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].id        = EM_ACTIVE_PLUS_CUR_MONTH;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_PLUS_CUR_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].id        = EM_ACTIVE_MINUS_CUR_MONTH;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_MINUS_CUR_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].id        = EM_ACTIVE_PLUS_PREV_MONTH;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_PLUS_PREV_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].id        = EM_ACTIVE_MINUS_PREV_MONTH;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].physic    = PHYSIC_ACTIVE_POWER;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_ACTIVE_MINUS_PREV_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].id        = EM_REACTIVE_ENERGY;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].physic    = PHYSIC_REACTIVE_ENERGY;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_ENERGY - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].id        = EM_REACTIVE_PLUS_ALL;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_PLUS_ALL - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].id        = EM_REACTIVE_PLUS_CUR_DAY;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_PLUS_CUR_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].id        = EM_REACTIVE_MINUS_CUR_DAY;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_MINUS_CUR_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].id        = EM_REACTIVE_PLUS_PREV_DAY;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_PLUS_PREV_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].id        = EM_REACTIVE_MINUS_PREV_DAY;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_MINUS_PREV_DAY - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].id        = EM_REACTIVE_PLUS_CUR_MONTH;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_PLUS_CUR_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].id        = EM_REACTIVE_MINUS_CUR_MONTH;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_MINUS_CUR_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].id        = EM_REACTIVE_PLUS_PREV_MONTH;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_PLUS_PREV_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].id        = EM_REACTIVE_MINUS_PREV_MONTH;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].operation = OPERATION_READ;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].physic    = PHYSIC_REACTIVE_POWER;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].value.float_t     = 0.0;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].min       = 0.0;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].max       = 100000.0;
  parameters_[EM_REACTIVE_MINUS_PREV_MONTH - EM_BEGIN].def       = 0.0;

  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].id        = EM_COEFFICIENT_TRANS_CURRENT;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].operation = OPERATION_WRITE;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].value.float_t     = 1.0;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].min       = 0.0;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].max       = 100000.0;
  parameters_[EM_COEFFICIENT_TRANS_CURRENT - EM_BEGIN].def       = 1.0;

  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].id        = EM_COEFFICIENT_TRANS_VOLTAGE;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].access    = ACCESS_OPERATOR;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].operation = OPERATION_WRITE;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].physic    = PHYSIC_NUMERIC;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].validity  = VALIDITY_ERROR;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].value.float_t     = 1.0;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].min       = 0.0;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].max       = 100000.0;
  parameters_[EM_COEFFICIENT_TRANS_VOLTAGE - EM_BEGIN].def       = 1.0;
}

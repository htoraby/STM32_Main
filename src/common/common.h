#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

enum StatusType {
  StatusOk,
  StatusError,
};

#ifdef __cplusplus

/*!
 \brief Метод подсчёта контрольной суммы

 \param buf  - указатель на данные
 \param size - размер данных
 \param crc - начальное значение контрольной суммы (по умолчанию: 0xFFFF)
 \return uint16_t - контрольная сумма
*/
uint16_t crc16_ibm(uint8_t *buf, uint8_t size, uint16_t crc = 0xFFFF);

#endif

#endif // COMMON_H

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

enum StatusType {
  StatusOk,
  StatusError,
};

#ifdef __cplusplus

/*!
 * \brief Переопределение оператора new
 *
 * \param size - размер необходимой памяти
 * \return void * - указатель на выделенную область памяти
 */
void *operator new(size_t size);

/*!
 * \brief Переопределение оператора delete
 *
 * \param p - указатель на объект
 */
void operator delete(void *p);

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

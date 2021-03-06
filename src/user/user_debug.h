#ifndef USER_DEBUG_H
#define USER_DEBUG_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

 /*!
 * \brief Получение регистров из стека
 * \param pulFaultStackAddress
 */
void getRegistersFromStack(uint32_t *pulFaultStackAddress);

/*!
 * \brief Проверка регистров полученных из стека
 */
void checkRegistersFromStack();

/*!
 * \brief Проверка стетчиков перезапуска СPU
 */
void checkRcauseCounters();

void calcIrqError(uint8_t type, uint8_t state);
void resetIrqError();
void checkIrqError();

#ifdef __cplusplus
}
#endif

#endif // USER_DEBUG_H
